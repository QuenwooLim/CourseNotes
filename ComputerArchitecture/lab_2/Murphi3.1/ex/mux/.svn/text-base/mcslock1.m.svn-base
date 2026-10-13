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
-- File:        mcslock1.m                                                
--                                                                        
-- Content:     MCS distributed list-based queuing lock                   
--              with atomic compare_and_swap operations                   
--                                                                        
-- Summary of result:                                                     
--          1)  No bug is discovered 					  
--          2)  Details of result can be found at the end of this file.   
--                                                                        
-- References: 						       	       	  
-- J.M. Mellor-Crummey and M.L. Scott,					  
-- Algorithms for Scalable Synchronization on Shared-Memory Multiprocessors,
-- ACM transactions on computer systems, Vol 9, No 1, 1991.               
--                                                                        
-- Date created:         28 Oct 92                                        
-- Last Modified:        17 Feb 93                                        
--                                                                        
--------------------------------------------------------------------------

Const
  N: 4;

Type
  --  The scalarset is used for symmetry, which is implemented in Murphi 1.5
  --  and not upgraded to Murphi 2.0 yet
  pid: scalarset (N);
  -- pid: 1..N;
  label_t: Enum{L0,  -- : non critical section; 
		     -- : acquire => R->next := nil
		L1,  -- :            predecessor := fetch_and_store( L,R )
		L2,  -- : 	     if (predecessor != nil)
		L3,  -- : 		R->locked := true
		L4,  -- : 		predecessor->next := R
		L5,  -- : 		repeat while R->locked
		L6,  -- : critical section; 
 		     -- : release => if (R->next = nil)
		L7,  -- : 		 	if compare_and_swap (L,R,nil)
		     -- : 				return
		L8,  -- :			repeat while R->next = nil
		L9   -- : R->next->locked := false
 		};

  ptqnode: Record
  	     nil: boolean;
             p: pid;
           End;

  qnode: Record
  	   next: ptqnode;
           locked: boolean;
	 End;
Var
  P: Array [ pid ] Of label_t;
  R: Array [ pid ] Of qnode;
  localpred: Array [ pid ] Of ptqnode;
  lock: ptqnode;

Procedure setNextNil( i:pid);
Begin
    R[i].next.nil := true;
    Undefine R[i].next.p;
End; 

Procedure setNext( i, n:pid);
Begin
    R[i].next.nil := false;
    R[i].next.p := n;
End; 

Procedure setPredNil( i:pid);
Begin
    localpred[i].nil := true;
    Undefine localpred[i].p;
End; 
  
Procedure setPred( i, n:pid);
Begin
    localpred[i].nil := false;
    localpred[i].p := n;
End; 

Procedure setLockNil();
Begin
    lock.nil := true;
    Undefine lock.p;
End; 

Procedure setLock( i:pid);
Begin
    lock.nil := false;
    lock.p := i;
End; 

Ruleset i: pid  Do

  Rule "execute non crit and assign next nil"
    P[i] = L0
  ==>
  Begin
    setNextNil(i);
    P[i] := L1; 
  End;

  Rule "execute assign pred FandS L I"
    P[i] = L1
  ==>
  Begin
    If ( lock.nil )
    Then
      setPredNil(i);
    Else
      setPred(i,lock.p);
    End; --If
    setLock(i);
    P[i] := L2; 
  End;

  Rule "execute if pred nil"
    P[i] = L2
  ==>
  Begin
    If ( localpred[i].nil )
    Then
      P[i] := L6; 
    Else
      P[i] := L3;
    End; --If
  End;

  Rule "execute assign Ilocked true"
    P[i] = L3
  ==>
  Begin
    R[i].locked := true; 
    P[i] := L4; 
  End;

  Rule "execute assign prednext I"
    P[i] = L4
  ==>
  Begin
    setNext(localpred[i].p, i);
    P[i] := L5;
  End;

  Rule "execute repeat while Ilocked"
    P[i] = L5
  ==>
  Begin
    If ( !R[i].locked )
    Then
      P[i] := L6;
    End; --If
  End;

  Rule "execute crit and if Inext nil"
    P[i] = L6
  ==>
  Begin
    If ( R[i].next.nil )
    Then
      P[i] := L7;
    Else
      P[i] := L9;
    End; --If
  End;

  Rule "execute if CandS L I nil" 
    P[i] = L7
  ==>
  Begin
    If ( lock.p = i )
    Then
      setLockNil();
      P[i] := L0;
    Else
      P[i] := L8;
    End; --If
  End;
      
  Rule "execute repeat while Inext nil"
    P[i] = L8
  ==>
  Begin
    If ( !R[i].next.nil )
    Then
      P[i] := L9;
    End; --If
  End;

  Rule "execute assign Inextlocked false"
    P[i] = L9
  ==>
  Begin
    R[R[i].next.p].locked := false;
    P[i] := L0; 
  End;

End; --Ruleset
   
Startstate
Begin 
  For i:pid Do
    P[i] := L0;
    setNextNil(i);
    R[i].locked := false;
    setPredNil(i);
  End; --For
  setLockNil();
End;  
  
Invariant
  ! Exists i1: pid Do
    Exists i2: pid Do
      ( i1 != i2
      & P[i1] = L6 -- critical
      & P[i2] = L6 -- critical
      )
    End  --exists
    End; --Exists
 

/******************

Summary of Result (using release 2.3):

1) 3 processes

   breath-first search
   36 bits (5 bytes) per state
   7597 states with a max of about 374 states in queue
   22791 rules fired
   6.85s in sun sparc 2 station

2) 4 processes

   breath-first search
   47 bits (6 bytes) per state
   554,221 states with a max of about 21,700 states in queue
   2,216,884 rules fired
   767.09s in sun sparc 2 station

2.73S

*  4 processes
   69 bits (rounded up to 9 bytes).

   BFS sym1
   23636 states with 936 max in queue
   94544 rules fired
   87.30s in sun sparc 2 station

   BFS sym3
   25757 states 1061 max in queue.
   103028 rules fired
   87.63s in sun sparc 2 station

   BFS nosym
   554221 states 21728 max in queue.
   2216884 rules fired
   740.03s in sun sparc 2 station

*  3 processes
	* The size of each state is 46 bits (rounded up to 6 bytes).
	-sym1
	1285 states, 3855 rules fired in 3.26s.
	-nosym
	7597 states, 22791 rules fired in 7.82s.


*  2 processes

	* The size of each state is 32 bits (rounded up to 4 bytes).
	-sym1
	81 states, 162 rules fired in 0.20s.
	-nosym
	159 states, 318 rules fired in 0.22s.

gamma2.9S on theforce.stanford.edu

  	3 proc
	-O4 compile 144.0s 2.9Mbytes
	    (20 bytes per states)
	-sym3 1285 states, 3855 rules 2.8s

	4 proc
	-O4 compile 144.1s 2.9Mbytes
	    (28 bytes per states)
	-sym3 23636 states, 94544 rules 45.14s
	-sym2 23636 states, 94544 rules 61.00s
	-sym4 24668 states, 98672 rules 41.41s

	5 proc
	-O4 compile 145.9s 2.9Mbytes
	    (32 bytes per states)

Release 2.9S (Sparc 20, cabbage.stanford.edu)

     4 processes

        * The size of each state is 208 bits (rounded up to 28 bytes).
     -b * The size of each state is 69 bits (rounded up to 12 bytes).

        23643 states, 94572 rules fired in 24.64s.
     -b 23644 states, 94576 rules fired in 27.07s.
     -c 23643 states, 94572 rules fired in 19.36s.

******************/
