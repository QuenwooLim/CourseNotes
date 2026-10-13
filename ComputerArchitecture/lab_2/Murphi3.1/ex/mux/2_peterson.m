-------------------------------------------------------------------------
-- Copyright (C) 1992, 1993, 1994, 1995 by the Board of Trustees of 		 
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
-- Engineer:    C. Norris Ip                                             
--                                                                       
-- File:        2_peterson.m                                                   
--                                                                       
-- Content:     Peterson's algorithm (mutual exclusion for 2-processes)  
--                                                                       
-- Summary of result:                                                    
--          1)  No bug is discovered 					 
--          2)  Details of result can be found at the end of this file.  
--                                                                       
-- References: 						       	       	 
-- Peterson, G.L.,  Myths about the mutual exclusion problem,            
-- Information processing letters, Vol 12, No 3, 1981.                   
--                                                                       
-- Date created:         28 Oct 92                                       
-- Last Modified:        17 Feb 93                                       
--                                                                       
-------------------------------------------------------------------------

Const
  N: 2; -- and two only

Type
  pid: Scalarset(N);
  label_t: Enum{L0, -- : non critical section; Q1 := true;
 		L1,  -- : turn := 1;
		L2,  -- : wait until not Q2 or turn = 2
		L3,  -- : critical section
		L4   -- : Q1:= false;
 		};

Var
  P: Array [ pid ] Of label_t;
  Q: Array [ pid ] Of boolean;
  turn: pid;

Ruleset i: pid  Do

  Rule "execute assign Qi true"
    P[i] = L0  ==>
  Begin
    Q[i] := true;
    P[i] := L1; 
  End;

  Rule "execute assign turn i"
    P[i] = L1  ==>
  Begin
    turn := i;
    P[i] := L2; 
  End;

  Ruleset j: pid Do
    Rule "execute wait until"
      P[i] = L2
      & j != i    ==>
    Begin
      If ( !Q[j]
         | turn = j )
      Then 
        P[i] := L3; 
      End; --If
    End;
  End; --Ruleset

  Rule "execute critical section"
    P[i] = L3  ==>
  Begin
    P[i] := L4; 
  End;

  Rule "execute assign Qi false"
    P[i] = L4  ==>
  Begin
    Q[i] := false;
    P[i] := L0;
  End;

  Startstate
  Begin
    For k:pid Do
      P[k] := L0;
      Q[k] := false;
    End; --For
    turn := i;
  End;
  
End; --Ruleset

Invariant "mutual exclusion"
  ! Exists i1: pid Do
    Exists i2: pid Do
      ( i1 != i2
      & P[i1] = L3 -- critical
      & P[i2] = L3 -- critical
      )
    End  --exists
    End; --Exists
 
/******************

Summary of Result (using release 2.3):

1) 2 processes

   breath-first search
   9 bits (2 bytes) per state
   26 states 
   52 rules fired
   0.1s in sun sparc 2 station

******************/
