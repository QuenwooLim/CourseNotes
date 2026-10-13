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
-- Filename: 	arbiter.m
-- Content:	Hardware arbiter, travelling token, synchronous control
--		A design that can Deadlock.
-- Version:	Murphi 2.3
-- Engineer:	Nicolas Halbwachs, 1992
----------------------------------------------------------------------

const n : 4;   -- Number of units

type reg : Array[0..n-1] of boolean;

var 
   req : reg;                    -- req[u] == u requests the resource
   use : reg;                    -- use[u] == u uses the resource
   grant : reg;                  -- grant[u] == access is granted to u
   tk : Array[0..n] of boolean;  -- tk[u] == u receives token
   flop : reg;                   -- flop[u] == req[u] at last tk[u]
   ntk : reg;                    -- ntk[u] == u has the token
   Req_b : boolean;                -- at least one unit requests the resource
   Use_b : boolean;                -- at least one unit uses the resource
   p_req : reg;                   -- the value of "req" at last arbitration request

procedure NewToken();
begin
   tk[0] := true; -- a new token is emitted
   -- the current requests are recorded for checking the priority rule
   for v : 0..n-1 do
      p_req[v] := req[v];
   end;
end;

ruleset u : 0..n-1 do  -- rules of units
   rule "start requesting"
         !req[u] & !use[u] ==>  
                 begin
                 req[u] := true; 
                 if (!Req_b & !Use_b) then NewToken() end;
                 Req_b := true;
                 end;
   rule "start using"
          grant[u] ==> 
                 begin
                 assert !Use_b; -- exclusive access
                 /* Doesn't work, why??
                 assert forall v : 0..u-1 do !p_req[v] end; -- priority rule
                 */
                 assert req[u]; -- reactivity
                 use[u] := true; req[u] := false; Use_b := true; grant[u] := false;
                 Req_b := exists v:0..n-1 do req[v] end;
                 end;
   rule "end using"
          use[u] ==>
                 begin
                 use[u] := false; Use_b := exists v:0..n-1 do use[v] end;
                 assert !Use_b; -- exclusive access
                 if !Req_b & !Use_b then tk[0] := true; end;
                 end;
end;

ruleset u : 0..n-1 do -- rules of arbiters
   rule "D-flip-flop"
         tk[u] ==> 
                 begin
                 flop[u] := req[u]; tk[u] := false; ntk[u] := true;
                 end;
   rule "access granting"
         ntk[u] & flop[u] ==> 
                 begin
                 grant[u] := true; ntk[u] := false;
                 end;
   rule "token passing"
         ntk[u] & !flop[u] ==> 
                 begin
                 tk[u+1] := true; ntk[u] := false;
                 end;
end;


startstate
   begin
   for u  : 0..n-1 do
       req[u] := false; use[u] := false; grant[u] := false;
       flop[u] := false; tk[u] := false; ntk[u] := false; 
       p_req[u] := false;
   end;
   Req_b := false; Use_b := false; tk[n] := false;
   end;

invariant  " no token lost " tk[n] = false;


