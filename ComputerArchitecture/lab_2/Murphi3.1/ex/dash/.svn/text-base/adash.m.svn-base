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
-- Author:    C. Norris Ip                                                
--                                                                        
-- File:        adash.m                                                   
--                                                                        
-- Content:     abstract Dash Protocol                                    
--              with elementary operations and extended DMA operations    
--              suitable for conversion to use scalarset declarations     
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
--          1)  A "non-obvious" bug is rediscovered.                      
--          2)  No bug is discovered for the final version                
--              of the protocol.                                          
--          3)  Details of result can be found at the end of this file.   
--                                                                        
-- Options:                                                               
--          The code for the bug is retained in the description.          
--          To see the result of using a erronous protocol,               
--          use option flag 'bug1'.                                       
--                                                                        
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
channel. The size of the array is estimated to be larger than the
max possible number of message in network.

All the addresses are in the cacheable address space.  
*/

--------------------------------------------------
-- Constant Declarations
--------------------------------------------------
Const
  HomeCount:    1;              -- number of homes.
  RemoteCount:  2;              -- number of remote nodes.
  ProcCount:    HomeCount+RemoteCount;
                                -- number of processors with cache.
  AddressCount: 1;              -- number of address at each home cluster.
  ValueCount:   2;              -- number of data values.
                                -- $2^k$ where k = size of cache line
  DirMax:       ProcCount-1;    -- number of directory entries
                                -- that can be kept -- full map
  ChanMax:      2*ProcCount*AddressCount*HomeCount;
                                -- buffer size in a single channel

  -- options
  bug1:         false;           -- options to introduce the bug described or not
  nohome:       true;           -- options to switch off processors at Home.
                                -- to simplify the protocol.

--------------------------------------------------
-- Type Declarations
--------------------------------------------------
Type
  --  The scalarset is used for symmetry, which is implemented in Murphi 1.5
  --  and not upgraded to Murphi 2.0 yet
  --  Proc:    Scalarset {Home HomeCount} {NonHome RemoteCount};
  --  Address: Scalarset {Add AddressCount};
  --  Value:   Scalarset {Val ValueCount};
  Home:      Scalarset(HomeCount);
  Remote:    Scalarset(RemoteCount);
  Proc:      Union{Home, Remote};
  DirIndex:  0..DirMax-1;
  NodeCount: 0..ProcCount-1;
  Address:   Scalarset(AddressCount);
  Value:     Scalarset(ValueCount);

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
        DXFER,   -- basic operation -- dirty transfer request
        DRD_H,   -- DMA operation   -- read request to Home (DRD)
        DRD_RAC, -- DMA operation   -- read request to Remote (DRD)
        DWR_H,   -- DMA operation   -- write request to Home (DWR)
        DWR_RAC, -- DMA operation   -- write request to Remote (DWR)
        DUP      -- DMA operation   -- update request
        };

  -- the type of reply that go into the reply network
  -- Cacheable and DMA operations
  ReplyType:
    enum{
        ACK,   -- basic operation       -- read reply
               -- basic operation       -- read exclusive reply (inv count = 0)
               -- DMA operation         -- read reply
               -- DMA operation         -- write acknowledge
        NAK,   -- ANY kind of operation -- negative acknowledge
        IACK,  -- basic operation       -- read exclusive reply (inv count !=0)
               -- DMA operation         -- acknowledge with update count
        SACK   -- basic operation       -- invalidate acknowledge
               -- basic operation       -- dirty transfer acknowledge
               -- DMA operation         -- update acknowledge
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
        RRD,     -- basic operation -- invalidated read/read
                                    -- with ownership request
        WDMAR,   -- DMA operation   -- waiting for a DMA read reply
        WUP,     -- DMA operation   -- waiting for update acknowledges
        WDMAW    -- DMA operation   -- waiting for a DMA write acknowledge
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
                 State:    RAC_State;
                 Value:    Value;
                 InvCount: NodeCount;
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

-- parameter that may be used for symmetry reduction heuristic
-- Parameter
--   Ndepth: 3;

/*
Procedures
-- Directory handling functions
-- Request Network handling functions
-- Reply Network handling functions
-- Sending request
-- Sending Reply
-- Sending DMA Request
-- Sending DMA Reply
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
    For i: 0..ChanMax -2 Do
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
    For i: 0..ChanMax -2 Do
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

--------------------------------------------------
-- Sending DMA Request
--------------------------------------------------
-- DMA Read request to home
Procedure Send_DMA_R_Req_H( Dst, Src : Proc;
                            Addr : Address);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := DRD_H;
    Undefine ReqNet[Src][Dst].Messages[Count].Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReqNet[Src][Dst].Messages[Count].Value;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(DRD_H, Dst, Src, Don'tCare, Addr, Don'tCare);
End;

-- DMA Read request to remote
Procedure Send_DMA_R_Req_RAC( Dst, Src, Aux : Proc;
                              Addr : Address);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := DRD_RAC;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReqNet[Src][Dst].Messages[Count].Value;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(DRD_RAC, Dst, Src, Aux, Addr, Don'tCare);
End;

-- send DMA write request from Local
Procedure Send_DMA_W_Req_H( Dst, Src : Proc;
                            Addr : Address;
                            Val : Value);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := DWR_H;
    Undefine ReqNet[Src][Dst].Messages[Count].Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    ReqNet[Src][Dst].Messages[Count].Value := Val;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(DWR_H, Dst, Src, Don'tCare, Addr, Val);
End;

-- send DMA update count to local
Procedure Send_DMA_Update_Count( Dst, Src : Proc;
                                 Addr : Address;
                                 SharedCount : NodeCount);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[Count].Mtype := IACK;
    ReplyNet[Src][Dst].Messages[Count].Aux := Src; --**
    ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReplyNet[Src][Dst].Messages[Count].Value;
    ReplyNet[Src][Dst].Messages[Count].InvCount := SharedCount;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Reply(IACK, Dst, Src, Src, Addr, Don'tCare, SharedCount);
End;

-- send DMA update request from Local
Procedure Send_DMA_Update_Req( Dst, Src, Aux : Proc;
                               Addr : Address;
                               Val : Value);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := DUP;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    ReqNet[Src][Dst].Messages[Count].Value := Val;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(DUP, Dst, Src, Aux, Addr, Val);
End;

-- send DMA update request from home
-- forward DMA update request from home
Procedure Send_DMA_W_Req_RAC( Dst, Src, Aux : Proc;
                              Addr : Address;
                              Val : Value);
Begin
  Alias
    Count : ReqNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[Count].Mtype := DWR_RAC;
    ReqNet[Src][Dst].Messages[Count].Aux := Aux;
    ReqNet[Src][Dst].Messages[Count].Addr := Addr;
    ReqNet[Src][Dst].Messages[Count].Value := Val;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Req(DWR_RAC, Dst, Src, Aux, Addr, Val);
End;

--------------------------------------------------
-- Sending DMA Reply
--------------------------------------------------

-- DMA read reply
Procedure Send_DMA_R_Reply( Dst, Src, Aux : Proc;
                            Addr : Address;
                            Val : Value);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[Count].Mtype := ACK;
    ReplyNet[Src][Dst].Messages[Count].Aux := Aux;
    ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
    ReplyNet[Src][Dst].Messages[Count].Value := Val;
    ReplyNet[Src][Dst].Messages[Count].InvCount := 0;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Reply(ACK, Dst, Src, Aux, Addr, Val, 0);
End;

-- send DMA write Acknowledge
Procedure Send_DMA_W_Ack( Dst, Src, Aux : Proc;
                          Addr : Address);
Begin
  Alias
    Count : ReplyNet[Src][Dst].Count
  Do
    Assert ( Count != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[Count].Mtype := ACK;
    ReplyNet[Src][Dst].Messages[Count].Aux := Aux;
    ReplyNet[Src][Dst].Messages[Count].Addr := Addr;
    Undefine ReplyNet[Src][Dst].Messages[Count].Value;
    ReplyNet[Src][Dst].Messages[Count].InvCount := 0;
    Count := Count + 1;
  End;
  --  style to be used with don't care passing
  --  Send_Reply(ACK, Dst, Src, Aux, Addr, Don'tCare, 0);
End;

-- DMA update acknowledge to local
Procedure Send_DMA_Update_Ack( Dst, Src, Aux : Proc;
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

/*
Rule Sets for fundamental memory access and DMA access
1) CPU Ia   : basic home memory requests from CPU
   CPU Ib   : DMA home memory requests from CPU
2) CPU IIa  : basic remote memory requests from CPU
   CPU IIb  : DMA remote memory requests from CPU
3) PCPU Ia  : handling basic requests to PCPU at memory cluster
   PCPU Ib  : handling DMA requests to PCPU at memory cluster
4) PCPU IIa : handling basic requests to PCPU in remote cluster
   PCPU IIb : handling DMA requests to PCPU in remote cluster
5) RCPU Iab : handling basic and DMA replies to RCPU in any cluster.

Note: Abstracted Version
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
    ( h = n ) & !nohome
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
      -- WRD, RRD, WINV, WRDX, WDMAR, WUP, WDMAW.
      Assert ( RAC.State != WRD
             & RAC.State != RRD ) "Funny RAC state at home cluster";
      -- conflict
    End;
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
      -- WRD, RRD, WDMAR, WUP, WDMAW.
      Assert ( RAC.State != WRD
             & RAC.State != RRD ) "Funny RAC state at home cluster";
      -- conflict
    End; --switch;
  End; -- rule -- local memory read exclusive request

End; --alias; -- RAC, Cache, Dir, Mem
End; --ruleset; -- a
End; --ruleset; -- h
End; --ruleset; -- n


/*
CPU Ib

The rule set indeterministically issue requests for local
cacheable memory. The requests include DMA read and DMA write.

Two sets of rules:
  Rule "DMA Local Memory Read Request"
  Rule "DMA Local Memory Write Request"

Issue messages:
         DRD_RAC
         DWR_RAC
         DUP
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
  -- Home CPU issue DMA read request
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "DMA Local Memory Read Request"
   ( h = n ) & !nohome
  ==>
  Begin
    Switch Dir.State
    Case Uncached:
      -- if RAC = INVAL => other cache supply data
      -- else           => conflict
    Case Shared_Remote:
      -- if RAC = INVAL => other cache supply data
      -- else           => conflict
    Case Dirty_Remote:
      Switch RAC.State
      Case INVAL:
        -- send DMA read request
        RAC.State := WDMAR;
        Send_DMA_R_Req_RAC(h,n,n,a);
      Case WINV:
        Error "inconsistent directory";
      Case WDMAR:
        -- merge
      Else
        -- WRD, WRDO, WRDX, RRD, WUP, WDMAW.
        Assert ( RAC.State != WRD
               & RAC.State != RRD ) "Funny RAC state at home cluster";
        -- conflict
      End;
    End;
  End; -- rule -- DMA Local Memory Read Request


  --------------------------------------------------
  -- Home CPU issue DMA write request
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Ruleset v:Value Do
  Rule "DMA Local Memory Write Request"
    ( h = n ) & !nohome
  ==>
  Begin
    Switch RAC.State
    Case INVAL:
      -- no pending event
      Switch Dir.State
      Case Uncached:
        Switch Cache.State
        Case Non_Locally_Cached:
          -- write to memory
          Mem := v;
        Case Locally_Shared:
          -- write to memory and cache
          Cache.Value := v;
          Mem := v;
        Case Locally_Exmod:
          -- write to cache
          Cache.Value := v;
        End; --switch;

      Case Shared_Remote:
        -- shared by remote cache
        -- a) update local cache
        -- b) update memory
        -- c) update remote cache
        Switch Cache.State
        Case Non_Locally_Cached:
          -- no in local cache
        Case Locally_Shared:
          -- update local cache
          Cache.Value := v;
        Case Locally_Exmod:
          Error "Shared_remote with Exmod asserted";
        End; --switch;
        Mem := v;
        RAC.State := WUP;
        RAC.InvCount := Dir.SharedCount;
        For i:NodeCount Do
          If ( i < Dir.SharedCount ) Then
            Send_DMA_Update_Req(Dir.Entries[i], n,n,a,v);
          End; --if;
        End; --for;

      Case Dirty_Remote:
        -- update remote master copy
        RAC.State := WDMAW;
        RAC.Value := v;
        Send_DMA_W_Req_RAC(Dir.Entries[0], n,n,a,v);
      End; --switch;

    Case WINV:
      -- local master copy
      -- write local cache
      If ( Cache.State = Locally_Exmod ) Then
        Procs[n].Cache[h][a].Value := v;
      Elsif ( Cache.State = Locally_Shared ) Then
        Cache.Value := v;
        Mem := v;
      End; --if;

    Else
      -- WRD, WRDO, WRDX, RRD, WDMAR, WUP, WDMAW
      Assert ( RAC.State != WRD ) "WRD at home cluster";
    End; --switch;
  End; -- rule -- DMA write
  End; --ruleset; -- v

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
    Case WRDO:
      Error "remote Cluster with WRDO";
    Case WDMAR:
      -- conflict
      Assert ( Cache.State = Non_Locally_Cached ) "WRD with data Locally_Cached";
    Case WUP:
      -- conflict
    Case WDMAW:
      -- conflict
      Assert ( Cache.State != Locally_Exmod ) "WRDX with data Locally_Exmod";
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
    Case WRDO:
      Error "remote cluster with WRDO";
    Case WDMAR:
      -- conflict
      Assert ( Cache.State != Locally_Exmod ) "WRD with Exmod asserted";
    Case WUP:
      -- conflict
    Case WDMAW:
      -- conflict
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
      Assert ( RAC.State = INVAL
             | RAC.State = WUP ) "Inconsistent Directory";
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
CPU IIb

The rule set indeterministically issue requests for remote
cacheable memory.  The requests include DMA read, DMA write.

Two sets of rules:
  Rule "DMA Remote Memory Read Request"
  Rule "DMA Remote Memory Write Request"

Issue messages:
         DRD_H
         DWR_H
*/

Ruleset n : Proc Do
Ruleset h : Home Do
Ruleset a : Address Do
Alias
  RAC : Procs[n].RAC[h][a];
  Cache : Procs[n].Cache[h][a]
Do

  --------------------------------------------------
  -- remote CPU issue DMA read request
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "DMA Remote Memory Read Request"
    ( h != n )
  ==>
  Begin
    Switch Cache.State
    Case Non_Locally_Cached:
      Switch RAC.State
      Case INVAL:
        RAC.State := WDMAR;
        Send_DMA_R_Req_H(h,n,a);
      Case WINV:
        Error "Inconsistent RAC and Cache";
      Else -- WRD, WRDX, RRD, WRDO, WDMAR, WUP, WDMAW
        Assert ( RAC.State != WRDO ) "WRDO at remote cluster";
        -- conflict or merge
      End;
    Case Locally_Shared:
      Assert ( RAC.State != WINV
             & RAC.State != WRDO
             & RAC.State != WRD
             & RAC.State != RRD
             & RAC.State != WDMAR ) "Inconsistent directory";
    Case Locally_Exmod:
      Assert ( RAC.State != WDMAW
             & RAC.State != WRDO
             & RAC.State != WRD
             & RAC.State != WRDX
             & RAC.State != RRD
             & RAC.State != WDMAR ) "Inconsistent directory";
    End;
  End; -- rule -- DMA Remote Memory Read Request


  --------------------------------------------------
  -- remote CPU issue DMA write request
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Ruleset v : Value Do
  Rule "DMA Remote Memory Write Request"
    ( h != n )
  ==>
  Begin
    Switch RAC.State
    Case INVAL:
      Switch Cache.State
        Case Locally_Exmod:
          -- update cache
          Cache.Value := v;
        Case Locally_Shared:
          -- no update to local cache
          -- update will be requested by home cluster later.
          -- send update request
          RAC.State := WDMAW;
          RAC.Value := v;
          Send_DMA_W_Req_H(h,n,a,v);
        Case Non_Locally_Cached:
          -- send update request
          RAC.State := WDMAW;
          RAC.Value := v;
          Send_DMA_W_Req_H(h,n,a,v);
      End; --switch;

    Case WINV:
      -- write local cache
      Assert ( Cache.State = Locally_Exmod ) "WINV with Exmod not asserted";
      Cache.Value := v;

    Else
      -- WRD, WRDO, WRDX, RRD, WDMAR, WUP, WDMAW
      Assert ( RAC.State != WRDO ) "WRDO at remote cluster";
      -- conflict or merge
    End;
  End; -- rule -- DMA Remote Memory Write Request
  End; --ruleset; -- v

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
      -- INVAL, WRDO, WRDX, RRD, WDMAR, WUP, WDMAW.
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
      -- INVAL, WRD, WRDO, WRDX, RRD, WDMAR, WUP, WDMAW.
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
        if (!bug1) then
          -- ############# no bug
          -- if you want protocol with no bug, use the following:
          -- send invalidation anyway to master-copy-requesting cluster, which has
          -- already invalidated the cache
          For i : NodeCount Do
            If ( i < Dir.SharedCount ) Then
              Send_Inv_Req(Dir.Entries[i], Dst, Src, Addr);
            End;
          End;
          Send_R_Ex_Reply(Src, Dst, Dst, Addr, Mem, Dir.SharedCount);
        else
          -- ############# bug I
          -- if you want protocol with bug rediscovered by Murphi
          -- which is a subtle bug that the designer once overlook
          -- and only discovered it by substantial simulation, use
          -- the following:
          -- no invalidation to master-copy-requesting cluster, which has
          -- already invalidated the cache
          For i : DirIndex Do
            If ( i < Dir.SharedCount
               & Dir.Entries[i] != Src )
            Then
              Send_Inv_Req(Dir.Entries[i], Dst, Src, Addr);
            End;
          End;
          If ( Exists i : DirIndex Do
               ( i<Dir.SharedCount
               & Dir.Entries[i] = Src )
             End )
          Then
            Send_R_Ex_Reply(Src, Dst, Dst, Addr, Mem, Dir.SharedCount -1);
          Else
            Send_R_Ex_Reply(Src, Dst, Dst, Addr, Mem, Dir.SharedCount);
          End;
        End; -- bug or no bug

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
PCPU Ib

PCPU handles DMA requests to Home for cacheable memory.

Two sets of rules:
  Rule "handle DMA read request to home"
  Rule "handle DMA write request to home"

Handle messages:
        DRD_H
        DWR_H
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
  -- PCPU DMA read request to home cluster handling procedure
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "handle DMA read request to home"
    ReqChan.Count > 0
    & Request = DRD_H
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
    Else -- INVAL, WRD, WRDO WRDX, RRD WDMAR, WUP, WDMAW.
      Switch Dir.State
      Case Uncached:
        If ( Cache.State = Locally_Exmod ) Then
          Send_DMA_R_Reply(Src, Dst, Dst, Addr, Cache.Value);
        Else
          Send_DMA_R_Reply(Src, Dst, Dst, Addr, Mem);
        End;
        Consume_Request(Src, Dst);
      Case Shared_Remote:
        Send_DMA_R_Reply(Src, Dst, Dst, Addr, Mem);
        Consume_Request(Src, Dst);
      Case Dirty_Remote:
        Send_DMA_R_Req_RAC(Dir.Entries[0], Dst, Src, Addr);
        Consume_Request(Src, Dst);
      End;
    End;
  End; -- alias : RAC, Cache, Dir, Mem
  End; -- rule -- handle DMA read request to home


  --------------------------------------------------
  -- PCPU DMA write request to home cluster handling procedure
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "handle DMA write request to home"
    ReqChan.Count > 0
    & Request = DWR_H
  ==>
  Begin
  Alias
    v : ReqNet[Src][Dst].Messages[0].Value;
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
    Else -- INVAL, WRD, WRDO WRDX, RRD WDMAR, WUP, WDMAW.
      Switch Dir.State
      Case Uncached:
        -- update local data
        Switch Cache.State
        Case Locally_Exmod:
          Cache.Value := v;
        Case Locally_Shared:
          Cache.Value := v;
          Mem := v;
        Case Non_Locally_Cached:
          Mem := v;
        End;
        Send_DMA_W_Ack(Src, Dst, Dst, Addr);
        Consume_Request(Src, Dst);

      Case Shared_Remote:
        -- update local data and remote shared caches
        If ( Cache.State = Locally_Shared ) Then
          Cache.Value := v;
        End;
        Mem := v;
        For i:NodeCount Do
          If ( i < Dir.SharedCount ) Then
            Send_DMA_Update_Req(Dir.Entries[i], Dst, Src, Addr, v);
          End;
        End;
        Send_DMA_Update_Count(Src, Dst, Addr, Dir.SharedCount);
        Consume_Request(Src, Dst);

      Case Dirty_Remote:
        -- update remote master copy
        Send_DMA_W_Req_RAC(Dir.Entries[0], Dst, Src, Addr, v);
        Consume_Request(Src, Dst);
      End;
    End; --switch
  End; -- alias : v, RAC, Cache, Dir, Mem
  End; -- rule -- handle DMA write request to home

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
      -- INVAL, WRDO, WRD, WRDX, RRD, WDMAR, WUP, WDMAW
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
      Case RRD:
        -- nochange
      Else
        -- INVAL, WRDX, WRDO, RRD, WDMAR, WUP, WDMAW.
        Assert ( RAC.State != WRDO ) "Inconsistent RAC with invalidation";
        Switch Cache.State
        Case Non_Locally_Cached:
          -- already flushed out of the cache
          -- DMA update causes an RRD, therefore not uptodate copy in cache
          -- while home cluster still think there is a shared copy.
          -- if you want to introduce some errors, add next line
          -- Error "checking if we model flushing";
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
      -- INVAL, WRDO, WRD, WRDX, RRD, WDMAR, WUP, WDMAW.
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
  End; -- alias : RAC, Cache
  End; -- rule -- handle read exclusive request to remote cluster

End; --alias; -- ReqChan, Request, Addr, Aux
End; --ruleset; -- Src
End; --ruleset; -- Dst


/*
PCPU IIb

PCPU handles DMA requests to non-home for cacheable memory.

Three sets of rules:
  Rule "handle DMA read request to remote cluster"
  Rule "handle DMA update request to remote cluster"
  Rule "handle DMA write request to remote cluster"

Handle Messages:
        DUP
        DWR_RAC
        DRD_RAC
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
  -- PCPU DMA Read request to remote cluster handling procedure
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "handle DMA read request to remote cluster"
    ReqChan.Count > 0
    & Request = DRD_RAC
  ==>
  Begin
  Alias
    RAC: Procs[Dst].RAC[Src][Addr];
    Cache: Procs[Dst].Cache[Src][Addr]
  Do
    Switch RAC.State
    Case WINV:
      Send_NAK(Aux, Dst, Src, Addr);
      Consume_Request(Src, Dst);
    Else -- INVAL, WRD, WRDO, RRD, WRDX, WDMAR, WUP WDMAW.
      Assert ( RAC.State !=  WRDO ) "WRDO in remote cluster";
      Switch Cache.State
      Case Locally_Exmod:
        -- reply with new data
        Send_DMA_R_Reply(Aux, Dst, Src, Addr, Cache.Value);
        Consume_Request(Src, Dst);
      Else
        -- cannot release
        -- possible situration is :
        -- WRDX => still waiting for reply
        -- i.e. request message received before reply
        Send_NAK(Aux, Dst, Src, Addr);
        Consume_Request(Src, Dst);
      End;
    End;
  End; -- alias : RAC, Cache
  End; -- rule -- handle DMA read request to remote cluster


  --------------------------------------------------
  -- PCPU DMA update request handling procedure
  --------------------------------------------------
  Rule "handle DMA update request to remote cluster"
    ReqChan.Count > 0
    & Request = DUP
  ==>
  Begin
  Alias
    v: ReqNet[Src][Dst].Messages[0].Value;
    RAC: Procs[Dst].RAC[Src][Addr];
    Cache: Procs[Dst].Cache[Src][Addr]
  Do
    Switch RAC.State
    Case WINV:
      Assert ( Cache.State = Locally_Exmod ) "WINV with Exmod not asserted.";
      Cache.Value := v;
    Case WRD:
      RAC.State := RRD;
    Case RRD:
      -- no change
    Else -- INVAL, WRDO, WRDX, WDMAR, WUP WDMAW.
      Assert ( RAC.State !=  WRDO ) "WRDO in remote cluster";
      Switch Cache.State
      Case Locally_Shared:
        Cache.Value := v;
      Case Non_Locally_Cached:
        -- cache already given up data
        -- possible situration :
        -- DMA update causes an RRD, therefore not uptodate copy in cache
        -- while home cluster still think there is a shared copy.
        -- if you want to introduce some errors, add next line
        -- Error "checking if we model flushing";
      Case Locally_Exmod:
        -- possible situration is :
        -- arise if the bug I is used...otherwise not.
        -- 1) cluster 1 get shared copy
        -- 2) home try DMA update
        -- 3) cluster 1 has given up data and get a master copy
        -- Error "add possible situration IV";
        Cache.Value := v;
        -- requested updated Exclusive copy ?
      End;
    End;
    Send_DMA_Update_Ack(Aux, Dst, Src, Addr);
    Consume_Request(Src, Dst);
  End; -- alias : v, RAC, Cache
  End; -- rule -- handle DMA update request to remote cluster


  --------------------------------------------------
  -- PCPU DMA write request to remote cluster handling procedure
  -- confirmed with tables net.tbl, rc_1.tbl, rc_3.tbl up_mp.tbl
  --------------------------------------------------
  Rule "handle DMA write request to remote cluster"
    ReqChan.Count > 0
    & Request = DWR_RAC
  ==>
  Begin
  Alias
    v: ReqNet[Src][Dst].Messages[0].Value;
    RAC: Procs[Dst].RAC[Src][Addr];
    Cache: Procs[Dst].Cache[Src][Addr]
  Do
    Switch RAC.State
    Case WINV:
      -- cannot update copy
      Assert ( Cache.State = Locally_Exmod ) "WINV with Exmod not asserted.";
      Send_NAK(Aux, Dst, Src, Addr);
      Consume_Request(Src, Dst);

    Else -- INVAL, WRD, WRDO, RRD, WRDX, WDMAR, WUP WDMAW.
      Switch Cache.State
      Case Locally_Exmod:
        Cache.Value := v;
        Send_DMA_W_Ack(Aux, Dst, Src, Addr);
        Consume_Request(Src, Dst);
      Else
        -- cannot update copy
        -- possible situration is :
        -- WRDX => still waiting for reply
        -- i.e. request message received before reply
        Send_NAK(Aux, Dst, Src, Addr);
        Consume_Request(Src, Dst);
      End;
    End;
  End; -- alias : v, RAC, Cache
  End; -- rule -- handle DMA update request to remote cluster

End; --alias; -- ReqChan, Request, Addr, Aux
End; --ruleset; -- Src
End; --ruleset; -- Dst

/*
RCPU Iab

RCPU handles cacheable and DMA acknowledgements and replies.

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
    -- DMA operation         -- read reply
    -- DMA operation         -- write acknowledge
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
    Case WDMAR:
      -- pending DMA read , i.e. read reply
      -- return data to cpu
      Undefine RAC;
      RAC.State := INVAL;
      RAC.InvCount := 0;
      Consume_Reply(Src, Dst);
    Case WDMAW:
      -- pending DMA write , i.e. write acknowledge
      Undefine RAC;
      RAC.State := INVAL;
      RAC.InvCount := 0;
      Consume_Reply(Src, Dst);
    Else
     -- WINV, WUP
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
    RAC : Procs[Dst].RAC[Aux][Addr];
    Cache : Procs[Dst].Cache[Aux][Addr];
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
      Case WDMAR:
        -- invalidated request, no data returned
        -- DMA read retry later
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
        Consume_Reply(Src, Dst);
      Case WDMAW:
        -- issue DMA write on bus again
        If ( Dst = Aux ) Then
          -- home cluster issuing DMA write
          Alias
            Dir : Homes[Dst].Dir[Addr];
            Mem : Homes[Dst].Mem[Addr]
          Do
            If ( Cache.State != Non_Locally_Cached ) Then
              Cache.Value := RAC.Value;
              Undefine RAC;
              RAC.State := INVAL;
              RAC.InvCount := 0;
            End;
            Switch Dir.State
            Case Uncached:
              Mem := RAC.Value;
              Undefine RAC;
              RAC.State := INVAL;
              RAC.InvCount := 0;
            Case Shared_Remote:
              Mem := RAC.Value;
              For i:NodeCount Do
                If ( i < Dir.SharedCount ) Then
                  Send_DMA_Update_Req(Dir.Entries[i], Dst, Dst, Addr, RAC.Value);
                End;
              End;
              RAC.State := WUP;
              RAC.InvCount := Dir.SharedCount;
              Undefine RAC.Value;
              RAC.State := INVAL;
              RAC.InvCount := 0;
            Case Dirty_Remote:
              RAC.State := WDMAW;
              Send_DMA_W_Req_RAC(Dir.Entries[0], Dst, Dst, Addr, RAC.Value);
            End;
          End; -- alias : Dir, Mem
        Else -- if
          -- remote cluster issuing DMA write
          Switch Cache.State
          Case Locally_Exmod:
            Error "Cache changed to Exmod while WDMAW outstanding";
            Cache.Value := RAC.Value;
            Undefine RAC;
            RAC.State := INVAL;
            RAC.InvCount := 0;
          Else
            RAC.State := WDMAW;
            Send_DMA_W_Req_H(Aux, Dst, Addr, RAC.Value);
          End;
        End;
        Consume_Reply(Src, Dst);
      Else --switch
        -- WINV, WUP
        Error "NAK in funny RAC state";
    End; --switch;
  End; -- alias : RAC
  End; -- rule -- handle NAK


  Rule "handle Indirect Acknowledgement"
    ReplyChan.Count > 0
    & Reply = IACK
    -- basic operation       -- read exclusive reply (inv count !=0)
    -- DMA operation         -- acknowledge with update count
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
        -- getting master copy back in home cluster
        Alias
          Dir : Homes[Dst].Dir[Addr]
        Do
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
    Case WDMAW:
      RAC.State := WUP;
      RAC.InvCount := ICount - RAC.InvCount;
      If ( RAC.InvCount = 0 ) Then
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
      End;
      Consume_Reply(Src, Dst);
    Else
      -- WRD, WRDO, RRD, WINV, WDMAR, WUP.
      Error "Read exclusive reply in funny RAC state.";
    End;
  End; -- alias : RAC, Cache, Dir
  End; -- rule -- IACK


  Rule "handle Supplementary Acknowledgement"
    ReplyChan.Count > 0
    & Reply = SACK
    -- basic operation       -- invalidate acknowledge
    -- basic operation       -- dirty transfer acknowledge
    -- DMA operation         -- update acknowledge
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
    Case WUP:
      RAC.InvCount := RAC.InvCount -1 ;
      If ( RAC.InvCount = 0 ) Then
        Undefine RAC;
        RAC.State := INVAL;
        RAC.InvCount := 0;
      End;
      Consume_Reply(Src, Dst);
    Case WDMAW:
      -- why is it a plus? !!
      RAC.InvCount := RAC.InvCount +1;
      Consume_Reply(Src, Dst);
    Else
      -- WRD, WRDO, RRD, WDMAR.
      Error "Invalidate acknowledge in funny RAC state.";
    End;
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

the memory can have arbitrary value
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
        i >= Homes[h].Dir[a].SharedCount
        ->
        Isundefined(Homes[h].Dir[a].Entries[i])
      End )
    &
    ( Procs[n].Cache[h][a].State = Non_Locally_Cached
      ->
      Isundefined(Procs[n].Cache[h][a].Value)
      )
    &
    ( Procs[n].RAC[h][a].State = INVAL
      ->
      ( Isundefined(Procs[n].RAC[h][a].Value)
      & Procs[n].RAC[h][a].InvCount = 0 ) )
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
              & ReqNet[m][h].Messages[i].Mtype = SHWB
              & ReqNet[m][h].Messages[i].Aux = n)
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
              & ReplyNet[m][h].Messages[i].Mtype = ACK
              & ReplyNet[m][h].Messages[i].Aux = n)
            End
            End )
      & Procs[n].RAC[h][a].State != WDMAW
      & ! ( Exists i:0..ChanMax-1 Do
                ( i<ReqNet[h][n].Count
                & ReqNet[h][n].Messages[i].Mtype = DUP )
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

1) 1 no-processor cluster with memory and 2 remote clusters
   with bug1

   Rel2.3
   breath-first search
   Error: Invariant Consistency of data failed.
   957bits (120bytes) per state
   14825 states with a max of about 2594 states in queue
   156392 rules fired
   972.42s in sun sparc 2 station

   Rel2.72S
   breath-first search
   Invariant Consistency of data failed.
   1126bits (141bytes) per state
   14825 states, 156392 rules fired in 594.2s.
   with a max of about 2594 states in queue
   in sun sparc 2 station

   Rel2.72S 
   breath-first search
   Error: Invariant Consistency of data failed.
   1126bits (141bytes) per state
   3741 states, 39619 rules fired in 333.45s.
   with a max of about 597 states in queue
   in sun sparc 2 station
   using symmetry algorithm 5

   error trace as  
Startstate Startstate 0, v:Value_1 fired.
Rule DMA Remote Memory Write Request, n:Remote_1, h:Home_1, a:Address_1, v:Value_1 fired.
Rule Remote Memory Read Request, n:Remote_2, h:Home_1, a:Address_1 fired.
Rule handle read request to home, Dst:Home_1, Src:Remote_2 fired.
Rule handle Acknowledgement, Dst:Remote_2, Src:Home_1 fired.
Rule handle DMA write request to home, Dst:Home_1, Src:Remote_1 fired.
Rule Remote Memory Read Exclusive Request, n:Remote_2, h:Home_1, a:Address_1 fired.
Rule handle read exclusive request to home, Dst:Home_1, Src:Remote_2 fired.
Rule handle Acknowledgement, Dst:Remote_2, Src:Home_1 fired.
Rule modifying value at cache, v:Value_2, h:Home_1, n:Remote_2, a:Address_1 fired.
Rule Explicit Writeback request, n:Remote_2, h:Home_1, a:Address_1 fired.
Rule handle writeback request to home, Dst:Home_1, Src:Remote_2 fired.
Rule Remote Memory Read Request, n:Remote_2, h:Home_1, a:Address_1 fired.
Rule handle read request to home, Dst:Home_1, Src:Remote_2 fired.
Rule handle Acknowledgement, Dst:Remote_2, Src:Home_1 fired.
Rule handle DMA update request to remote cluster, Dst:Remote_2, Src:Home_1 fired.
-----

2) 1 cluster with memory and 1 remote clusters
   with bug1

   breath-first search
   Error: Invariant Consistency of data failed.
   255bits (32bytes) per state
   2444 states with a max of about 405 states in queue
   28463 rules fired
   56.91s in sun sparc 2 station

3) 1 cluster with memory and 1 remote clusters
   without bug1

   breath-first search
   255bits (32bytes) per state
   2466 states with a max of about 435 states in queue
   30202 rules fired
   62.69s in sun sparc 2 station

   Comment: It was 32,536 edges in Murphi 1.5, or 2558 states and 33,700 rules
            in Murphi 1.5s, because of the don't care values.

4) 1 no-processor cluster with memory and 1 remote clusters
   without bug1

   breath-first search
   255bits (32bytes) per state
   78 states with a max of about 19 states in queue
   492 rules fired
   1.38s in sun sparc 2 station

5) 1 no-processor cluster with memory and 2 remote clusters
   without bug1

   breath-first search
   957bits (120bytes) per state
   41848 states with a max of about 3109 states in queue
   550644 rules fired
   3439.85s in sun sparc 2 station

   1126bits (141bytes) per state
   10466 states, 137708 rules fired in 1194.18s.
   with a max of about 725 states in queue.
   using symmetry algorithm 5.

Release 2.9S (Sparc 20, cabbage.stanford.edu)

   2 remote clusters and 2 values without bug1

      bit-compacted
      * The size of each state is 1126 bits (rounded up to 144 bytes).
      10466 states, 137708 rules fired in 329.12s.
      byte-compacted
      * The size of each state is 4192 bits (rounded up to 524 bytes).
      10466 states, 137708 rules fired in 159.87s.
      byte-compacted and hash-compacted
      * The size of each state is 4192 bits (rounded up to 524 bytes).
      10466 states, 137708 rules fired in 181.31s.

   2 remote clusters and 2 values with bug1
      Invariant "Consistency of data" failed.

      bit-compacted
      * The size of each state is 1126 bits (rounded up to 144 bytes).
      3741 states, 39621 rules fired in 51.41s.
      byte-compacted
      * The size of each state is 4192 bits (rounded up to 524 bytes).
      3741 states, 39621 rules fired in 91.18s.
      byte-compacted and hash-compacted
      * The size of each state is 4192 bits (rounded up to 524 bytes).
      3741 states, 39619 rules fired in 48.18s.

******************/
