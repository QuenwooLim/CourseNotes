/* -*- C++ -*-
 * expr.h
 * @(#) interfaces for Murphi expressions.
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

/* do not #include "mu.h"; mu.h includes this. */

/********************
  In each class there are 4 sections
  1) initializer --> declaration in expr.C
  2) supporting routines --> declaration in expr.C
  3) code generation --> declaration in cpp_code.C
  ********************/

/********************
  class expr
  ********************/
class expr: public TNode
{
protected:
  // variables
  int value; /* value if it has one. */
  typedecl *type; /* type of expression. */
  bool constval, sideeffects;
  
  // initializer used by subclasses. 
  expr(void);
  expr(typedecl * type, bool constval, bool sideeffects)
  : type(type), constval(constval), sideeffects(sideeffects) {};
  
public:
  // initializer
  expr(const int value, typedecl * const type);
  
  // supporting routines
  typedecl *gettype() const { return type; };
  virtual bool isquant() const { return FALSE; };
  virtual bool hasvalue() const { return constval; };
  virtual int getvalue() const { return value; };
  virtual bool islvalue() const { return FALSE; };
  virtual bool has_side_effects() const { return sideeffects; };
  virtual bool isdesignator() const { return FALSE; };
  virtual bool checkundefined() const { return FALSE; };

  virtual stecoll* used_stes() const { return new stecoll; };
  
  // code generation
  virtual char *generate_code();
};

/********************
  class unaryexpr
  -- class for all unary expression 
  ********************/
class unaryexpr: public expr
{
protected:
  int op; /* the operator value. */
  expr * left; /* the arguments. */
  
public:
  // initializer
  unaryexpr(typedecl *type, int op, expr *left);
  unaryexpr(typedecl *type, expr *left);

  virtual stecoll* used_stes() const {
    return (left ? left->used_stes() : new stecoll);
  }
  
  // code generation
  virtual char *generate_code()=0;
};

/********************
  class binaryexpr
  -- class for all binary expression
  ********************/
class binaryexpr: public expr
{
protected:
  int op; /* the operator value. */
  expr * left, * right; /* the arguments. */
  
public:
  // initializer
  binaryexpr(typedecl *type, int op, expr *left, expr *right);

  virtual stecoll* used_stes() const {
    stecoll *l = left  ? left->used_stes()  : new stecoll;
    stecoll *r = right ? right->used_stes() : new stecoll;

    l->append(r);

    return l;
  }
  
  // code generation
  virtual char *generate_code()=0;
};

/********************
  class boolexpr
  -- a binary expression of two boolean arguments.
  ********************/
class boolexpr: public binaryexpr
{
public:
  // initializer
  boolexpr(int op, expr *left, expr *right);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class notexpr
  -- boolean not
  ********************/
class notexpr: public unaryexpr
{
public:
  // initializer
  notexpr(expr *left);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class equalexpr
  -- A = B or A != B.
  ********************/
class equalexpr: public binaryexpr
{
public:
  // initializer
  equalexpr(int op, expr *left, expr *right);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class compexpr
  -- A < B, etc.
  ********************/
class compexpr: public binaryexpr
{
public:
  // initializer
  compexpr(int op, expr *left, expr *right);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class arithexpr
  -- an arithmetic expression.
  ********************/
class arithexpr: public binaryexpr
{
public:
  // initializer
  arithexpr(int op, expr *left, expr *right);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class unexpr
  -- an unary arithmetic expression.
  -- +expr or -expr.
  ********************/
class unexpr: public unaryexpr
{
public:
  // initializer
  unexpr(int op, expr *left);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class mulexpr
  -- a multiplication or division.
  ********************/
class mulexpr: public arithexpr
{
public:
  // initializer
  mulexpr(int op, expr *left, expr *right);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class quantexpr
  -- a quantified expression.
  ********************/
class quantexpr: public expr
{
protected:
  int op;          /* FORALL or EXISTS. */
  ste * parameter; /* the quantified variable. */
  expr * left;     /* the sub-expression. */
  
public:
  // initializer
  quantexpr(int op, ste * parameter, expr * left);
  
  // supporting routines
  ste *getparam() const { return parameter; };
  virtual bool isquant() const { return TRUE; };
  virtual bool hasvalue() const { return FALSE; };
  virtual int getvalue() const { return FALSE; };
  
  virtual stecoll* used_stes() const {
    // NOTE:  parameter is ignored
    return (left ? left->used_stes() : new stecoll);
  }
  
  // code generation
  virtual char *generate_code();
};

/********************
  class condexpr
  -- a ?: expression.  We need to have a subclass for this
  -- for the extra arg. 
  ********************/
class condexpr: public expr
{
protected:
  expr * test, * left, * right;
public:
  // initializer
  condexpr(expr * test,
	   expr * left,
	   expr * right);
  virtual bool hasvalue() const { return FALSE; };
  
  virtual stecoll* used_stes() const {
    
    stecoll* t = test  ? test->used_stes()  : new stecoll;
    stecoll* l = left  ? left->used_stes()  : new stecoll;
    stecoll* r = right ? right->used_stes() : new stecoll;
    
    t->append(l);
    t->append(r);
   
    return t;
  }
  
  // code generation
  virtual char *generate_code();
};

/********************
  class designator
  -- represented as a left-linear tree.
  ********************/
class designator: public expr
{
  // variables
  designator * left;
  ste * origin;    /* an identifier; */
  expr * arrayref; /* an array reference. */
  ste * fieldref;  /* a field reference. */
  bool lvalue;

  // undefined
  bool maybeundefined;
  
  // class identifiers
  enum designator_class { Base, ArrayRef, FieldRef };
  designator_class dclass;
  
public:
  // initializer
  designator(ste * origin,
	     typedecl * type,
	     bool islvalue,
	     bool isconst,
	     bool maybeundefined,
	     int val = 0);
  designator(designator *left, expr *ar);
  designator(designator *left, lexid *fr);
  
  // supporting routines
  virtual bool islvalue() const { return lvalue; };
  ste *getbase( void ) const;
  virtual bool isdesignator() const { return TRUE; };
  virtual bool checkundefined() const { return maybeundefined; };

  virtual stecoll* used_stes() const;
  
  // code generation
  virtual char *generate_code();
};

/********************
  class exprlist
  ********************/
struct exprlist
{
  expr * e;
  bool undefined;
  exprlist * next;
  
  // initializer
  exprlist (expr *e, exprlist *next = NULL);
  exprlist (bool b, exprlist *next = NULL); // b should be TRUE
                                            // in current implementation
  exprlist *reverse(); /* reverses it. */
friend bool matchparams(ste *formals, exprlist *actuals);


  virtual stecoll* used_stes() const {
    stecoll* x = e    ? e->used_stes()    : new stecoll;
    stecoll* n = next ? next->used_stes() : new stecoll;

    x->append(n);

    return x;
  }
  
  // code generation
  virtual char *generate_code(char *name, ste *formals);
};

/********************
  class funccall
  ********************/
struct funccall: public expr
{
  ste * func;
  exprlist * actuals;
  
  // initializer
  funccall(ste *func, exprlist *actuals);

  virtual stecoll* used_stes() const {
    // NOTE: func is ignored
    return (actuals ? actuals->used_stes() : new stecoll);
  }
  
  // code generation
  virtual char *generate_code();
};

/********************
  class isundefined
  ********************/
struct isundefined: public unaryexpr
{
  // initializer
  isundefined(expr *left);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class ismember
  ********************/
struct ismember: public unaryexpr
{
  // variable
  typedecl *t;

  // initializer
  ismember(expr *left, typedecl *type);
  
  // code generation
  virtual char *generate_code();
};

/********************
  class multisetcount
  ********************/
class multisettypedecl;

class multisetcount: public expr
{
  static int num_multisetcount;

  // variablea
  ste *index;
  designator *set;
  expr *filter;
  int multisetcount_num;

public:
  // initializer
  multisetcount(ste * index, designator *set, expr *filter);
  
  virtual stecoll* used_stes() const {
    // NOTE: func is ignored
    stecoll *i = index  ? new stecoll(index)  : new stecoll;
    stecoll *s = set    ? set->used_stes()    : new stecoll;
    stecoll *f = filter ? filter->used_stes() : new stecoll;

    i->append(s);
    i->append(f);

    return i;
  }
  
  // code generation
  virtual void generate_decl(multisettypedecl * mset);
  virtual void generate_procedure();
  virtual char *generate_code();
};

/********************
  extern variables
  ********************/
extern designator * error_designator;
extern expr * error_expr;
extern expr * true_expr;
extern expr * false_expr;

/****************************************
  * Dec 93 Norris Ip:
  added ismember and isundefined
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  ******
  * Subject: Extension for {multiset, undefined value, general union}
  * 24 May 94 Norris Ip:
  exprlist::exprlist(bool b; ...) to use
  general undefined expression 
  * 25 May 94 Norris Ip:
  designator:: added maybeundefined field
  * 7 OCT 94 Norris Ip:
  added multiset count
****************************************/

/********************
 $Log: expr.h,v $
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
