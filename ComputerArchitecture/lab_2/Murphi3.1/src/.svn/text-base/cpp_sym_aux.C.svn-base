/* -*- C++ -*-
 * cpp_sym_aux.C
 * @(#) Auxiliary Symmetry Code Generation
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
 * Created: 20 Dec 1993
 * 
 * Major Update:
 *
 * Denis Leroy and Norris Ip
 * Subject: removed useless run-time checking,
 *          faster SimpleToExplicit routines.
 * Last modified: June 18, 1995
 *
 * For details in creating and updating the file,
 * please check at the end.
 */ 

#include "mu.h"
#include <limits.h>   // Uli: needed for ULONG_MAX

/********************
  analyse variable list and put them in desired order
  according to their complexity toward canonicalization
  ********************/

void symmetryclass::set_var_list(ste * globals)
{
  int i,p;
  int priority[MAX_NUM_GLOBALS];
  ste *next;
  stelist * var;
  stelist * sortedvarlist = NULL;
  

  // form unprioritized varlist
  if ( globals != NULL ) {
      next = globals->getnext();
      if ( next != NULL && next->getscope() == globals->getscope() )
        set_var_list( next );
      if ( globals->getvalue()->getclass() == decl::Var)
        varlist = new stelist (globals, varlist);
   }

  // set priority according to complexity
  for (i=0, var = varlist; var != NULL && i<MAX_NUM_GLOBALS;
       i++, var = var->next)
    switch (var->s->getvalue()->gettype()->getstructure()) {
    case typedecl::NoScalarset:
      priority[i] = 0;
      break;
    case typedecl::ScalarsetVariable:
      priority[i] = 1;
      break;
    case typedecl::ScalarsetArrayOfFree:
      priority[i] = 2;
      break;
    case typedecl::MultisetOfFree:
      priority[i] = 3;
      break;
    case typedecl::ScalarsetArrayOfScalarset:
      priority[i] = 4;
      no_need_for_perm = FALSE;
      break;
    case typedecl::MultisetOfScalarset:
      priority[i] = 5;
      no_need_for_perm = FALSE;
      break;
    case typedecl::Complex:
      priority[i] = 6;
      no_need_for_perm = FALSE;
      break;
    }
  if (i>MAX_NUM_GLOBALS) Error.Error("Too many global variables.");

  // form prioritized var list
  for (p=6; p>=0; p--)
    for (i=0, var = varlist; var != NULL; i++, var = var->next)
      if (priority[i]==p) 
        sortedvarlist = new stelist ( var->s , sortedvarlist);
  varlist = sortedvarlist;
}

/********************
  utilities for symmetry class
  ********************/

// Uli: changed this routine so that in case of overflow ULONG_MAX is
//      returned
unsigned long symmetryclass::factorial(int n)
{
  double r = 1;
  for (int i=1; i<=n; i++) {
    r*=i;
    if (r>ULONG_MAX) return ULONG_MAX;
  }
  return (unsigned long) r;
}

/********************
  setup statistic for symmetries
  ********************/
// Uli: changed this routine so that in the case of overflow size_of_set
//      is set to ULONG_MAX (and not to the "random" modulo value that 
//      results from unsigned long arithmetic)
void symmetryclass::setup()
{
  int i;
  double ss;
  stelist *member;

  // prepare size information
  ss = 1;
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      size[i] = ((scalarsettypedecl *) member->s->getvalue())->getsize();
      factorialsize[i] = factorial(size[i]);
      ss *= factorialsize[i];
      if (ss>ULONG_MAX)
        ss = ULONG_MAX;
    }
  size_of_set = (unsigned long) ss;
  num_scalarset = i;
}

/********************
  generate simple set class
  ********************/
void symmetryclass::generate_set_class()
{
//   int i;
//   stelist *member;
//   scalarsettypedecl *t;
// 
//   fprintf(codefile,
//           "\n/********************\n"
//           " Simple Set Class\n"
//           " ********************/\n");
// 
//   for (i=0, member = scalarsetlist;
//        member != NULL;
//        i++, member = member->next)
//     {
//       t = (scalarsettypedecl *) member->s->getvalue();
//       fprintf(codefile,
// 	      "\n/********************\n"
// 	      "   set_of_%s\n"
// 	      "*/\n"
// 	      "class set_of_%s\n"
// 	      "{\n"
// 	      "public: \n"
// 	      "  bool in[%d];\n"
// 	      "  set_of_%s();\n"
// 	      "  void setemptyset();\n"
// 	      "  void setfullset();\n"
// 	      "  void setsingleton(int a);\n"
// 	      "  bool issingleton();\n"
// 	      "  int getsingleton();\n"
// 	      "  void setdifferent(set_of_%s s);\n"
// 	      "  bool issubset(set_of_%s s);\n"
// 	      "  int getsize();\n"
// 	      "  void setinterset(set_of_%s s);\n"
// 	      "  bool interset(set_of_%s s);\n"
// 	      "  bool isempty();\n"
// 	      "  bool isfull();\n"
// 	      "};\n"
// 	      "\n"
// 	      "// the following cannot be declared inline?  some of them if declared inline\n"
// 	      "// will give wrong answer!\n"
// 	      "set_of_%s::set_of_%s()\n"
// 	      "{ for (int i=0; i<%d; i++) in[i] = FALSE; };\n"
// 	      "void set_of_%s::setemptyset()\n"
// 	      "{ for (int i=0; i<%d; i++) in[i] = FALSE; };\n"
// 	      "void set_of_%s::setfullset()\n"
// 	      "{ for (int i=0; i<%d; i++) in[i] = TRUE; };\n"
// 	      "void set_of_%s::setsingleton(int a)\n"
// 	      "{ for (int i=0; i<%d; i++) in[i] = FALSE; in[a]= TRUE;};\n"
// 	      "bool set_of_%s::issingleton()\n"
// 	      "{ int a = 0; for (int i=0; i<%d; i++) if (in[i]) a++;\n"
// 	      "  if (a==1) return TRUE; return FALSE; };\n"
// 	      "int set_of_%s::getsingleton()\n"
// 	      "{ for (int i=0; i<%d; i++) if (in[i]) return i; };\n"
// 	      "void set_of_%s::setdifferent(set_of_%s s)\n"
// 	      "{ for (int i=0; i<%d; i++)\n"
// 	      "   if (in[i] && s.in[i]) in[i] = FALSE; };\n"
// 	      "bool set_of_%s::issubset(set_of_%s s)\n"
// 	      "{ for (int i=0; i<%d; i++)\n"
// 	      "	   if (!in[i] && s.in[i]) return FALSE; return TRUE;};\n"
// 	      "int set_of_%s::getsize()\n"
// 	      "{ int ret = 0; for (int i=0; i<%d; i++) if (in[i]) ret++; return ret; };\n"
// 	      "void set_of_%s::setinterset(set_of_%s s)\n"
// 	      "{ for (int i=0; i<%d; i++)\n"
// 	      "  if (! (in[i] && s.in[i])) in[i] = FALSE; };\n"
// 	      "bool set_of_%s::interset(set_of_%s s)\n"
// 	      "{ for (int i=0; i<%d; i++) if (in[i] && s.in[i]) return TRUE; \n"
// 	      "  return FALSE;  };\n"
// 	      "bool set_of_%s::isempty()\n"
// 	      "{ for (int i=0; i<%d; i++) if (in[i]) return FALSE; return TRUE; };\n"
// 	      "bool set_of_%s::isfull()\n"
// 	      "{ for (int i=0; i<%d; i++) if (!in[i]) return FALSE; return TRUE; };\n",
// 	      t->mu_name,
// 	      t->mu_name, size[i], t->mu_name, t->mu_name, t->mu_name, t->mu_name, t->mu_name,
// 	      t->mu_name, t->mu_name, size[i],
// 	      t->mu_name, size[i],
// 	      t->mu_name, size[i],
// 	      t->mu_name, size[i],
// 	      t->mu_name, size[i],
// 	      t->mu_name, size[i],
// 	      t->mu_name, t->mu_name, size[i],
// 	      t->mu_name, t->mu_name, size[i],
// 	      t->mu_name, size[i],
// 	      t->mu_name, t->mu_name, size[i],
// 	      t->mu_name, t->mu_name, size[i],
// 	      t->mu_name, size[i],
// 	      t->mu_name, size[i]
// 	      );
//     }
}

/********************
  generate permutation set class
  ********************/

void symmetryclass::generate_permutation_class()
{
  int i;
  stelist *member;
  scalarsettypedecl *t;
  
  fprintf(codefile,
          "\n/********************\n"
          " Permutation Set Class\n"
          " ********************/\n");

  // generate permutation set class
  fprintf( codefile, 
           "class PermSet\n"
           "{\n"
           "public:\n"
	   "  // book keeping\n"
	   "  enum PresentationType {Simple, Explicit};\n"
	   "  PresentationType Presentation;\n"
	   "\n"
	   "  void ResetToSimple();\n"
	   "  void ResetToExplicit();\n"
	   "  void SimpleToExplicit();\n"
	   "  void SimpleToOne();\n"
	   "  bool NextPermutation();\n"
	   "\n"
	   "  void Print_in_size()\n"
	   "  { int ret=0; for (int i=0; i<count; i++) if (in[i]) ret++; cout << \"in_size:\" << ret << \"\\n\"; }\n"
	   "\n"
	   );

  fprintf(codefile,
          "\n  /********************\n"
          "   Simple and efficient representation\n"
          "   ********************/\n");
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "  int class_%s[%d];\n"
	       "  int undefined_class_%s;// has the highest class number\n"
	       "\n" 
	       "  void Print_class_%s();\n"
	       "  bool OnlyOneRemain_%s;\n"
	       "  bool MTO_class_%s()\n"
	       "  {\n"
	       "    int i,j;\n"
	       "    if (OnlyOneRemain_%s)\n"
	       "      return FALSE;\n"
	       "    for (i=0; i<%d; i++)\n"
	       "      for (j=0; j<%d; j++)\n"
	       "        if (i!=j && class_%s[i]== class_%s[j])\n"
	       "	    return TRUE;\n"
	       "    OnlyOneRemain_%s = TRUE;\n"
	       "    return FALSE;\n"
	       "  }\n",
	       t->mu_name,
	       size[i],
	       t->mu_name,
	       t->mu_name,
	       t->mu_name,
	       t->mu_name,
	       t->mu_name,
	       size[i],
	       size[i],
	       t->mu_name,
	       t->mu_name,
	       t->mu_name
	       );
    }

  fprintf( codefile, 
	   "  bool AlreadyOnlyOneRemain;\n"
	   "  bool MoreThanOneRemain();\n"
	   "\n"
	   );

  fprintf(codefile,
          "\n  /********************\n"
          "   Explicit representation\n"
          "  ********************/\n"
	  "  unsigned long size;\n"  // what are they?
	  "  unsigned long count;\n" // what are they?
	  "  // in will be of product of factorial sizes for fast canonicalize\n"
	  "  // in will be of size 1 for reduced local memory canonicalize\n"
	  "  bool * in;\n"
	  "\n");
  
  fprintf( codefile, 
	   "  // auxiliary for explicit representation\n"
	   "\n"
	   "  // in/perm/revperm will be of factorial size for fast canonicalize\n"
	   "  // they will be of size 1 for reduced local memory canonicalize\n"
	   "  // second range will be size of the scalarset\n"
	   );
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "  int * in_%s;\n"
	       "  typedef int arr_%s[%d];\n"
	       "  arr_%s * perm_%s;\n"
	       "  arr_%s * revperm_%s;\n"
	       "\n",
	       t->mu_name,
	       t->mu_name, size[i],
	       t->mu_name, t->mu_name,
	       t->mu_name, t->mu_name
	       );     
      fprintf( codefile, 
	       "  int size_%s[%d];\n"
	       "  bool reversed_sorted_%s(int start, int end);\n"
	       "  void reverse_reversed_%s(int start, int end);\n"
	       "\n",
	       t->mu_name, size[i],
	       t->mu_name,
	       t->mu_name
	       );     
    }
  
  fprintf( codefile, 
	   "  // procedure for explicit representation\n");
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
               "  bool ok%d(%s* perm, int size, %s k);\n",
               i,
               t->mu_name, 
               t->mu_name
               );
      fprintf( codefile, 
               "  void GenPerm%d(%s* perm, int size, unsigned long& index);\n",
               i, t->mu_name,
               i, t->mu_name,
               i, t->mu_name
               );
      fprintf( codefile,"\n");
    }
  fprintf( codefile, 
	   "  // General procedure\n"
           "  PermSet();\n"
           "  bool In(int i) const { return in[i]; };\n"
           "  void Add(int i) { for (int j=0; j<i; j++) in[j] = FALSE;};\n"
           "  void Remove(int i) { in[i] = FALSE; };\n"
           "};\n"
           );

  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "void PermSet::Print_class_%s()\n"
	       "{\n"
	       "  cout << \"class_%s:\\t\";\n"
	       "  for (int i=0; i<%d; i++)\n"
	       "    cout << class_%s[i];\n"
	       "  cout << \" \" << undefined_class_%s << \"\\n\";\n"
	       "}\n", 
	       t->mu_name, // procedure name
	       t->mu_name, // class_%s
	       size[i], // i-for
	       t->mu_name,
	       t->mu_name
	       );
    }

  fprintf( codefile, 
	   "bool PermSet::MoreThanOneRemain()\n"
	   "{\n"
	   "  int i,j;\n"
	   "  if (AlreadyOnlyOneRemain)\n"
	   "    return FALSE;\n"
	   "  else {\n"
	   );
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "    for (i=0; i<%d; i++)\n"
	       "      for (j=0; j<%d; j++)\n"
	       "        if (i!=j && class_%s[i]== class_%s[j])\n"
	       "	    return TRUE;\n",
	       t->getsize(),
	       t->getsize(),
	       t->mu_name,
	       t->mu_name
	       );
    }
  fprintf( codefile, 
	   "  }\n"
	   "  AlreadyOnlyOneRemain = TRUE;\n"
	   "  return FALSE;\n"
	   "}\n"
	   );

  // ******************************
  // generate PermSet initiator
  // ******************************

  fprintf( codefile, 
           "PermSet::PermSet()\n"
	   ": Presentation(Simple)\n"
           "{\n"
           "  int i,j,k;\n"
	   );

  if (no_need_for_perm)
    fprintf( codefile, 
	     "  if (  args->sym_alg.mode == argsym_alg::Exhaustive_Fast_Canonicalize) {\n"
	     );
  else 
    fprintf( codefile, 
	     "  if (  args->sym_alg.mode == argsym_alg::Exhaustive_Fast_Canonicalize\n"
	     "     || args->sym_alg.mode == argsym_alg::Heuristic_Fast_Canonicalize) {\n"
	     );

  if (size_of_set > MAX_SCALARSET_PERM)
    fprintf( codefile, 
	   "    Error.Error(\"The sizes of the scalarsets are too large for this algorithm.\\n\");\n"
           );
  else
    {

  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    fprintf( codefile, 
	     "    %s Perm%d[%d];\n",
	     member->s->getvalue()->mu_name,
	     i, size[i]);

  fprintf(codefile,
          "\n  /********************\n"
          "   declaration of class variables\n"
          "  ********************/\n"
	  );
  fprintf( codefile, 
	   "  in = new bool[%ld];\n",
	   size_of_set);
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    fprintf( codefile, 
	     " in_%s = new int[%ld];\n"
	     " perm_%s = new arr_%s[%ld];\n"
	     " revperm_%s = new arr_%s[%ld];\n",
	     member->s->getvalue()->mu_name, size_of_set,
	     member->s->getvalue()->mu_name, member->s->getvalue()->mu_name, factorialsize[i],
	     member->s->getvalue()->mu_name, member->s->getvalue()->mu_name, factorialsize[i]
	     );

  fprintf( codefile, 
	   "\n"
	   "    // Set perm and revperm\n"
	   );

  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile,
	       "    count = 0;\n"
	       "    for (i=%d; i<=%d; i++)\n"
	       "      {\n"
	       "        Perm%d[0].value(i);\n"
	       "        GenPerm%d(Perm%d, 1, count);\n"
	       "      }\n"
	       "    if (count!=%ld)\n"
	       "      Error.Error( \"unable to initialize PermSet\");\n"
	       "    for (i=0; i<%ld; i++)\n"
	       "      for (j=%d; j<=%d; j++)\n"
	       "        for (k=%d; k<=%d; k++)\n"
	       "          if (revperm_%s[i][k-%d]==j)   // k - base \n"
	       "            perm_%s[i][j-%d]=k; // j - base \n",
	       t->getleft(),     // left bound in i-for
	       t->getright(),    // right bound in i-for
	       i,                // Perm%d
	       i,                // GenPerm%d
	       i,                // Perm%d
	       factorialsize[i], // if (index!=%d)
	       factorialsize[i], // right bound in i-for 
	       t->getleft(),     // left bound in j-for
	       t->getright(),    // right bound in j-for
	       t->getleft(),     // left bound in k-for
	       t->getright(),    // right bound in k-for
	       t->mu_name,       // perm%s
	       t->getleft(),     // k-%d
	       t->mu_name,       // revperm%s
	       t->getleft()      // j-%d
	       );
    }
  
  fprintf( codefile,
	   "\n"  
	   "    // setting up combination of permutations\n"
	   "    // for different scalarset\n"
	   "    int carry;\n"
	   );

  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "    int i_%s = 0;\n",
	       t->mu_name
	       );
    }

  fprintf( codefile,
	   "    size = %ld;\n"
	   "    count = %ld;\n"
	   "    for (i=0; i<%ld; i++)\n"
	   "      {\n"
	   "        carry = 1;\n"
	   "        in[i]= TRUE;\n",
	   size_of_set, size_of_set, size_of_set
	   );

  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "      in_%s[i] = i_%s;\n"
	       "      i_%s += carry;\n"
	       "      if (i_%s >= %ld) { i_%s = 0; carry = 1; } \n"
	       "      else { carry = 0; } \n",
	       t->mu_name, t->mu_name,
	       t->mu_name, 
	       t->mu_name, factorialsize[i], t->mu_name
	       );
    }

  fprintf( codefile, 
	   "    }\n"
	   );
  }

  fprintf( codefile, 
	   "  }\n"
	   "  else\n"
	   "  {\n"
	   );
  
  fprintf(codefile,
          "\n  /********************\n"
          "   declaration of class variables\n"
          "  ********************/\n"
	  );
  fprintf( codefile, 
	   "  in = new bool[1];\n");
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    fprintf( codefile, 
	     " in_%s = new int[1];\n"
	     " perm_%s = new arr_%s[1];\n"
	     " revperm_%s = new arr_%s[1];\n",
	     member->s->getvalue()->mu_name,
	     member->s->getvalue()->mu_name, member->s->getvalue()->mu_name,
	     member->s->getvalue()->mu_name, member->s->getvalue()->mu_name
	     );

  fprintf( codefile, 
	   "  in[0] = TRUE;\n");
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "    in_%s[0] = 0;\n",
	       t->mu_name);
    }

  fprintf( codefile, 
	   "  }\n"
           "}\n");

  // ****************************** 
  // generate ResetToSimple()
  // ****************************** 
  fprintf( codefile, 
           "void PermSet::ResetToSimple()\n"
           "{\n"
	   "  int i;\n"
	   );

  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "  for (i=0; i<%d; i++)\n"
	       "    class_%s[i]=0;\n"
	       "  undefined_class_%s=0;\n"
	       "  OnlyOneRemain_%s = FALSE;\n",
	       t->getsize(),
	       t->mu_name,
	       t->mu_name,
	       t->mu_name
	       );
    }
  fprintf( codefile, 
           "\n"
	   "  AlreadyOnlyOneRemain = FALSE;\n"
	   "  Presentation = Simple;\n"
           "}\n");

  // ****************************** 
  // generate ResetToExplicit()
  // ****************************** 
  if (size_of_set > MAX_SCALARSET_PERM)
    fprintf( codefile, 
	     "void PermSet::ResetToExplicit()\n"
	     "{\n"
	     "    Error.Error(\"The sizes of the scalarsets are too large for this algorithm.\\n\");\n"
	     "}\n"
           );
  else
    fprintf( codefile, 
	     "void PermSet::ResetToExplicit()\n"
	     "{\n"
	     "  for (int i=0; i<%ld; i++) in[i] = TRUE;\n"
	     "  Presentation = Explicit;\n"
	     "}\n",
	     size_of_set
	     );

  // ******************************
  // generate SimpleToExplicit
  // ******************************
  if (size_of_set > MAX_SCALARSET_PERM)
    fprintf( codefile, 
	     "void PermSet::SimpleToExplicit()\n"
	     "{\n"
	     "    Error.Error(\"The sizes of the scalarsets are too large for this algorithm.\\n\");\n"
	     "}\n"
           );
  else
    {
      fprintf( codefile, 
	       "void PermSet::SimpleToExplicit()\n"
	       "{\n"
	       "  int i,j,k;\n"
	       "  int start, class_size;\n"
	       );
      
      for (i=0, member = scalarsetlist;
	   member != NULL;
	   i++, member = member->next)
	fprintf( codefile, 
		 "  int start_%s[%d];\n"
		 "  int size_%s[%d];\n"
		 "  bool should_be_in_%s[%ld];\n",
		 member->s->getvalue()->mu_name, size[i],
		 member->s->getvalue()->mu_name, size[i],
		 member->s->getvalue()->mu_name, factorialsize[i]
		 );
      
      fprintf( codefile, 
	       "\n"
	       "  // Setup range for mapping\n"
	       );
      for (i=0, member = scalarsetlist;
	   member != NULL;
	   i++, member = member->next)
	{
	  t = (scalarsettypedecl *) member->s->getvalue();
	  fprintf( codefile,
		   "  start = 0;\n"
		   "  for (j=0; j<=undefined_class_%s; j++) // class number\n"
		   "    {\n"
		   "      class_size = 0;\n"
		   "      for (k=0; k<%d; k++) // step through class_mu_1_pid[k]\n"
		   "	if (class_%s[k]==j)\n"
		   "	  class_size++;\n"
		   "      for (k=0; k<%d; k++) // step through class_mu_1_pid[k]\n"
		   "	if (class_%s[k]==j)\n"
		   "	  {\n"
		   "	    size_%s[k] = class_size;\n"
		   "	    start_%s[k] = start;\n"
		   "	  }\n"
		   "      start+=class_size;\n"
		   "    }\n",
		   t->mu_name,   // j-for
		   t->getsize(), // k-for
		   t->mu_name,   // class_%s
		   t->getsize(), // k-for
		   t->mu_name,   // class_%s
		   t->mu_name,   // size_%s
		   t->mu_name    // start_%s
		   );
	}
      
      fprintf( codefile, 
	       "\n"
	       "  // To be In or not to be\n"
	       );
      for (i=0, member = scalarsetlist;
	   member != NULL;
	   i++, member = member->next)
	{
	  t = (scalarsettypedecl *) member->s->getvalue();
	  fprintf( codefile,
		   "  for (i=0; i<%ld; i++) // set up\n"
		   "    should_be_in_%s[i] = TRUE;\n"
		   "  for (i=0; i<%ld; i++) // to be in or not to be\n"
		   "    for (k=0; k<%d; k++) // step through class_mu_1_pid[k]\n"
		   "      if (! (perm_%s[i][k]-%d >=start_%s[k] \n"
		   "	     && perm_%s[i][k]-%d < start_%s[k] + size_%s[k]) )\n"
		   "  	    {\n"
		   "	      should_be_in_%s[i] = FALSE;\n"
		   "	      break;\n"
		   "	    }\n",
		   factorialsize[i], // i-for
		   t->mu_name, // should_be_in_%s
		   factorialsize[i], // i-for
		   t->getsize(),  // k-for
		   t->mu_name, t->getleft(), t->mu_name, // perm_%s ... start_%s
		   t->mu_name, t->getleft(), t->mu_name, t->mu_name, // perm_%s ... start_%s
		   t->mu_name // should_be_in_%s
		   );
	}
      
      fprintf( codefile, 
	       "\n"
	       "  // setup explicit representation \n"
	       "  // Set perm and revperm\n"
	       "  for (i=0; i<%ld; i++)\n"
	       "    {\n"
	       "      in[i] = TRUE;\n",
	       size_of_set
	       );
      for (i=0, member = scalarsetlist;
	   member != NULL;
	   i++, member = member->next)
	{
	  t = (scalarsettypedecl *) member->s->getvalue();
	  fprintf( codefile,
		   "      if (in[i] && !should_be_in_%s[in_%s[i]]) in[i] = FALSE;\n",
		   t->mu_name, t->mu_name
		   );
	}
      fprintf( codefile, 
	       "    }\n");
      
      fprintf( codefile, 
	       "  Presentation = Explicit;\n"
	       "  if (args->test_parameter1.value==0) Print_in_size();\n"
	       "}\n"
	       );
    }
  
  // ******************************
  // generate SimpleToOne
  // ******************************
  
//       fprintf( codefile, 
// 	       "void PermSet::SimpleToOne()\n"
// 	       "{\n"
// 	       "  int i,j,k;\n"
// 	       "  bool ok;\n"
// 	       "  int start, class_size;\n"
// 	       );
//       for (i=0, member = scalarsetlist;
// 	   member != NULL;
// 	   i++, member = member->next)
// 	{
// 	  fprintf( codefile, 
// 		   "  int start_%s[%d];\n"
// 		   "  int size_%s[%d];\n",
// 		   member->s->getvalue()->mu_name, size[i],
// 		   member->s->getvalue()->mu_name, size[i]
// 		   );
// 	}
//       
//       fprintf( codefile, 
// 	       "\n"
// 	       "  // Setup range for mapping\n"
// 	       );
//       for (i=0, member = scalarsetlist;
// 	   member != NULL;
// 	   i++, member = member->next)
// 	{
// 	  t = (scalarsettypedecl *) member->s->getvalue();
// 	  fprintf( codefile,
// 		   "  start = 0;\n"
// 		   "  for (j=0; j<=undefined_class_%s; j++) // class number\n"
// 		   "    {\n"
// 		   "      class_size = 0;\n"
// 		   "      for (k=0; k<%d; k++) // step through class_mu_1_pid[k]\n"
// 		   "	if (class_%s[k]==j)\n"
// 		   "	  class_size++;\n"
// 		   "      for (k=0; k<%d; k++) // step through class_mu_1_pid[k]\n"
// 		   "	if (class_%s[k]==j)\n"
// 		   "	  {\n"
// 		   "	    size_%s[k] = class_size;\n"
// 		   "	    start_%s[k] = start;\n"
// 		   "	  }\n"
// 		   "      start+=class_size;\n"
// 		   "    }\n",
// 		   t->mu_name,   // j-for
// 		   t->getsize(), // k-for
// 		   t->mu_name,   // class_%s
// 		   t->getsize(), // k-for
// 		   t->mu_name,   // class_%s
// 		   t->mu_name,   // size_%s
// 		   t->mu_name    // start_%s
// 		   );
// 	}
//       
//       fprintf( codefile, 
// 	       "\n"
// 	       "  // setup explicit representation \n"
// 	       "  // Set perm and revperm\n"
// 	       "  count = 1;\n"
// 	       "  in[0] = TRUE;\n"
// 	       );
//       for (i=0, member = scalarsetlist;
// 	   member != NULL;
// 	   i++, member = member->next)
// 	{
// 	  t = (scalarsettypedecl *) member->s->getvalue();
// 	  fprintf( codefile,
// 		   "  for (i=0; i<%ld; i++) // step through perm\n"
// 		   "    {\n"
// 		   "      ok = TRUE;\n"
// 		   "      for (k=0; k<%d; k++) // step through class[k]\n"
// 		   "	if (! (perm_%s[i][k]-%d >=start_%s[k] \n"
// 		   "	       && perm_%s[i][k]-%d < start_%s[k] + size_%s[k]) )\n"
// 		   "	  ok = FALSE;\n"
// 		   "      if (ok) break;\n"
// 		   "    }\n"
// 		   "  if (ok) in_%s[0] = i;\n"
// 		   "  else Error.Error(\"Internal: SimpleToOne()\\n\");\n",
// 		   factorialsize[i],
// 		   t->getsize(),  // k-for
// 		   t->mu_name, t->getleft(), t->mu_name, // perm_%s ... start_%s
// 		   t->mu_name, t->getleft(), t->mu_name, t->mu_name, // perm_%s ... start_%s
// 		   t->mu_name
// 		   );
// 	}
//       
//       fprintf( codefile, 
// 	       "  Presentation = Explicit;\n"
// 	       "}\n"
// 	       );
//     }

  fprintf( codefile, 
	   "void PermSet::SimpleToOne()\n"
	   "{\n"
	   "  int i,j,k;\n"
	   "  int class_size;\n"
	   "  int start;\n"
	   "\n"
	   );
  fprintf( codefile, 
	   "\n"
	   "  // Setup range for mapping\n"
	   );
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile,
	       "  start = 0;\n"
	       "  for (j=0; j<=undefined_class_%s; j++) // class number\n"
	       "    {\n"
	       "      class_size = 0;\n"
	       "      for (k=0; k<%d; k++) // step through class_mu_1_pid[k]\n"
	       "	if (class_%s[k]==j)\n"
	       "	  class_size++;\n"
	       "      for (k=0; k<%d; k++) // step through class_mu_1_pid[k]\n"
	       "	if (class_%s[k]==j)\n"
	       "	  {\n"
	       "	    size_%s[k] = class_size;\n"
	       "	  }\n"
	       "      start+=class_size;\n"
	       "    }\n",
	       t->mu_name,   // j-for
	       t->getsize(), // k-for
	       t->mu_name,   // class_%s
	       t->getsize(), // k-for
	       t->mu_name,   // class_%s
	       t->mu_name   // size_%s
	       );
    }
  
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile,
	       "  start = 0;\n"
	       "  for (j=0; j<=undefined_class_%s; j++) // class number\n"
	       "    {\n"
	       "      for (k=0; k<%d; k++) // step through class_mu_1_pid[k]\n"
	       "	    if (class_%s[k]==j)\n"
	       "	      revperm_%s[0][start++] = k+%d;\n"
	       "    }\n"
	       "  for (j=0; j<%d; j++)\n"
	       "    for (k=0; k<%d; k++)\n"
	       "      if (revperm_%s[0][k]==j+%d)\n"
	       "        perm_%s[0][j]=k+%d;\n",
	       t->mu_name,   // j-for
	       t->getsize(), // k-for
	       t->mu_name,   // class_%s
	       t->mu_name,   // revperm_%s
	       t->getleft(),  // k+%d
	       t->getsize(), // j-for
	       t->getsize(), // k-for
	       t->mu_name,   // revperm_%s
	       t->getleft(),  // j+%d
	       t->mu_name,   // revperm_%s
	       t->getleft()  // k+%d
	       );
    }
  
  fprintf( codefile, 
	   "  Presentation = Explicit;\n"
	   "}\n"
	   );
  
  // ******************************
  // generate ok and GenPerm
  // ******************************
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "bool PermSet::ok%d(%s* Perm, int size, %s k)\n"
	       "{\n"
	       "  for (int i=0; i<size; i++)\n"
	       "    if(Perm[i].value()==k)\n"
	       "      return FALSE;\n"
	       "  return TRUE;\n"
	       "}\n",
	       i, t->mu_name, t->mu_name);
      fprintf( codefile, 
	       "void PermSet::GenPerm%d(%s* Perm,int size, unsigned long& count)\n"
	       "{\n"
	       "  int i;\n"
	       "  if (size!=%d)\n"
	       "    {\n"
	       "      for (i=%d; i<=%d; i++)\n"
	       "        if(ok%d(Perm,size,i))\n"
	       "          {\n"
	       "            Perm[size].value(i);\n"
	       "            GenPerm%d(Perm, size+1, count);\n"
	       "          }\n"
	       "    }\n"
	       "  else\n"
	       "    {\n"
	       "      for (i=%d; i<=%d; i++)\n"
	       "        revperm_%s[count][i-%d]=Perm[i-%d].value();"
	       "// i - base\n"
	       "      count++;\n"
	       "    }\n"
	       "}\n",
	       i, t->mu_name, // GenPerm%d(%s
	       t->getsize(),  // size!=%d
	       t->getleft(), t->getright(), // i-for
	       i, i,          // ok%d, GenPerm%d
	       t->getleft(), t->getright(), // i-for
	       t->mu_name, t->getleft(), t->getleft() // revperm_%s...
	       );
    }

  // ******************************
  // generate Next Permutation
  // ******************************

  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      fprintf( codefile, 
	       "bool PermSet::reversed_sorted_%s(int start, int end)\n"
	       "{\n"
	       "  int i,j;\n"
	       "\n"
	       "  for (i=start; i<end; i++)\n"
	       "    if (revperm_%s[0][i]<revperm_%s[0][i+1])\n"
	       "      return FALSE;\n"
	       "  return TRUE;\n"
	       "}\n",
	       t->mu_name, t->mu_name, t->mu_name
	       );

      fprintf( codefile, 
	       "void PermSet::reverse_reversed_%s(int start, int end)\n"
	       "{\n"
	       "  int i,j;\n"
	       "  int temp;\n"
	       "\n"
	       "  for (i=start, j=end; i<j; i++,j--) \n"
	       "    {\n"
	       "      temp = revperm_%s[0][j];\n"
	       "      revperm_%s[0][j] = revperm_%s[0][i];\n"
	       "      revperm_%s[0][i] = temp;\n"
	       "    }\n"
	       "}\n",
	       t->mu_name, t->mu_name, t->mu_name, t->mu_name, t->mu_name
	       );
    }

  // ******************************
  // generate Next Permutation
  // ******************************

  fprintf( codefile, 
	   "bool PermSet::NextPermutation()\n"
	   "{\n"
	   "  bool nexted = FALSE;\n"
	   "  int start, end; \n"
	   "  int class_size;\n"
	   "  int temp;\n"
	   "  int j,k;\n"
	   "\n"
	   "  // algorithm\n"
	   "  // for each class\n"
	   "  //   if forall in the same class reverse_sorted, \n"
	   "  //     { sort again; goto next class }\n"
	   "  //   else\n"
	   "  //     {\n"
	   "  //       nexted = TRUE;\n"
	   "  //       for (j from l to r)\n"
	   "  // 	       if (for all j+ are reversed sorted)\n"
	   "  // 	         {\n"
	   "  // 	           swap j, j+1\n"
	   "  // 	           sort all j+ again\n"
	   "  // 	           break;\n"
	   "  // 	         }\n"
	   "  //     }\n"
	   );

  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
      if (i!=0) 
	fprintf( codefile, 
		 "if (!nexted) {\n"
		 );

      fprintf( codefile, 
	       "  for (start = 0; start < %d; )\n"
	       "    {\n"
	       "      end = start-1+size_%s[revperm_%s[0][start]-%d];\n"
	       "      if (reversed_sorted_%s(start,end))\n"
	       "	       {\n"
	       "	  reverse_reversed_%s(start,end);\n"
	       "	  start = end+1;\n"
	       "	}\n"
	       "      else\n"
	       "	{\n"
	       "	  nexted = TRUE;\n"
	       "	  for (j = start; j<end; j++)\n"
	       "	    {\n"
	       "	      if (reversed_sorted_%s(j+1,end))\n"
	       "		{\n"
	       "		  for (k = end; k>j; k--)\n"
	       "		    {\n"
	       "		      if (revperm_%s[0][j]<revperm_%s[0][k])\n"
	       "			{\n"
	       "			  // swap j, k\n"
	       "			  temp = revperm_%s[0][j];\n"
	       "			  revperm_%s[0][j] = revperm_%s[0][k];\n"
	       "			  revperm_%s[0][k] = temp;\n"
	       "			  break;\n"
	       "			}\n"
	       "		    }\n"
	       "		  reverse_reversed_%s(j+1,end);\n"
	       "		  break;\n"
	       "		}\n"
	       "	    }\n"
	       "	  break;\n"
	       "	}\n"
	       "    }\n",
	       size[i],
	       t->mu_name, t->mu_name, t->getleft(),
	       t->mu_name,
	       t->mu_name,
	       t->mu_name,
	       t->mu_name, t->mu_name, 
	       t->mu_name, t->mu_name, t->mu_name, t->mu_name, 
	       t->mu_name
	       );
      if (i!=0) 
	fprintf( codefile, 
		 "}\n"
		 );
    }

  fprintf( codefile, 
	   "if (!nexted) return FALSE;\n"
	   );
  
  for (i=0, member = scalarsetlist;
       member != NULL;
       i++, member = member->next)
    {
      t = (scalarsettypedecl *) member->s->getvalue();
	fprintf( codefile, 
		 "  for (j=%d; j<%d; j++)\n"
		 "    for (k=%d; k<%d; k++)\n"
		 "      if (revperm_%s[0][k]==j+%d)   // k - base \n"
		 "	perm_%s[0][j]=k+%d; // j - base \n",
		 0, size[i],
		 0, size[i],
		 t->mu_name, t->getleft(),
		 t->mu_name, t->getleft()
		 );
    }
  fprintf( codefile, 
	   "  return TRUE;\n"
	   "}\n"
	   );
  

}

/****************************************
  * 20 Dec 93 Norris Ip: 
  basic structures
  * 4 Feb 94 Norris Ip:
  Simplified to remove matrix representation.
beta2.9S released
  * 14 July 95 Norris Ip:
  Tidying/removing old algorithm
  adding small memory algorithm for larger scalarsets
****************************************/

/********************
 $Log: cpp_sym_aux.C,v $
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
