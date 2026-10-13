/* -*- C++ -*-
 * lextable.h
 * @(#) interface for the hash table used by the Murphi lexer.
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
 */ 

/* "y.tab.h" _must_ be included before this is included! */
// #include "mu.h"

/********************
  constants
  ********************/
#define LEXTABLESIZE 1024    /* must be a power of two. */
#define RESERVETABLESIZE 512 /* must be a power of two. */

/********************
  class lextable
         -- hash table with open chaining for lex's name table. 
  ********************/
class lextable 
{
  lexid *table[ LEXTABLESIZE ];       // table for user defined words
  lexid *rtable[ RESERVETABLESIZE ];  // table for reserved words
  int hash(char *str) const;          // hash a string into an integer. 
  int rehash(int h) const;            // if table[h] is taken, compute a new value. 
 public:
  // initializer
  lextable(void);

  // supporting routines
  lexid *enter(char *str, int lextype = ID ); 
          // return a pointer to the id lexid associated with str, creating the
          // lexid if necessary.
  bool reserved(char *str);  // check to see if it is a reserved word
  lexid *enter_reserved(char *str, int lextype = ID ); 
          // return a pointer to the id lexid associated with str, creating the
          // lexid if necessary.
};

/********************
  extern variables
  ********************/
extern lextable ltable;

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
****************************************/

/********************
 $Log: lextable.h,v $
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
