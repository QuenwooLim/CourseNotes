/* -*- C++ -*-
 * mu_verifier.h
 * @(#) common information used by the generated code of Murphi verifiers
 *      and supporting codes. 
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
 *
 */

/* 
 * Original Author: Ralph Melton
 * Extracted from mu_epilog.inc and mu_prolog.inc
 * by C. Norris Ip
 * Created: 21 April 93
 *
 * Update:
 *
 */ 

/****************************************
  Include library
 ****************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
//#include <values.h> /* for BITSPERBYTE and BITS() */
// Uli: values.h is obsolete, replaced by limits.h
#include <limits.h>
#include <new>    /* for new_handler stuff. */
#include <signal.h> /* To trap division by zero. */
#include <assert.h>

/****************************************   // added by Uli
  C Objects
 ****************************************/

extern "C" int remove(const char *filename);
extern "C" long sysconf(int name) throw();


/****************************************
  General Constants
 ****************************************/

#define FALSE 0
#define TRUE 1
#define mu_false 0
#define mu_true 1
#define BIT_BLOCK unsigned char /* type of an element for a bit vector. */

// Uli: replace the BITS() macro from values.h
#ifndef BITS
#define BITS(type) ((int)sizeof (type) * CHAR_BIT)
#endif

//typedef char bool;
typedef void (*proc) (void);
typedef bool (*boolfunc) (void);


struct rulerec {
  char *name;
  boolfunc condition;
  proc code;
  bool unfair;
};

#ifndef ALIGN
typedef struct {
  int longoffset;   /* offset in the state vector, on long boundary */
  unsigned int mask1, mask2;  /* masks for fast access */
  int shift1, shift2;         /* shift values for fast access */
} position;
#endif

/****************************************
  For Liveness
 ****************************************/
enum space { PRE, LEFT, RIGHT };
enum live_type { E, AE, EA, U, AIE, AIU };
struct liverec {
  char *name;
  boolfunc condition_pre;
  boolfunc condition_left;
  boolfunc condition_right;
  live_type livetype;
};


/****************************************
  class declaration
 ****************************************/

// declared in mu_util.h
class mu__int;      /* a base for a value */
class mu_boolean;   /* a base for a boolean */
class world_class;  /* class for variables in expanded state */
class state_queue;  /* class for search queue for bfs */
class state_stack;  /* class for search queue for dfs */
class state_set;    /* hash table for storing state examined */

// declared in mu_io.h
class Error_handler;/* class for error handling */
class argclass;     /* class for handling command line argument */

// declared in here
class dynBitVec;    /* class for bit vector , state */

// declared in mu_dep.h
class state;        /* a state in the state graph -- a bit vector */
class state_L;      /* a state in the state graph for liveness -- with more info */
class setofrules;   /* class to store a set of rules */
class sleepset;     /* sleepset for partial order reduction technique */
class rule_matrix;  /* class for square matrix of dimension numrules */

/****************************************
  external variables
 ****************************************/

// from murphi code
// extern const rulerec rules[];
extern const unsigned numrules;   // Uli: unsigned short -> unsigned
extern const rulerec startstates[];
extern const unsigned short numstartstates;
extern const rulerec invariants[];
extern const unsigned short numinvariants;
extern const rulerec fairnesses[];
extern const unsigned short numfairnesses;
extern const liverec livenesses[];
extern const unsigned short numlivenesses;

#define STARTSTATE 0
#define CONDITION 1
#define RULE 2
#define INVARIANT 3

class StartStateManager;
class RuleManager;
class PropertyManager;
class StateManager;
class SymmetryManager;
class POManager;
class ReportManager;
class AlgorithmManager;

extern StartStateManager *StartState;  // manager for all startstate related operation
extern RuleManager *Rules;             // manager for all rule related operation
extern PropertyManager *Properties;    // manager for all property related operation
extern StateManager *StateSet;         // manager for all state related information
extern SymmetryManager *Symmetry;      // manager for all symmetry information
extern POManager *PO;                  // manager for all symmetry information
extern ReportManager *Reporter;        // manager for all diagnostic messages
extern AlgorithmManager *Algorithm;    // manager for all algorithm related issue

extern Error_handler Error;       // general error handler.
extern argclass *args;            // the record of the arguments.
extern state *curstate;        // current state at the beginning of the rule-firing
extern state *const workingstate;   // Uli: this pointer points to a working-
                                    //      buffer
extern world_class theworld;          // the set of global variables.
extern int category;                  // working on startstate, rule or invariant


/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 12 April 94 Norris Ip:
  add information about error in the condition of the rules
  category = CONDITION
****************************************/

/********************
  $Log: mu_verifier.h,v $
  Revision 1.2  1999/01/29 07:49:11  uli
  bugfixes

  Revision 1.4  1996/08/07 18:54:33  ip
  last bug fix on NextRule/SetNextEnabledRule has a bug; fixed this turn

  Revision 1.3  1996/08/07 01:00:18  ip
  Fixed bug on what_rule setting during guard evaluation; otherwise, bad diagnoistic message on undefine error on guard

  Revision 1.2  1996/08/07 00:15:26  ip
  fixed while code generation bug

  Revision 1.1  1996/08/07 00:14:46  ip
  Initial revision

********************/
