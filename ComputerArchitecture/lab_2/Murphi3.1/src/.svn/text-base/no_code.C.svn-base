/* -*- C++ -*-
 * no_code.C
 * @(#) Empty code generation for the Murphi compiler.
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
 * None
 *
 */ 

#include "mu.h"

/*
 * Each generate_code() member function outputs to stdout the code
 * necessary to evaluate that thing, and returns a string with
 * what is necessary to use it as a value.
 *
 * Some examples:
 *
 * The statement "A := foo" sends this to stdout:
 *   A = foo;
 * And returns the null string.
 *
 * the expression "A[i]" sends nothing to stdout and returns
 * the string "A[i]".
 *
 * A more complicated example:
 * The expression FORALL i : 0..1 DO boolarray[i]
 * sends something like the following to stdout, which is what is
 * required to compute the forall:
 * 
 *   mu__forall_1 = TRUE;
 *   for(i = 0; i <= 1; i++)
 *     {
 *       if ( !(boolarray[i]) )
 * 	   {
 * 	     mu__forall_1 = FALSE;
 * 	     break;
 * 	   }
 *     }
 *
 * and then it returns "mu__forall_1", which is what\'s necessary to
 * use the value of the forall.
 *
 * The generate_decl() functions do the same thing, but generate
 * the code necessary to create a declaration for the object.
 * Example of the difference between generate_decl() and generate_code():
 * for the declaration
 *   VAR tf: BOOLEAN;
 * A vardecl would be created representing that declaration.  Call it
 * tfdecl. tfdecl->generate_decl() would send
 *   bool tf;
 * to stdout and return a NULL string; tfdecl->generate_code()
 * would return "tf", the string needed to access that variable.
 */


/* code for declarations. */
char *decl::generate_decl()
{
  Error.Error("Internal:  decl::generate_decl should not have been called.");
}

char *decl::generate_code()
{
  Error.Error("Internal:  decl::generate_code should not have been called.");
}

char *typedecl::generate_decl()
{
  Error.Error("Internal:  typedecl::generate_decl should never have been called.  Go find the programmer of the Murphi system and get a lollipop for uncovering this bug.");
}

#ifdef GENERATE_DECL_CODE
char *typedecl::generate_code()
{
  Error.Error("Internal:  typedecl::generate_code should never have been called.  Go find the programmer of the Murphi system and get a lollipop for uncovering this bug.");
}
#endif

char *enumtypedecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *enumtypedecl::generate_code()
{
}
#endif

char *subrangetypedecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *subrangetypedecl::generate_code()
{
}
#endif

char *arraytypedecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *arraytypedecl::generate_code()
{
}
#endif

char *recordtypedecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *recordtypedecl::generate_code()
{
}
#endif

char *constdecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *constdecl::generate_code()
{
}
#endif

char *vardecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *vardecl::generate_code()
{
}
#endif

char *aliasdecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *aliasdecl::generate_code()
{
}
#endif


char *quantdecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *quantdecl::generate_code()
{
}
#endif


char *param::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *param::generate_code()
{
}
#endif


char *varparam::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *varparam::generate_code()
{
}
#endif


char *valparam::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *valparam::generate_code()
{
}
#endif


char *constparam::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *constparam::generate_code()
{
}
#endif


char *procdecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *procdecl::generate_code()
{
}
#endif


char *funcdecl::generate_decl()
{
}

#ifdef GENERATE_DECL_CODE
char *funcdecl::generate_code()
{
}
#endif


/* Expression code. */
char *expr::generate_code()
{
}


char *condexpr::generate_code()
{
}


char *quantexpr::generate_code()
{
}


char *designator::generate_code()
{
}


char *exprlist::generate_code()
{
}


char *funccall::generate_code()
{
}




/* Statement code. */
char *stmt::generate_code()
{
}


char *assignment::generate_code()
{
}


char *whilestmt::generate_code()
{
}


char *ifstmt::generate_code()
{
}


char *caselist::generate_code()
{
}


char *switchstmt::generate_code()
{
}


char *forstmt::generate_code()
{
}


char *proccall::generate_code()
{
}


char *clearstmt::generate_code()
{
}


char *errorstmt::generate_code()
{
}


char *assertstmt::generate_code()
{
}


char *putstmt::generate_code()
{
}


char *alias::generate_code()
{
}

char *aliasstmt::generate_code()
{
}


char *returnstmt::generate_code()
{
}




/* Rule code. */
char *rule::generate_code()
{
}


char *simplerule::generate_code()
{
}


char *startstate::generate_code()
{
}


char *invariant::generate_code()
{
}


char *quantrule::generate_code()
{
}


char *aliasrule::generate_code()
{
}




/* The main program. */
char *program::generate_code()
{
}


/********************
 $Log: no_code.C,v $
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
