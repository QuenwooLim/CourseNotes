/* -*- C -*-
 * mu_prolog.inc
 * @(#) information used by the generated code of Murphi verifiers.
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
 * Murphi without the -l switch for additional information.
 *
 *
 */

/* 
 * Original Author: Ralph Melton
 * 
 * Update:
 *
 * C. Norris Ip
 * Subject: Breakdown into files
 * First modified : 14 Jan 93
 * Last modified : 23 March 93
 *
 * Seungjoon Park
 * Subject: Portability
 * Modefied : Fall 93
 *
 * C. Norris Ip 
 * Subject: symmetry syntax
 * First version: Jan 14, 93 -- June 11, 93
 * Transfer to Liveness Version:
 * First modified: November 8, 93
 *
 */ 

/*  Define this macro for pre 2.x G++ versions.
    It controls whether delete has a size argument,
    which is required by old g++ and generates a
    warning in new g++ compilers */

#ifdef OLDGPP
// g++ 1.x
#define OLD_GPP(arg) arg
#else
// g++ 2.x
#define OLD_GPP(arg)
#endif

/****************************************
  Default Value or Constant that can be set by user
 ****************************************/

/* number of times you can go around a while loop. */
#define DEF_LOOPMAX     1000

/* Default memory for Unix. */
#define DEFAULT_MEM     8000000         

// Uli: default number of bits for hash compaction and
//      suffix of trace info file
#ifdef HASHC
#define DEFAULT_BITS   40
#define TRACE_FILE     ".trace"
#endif

/* Default Maximum number of error to search when -finderrors is used */
#define DEFAULT_MAX_ERRORS 100

/* the size of search queues as a percentage
   of the maximum number of states */
#define gPercentActiveStates 0.10

// for use in tsprintf
#define BUFFER_SIZE 1024

/****************************************
  Release information
 ****************************************/
/* last update */
#define INCLUDE_FILE_DATE "Jan 29 1999"

/* Murphi Version Number */
#define INCLUDE_FILE_VERSION "Murphi Release 3.1"

/****************************************
  main headers
  ****************************************/
#include "mu_verifier.h"

/****************************************
  other headers
  ****************************************/
#ifdef HASHC
#include "mu_hash.h"
#endif
#include "mu_sym.h"
#include "mu_util.h"
#include "mu_io.h"
#include "mu_state.h"
#include "mu_system.h"


/****************************************
  Modification:
  1) Jan 93 Norris Ip: 
  move user configurable parameter to here
  2) 8 March 94 Norris Ip:
  merge with the latest rel2.6
****************************************/

/********************
  $Log: mu_prolog.inc,v $
  Revision 1.2  1999/01/29 07:49:10  uli
  bugfixes

  Revision 1.4  1996/08/07 18:54:33  ip
  last bug fix on NextRule/SetNextEnabledRule has a bug; fixed this turn

  Revision 1.3  1996/08/07 01:00:18  ip
  Fixed bug on what_rule setting during guard evaluation; otherwise, bad diagnoistic message on undefine error on guard

  Revision 1.2  1996/08/07 00:15:26  ip
  fixed while code generation bug

  Revision 1.1  1996/08/07 00:14:46  ip
  Initial revision

********************/
