------------------------------------------------------------------------
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

----------------------------------------------------------------------
-- Filename:	dek.m
-- Version:	Murphi 2.3
-- Content: 	Dekker's algorithm for mutual exclusion.
--		Satisfies all conditions for a correct solution.
-- Last modification:
--              modified 8/25/92 by Ralph Melton for Murphi 2.3
----------------------------------------------------------------------

Type
	ind_t :	0..1;
	label_t : Enum { init, whileOtherLocked, checkTurn, unlock, waitForTurn, lockAndRetry, crit, exitCrit };
	lock_t : Enum { locked, unlocked };

Var
	s :	Array[ ind_t ] Of label_t;
	c :	Array[ ind_t ] Of lock_t;
	turn :	0..1;


Procedure Goto(  p : ind_t;  label : label_t );
Begin
	s[ p ] := label;
End;

Ruleset p : ind_t Do

	Rule "Init"
		s[p] = init
	==>
	Begin
		c[p] := locked;
		Goto( p, whileOtherLocked );
	End;

	Rule "WhileOtherLocked"			-- While c[1-p]=locked
		s[p] = whileOtherLocked
	==>
	Begin
		If c[1-p] = unlocked Then
			Goto( p, crit );
		Else
			Goto( p, checkTurn );
		End;
	End;

	Rule "CheckTurn"
		s[p] = checkTurn
	==>
	Begin
		If turn = 1-p Then
			Goto( p, unlock );
		Else
			Goto( p, whileOtherLocked );
		End;
	End;

	Rule "Unlock"
		s[p] = unlock
	==>
	Begin
		c[p]:=unlocked;
		Goto( p, waitForTurn );
	End;

	Rule "WaitForTurn"			-- Repeat until turn=p
		s[p] = waitForTurn
	==>
	Begin
		If turn != p Then
			Goto( p, lockAndRetry );
		End;
	End;

	Rule "LockAndRetry"
		s[p] = lockAndRetry
	==>
	Begin
		c[p] := locked;
		Goto( p, whileOtherLocked );
	End;

	Rule "Crit"
		s[p] = crit
	==>
	Begin
		Goto( p, exitCrit );
	End;

	Rule "ExitCrit"
		s[p] = exitCrit
	==>
	Begin
		c[p] := unlocked;
		turn := 1-p;
		Goto( p, init );
	End;

End;



Startstate
  Begin
	For p : ind_t Do
		Goto( p, init );
		c[ p ] := unlocked;
	End;
	turn := 0;
End;

Invariant
	!( s[0] = crit & s[1] = crit );
