--------------------------------------------------------------------------------
-- Copyright (C) 1992, 1993 by the Board of Trustees of 		      --	
-- Leland Stanford Junior University.					      --	
--									      --
-- This description is provided to serve as an example of the use	      --
-- of the Murphi description language and verifier, and as a benchmark	      --
-- example for other verification efforts.				      --	
--									      --	
-- License to use, copy, modify, sell and/or distribute this description      --
-- and its documentation any purpose is hereby granted without royalty,       --
-- subject to the following terms and conditions, provided		      --	
--									      --	
-- 1.  The above copyright notice and this permission notice must	      --	
-- appear in all copies of this description.				      --
-- 									      --
-- 2.  The Murphi group at Stanford University must be acknowledged	      --
-- in any publication describing work that makes use of this example. 	      --
-- 									      --	
-- Nobody vouches for the accuracy or usefulness of this description	      --
-- for any purpose.							      --
--------------------------------------------------------------------------------

----------------------------------------------------------------------
-- Filename: 	sort5.m
-- Content:	sorting by random swaping
-- Version:	Murphi 2.3
-- Engineer:	Update to Murphi 2.3 by Ralph Melton, 1993.
-- Result:	The program violates invariant when the data is sorted
----------------------------------------------------------------------

Const
	N: 5;

Type
	ind_t:	0..N-1;
	val_t:	0..N-1;
	arr_t:	Array[ ind_t ] Of val_t;
	bool_t: Boolean;

Var
	i:	ind_t;
	j:	ind_t;
	a:	arr_t;

Procedure IncrementMod( Var v : ind_t;  m : ind_t );
Begin
	If v = m then
		v := 0;
        Else
  		v := v + 1;
	End;
End;

Procedure Increment( Var v : ind_t );
Begin
	IncrementMod( v, N-1 );
End;

Procedure Swap( Var a : arr_t );
Var tmp : val_t;
Begin
	tmp := a[i];
	a[i] := a[j];
	a[j] := tmp;
	tmp := 0;
End;


Rule "i"
	true
==>
Begin
	IncrementMod( i, N-1 );
End;

Rule "j"
	true
==>
Begin
	Increment( j );
End;

Rule "swap"
	i < j  &  a[i] > a[j]
==>
Begin
	Swap( a );
End;


Startstate
Begin
	Clear i;
	Clear j;
	For f : ind_t Do
		a[ f ] := N-1 - f;
	End;
End;

Invariant
	Exists i : 0..N-2 Do a[i] > a[i+1] End;
