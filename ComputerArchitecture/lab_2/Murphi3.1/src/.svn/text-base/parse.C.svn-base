/*  -*- C++ -*-
 * parse.C
 * @(#) general structures to support parsing for the Murphi compiler.
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
 * First modified: November 8, 93
 *
 * C. Norris Ip
 * Subject: Extension for {multiset, undefined value, general union}
 * First modified: May 24, 94
 *
 */ 

#include "mu.h"

/********************
  variable declarations
  ********************/
int gLineNum = 1;
char *gFileName;
extern char *yytext; /* lex\'s token matched. */
int offset = 0;

/********************
  void yyerror(char *s)
  ********************/
void yyerror(char *s)
{
  Error.Error("%s. Last token read was \'%s\'",s,yytext);
}

/********************
  bool matchparams(char *name, ste *formals, exprlist *actuals)
  -- matching type for the parameters
  ********************/
bool matchparams(char *name, ste *formals, exprlist *actuals)
{
  bool match = TRUE;
  for(;
      formals != NULL && actuals != NULL;
      formals = formals->getnext(), actuals = actuals->next)
    {
      param *f = (param *) formals->getvalue();
      if (!actuals->undefined)
	{
	  expr *e = actuals->e;
	  if ( !type_equal( f->gettype(), e->gettype() ) )
	    {
	      Error.Error("Actual parameter to %s has wrong type.", name);
	      match = FALSE;
	    }
	  if ( f->getparamclass() == param::Var )
	    {
	      if ( Error.CondError( !e->islvalue(),
				    "Non-variable expression can't be passed to VAR parameter of %s.",name) )
		match = FALSE;
	      if ( Error.CondError(f->gettype() != e->gettype(),
				   "Var parameter to %s has wrong type.", name) )
		match = FALSE;
	    }
	}
      else
	{
	  if ( f->getparamclass() == param::Var )
	    {
	      Error.Error("UNDEFINED value can't be passed to VAR parameter of %s.",name);
	      match = FALSE;
	    }
	}
    }
  if ( (formals != NULL && actuals == NULL ) ||
       (actuals != NULL && formals == NULL ) )
    {
      Error.Error("Wrong number of parameters to function %s.", name);
      match = FALSE;
    }
  return match;
}

/********************
  bool type_equal(typedecl *a, typedecl *b)
  -- whether two types should be considered compatible.
  ********************/
bool type_equal(typedecl *a, typedecl *b)
{
  stelist * member;

//   if ( ( a->gettypeclass() == typedecl::Error_type ) ||
//        ( b->gettypeclass() == typedecl::Error_type ) )
//     return TRUE;

  // added by Uli
  if (a->gettype()->gettypeclass() == typedecl::MultiSetID
      && b->gettype()->gettypeclass() != typedecl::MultiSetID)
    return FALSE;
  if (a->gettype()->gettypeclass() != typedecl::MultiSetID
      && b->gettype()->gettypeclass() == typedecl::MultiSetID)
    return FALSE;
  if (a->gettype()->gettypeclass() == typedecl::MultiSetID
      && b->gettype()->gettypeclass() == typedecl::MultiSetID)
        return 0 ==
        strcmp(((multisetidtypedecl*)a)->getparentname(),
                ((multisetidtypedecl*)b)->getparentname());
  // end added by Uli

  if ( ( a->gettype()->gettypeclass() == typedecl::Enum
	 || a->gettype()->gettypeclass() == typedecl::Scalarset )
       && b->gettype()->gettypeclass() == typedecl::Union )
    {
      for ( member = ((uniontypedecl *) b->gettype())->getunionmembers();
	    member != NULL;
	    member = member->next)
	if ( a->gettype() == member->s->getvalue() )
	  return TRUE;
      return FALSE;
    }

  if ( a->gettype()->gettypeclass() == typedecl::Union
       && ( b->gettype()->gettypeclass() == typedecl::Enum
	    || b->gettype()->gettypeclass() == typedecl::Scalarset ) )
    {
      for ( member = ((uniontypedecl *) a->gettype())->getunionmembers();
	    member != NULL;
	    member = member->next)
	if ( b->gettype() == member->s->getvalue() )
	  return TRUE;
      return FALSE;
    }

  return ( a->gettype() == b->gettype() ); 
}

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  ******
  * Subject: Extension for {multiset, undefined value, general union}
  * 24 May 94 Norris Ip:
  matchparams changed to reflect undefined parameter passing.
  * 25 May 94 Norris Ip:
  rename all scalarsetunion to union
****************************************/

/********************
 $Log: parse.C,v $
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
