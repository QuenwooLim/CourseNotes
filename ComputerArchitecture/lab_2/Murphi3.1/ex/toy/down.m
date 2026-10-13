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
----------------------------------------------------------------------
----------------------------------------------------------------------
-- Filename: 	down.m
-- Content:	Down Benchmark
-- Engineer:	Andreas J. Drexler, 1991
-- Modified:
--		8/25/92 by Ralph Melton for Murphi 2.0.
-- Result: 	violate invariant when all data is zero
----------------------------------------------------------------------

Const
	INITIAL_VAL: 5;
	L: 1;
	H: 6;
	MAX_VAL: (H-L+1) * INITIAL_VAL;

Type
	ind_t: L..H;
	val_t: 0..INITIAL_VAL;
	arr_t: Array[ ind_t ] Of val_t;
	sum_t: 0 .. MAX_VAL;

Var
	a: arr_t;

Procedure Decrement( Var dec : val_t );
Begin
	If dec >= 1 then
		dec := dec - 1;
	End;
End;

Function Sum(  a : arr_t ): sum_t;
Var sum : sum_t;
Begin
	sum := 0;
	For i : ind_t Do
		sum := sum + a[i];
	End;
	return sum;
End;


Ruleset i : L .. H Do
	Rule
		a[i] > 0
	==>
	Begin
		Decrement( a[i] );
		If i+1 < H then
			If a[i+1] > 0 then
				Decrement( a[i+1] );
			End;
		End;
	End;
End;

Startstate
  Begin
	For i : ind_t Do
		a[ i ] := INITIAL_VAL;
	End;
  End;

Invariant "Positive sum"
	Sum(a) > 0;

