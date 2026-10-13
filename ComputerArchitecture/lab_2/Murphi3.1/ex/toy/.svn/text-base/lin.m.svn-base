----------------------------------------------------------------------------
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

-------------------------------------------------
-- Filename: 	lin.m
-- Content:  	Linear state space.
-- Engineer: 	Andreas Drexler, 1991
-- Modified: 	Ralph Melton 8/25/92 for Murphi 2.0.
-- Result:  	a linear state space of size 100 is obtained
--		before the invariant is violated
-------------------------------------------------

Const
	VAL_LIM: 1000;

Type
	val_t: 0..VAL_LIM;

Var
	v: val_t;


Rule "incBy1"
	v <= VAL_LIM - 1
==>
Begin
	v := v + 1;
End;

Rule "incBy2"
	v <= 50
==>
Begin
	v := v + 2;
End;

Startstate
Begin
	v := 1;
End;

Invariant
	v <= 100;
