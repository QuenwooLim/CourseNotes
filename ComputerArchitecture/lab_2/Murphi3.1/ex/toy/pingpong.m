--------------------------------------------------------------------------
-- Copyright (C) 1992, 1993 by the Board of Trustees of 		  
-- Leland Stanford Junior University.					  
--									  
-- This description is provided to serve as an example of the use	  
-- of the Murphi description language and verifier, and as a benchmark	  
-- example for other verification efforts.
--	
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

/* pingpong.m
 * Ralph Melton, 8/30/92.
 * Done to test Murphi and
 * because it's an important toy problem that we don't already have.
 */

Type player_t : 0..1;
Var Players : Array[ player_t ] of Record
			       hasball, gotball: boolean
			     End;

Ruleset p : player_t Do
  Alias ping: Players[p];
        pong: Players[ 1 - p ] Do

    Rule "Get ball"
      ping.gotball
    ==>
    Begin
      ping.hasball := true;
      ping.gotball := false;
    End;

    Rule "Keep ball"
      ping.hasball
    ==>
    Begin
    End;

    Rule "Pass ball"
      ping.hasball
    ==>
    begin
      ping.hasball := false;
      pong.gotball := true;
    End;

    Startstate /* Yes, a startstate within an alias and a ruleset. */
    Begin
      ping.hasball := true;
      ping.gotball := false;
      clear pong;
    End;

  End;

End;

Invariant "Only one ball in play."
  Forall p : player_t Do
    !(Players[p].hasball & Players[p].gotball) &
    (Players[p].hasball | Players[p].gotball) ->
    Forall q : player_t Do
      (Players[q].hasball | Players[q].gotball) -> p = q
    End
  End;
  
--Added by Gaurav Uttreja
Invariant "Each player either hasball or gotball."
  Forall p : player_t Do
    (Players[p].hasball = true -> Players[p].gotball = false) & (Players[p].gotball = true -> Players[p].hasball = false)
  End;
  
