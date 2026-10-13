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

------------------------------------------------------------------
-- Engineer:    C. Norris Ip                                    --
--                                                              --
-- File:        dpnew.m						--
--                                                              --
--	Dining Philosophers					--
--                                                              --
--      DeadLock retaining options				--
--      1)   use Rule "stop thinking and sit"                   --
--		 Rule "start eating if got both forks"		--
--               Rule "take fork"				--
--      DeadLock removal options				--
--	2) get both forks at the same time			--
--           use Rule "stop thinking and sit"                   --
--		 Rule "take both fork"				--
--      3) sit N-1 philosophers only				--
--         the one used in Hoare's book                         --
--           use Rule "stop thinking and sit if ok"          	--
--		 Rule "start eating if got both forks"		--
--               Rule "take fork"				--
--                                                              --
-- Date Created:        10 DEC 92                               --
-- Last modified:       14 Feb 93                               --
--                                                              --
------------------------------------------------------------------
Const
  N: 3;      -- Number of philosophers.
  option: 1; -- options as shown in the header

Type
  Phil_ID: 0..N-1;
  side: enum {left, right};

Var
  Philosophers: 
    Array [Phil_ID] of
      Record
        Status: Enum { think, take, eat, release };
        Got_Forks: Array [side] of Boolean;
      End;

Ruleset ID : Phil_ID Do
Alias p: Philosophers[ID] Do

  Rule "stop thinking and sit"
    (option = 1 | option = 2)
    &
    p.Status = think
  ==>
  Begin
    p.Status := take;
  End;

  Rule "stop thinking and sit if ok"
    -- only if not all others want to eat already
    option = 3
    &
    p.Status = think
    &
    Exists ID1: Phil_ID Do
      ID1 != ID & Philosophers[ID1].Status = think 
    End
  ==>
  Begin
    p.Status := take;
  End;

  Rule "take left fork"
    (option = 1 | option = 3)
    &
    p.Status = take
    &
    p.Got_Forks[left] = false
  ==>
  Begin
    If !(Philosophers[(ID=N-1)?0:ID+1].Got_Forks[right]) Then
      p.Got_Forks[left] := true;
    Else
      -- cannot get fork, have to wait
    End;
  End;
  
  Rule "take right fork"
    (option = 1 | option = 3)
    &
    p.Status = take
    &
    p.Got_Forks[right] = false
  ==>
  Begin
    If !(Philosophers[(ID=0)?N-1:ID-1].Got_Forks[left]) Then
      p.Got_Forks[right] := true;
    Else
      -- cannot get fork, have to wait
    End;
  End;

  Rule "start eating if got both forks"
    (option = 1 | option = 3)
    &
    Forall s: side Do
      p.Got_Forks[s]
    End
  ==>
  Begin
    p.Status := eat;
  End;

  Rule "take both fork"
    option = 2
    &
    p.Status = take
    &
    Forall s: side Do
      !p.Got_Forks[s]
    End
  ==>
  Begin 
    If ( !(Philosophers[(ID=0)?N-1:ID-1].Got_Forks[left]) 
       & !(Philosophers[(ID=N-1)?0:ID+1].Got_Forks[right]) )
    Then
      For s: side Do
        p.Got_Forks[s] := true;
      End;
      p.Status := eat;
    Else
      -- cannot get fork, have to wait 
    End;
  End;

  Rule "finished eating"
    p.Status = eat
  ==>
  Begin
    p.Status := release;
  End;

  Rule "release left fork"
    p.Status = release
  ==>
  Begin
    p.Got_Forks[left] := false;
  End;

  Rule "release right fork"
    p.Status = release
  ==>
  Begin
    p.Got_Forks[right] := false;
  End;

  Rule "start thinking"
    p.Status = release
    & 
    Forall s: side Do
      p.Got_Forks[s] = false
    End
  ==>
  Begin
    p.Status := think;
  End;

Endalias;
Endruleset;

/*
	Startstate and invariant
*/

Startstate
Begin
  For ID: Phil_ID Do
    Philosophers[ID].Status := think;
    For s: side Do
      Philosophers[ID].Got_Forks[s] := false;
    End;
  End;
End;

Invariant "All eating philosophers have both forks available."
  Forall ID : Phil_ID Do
    Philosophers[ID].Status = eat
    ->
    Forall s: side Do
      Philosophers[ID].Got_Forks[s] 
    End
  End
  ;

Invariant "A fork is used by upto one philosophers only"
  Forall ID: Phil_ID Do
    Philosophers[ID].Got_Forks[left]
    ->
    !Philosophers[(ID=N-1)?0:ID+1].Got_Forks[right]
  End
  &   
  Forall ID: Phil_ID Do
    Philosophers[ID].Got_Forks[right]
    ->
    !Philosophers[(ID=0)?N-1:ID-1].Got_Forks[left]
  End
  ;

/*
-- option to check the result of violating invariant rather than deadlock detection. 
Invariant "deadlock!"
  !Forall ID : Phil_ID Do
    Philosophers[ID].Status = take
    &
    Philosophers[ID].Got_Forks[left] = true
    &
    Philosophers[ID].Got_Forks[right] = false
  End
  ;
*/