/* -*- C++ -*-
 * mu.C
 * @(#) main() for the Murphi compiler.
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
 * Seungjoon Park
 * Subject: liveness syntax
 * Last modified: Novemeber  5, 93
 *
 * C. Norris Ip
 * Subject: Extension for {multiset, undefined value, general union}
 * First modified: May 24, 94
 *
 * Denis Leroy
 * Subject: 'no compression' option
 * Last modified: May 13, 95
 */ 

#include "mu.h"
#include <string.h>
#include <new>
using namespace std;

/********************
  variable declarations
  ********************/
program *theprog = NULL;     
symboltable *symtab = NULL;
FILE *codefile;

/********************
  void err_new_handler()
  ********************/
void err_new_handler()
{
  Error.FatalError("Unable to allocate enough memory.");
}

/********************
  void init_globals()
  -- initialize all global variables
  ********************/
void init_globals()
{
  set_new_handler(&err_new_handler);
  booltype = new enumtypedecl(0,1);
  booltype->declared = TRUE;
  booltype->mu_name = tsprintf("mu_0_boolean");
  inttype = new enumtypedecl(0, 10000);
  inttype->declared = TRUE;
  /* An enum, not a subrange, because an integer is a primitive type. */
  /* The upper bound doesn\'t really matter; it doesn\'t get used. MAXINT
   * seems like a logical value, but that causes an overflow in the
   * arithmetic used to calculate the number of bits it requires. I\'ve
   * chosen a value that shouldn\'t create any problems, even with small
   * ints. */
  errortype = new errortypedecl("ERROR");
  voidtype = new typedecl("VOID_TYPE");
  errorparam = new param(errortype);
  errordecl = new error_decl("ERROR_DECL");
  error_expr = new expr(0,errortype);
  true_expr = new expr(TRUE, booltype);
  false_expr = new expr(FALSE, booltype);
  error_designator = new designator(NULL, errortype, FALSE, FALSE, FALSE);
  nullstmt = new stmt;
  error_rule = new simplerule (NULL,NULL,NULL,NULL,FALSE, 0);
  symtab = new symboltable;
  theprog = new program;
  typedecl::origin = NULL;
}

/********************
  void setup_infile(char *filename)
  -- setup input file handler
  ********************/
void setup_infile(char *filename)
{
  /* pre-checking on filename. */
  int len = strlen(filename);
  if ( filename[len - 2] != '.' ||
       filename[len - 1] != 'm' )
    {
      Error.FatalError("Murphi only handles files ending in \".m\"");
    }
  else
    {
      gFileName = filename;
      yyin = fopen(filename, "r"); // yyin is flex\'s global for the input file
      if ( yyin == NULL )
	{
	  Error.FatalError("%s:No such file or directory.", filename);
	}
    }
}

/********************
  FILE *setup_outfile(char *infilename)
  -- setup output file handler
  ********************/
FILE *setup_outfile(char *infilename)
{
  char outfilename[128];
  FILE *outfile;
  int len = strlen(infilename);

  strcpy( outfilename, infilename);
  outfilename[len - 1] = 'C';
  outfile = fopen(outfilename, "w");
  if ( outfile == NULL )
    {
      Error.FatalError("Unable to open/create %s",outfilename);
    };
  return outfile;
}

/********************
  class argclass
  ********************/
bool Sw(char* a, int ac, char** av)
{
  for (int i=1; i<ac; i++)
    if (strcmp(av[i], a) == 0)
      return TRUE;
  return FALSE;
}   

argclass::argclass(int ac, char** av)
:argc(ac), argv(av), print_license(FALSE), help(FALSE), checking(TRUE),
 no_compression(TRUE), hash_compression(FALSE)
{
  bool initialized_filename = FALSE;
  int i;

  if (ac==1) {  // if there is no argument, print help
    help = TRUE;
    PrintInfo();
    exit(1);
  }
  for (i=1; i<ac; i++) 
    {
      if (strcmp(av[i], "-l") == 0) {
	print_license = TRUE;
	continue;
      } 
      if (strcmp(av[i], "-b") == 0) {
	no_compression = FALSE;
	continue;
      } 
      if (strcmp(av[i], "-c") == 0) {
	hash_compression = TRUE;
	continue;
      } 
      if (strcmp(av[i], "-h") == 0) {
	help = TRUE;
	PrintInfo();
	exit(1);
      } 
//       if ( strncmp(av[i], "-sym", strlen("-sym") ) == 0 )
//         {
// 	  // we should change it to check whether the number after prefix
// 	  // is really a number
//           if ( strlen(av[i]) <= strlen("-sym") ) /* We have a space before the number,
//                                       * so it\'s in the next argument. */
//             sscanf( av[++i], "%d", &symmetry_algorithm_number);
//           else
//             sscanf( av[i] + strlen("-sym"), "%d", &symmetry_algorithm_number);
//           continue;
//         }

      if ( av[i][0]!='-' && !initialized_filename)
	{
	  initialized_filename = TRUE;
	  filename = av[i];
	  continue;
	}
      else if ( av[i][0]!='-' && initialized_filename)
	{
	  fprintf(stderr, "Duplicated input filename.\n");
	  exit(1);
	}
      fprintf(stderr, "Unrecognized flag. Do '%s -h' for a list of valid arguments.\n",
	      av[0]);
      exit(1);
    }

  if ( !initialized_filename ) { // print help
    help = TRUE;
    PrintInfo();
    exit(0);
  }

  PrintInfo();
} 

void argclass::PrintInfo( void )
{
  if (print_license) PrintLicense();

  if (!print_license)
    {
      printf("Call with the -l flag or read the license file for terms\n");
      printf("and conditions of use.\n");
    }

  if (!help)
    printf("Run this program with \"-h\" for the list of options.\n");

  printf("Bugs, questions, and comments should be directed to\n");
  printf("\"murphi@verify.stanford.edu\".\n\n");

  printf("Murphi compiler last modified date: %s\n", LAST_DATE);
  printf("Murphi compiler last compiled date: %s\n", __DATE__);
  printf("\
===========================================================================\n");

  if (help) PrintOptions();
  fflush(stdout);
}

void argclass::PrintOptions( void )
{
  printf("The options are shown as follows:\n\
\n\
\t-h    \t\t   \thelp\n\
\t-l    \t\t   \tprint license\n\
\t-b    \t\t   \tuse bit-compacted states\n\
\t-c    \t\t   \tuse hash compaction\n\
\n\
An argument without a leading '-' is taken to be the input filename,\n\
\twhich must end with '.m'\n\
");
// \t-sym <num> \t\tsymmetry reduction algorithm <num>\n
}

void argclass::PrintLicense( void )
{
  printf("License Notice: \n\n");
  printf("\
Copyright (C) 1992 - 1999 by the Board of Trustees of \n\
Leland Stanford Junior University.\n\
\n\
License to use, copy, modify, sell and/or distribute this software\n\
and its documentation any purpose is hereby granted without royalty,\n\
subject to the following terms and conditions:\n\
\n\
1.  The above copyright notice and this permission notice must\n\
appear in all copies of the software and related documentation.\n\
\n\
2.  The name of Stanford University may not be used in advertising or\n\
publicity pertaining to distribution of the software without the\n\
specific, prior written permission of Stanford.\n\
\n\
3.  This software may not be called \"Murphi\" if it has been modified\n\
in any way, without the specific prior written permission of David L.\n\
Dill.\n\
\n\
4.  THE SOFTWARE IS PROVIDED \"AS-IS\" AND STANFORD MAKES NO\n\
REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED, BY WAY OF EXAMPLE,\n\
BUT NOT LIMITATION.  STANFORD MAKES NO REPRESENTATIONS OR WARRANTIES\n\
OF MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE\n\
USE OF THE SOFTWARE WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS\n\
TRADEMARKS OR OTHER RIGHTS. STANFORD SHALL NOT BE LIABLE FOR ANY\n\
LIABILITY OR DAMAGES WITH RESPECT TO ANY CLAIM BY LICENSEE OR ANY\n\
THIRD PARTY ON ACCOUNT OF, OR ARISING FROM THE LICENSE, OR ANY\n\
SUBLICENSE OR USE OF THE SOFTWARE OR ANY SERVICE OR SUPPORT.\n\
\n\
LICENSEE shall indemnify, hold harmless and defend STANFORD and its\n\
trustees, officers, employees, students and agents against any and all\n\
claims arising out of the exercise of any rights under this Agreement,\n\
including, without limiting the generality of the foregoing, against\n\
any damages, losses or liabilities whatsoever with respect to death or\n\
injury to person or damage to property arising from or out of the\n\
possession, use, or operation of Software or Licensed Program(s) by\n\
LICENSEE or its customers.\n\
\n\
Notes:\n\
\n\
A.  Responsible use:\n\
\n\
Murphi is to be used as a DEBUGGING AID, not as a means of \n\
guaranteeing the correctness of a design.  We do not guarantee\n\
that all errors can be caught with Murphi.  There are many\n\
reasons for this:\n\
\n\
1. Specifications and verification conditions do not necessarily\n\
capture the conditions necessary for correct operation in practice.\n\
\n\
2. Many properties cannot be stated Murphi, including timing\n\
requirements, performance requirements, \"liveness\" properties\n\
(such as \"x will eventually occur\") and many others.\n\
\n\
3. Murphi cannot verify \"large\" systems.  Almost always, the sizes of\n\
various objects in the description must be modelled as being much\n\
smaller than they are in reality, in order to make verification\n\
feasible.  There is a high probability that design errors will only be\n\
manifested when the objects are large.\n\
\n\
4. The description of a design may not be consistent with what\n\
is actually implemented.\n\
\n\
5.  Murphi may have bugs that cause errors to be overlooked.\n\
\n\
In short, Murphi is totally inadequate for guaranteeing that there are\n\
no errors; however, it is sometimes effective for discovering errors\n\
that are difficult to detect by other means.\n\
\n\
B.  Courtesy\n\
\n\
Our motivation in distributing this software freely is to encourage\n\
others to evaluate its effectiveness on a wider range of applications\n\
than we have resources to attempt, and to provide a foundation for\n\
further development of automatic verification techniques.\n\
\n\
We would very much appreciate learning about other's experiences with\n\
the system and suggestions for improvements.  Even more, we would\n\
appreciate contributions of two kinds: additional verification\n\
examples that can be added to the distribution, and enhancements to\n\
the verification system.  Although we do not promise to distribute the\n\
examples or enhancements, we may do so if feasible.\n\
\n\
C.  Historical Notes\n\
\n\
The first version of the Murphi language and verification system was\n\
originally designed in 1990-1991 by David Dill, Andreas Drexler, Alan\n\
Hu, and C. Han Yang of the Stanford University Computer Systems\n\
Laboratory.  The first version of the program was primarily\n\
implemented by Andreas Drexler.\n\
\n\
The Murphi language was extensively modified and extended by David\n\
Dill, Alan Hu, Norris Ip, Ralph Melton, Seungjoon Park, and C. Han Yang\n\
in 1992.  The new version was almost entirely reimplemented by Ralph\n\
Melton during the summer and fall of 1992.\n\
\n\
Financial and other support for the design and implementation of\n\
Murphi has come from many sources, the Defense Advanced Research\n\
Projects Agency (under contract number N00039-91-C-0138), the National\n\
Science Foundation (grant number MIP-8858807), the Powell Foundation,\n\
the Stanford Center for Integrated Systems, the U.S. Office of Naval\n\
Research, and Mitsubishi Electronic Laboratories America.  Equipment\n\
was provided by Sun Microsystems, the Digital Equipment Corporation,\n\
and IBM.\n\
\n\
These notes are based on information provided to Stanford that has\n\
not been independently verified or checked.\n\
\n\
D.  Support, comments, feedback\n\
\n\
If you need help or have comments or suggestions regarding Murphi,\n\
please send electronic mail to \"murphi@verify.stanford.edu\".  We do\n\
not have the resources to provide commercial-quality support,\n\
but we may be able to help you.\n\
\n\
End of license.\n\
\n\
===========================================================================\n\
" );
fflush(stdout);
}

/********************
  variable declarations
  ********************/
argclass *args;

/********************
  various print routines  
  ********************/
void print_header()
{
 printf("\n\
===========================================================================\n");
 printf("%s\nFinite-state Concurrent System Compiler.\n", MURPHI_VERSION);
 printf("\n\
Copyright (C) 1992 - 1999 by the Board of Trustees of\n\
Leland Stanford Junior University.\n\
===========================================================================\n");
}

void print_result()
{
  int len = strlen(args->filename);
  args->filename[len-1] = 'C';
  printf("Code generated in file %s\n", args->filename);
}

/********************
  main routines
  ********************/
int main(int argc, char *argv[])
{
  int error;
  print_header();
  args = new argclass(argc, argv);

  init_globals();
  setup_infile( args->filename );
  error = yyparse();
  if ( !error && Error.getnumerrors() == 0 )
    {
      codefile = setup_outfile( args->filename );
      theprog->generate_code();
      fclose(codefile);
      print_result();
      exit(0);
    }
  else
    {
      exit(1);
    }
}

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 17 March 94 Norris Ip:
  fixed ruleset boolean segmentation fault
  ******
  * Subject: Extension for {multiset, undefined value, general union}
  * 25 May 94 Norris Ip:
  designator:: added maybeundefined field
****************************************/

/********************
 $Log: mu.C,v $
 Revision 1.3  1999/01/29 08:28:10  uli
 efficiency improvements for security protocols

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

