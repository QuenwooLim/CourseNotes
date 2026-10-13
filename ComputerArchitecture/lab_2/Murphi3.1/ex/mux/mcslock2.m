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
-- File:        mcslock2.m                                                
--                                                                        
-- Content:     MCS distributed list-based queuing lock                   
--              without atomic compare_and_swap operations                
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
  N: 3;

Type
  --  The scalarset is used for symmetry, which is implemented in Murphi 1.5
  --  and not upgraded to Murphi 2.0 yet
  pid: scalarset (N);
  label_t: Enum{L0,  -- : non critical section; 
		     -- : acquire => R->next := nil
		L1,  -- :            predecessor := fetch_and_store( L,R )
		L2,  -- :  	     if (predecessor != nil)
		L3,  -- : 		R->locked := true
		L4,  -- : 		predecessor->next := R
		L5,  -- : 		repeat while I->locked
		L6,  -- : critical section; 
		     -- : release => if (R->next = nil)
		L7,  -- : 		old_tail = fetch_and_store( L,nil )
 		L8,  -- : 		if (old_tail = R)
		     -- :			return
		L9,  -- :		usuper := fetch_and_store ( L, old_tail)
		L10, -- :		repeat while R->next = nil
		L11, -- :		if (usuper != nil)
		     -- :			usuper->next := R-> next
		     -- : 		else
		     -- :			R->next->locked := false
		L12  -- :	    else
		     -- :		R->next->locked := false
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
  local: Array [ pid ] Of
           Record
	     pred: ptqnode;
	     tail: ptqnode;
 	     usuper: ptqnode;
	   End;		
  L: ptqnode;

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
    local[i].pred.nil := true;
    Undefine local[i].pred.p;
End; 
  
Procedure setPred( i, n:pid);
Begin
    local[i].pred.nil := false;
    local[i].pred.p := n;
End; 

Procedure setTailNil( i:pid);
Begin
    local[i].tail.nil := true;
    Undefine local[i].tail.p;
End; 
  
Procedure setTail( i, n:pid);
Begin
    local[i].tail.nil := false;
    local[i].tail.p := n;
End; 

Procedure setUsuperNil( i:pid);
Begin
    local[i].usuper.nil := true;
    Undefine local[i].usuper.p;
End; 
  
Procedure setUsuper( i, n:pid);
Begin
    local[i].usuper.nil := false;
    local[i].usuper.p := n;
End; 

Procedure setLockNil();
Begin
    L.nil := true;
    Undefine L.p;
End; 

Procedure setLock( i:pid);
Begin
    L.nil := false;
    L.p := i;
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
    If ( L.nil )
    Then
      setPredNil(i);
    Else
      setPred(i,L.p);
    End; --If
    setLock(i);
    P[i] := L2; 
  End;

  Rule "execute if pred nil"
    P[i] = L2
  ==>
  Begin
    If ( local[i].pred.nil )
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
    setNext(local[i].pred.p, i);
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
      P[i] := L12;
    End; --If
  End;

  Rule "execute assign oldtail FandS L nil"
    P[i] = L7
  ==> 
  Begin
    If ( L.nil )
    Then
      setTailNil(i);
    Else
      setTail(i,L.p);
    End; --If
    setLockNil();
    P[i] := L8; 
  End;

  Rule "execute if tail eq I"
    P[i] = L8
  ==>
  Begin
    If ( local[i].tail.p = i )
    Then
      P[i] := L0;
    Else
      P[i] := L9;
    End; --If
  End;
      
  Rule "execute assign usuper FandS L tail"
    P[i] = L9
  ==>
  Var murphilocal: ptqnode;
  Begin
    murphilocal := local[i].tail;
    If ( L.nil )
    Then
      setUsuperNil(i);
    Else
      setUsuper(i,L.p);
    End; --If
    If ( murphilocal.nil )
    Then
      Error "tail cannot be nil at this point.";	
      setLockNil();
    Else
      setLock(murphilocal.p);
    End; --If
    P[i] := L10; 
  End; 
    
  Rule "execute repeat while Inext nil"
    P[i] = L10
  ==>
  Begin
    If ( !R[i].next.nil )
    Then
      P[i] := L11;
    End; --If
  End;

  Rule "execute if usuper neq nil then assign usupernext Inext"
    P[i] = L11
  ==>
  Begin
    If ( !local[i].usuper.nil )
    Then
      R[local[i].usuper.p].next := R[i].next;
    Else
      R[R[i].next.p].locked := false;
    End; --If
    P[i] := L0;
  End;

  Rule "execute assign Inextlocked false"
    P[i] = L12
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
    setTailNil(i);
    setUsuperNil(i);
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

1) 2 processes

   breath-first search
   28 bits (4 bytes) per state
   1098 states with a max of about 60 states in queue
   2196 rules fired
   0.87s in sun sparc 2 station

2.73S

*  3 processes (in sparc 2 station)
	* The size of each state is 70 bits (rounded up to 9 bytes).

	BFS -sym1
	540219 states 15713 max in queue.
	1620657 rules fired
	1303.74s in sun sparc 2 station
	
	BFS -nosym 
	3240032 states 94222 max in queue
	9720096 rules fired
	5818.00s in sun sparc 2 station

	BFS -sym3
	543763 states, 1631289 rules fired in 1322.08s.
	15619 max in queue 

gamma2.9S on theforce.stanford.edu 

  	2 proc
	-O4 compile 169.7s 3.2Mbytes
	    (24 bytes per states)
	-sym3  552 states, 1104 rules 1.88s


	3 proc
	-O4 compile 173.4s 3.2Mbytes
	    (32 bytes per states)
        -sym3 540,219 states, 1620657 rules 1114.48s
        -sym2 540,219 states, 1620657 rules 1064.57s
        -sym4 542,071 states, 1626213 rules 992.39s	      

Release 2.9S (Sparc 20, cabbage.stanford.edu)

   3 processes  
       * The size of each state is 256 bits (rounded up to 32 bytes).
    -b * The size of each state is 256 bits (rounded up to 32 bytes).

        540219 states, 1620657 rules fired in 378.45s.
    -b  540219 states, 1620657 rules fired in 371.69s.
    -c  540219 states, 1620657 rules fired in 387.64s.

******************/


