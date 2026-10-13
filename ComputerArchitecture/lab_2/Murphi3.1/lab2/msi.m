----------------------------------------------------------------------
-- Simple Snooping MSI Protocol (3 caches, 1 memory block)
----------------------------------------------------------------------

const
  NUM_CACHE : 3;
  NUM_MEM   : 1;

type
  StateType : enum {M, S, I};
  CacheId   : scalarset(NUM_CACHE);
  CountType : 0..NUM_CACHE;

  CacheLine : record
    state : StateType;
    data  : 0..15;
  end;

  Cache : record
    line : CacheLine;
  end;

  MemBlock : record
    data : 0..15;
  end;

  BusReqType : enum {BusNone, BusRd, BusRdX};
  BusCtrlType : enum {CtrlNone, CtrlFlush};
  Bus : record
    req     : BusReqType;
    source  : CacheId;
    data    : -1..15;
    ctrl    : BusCtrlType;
  end;

var
  caches : array[CacheId] of Cache;
  mem    : MemBlock;
  bus    : Bus;

function CountModifiedCache(): CountType;
var cnt: CountType;
begin
  cnt := 0;
  for i: CacheId do
    if (caches[i].line.state = M) then
      cnt := cnt + 1;
    end;
  end;
  return cnt;
end;

----------------------------------------------------------------------
-- Bus Snooping
----------------------------------------------------------------------
procedure Snoop(i: CacheId);
begin
  if bus.req = BusRd then
    switch caches[i].line.state
      case M:
        -- M状态收到BusRd：提供数据并转为S状态，同时写回内存
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := S;
        -- 写回内存以保持一致性
        mem.data := caches[i].line.data;

      case S:
        -- S状态收到BusRd：保持S状态，无操作
        -- 多个S状态的缓存可以共存

      case I:
        -- I状态收到BusRd：无操作

    endswitch;
  elsif bus.req = BusRdX then
    switch caches[i].line.state
      case M:
        -- M状态收到BusRdX：提供数据并转为I状态，同时写回内存
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := I;
        -- 写回内存以保持一致性
        mem.data := caches[i].line.data;

      case S:
        -- S状态收到BusRdX：转为I状态
        caches[i].line.state := I;

      case I:
        -- I状态收到BusRdX：无操作

    endswitch;
  endif;
end;

----------------------------------------------------------------------
-- Processor Read Request (PrRd)
----------------------------------------------------------------------
ruleset i : CacheId do

  rule "PrRd, Cache State M"
    (caches[i].line.state = M) &
    (bus.req = BusNone)
  ==>
  begin
    -- M状态读命中，无需任何操作
    -- 数据已经在本地缓存中，且是最新的
  end;

  rule "PrRd, Cache State S"
    (caches[i].line.state = S) &
    (bus.req = BusNone)
  ==>
  begin
    -- S状态读命中，无需任何操作
    -- 数据已经在本地缓存中
  end;

  rule "PrRd, Cache State I"
    (caches[i].line.state = I) &
    (bus.req = BusNone)
  ==>
  begin
    -- I状态读缺失，发起BusRd请求
    bus.req := BusRd;
    bus.source := i;
    bus.data := -1;
    bus.ctrl := CtrlNone;
  end;

end;

----------------------------------------------------------------------
-- Processor Write Request (PrWr)
----------------------------------------------------------------------
ruleset i : CacheId do
  rule "PrWr, Cache State M"
    (caches[i].line.state = M) &
    (bus.req = BusNone)
  ==>
  begin
    -- simulate a write operation
    caches[i].line.data := (caches[i].line.data + 1)%16;
  end;

  rule "PrWr, Cache State S"
    (caches[i].line.state = S) &
    (bus.req = BusNone)
  ==>
  begin
    -- S状态写缺失，发起BusRdX请求获取独占权
    bus.req := BusRdX;
    bus.source := i;
    bus.data := -1;
    bus.ctrl := CtrlNone;
  end;

  rule "PrWr, Cache State I"
    (caches[i].line.state = I) &
    (bus.req = BusNone)
  ==>
  begin
    -- I状态写缺失，发起BusRdX请求
    bus.req := BusRdX;
    bus.source := i;
    bus.data := -1;
    bus.ctrl := CtrlNone;
  end;

end;

----------------------------------------------------------------------
-- Process Bus Transaction
----------------------------------------------------------------------
rule "Process bus transaction"
  bus.req != BusNone
  ==>
  begin
    -- Cache Responds to Bus Transaction
    for i : CacheId do
      if i != bus.source then
        Snoop(i);
      endif;
    endfor;

    -- 内存响应总线事务
    if bus.ctrl = CtrlNone then
      -- 没有缓存提供数据，由内存响应
      if bus.req = BusRd then
        -- BusRd：从内存读取数据到请求缓存，状态变为S
        caches[bus.source].line.data := mem.data;
        caches[bus.source].line.state := S;
      elsif bus.req = BusRdX then
        -- BusRdX：从内存读取数据到请求缓存，状态变为M
        caches[bus.source].line.data := mem.data;
        caches[bus.source].line.state := M;
      endif;
    else
      -- 有缓存提供数据（bus.ctrl = CtrlFlush）
      if bus.req = BusRd then
        -- BusRd：获取数据，状态变为S
        caches[bus.source].line.data := bus.data;
        caches[bus.source].line.state := S;
        -- 内存也应该更新为最新数据
        mem.data := bus.data;
      elsif bus.req = BusRdX then
        -- BusRdX：获取数据并获得独占权，状态变为M
        caches[bus.source].line.data := bus.data;
        caches[bus.source].line.state := M;
        -- 内存更新为最新数据
        mem.data := bus.data;
      endif;
    endif;

    bus.req := BusNone;
    undefine bus.source;
    bus.data := -1;
    bus.ctrl := CtrlNone;

  end;

----------------------------------------------------------------------
-- Initialization
----------------------------------------------------------------------
startstate "Init"
  for i : CacheId do
    caches[i].line.state := I;
    undefine caches[i].line.data;
  end;

  mem.data := 0;

  bus.req := BusNone;
  undefine bus.source;
  bus.data := -1;
  bus.ctrl := CtrlNone;

end;

----------------------------------------------------------------------
-- Invariants
----------------------------------------------------------------------
invariant "Only one Cache may be Modified"
  CountModifiedCache() <= 1;

invariant "All Shared Caches have same data"
  forall i : CacheId do
    forall j : CacheId do
        ((caches[i].line.state = S) &
         (caches[j].line.state = S))
        ->
        (caches[i].line.data = caches[j].line.data)
    end
  end;

invariant "Modified Cache implies no Shared Cache"
  ( CountModifiedCache() = 1 )
  ->
  forall i : CacheId do
    caches[i].line.state != S
  end;

invariant "Shared Caches equal memory (when no Modified cache exists)"
  ( CountModifiedCache() = 0 )
  -> 
  forall i : CacheId do
    (caches[i].line.state = S) -> (caches[i].line.data = mem.data)
  end;