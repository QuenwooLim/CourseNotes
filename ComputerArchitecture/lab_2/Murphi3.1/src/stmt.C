/* -*- C++ -*-
 * stmt.C
 * @(#) code for statement classes for Murphi.
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
 * (note that check_sset_loop_restriction() is not implemented)
 *
 */ 

#include "mu.h"

/********************
  variable declaration
  ********************/
typedecl *switchtype = NULL;

/********************
  initializers for
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
  -- return stmt
  ********************/

assignment::assignment(designator *target, expr *src)
: target(target), src(src)
{
  Error.CondError(!type_equal(target->gettype(),
			      src->gettype() ),
		  "Type of target of assignment doesn't match type of source.");
  Error.CondError(!target->islvalue(),
		  "Target of assignment is not a variable.");
}

whilestmt::whilestmt(expr *test, stmt *body)
: test(test), body(body)
{
  Error.CondError(!type_equal(test->gettype(), booltype),
		  "Test of while loop must be boolean.");
}

ifstmt::ifstmt(expr *test,
	       stmt *body,
	       stmt *elsecode )
: test(test), body(body), elsecode(elsecode)
{
  Error.CondError(!type_equal( test->gettype(), booltype),
		  "Test of if statement must be boolean.");
}

caselist::caselist(exprlist *values, stmt *body, caselist *next )
:values(values), body(body), next(next)
{
  exprlist *v = values;
  for( ; v != NULL; v = v->next)
    {
      Error.CondError( !v->e->hasvalue(),
		       "Cases in switch statement must be constants.");
      Error.CondError( !type_equal( v->e->gettype(), switchtype),
		       "Type of case does not match type of switch expression.");
    }
}


switchstmt::switchstmt(expr *switchexpr, caselist *cases, stmt *elsecode)
:switchexpr(switchexpr), cases(cases), elsecode(elsecode)
{
}


forstmt::forstmt(ste *index, stmt *body)
:index(index), body(body)
{
  quantdecl *q = (quantdecl *) index->getvalue();

  if (q->gettype()->gettypeclass() == typedecl::Scalarset 
      || q->gettype()->gettypeclass() == typedecl::Union) 
    check_sset_loop_restriction();
}

void forstmt::check_sset_loop_restriction()
{
  // if there is no statement in the loop, it satisfied the restriction.
  if (body==NULL)
    return; 

  Error.Warning("Scalarset is used in loop index.\n\
\tPlease make sure that the iterations are independent.");

  return;

/*
  quantdecl *q = (quantdecl *) index->getvalue();
  analysis_rec *vi, *vj;
  int i, j;
  bool violated = FALSE;

  // check for body statements
  // slow implementation --> n^2 rather than n
  if (q->left == NULL)
    {
      // i.e. < for id: type Do >
      for (i = q->type->getleft(); i<= q->type->getright(); i++)
	for (j = q->type->getleft(); j<= q->type->getright(); j++)
	  if (i!=j)
	    {
	      index->setParaValue(i);
	      vi = body->analyse_var_access();
	      index->setParaValue(j);
	      vj = body->analyse_var_access();
	      if (conflict(vi,vj)) violated = TRUE;
	    }
    }      
  else
    {
      // i.e. < for id = left to right by amount Do >
      for (i = q->left->getvalue(); i<= q->right->getvalue(); i += q->by )
	for (j = q->left->getvalue(); j<= q->right->getvalue(); j += q->by )
	  if (i!=j)	
	    {
	      index->setParaValue(i);
	      vi = body->analyse_var_access();
	      index->setParaValue(j);
	      vj = body->analyse_var_access();
	      if (conflict(vi,vj)) violated = TRUE;
	    }
    }      
  Error.CondError(violated, "The for statement with scalarset breaks symmetry.");
*/

}

proccall::proccall(ste *procedure, exprlist *actuals)
:procedure(procedure), actuals(actuals)
{
#ifdef TEST
  printf("Entering proccall::proccall.\n");
#endif
  if ( procedure != 0 )
    {
      if (!Error.CondError(procedure->getvalue()->getclass() != decl::Proc,
			  "%s is not a procedure.",
			  procedure->getname()->getname() ) )
	{
	  procdecl *p = (procdecl *) procedure->getvalue();
	  matchparams(procedure->getname()->getname(), p->params, actuals);
	}
    }
}

clearstmt::clearstmt( designator *target)
:target(target)
{
  Error.CondError( !target->islvalue(),
		   "Target of CLEAR statement must be a variable.");
  Error.CondError( target->gettype()->HasScalarsetLeaf(),
		   "Target of CLEAR statement must not contain a scalarset variable.");
}

undefinestmt::undefinestmt( designator *target)
:target(target)
{
  Error.CondError( !target->islvalue(),
		   "Target of UNDEFINE statement must be a variable.");
}

multisetaddstmt::multisetaddstmt(designator *element, designator *target)
:element(element), target(target)
{
  Error.CondError(element->gettype() != target->gettype()->getelementtype(),
		  "Multisetadd statement -- 1st argument must be of elementtype of the 2nd argument.");
}

multisetremovestmt::multisetremovestmt(expr *criterion, designator *target)
:index(NULL), target(target), criterion(criterion) 
{
  if (criterion->isdesignator() 
      && criterion->gettype()->gettypeclass() == typedecl::MultiSetID)
    {
      Error.CondError(((multisetidtypedecl *) criterion->gettype())->getparenttype() != target->gettype(),
		      "Multisetremove statement -- 2nd argument does not match 3rd argument");
      matchingchoose = TRUE;
    }
  else
    Error.Error("Internal Error: cannot parse MultiSetRemove.");
}    

multisetremovestmt::multisetremovestmt(ste * index, designator *target, expr *criterion)
:index(index), target(target), criterion(criterion) 
{
  Error.CondError(criterion->gettype() != booltype,
		  "Multisetremove statement -- 3rd argument must be a boolean expression.");
  matchingchoose = FALSE;
  multisetremove_num = num_multisetremove;
  num_multisetremove++;
  ((multisettypedecl *) target->gettype())->addremove(this);
}

int multisetremovestmt::num_multisetremove=0;

errorstmt::errorstmt(char *string)
:string(string != NULL ?
	string :
	tsprintf("%s, line %d.",
		 gFileName,
		 gLineNum))
{
}

assertstmt::assertstmt(expr *test, char *string)
: errorstmt(string != NULL ?
	string :
	tsprintf("%s, line %d.",
		 gFileName,
		 gLineNum)),
test(test)
{
  Error.CondError( !type_equal( test->gettype(), booltype),
		  "ASSERT condition must be a boolean expression.");
}


putstmt::putstmt(expr *putexpr)
: putexpr(putexpr), putstring(NULL)
{
}


putstmt::putstmt(char *putstring)
: putexpr(NULL), putstring(putstring)
{
}


alias::alias(ste *name, designator *target, alias *next )
: name(name), target(target), next(next)
{
}


aliasstmt::aliasstmt(ste *aliases,stmt *body)
: aliases(aliases), body(body)
{
}


returnstmt::returnstmt( expr *returnexpr )
: retexpr(returnexpr)
{
// if (returnexpr==NULL)
// printf("returnstmt::returnstmt called with null.\n");
// else printf("returnstmt::returnstmt called with something.\n");
}

/********************
  variable declaration
  ********************/
stmt *nullstmt = NULL;

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 5 April 94 Norris Ip:
  forbid clearing a scalarset
  * 14 April 94 Norris Ip:
  added warning to sset for loop restriction
  * 7 OCT 94 Norris Ip:
  added multiset add
****************************************/

/********************
 $Log: stmt.C,v $
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
