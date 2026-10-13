/* -*- C++ -*-
 * expr.C
 * @(#) Implementation for Murphi expressions.
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
 * C. Norris Ip
 * Subject: Extension for {multiset, undefined value, general union}
 * First modified: May 24, 94
 *
 */ 

#include "mu.h"
// #include "y.tab.h" /* for operator values. */

/********************
  class expr
  ********************/
expr::expr(const int value, typedecl * const type)
:value(value), type(type), constval(TRUE), sideeffects(FALSE)
{
}

expr::expr( void )
:value(0), type(NULL), constval(FALSE), sideeffects(FALSE)
{
}

/********************
  class unaryexpr
  -- class for all unary expression
  ********************/
unaryexpr::unaryexpr(typedecl *type, int op, expr *left)
:expr(type, left->hasvalue(), left->has_side_effects()),
 op(op), left(left)
{
}

unaryexpr::unaryexpr(typedecl *type, expr *left)
:expr(type, left->hasvalue(), left->has_side_effects()),
left(left)
{
}

/********************
  class binaryexpr
  -- class for all binary expression
  ********************/
binaryexpr::binaryexpr(typedecl *type, int op, expr *left, expr *right)
:expr(type, left->hasvalue() && right->hasvalue(),
      left->has_side_effects() || right->has_side_effects() ),
op(op), left(left), right(right)
{
}

/********************
  class boolexpr
  -- a binary expression of two boolean arguments.
  ********************/
boolexpr::boolexpr(int op, expr *left, expr *right)
:binaryexpr(booltype, op, left, right)
{
  Error.CondError( !type_equal( left->gettype(), booltype),
		   "Arguments of &, |, -> must be boolean.");
  Error.CondError( !type_equal( right->gettype(), booltype),
		   "Arguments of &, |, -> must be boolean.");
  Error.CondWarning(sideeffects,
		    "Expressions with side effects may behave oddly in some boolean expressions.");
  if ( constval )
    {
      switch(op) {
      case IMPLIES:
	value = !(left->getvalue() ) || (right->getvalue());
	break;
      case '&':
	value = left->getvalue() && right->getvalue();
	break;
      case '|':
	value = left->getvalue() || right->getvalue();
	break;
      default:
	Error.Error("Internal: surprising value for op in boolexpr::boolexpr");
      }
    }
}

/********************
  class notexpr
  -- boolean not
  ********************/
notexpr::notexpr(expr *left)
:unaryexpr(booltype, left)
{
  Error.CondError( !type_equal( left->gettype(), booltype),
		   "Arguments of ! must be boolean.");
  Error.CondWarning(sideeffects,
		    "Expressions with side effects may behave oddly in some boolean expressions.");
  if ( constval )
    value = !left->getvalue();
}

/********************
  class equalexpr
  -- A = B or A != B.
  ********************/
equalexpr::equalexpr(int op, expr *left, expr *right)
:binaryexpr(booltype, op, left, right)
{
  Error.CondError( !type_equal( left->gettype(),
				right->gettype()),
		   "Arguments of = or <> must have same types.");
  Error.CondError(!left->gettype()->issimple(),
		  "Arguments of = or <> must be of simple type.");
  if ( constval )
    {
      switch (op) {
      case '=':
	value = left->getvalue() == right->getvalue();
	break;
      case NEQ:
	value = left->getvalue() != right->getvalue();
	break;
      default:
	Error.Error("Internal: surprising value for op in equalexpr::equalexpr");
	break;
      }
    }
}

/********************
  class compexpr
  -- A < B, etc.
  ********************/
compexpr::compexpr(int op, expr *left, expr *right)
:binaryexpr(booltype, op, left, right)
{
  Error.CondError( !type_equal( left->gettype(), inttype),
		   "Arguments of <, <=, >, >= must be integral.");
  Error.CondError( !type_equal( right->gettype(), inttype),
		   "Arguments of  <, <=, >, >= must be integral.");
  if ( constval )
    {
      switch (op) {
      case '<':
	value = left->getvalue() < right->getvalue();
	break;
      case LEQ:
	value = left->getvalue() <= right->getvalue();
	break;
      case '>':
	value = left->getvalue() > right->getvalue();
	break;
      case GEQ:
	value = left->getvalue() >= right->getvalue();
	break;
      default:
	Error.Error("Internal: surprising value for op in compexpr::compexpr.");
	break;
      }
    }
}

/********************
  class arithexpr
  -- an arithmetic expression.
  ********************/
arithexpr::arithexpr(int op, expr *left, expr *right)
:binaryexpr(inttype, op, left, right)
{
  Error.CondError( !type_equal( left->gettype(), inttype),
		   "Arguments of %c must be integral.", op);
  Error.CondError( !type_equal( right->gettype(), inttype),
		   "Arguments of %c must be integral.", op);
  if ( constval )
    {
      switch (op) {
      case '+':
	value = left->getvalue() + right->getvalue();
	break;
      case '-':
	value = left->getvalue() - right->getvalue();
	break;
      default:
	/* Commented out to allow for mulexprs. */
	// Error.Error("Internal: surprising value for op in arithexpr::arithexpr.");
	break;
      }
    }
}

/********************
  class unexpr
  -- an unary arithmetic expression.
  -- +expr or -expr.
  ********************/
unexpr::unexpr(int op, expr *left)
:unaryexpr(inttype, op, left)
{
  Error.CondError( !type_equal( left->gettype(), inttype),
		   "Arguments of %c must be integral.", op);
  if ( constval )
    {
      switch (op) {
      case '+':
	value = left->getvalue();
	break;
      case '-':
	value = - left->getvalue();
	break;
      default:
	Error.Error("Internal: surprising value for op in unexpr::unexpr.");
	break;
      }
    }
}

/********************
  class mulexpr
  -- a multiplication or division.
  ********************/
mulexpr::mulexpr(int op, expr *left, expr *right)
: arithexpr(op, left, right)
{
  if ( constval )
    {
      switch (op) {
      case '*':
	value = left->getvalue() * right->getvalue();
	break;
      case '/':
	value = left->getvalue() / right->getvalue();
	break;
      case '%': /* not allowed as of 9/92, but just in case. */
	value = left->getvalue() % right->getvalue();
	break;
      default:
	Error.Error("Internal: surprising value for op in mulexpr::mulexpr.");
	break;
      }
    }
}

/********************
  class quantexpr
  -- a quantified expression.
  ********************/
quantexpr::quantexpr(int op, ste *parameter, expr *left)
:expr(booltype, FALSE, left->has_side_effects() ), parameter(parameter),
 op(op), left(left)
{
  Error.CondError( !type_equal( left->gettype(), booltype),
		   "Quantified subexpressions must be boolean.");
  Error.CondWarning(sideeffects,
		    "Expressions with side effects in quantified expressions may behave oddly.");
  
}

/********************
  class condexpr
  -- a ?: expression.  We need to have a subclass for this
  -- for the extra arg. 
  ********************/
condexpr::condexpr( expr *test, expr *left, expr *right)
:expr(left->gettype(),
      FALSE,
      test->has_side_effects() ||
      left->has_side_effects() ||
      right->has_side_effects()), test(test), left(left), right(right)
{
  Error.CondError( !type_equal( test->gettype(), booltype),
		   "First argument of ?: must be boolean.");
  Error.CondError( !type_equal( left->gettype(),
				right->gettype() ),
		   "Second and third arguments of ?: must have same type.");
}

/********************
  class designator
  -- represented as a left-linear tree.
  ********************/
designator::designator(ste *origin,
		       typedecl *type,
		       bool islvalue,
		       bool isconst,
		       bool maybeundefined,
		       int val)
:expr(val,type), origin(origin), lvalue(islvalue), left(NULL),
 dclass(Base), maybeundefined(maybeundefined)
{
  constval = isconst;
  sideeffects = 0;
}

designator::designator(designator *left, expr *ar)
:expr(NULL, FALSE,
      left->has_side_effects() || ar->has_side_effects() ),
left(left), arrayref(ar), lvalue(left->lvalue), dclass(ArrayRef)
{
  typedecl *t = left->gettype();
  if (Error.CondError( t->gettypeclass() != typedecl::Array
		       && t->gettypeclass() != typedecl::MultiSet,
		       "Not an array/multiset type.") )
    {
      type = errortype;
    }
  else if (t->gettypeclass() == typedecl::Array)
    {
      if (Error.CondError( !type_equal( ((arraytypedecl *)t)->getindextype(),
					ar->gettype() ),
			   "Wrong index type for array reference.") )
	{
	  type = errortype;
	}
      else
	{
	  type = ((arraytypedecl *) t)->getelementtype();
	}
    }
  else
    {
      if (Error.CondError( !ar->isdesignator(), "B")
	  || 
	  Error.CondError( ar->gettype()->gettypeclass() != typedecl::MultiSetID, "A")
	  || 
	  Error.CondError( !type_equal( (multisettypedecl *) t, ((multisetidtypedecl *)ar->gettype())->getparenttype() ),
			   "Wrong index type for MultiSet reference.") )
	{
	  type = errortype;
	}
      else
	{
	  type = ((multisettypedecl *) t)->getelementtype();
	}
    }
}

designator::designator(designator *left, lexid *fr)
:expr(NULL, FALSE,
      left->has_side_effects() ),
left(left), lvalue(left->lvalue),dclass(FieldRef)
{
  typedecl *t = left->gettype();
  if (!Error.CondError( t->gettypeclass() != typedecl::Record,
			"Not a record type.") )
    {
      ste *field = ((recordtypedecl *)t)->getfields()->search(fr);
      if (Error.CondError( field == NULL,
			   "No field with name %s.",fr->getname() ) )
	{
	  type = errortype;
	}
      else
	{
	  fieldref = field;
	  type = field->getvalue()->gettype();
	}
    }
  else
    {
      type = errortype;
    }
}

ste *designator::getbase(void) const
{
  designator *d = (designator *) this;
  while (d->left != NULL) d = d->left;
  return d->origin;
}

stecoll* designator::used_stes() const 
{
  stecoll *l = left     ? left->used_stes()     : new stecoll;
  stecoll *o = origin   ? new stecoll(origin)   : new stecoll;
  stecoll *a = arrayref ? arrayref->used_stes() : new stecoll;
  // NOTE:  fieldref is ignored

  l->append(o);
  l->append(a);

  return l;
}


/********************
  class exprlist
  ********************/
exprlist::exprlist(expr *e, exprlist *next)
: e(e), undefined(FALSE), next(next)
{
}

exprlist::exprlist(bool b, exprlist *next)
: e(NULL), undefined(b), next(next)
{
  Error.CondError( b == FALSE, "Internal Error::exprlist()");
}

exprlist *exprlist::reverse( void )
{
  exprlist *in = this, *out = NULL, *temp = NULL;
  while (in != NULL)
    {
      temp = in;
      in = in->next;
      temp->next = out;
      out = temp;
    }
  return out;
}

/********************
  class funccall
  ********************/
funccall::funccall(ste *func, exprlist *actuals)
:expr(NULL, FALSE, FALSE), func(func), actuals(actuals)
{
  if ( func != 0 )
    {
      if (Error.CondError(func->getvalue()->getclass() != decl::Func,
			  "%s is not a function.",
			  func->getname()->getname() ) )
	{
	  type = errortype;
	}
      else
	{
	  funcdecl *f = (funcdecl *) func->getvalue();
	  matchparams(func->getname()->getname(), f->params, actuals);
	  type = f->returntype;
	  sideeffects = f->has_side_effects();
	}
    }
}

/********************
  class isundefined
  ********************/
isundefined::isundefined(expr *left)
:unaryexpr(booltype, left)
{
  if ( constval ) value = FALSE;
  Error.CondError( !left->gettype()->issimple(),
 		   "The designator in ISUNDEFINED statement must be a simple variable.");
}

/********************
  class ismember
  ********************/
ismember::ismember(expr *left, typedecl *type)
:unaryexpr(booltype, left)
{
  if (type->gettypeclass() != typedecl::Scalarset
      && type->gettypeclass() != typedecl::Enum )
    {
      Error.Error("The type in ISMEMBER statement must be a scalarset/enum type.");
      t = errortype;
    }
  else if (type->gettypeclass() == typedecl::Scalarset 
	   && !((scalarsettypedecl *)type)->isnamed())
    {
      Error.Warning("The anonymous scalarset type in ISMEMBER is useless.");
      t = errortype;
    }
  else
    {
      t = type;
    }
}

/********************
  class multisetcount
  ********************/
multisetcount::multisetcount(ste * index, designator *set, expr * filter)
:expr(inttype, FALSE, FALSE), index(index), set(set), filter(filter)
{
  Error.CondError(set->gettype()->gettypeclass() != typedecl::MultiSet,
		   "2nd Argument of MultiSetCount must be a MultiSet.");
  Error.CondError( !type_equal( filter->gettype(), booltype),
		   "3rd Argument of MultiSetCount must be boolean.");
  multisetcount_num = num_multisetcount;
  num_multisetcount++;
  ((multisettypedecl *) set->gettype())->addcount(this);
}

int multisetcount::num_multisetcount=0;

/********************
  Global objects. 
  ********************/
expr *error_expr = NULL;
expr *true_expr = NULL;
expr *false_expr = NULL;
designator *error_designator = NULL;

/****************************************
  * Dec 93 Norris Ip:
  added ismember and isundefined
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 11 April 94 Norris Ip:
  fixed type checking for undefine inside a procedure.
  you can now check undefinedness of a parameter variable
  ******
  * Subject: Extension for {multiset, undefined value, general union}
  * 24 May 94 Norris Ip:
  exprlist::exprlist(bool b; ...) to use
  general undefined expression 
  * 25 May 94 Norris Ip:
  designator:: added maybeundefined field
  change ismember to include enum
  * 7 OCT 94 Norris Ip:
  added multiset count
  * 11 OCT 94 Norris Ip:
  changed array reference checking
****************************************/

/********************
 $Log: expr.C,v $
 Revision 1.3  1999/01/29 08:28:10  uli
 efficiency improvements for security protocols

 Revision 1.2  1999/01/29 07:49:12  uli
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
