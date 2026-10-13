-------------------------------------------------------------------------
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

----------------------------------------------------------------------
-- Filename:	sets.m
-- Content:	Representation of Sets in Murphi
-- Engineer:	Andreas J. Drexler, September 1991
-- Modified:    1) Ralph Melton 8/25/92, for Murphi 2.3
--              2) Norris Ip 3/4/93.
--		   Complete transforming to Murphi 2.3 and
--		   Corrected incorrect specification of a set
--		   by adding 0 as invalid entry of a set	
--Result: 	Invariant limited the insertion and deletion to be within 5
----------------------------------------------------------------------

Const
	MAX_SET_SIZE: 12;
	MAX_OPS: 1000;
	NUM_ELT: 4;

Type
	elt_t: 1..NUM_ELT;
	entry_t: 0..NUM_ELT; 
	-- 1..NUM_ELT corresponds to the elements and 0 implies invalid;

	setInd_t: 0..MAX_SET_SIZE-1;

	set_t: Record
		a: Array [ setInd_t ] Of entry_t;
		n: 0..MAX_SET_SIZE;		-- Number of elts in set.
		numIns: 0..MAX_OPS;		-- Number of insertions.
		numDel: 0..MAX_OPS;		-- Number of deletions.
	End;

Var
	s: set_t;


/*
	Procedures
*/

Procedure Sort( Var s: set_t );
Var tmpElt : entry_t;
Begin
	For i: 0..MAX_SET_SIZE-2 Do
		For j := i + 1 to  MAX_SET_SIZE-1 Do
			If i < j & s.a[i] < s.a[j] then
				tmpElt := s.a[i];
				s.a[i] := s.a[j];
				s.a[j] := tmpElt;
				tmpElt := 0;
			End;
		End;
	End;
End;

Procedure Insert( Var s: set_t;   e: elt_t );
-- Assumes that there is room for the element to be inserted.
Begin
	s.a[ s.n ] := e;
	s.n := s.n + 1;
	s.numIns := s.numIns + 1;
	Sort( s );
End;

Procedure Delete( Var s: set_t;  e:elt_t );
-- Deletes elt `e' from set `s'.
Begin
	For i: setInd_t Do
		If s.a[ i ] = e then
			s.a[ i ] := 0;
			s.n := s.n - 1;
			s.numDel := s.numDel + 1;
		End;
	End;
	Sort( s );
End;


/*
	Rules
*/

Ruleset e : elt_t Do
	Rule "Insert elt"
		s.n < MAX_SET_SIZE - 1  &  ! Exists i: setInd_t Do s.a[i] = e End
	==>
	Begin
		Insert( s, e );
	End;

	Rule "Delete last inserted elt"
		s.n > 0
	==>
	Begin
		Delete( s, e );
	End;

End;


/*
	Startstate and invariant
*/

Startstate
  Begin
	s.n := 0;			-- Start with the empty set.
	For i : setInd_t Do
		s.a[ i ] := 0;
	End;
	s.numIns := 0;
	s.numDel := 0;
  End;

Invariant
	s.numIns + s.numDel < 5;
