/* -*- C++ -*-
 * util.C
 * @(#) utility routines for the Murphi compiler.
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
#include <stdarg.h>

/********************
  variable declaration
  ********************/
TNode *error = new TNode;

/********************
  int CeilLog2( int n )
  -- Number of bits needed to represent a type with n distinct values.
  ********************/
int CeilLog2( int n )
{
  /* Except 0 maps to 1. */  // now fixed. 
  /* taken from Andreas Drexler's code. */
  /* This is the number of bits required to represent `n' different numbers. */
  int	retval, i=0;	
  Error.CondError( n < 0, "Internal:NumBits: Negative number.");
  if( n == 0 || n == 1 ) {
    retval = 0; // Changed from 1.
  } else {
    n--;
    while( n > 0 ) {
      i++;
      n >>= 1;
    }
    retval = i;
  }
  return retval;
}

/********************
  char *tsprintf (const char *fmt, ...)
  -- sprintf's the arguments into dynamically allocated memory.  Returns the
  -- dynamically allocated string. 
  ********************/
char *tsprintf (const char *fmt, ...)
{
  static char temp_buffer[BUFFER_SIZE]; // hope that\'s enough.
  va_list argp;
  char *retval;

  va_start(argp,fmt);
  vsprintf(temp_buffer, fmt, argp);
  va_end(argp);

  if (strlen(temp_buffer) >= BUFFER_SIZE)
    Error.Error("Temporary buffer overflow.\n\
Please increase the constant BUFFER_SIZE in file mu.h and recompile Murphi\n\
(you may also reduce the length of expression by using function call.");

  retval = new char[ strlen(temp_buffer) + 1 ]; // + 1 for the \0.
  strcpy(retval, temp_buffer);
  return ( retval );
}

/********************
  int new_int()
  -- generate a new integer
  ********************/
int new_int()
{
  static int n = 0;
  return n++;
}

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
****************************************/

/********************
 $Log: util.C,v $
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
