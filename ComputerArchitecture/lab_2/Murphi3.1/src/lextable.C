/*  -*- C++ -*-
 * lextable.C
 * @(#) implementation for the hash table used by the Murphi lexer.
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
 * Subject: Case insensitive keywords and case sensitive identifiers
 * Modified: April 93
 * 
 * Seungjoon Park
 * Subject: liveness syntax
 * Last modified: Novemeber  5, 93
 *
 * C. Norris Ip 
 * Subject: symmetry syntax
 * Last modified: November 8, 93
 *
 * C. Norris Ip
 * Subject: Extension for {multiset, undefined value, general union}
 * First modified: May 24, 94
 *
 */ 

#include "mu.h"
#include <string.h>
#include <ctype.h>

/********************
  Hash table
  -- Mostly copied from the hash table used in CS143, Spring '92. rlm
  -- A simple and mediocre hash table implementation.
  -- This will go into an infinite loop when the hash table fills up.
  ********************/

/********************
  Constant
  -- constant for int lextable::hash(char *str) const
  ********************/
#define M1		71
#define M2		578393
#define M3		358723

/********************
  class lexid
  -- from mu.h
  ********************/
lexid::lexid(char *name, int lextype)
{
  this->name = new char[ strlen(name) + 1]; // +1 for the '\0'.
  strcpy(this->name,name);
  this->lextype = lextype;
}

/********************
  utilities
  ********************/
char *my_strtolower(char * str)
{
  char *ret;

  ret = new char[ strlen(str) + 1];
  strcpy(ret, str);
  for (int i=0; i<strlen(ret); i++)
    ret[i]=tolower(ret[i]);
  return ret;
}

/********************
  class lextable
  ********************/
int lextable::hash(char *str) const
/* This returns an integer hash value for a string. */
{
  char c;
  int h = 0;
  while ((c = *str++) != '\0')
    h = (h * M1 + (int) c );
  return(h);
}

int lextable::rehash(int h) const
/* If hash location is already full, this computes a new hash address to 
   try. */
{
  return(h +1);
}

bool lextable::reserved(char *str)
{
  // in order to allow at least and at most one entry
  // of reserved words in the main lex table,
  // the lower case version is considered not reserved here,
  // so that the same enter routine can be used in the 
  // main lex table for initializing keywords.

  int h;
  char *lowerstr;

  lowerstr = my_strtolower(str);
  h = hash(lowerstr);

  while (1) {
    int i = (h & RESERVETABLESIZE-1);
    lexid *entry = rtable[i];
    if (entry == NULL) {
      return(FALSE);
    }
    else if ( strcmp(entry->getname(), lowerstr) == 0) {
      return(TRUE);
    }
    else {
      /* try again */
      h = rehash(h);
    }
  }
}

lexid *lextable::enter_reserved(char *str, int lextype )
{
  int h;
  char *lowerstr;

  lowerstr = my_strtolower(str);
  h = hash(lowerstr);

  while (1) {
    int i = (h & RESERVETABLESIZE-1);
    lexid *entry = rtable[i];
    if (entry == NULL) {
      /* enter it and return */
      rtable[i] = entry = new lexid(lowerstr, lextype);
      return(entry);
    }
    else if ( strcmp(entry->getname(), lowerstr) == 0) {
      return(entry);
    }
    else {
      /* try again */
      h = rehash(h);
    }
  }
}

lexid *lextable::enter(char *str, int lextype )
{
  int h = hash(str);
  while (1) {
    int i = (h & LEXTABLESIZE-1);
    lexid *entry = table[i];
    if (entry == NULL) {
      // there is no entry in the lex table
      // but is it a reserved word in a different case context?
      if (reserved(str))
      {
	// yes, it is.  get the entry in the reserved words table
        return enter_reserved(str);
      }
      else
      {
        /* no, it is not.  Enter it and return */
        table[i] = entry = new lexid(str, lextype);
        return(entry);
      }
    }
    else if ( strcmp(entry->getname(), str) == 0) {
      return(entry);
    }
    else {
      /* try again */
      h = rehash(h);
    }
  }
}

lextable::lextable()
{
  int i; /* loop index. */
  for( i=0; i < LEXTABLESIZE; i++) table[i] = NULL;

  // table[] is case sensitive and rtable[] is case insensitive
  // before entering new entry, the string is checked again
  // the reserve table in lower case.  Therefore, all reserved words
  // are case insensitive and everything else are case sensitive
  enter_reserved("end",END);
  enter_reserved("program",PROGRAM); 
  // enter_reserved("process",PROCESS); 
  enter_reserved("procedure",PROCEDURE);
  enter_reserved("endprocedure",ENDPROCEDURE);
  enter_reserved("function",FUNCTION);
  enter_reserved("endfunction",ENDFUNCTION);
  enter_reserved("rule",RULE);
  enter_reserved("endrule",ENDRULE);
  enter_reserved("ruleset",RULESET);
  enter_reserved("endruleset",ENDRULESET);
  enter_reserved("alias",ALIAS);
  enter_reserved("endalias",ENDALIAS);
  enter_reserved("if",IF);
  enter_reserved("then",THEN);
  enter_reserved("elsif",ELSIF);
  enter_reserved("else",ELSE);
  enter_reserved("endif",ENDIF);
  enter_reserved("switch",SWITCH);
  enter_reserved("case",CASE);
  enter_reserved("endswitch",ENDSWITCH);
  enter_reserved("for",FOR);
  enter_reserved("forall",FORALL);
  enter_reserved("exists",EXISTS);
  enter_reserved("in",IN);
  enter_reserved("do",DO);
  enter_reserved("endfor",ENDFOR);
  enter_reserved("endforall",ENDFORALL);
  enter_reserved("endexists",ENDEXISTS);
  enter_reserved("while",WHILE);
  enter_reserved("endwhile",ENDWHILE);
  enter_reserved("return",RETURN);
  enter_reserved("to", TO);
  enter_reserved("begin",bEGIN);
  enter_reserved("by",BY);
  enter_reserved("clear",CLEAR);
  enter_reserved("error",ERROR);
  enter_reserved("assert",ASSERT);
  enter_reserved("put",PUT);
  enter_reserved("const",CONST);
  enter_reserved("type",TYPE);
  enter_reserved("var",VAR);
  enter_reserved("enum",ENUM);
  enter_reserved("interleaved",INTERLEAVED);
  enter_reserved("record",RECORD);
  enter_reserved("array",ARRAY);
  enter_reserved("of",OF);
  enter_reserved("endrecord",ENDRECORD);
  enter_reserved("startstate",STARTSTATE);
  enter_reserved("endstartstate",ENDSTARTSTATE);
  enter_reserved("invariant", INVARIANT);
  enter_reserved("traceuntil",TRACEUNTIL);

  /* liveness */
  enter_reserved("fairness", FAIRNESS);
  enter_reserved("fairnessset",FAIRNESSSET);
  enter_reserved("endfairnessset",ENDFAIRNESSSET);
  enter_reserved("liveness", LIVENESS);
  enter_reserved("livenessset",LIVENESSSET);
  enter_reserved("endlivenessset",ENDLIVENESSSET);
  enter_reserved("always", ALWAYS);
  enter_reserved("eventually", EVENTUALLY);
  enter_reserved("until", UNTIL);
  enter_reserved("unfair", UNFAIR);

  /* scalarset */
  enter_reserved("scalarset",SCALARSET);
  enter_reserved("ismember",ISMEMBER);

  /* undefined */
  enter_reserved("undefine",UNDEFINE);
  enter_reserved("isundefined",ISUNDEFINED);
  enter_reserved("undefined",UNDEFINED);

  /* general union */
  enter_reserved("union",UNION);

  /* multiset */
  enter_reserved("multiset",MULTISET);
  enter_reserved("multisetremove",MULTISETREMOVE);
  enter_reserved("multisetremovepred",MULTISETREMOVEPRED);
  enter_reserved("multisetadd",MULTISETADD);
  enter_reserved("multisetcount",MULTISETCOUNT);
  enter_reserved("choose",CHOOSE);
  enter_reserved("endchoose",ENDCHOOSE);

  /* first definitions. */
  enter_reserved("boolean",ID);
  enter_reserved("true",ID);
  enter_reserved("false",ID);
}

/********************
  variable declarations
  ********************/
lextable ltable;    

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  ******
  * Subject: Extension for {multiset, undefined value, general union}
  * 24 May 94 Norris Ip:
  added keyword undefined
  * 25 May 94 Norris Ip:
  changed keyword scalarsetunion to union
  * 7 OCT 94 Norris Ip:
  added keywords on multiset
****************************************/

/********************
 $Log: lextable.C,v $
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
