/* -*- C++ -*-
 * mu_util_dep.h
 * @(#) part II of the header for Auxiliary routines for the driver 
 * for Murphi verifiers: routines depend on constants declared
 * in the middle of the compiled program.
 *      RULES_IN_WORLD
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
 * Original Author: Ralph Melton
 * Extracted from mu_epilog.inc and mu_prolog.inc
 * by C. Norris Ip
 * Created: 21 April 93
 *
 * Update:
 *
 */ 

/****************************************
  There are 2 groups of declarations:
  1) setofrule and sleepset
  2) rule_matrix
  ****************************************/

/****************************************
  class setofrule and sleepset
  require RULES_IN_WORLD
  ****************************************/

#define BLOCKS_IN_SETOFRULES ( (RULES_IN_WORLD + BITS( BIT_BLOCK ) - 1 ) / \
			  BITS( BIT_BLOCK ))

/* RULES_IN_WORLD gets defined by the generated code. */
/* The extra addition is there so that we round up to the greater block. */
     
class setofrules
{
protected:
  BIT_BLOCK bits[BLOCKS_IN_SETOFRULES];
  unsigned NumRules;   // Uli: unsigned short -> unsigned

  int Index(int i) const { return i / BITS( BIT_BLOCK ); };
  int Shift(int i) const { return i % BITS( BIT_BLOCK ); };
  int Get1(int i) const { return ( bits[ Index(i) ] >> Shift(i) ) & 1; };
  void Set1(int i, int val) /* Set bit i to the low bit of val. */
  {
    if ( (val & 1) != 0 )
      bits[ Index(i) ] |= ( 1 << Shift(i));
    else 
      bits [ Index(i) ] &= ~( 1 << Shift(i));
  };

public:

  // set of rules manipulation
friend setofrules interset(setofrules rs1, setofrules rs2);
friend setofrules different(setofrules rs1, setofrules rs2);
friend bool subset(setofrules rs1, setofrules rs2);

  // conflict set manipulation
friend setofrules conflict(unsigned rule);

  setofrules() 
  : NumRules(0)
  { for (int i=0; i<BLOCKS_IN_SETOFRULES; i++) bits[i]=0;};

  virtual ~setofrules() {};

  bool in(int rule) const { return (bool) Get1(rule); };
  void add(int rule) 
  {
    if (!in(rule))
      {
	Set1(rule,TRUE); 
	NumRules++;
      }
  };
  void remove(int rule)
  { 
    if (in(rule))
      {
	Set1(rule,FALSE); 
	NumRules--;
      }
  }
  bool nonempty() 
  {
    return (NumRules!=0);
  };
  int size()
  { 
    return NumRules;
  };
  void print() 
  { 
    cout << "The set of rules =\t";
    for (int i=0; i<RULES_IN_WORLD; i++) 
      cout << (Get1(i)?1:0) << ',';
    cout << "\n";
  };

  // for simulation 
  void removeall()
  { 
    for (int i=0; i<RULES_IN_WORLD; i++) 
      Set1(i,FALSE);
    NumRules = 0;
  };

  // for simulation 
  void includeall()
  { 
    for (int i=0; i<RULES_IN_WORLD; i++) 
      Set1(i,TRUE);
    NumRules = RULES_IN_WORLD;
  };
  unsigned getnthrule(unsigned rule)
  {
    unsigned r=0;
    int i=0;
    while (1)
      if (Get1(i) && r==rule)
	{ Set1(i,FALSE); NumRules--; return (unsigned) i; }
      else if (Get1(i))
	{ i++; r++; }
      else
	{ i++; } 
  };
};

/****************************************
  1) 8 March 94 Norris Ip:
  merge with the latest rel2.6
****************************************/

/********************
  $Log: mu_util_dep.h,v $
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
