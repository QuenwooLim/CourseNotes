/*  -*- C++ -*-
 * rule.C
 * @(#) code for rules, startstates, and invariants for the Murphi compiler.
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

#include "mu.h"
//#include <stream.h>

/********************
  class rule
  ********************/
rule::rule( void)
: next(NULL)
{
}

/********************
  class simplerule and variable simplerulenum
  ********************/
int simplerulenum = 0;
simplerule::simplerule(ste *enclosures,
		       expr *condition,
		       ste *locals,
		       stmt *body,
		       bool unfair,
                       int priority)
:rule(), name( name ), enclosures(enclosures),  condition(condition),
 locals(locals), body(body), condname(NULL), rulename(NULL), unfair(unfair),
 priority(priority)
{
  if ( condition != NULL && body != NULL )
    {
      Error.CondError(condition->has_side_effects(),
		      "Rule Condition must not have side effects.");
      Error.CondError(!type_equal(condition->gettype(), booltype),
		      "Condition for rule must be a boolean expression.");
    }
  NextSimpleRule = SimpleRuleList;
  SimpleRuleList = this;

  size = CountSize(enclosures);

  // Rearrange enclosures so that the ones that are NOT
  // mentioned in the condition go first
  rearrange_enclosures();
  
}

simplerule * simplerule::SimpleRuleList = NULL;

int simplerule::CountSize(ste * enclosures)
{
  int ret;
  if ( enclosures != NULL &&
       ( enclosures->getvalue()->getclass() == decl::Quant ||
	 enclosures->getvalue()->getclass() == decl::Alias ||
	 enclosures->getvalue()->getclass() == decl::Choose ) )
    {
      ret = CountSize(enclosures->getnext() );
      if ( enclosures->getvalue()->getclass() == decl::Quant ||
	   enclosures->getvalue()->getclass() == decl::Choose )
	return ret * enclosures->getvalue()->gettype()->getsize();
      else
	return ret;
    }
  else
    return 1;
}


void simplerule::rearrange_enclosures()
{
  stecoll* us = condition ? condition->used_stes() : NULL;
  ste* front = NULL;
  ste* end = NULL;
  ste* rest = NULL;
  ste* ep = enclosures;

  bool is_dep = FALSE;  // Is the current ste mentioned in rule cond?
  bool is_cp = FALSE;   // Is the current ste a choose parameter?
  ste* cp = NULL;       // Choose parameter

  // Cardinality of parameters not mentioned in the condition
  indep_card = 1;
  // Lists of choose parameters
  dep_choose = indep_choose = NULL;

  if(!us) return;

  // Iterate over all relevant ste's in enclosures
  ep = enclosures;
  while( ep != NULL &&
       ( ep->getvalue()->getclass() == decl::Quant ||
         ep->getvalue()->getclass() == decl::Alias ||
         ep->getvalue()->getclass() == decl::Choose ) ) {

    // Create a copy of ep
    ste* ns = new ste(ep->getname(), ep->getscope(), ep->getvalue());
    ns->setnext(NULL);

    // If it's a choose parameter, create another copy
    is_cp = (ep->getvalue()->getclass() == decl::Choose);
    if(is_cp) {
      cp = new ste(ep->getname(), ep->getscope(), ep->getvalue());
      cp->setnext(NULL);
    }

    if(us->includes(ep)) {
      // ep is mentioned in the condition
      is_dep = TRUE;

      if (ep->getvalue()->getclass() == decl::Alias) {
         stecoll* alias_stes = ep->getvalue()->getexpr()->used_stes();
         us->append(alias_stes); 
      }

      // if it's a choose parameter,
      // add to the list of ``dependent'' choose parameters
      if(is_cp) {
        cp->setnext(dep_choose);
        dep_choose = cp;
      }
    }

    else {
      // ep is NOT mentioned in the condition
      is_dep = FALSE;

      // Cardinality of Alias is always 1
      if (ep->getvalue()->getclass() != decl::Alias) {
         indep_card *= ns->getvalue()->gettype()->getsize();
      }

      // if it's a choose parameter,
      // add to the list of ``independent'' choose parameters
      if(is_cp) {
        cp->setnext(indep_choose);
        indep_choose = cp;
      }
    }

    // If it's not an alias and it's mentioned in the condition, 
    //    move to the front of the list;
    // otherwise add to the end of the list
    if(!front) {
      front = end = ns; }
    else {
      // if(is_dep) {
      if(is_dep && 
        (ep->getvalue()->getclass() != decl::Alias)) {
        ns->setnext(front);
        front = ns; }
      else {
        end->setnext(ns);
        end = ns; }
    }

    ep = ep->getnext();
  }
  // The rest of the enclosures
  rest = ep;

  if(front) {
    enclosures = front;
    end->setnext(rest);
  }

}


/********************
  class startstate and variable startstatenum
  ********************/
int startstatenum = 0;
startstate::startstate(ste *enclosures,
		       ste *locals,
		       stmt *body)
:simplerule(enclosures, NULL, locals, body, FALSE, 0)
{
}

/********************
  class invariant and variable invariantnum
  ********************/
int invariantnum = 0;
invariant::invariant(ste *enclosures, expr *test)
:simplerule(enclosures, test, NULL, NULL, FALSE, 0)
{
  Error.CondError(!type_equal( test->gettype(), booltype),
		  "Invariant must be a boolean expression.");
  Error.CondError(test->has_side_effects(),
		  "Invariant must not have side effects.");
}

/********************
  class fairness and variable fairnessnum
  ********************/
int fairnessnum = 0;
fairness::fairness(ste *enclosures, expr *test)
:simplerule(enclosures, test, NULL, NULL, FALSE, 0)
{
  Error.CondError(!type_equal( test->gettype(), booltype),
                  "Fairness must be a boolean expression.");
  Error.CondError(test->has_side_effects(),
                  "Fairness must not have side effects.");
}

/********************
  class liveness and variable livenessnum
  ********************/
int livenessnum = 0;
liveness::liveness(ste *enclosures, expr *ptest, expr *ltest, expr *rtest,
                        live_type livetype)
:enclosures(enclosures), precondname(NULL), rightcondname(NULL),
 leftcondname(NULL), precondition(ptest), leftcondition(ltest),
 rightcondition(rtest), livetype(livetype) , name(NULL) {};


/********************
  class quantrule
  ********************/
quantrule::quantrule(ste *quant, rule *rules)
: rule(), quant(quant), rules(rules)
{
  quantdecl *q = (quantdecl *) quant->getvalue();
  if ( q->left != NULL )
    {
      Error.CondError( !q->left->hasvalue(),
		       "Bounds for ruleset parameters must be constants.");
      Error.CondError( !q->right->hasvalue(),
		       "Bounds for ruleset parameters must be constants.");
    }
}

/********************
  class chooserule
  ********************/
chooserule::chooserule(ste *index, designator *set, rule *rules)
: rule(), index(index), set(set), rules(rules)
{
  Error.CondError( ((multisetidtypedecl *)index->getvalue()->gettype())->getparenttype() != set->gettype(),
		   "Internal Error: Parameter for Choose declared in type other than the multiset index");
  Error.CondError( set->gettype()->gettypeclass() != typedecl::MultiSet,
		   "Parameter for choose must be a multiset");
}

/********************
  class aliasrule
  ********************/
aliasrule::aliasrule(ste *aliases, rule *rules)
: rule(), aliases(aliases), rules(rules)
{
}

/********************
  variable declaration
  ********************/
simplerule *error_rule = NULL;

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 7 OCT 94 Norris Ip:
  added multiset choose rule
****************************************/

/********************
 $Log: rule.C,v $
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
