/* -*- C++ -*-
 * stmt.h
 * @(#) interfaces for Murphi statements.
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
 * C. Norris Ip 
 * Subject: symmetry extension
 * First version: Jan 25 93 - June 2 93
 * Transfer to Liveness Version:
 * First modified: November 8, 93
 *
 */ 

// do not #include "mu.h"; mu.h includes this.

/********************
  class stmt
  ********************/
class stmt: public TNode
{
 public:
  stmt *next;
  stmt(void)
  :next(NULL) { };
  virtual char *generate_code();
};

/********************
  classes:
  -- assignment
  -- whilestmt
  -- ifstmt
  -- caselist
  -- switchstmt
  -- forstmt
  -- proccall
  -- clearstmt
  -- errorstmt
  -- assertstmt
  -- putstmt
  -- alias
  -- aliasstmt
  -- returnstmt
  ********************/

struct assignment:stmt
{
  designator *target;
  expr *src;
  assignment(designator *target, expr *src);
  virtual char *generate_code();
};


struct whilestmt: stmt
{
  expr *test;
  stmt *body;
  whilestmt(expr *test, stmt *body);
  virtual char *generate_code();
};

struct ifstmt: stmt
{
  expr *test;
  stmt *body;
  stmt *elsecode;
  ifstmt (expr *test, stmt *body, stmt *elsecode = NULL);
  virtual char *generate_code();
};


struct caselist
{
  exprlist *values;
  stmt *body;
  caselist *next;
  caselist(exprlist *values,
	   stmt *body,
	   caselist *next = NULL);
  virtual char *generate_code();
};

struct switchstmt: stmt
{
  expr *switchexpr;
  caselist *cases;
  stmt *elsecode;
  switchstmt(expr *switchexpr,
	     caselist *cases,
	     stmt *elsecode);
  virtual char *generate_code();
};


struct forstmt: stmt
{
  ste *index;
  stmt *body;
  forstmt(ste *index, stmt *body);
  virtual char *generate_code();

  // special for loop restriction for scalarset quantified loop
  // not yet implemented
  void check_sset_loop_restriction();
};

struct proccall: stmt
{
  ste *procedure;
  exprlist *actuals;
  proccall(ste *procedure, exprlist *actuals);
  virtual char *generate_code();
};


struct clearstmt: stmt
{
  designator *target;
  clearstmt( designator *target);
  virtual char *generate_code();
};

struct undefinestmt: stmt
{
  designator *target;
  undefinestmt( designator *target);
  virtual char *generate_code();
};

struct multisetaddstmt: stmt
{
  designator *element;
  designator *target;
  multisetaddstmt(designator *element,  designator *target);
  virtual char *generate_code();
};

class multisettypedecl;

struct multisetremovestmt: stmt
{
  static int num_multisetremove;

  bool matchingchoose;
  ste * index;
  designator *target;
  expr *criterion;
  int multisetremove_num;

  multisetremovestmt(ste * index, designator *target, expr *criterion);
  multisetremovestmt(expr * criterion, designator *target);
  virtual void generate_decl(multisettypedecl * mset);
  virtual void generate_procedure();
  virtual char *generate_code();
};

struct errorstmt: stmt
{
  char *string;
  errorstmt (char *string);
  virtual char *generate_code();
};


struct assertstmt: errorstmt
{
  expr *test;
  assertstmt (expr *test, char *string);
  virtual char *generate_code();
};

struct putstmt: stmt
{
  expr *putexpr;
  char *putstring;
  putstmt (expr *putexpr);
  putstmt (char *putstring);
  virtual char *generate_code();
};


struct alias: public TNode
{
  ste *name;
  designator *target;
  alias *next;
  alias (ste *name, designator *target, alias *next =NULL);
  virtual char *generate_code();
};


struct aliasstmt: stmt
{
  ste *aliases;
  stmt *body;
  aliasstmt ( ste *aliases, stmt *body );
  virtual char *generate_code();
};


struct returnstmt: stmt
{
  expr *retexpr;
  returnstmt( expr *returnexpr = NULL );
  virtual char *generate_code();
};

/********************
  extern variable
  ********************/
extern stmt *nullstmt;

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 7 OCT 94 Norris Ip:
  added multiset add
****************************************/

/********************
 $Log: stmt.h,v $
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
