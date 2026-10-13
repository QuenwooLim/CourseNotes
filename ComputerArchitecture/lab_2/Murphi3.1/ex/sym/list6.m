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
-- File:        list6.m                                                
--                                                                        
-- Content:     distributed list using a single linked list
--                                                                        
-- Date created:         April 94
--                                                                        
--------------------------------------------------------------------------

const
  CellCount: 5;
  NetMax: CellCount+3;

Type
  HeadCell: Scalarset(1);
  RestCells: Scalarset(CellCount-1);
  CellPtr: Union{HeadCell, RestCells};

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
      Ar: Array [0..NetMax-1] of Message;
    End;

-- Network functions

Procedure Send(t: Message_Type;
	       Dst: CellPtr;
	       Src: CellPtr;
	       New: CellPtr;
	       Old: CellPtr);
  If Net.Count = NetMax
  Then
    Error "Network is full";
  Else
    Net.Ar[Net.Count].MType := t;
    Net.Ar[Net.Count].Source := Src;
    Net.Ar[Net.Count].Destination := Dst;
    if (Isundefined(New)) Then
      Undefine Net.Ar[Net.Count].New;
    else
      Net.Ar[Net.Count].New := New;
    endif;   
    if (Isundefined(Old)) Then
      Undefine Net.Ar[Net.Count].Old;
    else
      Net.Ar[Net.Count].Old := Old;
    endif;   
    Net.Count := Net.Count+1;
  Endif;
End;

Procedure Consume_Message(M_Index:0..NetMax-1);
  If M_Index >= Net.Count
  Then
    Error "Invalid Message Index";
  Else 
    For i:0..NetMax-2 Do
      If i >= M_Index
      Then
        Net.Ar[i] := Net.Ar[i+1];
      Endif;
    Endfor;
    -- Clear last entry.
    Undefine Net.Ar[Net.Count-1];
  Endif;
  Net.Count := Net.Count -1;
End;

-- Special send functions
Procedure Send_Add(d:CellPtr; s:CellPtr);
Var UNDEF:CellPtr;
Begin
  Send(Add, d, s, UNDEF, UNDEF);
End;

Procedure Send_Head(d:CellPtr; s:CellPtr; c:CellPtr);
Var UNDEF:CellPtr;
Begin
  Undefine UNDEF;
  Send(Head, d, s, c, UNDEF);
End;

Procedure Send_Pred(d:CellPtr; s:CellPtr);
Var UNDEF:CellPtr;
Begin
  Undefine UNDEF;
  Send(Pred, d, s, UNDEF, UNDEF);
End;

Procedure Send_ChNext(d:CellPtr; s:CellPtr; old:CellPtr; new:CellPtr);
  Send(ChNext, d, s, new, old);
End;

Procedure Send_Del_Ack(d:CellPtr; s:CellPtr);
Var UNDEF:CellPtr;
Begin
  Undefine UNDEF;
  Send(Del_Ack, d, s, UNDEF, UNDEF);
End;

-- Rules
Ruleset headptr:HeadCell Do
  -- These rules spontaneously initiate add/delete requests
  -- from cells other than 0.
  Ruleset c:RestCells Do
    Rule
      Cells[c].State = Normal
      & !Cells[c].IsInList
      & Net.Count < NetMax - 3  -- plenty of room in net.
    ==>
      Send_Add(headptr, c);
      Cells[c].State := Wait_Head;
    End;

    Rule
     Cells[c].State = Normal
     & Cells[c].IsInList
     & Net.Count < NetMax - 3  -- plenty of room in net.
    ==>
     Cells[c].State := Wait_Pred;
    End;
  Endruleset;    
  
  Ruleset M_Index:0..NetMax-1 Do
    Rule
      M_Index < Net.Count -- message is valid.
    ==>
      Alias msg:Net.Ar[M_Index]; n:msg.Destination; me:Cells[msg.Destination] Do

	Switch msg.MType

	-- messages for home node.	
	Case Add:
	  -- Sent from list cell to headptr.
	  -- Tell cell what the old list head is.
	  If me.State = Normal
	  Then
	    Send_Head(msg.Source, headptr, me.Next);
	    me.Next := msg.Source;
	    Consume_Message(M_Index);
	  Else
	    -- Block the message
	  Endif;

	Case Delete:
	  -- This is sent to 0 from a cell that wants
	  -- to delete itself.
	  Consume_Message(M_Index);

	-- Rules for remote cells.	

	Case Head:
	  -- We're adding.  Head message from 0 tells
	  -- us what our new Next should be.
	  Assert me.State = Wait_Head;
	  me.Next := msg.New;
	  me.IsInList := true;
	  me.State := Normal;
	  Consume_Message(M_Index);

	Case Pred:
	  Switch me.State
	  Case Normal:
	    -- Forward the message to Next and forget it.
	    Send_Pred(me.Next, n);
	    Consume_Message(M_Index);

	  Case Wait_Pred:
	    me.State := Wait_Del_Ack;
	    Send_ChNext(msg.Source, n, n, me.Next);
	    Consume_Message(M_Index);

	  Case Wait_Head:
	    -- Block all messages

	  Else
	    Error "Got Pred in funny state";
	  Endswitch;
  
	-- This may go to 0 or other.

	Case ChNext:
	  -- I've been told to change my next pointer.
	  If me.State = Normal | me.State = Wait_Pred
	  Then
	    If me.Next != msg.Old 
	    Then
	      -- forward if another cell got inserted between us and 
	      -- cell to be deleted (we're 0).
	      Send_ChNext(Cells[n].Next, n, msg.Old, msg.New);
	      Consume_Message(M_Index);
	    Else
	      me.Next := msg.New;
	      Send_Del_Ack(msg.Old, n);
	      Send_Pred(msg.New, n);
	      Consume_Message(M_Index);
	    Endif;


	  -- else, block the message.
	  Endif;

	Case Del_Ack:
	  If me.State = Wait_Del_Ack
	  Then
	     me.State := Normal;
	     me.Next := msg.Destination;
	     me.IsInList := false;
	     Consume_Message(M_Index);
	  Else
	    Error "Got unexpected Del_Ack";
	  Endif;

	Endswitch;
      Endalias;
    Endrule;
  Endruleset;
  
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
    Send_Pred(Cells[headptr].Next, headptr);
  End;

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
Endruleset; -- headcell

/*
   Rel2.8S

   3 remote cells: (-sym1)	1489 states, 4956 rules fired in 5.43s.
		   (-nosym)     8893 states, 29584 rules fired in 22.35s.

Release 2.9S (Sparc 20, cabbage.stanford.edu)

   4 remote cells 
        * The size of each state is 448 bits (rounded up to 56 bytes).
     -b * The size of each state is 164 bits (rounded up to 24 bytes).

        23410 states, 99874 rules fired in 21.87s.
     -b 23410 states, 99874 rules fired in 25.38s.
     -c 23410 states, 99874 rules fired in 18.87s.

   3 remote cells

        * The size of each state is 384 bits (rounded up to 48 bytes).
        1489 states, 4956 rules fired in 11.32s.

*/