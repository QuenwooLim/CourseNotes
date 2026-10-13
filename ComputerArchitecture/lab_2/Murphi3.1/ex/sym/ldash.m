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
-- File:        ldash.m                                                   
--                                                                        
-- Content:     Concrete Dash Protocol                                    
--              with spinning locks only                                  
--		with symmetry		
--                                                                        
--              Checked with the PROM tables                              
--                                                                        
-- Specification decision:                                                
--          1)  Each cluster is modeled by a lock address cache (LCache), 
--              a lock address RAC (LRAC), and a unlock operation RAC (ULRAC)
--              to store outstanding operations.                          
--          2)  Seperate network channels are used.                       
--              (request and reply)                                       
--          3)  Aliases are used extensively.                             
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
-- Last Modified:        20 April 94
--                                                                        
--------------------------------------------------------------------------

/*
Declarations
The number of clusters is defined by 'ProcCount'.  To simplify the
description, only some of the clusters have lock memory ('HomeCount').
The number of clusters without lock memory is given by 'RemoteCount'.

The directory used by the protocol is a full mapped directory.
Instead of using bit map as in the real implementation, array of
cluster ID is used to simply the rules used to manipulate the
directory.

Array is used to model every individual cluster-to-cluster network
channel. The size of the array is estimated to be or larger than the
max possible number of message in network.

Spinning lock is operated under different address space
(lock address space) from the cacheable address.
Therefore different RAC and cache entries are used,
modelled as LRAC and LCache.

Lock Release operation is operated under IO Dir address space,
and separate RAC entries are used as in ULRAC.

only lock address space and IODir address space are used.
*/

--------------------------------------------------
-- Constant Declarations
--------------------------------------------------
Const 
  HomeCount:        1;         	 -- number of homes.
  RemoteCount:      4;		 -- number of remote nodes.
  ProcCount:        HomeCount+RemoteCount;
                        	 -- number of processors with cache.
  LockAddressCount: 1;  	 -- number of lock address at each home cluster.
  DirMax:           ProcCount; 	 -- number of directory entries
		                 -- that can be kept -- full map
  ChanMax:          3*LockAddressCount*HomeCount;
                                 -- buffer size in a single channel

  -- options
  nohome:       true;           -- options to switch off processors at Home.
                                -- to simplify the protocol.

--------------------------------------------------
-- Type Declarations
--------------------------------------------------
Type
  Home   : Scalarset (HomeCount);
  Remote : Scalarset (ProcCount-HomeCount);
  Proc   : Union {Home,Remote};
  LAddress: Scalarset(LockAddressCount);
  Dir_Index: 0..DirMax-1;

  -- the type of requests that go into the request network
  -- lock operations only
  Request_Type:
    enum{ 
        RD_L,    -- lock operation -- read request to Home 
        GSET,    -- lock operation -- grant request to grant cluster
        IOWR     -- lock operation -- unlock request to home 
        };

  -- the type of reply that go into the reply network
  -- lock operations only
  Reply_Type:
    enum{
        ACK_L,   -- lock operation -- grant acknowledge 
        LCK,     -- lock operation -- locked lock reply 
        UNLCK    -- lock operation -- unlocked lock reply
        };

  -- structure of the requests in the network
  Request:
    Record
      Mtype: Request_Type;
      Aux: Proc;
      LAddr: LAddress;
    End;

  -- structure of the reply in the network
  Reply:
    Record
      Mtype: Reply_Type;
      Aux: Proc;
      LAddr: LAddress;
    End;

  -- States in the Remote Access Cache (RAC) :
  -- a) maintaining the state of currently outstanding requests,
  -- b) buffering replies from the network 
  -- c) supplementing the functionality of the processors' caches
  RAC_State:
  enum{
        INVAL,   -- Invalid
        DLCK,    -- lock operaion -- reply locked data
        DULCK,   -- lock operaion -- reply unlocked data
        DGNTS,   -- lock operaion -- granted Set
        WLCK,    -- lock operaion -- waiting for lock reply
        RLCK,    -- lock operaion -- invalidated lock request (retry lock)
        WDIRW    -- IODir operaion -- waiting for a directory write
 		 -- IODir operaion -- (unlock) acknowledge
        };

  -- State of data in the cache
  Cache_State:
    enum{
        Non_Locally_Cached,
        Locally_Shared
        };


Type
  -- Directory Controller and the Memory
  -- a) Directory DRAM
  -- b) forward local requests to remotes, Reply to remote requests
  -- c) Respond to MPBUS with directory information
  -- d) storage of locks and lock queues
  HomeState:
    Record
      LDir: Array[LAddress] of
              Record
                State:  enum{ Unlocked, Locked, Queued }; 
                QueuedCount: Dir_Index;
                Entries: Array[Dir_Index] of Proc;
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
      LCache: Array[Home] of Array[LAddress] of
                Record
                  State: Cache_State;
                End;
      LRAC:   Array[Home] of Array[LAddress] of 
                Record
                  State: RAC_State;
                  Granting: Proc;
                End;
      ULRAC:  Array[Home] of Array[LAddress] of 
                Record
                  State: RAC_State;
                End;
    End;

--------------------------------------------------
-- Variable Declarations
--
-- Clusters 0..HomeCount-1 :  Clusters with distributed memory (Procs and Homes)
-- Clusters HomeCount..ProcCount-1 : Simplified Clusters w/o memory. (Procs only)
-- ReqNet : Virtual network with cluster-to-cluster channels
-- ReplyNet : Virtual network with cluster-to-cluster channels
--------------------------------------------------
Var
  ReqNet:    Array[Proc] of Array[Proc] of
               Record
                 Count: 0..ChanMax;
                 Messages: Array[0..ChanMax-1] of Request;
               End;
  ReplyNet:  Array[Proc] of Array[Proc] of
               Record
                 Count: 0..ChanMax;
                 Messages: Array[0..ChanMax-1] of Reply;
               End;
  Procs:     Array[Proc] of ProcState;
  Homes:     Array[Home] of HomeState;
  LockOwner: Array[Home] of Array[LAddress] of
               Record
                 State: enum{Invalid, Valid};
                 Proc: Proc;
               End;

/*
Procedures
-- Lock Directory handling functions
-- Request Network handling functions
-- Reply Network handling functions
-- Procedure Checking mutual exclusion
-- Sending Lock messages
*/

--------------------------------------------------
-- Lock Directory handling functions
-- a) remove a queued cluster from the lock queue
-- b) add a queued cluster to the lock queue
--------------------------------------------------
Procedure Remove_from_Queue( h : Proc;
		             l : LAddress;
			     Entry : Dir_Index);
Begin
  Alias
    QueuedCount: Homes[h].LDir[l].QueuedCount
  Do
    -- rearrange queue (not fifo queue)
    For i := 0 to QueuedCount-2 Do
      If ( i >= Entry ) Then        
        -- move tail entries upwards; possibly empty records.
        Homes[h].LDir[l].Entries[i] := Homes[h].LDir[l].Entries[i+1];
      Endif;
    Endfor;
    Undefine Homes[h].LDir[l].Entries[QueuedCount-1];

    -- book keeping
    QueuedCount := QueuedCount -1;
    If ( QueuedCount = 0 ) Then
      -- change from Queued state to Locked state.
      Homes[h].LDir[l].State := Locked;
    Endif;
  Endalias;
Endprocedure;

Procedure Add_to_LDir_Entries( h : Proc;
			       l : LAddress;
			       n : Proc);
Begin
  Alias
    QueuedCount: Homes[h].LDir[l].QueuedCount
  Do
    If ( Forall i: Dir_Index Do
           ( i < QueuedCount )
         ->
           ( Homes[h].LDir[l].Entries[i] != n )
         Endforall )      
    Then
      Assert ( QueuedCount < DirMax ) "directory queue overflow";
      Homes[h].LDir[l].Entries[QueuedCount] := n;
      QueuedCount := QueuedCount +1;
    Endif;
  Endalias;
Endprocedure;

--------------------------------------------------
-- Request Network handling functions
-- a) A request is put into the end of a specific channel connecting
--    the source Src and the destination Dst.
-- b) Request is only consumed at the head of the queue, forming
--    a FIFO ordered network channel.
--------------------------------------------------
Procedure Send_Req( t : Request_Type;
                    Dst, Src, Aux : Proc;
                    LAddr : LAddress );
Begin
  Alias
    -- messages enter at the end
    -- message order preserved
    NewID : ReqNet[Src][Dst].Count
  Do
    Assert ( NewID != ChanMax ) "Request Channel is full";
    ReqNet[Src][Dst].Messages[NewID].Mtype := t;
    If (Isundefined(Aux)) Then
      Undefine ReqNet[Src][Dst].Messages[NewID].Aux;
    Else
      ReqNet[Src][Dst].Messages[NewID].Aux := Aux;
    Endif;  
    ReqNet[Src][Dst].Messages[NewID].LAddr := LAddr;
    NewID := NewID + 1;
  Endalias;
Endprocedure;

Procedure Consume_Request( Src, Dst : Proc);
-- messages removed at the front
-- message order preserved
Begin
  Alias
    NumID : ReqNet[Src][Dst].Count
  Do
    For i := 0 to NumID-2 Do
      ReqNet[Src][Dst].Messages[i] := ReqNet[Src][Dst].Messages[i+1];
    Endfor;
    Undefine ReqNet[Src][Dst].Messages[NumID-1];
    NumID := NumID -1;
  Endalias;
Endprocedure;

--------------------------------------------------
-- Reply Network handling functions
-- a) A Reply is put into the end of a specific channel connecting
--    the source Src and the destination Dst.
-- b) Reply is only consumed at the head of the queue, forming
--    a FIFO ordered network channel.
--------------------------------------------------
Procedure Send_Reply( t : Reply_Type;
		      Dst, Src, Aux : Proc;
                      LAddr : LAddress );
Begin
  Alias
    -- messages enter at the end
    -- message order preserved
    NewID : ReplyNet[Src][Dst].Count
  Do
    Assert ( NewID != ChanMax ) "Reply Channel is full";
    ReplyNet[Src][Dst].Messages[NewID].Mtype := t;
    If (Isundefined(Aux)) Then
      Undefine ReplyNet[Src][Dst].Messages[NewID].Aux;
    Else      
      ReplyNet[Src][Dst].Messages[NewID].Aux := Aux;
    Endif;
    ReplyNet[Src][Dst].Messages[NewID].LAddr := LAddr;
    NewID := NewID + 1;
  Endalias;
Endprocedure;

Procedure Consume_Reply( Src, Dst : Proc);
-- messages removed at the front
-- message order preserved
Begin
  Alias
    NumID : ReplyNet[Src][Dst].Count
  Do
    For i:= 0 to NumID-2 Do
      ReplyNet[Src][Dst].Messages[i] := ReplyNet[Src][Dst].Messages[i+1];
    Endfor;
    Undefine ReplyNet[Src][Dst].Messages[NumID-1];
    NumID := NumID - 1;
  Endalias;
Endprocedure;

--------------------------------------------------
-- Procedure Checking mutual exclusion
-- rules should call this function whenever it wants to
-- enter Critical section.
--------------------------------------------------
Procedure Check_Mutual_Exclusion( h : Home;
				  l : LAddress);
Begin
  Assert ( LockOwner[h][l].State != Valid) "Mutual exclusion violated";
Endprocedure;

--------------------------------------------------
-- Sending Lock messages
--------------------------------------------------
-- Granting acknowledge
Procedure Send_ACK_L( Dst, Src, Aux : Proc;
		      l : LAddress);
Begin
  Send_Reply(ACK_L, Dst, Src, Aux, l);
Endprocedure;

-- lock read request
Procedure Send_RD_L( Dst, Src : Proc;
		     l : LAddress);
Var UNDEF: Proc;
Begin
  Undefine UNDEF;
  Send_Req(RD_L, Dst, Src, UNDEF, l);
Endprocedure;

-- grant set request
Procedure Send_Grant_SET( Dst, Src, Aux : Proc;
			  l : LAddress);
Begin
  Send_Req(GSET, Dst, Src, Aux, l);
Endprocedure;

-- unlock request
Procedure Send_Unlock( Dst, Src, Aux : Proc;
		       l : LAddress);
Begin
  Send_Req(IOWR, Dst, Src, Aux, l);
Endprocedure;

-- unlock reply
Procedure Send_Unlock_Reply( Dst, Src : Proc;
			     l : LAddress);
Var UNDEF: Proc;
Begin
  Undefine UNDEF;
  Send_Reply(UNLCK, Dst, Src, UNDEF, l );
End;

-- lock reply
Procedure Send_Lock_Reply( Dst, Src : Proc;
			   l : LAddress);
Var UNDEF: Proc;
Begin
  Undefine UNDEF;
  Send_Reply(LCK, Dst, Src, UNDEF, l);
End;

/*
Rule Sets for fundamental memory access
1) CPU III  : lock requests issued by CPU
2) CPU IV   : unlock requests issued by CPU
3) CPU V    : time-out interrupt at RAC
4) PCPU III : lock/unlock request handled by PCPU
5) PCPU IV  : lock grant request handled by PCPU
6) RCPU II  : lock/unlock reply and granting acknowledgement handled by RCPU
*/

/*
CPU III

The rule set indeterministically issue lock request 

When a CPU has not acquired a arbitrary lock, it may 
issue a lock request.  

Home lock acquire: -- current commented out for system with only remote lock.
If the lock addresss is unlocked, acquire is successful.  Otherwise
the request is queued in the directory and the state of the directory
become Queued.  If later the lock has been granted to the CPU,
acknowledgement is sented to the granting cluster.

Remote lock acquire:
Request for lock is sent to the home cluster if lock has not been
granted to the CPU.  If the cached value of the lock is locked, the
CPU spin waiting on the locked value.  Unlocked value is never cached.
If later the lock has been granted to the CPU, acknowledgement is
sented to the granting cluster.

Two sets of rules:
  Rule "Home Lock Acquire"
  Rule "Remote Lock Acquire"

Issue messages:
        ACK_L
        RD_L
*/

Ruleset n:Proc Do
Ruleset h:Home Do
Ruleset l:LAddress Do
Alias
  LRAC: Procs[n].LRAC[h][l];
  ULRAC: Procs[n].ULRAC[h][l];
  LCache: Procs[n].LCache[h][l];
  LDir: Homes[h].LDir[l]
Do

  Rule "Home Lock Acquire"
    (h=n) & !nohome
    & !( LockOwner[h][l].State = Valid
       & LockOwner[h][l].Proc = n )
  ==> 
  Begin
    Switch LRAC.State
    Case INVAL:
      Switch LDir.State
      Case Unlocked:
        Switch LCache.State -- acquire successful
        Case Non_Locally_Cached:
          Check_Mutual_Exclusion(h,l);
          LockOwner[h][l].State := Valid;
          LockOwner[h][l].Proc := n; 
          LDir.State := Locked;
        Case Locally_Shared:
          Error "inconsistent LDir and LCache";
        Endswitch;

      Else
        Switch LCache.State
        Case Non_Locally_Cached: -- enter queue to wait for lock
          LCache.State := Locally_Shared;
          LDir.State := Queued;
          Add_to_LDir_Entries(h,l,n);
        Case Locally_Shared:
          -- spinning on cache; or processor works on other jobs
        Endswitch;
      Endswitch;      

    Case DGNTS:
      Switch LDir.State
      Case Unlocked:
        Error "DGNTS with unlocked lock";
      Else -- acquire successful
        If ( LCache.State = Locally_Shared ) Then
          LCache.State := Non_Locally_Cached;
        Endif;
        Check_Mutual_Exclusion(h,l);
        LockOwner[h][l].State := Valid;
        LockOwner[h][l].Proc := n; 
        If LRAC.Granting = h
        Then
          Assert ( ULRAC.State = WDIRW ) "lock ack to funny ULRAC state";
          ULRAC.State := INVAL;
        Else
          Send_ACK_L(LRAC.Granting, n, h, l);
        Endif;
	LRAC.State := INVAL;
        Undefine LRAC.Granting;
      End; --switch;      
    End; --switch;
  End; --rule;  


  Rule "Remote Lock Acquire"
    ( h != n )
    & !( LockOwner[h][l].State = Valid
       & LockOwner[h][l].Proc = n )
  ==> 
  Begin
    Switch LRAC.State
    Case INVAL:
      Switch LCache.State
      Case Non_Locally_Cached: -- lock status not known
        LRAC.State := WLCK;
        Send_RD_L(h, n, l);
      Case Locally_Shared: -- locked
        -- no action; processor do other jobs and then come back. 
      Endswitch;

    Case DLCK: -- already have reply from home saying locked
      Switch LCache.State
      Case Non_Locally_Cached: -- lock status not known
        LCache.State := Locally_Shared;
	LRAC.State := INVAL;
        Undefine LRAC.Granting;
      Case Locally_Shared:
        Error "DLCK with cached lock";
      Endswitch;

    Case DULCK: -- already have reply from home saying not locked
      Switch LCache.State
      Case Non_Locally_Cached:
        Check_Mutual_Exclusion(h,l);
        LockOwner[h][l].State := Valid;
        LockOwner[h][l].Proc := n; 
	LRAC.State := INVAL;
        Undefine LRAC.Granting;
      Case Locally_Shared:
        Error "DULCK with cached lock";
      Endswitch;

    Case DGNTS: -- lock granted from the former owner cluster
      If ( LCache.State = Locally_Shared ) Then
	-- Undefine old locked message in cache
        LCache.State := Non_Locally_Cached;
      Endif;
      Check_Mutual_Exclusion(h,l);
      LockOwner[h][l].State := Valid;
      LockOwner[h][l].Proc := n; 
      Send_ACK_L(LRAC.Granting, n, h, l);
      LRAC.State := INVAL;
      Undefine LRAC.Granting;

    Case WLCK: -- still waiting for reply from home
      Switch LCache.State
      Case Non_Locally_Cached: -- merge request
      Case Locally_Shared:
        Error "WLCK with cached lock";
      Endswitch;

    Case RLCK: -- invalidated reply from home
      Switch LCache.State
      Case Non_Locally_Cached: -- conflict
      Case Locally_Shared:
        Error "WLCK with cached lock";
      Endswitch;
 
    Else
      Error "funny LRAC state with lock request enabled.";

    Endswitch; -- LRAC state
  Endrule;

Endalias; -- LRAC, ULRAC, LCache, LDir
Endruleset; -- l
Endruleset; -- h
Endruleset; -- n

/*
CPU IV

The CPU indeterministically issue unlock request 

When a CPU has acquired a arbitrary lock, it may release 
it at any time.

Home lock release:
If there is no conflict to the ULRAC entry, the lock is unlocked
or granted to the queued cluster.  And the ULRAC wait for Granting
acknowledgement.

Remote lock release:
If there is no conflict to the ULRAC entry, release request is sent
to the home cluster and the ULRAC wait for granting acknowledgement.

Two sets of rules
  Rule "Home Lock Release"
  Rule "Remote Lock Release"

Issue messages:
   GSET
   IOWR
*/

Ruleset n:Proc Do
Ruleset h:Home Do
Ruleset l:LAddress Do
Alias
  ULRAC: Procs[n].ULRAC[h][l];
  LCache: Procs[n].LCache[h][l];
  LDir: Homes[h].LDir[l]
Do

  Ruleset j:Dir_Index Do
  -- select cluster at queue to grant lock nondeterministically
  Rule "Home Lock Release"
    (h=n) & !nohome
    & LockOwner[h][l].State = Valid
    & LockOwner[h][l].Proc = n
  ==>
  Begin
    Switch ULRAC.State
    Case INVAL:
      Switch LDir.State
      Case Unlocked:
        Error "release with unlocked lock";
      Case Locked: -- release lock
        LDir.State := Unlocked;
        LDir.QueuedCount := 0;
        Undefine LDir.Entries;
        LockOwner[h][l].State := Invalid;
        Undefine LockOwner[h][l].Proc;
      Case Queued: -- grant lock to other
        -- indeterministically grant a cluster 
        If ( j < LDir.QueuedCount
           & LDir.Entries[j] != h )
        Then
          ULRAC.State := WDIRW;
          Send_Grant_SET(LDir.Entries[j] , h, h, l);
          Remove_from_Queue(n,l,j);
          LockOwner[h][l].State := Invalid;
          Undefine LockOwner[h][l].Proc;
        Else
          -- repeat rule to choose other valid cluster.
        Endif;
      Endswitch;

    Case WDIRW: -- conflict
      -- processor works on other jobs first and then retry.
      -- possible sequence of actions that causes this situration 
      -- 1) release
      -- 2) remote lock and release
      -- 3) acquire again 
      -- 4) and try to release before ack come back.
 
    Else
      Error "funny ULRAC with lock release action enbled";

    Endswitch;
  Endrule;
Endruleset; -- j


  Rule "Remote Lock Release"
    (h!=n)
    & LockOwner[h][l].State = Valid
    & LockOwner[h][l].Proc = n
  ==>
  Begin
    Switch ULRAC.State
    Case INVAL: --no pending event; ready to go
      ULRAC.State := WDIRW;
      Send_Unlock(h, n, n, l);
      LockOwner[h][l].State := Invalid;
      Undefine LockOwner[h][l].Proc;

    Case WDIRW: -- conflict; 
      -- processor works on other jobs first and then retry.
      -- possible sequence of actions that causes this situration 
      -- 1) release
      -- 2) remote lock and release
      -- 3) acquire again 
      -- 4) and try to release before ack come back.

    Else
      Error "funny ULRAC with lock release action enbled";

    Endswitch;
  Endrule;

Endalias; -- ULRAC, LCache, LDir
Endruleset; -- l
Endruleset; -- h
Endruleset; -- n

/*
CPU V

Modelling lock acquire timeout

A granted lock may never be claimed by the requesting CPU,
as a result of process migration, etc.  A timeout mechanism is
implemented in DASH to refuse a granted lock. 

Two sets of rules
  Rule "Timeout at remote cluster -> Grant Rejected "
  Rule "Timeout at home cluster -> Grant Rejected"

Issue messages:
	IOWR        
*/
Ruleset n:Proc Do
Ruleset h:Home Do
Ruleset l:LAddress Do
Ruleset j:Dir_Index Do
Alias
  LRAC: Procs[n].LRAC[h][l];
  ULRAC: Procs[n].ULRAC[h][l];
  LCache: Procs[n].LCache[h][l];
  LDir: Homes[h].LDir[l]
Do

  Rule "Timeout at remote cluster -> Grant Rejected "
    ( h != n
    & LRAC.State = DGNTS )
  ==>
  Begin
    If ( LCache.State = Locally_Shared ) Then
      LCache.State := Non_Locally_Cached;
    Endif;

    Send_Unlock(h, n, LRAC.Granting, l);
    LRAC.State := INVAL;
    Undefine LRAC.Granting;
  End;


  Rule "Timeout at home cluster -> Grant Rejected"
    ( h = n) & !nohome
    & LRAC.State = DGNTS
  ==>
  Begin
    If ( LCache.State = Locally_Shared ) Then
      LCache.State := Non_Locally_Cached;
    Endif;
    Switch LDir.State
    Case Unlocked:
      Error "release with Unlocked lock";
    Case Locked:
      LDir.State := Unlocked;
      LDir.QueuedCount := 0;
      Undefine LDir.Entries;
      If ( LRAC.Granting = n )
      Then
        Assert ( ULRAC.State = WDIRW ) "lock ack to funny ULRAC state";
        ULRAC.State := INVAL;
      Else
        Send_ACK_L(LRAC.Granting, n, n, l);
      Endif;
      LRAC.State := INVAL;
      Undefine LRAC.Granting;

    Case Queued:
      -- indeterministically grant a cluster 
      If ( j < LDir.QueuedCount
         & LDir.Entries[j] != n )
      Then
        Send_Grant_SET(LDir.Entries[j] , n, LRAC.Granting, l);
        Remove_from_Queue(n,l,j);
        LRAC.State := INVAL;
        Undefine LRAC.Granting;
      Endif;
    Endswitch;
  Endrule;

Endalias; -- LRAC, ULRAC, LCache, LDir
Endruleset; -- j
Endruleset; -- l
Endruleset; -- h
Endruleset; -- n

/*
PCPU III

PCPU handle Lock/unlock request

The PCPU receives requests from the request network channels.

Lock request:
PCPU replies unlocked/locked according to whether the lock
is unlocked/locked .

Unlock request:
PCPU grants the lock to one of the queued clusters
indeterministically, or send unlock acknowledgement if there is not
queued cluster.

Two sets of rules:
  Rule "Handle lock read request"
  Rule "Handle unlock request to home"

Handle messages:
   RD_L     
   IOWR
*/

Ruleset Dst: Proc Do
Ruleset Src: Proc Do
Alias
  ReqChan: ReqNet[Src][Dst];
  Request: ReqNet[Src][Dst].Messages[0].Mtype;
  Aux: ReqNet[Src][Dst].Messages[0].Aux;
  LAddr: ReqNet[Src][Dst].Messages[0].LAddr
Do

  Rule "Handle lock read request"
    ReqChan.Count > 0
    & Request = RD_L
  ==>
  Begin
  Alias
    LDir: Homes[Dst].LDir[LAddr];
    LCache: Procs[Dst].LCache[Dst][LAddr]
  Do
    Switch LDir.State
    Case Unlocked: -- acquire successful; lock granted to the request cluster.
      Switch LCache.State
      Case Non_Locally_Cached:
        LDir.State := Locked;
        Send_Unlock_Reply(Src, Dst, LAddr);
      Case Locally_Shared:
        Error "inconsistent LDir and LCache";
      Endswitch;
    Else -- request unsuccessful; enter queue (not fifo queue)
      LDir.State := Queued;
      Add_to_LDir_Entries(Dst,LAddr,Src);
      Send_Lock_Reply(Src, Dst, LAddr);
    Endswitch;     
    Consume_Request(Src, Dst);
  Endalias; -- LDir, LCache
  Endrule; -- handle lock read 


  Ruleset j:Dir_Index Do
  -- select cluster at queue to grant lock nondeterministically

  Rule "Handle unlock request to home"
    ReqChan.Count > 0
    & Request = IOWR 
  ==>
  Begin
  Alias
    LRAC: Procs[Dst].LRAC[Dst][LAddr];
    ULRAC: Procs[Dst].ULRAC[Dst][LAddr];
    LDir: Homes[Dst].LDir[LAddr]
  Do
    Switch LRAC.State
    Case INVAL:
      Switch LDir.State
      Case Unlocked:
        Error "release when lock is not acquired.";

      Case Locked: -- no cluster is waiting
        LDir.State := Unlocked;
        LDir.QueuedCount := 0;
        Undefine LDir.Entries;

        If ( Aux = Dst ) -- lock granted by home cluster
        Then
          Assert ( ULRAC.State = WDIRW ) "lock ack to funny ULRAC state";
          ULRAC.State := INVAL;
        Else
          Send_ACK_L(Aux, Dst, Dst, LAddr);
        Endif;
        Consume_Request(Src, Dst);

      Case Queued: -- indeterministically grant a cluster the lock
        If ( j < LDir.QueuedCount 
           & LDir.Entries[j] != Dst ) 
        Then
          -- chosen cluster is a remote cluster
          Send_Grant_SET(LDir.Entries[j] , Dst, Aux, LAddr);
          Remove_from_Queue(Dst,LAddr,j);
          Consume_Request(Src, Dst);
        Elsif ( j < LDir.QueuedCount
	      & LDir.Entries[j] = Dst )
        Then
          -- chosen cluster is the home cluster
          LRAC.State := DGNTS;
          LRAC.Granting := Aux;
          Remove_from_Queue(Dst,LAddr,j);
          Consume_Request(Src, Dst);
        Endif;
      Endswitch; -- LDir state

    Else
      Error "IOWR in funny LRAC state";
    Endswitch; -- LRAC state
  Endalias; --LRAC, ULRAC, LDir
  Endrule; -- handle unlock request to home
  Endruleset; -- j: DirIndex

Endalias; -- ReqChan, Request, Aux, LAddr
Endruleset; -- Src
Endruleset; -- Dst

/*
PCPU IV

handle Lock grant request

The PCPU receives requests from the request network channels.

Granting request:
PCPU records the granting of the lock in one of the LRAC entries
and sends granting acknowledgement to the granting clucter.

One set of rules:
  Rule "Handle lock grant to grant cluster"

Handle messages:
   GSET
*/

Ruleset Dst: Proc Do
Ruleset Src: Proc Do
Alias 
  ReqChan: ReqNet[Src][Dst];
  Request: ReqNet[Src][Dst].Messages[0].Mtype;
  LAddr: ReqNet[Src][Dst].Messages[0].LAddr;
  Aux: ReqNet[Src][Dst].Messages[0].Aux
Do

  Rule "Handle lock grant to grant cluster"
    ReqChan.Count > 0
    & Request = GSET
  ==>
  Begin
  Alias
    LRAC: Procs[Dst].LRAC[Src][LAddr];
    ULRAC: Procs[Dst].ULRAC[Src][LAddr]
  Do
    Switch LRAC.State
    Case INVAL: -- grant lock
      LRAC.State := DGNTS;
      LRAC.Granting := Aux;
    Case WLCK: -- conflict -- Will receive LCK later
      LRAC.State := RLCK;
      Send_Unlock(Src, Dst, Aux, LAddr);
    Case DLCK: -- grant lock
      LRAC.State := DGNTS;
      LRAC.Granting := Aux;
    Case RLCK:
      Error "GSET with RLCK";
      -- in PROM table
      -- LRAC.State := RLCK;
      -- however, this case never arise
    Else
      Error "GSET with funny LRAC state";
    Endswitch;
    Consume_Request(Src, Dst);
  Endalias; -- LRAC, ULRAC
  Endrule;  -- handle lock grant

Endalias; -- ReqChan, Request, LAddr, Aux
Endruleset; -- Src
Endruleset; -- Dst

/*
RCPU II
 
handle locked/unlocked reply and granting acknowledgement

The locked/unlocked reply is recorded in the LRAC.
The acknowledgement Undefines the ULRAC entry.

Three sets of rules
  Rule "Handle unlock reply"
  Rule "Handle lock reply"
  Rule "Handle release acknowledgement"

Handle messages:
   UNLCK
   LCK
   ACK_L
*/

Ruleset Dst: Proc Do
Ruleset Src: Proc Do
Alias
  ReplyChan: ReplyNet[Src][Dst];
  Reply: ReplyNet[Src][Dst].Messages[0].Mtype;
  LAddr: ReplyNet[Src][Dst].Messages[0].LAddr;
  Aux: ReplyNet[Src][Dst].Messages[0].Aux
Do

  Rule "Handle unlock reply"
    ReplyChan.Count >0
    & Reply = UNLCK
  ==>
  Begin
  Alias
   LRAC: Procs[Dst].LRAC[Src][LAddr]
  Do
    Switch LRAC.State
    Case WLCK:
      LRAC.State := DULCK;
      -- and RCPU will release processor so that it may try again if it wants to 
    Case RLCK:
      Error "unlock reply with funny LRAC state";
      LRAC.State := DULCK;
      -- and RCPU will release processor so that it may try again if it wants to 
    Else
      Error "unlock reply with funny LRAC state";
    Endswitch;
    Consume_Reply(Src, Dst);
  Endalias; -- LRAC
  Endrule; -- handle unlock reply


  Rule "Handle lock reply"
    ReplyChan.Count > 0
    & Reply = LCK
  ==>
  Begin
  Alias
   LRAC: Procs[Dst].LRAC[Src][LAddr]
  Do
    Switch LRAC.State
    Case WLCK:
      LRAC.State := DLCK;
      -- and RCPU will release processor so that it may try again if it wants to 
    Case RLCK:
      LRAC.State := INVAL;
      Undefine LRAC.Granting;
    Else
      Error "lock reply with funny LRAC state";
    Endswitch;
    Consume_Reply(Src, Dst);
  Endalias; -- LRAC
  Endrule; -- handle lock reply

  Rule "Handle release acknowledgement"
    ReplyChan.Count >0
    & Reply = ACK_L
  ==>
  Begin
  Alias
    ULRAC: Procs[Dst].ULRAC[Aux][LAddr]
  Do
    Switch ULRAC.State
    Case WDIRW:
      ULRAC.State := INVAL;
    Else
      Error "release acknowledgement with funny ULRAC entry";
    End; --switch;
    Consume_Reply(Src, Dst);
  Endalias; -- ULRAC
  Endrule; -- handle release acknowledgement

Endalias; -- ReplyChan, Reply, LAddr, Aux
Endruleset; -- Src
Endruleset; -- Dst

/*
Start state
*/
Startstate
Begin  
  For h : Home Do
  For l : LAddress Do
    Homes[h].LDir[l].State := Unlocked;
    Homes[h].LDir[l].QueuedCount := 0;
    Undefine Homes[h].LDir[l].Entries;
  Endfor;
  Endfor;

  For n : Proc Do
  For h : Home do
  For l : LAddress Do
    Procs[n].LCache[h][l].State := Non_Locally_Cached;
    Procs[n].LRAC[h][l].State := INVAL;
    Procs[n].ULRAC[h][l].State := INVAL;
    Undefine Procs[n].LRAC[h][l].Granting;
  Endfor;
  Endfor;
  Endfor;

  For h : Home do
  For l : LAddress Do
    LockOwner[h][l].State := Invalid;
    Undefine LockOwner[h][l].Proc;
  Endfor;
  Endfor;

  For Dst: Proc Do 
  For Src: Proc Do 
    ReqNet[Src][Dst].Count := 0;
    Undefine ReqNet[Src][Dst].Messages;
    ReplyNet[Src][Dst].Count := 0;
    Undefine ReplyNet[Src][Dst].Messages;
  Endfor;
  Endfor;
End; 

/* 
Invarient: Correct lock state
Invarient: Correct lock queue
Invariant: Irrelevant queue members are set to zero
Invarient: Mutual Exclusion -- by error assertion
*/

Invariant "Correct lock state -> forward"
  Forall h : Home Do
  Forall l : LAddress Do
    LockOwner[h][l].State = Valid
    -> 
    ( Homes[h].LDir[l].State = Locked
    | Homes[h].LDir[l].State = Queued )
  Endforall
  Endforall;

Invariant "Correct lock state -> backward"
  Forall h : Home Do
  Forall l : LAddress Do
    ( Homes[h].LDir[l].State = Locked
    | Homes[h].LDir[l].State = Queued )
    ->
    ( LockOwner[h][l].State = Valid
    | Exists n : Proc Do
        ( Procs[n].LRAC[h][l].State = DULCK 
        | Procs[n].LRAC[h][l].State = DGNTS )
      Endexists
    | Exists n : Proc Do
      Exists i : 0..ChanMax-1 Do
        ( i < ReplyNet[h][n].Count
        & ReplyNet[h][n].Messages[i].Mtype = UNLCK )
        |
        ( i < ReqNet[n][h].Count
        & ReqNet[n][h].Messages[i].Mtype = IOWR )
        |
        ( i < ReqNet[h][n].Count
        & ReqNet[h][n].Messages[i].Mtype = GSET )
       Endexists
       Endexists )
  Endforall
  Endforall;

Invariant "Correct lock queue"
  Forall h : Home Do
  Forall l : LAddress Do
  Forall j : 0..DirMax-1 Do
  Forall n : Proc Do
    ( Homes[h].LDir[l].State = Queued
    & j < Homes[h].LDir[l].QueuedCount
    & Homes[h].LDir[l].Entries[j] = n )
    -> 
    ( Procs[n].LCache[h][l].State = Locally_Shared
    | Procs[n].LRAC[h][l].State = DLCK 
    | Exists i : 0..ChanMax-1 Do
        ( i < ReplyNet[h][n].Count
        & ReplyNet[h][n].Messages[i].Mtype = LCK )
      Endexists )
  Endforall
  Endforall
  Endforall
  Endforall;

Invariant "Irrelevant queue members are set to zero"
  Forall h : Home Do
  Forall l : LAddress Do
    ( Homes[h].LDir[l].State = Queued
    | Homes[h].LDir[l].QueuedCount = 0 )
    &
    ( Forall i:Dir_Index Do
        i>=Homes[h].LDir[l].QueuedCount
        -> Isundefined(Homes[h].LDir[l].Entries[i])
      Endforall )
  Endforall
  Endforall;

/******************

Summary of Result (using release 2.3):

1) 1 cluster with memory and 1 remote clusters

   breath-first search
   135 bits (17 bytes) per state
   163 states with a max of about 22 states in queue
   683  rules fired
   1.22s in sun sparc 2 station

2) 1 cluster with memory and 2 remote clusters

   breath-first search
   346 bits (44 bytes) per state
   12207 states with a max of about 1006 states in queue
   85650 rules fired
   195.57s in sun sparc 2 station

3) 1 no-processor cluster with memory and 3 remote clusters

   breath-first search
   595 bits (75 bytes) per state
   55366 states with a max of about 3812 states in queue
   422613 rules fired
   1643.37s in sun sparc 2 station

4) 1 no-processor cluster with memory and 1 remote clusters

   breath-first search
   135 bits (17 bytes) per state
   10 states
   20 rules fired
   0.12s in sun sparc 2 station

5) 1 no-processor cluster with memory and 2 remote clusters

   breath-first search
   346 bits (44 bytes) per state
   740 states with a max of about 64 states in queue
   3596 rules fired
   9.23s in sun sparc 2 station

2.73S
* 2 remote with home

   BFS sym1
   368 bits (46 bytes)
   6114 states with 484 max in queue
   42895 rules fired
   168.99 s in sun sparc 2 station

* 3 remote without home

   BFS sym1
   738 bits (93 bytes)
   9313 states with 654 max in queue
   71035 rule fired
   504.91s in sun sparc 2 station

   BFS sym3
   19537 states with 706 max in queue
   81307 rules fired
   558.90s in sun sparc 2 station

   BFS nosym
   55366 states with 3803 max in queue.
   422613 rules fired
   1426.55s in sun sparc 2 station

Release 2.9S (Sparc 20, cabbage.stanford.edu)

   4 remote processes
         * The size of each state is 4232 bits (rounded up to 532 bytes).
      -b * The size of each state is 1130 bits (rounded up to 144 bytes).
         254987 states, 2647242 rules fired in 4666.36s.
      -b 255028 states, 2647955 rules fired in 10611.92s.
      -c 254987 states, 2647242 rules fired in 5777.85s.



******************/


