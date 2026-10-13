/* -*- C++ -*-
 * mu_system.h
 * @(#) header defining the environment for the system
 *      to be simulated or verified
 *
 * Copyright (C) 1992 - 1999 by the Board of Trustees of              
 * Leland Stanford Junior University.
 *
 * License to use, copy, modify, sell and/or distribute this software
 * and its documentation any purpose is hereby granted without royalty,
 * subject to the following terms and conditions:
 *
 * 1.  The above copyright notice and this permission notice must
 * appear in all copies of the software and related documentation.
 *
 * 2.  The name of Stanford University may not be used in advertising or
 * publicity pertaining to distribution of the software without the
 * specific, prior written permission of Stanford.
 *
 * 3.  This software may not be called "Murphi" if it has been modified
 * in any way, without the specific prior written permission of David L.
 * Dill.
 *
 * 4.  THE SOFTWARE IS PROVIDED "AS-IS" AND STANFORD MAKES NO
 * REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED, BY WAY OF EXAMPLE,
 * BUT NOT LIMITATION.  STANFORD MAKES NO REPRESENTATIONS OR WARRANTIES
 * OF MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE
 * USE OF THE SOFTWARE WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS
 * TRADEMARKS OR OTHER RIGHTS. STANFORD SHALL NOT BE LIABLE FOR ANY
 * LIABILITY OR DAMAGES WITH RESPECT TO ANY CLAIM BY LICENSEE OR ANY
 * THIRD PARTY ON ACCOUNT OF, OR ARISING FROM THE LICENSE, OR ANY
 * SUBLICENSE OR USE OF THE SOFTWARE OR ANY SERVICE OR SUPPORT.
 *
 * LICENSEE shall indemnify, hold harmless and defend STANFORD and its
 * trustees, officers, employees, students and agents against any and all
 * claims arising out of the exercise of any rights under this Agreement,
 * including, without limiting the generality of the foregoing, against
 * any damages, losses or liabilities whatsoever with respect to death or
 * injury to person or damage to property arising from or out of the
 * possession, use, or operation of Software or Licensed Program(s) by
 * LICENSEE or its customers.
 *
 * Read the file "license" distributed with these sources, or call
 * Murphi with the -l switch for additional information.
 * 
 */

/* 
 * Original Author: C. Norris Ip 
 * Created: 19 Oct 94
 *
 * Update:
 *
 */ 

/************************************************************/

// Uli: added omission probability calculation & printing

class StateManager
{
  state_set *the_states;  // the set of states found.
  state_queue *queue;     // the queue for active states.
  unsigned long NumStates;

  // Uli: for omission probability calculation
  long statesCurrentLevel;   // number of states in the level that
                             //  is currently expanded
  long statesNextLevel;      // number of states in the next level
  long currentLevel;         // level that is currently expanded
                             //  (startstates: level 0)
  double pno;   // Pr(particular state not omitted)

  double harmonic(double n);   // return harmonic number H_n

public:
  StateManager(bool createqueue, unsigned long NumStates);
  ~StateManager();

  bool Add(state * s, bool valid, bool permanent);
  bool QueueIsEmpty();
  state * QueueTop();
  state * QueueDequeue();
  unsigned NextRuleToTry();   // Uli: unsigned short -> unsigned
  void NextRuleToTry(unsigned r);

  // Uli: routines for omission probability calculation & printing
  void CheckLevel();
  void PrintProb();

  void print_capacity();
  void print_all_states();
  void print_trace(StatePtr p);   // changes by Uli
  void print_trace_aux(StatePtr p);
  unsigned long NumElts();
  unsigned long NumEltsReduced();   // Uli
  unsigned long QueueNumElts();

};

// extern class StartStateGenerator;
class StartStateGenerator;

/************************************************************/
class StartStateManager
{
  static unsigned short numstartstates;
  unsigned short what_startstate; // for info at Error
  StartStateGenerator * generator;
  randomGen random;   // Uli: random number generator
public:
  StartStateManager();
  state * RandomStartState();
  void AllStartStates();
  state * NextStartState();
  state * StartState();
  char * LastStateName();
  char * StateName(StatePtr p);   // changes by Uli
};

// extern class NextStateGenerator;
class NextStateGenerator;

/************************************************************/
class RuleManager
{
  unsigned what_rule;       // for execution and info at Error
  unsigned long rules_fired;
  unsigned long * NumTimesFired; /* array for storing the number
				    of times fired for each rule */
  NextStateGenerator * generator;

  setofrules * EnabledTransition();
  bool AllNextStates(setofrules * fire);
  state * NextState();
  randomGen random;   // Uli: random number generator

  // Vitaly's additions
  int minp; 	// Minimum priority among all rules applicable
		// in the current state
  // End of Vitaly's additions

public:
  RuleManager();
  ~RuleManager();
  state * RandomNextState();
  state * SeqNextState();
  bool AllNextStates();
  void ResetRuleNum();
  void SetRuleNum(unsigned r);
  char * LastRuleName();
  unsigned long NumRulesFired();
  void print_rules_information();
  void print_world_to_state(StatePtr p, bool fullstate);   
    // changes by Uli
};

/************************************************************/
class PropertyManager
{
  unsigned short what_invariant;  // for info at Error
public:
  PropertyManager();
  bool CheckInvariants();
  char * LastInvariantName();
};

/************************************************************/
class SymmetryManager
{
  state_set *debug_sym_the_states;  // the set of states found without sym.
public:
  SymmetryManager();
};

/************************************************************/
class POManager // Partial Order
{
  rule_matrix *conflict_matrix;
public:
  POManager();
};

/************************************************************/
class AlgorithmManager
{
public:
  AlgorithmManager();
  void verify_bfs();
  void verify_dfs();
  void simulate();
};

/************************************************************/
StartStateManager *StartState;  // manager for all startstate related operation
RuleManager *Rules;             // manager for all rule related operation
PropertyManager *Properties;    // manager for all property related operation
StateManager *StateSet;         // manager for all state related information
SymmetryManager *Symmetry;      // manager for all symmetry information
POManager *PO;                  // manager for all symmetry information
ReportManager *Reporter;        // manager for all diagnostic messages
AlgorithmManager *Algorithm;    // manager for all algorithm related issue

Error_handler Error;       // general error handler.
argclass *args;            // the record of the arguments.
state *curstate;        // current state at the beginning of the rule-firing
state *const workingstate = new state;   // Uli: buffer for doing all state
                                         //      manipulation
world_class theworld;          // the set of global variables.
int category;                  // working on startstate, rule or invariant

#ifdef HASHC
TraceFileManager* TraceFile;   // Uli: manager for trace info file
#endif
unsigned long NumCurState;     // Uli: number of the current state for trace 
                               //      info file
 

/********************
  $Log: mu_system.h,v $
  Revision 1.3  1999/01/29 08:28:09  uli
  efficiency improvements for security protocols

  Revision 1.2  1999/01/29 07:49:11  uli
  bugfixes

  Revision 1.4  1996/08/07 18:54:33  ip
  last bug fix on NextRule/SetNextEnabledRule has a bug; fixed this turn

  Revision 1.3  1996/08/07 01:09:49  ip
  Fixed bug on what_rule setting during guard evaluation; otherwise, bad diagnoistic message on undefine error on guard

  Revision 1.2  1996/08/07 00:15:26  ip
  fixed while code generation bug

  Revision 1.1  1996/08/07 00:14:46  ip
  Initial revision

********************/
