/* -*- C++ -*-
 * cpp_code_as.C
 * @(#) C++ code generation module for the Murphi compiler.
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
 * Rewritten by Denis Leroy, June 15, 1995
 *
 * Update:
 *
 * None
 */ 

#include "mu.h"

void 
arraytypedecl::generate_assign()
{
  char indexstr[3];

  strcpy(indexstr, (indextype->getsize() > 1 ? "i" : "0"));
  fprintf(codefile,
	  "  %s& operator= (const %s& from)\n"
	  "  {\n",
	  mu_name, mu_name);
  if (indextype->getsize() > 1)
    fprintf(codefile,
            "    for (int i = 0; i < %d; i++)\n",
	    indextype->getsize());
  if (elementtype->issimple())
    fprintf(codefile,
	    "      array[%s].value(from.array[%s].value());\n",
	    indexstr, indexstr);
  else
    fprintf(codefile,
	    "      array[%s] = from.array[%s];\n",
	    indexstr, indexstr);
  fprintf(codefile,
            "    return *this;\n"
            "  }\n\n");
};

void 
multisettypedecl::generate_assign()
{
  char indexstr[3];

  strcpy(indexstr, (maximum_size > 1 ? "i" : "0"));
  fprintf(codefile,
	  "  %s& operator= (const %s& from)\n"
	  "  {\n",
	  mu_name, mu_name);
  if (maximum_size > 1)
    fprintf(codefile,
	    "    for (int i = 0; i < %d; i++)\n",
	    maximum_size);
  if (elementtype->issimple())
      fprintf(codefile,
	      "    {\n"
	      "        array[%s].value(from.array[%s].value());\n"
	      "        valid[%s].value(from.valid[%s].value());\n",
	      indexstr, indexstr, indexstr, indexstr);
  else
      fprintf(codefile,
	      "    {\n"
	      "       array[%s] = from.array[%s];\n"
	      "       valid[%s].value(from.valid[%s].value());\n",
	    indexstr, indexstr, indexstr, indexstr);
  fprintf(codefile,
	  "    };\n"
	  "    current_size = from.get_current_size();\n"
	  "    return *this;\n"
	  "  }\n\n");
};

void recordtypedecl::generate_assign()
{
  ste *f;

  fprintf(codefile,
	  "  %s& operator= (const %s& from) {\n",
	  mu_name,
	  mu_name);
  for( f = fields; f != NULL; f = f->getnext() ) {
    if (f->getvalue()->gettype()->issimple())
      fprintf(codefile,
	      "    %s.value(from.%s.value());\n",
	      f->getvalue()->generate_code(),
	      f->getvalue()->generate_code());
    else
      fprintf(codefile,
	      "    %s = from.%s;\n",
	      f->getvalue()->generate_code(),
	      f->getvalue()->generate_code());
  }
  fprintf(codefile,
	  "    return *this;\n"
	  "  };\n");
};

/********************
 $Log: cpp_code_as.C,v $
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
