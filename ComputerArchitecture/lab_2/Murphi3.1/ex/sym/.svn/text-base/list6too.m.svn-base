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
-- File:        list6too.m                                                
--                                                                        
-- Content:     distributed list using a single linked list
--		symmetry in the network is modeled.
--		(note that this way of modelling the network is obsolete;
--		 we can use multiset for the most uptodate murphi;
--		 see newlist6.m)
--                                                                        
-- Date created:         April 94
--                                                                        
--------------------------------------------------------------------------

const
  CellCount: 4;
  NetMax: CellCount+3;

Type
  HeadCell: Scalarset(1);
  RestCells: Scalarset(CellCount-1);
  CellPtr: Union{HeadCell, RestCells};

  NetIndex: Scalarset(NetMax);

  Cell:
    Record
      Next: CellPtr;
      State: Enum {Normal, Wait_Head, Wait_Pred, Wait_Del_Ack};
      IsInList: Boolean;  -- Does it think it's in the list?
			  -- For the invariant.
    End;    

  Message_Type:	enum{Add, Head, Delete, Pred, ChNext, Del_Ack};

  Message:
    Record
      MType: Message_Type;
      Source: CellPtr;
      Destination: CellPtr;
      Old: CellPtr;	-- old .next value	
      New: CellPtr;	-- new .next value
    End;


Var
  Cells: Array [CellPtr] of Cell;

  -- The Net is, abstractly, a bounded set of NetMax or
  -- fewer messages.  It is implemented as an unsorted array,
  -- along with a count of the number of elements in the set.
  -- The array entries from 0..Count-1 are all valid messages.
  -- The entries from Count..MaxNet-1 are all invalid, and are
  -- set to a fixed default value to reduce state space blowup.

  Net:
    Record
      Count: 0..NetMax;
      Ar: Array [NetIndex] of Message;
    End;

-- Network functions

Procedure Send(t: Message_Type;
	       i: NetIndex;
	       Dst: CellPtr;
	       Src: CellPtr;
	       New: CellPtr;
	       Old: CellPtr);
  If Net.Count = NetMax
  Then
    Error "Network is full";
  Else
    Net.Ar[i].MType := t;
    Net.Ar[i].Source := Src;
    Net.Ar[i].Destination := Dst;
    if (Isundefined(New)) Then
      Undefine Net.Ar[i].New;
    else
      Net.Ar[i].New := New;
    endif;   
    if (Isundefined(Old)) Then
      Undefine Net.Ar[i].Old;
    else
      Net.Ar[i].Old := Old;
    endif;   
    Net.Count := Net.Count+1;
  Endif;
End;

Procedure Consume_Message(i:NetIndex);
  Undefine Net.Ar[i];
  Net.Count := Net.Count -1;
End;

-- Special send functions
Procedure Send_Add(i:NetIndex; d:CellPtr; s:CellPtr);
Var UNDEF:CellPtr;
Begin
  Send(Add, i, d, s, UNDEF, UNDEF);
End;

Procedure Send_Head(i:NetIndex; d:CellPtr; s:CellPtr; c:CellPtr);
Var UNDEF:CellPtr;
Begin
  Send(Head, i, d, s, c, UNDEF);
End;

Procedure Send_Pred(i:NetIndex; d:CellPtr; s:CellPtr);
Var UNDEF:CellPtr;
Begin
  Send(Pred, i, d, s, UNDEF, UNDEF);
End;

Procedure Send_ChNext(i:NetIndex; d:CellPtr; s:CellPtr; old:CellPtr; new:CellPtr);
  Send(ChNext, i, d, s, new, old);
End;

Procedure Send_Del_Ack(i:NetIndex; d:CellPtr; s:CellPtr);
Var UNDEF:CellPtr;
Begin
  Send(Del_Ack, i, d, s, UNDEF, UNDEF);
End;

-- Rules

-- These rules spontaneously initiate add/delete requests
-- from cells other than 0.
Ruleset headptr:HeadCell Do
Ruleset c:RestCells Do
Ruleset i:NetIndex Do
  Rule
    Isundefined(Net.Ar[i].MType)
    & Cells[c].State = Normal
    & !Cells[c].IsInList
    & Net.Count < NetMax - 3  -- plenty of room in net.
  ==>
    Send_Add(i, headptr, c);
    Cells[c].State := Wait_Head;
  End;
Endruleset;
Endruleset;
Endruleset;

Ruleset c:RestCells Do
  Rule
    Cells[c].State = Normal
    & Cells[c].IsInList
    & Net.Count < NetMax - 3  -- plenty of room in net.
  ==>
    Cells[c].State := Wait_Pred;
  End;
Endruleset;    

Ruleset headptr:HeadCell Do
Ruleset i:NetIndex Do
Ruleset j:NetIndex Do
  Rule "Add messages for home node"
    !Isundefined(Net.Ar[i].MType)
    & Net.Ar[i].MType = Add
    & Isundefined(Net.Ar[j].MType)
  ==>
  Alias msg:Net.Ar[i]; me:Cells[msg.Destination] Do
    -- Sent from list cell to headptr.
    -- Tell cell what the old list head is.
    If me.State = Normal
    Then
      Send_Head(j, msg.Source, headptr, me.Next);
      me.Next := msg.Source;
      Consume_Message(i);
    Else
      -- Block the message
    Endif;
  Endalias;
  Endrule;
Endruleset;
Endruleset;
Endruleset;

Ruleset i:NetIndex Do
  Rule "Delete messages for home node"
    !Isundefined(Net.Ar[i].MType)
    & Net.Ar[i].MType = Delete
  ==>
    -- This is sent to 0 from a cell that wants
    -- to delete itself.
    Consume_Message(i);
  Endrule;
Endruleset;

Ruleset i:NetIndex Do
  Rule "Head messages for remote cells"
    !Isundefined(Net.Ar[i].MType)
    & Net.Ar[i].MType = Head
  ==>
  Alias msg:Net.Ar[i]; me:Cells[msg.Destination] Do
    -- We're adding.  Head message from 0 tells
    -- us what our new Next should be.
    Assert me.State = Wait_Head;
    me.Next := msg.New;
    me.IsInList := true;
    me.State := Normal;
    Consume_Message(i);
  Endalias;  
  Endrule;
Endruleset;

Ruleset i:NetIndex Do
Ruleset j:NetIndex Do
  Rule "Pred messages for remote cells"
    !Isundefined(Net.Ar[i].MType)
    & Net.Ar[i].MType = Pred
    & Isundefined(Net.Ar[j].MType)
  ==>
  Alias msg:Net.Ar[i]; n:msg.Destination; me:Cells[msg.Destination] Do
    Switch me.State
    Case Normal:
      -- Forward the message to Next and forget it.
      Send_Pred(j, me.Next, n);
      Consume_Message(i);
    Case Wait_Pred:
      me.State := Wait_Del_Ack;
      Send_ChNext(j, msg.Source, n, n, me.Next);
      Consume_Message(i);
    Case Wait_Head:
      -- Block all messages
    Else
      Error "Got Pred in funny state";
    Endswitch;
  Endalias;  
  Endrule;
Endruleset;
Endruleset;

Ruleset i:NetIndex Do
Ruleset j1:NetIndex Do
Ruleset j2:NetIndex Do
  Rule "ChNext messages for any cells"
    !Isundefined(Net.Ar[i].MType)
    & Net.Ar[i].MType = ChNext
    & j1 != j2
    & Isundefined(Net.Ar[j1].MType)
    & Isundefined(Net.Ar[j2].MType)
  ==>
  Alias msg:Net.Ar[i]; n:msg.Destination; me:Cells[msg.Destination] Do
    -- I've been told to change my next pointer.
    If me.State = Normal | me.State = Wait_Pred
    Then
      If me.Next != msg.Old 
      Then
        -- forward if another cell got inserted between us and 
        -- cell to be deleted (we're 0).
        Send_ChNext(j1, Cells[n].Next, n, msg.Old, msg.New);
        Consume_Message(i);
      Else
        me.Next := msg.New;
        Send_Del_Ack(j1, msg.Old, n);
        Send_Pred(j2, msg.New, n);
        Consume_Message(i);
      Endif;
    -- else, block the message.
    Endif;
  Endalias;  
  Endrule;
Endruleset;
Endruleset;
Endruleset;

Ruleset i:NetIndex Do
  Rule "Del_Ack messages for any cell"
    !Isundefined(Net.Ar[i].MType)
    & Net.Ar[i].MType = Del_Ack
  ==>
  Alias msg:Net.Ar[i]; me:Cells[msg.Destination] Do
    If me.State = Wait_Del_Ack
    Then
       me.State := Normal;
       me.Next := msg.Destination;
       me.IsInList := false;
       Consume_Message(i);
    Else
      Error "Got unexpected Del_Ack";
    Endif;
  Endalias;    
  Endrule;
Endruleset;

Ruleset headptr:HeadCell Do
Ruleset i:NetIndex Do
  Startstate
    For c:CellPtr Do
      Cells[c].Next := c;
      Cells[c].State := Normal;
      Cells[c].IsInList := false;
    Endfor;
    Cells[headptr].IsInList := true;
    undefine Net;
    Net.Count := 0;
    -- start the pred token circulating.
    Send_Pred(i, Cells[headptr].Next, headptr);
  End;
Endruleset;
Endruleset;

  Invariant  "Prev is in list"
    Forall c:CellPtr Do Cells[c].State = Normal End ->
    Forall c:CellPtr Do
      Cells[c].IsInList ->
	Exists p:CellPtr Do 
	  Cells[p].Next = c & Cells[p].IsInList &
	  Forall q:CellPtr Do
	    Cells[q].Next = c
	    -> p = q
	  Endforall
	Endexists
    Endforall;

  Invariant "Next is in list"
    Forall c:CellPtr Do Cells[c].State = Normal End ->
    Forall c:CellPtr Do
      Cells[c].IsInList -> 
	 Cells[Cells[c].Next].IsInList
    Endforall;

  Invariant "Dead cells point to themselves"
    Forall c:CellPtr Do
      !Cells[c].IsInList & Cells[c].State = Normal -> Cells[c].Next = c
    Endforall;

/******************

Summary of Result 

* 2 cells (net 5)
	* The size of each state is 72 bits (rounded up to 9 bytes).
	BFS -sym1
	12 states, 65 rules fired in 0.54s.
	BFS -nosym
	120 states, 595 rules fired in 1.10s.

	list6 -nosym
	13 states, 22 rules fired in 0.15s.

* 3 cells (net 6)
	* The size of each state is 90 bits (rounded up to 12 bytes).
	BFS -sym1
	107 states, 880 rules fired in 26.40s.
	BFS -nosym
	7686 states, 51174 rules fired in 109.15s.

	list6 -nosym
	257 states, 633 rules fired in 0.47s.
	list6 -sym1
	129 states, 318 rules fired in 0.39s.

* 4 cells (net 7)
	* The size of each state is 141 bits (rounded up to 18 bytes).
	BFS -sym1
	1069 states, 11550 rules fired in 3640.86s.
	BFS -sym3
	1248 states, 12969 rules fired in 83.28s.

	list6 -nosym
	8893 states, 29584 rules fired in 19.20s.
	list6 -sym1
	1489 states, 4956 rules fired in 6.36s.

* 5 cells (net 8)
	BFS -sym3
	19123 states, 230542 rules fired in 5498.64s.

	list6 -sym1
	23410 states, 99874 rules fired in 103.54s.
	list6 -nosym
	560185 states, 2389561 rules fired in 1288.65s.

Release 2.9S (Sparc 20, cabbage.stanford.edu)

    3 remote cells  
        * The size of each state is 384 bits (rounded up to 48 bytes).
        1077 states, 11622 rules fired in 20.92s.

    4 remote cells
        * The size of each state is 448 bits (rounded up to 56 bytes).
     -b * The size of each state is 164 bits (rounded up to 24 bytes).
        13497 states, 174797 rules fired in 196.71s.
     -b 13804 states, 178708 rules fired in 370.18s.
     -c 13497 states, 174797 rules fired in 192.02s.

******************/
