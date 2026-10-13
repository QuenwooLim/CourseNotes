/* -*- C++ -*-
 * rule.h
 * @(#) interfaces for Murphi rules, startstates, and invariants.
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
 * 
 * Update:
 *
 * Seungjoon Park
 * Subject: liveness syntax
 * Last modified: Novemeber  5, 93
 *
 */ 

/* do not #include "mu.h"; mu.h includes this. */

/********************
  In each class there are 4 sections
  1) initializer --> declaration in rule.C
  2) supporting routines --> declaration in rule.C
  3) code generation --> declaration in cpp_code.C
  ********************/

/********************
  class rule
  ********************/
struct rule:public TNode
{
  // class identifier
  enum rule_class { Simple, Startstate, Invar, Quant, Choose, Alias, Fair, Live};

  // variable
  rule *next;

  // initializer  
  rule( void );

  // supporting routines  
  virtual void set_name( char *name )
  { Error.Error("Internal: rule::set_name(%s)",name); };
  virtual rule_class getclass() const =0;

  // code generation
  virtual char *generate_code();
};

/********************
  extern variable
  ********************/
extern int simplerulenum;

/********************
  class simplerule
  ********************/
struct simplerule: rule
{
  static simplerule * SimpleRuleList;
  simplerule * NextSimpleRule;

  // variable
  char *name;
  char *condname, *rulename; /* internal names for rule and condition. */
  ste *enclosures;	/* enclosing ruleset params and aliases. */
  expr *condition;	/* condition for execution. */
  ste *locals;		/* things defined within the rule. */
  stmt *body;		/* code. */
  bool unfair;          /* if this is unfair rule. */
  int rulenumber;
  int maxrulenumber;
  int size;

  // Vitaly's additions
  int priority;
  // Cardinality of parameters NOT mentioned in the condition
  int indep_card;
  // List of choose parameters NOT mentioned in the condition
  ste *indep_choose;
  // List of choose parameters mentioned in the condition
  ste *dep_choose;
  // End of Vitaly's additions

  // initializer
  simplerule(ste *enclosures,
	     expr *condition,
	     ste *locals,
	     stmt *body,
	     bool unfair,
             int priority);

  // supporting routines
  virtual void set_name( char *name )
  { this->name = name != NULL ? name : tsprintf("Rule %d", simplerulenum++); }
  virtual rule_class getclass() const {return Simple; };

  // code generation
  virtual char *generate_code();

  int CountSize(ste * enclosures);

  void rearrange_enclosures();

  int getsize() { return size; }
};

/********************
  extern variable
  ********************/
extern int startstatenum;

/********************
  class startstate
  ********************/
struct startstate: simplerule
{
  // initializer
  startstate(ste *enclosures,
       	     ste *locals,
	     stmt *body);

  // supporting routines
  virtual void set_name( char *name )
  { this->name = name != NULL ? name : tsprintf("Startstate %d", startstatenum++); }
  virtual rule_class getclass() const {return Startstate; };

  // code generation
  virtual char *generate_code();
};

/********************
  extern variable
  ********************/
extern int invariantnum;

/********************
  class invariant
  ********************/
struct invariant: simplerule
{
  // initializer
  invariant(ste *enclosures, expr *test);

  // supporting routines
  virtual void set_name( char *name )
  { this->name = name != NULL ? name : tsprintf("Invariant %d", invariantnum++);}
  virtual rule_class getclass() const {return Invar; };

  // code generation
  virtual char *generate_code();
};

/********************
  extern variable
  ********************/
extern int fairnessnum;

/********************
  class fairness
  ********************/
struct fairness: simplerule
{
  fairness(ste *enclosures, expr *test);
  virtual void set_name( char *name )
    { this->name = name != NULL ? name : tsprintf("Fairness %d", fairnessnum++);}
  virtual rule_class getclass() const {return Fair; };
  virtual char *generate_code();
};

/********************
  extern variable
  ********************/
extern int livenessnum;

/********************
  class liveness
  ********************/
struct liveness : rule
{
  char *name;
  char *precondname, *leftcondname, *rightcondname;
  ste  *enclosures;     /* enclosing livenessset params and aliases. */
  expr *precondition, *leftcondition, *rightcondition;
  live_type livetype;

  liveness(ste *enclosures, expr *ptest, expr *ltest, expr *rtest,
                live_type livetype);
  virtual void set_name(char *name)
    { this->name = name != NULL ? name : tsprintf("Liveness %d", livenessnum++);}
  virtual rule_class getclass() const {return Live; };
  virtual char *generate_code();
};


/********************
  class quantrule
  ********************/
struct quantrule: rule
{
  ste *quant;
  rule *rules;

  // initializer and supporting routines
  quantrule (ste *param, rule *rules);
  virtual rule_class getclass() const {return Quant; };

  // code generation
  virtual char *generate_code();
};

/********************
  class chooserule for multiset
  ********************/
struct chooserule: rule
{
  ste *index;
  designator *set;
  rule *rules;

  // initializer and supporting routines
  chooserule (ste * index, designator *set, rule *rules);

  virtual rule_class getclass() const {return Choose; };

  // code generation
  virtual char *generate_code();
};

/********************
  class aliasrule
  ********************/
struct aliasrule: rule
{
  ste *aliases;
  rule *rules;

  // initializer and supporting routines
  aliasrule (ste *aliases, rule *rules);
  virtual rule_class getclass() const {return Alias; };

  // code generation
  virtual char *generate_code();
};

/********************
  extern variable
  ********************/
extern simplerule * error_rule;

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 7 OCT 94 Norris Ip:
  added multiset choose rule
****************************************/

/********************
 $Log: rule.h,v $
 Revision 1.3  1999/01/29 08:28:10  uli
 efficiency improvements for security protocols

 Revision 1.2  1999/01/29 07:49:13  uli
 bugfixes

 Revision 1.4  1996/08/07 18:54:00  ip
 last bug fix on NextRule/SetNextEnabledRule has a bug; fixed this turn

 Revision 1.3  1996/08/07 00:59:13  ip
 Fixed bug on what_rule setting during guard evaluation; otherwise, bad diagnoistic message on undefine error on guard

 Revision 1.2  1996/08/06 23:57:39  ip
 fixed while code generation bug

 Revision 1.1  1996/08/06 23:56:55  ip
 Initial revision

 ********************/
