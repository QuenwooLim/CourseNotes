/* -*- C++ -*-
 * cpp_sym.h
 * @(#) Symmetry Code Declaration
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
 */

/* 
 * Original Author: C. Norris Ip
 * Symmetry extension to Rel2.6
 * Created: 24 Nov 1993
 * 
 * Update:
 *
 * None
 *
 */ 

#define MAX_NUM_SCALARSET 512
#define MAX_NUM_GLOBALS 512
#define TRUNCATE_LIMIT_FOR_LIMIT_FUNCTION 1

class symmetryclass
{
  stelist *scalarsetlist;
  stelist *varlist;
  int num_scalarset;
  unsigned long size_of_set;
  int size[MAX_NUM_SCALARSET];
  unsigned long factorialsize[MAX_NUM_SCALARSET];

  bool no_need_for_perm;

  unsigned long factorial(int n);

  void setup();
  void generate_set_class();
  void generate_permutation_class();
  void generate_symmetry_class();

  void generate_exhaustive_fast_canonicalization();
  void generate_heuristic_fast_canonicalization();
  void generate_heuristic_small_mem_canonicalization();
  void generate_heuristic_fast_normalization();

  void generate_symmetry_code();
  void generate_match_function();

  void set_var_list(ste * globals);

public:
  symmetryclass() : scalarsetlist(NULL), varlist(NULL), no_need_for_perm(TRUE) {};
  void add(ste *s) { scalarsetlist = new stelist(s, scalarsetlist); };
  int getsize() { return size_of_set; }

  void generate_code(ste * globals);
  void generate_symmetry_function_decl();
};

bool is_simple_scalarset(typedecl * t);

class charlist
{
public:
  char * string;
  charlist * next;
  typedecl * e; 

  charlist(char * str, charlist * nxt) : string(str), next(nxt) {}
  charlist(char * str, typedecl * e, charlist * nxt) : string(str), next(nxt), e(e) {}
  virtual ~charlist() { delete string; delete next; }

};


/****************************************
  * 6 April 94 Norris Ip:
  generate code for Match() for correct error trace printing
beta2.9S released
  * 14 July 95 Norris Ip:
  Tidying/removing old algorithm
  adding small memory algorithm for larger scalarsets
****************************************/

/********************
 $Log: cpp_sym.h,v $
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
