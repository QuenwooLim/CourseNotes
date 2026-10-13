-----------------------------------------------------------------------
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
-------------------------------------------------------------------------

-------------------------------------------------------------------------
--
-- File : cache3.m
--
-- Cache Coherence Protocol on a General Network (message order
-- is NOT preserved).
--
-- David L. Dill at Stanford University.   Initial protocol design
--  by Andreas Nowatzyk, and the initial description was derived
--  from a description by Ken McMillan.  Modified by Norris Ip
--
-- Note from Dill: If I had to do it over again, I would model the
-- network as a 2D array indexed by source and destination instead
-- of a single array with all the messages in arbitrary order.
--
--  The invariant is also an unnecessarily complicated mess!
--
-------------------------------------------------------------------------
--
-- Status:  No violations of verification conditions, but it does
-- not necessarily implement an appropriate memory model.
--
--------------------------------------------------------------------------
-- 
-- Note:
--
-- For verification, it is convenient to split nodes into two
-- parts: "homes", which have the Memory, Directory and Remote Access
-- System (RAS) and "procs" (processors), which have the Cache and
-- Pending Message Module (PMM).
--
----------------------------------------------- March 4, 1993 ------------

Const
  HomeCount: 1;		-- number of homes.
  ProcCount: 2;		-- number of processors.
  AddressCount: 1;   	-- number of addresses
  ValueCount: 1;	-- number of data values
  DirMax: ProcCount;	-- Maximum number of dir
			-- entries that can be kept
  NetMax: DirMax + 3;	-- Network capacity.  Must be at least DirMax to
			-- allow for invalidations on Cache_Promote,
			-- Cache_Read_Ex.

Type
  Home: 0 .. HomeCount-1;
  Proc:	0 .. ProcCount-1;
  -- Kludge! so I don't have to redo all message field types.
  Node: 0 .. ProcCount-1;

  Address: 0 .. AddressCount-1;
  Value: 0 .. ValueCount-1;

  Message_Type:	enum{	Cache_Read,	-- request for shared copy
		   	Cache_Read_Ex,	-- request for master copy
			Cache_Promote,	-- request for changing shared copy
					-- to master copy
			Uncache,	-- request for removing shared copy
			Write_Back,	-- request for removing master copy
			Update, 	-- update data memory w/ new value
			Data,		-- reply with data
			Invalidate,     -- request invalidate data at cache
			Uncache_Ack,	-- acknowledge for uncache data
			Write_Back_Ack, -- acknowledge for write back
			Inv_Ack,	-- acknowledge for invalidate
			Fw_Cache_R,     -- forwarded shared copy request
					-- to remote
			Fw_Cache_R_Ex,  -- forwarded master copy request
					-- to remote
			Ack_Fw_Cache_Ex,-- acknowledge for forwarded
					-- master copy
			Cache_Promote_Ack-- grant permission to change to
					-- master copy
			};
  Message:
    Record
      MType: Message_Type;
      Source: Node;
      Destination: Node;
      Aux: Node;  -- Local in Fw_Cache_R, Fw_Cache_R_Ex, Home in Data.
      Address: Address;
      Value: Value;
    End;

  -- "Address" is node-relative.  A physical address is a pair consisting
  -- of a node and address.

Type
  HomeState:
    Record
      Mem:  Array [Address] of Value;
      Dir:  Array [Address] of
        Record
          State: enum{	Inv, 		-- Invalid
			Shared_Remote,  -- Shared by remote clusters
			Master_Remote,  -- Owned by a remote cluster
			Wait_Inv,	-- Waiting for Invalidate 
					-- Acknowledge
			Wait_Update,	-- Waiting for Update to home
			Wait_WB		-- Waiting for ack for the
					-- transfer of ownership or
					-- write back of master copy
			};
          Shared_Count: 0..DirMax; -- "DirMax" means "DirMax or more".
	  Entries: array [0..DirMax-1] of Proc;
	  Inv_Count: Proc; -- number of pending invalidates.
        End;
    End;

  ProcState:
    Record
      -- Cache and Pending Message Module (PMM)
      -- must keep track of physical addresses.
      Cache:  Array [Home] of Array [Address] of
        Record
          State: enum{I, S, M}; -- Invalid, Shared, or Master
          Value: Value;
        End;
      PMMState:  Array [Home] of Array [Address] of 
		   enum{NOP,		-- invalid
			CR_Pend,	-- waiting for Cache_Read reply
			CRE_Pend,	-- waiting for Cache_Read_Ex reply
			CP_Pend,	-- waiting for Cache_Promote reply
			WB_Pend, 	-- waiting for Write_Back_Ack
			Uncache_Pend	-- waitign for Uncache_ack
			};
    End;

Var
  Homes:  Array [Home] of HomeState;
  Procs:  Array [Proc] of ProcState;

  -- The Net is, abstractly, a bounded set of NetMax or
  -- fewer messages.  It is implemented as an unsorted array,
  -- along with a count of the number of elements in the set.
  -- The array entries from 0..Count-1 are all valid messages.
  -- The entries from Count..MaxNet-1 are all invalid, and are
  -- set to a fixed default value to reduce state space blowup.

  Net:
    Record
      Count: 0..NetMax;
      Ar: Array [0..NetMax-1] of Message;
    End;

-- Network functions

Procedure Send( t: Message_Type;
	        Dst: Node;
   	        Src: Node;
	        Loc: Node;
	        Addr: Address;
	        Val: Value);

  If Net.Count = NetMax
  Then
    Error "Network is full";
  Else
    Net.Ar[Net.Count].MType := t;
    Net.Ar[Net.Count].Source := Src;
    Net.Ar[Net.Count].Destination := Dst;
    Net.Ar[Net.Count].Aux := Loc;
    Net.Ar[Net.Count].Address := Addr;
    Net.Ar[Net.Count].Value := Val;
    Net.Count := Net.Count+1;
  End;
End;

Procedure Consume_Message( M_Index:0..NetMax-1);
  If M_Index >= Net.Count
  Then
    Error "Invalid Message Index";
  Else 
    For i:0..NetMax-2 Do
      If i >= M_Index
      Then
        Net.Ar[i] := Net.Ar[i+1];
      End;
    End;
    -- Clear last entry.
    Net.Ar[Net.Count-1].MType := Cache_Read;
    Net.Ar[Net.Count-1].Source := 0;
    Net.Ar[Net.Count-1].Destination := 0;
    Net.Ar[Net.Count-1].Aux := 0;
    Net.Ar[Net.Count-1].Address := 0;
    Net.Ar[Net.Count-1].Value := 0;
  End;
  Net.Count := Net.Count -1;
End;
-- Special send functions

Procedure Send_Cache_Read( Dst:Node;  Src:Node;  Addr:Address);
  Send(Cache_Read, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Cache_Read_Ex( Dst:Node;  Src:Node;  Addr:Address);
  Send(Cache_Read_Ex, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Uncache( Dst:Node;  Src:Node;  Addr:Address);
  Send(Uncache, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Uncache_Ack( Dst:Node;  Src:Node;  Addr:Address);
  Send(Uncache_Ack, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Data( Dst:Node;  Src:Node;  Home:Node;
		     addr:Address;  val:Value);
  Send(Data, Dst, Src, Home, addr, val);
End;

Procedure Send_Fw_Cache_R( Dst:Node;  Src:Node; 
		      Loc:Node;  Addr:Address);
  Send(Fw_Cache_R, Dst, Src, Loc, Addr, 0);
End;

Procedure Send_Fw_Cache_R_Ex( Dst:Node;  Src:Node; 
		      Loc:Node;  Addr:Address);
  Send(Fw_Cache_R_Ex, Dst, Src, Loc, Addr, 0);
End;

Procedure Send_Ack_Fw_Cache_Ex( Dst:Node;  Src:Node;  Addr:Address);
  Send(Ack_Fw_Cache_Ex, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Invalidate( Dst:Node;  Src:Node;  Addr:Address);
  Send(Invalidate, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Inv_Ack( Dst:Node;  Src:Node;  Addr:Address);
  Send(Inv_Ack, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Write_Back( Dst:Node;  Src:Node;
			   Addr:Address;  Data:Value);
  Send(Write_Back, Dst, Src, 0, Addr, Data);
End;

Procedure Send_Update( Dst:Node;  Src:Node;  Addr:Address;
		       Data:Value);
  Send(Update, Dst, Src, 0, Addr, Data);
End;

Procedure Send_Cache_Promote( Dst:Node;  Src:Node;  Addr:Address);
  Send(Cache_Promote, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Cache_Promote_Ack( Dst:Node;  Src:Node;  Addr:Address);
  Send(Cache_Promote_Ack, Dst, Src, 0, Addr, 0);
End;

Procedure Send_Write_Back_Ack( Dst:Node;  Src:Node; 
			       Addr:Address);
  Send(Write_Back_Ack, Dst, Src, 0, Addr, 0);
End;

-- Directory support functions

Procedure add_dir_entry( h:Node;  a:Address;  n:Node);
  -- h = home node, a = address, n = node to be added.
  If Homes[h].Dir[a].Shared_Count = DirMax
  Then
    Error "Directory overflow";
  End; 
  Homes[h].Dir[a].Entries[Homes[h].Dir[a].Shared_Count] := n;
  Homes[h].Dir[a].Shared_Count := Homes[h].Dir[a].Shared_Count+1;
End;

-- kludged a bit to avoid needing a local variable.
-- WARNING! Calling this rearranges the directory, so
-- indices of directory entries will change!
Procedure remove_dir_entry( h:Node;  a:Address;  n:Node);
  If Homes[h].Dir[a].Shared_Count = 0
  Then
    Error "Cannot remove from empty directory";
  End;	 
  -- h = home node, a = address, n = node to be added.
  For i : 0..DirMax-1 Do
    If (i < Homes[h].Dir[a].Shared_Count) &
       (Homes[h].Dir[a].Entries[i] = n)
    Then
      -- overwrite this entry with last entry.
      Homes[h].Dir[a].Entries[i] := 
	Homes[h].Dir[a].Entries[Homes[h].Dir[a].Shared_Count-1];
      -- clear last entry	
      Homes[h].Dir[a].Entries[Homes[h].Dir[a].Shared_Count-1] := 0;
      Homes[h].Dir[a].Shared_Count := Homes[h].Dir[a].Shared_Count-1;
    End;
  End;	
  -- if we removed last entry, go to Inv state.
  If Homes[h].Dir[a].Shared_Count = 0
  Then
    Homes[h].Dir[a].State := Inv;
  End;
End;

-- Procedures for operations that are repeated in several places.

-- This procedure is called when an Inv_Ack message is received,
-- or when we would like to pretend that that has happened.
Procedure Handle_Inv_Ack( n:Node;  addr: Address);
  If Homes[n].Dir[addr].Inv_Count = 0
  Then 
    Error "Bad invalidation count";
  End;
  Homes[n].Dir[addr].Inv_Count := Homes[n].Dir[addr].Inv_Count-1;
  If Homes[n].Dir[addr].Inv_Count = 0
  Then
    If Homes[n].Dir[addr].State != Wait_Inv
    Then
      Error "Should be in RE state";
    End;
    Homes[n].Dir[addr].State := Master_Remote;
  End;
End;

Procedure Handle_Uncache_Ack( n:Node;  home:Node; 
			      addr:Address);
  If Procs[n].PMMState[home][addr] != Uncache_Pend
  Then
    Error "PMM state not Uncache_Pend";
  End;
  Procs[n].PMMState[home][addr] := NOP;
  Procs[n].Cache[home][addr].State := I;
  Procs[n].Cache[home][addr].Value := 0;
End;

Procedure Handle_Write_Back_Ack( n:Node;  home:Node; 
				 addr:Address);
  If Procs[n].PMMState[home][addr] != WB_Pend
  Then
    Error "PMM state not WB_Pend";
  End;
  Procs[n].PMMState[home][addr] := NOP;
  Procs[n].Cache[home][addr].State := I;
  Procs[n].Cache[home][addr].Value := 0;
End;

-- PMM rules
Ruleset n:Proc Do
  Alias me:Procs[n] Do
    Ruleset h:Home Do
      Ruleset a:Address Do

	-- Transaction initiation ("send" part of PMM).
	-- All of these messages must leave lots of room in the network
	-- to avoid overfilling it with invalidate messages if there
	-- is pending Cache_Promote or Cache_Read_Ex message.
        Rule
           -- spontaneously generate a Coherent Read
            (me.Cache[h][a].State = I)
          & (me.PMMState[h][a] = NOP)
	  & (Net.Count <= NetMax - ProcCount - 1)  -- avoid deadlock
        ==>
          me.PMMState[h][a] := CR_Pend;
          Send_Cache_Read(h, n, a);
	End; -- rule

        Rule
           -- spontaneously generate a Coherent Read Invalidate
            (me.Cache[h][a].State = I)
          & (me.PMMState[h][a] = NOP)
	  & (Net.Count <= NetMax - ProcCount - 1)  -- avoid deadlock
        ==>
          me.PMMState[h][a] := CRE_Pend;
          Send_Cache_Read_Ex(h, n, a);
	End; -- rule

        Rule
           -- spontaneously generate a Coherent Invalidate
            (me.Cache[h][a].State = S)
          & (me.PMMState[h][a] = NOP)
	  & (Net.Count <= NetMax - ProcCount - 1)  -- avoid deadlock
        ==>
          me.PMMState[h][a] := CP_Pend;
          Send_Cache_Promote(h, n, a);
	End; -- rule
-- PMM rules, cont.

	Rule
	  -- uncache
	    (me.Cache[h][a].State = S)
	  & (me.PMMState[h][a] = NOP)
	  & (Net.Count <= NetMax - ProcCount - 1)  -- avoid deadlock
	==>
	  me.PMMState[h][a] := Uncache_Pend; 
	  Send_Uncache(h, n, a);
	End; -- rule

	Rule
	  -- writeback due to replacement.
	    (me.Cache[h][a].State = M)
	  & (me.PMMState[h][a] = NOP)
	  & (Net.Count <= NetMax - ProcCount - 1)  -- avoid deadlock
	==>
	  me.PMMState[h][a] := WB_Pend;
	  Send_Write_Back(h, n, a, me.Cache[h][a].Value);
	End; -- rule

	-- When an address is cached and writable, we can
	-- change its value arbitrarily.
	Ruleset v:Value Do
	  Rule
	      (me.Cache[h][a].State = M)
            & (me.PMMState[h][a] = NOP)
	  ==>
	    me.Cache[h][a].Value := v;
	  End;	 -- rule
	End; -- ruleset

      End; -- ruleset
    End; -- ruleset
  End; -- alias
End; -- ruleset
 
-- Receive part of PMM.  Note: "n" and "me" are bound in enclosing
-- ruleset and alias.

-- This rule set nondeterministically chooses a candidate
-- message from the Net.
Ruleset M_Index: 0..NetMax-1 Do
  Alias msg:Net.Ar[M_Index] Do
    Alias n:msg.Destination Do
      Alias me:Procs[n] Do
        Rule
            -- is it a valid message?
            (M_Index < Net.Count)
        ==>
	  Switch msg.MType

	  Case Data:
            Switch me.PMMState[msg.Aux][msg.Address]
  	    Case CR_Pend:
	      -- home is msg.Aux.
              me.PMMState[msg.Aux][msg.Address] := NOP;
              me.Cache[msg.Aux][msg.Address].State := S;
              me.Cache[msg.Aux][msg.Address].Value := msg.Value;
              Consume_Message(M_Index);
	    Case CRE_Pend:
              me.PMMState[msg.Aux][msg.Address] := NOP;
              me.Cache[msg.Aux][msg.Address].State := M;
              me.Cache[msg.Aux][msg.Address].Value := msg.Value;
              Consume_Message(M_Index);
	    Else
	      Error "Data received in funny PMMState";
	    End; -- switch

	  Case Invalidate:
	    If   (  me.Cache[msg.Source][msg.Address].State = S 
		  | me.Cache[msg.Source][msg.Address].State = I)
	    Then
	      me.Cache[msg.Source][msg.Address].State := I;
	      me.Cache[msg.Source][msg.Address].Value := 0;
	      Switch me.PMMState[msg.Source][msg.Address]
	        Case NOP:
		  Send_Inv_Ack(msg.Source, n, msg.Address);
	          Consume_Message(M_Index);
	        Case CR_Pend: -- !! Block.  Wait for the data.
	        Case CRE_Pend:
	          -- Abort.  Cache_Read_Ex will be treated as Inv_Ack
	          -- by home.
	          me.PMMState[msg.Source][msg.Address] := NOP;
	          Consume_Message(M_Index);
	        Case CP_Pend:
	          -- Abort.  Cache_Promote will be treated as Inv_Ack
	          -- by home.
	          me.PMMState[msg.Source][msg.Address] := NOP;
	          Consume_Message(M_Index);
	        Case Uncache_Pend:
	          -- Invalidate and Uncache passed in the network.
	          -- Treat invalidate like an Uncache_Ack.
	          Handle_Uncache_Ack(n, msg.Source, msg.Address);
		  Consume_Message(M_Index);
	        Else
	          Error "Got Invalidate with funny PMM state";
	        End; -- switch
	    Else
	      Error "Invalidate message when in M state";	
	    End; -- if
-- PMM rules, cont.

          Case Fw_Cache_R:
	    -- We have an M copy.  Someone else wants an S copy.
	    Switch me.PMMState[msg.Source][msg.Address]
	    Case NOP:
	      -- Normal case.  Change Cache state to S.  Update
	      -- home, forward data to local.
              me.Cache[msg.Source][msg.Address].State := S;
	      Send_Data(msg.Aux, n, msg.Source, msg.Address,
		        me.Cache[msg.Source][msg.Address].Value);
              Send_Update(msg.Source, -- home.
                          n,
                          msg.Address,
                          me.Cache[msg.Source][msg.Address].Value);
              Consume_Message(M_Index);
	    Case CRE_Pend: -- Block.
	      -- !! This case is tricky.  Home sent data, then
	      -- got a Cache_Read from someone else and sent a Fw_Cache_R.
	      -- But the Fw_Cache_R got here first.
	      -- We have to block, because we don't have the
	      -- data to forward.
	      -- The data is definitely on its way, so we won't
	      -- deadlock.	
	      -- Let the Fw_Cache_R sit in the queue.
	    Case CP_Pend: -- !! Block. 
	      -- We did a Cache_Promote successfully, but Cache_Read from
	      --  someone else caused a Fw_Cache_R, which got here before
	      -- the Cache_Promote_Ack.
	      -- There may be some way to abort this guy.
	    Case WB_Pend:
	      -- We're already writing it back.  Treat this as
	      -- a Write_Back_Ack, but forward data to local.
	      Send_Data(msg.Aux, n, msg.Source, msg.Address,
		        me.Cache[msg.Source][msg.Address].Value);
	      Handle_Write_Back_Ack(n, msg.Source, msg.Address);
              Consume_Message(M_Index);
	    Else
	      Error "Fw_Cache_R received with funny PMMState";	
	    End; -- switch
-- PMM rules, cont.

	  Case Fw_Cache_R_Ex:
	    -- We have an M copy. Someone else wants one,
	    -- so we have to write ours back, convert to I.
	    Switch me.PMMState[msg.Source][msg.Address]
	    Case NOP:
	      -- Normal case.
	      If me.Cache[msg.Source][msg.Address].State != M
	      Then
	        Error "Fw_Cache_R_Ex received, but Cache not M";
	      End;
	      -- Forward the data immediately.  Send Ack_Fw_Cache_Ex to home.
	      -- clear Cache entry.
	      Send_Data(msg.Aux,
			n,
			msg.Source,
			msg.Address,
			me.Cache[msg.Source][msg.Address].Value);
	      me.Cache[msg.Source][msg.Address].State := I;
	      me.Cache[msg.Source][msg.Address].Value := 0;
	      Send_Ack_Fw_Cache_Ex(msg.Source, n, msg.Address);
	      Consume_Message(M_Index);
	    Case WB_Pend:
	      -- We are already in the middle of a writeback.
	      -- Treat Fw_Cache_R_Ex like a Write_Back_Ack, but forward data to 
	      -- local.
	      Send_Data(msg.Aux, n, msg.Source, msg.Address,
			me.Cache[msg.Source][msg.Address].Value);
	      Handle_Write_Back_Ack(n, msg.Source, msg.Address);
	      Consume_Message(M_Index);
	    -- !! Note that in the next two cases, data can get
	    -- snatched away from a node before the processor
	    -- has a chance to write to it (Fw_Cache_R_Ex is waiting for
	    -- data to arrive).  I hope it's not a problem.
	    Case CP_Pend:    -- We don't have the data yet, so block
	    Case CRE_Pend:   -- these messages until we do.
	    Else
	      Error "Fw_Cache_R_Ex received in funny PMMState";
	    End; -- switch
-- PMM rules, cont.

	  Case Cache_Promote_Ack:
	    -- Our Cache_Promote has succeeded.  Make the copy writeable.
	    If me.PMMState[msg.Source][msg.Address] = CP_Pend
	    Then 
	      me.PMMState[msg.Source][msg.Address] := NOP;
              me.Cache[msg.Source][msg.Address].State := M;
              Consume_Message(M_Index);
	    Else
              Error "Cache_Promote_Ack when PMMstate not CP_Pend";
	    End; -- if

	  Case Uncache_Ack:
            -- Home has seen the uncache.  Invalidate the entry.
	    If me.PMMState[msg.Source][msg.Address] = Uncache_Pend
	    Then
	      Handle_Uncache_Ack(n, msg.Source, msg.Address);
	      Consume_Message(M_Index);
	    Else
	      Error "Uncache_Ack received in funny PMMState";
	    End; -- if

	  Case Write_Back_Ack:
	    If me.PMMState[msg.Source][msg.Address] = WB_Pend
	    Then		
	      Handle_Write_Back_Ack(n, msg.Source, msg.Address);
              Consume_Message(M_Index);
	    Else
	      Error "Write_Back_Ack when PMMState not WB_Pend";	
	    End;

	  Else
	    -- do nothing.  Message wasn't for us.
	  End; -- switch
        End; -- rule 

      End; -- alias
    End; -- alias
  End; -- alias
End; -- alias

-- RAS< rules. "n" and "me" still bound
Ruleset M_Index:0..NetMax-1 Do
  Alias msg:Net.Ar[M_Index] Do
    Alias n:msg.Destination Do
          Rule
              -- is it a valid message?
              (M_Index < Net.Count)
              &
	      -- is it for home	
              ( msg.MType = Cache_Read
              | msg.MType = Cache_Read_Ex
              | msg.MType = Cache_Promote
              | msg.MType = Write_Back
              | msg.MType = Update
              | msg.MType = Uncache
              | msg.MType = Ack_Fw_Cache_Ex
              | msg.MType = Inv_Ack )
          ==>
          Alias me:Homes[n] Do
            -- Case on message type
            Switch msg.MType
            Case Cache_Read:
              -- Someone wants an S copy.
              Switch me.Dir[msg.Address].State
              Case Inv:
                -- Easy case: send him the data, update Dir.
                me.Dir[msg.Address].State := Shared_Remote;
                me.Dir[msg.Address].Shared_Count := 1;
                me.Dir[msg.Address].Entries[0] := msg.Source;
                Send_Data(msg.Source, n, n, msg.Address, me.Mem[msg.Address]);
                Consume_Message(M_Index);
              Case Shared_Remote:
		-- Others already have S copies.  Send him the data,
		-- Add him to the directory list.
                add_dir_entry(n, msg.Address, msg.Source);
                Send_Data(msg.Source, n, n, msg.Address, me.Mem[msg.Address]);
                Consume_Message(M_Index);
              Case Master_Remote:
		-- Someone has an M copy.  Ask him to update by
		-- sending Fw_Cache_R, then go into Wait_Update state to wait
		-- for the update to come back.
                me.Dir[msg.Address].State := Wait_Update;
                me.Dir[msg.Address].Shared_Count := 1;
                -- Tell remote cache to update modified entry.
                Send_Fw_Cache_R(me.Dir[msg.Address].Entries[0], -- remote owner
                           n,
                           msg.Source,
                           msg.Address);
                add_dir_entry(n, msg.Address, msg.Source);
		me.Dir[msg.Address].State := Wait_Update;
                Consume_Message(M_Index);
	      Case Wait_Inv:  -- !! Block. Wait for the Inv_Ack
	      Case Wait_WB:  -- !! Block. Wait for the Ack_Fw_Cache_Ex or Write_Back.
	      Case Wait_Update:  -- !! Block. Wait for the Update.
	      Else
		Error "Cache_Read received in funny Dir state";
              End; -- switch
-- RAS rules, cont.

	    Case Cache_Read_Ex:
	      Switch me.Dir[msg.Address].State
	      Case Inv: 
		-- add local to the directory in Master_Remote state.
                me.Dir[msg.Address].State := Master_Remote;
	        add_dir_entry(n, msg.Address, msg.Source);
                -- send data back to local
                Send_Data(msg.Source, n, n, msg.Address, me.Mem[msg.Address]);
                Consume_Message(M_Index);	    
	      Case Shared_Remote:
  		-- Send invalidate messages to remotes.
		For i:0..DirMax-1 Do
		  If (i < me.Dir[msg.Address].Shared_Count)
		  Then
		    Send_Invalidate(me.Dir[msg.Address].Entries[i], n, msg.Address);
		  End;
		End;
	        -- Set counter so we know when all Inv_Ack's are in.
		me.Dir[msg.Address].Inv_Count := me.Dir[msg.Address].Shared_Count;
		Send_Data(msg.Source, n, n, msg.Address, me.Mem[msg.Address]);
		-- empty the directory.
	        Clear me.Dir[msg.Address].Entries;
	        me.Dir[msg.Address].Shared_Count := 0;
		-- Add local to Dir.  Wait_Inv = waiting for Inv_Acks.
	        me.Dir[msg.Address].State := Wait_Inv;
                add_dir_entry(n, msg.Address, msg.Source);
                Consume_Message(M_Index);
	      Case Master_Remote:
		Send_Fw_Cache_R_Ex(me.Dir[msg.Address].Entries[0],
			   n,
			   msg.Source,
			   msg.Address);
		-- remote will forward the data.
	        Clear me.Dir[msg.Address].Entries;
	        me.Dir[msg.Address].Shared_Count := 0;
		-- Add local to Dir.  Wait_WB = wait for Ack_Fw_Cache_Ex
	        me.Dir[msg.Address].State := Wait_WB;
	        add_dir_entry(n, msg.Address, msg.Source);
		Consume_Message(M_Index);
	      Case Wait_Inv:  -- Block. Wait for the Inv_Ack
	      Case Wait_WB:  -- Block. Wait for writeback
	      Case Wait_Update:  -- Block. Wait for update

/*	      Case Master_Remote:	
	        -- Treat like Inv_Ack (someone else got entry in M state).
	        Handle_Inv_Ack(n, msg.Address);
		Consume_Message(M_Index);	
*/
	      Else
	        Error "Cache_Read_Ex received in funny Dir state";
	      End; -- switch
-- RAS rules, cont.

	    Case Cache_Promote:
	      Switch me.Dir[msg.Address].State
	      Case Inv: 
		Error "Home in Inv state on Cache_Promote";
	      Case Shared_Remote:
	        If me.Dir[msg.Address].Shared_Count = 1
		Then
		  -- Only entry is the local doing the Cache_Promote.
		  -- No invalidations necessary.
	          me.Dir[msg.Address].State := Master_Remote;
	        Else
  		  -- Send invalidate messages to remotes.
		  -- But not to the local that originated the Cache_Promote!
		  For i:0..DirMax-1 Do
		    If   (i < me.Dir[msg.Address].Shared_Count)
		       & (me.Dir[msg.Address].Entries[i] != msg.Source ) 
		    Then
		      Send_Invalidate(me.Dir[msg.Address].Entries[i], n, msg.Address);
		    End;
		  End;
		  -- Set counter so we know when we have all Inv_Acks back.
		  me.Dir[msg.Address].Inv_Count := me.Dir[msg.Address].Shared_Count - 1;
		  -- Empty out the directory.
		  Clear me.Dir[msg.Address].Entries;
	          me.Dir[msg.Address].Shared_Count := 0;
		  -- Add local to directory. 
	          -- Wait_Inv = Waiting for Inv_Acks.
	          me.Dir[msg.Address].State := Wait_Inv;
                  add_dir_entry(n, msg.Address, msg.Source);
	        End; 
	        -- Acknowledge the Cache_Promote
		Send_Cache_Promote_Ack(msg.Source, n, msg.Address);
                Consume_Message(M_Index);
	      Case Wait_Inv: 
		-- Treat Cache_Promote as Inv_Ack, because it was or will be
		-- aborted (home is handling a Cache_Read_Ex).
	        Handle_Inv_Ack(n, msg.Address);
		Consume_Message(M_Index);	
	      Case Master_Remote:
		-- In this case, Cache_Promote is treated as an Inv_Ack
		-- because a competitor has gotten an exclusive
		-- copy (via Cache_Promote or Cache_Read_Ex).
	        Handle_Inv_Ack(n, msg.Address);
		Consume_Message(M_Index);	
	      Case Wait_Update: -- Block.  Wait for the update.
	      Else
	        Error "Cache_Promote received in funny Dir state";
	      End; -- switch
-- RAS rules, cont.

            Case Write_Back:
	      -- Write back happens when local invalidates
	      -- a writable copy (in response to Invalidate
	      -- or Fw_Cache_R_Ex message).  Or spontaneously.
	      Switch me.Dir[msg.Address].State
	      Case Master_Remote:
		-- Normal case.  Remove the writeback node from
		-- the directory, acknowledge the write_back.
		me.Mem[msg.Address] := msg.Value;
	        remove_dir_entry(n, msg.Address, msg.Source);
		Send_Write_Back_Ack(msg.Source, n, msg.Address);
                Consume_Message(M_Index);
	      Case Wait_WB: 
		-- writeback arrived while awaiting an Ack_Fw_Cache_Ex.
		-- !! Except you have to make sure it's from
		-- the OLD node, not the new one.
		If me.Dir[msg.Address].Entries[0] != msg.Source
		Then
		  -- Writeback from OLD owner.  Pretend it's the Ack_Fw_Cache_Ex.
		  me.Mem[msg.Address] := msg.Value;
		  me.Dir[msg.Address].State := Master_Remote;
		  Consume_Message(M_Index);
	        Else
		  -- write_back from new owner.  Block it.
		End;
	      Case Wait_Update:
		-- Waiting for an update (because of Fw_Cache_R), but
		-- got a writeback instead.
		-- Update memory, remove local from dir, go into
		-- shared state.
		me.Mem[msg.Address] := msg.Value;
		remove_dir_entry(n, msg.Address, msg.Source);
		me.Dir[msg.Address].State := Shared_Remote;	
		-- The Fw_Cache_R we already sent is the Write_Back_Ack
	        Consume_Message(M_Index);
	      Case Wait_Inv:
		-- Similar to Wait_WB case.
		If me.Dir[msg.Address].Entries[0] != msg.Source
	        Then
	          -- Writeback and invalidate passed each other
	          -- Treat Write_Back like Inv_Ack
		  Handle_Inv_Ack(n, msg.Address);
		  Consume_Message(M_Index);
	        Else
		  --  !! Data goes to local, which then decides
		  --  to write_back, before inv_ack has returned
		  --  from remote.  Block the writeback.
		End;  
	      Else
		Error "Writeback received in funny Dir state";
	      End; -- switch	
-- RAS rules, cont.

            Case Update:
	      -- This happens when remote wants to update the main memory
	      -- while keeping a shared copy (in response to Fw_Cache_R).	
	      Switch me.Dir[msg.Address].State
	      Case Wait_Update:
		-- waiting for update in response to our Fw_Cache_R.
		-- update memory.
		me.Mem[msg.Address] := msg.Value;
		me.Dir[msg.Address].State := Shared_Remote;
		Consume_Message(M_Index);
	      Else
		Error "Update received in funny Dir state";
	      End; -- swtich

	    Case Uncache:
	      -- Message sent by node in S state to indicate that
	      -- entry is no longer cached.
	      Switch me.Dir[msg.Address].State 
	      Case Wait_Inv:
	        -- Home just sent an invalidate to everyone
	        -- including local because of a Cache_Read_Ex on another
	        -- node.  Treat the uncache like an Inv_Ack.
		-- Local should treat the invalidate like an
		-- Uncache_Ack.
	        Handle_Inv_Ack(n, msg.Address);
	        Consume_Message(M_Index);
	      Case Wait_Update: -- Block.
		-- We are waiting for an update.
	      Else
	        -- Normal case.
		-- Written this way because we can't have multiple
		-- expressions in a Case.
	        If   (me.Dir[msg.Address].State = Shared_Remote)
		   | (me.Dir[msg.Address].State = Wait_Update)
	        Then
	          remove_dir_entry(n, msg.Address, msg.Source);
		  Send_Uncache_Ack(msg.Source, n, msg.Address);
                  Consume_Message(M_Index);
	        Else
	          Error "Uncache received in funny Dir state";
		End;
	      End; -- switch
-- RAS rules, cont.

	    Case Ack_Fw_Cache_Ex:
	      -- Acknowledge from Fw_Cache_R_Ex
	      Switch me.Dir[msg.Address].State
	      Case Wait_WB:
	        me.Dir[msg.Address].State := Master_Remote;
		Consume_Message(M_Index);
	      Else
	        Error "Ack_Fw_Cache_Ex received in funny Dir state";
	      End; -- switch

	    Case Inv_Ack:
	      Handle_Inv_Ack(n, msg.Address);
              Consume_Message(M_Index);		
	    Else 
	      -- otherwise, it's a message we don't care about. Do nothing.
	    End; -- switch
          End; -- alias
          End; -- rule
    End; -- alias
  End; -- alias
End; -- ruleset
 -- Initialization

Ruleset v: Value Do
Startstate
  For h:Home Do
    For a:Address Do
      Homes[h].Mem[a] := v;
      Homes[h].Dir[a].State := Inv;
      Homes[h].Dir[a].Shared_Count := 0;
      For i:0 .. DirMax-1 Do
	Homes[h].Dir[a].Entries[i] := 0;
      End;
      Homes[h].Dir[a].Inv_Count := 0;
    End;
  End;

  For l:Proc Do
    For h:Home Do
      For a:Address Do
        Procs[l].Cache[h][a].State := I;
        Procs[l].Cache[h][a].Value := v;
        Procs[l].PMMState[h][a] := NOP;
      End;
    End;
  End;

  Net.Count := 0;
  For i:0..NetMax-1 Do
    Clear Net.Ar;
  End;
End; -- startstate
End; -- ruleset

-- Specification
-- This can be simplified a lot by symmetry arguments.

  Invariant
    Forall n:Home Do
      Forall a:Address Do
          (!(Homes[n].Dir[a].State = Inv) | (Homes[n].Dir[a].Shared_Count = 0))
        & (Forall i:0..DirMax-1 Do
                (i >= Homes[n].Dir[a].Shared_Count)
             -> (Homes[n].Dir[a].Entries[i] = 0)
  	   End)
      End
    End;

  Invariant	
    Forall n1:Proc Do
      Forall n2:Proc Do
        Forall home:Home Do 
          Forall addr:Address Do
  	  !(n1 != n2) |
  	  !( (  (Procs[n1].Cache[home][addr].State = M)
  	      & (Procs[n1].PMMState[home][addr] != WB_Pend))
  	    &((  (Procs[n2].Cache[home][addr].State = M)
  	       & (Procs[n2].PMMState[home][addr] != WB_Pend))
  	      | (  (Procs[n2].Cache[home][addr].State = S) 
  		 & (Procs[n2].PMMState[home][addr] != Uncache_Pend)
  		 & !Exists i:0..NetMax-1 Do
  	                (i < Net.Count) 
  	              & (Net.Ar[i].MType = Invalidate)
  		      & (Net.Ar[i].Source = home)
  	              & (Net.Ar[i].Destination = n2)
  	            End
  		  )))
    	  End
        End
      End
    End;

  Invariant
    Forall n1:Proc Do
      Forall home:Home Do
        Forall addr:Address Do
  	  !(  Procs[n1].Cache[home][addr].State = S 
  	    & (Procs[n1].Cache[home][addr].Value != Homes[home].Mem[addr])
  	    & (Procs[n1].PMMState[home][addr] != Uncache_Pend)
  	    & !Exists i:0..NetMax-1 Do
  	          (i < Net.Count) 
  	        & ( (  (Net.Ar[i].MType = Invalidate)
  	             & (Net.Ar[i].Source = home)
  	             & (Net.Ar[i].Destination = n1))
  		   | (  (Net.Ar[i].MType = Update)
  	             & (Net.Ar[i].Destination = home)
  		     & (Net.Ar[i].Value = Procs[n1].Cache[home][addr].Value))
  		   | (  (Net.Ar[i].MType = Write_Back)
  	             & (Net.Ar[i].Destination = home)
  		     & (Net.Ar[i].Value = Procs[n1].Cache[home][addr].Value)))
  	      End)
        End
      End
    End;

/******************

Summary of Result (using release 2.3):

1) 1 Memory Module and 2 processors

   breath-first search
   69 bits (9 bytes) per state
   577 states with a max of about 110 states in queue
   2440 rules fired
   1.96s in sun sparc 2 station

******************/
