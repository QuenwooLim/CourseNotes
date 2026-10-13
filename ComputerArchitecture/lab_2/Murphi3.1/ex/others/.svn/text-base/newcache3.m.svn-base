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
-- File : newcache3.m
--
-- Cache Coherence Protocol on a incoming buffer Network (message order
-- is NOT preserved). ... using multiset
--
-- David L. Dill at Stanford University.   Initial protocol design
--  by Andreas Nowatzyk, and the initial description was derived
--  from a description by Ken McMillan.  Modified by Norris Ip.
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
  ProcCount: 4;		-- number of processors.
  AddressCount: 1;   	-- number of addresses
  ValueCount: 2;	-- number of data values
  DirMax: ProcCount;	-- Maximum number of dir
			-- entries that can be kept
  NetMax: 2 * ProcCount;-- Channel capacity.

Type
  Home:	Scalarset (HomeCount);
  Proc: Scalarset (ProcCount); 
  Node: Union { Home, Proc};
  -- Kludge! so I don't have to redo all message field types.
  Address: Scalarset (AddressCount);
  Value: Scalarset (ValueCount);
  -- NetIndex: Scalarset (NetMax);

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
--      Destination: Node;
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
	  Inv_Count: 0..(ProcCount-1); -- number of pending invalidates.
	  --- to be fixed
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
  -- The Net is, abstractly, a bounded set of NetMax or
  -- fewer messages.  It is implemented as an unsorted array,
  -- along with a count of the number of elements in the set.
  -- The array entries from 0..Count-1 are all valid messages.
  -- The entries from Count..MaxNet-1 are all invalid, and are
  -- set to a fixed default value to reduce state space blowup.

  Net:    Array [Node] of Multiset [ NetMax ] of Message;
  Homes:  Array [Home] of HomeState;
  Procs:  Array [Proc] of ProcState;

-- Network functions
Procedure Send(M: Message_Type; 
	       Dst:Node; Src:Node; Aux: Node;
	       Addr:Address; Val:Value);
Var msg:Message;
Begin
  Assert (MultiSetCount(i:Net[Dst], true) < NetMax) "Too many messages";
  msg.MType := M;
  msg.Source := Src;
  msg.Aux := Aux;
  msg.Address := Addr;
  msg.Value := Val;
  MultiSetAdd(msg,Net[Dst]);
End;

-- Special send functions
Procedure Send_Cache_Read(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Cache_Read, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Cache_Read_Ex(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Cache_Read_Ex, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Uncache(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Uncache, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Uncache_Ack(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Uncache_Ack, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Data(Dst:Node; Src:Node; Home:Node; Addr:Address; Val:Value);
Begin
  Send(Data, Dst, Src, Home, Addr, Val);
End;

Procedure Send_Fw_Cache_R(Dst:Node; Src:Node; Loc:Node; Addr:Address);
Begin
  Send(Fw_Cache_R, Dst, Src, Loc, Addr, UNDEFINED);
End;

Procedure Send_Fw_Cache_R_Ex(Dst:Node; Src:Node; Loc:Node; Addr:Address);
Begin
  Send(Fw_Cache_R_Ex, Dst, Src, Loc, Addr, UNDEFINED);
End;

Procedure Send_Ack_Fw_Cache_Ex(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Ack_Fw_Cache_Ex, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Invalidate(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Invalidate, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Inv_Ack(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Inv_Ack, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Write_Back(Dst:Node; Src:Node; Addr:Address; Data:Value);
Begin
  Send(Write_Back, Dst, Src, UNDEFINED, Addr, Data);
End;

Procedure Send_Update(Dst:Node; Src:Node; Addr:Address; Data:Value);
Begin
  Send(Update, Dst, Src, UNDEFINED, Addr, Data);
End;

Procedure Send_Cache_Promote(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Cache_Promote, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Cache_Promote_Ack(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Cache_Promote_Ack, Dst, Src, UNDEFINED, Addr, UNDEFINED);
End;

Procedure Send_Write_Back_Ack(Dst:Node; Src:Node; Addr:Address);
Begin
  Send(Write_Back_Ack, Dst, Src, UNDEFINED, Addr, UNDEFINED);
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
      Undefine Homes[h].Dir[a].Entries[Homes[h].Dir[a].Shared_Count-1];
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
      Error "Should be in Wait Inv state";
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
  Undefine Procs[n].Cache[home][addr].Value;
End;

Procedure Handle_Write_Back_Ack( n:Node;  home:Node; 
				 addr:Address);
  If Procs[n].PMMState[home][addr] != WB_Pend
  Then
    Error "PMM state not WB_Pend";
  End;
  Procs[n].PMMState[home][addr] := NOP;
  Procs[n].Cache[home][addr].State := I;
  Undefine Procs[n].Cache[home][addr].Value;
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
--	  & (MultiSetCount(i:Net[h],true) <= NetMax - ProcCount - 1)  -- avoid deadlock
        ==>
          me.PMMState[h][a] := CR_Pend;
          Send_Cache_Read(h, n, a);
	Endrule;

        Rule
           -- spontaneously generate a Coherent Read Invalidate
            (me.Cache[h][a].State = I)
          & (me.PMMState[h][a] = NOP)
--	  & (MultiSetCount(i:Net[h],true) <= NetMax - ProcCount - 1)  -- avoid deadlock
        ==>
          me.PMMState[h][a] := CRE_Pend;
          Send_Cache_Read_Ex(h, n, a);
	Endrule;

        Rule
           -- spontaneously generate a Coherent Invalidate
            (me.Cache[h][a].State = S)
          & (me.PMMState[h][a] = NOP)
--	  & (MultiSetCount(i:Net[h],true) <= NetMax - ProcCount - 1)  -- avoid deadlock
        ==>
          me.PMMState[h][a] := CP_Pend;
          Send_Cache_Promote(h, n, a);
	Endrule;

-- PMM rules, cont.

	Rule
	  -- uncache
	    (me.Cache[h][a].State = S)
	  & (me.PMMState[h][a] = NOP)
--	  & (MultiSetCount(i:Net[h],true) <= NetMax - ProcCount - 1)  -- avoid deadlock
	==>
	  me.PMMState[h][a] := Uncache_Pend; 
	  Send_Uncache(h, n, a);
	Endrule;

	Rule
	  -- writeback due to replacement.
	    (me.Cache[h][a].State = M)
	  & (me.PMMState[h][a] = NOP)
--	  & (MultiSetCount(i:Net[h],true) <= NetMax - ProcCount - 1)  -- avoid deadlock
	==>
	  me.PMMState[h][a] := WB_Pend;
	  Send_Write_Back(h, n, a, me.Cache[h][a].Value);
	Endrule;

	-- When an address is cached and writable, we can
	-- change its value arbitrarily.
	Ruleset v:Value Do
	  Rule
	      (me.Cache[h][a].State = M)
            & (me.PMMState[h][a] = NOP)
	  ==>
	    me.Cache[h][a].Value := v;
	  Endrule;
	Endruleset;

      Endruleset;
    Endruleset;
  Endalias;
Endruleset;
 
-- Receive part of PMM.  Note: "n" and "me" are bound in enclosing
-- ruleset and alias.

-- This rule set nondeterministically chooses a candidate
-- message from the Net.

Ruleset n:Proc Do
Choose M_Index: Net[n] Do

  Rule
      true
    ==>
      Alias msg:Net[n][M_Index] Do
      Alias me:Procs[n] Do
        Switch msg.MType
	  Case Data:
            Switch me.PMMState[msg.Aux][msg.Address]
  	    Case CR_Pend:
	      -- home is msg.Aux.
              me.PMMState[msg.Aux][msg.Address] := NOP;
              me.Cache[msg.Aux][msg.Address].State := S;
              me.Cache[msg.Aux][msg.Address].Value := msg.Value;
	      MultiSetRemove(M_Index, Net[n]);
	    Case CRE_Pend:
              me.PMMState[msg.Aux][msg.Address] := NOP;
              me.Cache[msg.Aux][msg.Address].State := M;
              me.Cache[msg.Aux][msg.Address].Value := msg.Value;
	      MultiSetRemove(M_Index, Net[n]);
	    Else
	      Error "Data received in funny PMMState";
	    Endswitch;

	  Case Invalidate:
	    If   (  me.Cache[msg.Source][msg.Address].State = S 
		  | me.Cache[msg.Source][msg.Address].State = I)
	    Then
	      me.Cache[msg.Source][msg.Address].State := I;
	      Undefine me.Cache[msg.Source][msg.Address].Value;
	      Switch me.PMMState[msg.Source][msg.Address]
	        Case NOP:
		  Send_Inv_Ack(msg.Source, n, msg.Address);
		  MultiSetRemove(M_Index, Net[n]);
	        Case CR_Pend: -- !! Block.  Wait for the data.
	        Case CRE_Pend:
	          -- Abort.  Cache_Read_Ex will be treated as Inv_Ack
	          -- by home.
	          me.PMMState[msg.Source][msg.Address] := NOP;
		  MultiSetRemove(M_Index, Net[n]);
	        Case CP_Pend:
	          -- Abort.  Cache_Promote will be treated as Inv_Ack
	          -- by home.
	          me.PMMState[msg.Source][msg.Address] := NOP;
		  MultiSetRemove(M_Index, Net[n]);
	        Case Uncache_Pend:
	          -- Invalidate and Uncache passed in the network.
	          -- Treat invalidate like an Uncache_Ack.
	          Handle_Uncache_Ack(n, msg.Source, msg.Address);
		  MultiSetRemove(M_Index, Net[n]);
	        Else
	          Error "Got Invalidate with funny PMM state";
	        Endswitch;
	    Else
	      Error "Invalidate message when in M state";	
	    Endif;

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
	      MultiSetRemove(M_Index, Net[n]);
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
	      MultiSetRemove(M_Index, Net[n]);
	    Else
	      Error "Fw_Cache_R received with funny PMMState";	
	    Endswitch;

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
	      Undefine me.Cache[msg.Source][msg.Address].Value;
	      Send_Ack_Fw_Cache_Ex(msg.Source, n, msg.Address);
	      MultiSetRemove(M_Index, Net[n]);
	    Case WB_Pend:
	      -- We are already in the middle of a writeback.
	      -- Treat Fw_Cache_R_Ex like a Write_Back_Ack, but forward data to 
	      -- local.
	      Send_Data(msg.Aux, n, msg.Source, msg.Address,
			me.Cache[msg.Source][msg.Address].Value);
	      Handle_Write_Back_Ack(n, msg.Source, msg.Address);
	      MultiSetRemove(M_Index, Net[n]);
	    -- !! Note that in the next two cases, data can get
	    -- snatched away from a node before the processor
	    -- has a chance to write to it (Fw_Cache_R_Ex is waiting for
	    -- data to arrive).  I hope it's not a problem.
	    Case CP_Pend:    -- We don't have the data yet, so block
	    Case CRE_Pend:   -- these messages until we do.
	    Else
	      Error "Fw_Cache_R_Ex received in funny PMMState";
	    Endswitch;
-- PMM rules, cont.

	  Case Cache_Promote_Ack:
	    -- Our Cache_Promote has succeeded.  Make the copy writeable.
	    If me.PMMState[msg.Source][msg.Address] = CP_Pend
	    Then 
	      me.PMMState[msg.Source][msg.Address] := NOP;
              me.Cache[msg.Source][msg.Address].State := M;
	      MultiSetRemove(M_Index, Net[n]);
	    Else
              Error "Cache_Promote_Ack when PMMstate not CP_Pend";
	    Endif;

	  Case Uncache_Ack:
            -- Home has seen the uncache.  Invalidate the entry.
	    If me.PMMState[msg.Source][msg.Address] = Uncache_Pend
	    Then
	      Handle_Uncache_Ack(n, msg.Source, msg.Address);
	      MultiSetRemove(M_Index, Net[n]);
	    Else
	      Error "Uncache_Ack received in funny PMMState";
	    Endif;

	  Case Write_Back_Ack:
	    If me.PMMState[msg.Source][msg.Address] = WB_Pend
	    Then		
	      Handle_Write_Back_Ack(n, msg.Source, msg.Address);
	      MultiSetRemove(M_Index, Net[n]);
	    Else
	      Error "Write_Back_Ack when PMMState not WB_Pend";	
	    Endif;

	  Else
	    -- do nothing.  Message wasn't for us.
	  Endswitch;
      Endalias;
      Endalias;
  Endrule;
Endchoose;
Endruleset;

-- RAS< rules. "n" and "me" still bound

Ruleset n:Home Do
Choose M_Index: Net[n] Do

  Rule
      true
    ==>
      Alias msg:Net[n][M_Index] Do
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
		MultiSetRemove(M_Index, Net[n]);
              Case Shared_Remote:
		-- Others already have S copies.  Send him the data,
		-- Add him to the directory list.
                add_dir_entry(n, msg.Address, msg.Source);
                Send_Data(msg.Source, n, n, msg.Address, me.Mem[msg.Address]);
		MultiSetRemove(M_Index, Net[n]);
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
		MultiSetRemove(M_Index, Net[n]);
	      Case Wait_Inv:  -- !! Block. Wait for the Inv_Ack
	      Case Wait_WB:  -- !! Block. Wait for the Ack_Fw_Cache_Ex or Write_Back.
	      Case Wait_Update:  -- !! Block. Wait for the Update.
	      Else
		Error "Cache_Read received in funny Dir state";
              Endswitch;
-- RAS rules, cont.

	    Case Cache_Read_Ex:
	      Switch me.Dir[msg.Address].State
	      Case Inv: 
		-- add local to the directory in Master_Remote state.
                me.Dir[msg.Address].State := Master_Remote;
	        add_dir_entry(n, msg.Address, msg.Source);
                -- send data back to local
                Send_Data(msg.Source, n, n, msg.Address, me.Mem[msg.Address]);
		MultiSetRemove(M_Index, Net[n]);

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
	        Undefine me.Dir[msg.Address].Entries;
	        me.Dir[msg.Address].Shared_Count := 0;
		-- Add local to Dir.  Wait_Inv = waiting for Inv_Acks.
	        me.Dir[msg.Address].State := Wait_Inv;
                add_dir_entry(n, msg.Address, msg.Source);
		MultiSetRemove(M_Index, Net[n]);
	      Case Master_Remote:
		Send_Fw_Cache_R_Ex(me.Dir[msg.Address].Entries[0],
			   n,
			   msg.Source,
			   msg.Address);
		-- remote will forward the data.
	        Undefine me.Dir[msg.Address].Entries;
	        me.Dir[msg.Address].Shared_Count := 0;
		-- Add local to Dir.  Wait_WB = wait for Ack_Fw_Cache_Ex
	        me.Dir[msg.Address].State := Wait_WB;
	        add_dir_entry(n, msg.Address, msg.Source);
		MultiSetRemove(M_Index, Net[n]);
	      Case Wait_Inv:  -- Block. Wait for the Inv_Ack
	      Case Wait_WB:  -- Block. Wait for writeback
	      Case Wait_Update:  -- Block. Wait for update

/*	      Case Master_Remote:	
	        -- Treat like Inv_Ack (someone else got entry in M state).
	        Handle_Inv_Ack(n, msg.Address);
		MultiSetRemove(M_Index, Net[n]);
*/
	      Else
	        Error "Cache_Read_Ex received in funny Dir state";
	      Endswitch;
-- RAS rules, cont.

	    Case Cache_Promote:
	      Switch me.Dir[msg.Address].State
	      Case Inv: 
		Error "Home in Inv state on Cache_Promote";
	      Case Shared_Remote:
	        -- do nothing -- moved to another rule

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
		  Undefine me.Dir[msg.Address].Entries;
	          me.Dir[msg.Address].Shared_Count := 0;
		  -- Add local to directory. 
	          -- Wait_Inv = Waiting for Inv_Acks.
	          me.Dir[msg.Address].State := Wait_Inv;
                  add_dir_entry(n, msg.Address, msg.Source);
	        End; 
	        -- Acknowledge the Cache_Promote
		Send_Cache_Promote_Ack(msg.Source, n, msg.Address);
		MultiSetRemove(M_Index, Net[n]);

	      Case Wait_Inv: 
		-- Treat Cache_Promote as Inv_Ack, because it was or will be
		-- aborted (home is handling a Cache_Read_Ex).
	        Handle_Inv_Ack(n, msg.Address);
		MultiSetRemove(M_Index, Net[n]);
	      Case Master_Remote:
		-- In this case, Cache_Promote is treated as an Inv_Ack
		-- because a competitor has gotten an exclusive
		-- copy (via Cache_Promote or Cache_Read_Ex).
	        Handle_Inv_Ack(n, msg.Address);
		MultiSetRemove(M_Index, Net[n]);
	      Case Wait_Update: -- Block.  Wait for the update.
	      Else
	        Error "Cache_Promote received in funny Dir state";
	      Endswitch;
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
		MultiSetRemove(M_Index, Net[n]);
	      Case Wait_WB: 
		-- writeback arrived while awaiting an Ack_Fw_Cache_Ex.
		-- !! Except you have to make sure it's from
		-- the OLD node, not the new one.
		If me.Dir[msg.Address].Entries[0] != msg.Source
		Then
		  -- Writeback from OLD owner.  Pretend it's the Ack_Fw_Cache_Ex.
		  me.Mem[msg.Address] := msg.Value;
		  me.Dir[msg.Address].State := Master_Remote;
  		  MultiSetRemove(M_Index, Net[n]);
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
		MultiSetRemove(M_Index, Net[n]);
	      Case Wait_Inv:
		-- Similar to Wait_WB case.
		If me.Dir[msg.Address].Entries[0] != msg.Source
	        Then
	          -- Writeback and invalidate passed each other
	          -- Treat Write_Back like Inv_Ack
		  Handle_Inv_Ack(n, msg.Address);
		  MultiSetRemove(M_Index, Net[n]);
	        Else
		  --  !! Data goes to local, which then decides
		  --  to write_back, before inv_ack has returned
		  --  from remote.  Block the writeback.
		End;  
	      Else
		Error "Writeback received in funny Dir state";
	      Endswitch;
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
		MultiSetRemove(M_Index, Net[n]);
	      Else
		Error "Update received in funny Dir state";
	      Endswitch;

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
		MultiSetRemove(M_Index, Net[n]);
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
		  MultiSetRemove(M_Index, Net[n]);
	        Else
	          Error "Uncache received in funny Dir state";
		End;
	      Endswitch;

-- RAS rules, cont.

	    Case Ack_Fw_Cache_Ex:
	      -- Acknowledge from Fw_Cache_R_Ex
	      Switch me.Dir[msg.Address].State
	      Case Wait_WB:
	        me.Dir[msg.Address].State := Master_Remote;
		MultiSetRemove(M_Index, Net[n]);
	      Else
	        Error "Ack_Fw_Cache_Ex received in funny Dir state";
	      Endswitch;

	    Case Inv_Ack:
	      Handle_Inv_Ack(n, msg.Address);
	      MultiSetRemove(M_Index, Net[n]);
	    Else 
	      -- otherwise, it's a message we don't care about. Do nothing.
	    Endswitch;

    Endalias;
    Endalias;
  Endrule;
Endchoose;
Endruleset;

 -- Initialization

Ruleset v: Value Do
Startstate
  For h:Home Do
    For a:Address Do
      Homes[h].Mem[a] := v;
      Homes[h].Dir[a].State := Inv;
      Homes[h].Dir[a].Shared_Count := 0;
      For i:0 .. DirMax-1 Do
	Undefine Homes[h].Dir[a].Entries[i];
      Endfor;
      Homes[h].Dir[a].Inv_Count := 0;
    Endfor;
  Endfor;

  For l:Proc Do
    For h:Home Do
      For a:Address Do
        Procs[l].Cache[h][a].State := I;
        Undefine Procs[l].Cache[h][a].Value;
        Procs[l].PMMState[h][a] := NOP;
      Endfor;
    Endfor;
  Endfor;

  Undefine Net;

Endstartstate;
Endruleset;

-- Specification
-- This can be simplified a lot by symmetry arguments.

  Invariant "Unused Dir Entries Undefined"
    Forall n:Home Do
      Forall a:Address Do
          (!(Homes[n].Dir[a].State = Inv) | (Homes[n].Dir[a].Shared_Count = 0))
        & (Forall i:0..DirMax-1 Do
                (i >= Homes[n].Dir[a].Shared_Count)
             -> Isundefined(Homes[n].Dir[a].Entries[i])
  	   Endforall)
      Endforall
    Endforall;

  Invariant "Mutual Exclusion on Dirty Data"	
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
  		 & MultiSetCount(i:Net[n2],
  	              Net[n2][i].MType = Invalidate &
  		      Net[n2][i].Source = home
		      ) = 0
  		  )))
    	  Endforall
        Endforall
      Endforall
    Endforall;

  Invariant "Coherency in Data"
    Forall n1:Proc Do
      Forall home:Home Do
        Forall addr:Address Do
  	  !(  Procs[n1].Cache[home][addr].State = S 
  	    & (Procs[n1].Cache[home][addr].Value != Homes[home].Mem[addr])
  	    & (Procs[n1].PMMState[home][addr] != Uncache_Pend)
  	    & MultiSetCount(i:Net[n1],
  	             ( Net[n1][i].MType = Invalidate
  	             & Net[n1][i].Source = home )) = 0
	    & MultisetCount(i:Net[home],
  		   ( (Net[home][i].MType = Update
  		     & Net[home][i].Value = Procs[n1].Cache[home][addr].Value)
  		   | ( Net[home][i].MType = Write_Back
  		     & Net[home][i].Value = Procs[n1].Cache[home][addr].Value) )
		     ) = 0
		     )
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

2.73S
* 5 processors 1 data value 
   179 bits (23 bytes)

   BFS -sym1
   18962 states with a max of about 2063 states in queue
   155739 rules fired
   186.06s in sun sparc 2 station

   BFS -nosym
   2093231 states 243393 max in queue
   17321850 rules fired
   13807.54s in sun sparc 2 station

* 4 processors 2 data value
	* The size of each state is 167 bits (rounded up to 21 bytes).

        BFS -sym1
	16169 states, 121494 rules fired in 172.51s.
        1411 states in the queue.

	BFS -nosym
	762114 states, 5720176 rules fired in 3473.20s.
	66693 states max in the queue.

---------
multiset network (2.8S)

	4 processors 2 data values
 	* The size of each state is 426 bits (rounded up to 54 bytes).

	BFS -sym1
	12487 states, 57501 rules fired in 247.69s
	1210 states max in the queue.

	5 processors 1 data values

	BFS -sym1
	
----- removed the limit in the number of messages in the network
Release 2.9S (Sparc 20, cabbage.stanford.edu)

  4 processes
        * The size of each state is 2080 bits (rounded up to 260 bytes).
     -b * The size of each state is 891 bits (rounded up to 112 bytes).

        34781 states, 217195 rules fired in 201.15s.
     -b 34781 states, 217203 rules fired in 291.32s.
     -c 34781 states, 217195 rules fired in 163.95s.

******************/
