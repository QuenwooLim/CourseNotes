--------------------------------------------------------------------------
-- Copyright (C) 1992, 1993 by the Board of Trustees of 		  
-- Leland Stanford Junior University.					  
--									  
-- This description is provided to serve as an example of the use	  
-- of the Murphi description language and verifier, and as a benchmark	  
-- example for other verification efforts.				  
--									  
-- License to use, copy, modify, sell and/or distribute this description  
-- and its documentation any purpose is hereby granted without royalty,   
-- subject to the following terms and conditions, provided		  
--									  
-- 1.  The above copyright notice and this permission notice must	  
-- appear in all copies of this description.				  
-- 									  
-- 2.  The Murphi group at Stanford University must be acknowledged	  
-- in any publication describing work that makes use of this example. 	  
-- 									  
-- Nobody vouches for the accuracy or usefulness of this description	  
-- for any purpose.							  
--------------------------------------------------------------------------

--------------------------------------------------------------------------
--
--                                                                        
-- File:        eadash.m                                                  
--                                                                        
-- Content:     elementary abstract Dash Protocol                         
--              with elementary operations only                           
--		with symmetry     
--                                                                        
-- Specification decision:                                                
--          1)  Each cluster is modeled by a cache and a RAC              
--              to store outstanding operations.                          
--          2)  Simplification:                                           
--              RAC is not used to stored data; the cluster               
--              acts as a single processor/global shared cache            
--          3)  Seperate network channels are used.                       
--              (request and reply)                                       
--          4)  Aliases are used extensively.                             
--                                                                        
-- Summary of result:                                                     
--          1)  No bug is discovered for the final version                
--              of the protocol.                                          
--          2)  Details of result can be found at the end of this file.   
--                                                                        
-- Options:                                                               
--          An option flag 'nohome' is used to switch on/off the local    
--          memory action.  This enables us to simplify the protocol to   
--          examine the behaviour when the number of processor increases. 
--                                                                        
-- References: 						       	       	  
--          1) Daniel Lenoski, James Laudon, Kourosh Gharachorloo, 	  
--             Wlof-Dietrich Weber, Anoop Gupta, John Hennessy, 	  
--             Mark Horowitz and Monica Lam.				  
--             The Stanford DASH Multiprocessor.			  
--             Computer, Vol 25 No 3, p.63-79, March 1992.		  
--             (available online)					  
--          2) Daniel Lenoski, 						  
--             DASH Prototype System,					  
--             PhD thesis, Stanford University, Chapter 3, 1992		  
--             (available online)					  
--                                                                        
-- Last Modified:        17 Feb 93                                        
--                                                                        
--------------------------------------------------------------------------

/*
Declarations

The number of clusters is defined by 'ProcCount'.  To simplify the
description, only some of the clusters have memory ('HomeCount').
The number of clusters without memory is given by 'RemoteCount'.

The directory used by the protocol is a full mapped directory.
Instead of using bit map as in the real implementation, array of
cluster ID is used to simply the rules used to manipulate the
directory.

Array is used to model every individual cluster-to-cluster network
channel. The size of the array is estimated to be or larger than the
max possible number of message in network.

All the addresses are in the cacheable address space.
*/

--------------------------------------------------
-- Constant Declarations
--------------------------------------------------
Const
  HomeCount:    1;              -- number of homes.
  RemoteCount:  4;              -- number of remote nodes.
  ProcCount:    HomeCount+RemoteCount;
                                -- number of processors with cache.
  AddressCount: 1;              -- number of address at each home cluster.
  ValueCount:   2;              -- number of data values.
                                -- $2^k$ where k = size of cache line
  DirMax:       ProcCount-1;    -- number of directory entries
                                -- that can be kept -- full map
  ChanMax:      ProcCount*HomeCount*AddressCount;
                                -- buffer size in a single channel

  -- options
  nohome:       true;           -- options to switch off processors at Home.
                                -- to simplify the protocol.

--------------------------------------------------
-- Type Declarations
--------------------------------------------------
Type
  --  The scalarset is used for symmetry, which is implemented in Murphi 1.5
  --  and not upgraded to Murphi 2.0 yet
  Home:    Scalarset (HomeCount);
  Remote:  Scalarset (RemoteCount);
  Proc:    Union {Home, Remote};
  Address: Scalarset (AddressCount);
  Value:   Scalarset (ValueCount);
  -- Home:      0..HomeCount-1;
  -- Proc:      0..ProcCount-1;
  DirIndex:  0..DirMax-1;
  NodeCount: 0..ProcCount-1;
  -- Address:   0..AddressCount-1;
  -- Value:     0..ValueCount-1;

  -- the type of requests that go into the request network
  -- Cacheable and DMA operations
  RequestType:
    enum{
        RD_H,    -- basic operation -- read request to Home (RD)
        RD_RAC,  -- basic operation -- read request to Remote (RD)
        RDX_H,   -- basic operation -- read exclusive request to Home (RDX)
        RDX_RAC, -- basic operation -- read exclusive request to Remote (RDX)
        INV,     -- basic operation -- invalidate request
        WB,      -- basic operation -- Explicit Write-back
        SHWB,    -- basic operation -- sharing writeback request
        DXFER    -- basic operation -- dirty transfer request
        };

  -- the type of reply that go into the reply network
  -- Cacheable and DMA operations
  ReplyType:
    enum{
        ACK,   -- basic operation       -- read reply
               -- basic operation       -- read exclusive reply (inv count = 0)
        NAK,   -- ANY kind of operation -- negative acknowledge
        IACK,  -- basic operation       -- read exclusive reply (inv count !=0)
        SACK   -- basic operation       -- invalidate acknowledge
               -- basic operation       -- dirty transfer acknowledge
        };

  -- struct of the requests in the network
  Request:
    Record
      Mtype: RequestType;
      Aux:   Proc;
      Addr:  Address;
      Value: Value;
    End;

  -- struct of the reply in the network
  Reply:
    Record
      Mtype:    ReplyType;
      Aux:      Proc;
      Addr:     Address;
      InvCount: NodeCount;
      Value:    Value;
    End;

  -- States in the Remote Access Cache (RAC) :
  -- a) maintaining the state of currently outstanding requests,
  -- b) buffering replies from the network
  -- c) supplementing the functionality of the processors' caches
  RAC_State:
  enum{
        INVAL,   -- Invalid
        WRD,     -- basic operation -- waiting for a read reply
        WRDO,    -- basic operation -- waiting for a read reply
                                    -- with ownership transfer
        WRDX,    -- basic operation -- waiting for a read exclusive reply
        WINV,    -- basic operation -- waiting for invalidate acknowledges
        RRD      -- basic operation -- invalidated read/read
                                    -- with ownership request
        };

  -- State of data in the cache
  CacheState:
    enum{
        Non_Locally_Cached,
        Locally_Shared,
        Locally_Exmod
        };


Type
  -- Directory Controller and the Memory
  -- a) Directory DRAM
  -- b) forward local requests to remotes, Reply to remote requests
  -- c) Respond to MPBUS with directory information
  -- d) storage of locks and lock queues
  HomeState:
    Record
      Mem: Array[Address] of Value;
      Dir: Array[Address] of
             Record
               State:        enum{Uncached, Shared_Remote, Dirty_Remote};
               SharedCount:  0..DirMax;
               Entries:      Array[DirIndex] of Proc;
             End;
    End;

  -- 1. Snoopy Caches
  -- 2. Pseudo-CPU (PCPU)
  --    a) Forward remote CPU requests to local MPBUS
  --    b) issue cache line invalidations and lock grants
  -- 3. Reply Controller (RC)
  --    a) Remote Access Cache (RAC) stores state of
  --       on-going memory requests and remote replies.
  --    b) Per processor invalidation counters (not implemented)
  --    c) RAC snoops on bus
  ProcState:
    Record
      Cache: Array[Home] of Array[Address] of
               Record
                 State: CacheState;
                 Value: Value;
               End;
      RAC:   Array[Home] of Array[Address] of
               Record
                 State:     RAC_State;
                 Value:     Value;
                 InvCount:  NodeCount;
               End;
    End;

--------------------------------------------------
-- Variable Declarations
--
-- Clusters 0..HomeCount-1 :  Clusters with distributed memory
-- Clusters HomeCount..ProcCount-1 : Simplified Clusters without memory.
-- ReqNet : Virtual network with cluster-to-cluster channels
-- ReplyNet : Virtual network with cluster-to-cluster channels
--------------------------------------------------
Var
  ReqNet:   Array[Proc] of Array[Proc] of
              Record
                Count:    0..ChanMax;
                Messages: Array[0..ChanMax-1] of Request;
              End;
  ReplyNet: Array[Proc] of Array[Proc] of
              Record
                Count:    0..ChanMax;
                Messages: Array[0..ChanMax-1] of Reply;
              End;
  Procs:    Array[Proc] of ProcState;
  Homes:    Array[Home] of HomeState;

/*
Procedures
-- Directory handling functions
-- Request Network handling functions
-- Reply Network handling functions
-- Sending request
-- Sending Reply
*/
--------------------------------------------------
-- Directory handling functions
-- a) set first entry in directory and clear all other
-- b) add node to directory if it does not already exist
--------------------------------------------------
Procedure Set_Dir_1st_Entry( h : Home;
                             a : Address;
                             n : Proc);
Begin
  Undefine Homes[h].Dir[a].Entries;
  Homes[h].Dir[a].Entries[0] := n;
End;

Procedure Add_to_Dir_Entries( h : Home;
                              a : Address;
                              n : Proc);
Begin
  Alias
    SharedCount: Homes[h].Dir[a].SharedCount
  Do
    If ( Forall i:0..DirMax-1 Do
           ( i < SharedCount )
           ->
           ( Homes[h].Dir[a].Entries[i] != n )
         End )
    Then
      Homes[h].Dir[a].Entries[SharedCount] := n;
      SharedCount := SharedCount + 1;
    End;
  End;
End;

--------------------------------------------------
-- Request Network handling functions
-- a) A request is put into the end of a specific channel connecting
--    the source Src and the destination Dst.
-- b) Request is only consumed at the head of the queue, forming
--    a FIFO ordered network channel.
--------------------------------------------------
Procedure Send_Req( t : RequestType;
                    Dst, Src, Aux : Proc;
                    Addr : Address;
                    Val : Value );
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := t;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    ReqNet[Src][Dst].Messages[Count].Value := Val;
    Count := Count + 1;
  End;
End;

Procedure Consume_Request( Src, Dst: Proc);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    For i:=0 to Count-2 Do
      ReqNet[Src][Dst].Messages[i] := ReqNet[Src][Dst].Messages[i+1];
    End;
    Undefine ReqNet[Src][Dst].Messages[Count-1];
    Count := Count - 1;
  End;
End;

--------------------------------------------------
-- Reply Network handling functions
-- a) A Reply is put into the end of a specific channel connecting
--    the source Src and the destination Dst.
-- b) Reply is only consumed at the head of the queue, forming
--    a FIFO ordered network channel.
--------------------------------------------------
Procedure Send_Reply( t : ReplyType;
                      Dst, Src, Aux : Proc;
                      Addr : Address;
                      Val : Value;
                      InvCount : NodeCount );
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[Count].Mtype := t;
    ReplyNet[Src][Dst].Messages[Count].Aux := Aux;
    ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
    ReplyNet[Src][Dst].Messages[Count].Value := Val;
    ReplyNet[Src][Dst].Messages[Count].InvCount := InvCount;
    Count := Count + 1;
  End;
End;

Procedure Consume_Reply( Src, Dst : Proc);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    For i:=0 to Count-2 Do
      ReplyNet[Src][Dst].Messages[i] := ReplyNet[Src][Dst].Messages[i+1];
    End;
    Undefine ReplyNet[Src][Dst].Messages[Count-1];
    Count := Count - 1;
  End;
End;

--------------------------------------------------
-- Sending request
--------------------------------------------------
-- send read request to home cluster
Procedure Send_R_Req_H( Dst, Src : Proc;
                        Addr : Address);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := RD_H;
    Undefine ReqNet[Src][Dst].Messages[Count].Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReqNet[Src][Dst].Messages[Count].Value;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(RD_H, Dst, Src, Don'tCare, Addr, Don'tCare);
End;

-- send read request to dirty remote block
--      Aux = where the request originally is from
Procedure Send_R_Req_RAC( Dst, Src, Aux : Proc;
                          Addr : Address);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := RD_RAC;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReqNet[Src][Dst].Messages[Count].Value;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(RD_RAC, Dst, Src, Aux, Addr, Don'tCare);
End;

-- send sharing writeback to home cluster
--      Aux = new sharing cluster
Procedure Send_SH_WB_Req( Dst, Src, Aux : Proc;
                          Addr : Address;
                          Val : Value);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := SHWB;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    ReqNet[Src][Dst].Messages[Count].Value := Val;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(SHWB, Dst, Src, Aux, Addr, Val);
End;

-- send invalidate request to shared remote clusters
--      Aux = where the request originally is from
Procedure Send_Inv_Req( Dst, Src, Aux : Proc;
                        Addr : Address);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := INV;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReqNet[Src][Dst].Messages[Count].Value;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(INV, Dst, Src, Aux, Addr, Don'tCare);
End;

-- send read exclusive request
Procedure Send_R_Ex_Req_RAC( Dst, Src, Aux : Proc;
                             Addr : Address);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := RDX_RAC;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReqNet[Src][Dst].Messages[Count].Value;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(RDX_RAC, Dst, Src, Aux, Addr, Don'tCare);
End;

-- send read exclusive local request
Procedure Send_R_Ex_Req_H( Dst, Src : Proc;
                           Addr : Address);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := RDX_H;
    Undefine ReqNet[Src][Dst].Messages[Count].Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReqNet[Src][Dst].Messages[Count].Value;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(RDX_H, Dst, Src, Don'tCare, Addr, Don'tCare);
End;

-- send dirty transfer request to home cluster
Procedure Send_Dirty_Transfer_Req( Dst, Src, Aux : Proc;
                                   Addr : Address);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := DXFER;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReqNet[Src][Dst].Messages[Count].Value;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(DXFER, Dst, Src, Aux, Addr, Don'tCare);
End;

-- Explicit writeback request
Procedure Send_WB_Req( Dst, Src : Proc;
                       Addr : Address;
                       Val : Value);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := WB;
    Undefine ReqNet[Src][Dst].Messages[Count].Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    ReqNet[Src][Dst].Messages[Count].Value := Val;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(WB, Dst, Src, Don'tCare, Addr, Val);
End;

--------------------------------------------------
-- Sending Reply
--------------------------------------------------
-- send read reply
--      Aux = home cluster
Procedure Send_R_Reply( Dst, Src, Home : Proc;
                        Addr : Address;
                        Val : Value);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[Count].Mtype := ACK;
    ReplyNet[Src][Dst].Messages[Count].Aux := Home;
    ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
    ReplyNet[Src][Dst].Messages[Count].Value := Val;
    ReplyNet[Src][Dst].Messages[Count].InvCount := 0;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Reply(ACK, Dst, Src, Home, Addr, Val, 0);
End;

-- send negative ack to requesting cluster
Procedure Send_NAK( Dst, Src, Aux : Proc;
                    Addr : Address);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[Count].Mtype := NAK;
    ReplyNet[Src][Dst].Messages[Count].Aux := Aux;
    ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReplyNet[Src][Dst].Messages[Count].Value;
    ReplyNet[Src][Dst].Messages[Count].InvCount := 0;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Reply(NAK, Dst, Src, Aux, Addr, Don'tCare, 0);
End;

-- send invalidate acknowledge from shared remote clusters
--      Aux = where the request originally is from
Procedure Send_Inv_Ack( Dst, Src, Aux : Proc;
                        Addr : Address);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[Count].Mtype := SACK;
    ReplyNet[Src][Dst].Messages[Count].Aux := Aux;
    ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReplyNet[Src][Dst].Messages[Count].Value;
    ReplyNet[Src][Dst].Messages[Count].InvCount := 0;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Reply(SACK, Dst, Src, Aux, Addr, Don'tCare, 0);
End;

-- send read exclusive remote reply to requesting cluster
--      Aux = where the request originally is from
Procedure Send_R_Ex_Reply( Dst, Src, Aux : Proc;
                           Addr : Address;
                           Val : Value;
                           InvCount : NodeCount);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    If ( InvCount = 0 ) Then
      ReplyNet[Src][Dst].Messages[Count].Mtype := ACK;
      ReplyNet[Src][Dst].Messages[Count].Aux := Aux;
      ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
      ReplyNet[Src][Dst].Messages[Count].Value := Val;
      ReplyNet[Src][Dst].Messages[Count].InvCount := 0;
      Count := Count + 1;
      --  style to be used with don't care passing
      --    Send_Reply(ACK, Dst, Src, Aux, Addr, Val, 0);
    Else
      ReplyNet[Src][Dst].Messages[Count].Mtype := IACK;
      ReplyNet[Src][Dst].Messages[Count].Aux := Aux;
      ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
      ReplyNet[Src][Dst].Messages[Count].Value := Val;
      ReplyNet[Src][Dst].Messages[Count].InvCount := InvCount;
      Count := Count + 1;
      --  style to be used with don't care passing
      --  Send_Reply(IACK, Dst, Src, Aux, Addr, Val, InvCount);
    End; --if;
  End; --alias
End;

-- send dirty transfer ack to new master
Procedure Send_Dirty_Transfer_Ack( Dst, Src : Proc;
                                   Addr : Address);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[Count].Mtype := SACK;
    ReplyNet[Src][Dst].Messages[Count].Aux := Src;
    ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReplyNet[Src][Dst].Messages[Count].Value;
    ReplyNet[Src][Dst].Messages[Count].InvCount := 0;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Reply(SACK, Dst, Src, Src, Addr, Don'tCare, 0);
End;


/*
Rule Sets for fundamental memory access
1) CPU Ia   : basic home memory requests from CPU
2) CPU IIa  : basic remote memory requests from CPU
3) PCPU Ia  : handling basic requests to PCPU at memory cluster
4) PCPU IIa : handling basic requests to PCPU in remote cluster
5) RCPU Ia  : handling basic replies to RCPU in any cluster.
*/

/*
CPU Ia

The rule set indeterministically issue requests for local
cacheable memory. The requests include read, exclusive read.

Two sets of Rules;
  Rule "Local Memory Read Request"
  Rule "Local Memory Read Exclusive Request"

Issue messages:
         RD_RAC
         RDX_RAC
         INV
*/

Ruleset n : Proc Do
Ruleset h : Home Do
Ruleset a : Address Do
Alias
  RAC : Procs[n].RAC[h][a];
  Cache : Procs[n].Cache[h][a];
  Dir : Homes[h].Dir[a];
  Mem : Homes[h].Mem[a]
Do

  --------------------------------------------------
  -- Home CPU issue read request
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "Local Memory Read Request"
    (h = n) & !nohome
  ==>
  Begin
    Switch RAC.State
    Case INVAL:
      -- no pending event
      Switch Dir.State
      Case Dirty_Remote:
        -- send request to master cluster
        RAC.State := WRDO;
        Send_R_Req_RAC(Dir.Entries[0],h,h,a);
      Else
        -- get from memory
        Switch Cache.State
        Case Locally_Exmod:
          -- write back local master through snoopy protocol
          Cache.State := Locally_Shared;
          Mem := Cache.Value;
        Case Locally_Shared:
          -- other cache supply data
        Case Non_Locally_Cached:
          -- update cache
          Cache.State := Locally_Shared;
          Cache.Value := Mem;
        End; --switch;
      End; --switch;

    Case WRDO:
      -- merge
    Else
      -- WINV, WRDX, WRD, RRD
      Assert ( RAC.State != WRD
             & RAC.State != RRD ) "Funny RAC state at home cluster";
    End; --switch;
  End; -- rule -- Local Memory Read Request


  --------------------------------------------------
  -- Home CPU issue read exclusive request
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "Local Memory Read Exclusive Request"
    ( h = n ) & !nohome
  ==>
  Begin
    Switch RAC.State
    Case INVAL:
      -- no pending event
      Switch Dir.State
      Case Uncached:
        -- get from memory
        -- invalidate local copy through snoopy protocol
        Cache.State := Locally_Exmod;
        Cache.Value := Mem;
      Case Shared_Remote:
        -- get from memory
        Cache.State := Locally_Exmod;
        Cache.Value := Mem;
        -- invalidate all remote shared read copies
        RAC.State := WINV;
        RAC.InvCount := Dir.SharedCount;
        For i : NodeCount Do
          If ( i < RAC.InvCount ) Then
            Send_Inv_Req(Dir.Entries[i],h,h,a);
          End;
        End;
        Dir.State := Uncached;
        Dir.SharedCount := 0;
        Undefine Dir.Entries;
      Case Dirty_Remote:
        -- send request to master cluster
        -- (switch requesting processor to do other jobs)
        RAC.State := WRDX;
        Send_R_Ex_Req_RAC(Dir.Entries[0],h,h,a);
      End; --switch;

    Case WINV:
      -- other local processor already get the dirty copy
      -- other Cache supply data
      Assert ( Dir.State = Uncached ) "Inconsistent Directory";

    Case WRDX: -- merge
      Switch Dir.State
      Case Uncached:
        -- only arise in case of:
        -- remote cluster WB
      Case Shared_Remote:
        -- only arise in case of:
        -- remote cluster WB and RD
      Case Dirty_Remote:
        -- merge
      End; --switch;

    Case WRDO:
      -- conflict
    Else
      -- WRD, RRD
      Error "Funny RAC state at home cluster";
    End; --switch;
  End; -- rule -- local memory read exclusive request

End; --alias; -- RAC, Cache, Dir, Mem
End; --ruleset; -- a
End; --ruleset; -- h
End; --ruleset; -- n

/*
CPU IIa

The rule set indeterministically issue requests for remote
cacheable memory.  The requests include read, exclusive read,
Explicit write back.

Three sets of rules:
  Rule "Remote Memory Read Request"
  Rule "Remote Memory Read Exclusive Request"
  Rule "Explicit Writeback request"

Issue messages:
         RD_H
         RDX_H
         WB
*/

Ruleset n : Proc Do
Ruleset h : Home Do
Ruleset a : Address Do
Alias
  RAC : Procs[n].RAC[h][a];
  Cache : Procs[n].Cache[h][a]
Do

  --------------------------------------------------
  -- remote CPU issue read request
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "Remote Memory Read Request"
    ( h != n )
  ==>
  Begin
    Switch RAC.State
    Case INVAL:
      -- no pending event
      Switch Cache.State
      Case Non_Locally_Cached:
        -- send request to home cluster
        RAC.State := WRD;
        Send_R_Req_H(h,n,a);
      Else
        -- other cache supply data using snoopy protocol
      End;

    Case WINV:
      -- RAC take dirty ownership (simplified)
      Assert ( Cache.State = Locally_Exmod ) "WINV with Exmod not asserted";
    Case WRD:
      -- merge
      Assert ( Cache.State = Non_Locally_Cached ) "WRD with data Locally_Cached";
    Case WRDX:
      -- conflict
      Assert ( Cache.State != Locally_Exmod ) "WRDX with data Locally_Exmod";
    Case RRD:
      -- conflict
      Assert ( Cache.State = Non_Locally_Cached ) "WRDX with funny cache state";
    Else -- WRDO
      Error "remote Cluster with WRDO";
    End; --switch;
  End; -- rule -- Remote Memory Read Request


  --------------------------------------------------
  -- remote CPU issue read exclusive request
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "Remote Memory Read Exclusive Request"
    ( h != n )
  ==>
  Begin
    Switch RAC.State
    Case INVAL:
      -- no pending event
      Switch Cache.State
      Case Locally_Exmod:
        -- other cache supply data
      Else
        -- send request to home cluster
        RAC.State := WRDX;
        Send_R_Ex_Req_H(h,n,a);
      End;

    Case WINV:
      -- other cache supply data
      Assert ( Cache.State = Locally_Exmod ) "WINV with Exmod not asserted";
    Case WRDX:
      -- merge
      Assert ( Cache.State != Locally_Exmod ) "WRDX with Exmod asserted";
    Case WRD:
      -- conflict
      Assert ( Cache.State != Locally_Exmod ) "WRD with Exmod asserted";
    Case RRD:
      -- conflict
      Assert ( Cache.State != Locally_Exmod ) "RRD with Exmod asserted";
    Else -- WRDO
      Error "remote cluster with WRDO";
    End; --switch;
  End; -- rule -- Remote Memory Read Exclusive Request


  --------------------------------------------------
  -- remote CPU issue explicit writeback request
  --------------------------------------------------
  Rule "Explicit Writeback request"
    ( h != n )
    & ( Cache.State = Locally_Exmod )
  ==>
  Begin
    If ( RAC.State = WINV ) Then
      -- retry later
    Else
      -- send request to home cluster
      Assert ( RAC.State = INVAL ) "Inconsistent Directory";
      Send_WB_Req(h,n,a,Cache.Value);
      Cache.State := Non_Locally_Cached;
      Undefine Cache.Value;
    End;
  End; -- rule -- Explicit Writeback request

End; --alias; -- RAC, Cache
End; --ruleset; -- a
End; --ruleset; -- h
End; --ruleset; -- n

/*
PCPU Ia

PCPU handles basic requests to Home for cacheable memory.

Five sets of rules:
  Rule "handle read request to home"
  Rule "handle read exclusive request to home"
  Rule "handle Sharing writeback request to home"
  Rule "handle dirty transfer request to home"
  Rule "handle writeback request to home"

Handle messages:
        RD_H
        RDX_H
        SHWB
        DXFER
        WB
*/

Ruleset Dst : Proc Do
Ruleset Src : Proc Do
Alias
  ReqChan : ReqNet[Src][Dst];
  Request : ReqNet[Src][Dst].Messages[0].Mtype;
  Addr : ReqNet[Src][Dst].Messages[0].Addr;
  Aux : ReqNet[Src][Dst].Messages[0].Aux
Do

  --------------------------------------------------
  -- PCPU handle Read request to home cluster
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "handle read request to home"
    ReqChan.Count > 0
    & Request = RD_H
  ==>
  Begin
  Alias
    RAC : Procs[Dst].RAC[Dst][Addr];
    Cache : Procs[Dst].Cache[Dst][Addr];
    Dir : Homes[Dst].Dir[Addr];
    Mem : Homes[Dst].Mem[Addr]
  Do
    Switch RAC.State
    Case WINV:
      -- cannot release copy
      Send_NAK(Src, Dst, Dst, Addr);
      Consume_Request(Src, Dst);
    Else
      -- INVAL, WRDO, WRDX, RRD,
      Assert ( RAC.State != WRD ) "WRD at home cluster";
      Switch Dir.State
      Case Uncached:
        -- no one has a copy. send copy to remote cluster
        If ( Cache.State = Locally_Exmod ) Then
          Cache.State := Locally_Shared;
          Mem := Cache.Value;
        End;
        Dir.State := Shared_Remote;
        Dir.SharedCount := 1;
        Set_Dir_1st_Entry(Dst, Addr, Src);
        Send_R_Reply(Src, Dst, Dst, Addr, Mem);
        Consume_Request(Src, Dst);
      Case Shared_Remote:
        -- some one has a shared copy. send copy to remote cluster
        Add_to_Dir_Entries(Dst, Addr, Src);
        Send_R_Reply(Src, Dst, Dst, Addr, Mem);
        Consume_Request(Src, Dst);
      Case Dirty_Remote:
        -- some one has a master copy. forward request to master cluster
        Send_R_Req_RAC(Dir.Entries[0], Dst, Src, Addr);
        Consume_Request(Src, Dst);
      End; --switch;
    End; --switch;
  End; -- alias : RAC, Cache, Dir, Mem
  End; -- rule -- handle read request to home


  --------------------------------------------------
  -- PCPU Read exclusive request to home cluster handling procedure
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "handle read exclusive request to home"
    ReqChan.Count > 0
    & Request = RDX_H
  ==>
  Begin
  Alias
    RAC : Procs[Dst].RAC[Dst][Addr];
    Cache : Procs[Dst].Cache[Dst][Addr];
    Dir : Homes[Dst].Dir[Addr];
    Mem : Homes[Dst].Mem[Addr]
  Do
    Switch RAC.State
    Case WINV:
      -- cannot release copy
      Assert ( Cache.State = Locally_Exmod ) "WINV with Exmod not asserted";
      Send_NAK(Src, Dst, Dst, Addr);
      Consume_Request(Src, Dst);
    Else
      -- INVAL, WRD, WRDO, WRDX, RRD.
      Assert ( RAC.State != WRD ) "WRD at home cluster";
      Switch Dir.State
      Case Uncached:
        -- no one has a copy. send copy to remote cluster
        If ( Cache.State = Locally_Exmod ) Then
          -- write back dirty copy
          Mem := Cache.Value;
        End;
        Cache.State := Non_Locally_Cached;
        Undefine Cache.Value;
        Dir.State := Dirty_Remote;
        Dir.SharedCount := 1;
        Set_Dir_1st_Entry(Dst, Addr, Src);
        Send_R_Ex_Reply(Src, Dst, Dst, Addr, Mem, 0);
        Consume_Request(Src, Dst);

      Case Shared_Remote:
        -- some one has a shared copy. send copy to remote cluster
        Cache.State := Non_Locally_Cached;
        Undefine Cache.Value;
        -- invalidate every shared copy
        For i : NodeCount Do
          If ( i < Dir.SharedCount ) Then
            Send_Inv_Req(Dir.Entries[i], Dst, Src, Addr);
          End;
        End;
        Send_R_Ex_Reply(Src, Dst, Dst, Addr, Mem, Dir.SharedCount);
        Dir.State := Dirty_Remote;
        Dir.SharedCount := 1;
        Set_Dir_1st_Entry(Dst, Addr, Src);
        Consume_Request(Src, Dst);

      Case Dirty_Remote:
        -- some one has a master copy. forward request to master cluster
        Send_R_Ex_Req_RAC(Dir.Entries[0], Dst, Src, Addr);
        Consume_Request(Src, Dst);
      End; --switch;
    End; --switch; -- RDX_H
  End; -- alias : RAC, Cache, Dir, Mem
  End; -- rule -- handle read exclusive request to home


  --------------------------------------------------
  -- PCPU sharing writeback request to home cluster handling procedure
  --------------------------------------------------
  Rule "handle Sharing writeback request to home"
    ReqChan.Count > 0
    & Request = SHWB
  ==>
  Begin
  Alias
    v : ReqNet[Src][Dst].Messages[0].Value;
    Dir : Homes[Dst].Dir[Addr];
    Mem : Homes[Dst].Mem[Addr]
  Do
    Assert ( Dir.State = Dirty_Remote ) "Writeback to non dirty remote memory";
    Assert ( Dir.Entries[0] = Src ) "Writeback by non owner";
    Mem := v;
    Dir.State := Shared_Remote;
    Add_to_Dir_Entries(Dst, Addr, Aux);
    Consume_Request(Src, Dst);
  End; -- alias : v, Dir, Mem
  End; -- rule -- handle sharing writeback to home

  --------------------------------------------------
  -- PCPU dirty transfer request to home cluster handling procedure
  --------------------------------------------------
  Rule "handle dirty transfer request to home"
    ReqChan.Count > 0
    & Request = DXFER
  ==>
  Begin
  Alias
    Dir : Homes[Dst].Dir[Addr]
  Do
    Assert ( Dir.State = Dirty_Remote ) "Dirty transfer for non dirty remote memory";
    Assert ( Dir.Entries[0] = Src ) "Dirty transfer by non owner";
    Set_Dir_1st_Entry(Dst, Addr, Aux);
    Send_Dirty_Transfer_Ack(Aux, Dst, Addr);
    Consume_Request(Src, Dst);
  End; -- alias : Dir
  End; -- rule -- handle dirty transfer to home

  --------------------------------------------------
  -- PCPU writeback request to home cluster handling procedure
  --------------------------------------------------
  Rule "handle writeback request to home"
    ReqChan.Count > 0
    & Request = WB
  ==>
  Begin
  Alias
    v : ReqNet[Src][Dst].Messages[0].Value;
    Dir : Homes[Dst].Dir[Addr];
    Mem : Homes[Dst].Mem[Addr]
  Do
    Assert ( Dir.State = Dirty_Remote ) "Explicit writeback for non dirty remote";
    Assert ( Dir.Entries[0] = Src ) "Explicit writeback by non owner";
    Mem := v;
    Dir.State := Uncached;
    Dir.SharedCount := 0;
    Undefine Dir.Entries;
    Consume_Request(Src, Dst);
  End; -- alias : v, Dir, Mem
  End; -- rule -- handle writeback

End; --alias; -- ReqChan, Request, Addr, Aux
End; --ruleset; -- Src
End; --ruleset; -- Dst

/*
PCPU IIa

PCPU handles basic requests to non-home for cacheable memory.

Three sets of rules:
  Rule "handle read request to remote cluster"
  Rule "handle Invalidate request to remote cluster"
  Rule "handle read exclusive request to remote cluster"

Handle Messages:
        RD_RAC
        INV
        RDX_RAC
*/

Ruleset Dst: Proc Do
Ruleset Src: Proc Do
Alias
  ReqChan: ReqNet[Src][Dst];
  Request: ReqNet[Src][Dst].Messages[0].Mtype;
  Addr: ReqNet[Src][Dst].Messages[0].Addr;
  Aux: ReqNet[Src][Dst].Messages[0].Aux
Do

  --------------------------------------------------
  -- PCPU read request to remote cluster handling procedure
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  -- Case DRDX: -- ambiguious in their table
  --------------------------------------------------
  Rule "handle read request to remote cluster"
    ReqChan.Count > 0
    & Request = RD_RAC
  ==>
  Begin
  Alias
    RAC: Procs[Dst].RAC[Src][Addr];
    Cache: Procs[Dst].Cache[Src][Addr]
  Do
    Switch RAC.State
    Case WINV:
      -- cannot release copy
      Assert ( Cache.State = Locally_Exmod ) "WINV with Exmod not asserted.";
      Send_NAK(Aux, Dst, Src, Addr);
      Consume_Request(Src, Dst);

    Else
      -- INVAL, WRDO, WRD, WRDX, RRD.
      Assert ( RAC.State != WRDO ) "WRDO at remote cluster";
      Switch Cache.State
      Case Locally_Exmod:
        -- has master copy; sharing write back data
        Cache.State := Locally_Shared;
        If ( Src = Aux ) Then
          -- read req from home cluster
          Send_R_Reply(Aux, Dst, Src, Addr, Cache.Value);
        Else
          -- read req from local cluster
          Send_R_Reply(Aux, Dst, Src, Addr,  Cache.Value);
          Send_SH_WB_Req(Src, Dst, Aux, Addr, Cache.Value);
        End;
        Consume_Request(Src, Dst);
      Else
        -- cannot release
        -- possible situration is :
        -- WRDX => still waiting for reply
        -- i.e. request message received before reply
        Send_NAK(Aux, Dst, Src, Addr);
        Consume_Request(Src, Dst);
      End; --switch;
    End; --switch;
  End; -- alias : RAC, Cache
  End; -- rule -- handle read request to remote cluster


  --------------------------------------------------
  -- PCPU invalidate request handling procedure
  --------------------------------------------------
  Rule "handle Invalidate request to remote cluster"
    ReqChan.Count > 0
    & Request = INV
  ==>
  Begin
  Alias
    RAC: Procs[Dst].RAC[Src][Addr];
    Cache: Procs[Dst].Cache[Src][Addr]
  Do
    Assert ( Dst != Src ) "Invalidation to Local Memory";
    If ( Dst = Aux )
    Then
      --------------------------------------------------
      -- PCPU invalidate request to initiating cluster handling procedure
      --------------------------------------------------
      If ( Cache.State = Locally_Shared ) Then
        Cache.State := Non_Locally_Cached;
        Undefine Cache.Value;
      End;
      If ( RAC.State = WINV )Then
        -- have to wait for 1 less invalidation
        RAC.InvCount := RAC.InvCount -1;
      Else
        -- invalidation acknowledge come back before reply
        -- keep a count of how many acks so far
        RAC.InvCount := RAC.InvCount +1;
      End;
      If ( RAC.InvCount = 0
         & RAC.State = WINV )
      Then
        -- collected all the acknowledgements
        Undefine RAC;
	RAC.State := INVAL;
	RAC.InvCount := 0;
      End;
      Consume_Request(Src, Dst);

    Else
      --------------------------------------------------
      -- PCPU invalidate request handling procedure
      -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
      --------------------------------------------------
      Switch RAC.State
      Case WINV:
        Error "invalidation cannot be for this copy!";
      Case WRD:
        RAC.State := RRD;
      Else
        -- INVAL, WRDX, WRDO, RRD.
        Assert ( RAC.State != RRD
               & RAC.State != WRDO ) "Inconsistent RAC with invalidation";
        Switch Cache.State
        Case Non_Locally_Cached:
          -- already flushed out of the cache
          -- result: not model in this description
          Error "checking if we model flushing";
        Case Locally_Shared:
          -- invalidate cache
          Cache.State := Non_Locally_Cached;
          Undefine Cache.Value;
        Case Locally_Exmod:
          Error "Invalidate request to master remote block.";
        End;
      End;
      Send_Inv_Ack(Aux, Dst, Src, Addr);
      Consume_Request(Src, Dst);
    End; -- if
  End; -- alias : RAC, Cache
  End; -- rule -- handle invalidate request


  --------------------------------------------------
  -- PCPU Read exclusive request to remote cluster handling procedure
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------`
  Rule "handle read exclusive request to remote cluster"
    ReqChan.Count > 0
    & Request = RDX_RAC
  ==>
  Begin
  Alias
    RAC: Procs[Dst].RAC[Src][Addr];
    Cache: Procs[Dst].Cache[Src][Addr]
  Do
    Switch RAC.State
    Case WINV:
      -- cannot release copy
      Assert ( Cache.State = Locally_Exmod ) "WINV with Exmod not asserted.";
      Send_NAK(Aux, Dst, Src, Addr);
      Consume_Request(Src, Dst);
    Else
      -- INVAL, WRDO, WRD, WRDX, RRD.
      Assert ( RAC.State !=  WRDO ) "WRDO in remote cluster";
      Switch Cache.State
      Case Locally_Exmod:
        -- has master copy; dirty transfer data
        If ( Src = Aux ) Then
          -- request from home cluster
          Send_R_Ex_Reply(Aux, Dst, Src, Addr, Cache.Value, 0);
        Else
          -- request from remote cluster
          Send_R_Ex_Reply(Aux, Dst, Src, Addr, Cache.Value, 1);
          Send_Dirty_Transfer_Req(Src, Dst, Aux, Addr);
        End;
        Cache.State := Non_Locally_Cached;
        Undefine Cache.Value;
        Consume_Request(Src, Dst);
      Else
        -- cannot release
        -- possible situration is :
        -- WRDX => still waiting for reply
        -- i.e. request message received before reply
        Send_NAK(Aux, Dst, Src, Addr);
        Consume_Request(Src, Dst);
      End; --switch;
    End; --switch;
  End; -- alias : RAc, Cache
  End; -- rule -- handle read exclusive request to remote cluster

End; --alias; -- ReqChan, Request, Addr, Aux
End; --ruleset; -- Src
End; --ruleset; -- Dst


/*
RCPU Ia

RCPU handles cacheable acknowledgements and replies.

Four sets of rules:
  Rule "handle Acknowledgement"
  Rule "handle negative Acknowledgement"
  Rule "handle Indirect Acknowledgement"
  Rule "handle Supplementary Acknowledgement"

Handle messages:
   ACK
   NAK
   IACK
   SACK

-- confirmed with table net_up.tbl
-- except simplified in handling NAK on WDMAW
*/

Ruleset Dst : Proc Do
Ruleset Src : Proc Do
Alias
  ReplyChan : ReplyNet[Src][Dst];
  Reply : ReplyNet[Src][Dst].Messages[0].Mtype;
  Addr : ReplyNet[Src][Dst].Messages[0].Addr;
  Aux : ReplyNet[Src][Dst].Messages[0].Aux;
  v : ReplyNet[Src][Dst].Messages[0].Value;
  ICount : ReplyNet[Src][Dst].Messages[0].InvCount
Do

  Rule "handle Acknowledgement"
    ReplyChan.Count > 0
    & Reply = ACK
    -- basic operation       -- read reply
    -- basic operation       -- read exclusive reply (inv count = 0)
  ==>
  Begin
  Alias
    RAC : Procs[Dst].RAC[Aux][Addr];
    Cache : Procs[Dst].Cache[Aux][Addr]
  Do
    Switch RAC.State
    Case INVAL:
      -- no pending event
      Error "ACK in INVAL RAC state";
    Case WRD:
      -- pending read , i.e. read reply
      Cache.State := Locally_Shared;
      Cache.Value := v;
      Undefine RAC;
      RAC.State := INVAL;
      RAC.InvCount := 0;
      Consume_Reply(Src, Dst);
    Case WRDO:
      -- pending read , i.e. read reply
      Cache.State := Locally_Shared;
      Cache.Value := v;
      Homes[Dst].Mem[Addr] := v;
      Undefine RAC;
      RAC.State := INVAL;
      RAC.InvCount := 0;
      Consume_Reply(Src, Dst);
    Case WRDX:
      -- pending exclusive read, i.e. exclusive read reply
      -- no invalidation is required
      Cache.State := Locally_Exmod;
      Cache.Value := v;
      If ( Dst = Aux )
      Then
        Alias
          Dir : Homes[Dst].Dir[Addr]
        Do
          -- getting master copy back in home cluster
          -- no shared copy in the network
          Dir.State := Uncached;
          Dir.SharedCount := 0;
          Undefine Dir.Entries;
        End; -- alias : Dir
      End;
      Undefine RAC;
      RAC.State := INVAL;
      RAC.InvCount := 0;
      Consume_Reply(Src, Dst);
    Case RRD:
      -- invalidated pending event, ignore reply
      Undefine RAC;
      RAC.State := INVAL;
      RAC.InvCount := 0;
      Consume_Reply(Src, Dst);
    Else -- WINV
      Error "ACK in funny RAC state";
    End; --switch;
  End; -- alias : RAC, Cache, Dir, Mem
  End; -- rule -- handle ACK


  Rule "handle negative Acknowledgement"
    ReplyChan.Count > 0
    & Reply = NAK
    -- ANY kind of operation -- negative acknowledge
  ==>
  Begin
  Alias
    RAC : Procs[Dst].RAC[Aux][Addr]
  Do
    Switch RAC.State
      Case INVAL:
        Error "NAK in INVAL RAC state";
      Case WRD:
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
        Consume_Reply(Src, Dst);
      Case WRDO:
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
        Consume_Reply(Src, Dst);
      Case WRDX:
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
        Consume_Reply(Src, Dst);
      Case RRD:
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
        Consume_Reply(Src, Dst);
      Else
        -- WINV
        Error "NAK in funny RAC state";
    End; --switch;
  End; -- alias : RAC
  End; -- rule -- handle NAK


  Rule "handle Indirect Acknowledgement"
    ReplyChan.Count > 0
    & Reply = IACK
    -- basic operation       -- read exclusive reply (inv count !=0)
  ==>
  Begin
  Alias
    RAC : Procs[Dst].RAC[Aux][Addr];
    Cache : Procs[Dst].Cache[Aux][Addr]
  Do
    Switch RAC.State
    Case INVAL:
      -- no pending event
      Error "Read exclusive Reply in INVAL RAC state";
    Case WRDX:
      -- pending exclusive read, i.e. exclusive read reply
      -- require invalidation
      Cache.State := Locally_Exmod;
      Cache.Value := v;
      If ( Dst = Aux )
      Then
        Alias
          Dir : Homes[Dst].Dir[Addr]
        Do
          -- getting master copy back in home cluster
          -- never arises in this description
          Error "already sent invalidations to copy ??";
          Dir.State := Uncached;
          Dir.SharedCount := 0;
          Undefine Dir.Entries;
        End; -- alias : Dir
      End;
      RAC.InvCount := ICount - RAC.InvCount;
      RAC.State := WINV;
      If ( RAC.InvCount = 0 ) Then
        -- all invalidation acks received
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
      End;
      Consume_Reply(Src, Dst);
    Else
      -- WRD, WRDO, RRD, WINV
      Error "Read exclusive reply in funny RAC state.";
    End; --switch;
  End; -- alias : RAC, Cache, Dir
  End; -- rule -- IACK


  Rule "handle Supplementary Acknowledgement"
    ReplyChan.Count > 0
    & Reply = SACK
    -- basic operation       -- invalidate acknowledge
    -- basic operation       -- dirty transfer acknowledge
  ==>
  Begin
  Alias
    RAC : Procs[Dst].RAC[Aux][Addr]
  Do
    -- Inv_Ack, Dirty_Transfer_Ack.
    Switch RAC.State
    Case INVAL:
      -- no pending event
      Error "Invalidate acknowledge in INVAL RAC state";
    Case WINV:
      -- get invalidation acknowledgements
      RAC.InvCount := RAC.InvCount -1;
      If  ( RAC.InvCount = 0 ) Then
        -- get all invalidation acks
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
      End;
      Consume_Reply(Src, Dst);
    Case WRDX:
      -- get invalidation acknowledgement before reply
      RAC.InvCount := RAC.InvCount +1;
      Consume_Reply(Src, Dst);
    Else
      -- WRD, WRDO, RRD.
      Error "Invalidate acknowledge in funny RAC state.";
    End; --switch;
  End; -- alias : RAC
  End; -- rule -- SACK

End; --alias; -- ReplyChan, Reply, Addr, Aux, v, ICount
End; --ruleset; -- Src
End; --ruleset; -- Dst

/*
-- rule for indeterministically change the master copy
*/
Ruleset v : Value Do
Ruleset h : Home Do
Ruleset n : Proc Do
Ruleset a : Address Do

  Rule "modifying value at cache"
    Procs[n].Cache[h][a].State = Locally_Exmod
  ==>
  Begin
    Procs[n].Cache[h][a].Value := v;
  End;

End; --ruleset; -- a
End; --ruleset; -- n
End; --ruleset; -- h
End; --ruleset; -- v

/*
Start state
*/
Ruleset v: Value Do

  Startstate
  Begin
    For h : Home Do
    For a : Address Do
      Homes[h].Dir[a].State := Uncached;
      Homes[h].Dir[a].SharedCount := 0;
      Homes[h].Mem[a] := v;
      Undefine Homes[h].Dir[a].Entries;
    End;
    End;

    For n : Proc Do
    For h : Home do
    For a : Address Do
      Procs[n].Cache[h][a].State := Non_Locally_Cached;
      Procs[n].RAC[h][a].State := INVAL;
      Undefine Procs[n].Cache[h][a].Value;
      Undefine Procs[n].RAC[h][a].Value;
      Procs[n].RAC[h][a].InvCount := 0;
    End;
    End;
    End;

    For Src : Proc Do
    For Dst : Proc Do
      ReqNet[Src][Dst].Count := 0;
      Undefine ReqNet[Src][Dst].Messages;
      ReplyNet[Src][Dst].Count := 0;
      Undefine ReplyNet[Src][Dst].Messages;
    End;
    End;
  End; -- startstate

End; -- ruleset -- v

/*
Invariant "Globally invalid RAC state at Home Cluster"
Invariant "Globally invalid RAC state at Local Cluster"
Invariant "Only a single master copy exist"
Invariant "Irrelevant data is set to zero"
Invariant "Consistency within Directory"
Invariant "Condition for existance of master copy of data"
Invariant "Consistency of data"
Invariant "Adequate invalidations with Read Exclusive request"
*/

Invariant "Globally invalid RAC state at Home Cluster"
  Forall n : Proc Do
  Forall h : Home Do
  Forall a : Address Do
    ( h != n )
    |
    ( ( Procs[n].RAC[h][a].State != WRD
      & Procs[n].RAC[h][a].State != RRD ) )
  End
  End
  End; -- globally invalid RAC state at Home Cluster

Invariant "Gobally invalid RAC state at Local Cluster"
  Forall n : Proc Do
  Forall h : Home Do
  Forall a : Address Do
    ( h = n )
    |
    ( Procs[n].RAC[h][a].State != WRDO )
  End
  End
  End; -- globally invalid RAC state at Local Cluster

Invariant "Only a single master copy exist"
  Forall n1 : Proc Do
  Forall n2 : Proc Do
  Forall h : Home Do
  Forall a : Address Do
   ! ( n1 != n2
     & Procs[n1].Cache[h][a].State = Locally_Exmod
     & Procs[n2].Cache[h][a].State = Locally_Exmod )
  End
  End
  End
  End; -- only a single master copy exist

Invariant "Irrelevant data is set to zero"
  Forall n : Proc Do
  Forall h : Home Do
  Forall a : Address Do
    ( Homes[h].Dir[a].State != Uncached
    | Homes[h].Dir[a].SharedCount = 0 )
    &
    ( Forall i:0..DirMax-1 Do
        ( i >= Homes[h].Dir[a].SharedCount )
        ->
        ( Isundefined(Homes[h].Dir[a].Entries[i]) )
      End ) 
    &
    ( ( Procs[n].Cache[h][a].State = Non_Locally_Cached )
      ->
      ( Isundefined(Procs[n].Cache[h][a].Value) ) )
    &
    ( ( Procs[n].RAC[h][a].State = INVAL )
      ->
      ( Isundefined(Procs[n].RAC[h][a].Value)
      & ( Procs[n].RAC[h][a].InvCount = 0 ) ) )
  End
  End
  End; -- Irrelevant data is set to zero

Invariant "Consistency within Directory"
  Forall h : Home Do
  Forall a : Address Do
    ( Homes[h].Dir[a].State = Uncached
    & Homes[h].Dir[a].SharedCount = 0 )
    |
    ( Homes[h].Dir[a].State = Dirty_Remote
    & Homes[h].Dir[a].SharedCount = 1 )
    |
    ( Homes[h].Dir[a].State = Shared_Remote
    & Homes[h].Dir[a].SharedCount != 0
    & Forall i : DirIndex Do
      Forall j : DirIndex Do
        ( i != j
        & i < Homes[h].Dir[a].SharedCount
        & j < Homes[h].Dir[a].SharedCount )
        ->
        ( Homes[h].Dir[a].Entries[i] != Homes[h].Dir[a].Entries[j] )
      End
      End )
  End
  End; -- Consistency within Directory

Invariant "Condition for existance of master copy of data"
  Forall n : Proc Do
  Forall h : Home Do
  Forall a : Address Do
    ( Procs[n].Cache[h][a].State != Locally_Exmod
    | Procs[n].RAC[h][a].State = INVAL
    | Procs[n].RAC[h][a].State = WINV )
  End
  End
  End; -- Condition for existance of master copy of data

Invariant "Consistency of data"
  Forall n : Proc Do
  Forall h : Home Do
  Forall a : Address Do
    ! ( Procs[n].Cache[h][a].State = Locally_Shared
      & Procs[n].Cache[h][a].Value != Homes[h].Mem[a]
      & Homes[h].Dir[a].State != Dirty_Remote
      & ! ( Exists i : 0..ChanMax-1 Do
              ( i < ReqNet[h][n].Count
              & ReqNet[h][n].Messages[i].Mtype = INV )
            End )
      & ! ( Exists i:0..ChanMax-1 Do
              ( i < ReqNet[n][h].Count
              & ReqNet[n][h].Messages[i].Mtype = SHWB )
            End
            |
            Exists m : Proc Do
            Exists i : 0..ChanMax-1 Do
              ( i < ReqNet[m][h].Count
              & ReqNet[m][h].Messages[i].Aux = n
              & ReqNet[m][h].Messages[i].Mtype = SHWB)
            End
            End )
      & ! ( Exists i:0..ChanMax-1 Do
              ( i < ReplyNet[n][h].Count
              & ReplyNet[n][h].Messages[i].Mtype = ACK )
            End
            |
            Exists m:Proc Do
            Exists i:0..ChanMax-1 Do
              ( i < ReplyNet[n][h].Count
              & ReplyNet[m][h].Messages[i].Aux = n
              & ReplyNet[m][h].Messages[i].Mtype = ACK )
            End
            End ) )
  End
  End
  End; -- Consistency of data

Invariant "Adequate invalidations with Read Exclusive request"
  Forall n1 : Proc Do
  Forall n2 : Proc Do
  Forall h : Home Do
  Forall a : Address Do
    ( n1 = n2 )
    |
    !( ( Procs[n1].RAC[h][a].State = WINV )
       &
       ( Procs[n2].Cache[h][a].State = Locally_Shared )
       &
       ( ! Exists i : 0..ChanMax-1 Do
             ( i < ReqNet[h][n2].Count
             & ReqNet[h][n2].Messages[i].Mtype = INV )
           End ) )
  End
  End
  End
  End; -- Adequate invalidations with Read Exclusive request


/******************

Summary of Result (using release 2.3):

) 1 cluster with memory and 1 remote clusters

   breath-first search
   133bits (17bytes) per state
   402 states with a max of about 74 states in queue
   2514 rules fired
   3.7s in sun sparc 2 station

) 1 cluster with memory and 2 remote clusters

   breath-first search
   477bits (60bytes) per state
   24980 states with a max of about 2325 states in queue
   237904 rules fired
   875.32s in sun sparc 2 station

) 1 no-processor cluster with memory and 1 remote clusters

   breath-first search
   133bits (17bytes) per state
   38 states with a max of about 9 states in queue
   132 rules fired
   0.6s in sun sparc 2 station

) 1 no-processor cluster with memory and 2 remote clusters

   breath-first search
   477bits (60bytes) per state
   1694 states with a max of about 208 states in queue
   11712 rules fired
   48.14s in sun sparc 2 station

) 1 no-processor cluster with memory and 3 remote clusters

   breath-first search
   1103 bits (138 bytes) per state
   91,254 states with a max of about 7,700 states in queue
   930,414 rules fired
   6,971.29s in sun sparc 2 station

2.73S

* 1 home (no-processor) and 3 remote clusters with 2 data values
        (sparc 2 station)
	* The size of each state is 1568 bits (rounded up to 196 bytes).

	BFS -sym1
	7741 states, 78887 rules fired in 927.96s.
	629 max in queue.

	BFS -nosym
	91254 states, 930414 rules fired in 4413.00s.
	7723 max in queue. 

	1 data values
	* The size of each state is 1431 bits (rounded up to 179 bytes).
	-sym1
	4575 states, 44664 rules fired in 515.24s.
	428 max in queue.

	-nosym
	26925 states, 262986 rules fired in 1216.91s.
	2648 max in queue.

	3 data values
	* The size of each state is 1568 bits (rounded up to 196 bytes).
	8251 states, 87042 rules fired in 1555.60s.
	632 max in queue.

	4 data values
	* The size of each state is 1705 bits (rounded up to 214 bytes).
	8276 states, 90249 rules fired in 3510.73s.
	632 max in queue.

	5 data values
	* The size of each state is 1705 bits (rounded up to 214 bytes).
	8276 states, 93196 rules fired in 11214.96s.
	632 max in queue.

Release 2.9S (Sparc 20, cabbage.stanford.edu)

   4 remote clusters 2 values

        * The size of each state is 9656 bits (rounded up to 1208 bytes).
      -b* The size of each state is 2979 bits (rounded up to 376 bytes).

         just over 150MBytes
         stoped at 129000 states explored in 8463.29s,
         with 1707962 rules fired and 1470 states in the queue.
      -b 133498 states, 1786129 rules fired in 18071.32s.
      -c 133484 states, 1785955 rules fired in 10077.22s.




******************/
