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
-------------------------------------------------------------------------

----------------------------------------------------------------------
-- Filename:	dp4.m
-- Content:	Dining Philosophers
-- 		Solution using an imitation of monitors.
-- Version:	Murphi 2.3
-- Engineer:	Andreas J. Drexler, June 1991
-- Last modified:
--	Modified 8/25/92 by Ralph Melton to accomodate Murphi 2.0.
----------------------------------------------------------------------

Const
	N: 4;			-- Number of philosophers.

Type
	ind_t:	0..N-1;
	label_t: Enum { think, take, eat, release };

Var
	label: Array[ ind_t ] Of label_t;
	forksAvail: Array[ ind_t ] Of 0..2;



/*
	Procedures
*/

Function Left(  i : ind_t ): ind_t;
Begin
	If i = 0 then
		return N - 1;
	Else
		return i - 1;
	End;
End;


Function Right(  i : ind_t ): ind_t;
Begin
	If i = N - 1 then
		return 0;
	Else
		return i + 1;
	End;
End;

Procedure Goto(  p : ind_t;  lab : label_t );
Begin
	label[ p ] := lab;
End;


/*
	Rules
*/

Ruleset p : ind_t Do
Alias lp : label[ p ] Do

	Rule "Continue thinking"
		lp = think
	==>
	Begin
	End;

	Rule "Done thinking"
		lp = think
	==>
	Begin
		Goto( p, take );
	End;

	Alias fap : forksAvail[ p ] Do
		Rule "Try to take forks"
			lp =take
		==>
		Begin
			If fap = 2 then
				forksAvail[Left(p)] := forksAvail[Left(p)] - 1;
				forksAvail[Right(p)] := forksAvail[Right(p)] -1;
				Goto( p, eat );
			End;		-- Otherwise stay in this rule.
		End;
	End;


	Rule "Continue eating"
		lp = eat
	==>
	Begin
	End;

	Rule "Finish eating"
		lp = eat
	==>
	Begin
		Goto( p, release );
	End;

	Rule "Release"
		lp = release
	==>
	Begin
		forksAvail[ Left(p) ] := forksAvail[ Left(p) ] + 1;
		forksAvail[ Right(p) ] := forksAvail[ Right(p) ] + 1;
		Goto( p, think );
	End;

End;
End;



/*
	Startstate and invariant
*/

Startstate
begin
	For p : ind_t Do
		forksAvail[ p ] := 2;
		Goto( p, think );
	End;
End;

Invariant		-- "All eating philosophers have both forks available."
        Forall p : ind_t Do
		label[p] = eat  ->  forksAvail[p] = 2
	End;
