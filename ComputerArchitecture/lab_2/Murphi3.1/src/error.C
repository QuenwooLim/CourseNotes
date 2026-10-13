/* -*- C++ -*-
 * error.C
 * @(#) a simple error handler for the Murphi compiler.
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
#include <iostream>

using namespace std;
/********************
  class Error_handler
  ********************/
Error_handler::Error_handler( void ) 
: numerrors(0), numwarnings(0)
{
}

void Error_handler::vError( const char *fmt, va_list argp )
{
  cout.flush();
  fprintf(stderr, "%s:%d:", gFileName, gLineNum);
  vfprintf(stderr, fmt, argp);
  fprintf(stderr, "\n");
  numerrors++;
}

void Error_handler::Error( const char *fmt, ... )
{
  va_list argp;
  va_start ( argp, fmt);
  vError(fmt, argp);
  va_end(argp);
}

bool Error_handler::CondError( const bool test, const char *fmt, ... )
{
  if (test)
    {
      va_list argp;
      va_start ( argp, fmt);
      vError( fmt, argp);
      va_end(argp);
    }
  return test;
}

void Error_handler::FatalError( const char *fmt, ... )
{
  cout.flush();
  va_list argp;
  va_start ( argp, fmt);
  vfprintf(stderr, fmt, argp);
  fprintf(stderr, "\n");
  fflush(stderr);
  va_end(argp); /* This doesn\'t matter much, does it? */
  exit(1);
}

void Error_handler::vWarning( const char *fmt, va_list argp )
{
  cout.flush();
  fprintf(stderr, "%s:%d: warning: ", gFileName, gLineNum);
  vfprintf(stderr, fmt, argp);
  fprintf(stderr, "\n");
  numwarnings++;
}

void Error_handler::Warning( const char *fmt, ... )
{
  va_list argp;
  va_start ( argp, fmt);
  vWarning(fmt, argp);
  va_end(argp);
}

bool Error_handler::CondWarning( const bool test, const char *fmt, ... )
{
  if (test)
    {
      va_list argp;
      va_start ( argp, fmt);
      vWarning(fmt, argp);
      va_end(argp);
    }
  return test;
}

/********************
  declare Error_handler instances
  ********************/
Error_handler Error;

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
****************************************/

/********************
 $Log: error.C,v $
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
