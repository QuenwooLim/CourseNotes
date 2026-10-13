/*
 * cpp_sym.C
 * @(#) Symmetry Code Generation
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
 * Created: 18 Nov 1993
 * 
 * Major Update:
 *
 * Denis Leroy
 * Subject: update to deal with resident working state
 * First updated: May 13, 95
 *
 * For details in creating and updating the file,
 * please check at the end.
 */ 

#include "mu.h"

void symmetryclass::generate_symmetry_function_decl()
  {
	fprintf(codefile,
		"  virtual void Permute(PermSet& Perm, int i);\n"
		"  virtual void SimpleCanonicalize(PermSet& Perm);\n"
		"  virtual void Canonicalize(PermSet& Perm);\n"
		"  virtual void SimpleLimit(PermSet& Perm);\n"
		"  virtual void ArrayLimit(PermSet& Perm);\n"
		"  virtual void Limit(PermSet& Perm);\n"
		"  virtual void MultisetLimit(PermSet& Perm);\n"
		);
  }

/********************
  main procedure to generate code for symmetry
  ********************/

void symmetryclass::generate_code(ste * globals)
{
  stelist * var;

  // output comment on overall scalarset types on all highest level variables
  // not necessary, just to help you analyse your variables 
  fprintf(codefile, "/*\n");
  set_var_list(globals);
  for ( var = varlist; var!= NULL; var=var->next )
      fprintf(codefile, "%s:%s\n",
              var->s->getname()->getname(),
              var->s->getvalue()->gettype()->structurestring()
              );
  fprintf(codefile, "*/\n");

  // classify scalarset types as useful or useless
  // it is not useful if 
  //    1) it was not given an explicit name
  //    2) it is used in one of the state variables
  for ( var = varlist; var!= NULL; var=var->next )
    var->s->getvalue()->gettype()->setusefulness();

  // remove useless scalarset from scalarset list stored in symmetryclass
  stelist ** entry = &scalarsetlist;
  stelist * member;
  for (member = scalarsetlist;
       member != NULL;  
       member = member->next)
    if (!((scalarsettypedecl *) member->s->getvalue())->isuseless())
      {
	*entry = member;
	entry = &(member->next);
      }
  *entry = NULL;

  // generate symmetry code
  generate_symmetry_code();
}

/********************
  generate canonicalization algorithm
  ********************/

void symmetryclass::generate_symmetry_code()
{
  fprintf(codefile,
          "\n/********************\n"
          "Code for symmetry\n"
          " ********************/\n");

  setup();
  generate_permutation_class();
  generate_symmetry_class();
  
  fprintf(codefile,
	  "\n/********************\n"
	  " Permute and Canonicalize function for different types\n"
	  " ********************/\n");
  if (typedecl::origin != NULL) 
    typedecl::origin->generate_all_symmetry_decl(*this);

  generate_match_function();

  generate_exhaustive_fast_canonicalization();
  generate_heuristic_fast_canonicalization();
  generate_heuristic_small_mem_canonicalization();
  generate_heuristic_fast_normalization();
}

/********************
  generate symmetry class
  ********************/
void symmetryclass::generate_symmetry_class()
{
  fprintf(codefile,
	  "\n/********************\n"
	  " Symmetry Class\n"
	  " ********************/\n");
  fprintf(codefile,
	  "class SymmetryClass\n"

	  "{\n"
	  "  PermSet Perm;\n"
	  "  bool BestInitialized;\n"
	  "  state BestPermutedState;\n"
	  "\n"
	  "  // utilities\n"
	  "  void SetBestResult(int i, state* temp);\n"
	  "  void ResetBestResult() {BestInitialized = FALSE;};\n"
	  "\n"
	  "public:\n"
	  "  // initializer\n"
	  "  SymmetryClass() : Perm(), BestInitialized(FALSE) {};\n"
	  "  ~SymmetryClass() {};\n"
	  "\n"
	  "  void Normalize(state* s);\n"
	  "\n"  
	  "  void Exhaustive_Fast_Canonicalize(state *s);\n"
	  "  void Heuristic_Fast_Canonicalize(state *s);\n"
	  "  void Heuristic_Small_Mem_Canonicalize(state *s);\n"
	  "  void Heuristic_Fast_Normalize(state *s);\n"
	  "\n"
	  "  void MultisetSort(state* s);\n"
	  "};\n"
	  "\n"
	  );

  fprintf(codefile,
	  "\n/********************\n"
	  " Symmetry Class Members\n"
	  " ********************/\n");
  fprintf(codefile,
	  "void SymmetryClass::MultisetSort(state* s)\n"
	  "{\n"
	  );
  for ( stelist * var = varlist; var != NULL; var = var->next )
    fprintf( codefile, 
	     "        %s.MultisetSort();\n",
             var->s->getvalue()->mu_name
             );
  fprintf(codefile,
	  "}\n"
	  );

  fprintf(codefile,
	  "void SymmetryClass::Normalize(state* s)\n"
	  "{\n"
	  "  switch (args->sym_alg.mode) {\n"
	  "  case argsym_alg::Exhaustive_Fast_Canonicalize:\n"
	  "    Exhaustive_Fast_Canonicalize(s);\n"
	  "    break;\n"
	  "  case argsym_alg::Heuristic_Fast_Canonicalize:\n"
	  "    Heuristic_Fast_Canonicalize(s);\n"
	  "    break;\n"
	  "  case argsym_alg::Heuristic_Small_Mem_Canonicalize:\n"
	  "    Heuristic_Small_Mem_Canonicalize(s);\n"
	  "    break;\n"
	  "  case argsym_alg::Heuristic_Fast_Normalize:\n"
	  "    Heuristic_Fast_Normalize(s);\n"
	  "    break;\n"
	  "  default:\n"
	  "    Heuristic_Fast_Canonicalize(s);\n"
	  "  }\n"
	  "}\n"
	  );
}

/********************
  generate old algorithm I :
  simple exhaustive canonicalization
  ********************/

void symmetryclass::generate_match_function()   // changes by Uli
{
  stelist * var;

  fprintf(codefile,
          "\n/********************\n"
          " Auxiliary function for error trace printing\n"
          " ********************/\n");

  fprintf( codefile, 
           "bool match(state* ns, StatePtr p)\n"
           "{\n"
           "  int i;\n"
	   "  static PermSet Perm;\n"
	   "  static state temp;\n"
	   "  StateCopy(&temp, ns);\n"
	   );

  fprintf( codefile, 
	   "  if (args->symmetry_reduction.value)\n"
	   "    {\n"
	   );

  // ********************
  // matching by permuting
  // ********************
  if (no_need_for_perm)
    fprintf( codefile, 
	     "      if (  args->sym_alg.mode == argsym_alg::Exhaustive_Fast_Canonicalize) {\n"
	     );
  else 
    fprintf( codefile, 
	     "      if (  args->sym_alg.mode == argsym_alg::Exhaustive_Fast_Canonicalize\n"
	     "         || args->sym_alg.mode == argsym_alg::Heuristic_Fast_Canonicalize) {\n"
	     );

  // matching by explicit data  
  fprintf( codefile, 
	   "        Perm.ResetToExplicit();\n"
           "        for (i=0; i<Perm.count; i++)\n"
           "          if (Perm.In(i))\n"
           "            {\n"
	   "              if (ns != workingstate)\n"
	   "                  StateCopy(workingstate, ns);\n"
           "              \n"
           );
  for ( var = varlist; var != NULL; var = var->next )
    fprintf( codefile, 
	     "              %s.Permute(Perm,i);\n"
	     "              if (args->multiset_reduction.value)\n"
	     "                %s.MultisetSort();\n",
             var->s->getvalue()->mu_name,
             var->s->getvalue()->mu_name
             );
  fprintf( codefile,
	   "            if (p.compare(workingstate)) {\n"
                          // changed by Uli
	   "              StateCopy(workingstate,&temp); return TRUE; }\n"
           "          }\n"
	   "        StateCopy(workingstate,&temp);\n"
           "        return FALSE;\n"
	   "      }\n"
	   );

  // matching by generating the permutation one by one
  fprintf( codefile, 
	   "      else {\n"
	   "        Perm.ResetToSimple();\n"
	   "        Perm.SimpleToOne();\n"
	   );

  // first iteration -- may be skipped? 
  fprintf( codefile, 
	   "        if (ns != workingstate)\n"
	   "          StateCopy(workingstate, ns);\n"
           "\n"
           );
  for ( var = varlist; var != NULL; var = var->next )
    fprintf( codefile, 
	     "          %s.Permute(Perm,0);\n"
	     "          if (args->multiset_reduction.value)\n"
	     "            %s.MultisetSort();\n",
             var->s->getvalue()->mu_name,
             var->s->getvalue()->mu_name
             );
  fprintf( codefile, 
	   "        if (p.compare(workingstate)) {\n"   // changed by Uli
	   "          StateCopy(workingstate,&temp); return TRUE; }\n"
	   "\n"
	   );

  // all other iterations
  fprintf( codefile, 
	   "        while (Perm.NextPermutation())\n"
           "          {\n"
	   "            if (ns != workingstate)\n"
	   "              StateCopy(workingstate, ns);\n"
           "              \n"
           );
  for ( var = varlist; var != NULL; var = var->next )
    fprintf( codefile, 
	     "              %s.Permute(Perm,0);\n"
	     "              if (args->multiset_reduction.value)\n"
	     "                %s.MultisetSort();\n",
             var->s->getvalue()->mu_name,
             var->s->getvalue()->mu_name
             );
  fprintf( codefile, 
	   "            if (p.compare(workingstate)) {\n"   // changed by Uli
	   "              StateCopy(workingstate,&temp); return TRUE; }\n"
           "          }\n"
	   "        StateCopy(workingstate,&temp);\n"
           "        return FALSE;\n"
	   "      }\n"
	   );
	   
  // end matching by permuting
  fprintf( codefile, 
	   "    }\n"
	   );

  // matching by multisesort
  fprintf( codefile, 
	   "  if (!args->symmetry_reduction.value\n"
	   "      && args->multiset_reduction.value)\n"
	   "    {\n"
	   "      if (ns != workingstate)\n"
	   "          StateCopy(workingstate, ns);\n"
	   );
  for ( var = varlist; var != NULL; var = var->next )
    fprintf( codefile, 
	     "      %s.MultisetSort();\n",
             var->s->getvalue()->mu_name
             );
  fprintf( codefile, 
	   "      if (p.compare(workingstate)) {\n"   // changed by Uli
	   "        StateCopy(workingstate,&temp); return TRUE; }\n"
	   "      StateCopy(workingstate,&temp);\n"
           "      return FALSE;\n"
	   "    }\n"
	   );

  // matching by direct comparsion
  fprintf( codefile, 
	   "  return (p.compare(ns));\n"   // changed by Uli
           "}\n"
           );
}

/********************
  generate old algorithm :
  fast exhaustive canonicalization
  ********************/

void symmetryclass::generate_exhaustive_fast_canonicalization()
{
  fprintf(codefile,
          "\n/********************\n"
          " Canonicalization by fast exhaustive generation of\n"
          " all permutations\n"
          " ********************/\n");

  fprintf(codefile, 
	  "void SymmetryClass::Exhaustive_Fast_Canonicalize(state* s)\n"
	  "{\n"
	  "  int i;\n"
	  "  static state temp;\n"
	  "  Perm.ResetToExplicit();\n"
          "\n"
	  );

  for ( stelist * var = varlist; var != NULL; var = var->next )
    fprintf(codefile, 
	    "  StateCopy(&temp, workingstate);\n"
            "  ResetBestResult();\n"
            "  for (i=0; i<Perm.count; i++)\n"
            "    if (Perm.In(i))\n"
            "      {\n"
            "        StateCopy(workingstate, &temp);\n"
            "        %s.Permute(Perm,i);\n"
	    "        if (args->multiset_reduction.value)\n"
	    "          %s.MultisetSort();\n" 
            "        SetBestResult(i, workingstate);\n"
            "      }\n"
            "  StateCopy(workingstate, &BestPermutedState);\n"
            "\n",
            var->s->getvalue()->mu_name,
            var->s->getvalue()->mu_name
            );
  fprintf(codefile,
	  "};\n"); 
}

/********************
  generate normalization algorithm
  ********************/
void symmetryclass::generate_heuristic_fast_normalization()
{
  stelist * var;
  bool ToOne;
  
  fprintf(codefile,
          "\n/********************\n"
          " Normalization by fast simple variable canonicalization,\n"
	  " fast simple scalarset array canonicalization,\n"
	  " fast restriction on permutation set with simple scalarset array of scalarset,\n"
	  " and for all other variables, pick any remaining permutation\n" 
          " ********************/\n");
  
  fprintf(codefile, 
	  "void SymmetryClass::Heuristic_Fast_Normalize(state* s)\n"
	  "{\n"
	  "  int i;\n"
	  "  static state temp;\n"
	  "\n"
          "  Perm.ResetToSimple();\n"
          "\n"
	  );
  
  // simple variable
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetVariable()
	&& (var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetArrayOfFree
	    || var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetVariable
 	    || var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree) )
      fprintf(codefile, 
	      "  %s.SimpleCanonicalize(Perm);\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );
  
  
  // simple scalarset array
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetArrayOfFree()
        && ( var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetArrayOfFree
	     || var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree) )
      fprintf(codefile, 
	      "  %s.Canonicalize(Perm);\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // multiset
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  %s.MultisetSort();\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // if they are inside more complicated structure, only do a limit
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetVariable()
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.SimpleLimit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetArrayOfFree()
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.ArrayLimit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // refine Permutation set by checking graph structure of scalarset array of Scalarset
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetArrayOfS())
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.Limit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // checking if we need to change simple to one
  ToOne = FALSE;
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() != typedecl::NoScalarset
        && var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      ToOne = TRUE;
  if (ToOne)
    fprintf(codefile, 
	    "  Perm.SimpleToOne();\n"
	    "\n"
	    );

  // others
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() != typedecl::NoScalarset
        && var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  %s.Permute(Perm,0);\n"
	      "  if (args->multiset_reduction.value)\n"
	      "    %s.MultisetSort();\n" 
	      "\n",
	      var->s->getvalue()->mu_name,
	      var->s->getvalue()->mu_name,
	      var->s->getvalue()->mu_name,
	      var->s->getvalue()->mu_name
	      );

  fprintf(codefile,
	  "};\n"); 
}

/********************
  generate new algorithm IV :
  fast canonicalization for simple variable and simple scalarset array
  and also fast restriction in permutation set with simple scalarset array of scalarset
  and fast restriction for multiset
  ********************/
void symmetryclass::generate_heuristic_fast_canonicalization()
{
  stelist * var;
  bool ToExplicit;

  fprintf(codefile,
          "\n/********************\n"
          " Canonicalization by fast simple variable canonicalization,\n"
	  " fast simple scalarset array canonicalization,\n"
	  " fast restriction on permutation set with simple scalarset array of scalarset,\n"
	  " and fast exhaustive generation of\n"
          " all permutations for other variables\n"
          " ********************/\n");

  fprintf(codefile, 
	  "void SymmetryClass::Heuristic_Fast_Canonicalize(state* s)\n"
	  "{\n"
	  "  int i;\n"
	  "  static state temp;\n"
	  "\n"
          "  Perm.ResetToSimple();\n"
          "\n"
	  );

  // simple variable
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetVariable()
 	&& (var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetArrayOfFree
 	    || var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetVariable
 	    || var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree) )
      fprintf(codefile, 
	      "  %s.SimpleCanonicalize(Perm);\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // simple scalarset array
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetArrayOfFree()
        && ( var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetArrayOfFree
	     || var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree) )
      fprintf(codefile, 
	      "  %s.Canonicalize(Perm);\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // multiset
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  %s.MultisetSort();\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // if they are inside more complicated structure, only do a limit
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetVariable()
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.SimpleLimit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetArrayOfFree()
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.ArrayLimit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // refine Permutation set by checking graph structure of multiset array of Scalarset
  for ( var = varlist; var != NULL; var = var->next )
     if (var->s->getvalue()->gettype()->HasMultisetOfScalarset())
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.MultisetLimit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // refine Permutation set by checking graph structure of scalarset array of Scalarset
  for ( var = varlist; var != NULL; var = var->next )
     if (var->s->getvalue()->gettype()->HasScalarsetArrayOfS())
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.Limit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // checking if we need to change simple to explicit
  if (!no_need_for_perm)
    fprintf(codefile, 
	    "  Perm.SimpleToExplicit();\n"
	    "\n"
	    );

  // handle all other cases by explicit/exhaustive enumeration  
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() != typedecl::NoScalarset
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  StateCopy(&temp, workingstate);\n"
	      "  ResetBestResult();\n"
	      "  for (i=0; i<Perm.count; i++)\n"
	      "    if (Perm.In(i))\n"
	      "      {\n"
	      "        StateCopy(workingstate, &temp);\n"
	      "        %s.Permute(Perm,i);\n"
	      "        if (args->multiset_reduction.value)\n"
	      "          %s.MultisetSort();\n" 
	      "        SetBestResult(i, workingstate);\n"
	      "      }\n"
	      "  StateCopy(workingstate, &BestPermutedState);\n"
	      "\n",
	      var->s->getvalue()->mu_name,
	      var->s->getvalue()->mu_name
	      );
      
  fprintf(codefile,
	  "};\n"); 
}

/********************
  generate new algorithm V :
  fast canonicalization for simple variable and simple scalarset array
  and also fast restriction in permutation set with simple scalarset array of scalarset
  and fast restriction for multiset
  -- use less local memory
  ********************/
void symmetryclass::generate_heuristic_small_mem_canonicalization()
{
  stelist * var;
  bool ToExplicit;
  
  fprintf(codefile,
          "\n/********************\n"
          " Canonicalization by fast simple variable canonicalization,\n"
	  " fast simple scalarset array canonicalization,\n"
	  " fast restriction on permutation set with simple scalarset array of scalarset,\n"
	  " and fast exhaustive generation of\n"
          " all permutations for other variables\n"
	  " and use less local memory\n" 
          " ********************/\n");
  
  fprintf(codefile, 
	  "void SymmetryClass::Heuristic_Small_Mem_Canonicalize(state* s)\n"
	  "{\n"
	  "  unsigned long cycle;\n"
	  "  static state temp;\n"
	  "\n"
          "  Perm.ResetToSimple();\n"
          "\n"
	  );

  // simple variable
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetVariable()
 	&& (var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetArrayOfFree
 	    || var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetVariable
 	    || var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree) )
      fprintf(codefile, 
	      "  %s.SimpleCanonicalize(Perm);\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // simple scalarset array
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetArrayOfFree()
        && ( var->s->getvalue()->gettype()->getstructure() == typedecl::ScalarsetArrayOfFree
	     || var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree) )
      fprintf(codefile, 
	      "  %s.Canonicalize(Perm);\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // multiset
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() == typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  %s.MultisetSort();\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // if they are inside more complicated structure, only do a limit
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetVariable()
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.SimpleLimit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->HasScalarsetArrayOfFree()
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.ArrayLimit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // refine Permutation set by checking graph structure of multiset array of Scalarset
  for ( var = varlist; var != NULL; var = var->next )
     if (var->s->getvalue()->gettype()->HasMultisetOfScalarset())
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.MultisetLimit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // refine Permutation set by checking graph structure of scalarset array of Scalarset
  for ( var = varlist; var != NULL; var = var->next )
     if (var->s->getvalue()->gettype()->HasScalarsetArrayOfS())
      fprintf(codefile, 
	      "  if (Perm.MoreThanOneRemain()) {\n"
	      "    %s.Limit(Perm);\n"
	      "  }\n"
	      "\n",
	      var->s->getvalue()->mu_name
	      );

  // checking if we need to change simple to explicit
  ToExplicit = FALSE;
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() != typedecl::NoScalarset
        && var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      ToExplicit = TRUE;
  if (ToExplicit)
    fprintf(codefile, 
	    "  Perm.SimpleToOne();\n"
	    "\n"
	    "  StateCopy(&temp, workingstate);\n"
	    "  ResetBestResult();\n"
	    );

  // handle all other cases by explicit/exhaustive enumeration  
  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() != typedecl::NoScalarset
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "  %s.Permute(Perm,0);\n"
	      "  if (args->multiset_reduction.value)\n"
	      "    %s.MultisetSort();\n",
	      var->s->getvalue()->mu_name,
	      var->s->getvalue()->mu_name
	      );
      
  if (!no_need_for_perm)
    fprintf(codefile, 
	    "  BestPermutedState = *workingstate;\n"
	    "  BestInitialized = TRUE;\n"
	    "\n"
	    "  cycle=1;\n"
	    "  while (Perm.NextPermutation())\n"
	    "    {\n"
	    "      if (args->perm_limit.value != 0\n"
	    "          && cycle++ >= args->perm_limit.value) break;\n"
	    "      StateCopy(workingstate, &temp);\n"
	    );

  for ( var = varlist; var != NULL; var = var->next )
    if (var->s->getvalue()->gettype()->getstructure() != typedecl::NoScalarset
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& var->s->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile, 
	      "      %s.Permute(Perm,0);\n"
	      "      if (args->multiset_reduction.value)\n"
	      "        %s.MultisetSort();\n",
	      var->s->getvalue()->mu_name,
	      var->s->getvalue()->mu_name
	      );
      
  if (!no_need_for_perm)
    fprintf(codefile,
	    "      switch (StateCmp(workingstate, &BestPermutedState)) {\n"
	    "      case -1:\n"
	    "        BestPermutedState = *workingstate;\n"
	    "        break;\n"
	    "      case 1:\n"
	    "        break;\n"
	    "      case 0:\n"
	    "        break;\n"
	    "      default:\n"
	    "        Error.Error(\"funny return value from StateCmp\");\n"
	    "      }\n"
	    "    }\n"
	    "  StateCopy(workingstate, &BestPermutedState);\n"
	    "\n");
  fprintf(codefile,
	  "};\n"); 
}

/****************************************
  * 18 Nov 93 Norris Ip: 
  basic structures
  * 26 Nov 93 Norris Ip: 
  form varlist in class -> set_var_list(globals);
  rearrange varlist according to complexity of type and
  order of declaration in program.
  * 2 Dec 93 Norris Ip:
  generate perm class
  * 3 Dec 93 Norris Ip:
  generate simple exhaustive canonicalization
  * 7 Dec 93 Norris Ip:
  added typedecl::generate_permute_function()
  to complete the simple exhaustive canonicalization
  *** completed the old simple/fast exhaustive canonicalization
  *** for one scalarset
  * 8 Dec 93 Norris Ip:
  added typedecl::generate_canonicalize_function()
  for simple varible only
           void symmetryclass::generate_fast_canonicalization_1()
  * 13 Dec 93 Norris Ip:
  added typedecl::generate_canonicalize_function()
  for simple scalarset array (inefficient)
           void symmetryclass::generate_fast_canonicalization_2()
  * 13 Dec 93 Norris Ip:
  generate class set for easy manipulation of set
  * 14 Dec 93 Norris Ip:
  added typedecl::generate_canonicalize_function()
  for simple scalarset array
  using fast simple variable/array canonicalization and other
  variables using fast exhaustive
  * 20 Dec 93 Norris Ip:
  move auxiliary code to cpp_sym_aux.C and cpp_sym_decl.C
  created scalarset array of scalarset permutation set restriction
           void symmetryclass::generate_fast_canonicalization_3()
  * 31 Jan 94 Norris Ip:
  removed useless scalarset types from scalarsetlist
  * 22 Feb 94 Norris Ip:
  add fast normalize code
  * 25 Feb 94 Norris Ip:
  Fixes bugs about sym alg executed in the wrong order
  Added hasScalarsetVariable() and HasScalarsetArrayOfFree()
  * 6 April 94 Norris Ip:
  generate code for Match() for correct error trace printing
beta2.9S released
  * 14 July 95 Norris Ip:
  Tidying/removing old algorithm
  adding small memory algorithm for larger scalarsets
****************************************/

/********************
 $Log: cpp_sym.C,v $
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
