/* -*- C++ -*-
 * cpp_sym_decl.C
 * @(#) Symmetry Code Generation for Variabous Typedecl
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
 * Update:
 *
 * Denis Leroy and Norris Ip
 * Subject: removed useless run-time checking,
 *          reduced Limit functions
 * Last modified: 18 June 1995
 *
 */ 

#include "mu.h"

/********************
  Main procedure to generate symmetry code for typedecl
  ********************/

void typedecl::generate_all_symmetry_decl(symmetryclass& symmetry)
{
  if (next != NULL)
    {
      next->generate_all_symmetry_decl(symmetry);
    }
  generate_permute_function();
  generate_simple_canon_function(symmetry);
  generate_canonicalize_function(symmetry);
  generate_simple_limit_function(symmetry);
  generate_array_limit_function(symmetry);
  generate_limit_function(symmetry);
  generate_multisetlimit_function(symmetry);
}

/********************
  generate scalarset list
  ********************/
charlist * enumtypedecl::generate_scalarset_list(charlist * sl) { return sl; }
charlist * subrangetypedecl::generate_scalarset_list(charlist * sl) { return sl; }
charlist * scalarsettypedecl::generate_scalarset_list(charlist * sl)
{
  if (getsize() > 1 ) 
    {
      if (sl==NULL)
	return new charlist("", this, NULL);
      else
	{
	  for (charlist * sv=sl; sv!=NULL; sv=sv->next)
	    if (sv->e==this) return sl;
	  return new charlist("", this, sl);
	}
    }
  else
    return sl;
}
charlist * uniontypedecl::generate_scalarset_list(charlist * sl)
{
  stelist * t;
  typedecl * d;
  bool notexist;
  charlist * sv;

  for(t=getunionmembers(); t!= NULL; t=t->next)
    {
      d = (typedecl *)t->s->getvalue();
      if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
	{
	  if (sl==NULL)
	    sl = new charlist("", d, NULL);
	  else
	    {
	      notexist = TRUE;
	      for (sv=sl; sv!=NULL; sv=sv->next)
		if (sv->e==d) notexist = FALSE;
	      if (notexist) sl = new charlist("", d, sl);
	    }
	}
    }
  return sl;
}
charlist * multisettypedecl::generate_scalarset_list(charlist * sl)
{ 
  sl = elementtype->generate_scalarset_list(sl);
  return sl;
}
charlist * arraytypedecl::generate_scalarset_list(charlist * sl)
{ 
  sl = elementtype->generate_scalarset_list(sl);
  sl = indextype->generate_scalarset_list(sl);
  return sl;
}
charlist * recordtypedecl::generate_scalarset_list(charlist * sl)
{
  ste *f; 
  
  for (f = fields; f != NULL; f = f->next)
    sl = f->getvalue()->gettype()->generate_scalarset_list(sl);
  return sl;
}

/********************
  generate permute() for each typedecl
  ********************/
void enumtypedecl::generate_permute_function()
{
  if (already_generated_permute_function) return;
  else already_generated_permute_function = TRUE;
  
  if (strcmp(mu_name,"mu_0_boolean")!=0) 
    fprintf(codefile,
            "void %s::Permute(PermSet& Perm, int i) {};\n",
            mu_name);
};
void subrangetypedecl::generate_permute_function()
{
  if (already_generated_permute_function) return;
  else already_generated_permute_function = TRUE;
  
  fprintf(codefile,
          "void %s::Permute(PermSet& Perm, int i) {};\n",
          mu_name);
};
void scalarsettypedecl::generate_permute_function()
{
  if (already_generated_permute_function) return;
  else already_generated_permute_function = TRUE;
  
  if (!useless && getsize() > 1)
    fprintf(codefile,
            "void %s::Permute(PermSet& Perm, int i)\n"
            "{\n"
            "  if (Perm.Presentation != PermSet::Explicit)\n"
            "    Error.Error(\"Internal Error: Wrong Sequence of Normalization\");\n" 
            "  if (defined())\n"
            "    value(Perm.perm_%s[Perm.in_%s[i]][value()-%d]); // value - base\n"
            "};\n",
            mu_name,  
            mu_name,
            mu_name,
            left);
  else
    fprintf(codefile,
            "void %s::Permute(PermSet& Perm, int i) {}\n",
            mu_name);
}

void uniontypedecl::generate_permute_function()
{
  if (already_generated_permute_function) return;
  else already_generated_permute_function = TRUE;
  
  if (useless) 
    {
      fprintf(codefile,
              "void %s::Permute(PermSet& Perm, int i) {}\n",
              mu_name);
    }
  else
    {
      fprintf(codefile,
              "void %s::Permute(PermSet& Perm, int i)\n"
              "{\n"
              "  if (Perm.Presentation != PermSet::Explicit)\n"
              "    Error.Error(\"Internal Error: Wrong Sequence of Normalization\");\n" 
              "  if (defined()) {\n",
              mu_name);  
      
      stelist * t;
      typedecl * d;  
      int thisright = getsize();
      int thisleft;
      for(t=unionmembers; t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          thisleft = thisright-d->getsize()+1;
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
            fprintf(codefile,
                    "    if ( ( value() >= %d ) && ( value() <= %d ) )\n"
                    "      value(Perm.perm_%s[Perm.in_%s[i]][value()-%d]+(%d)); // value - base\n",
//                    thisleft, thisright, // fixing union permutation bug
		    d->getleft(), d->getright(),
                    d->generate_code(),
                    d->generate_code(),
//                    thisleft, thisleft - d->getleft() // fixing union permutation bug
		    d->getleft(), 0
                    );
          thisright= thisleft-1;
        }
      fprintf(codefile,
              "  }\n"
              "}\n"
              );
    }
}

void multisettypedecl::generate_permute_function()
{
  if (already_generated_permute_function) return;
  else already_generated_permute_function = TRUE;

  elementtype->generate_permute_function();
  
  fprintf(codefile,
          "void %s::Permute(PermSet& Perm, int i)\n"
          "{\n"
          "  static %s temp(\"Permute_%s\",-1);\n"
          "  int j;\n"
          "  for (j=0; j<%d; j++)\n"
          "    array[j].Permute(Perm, i);\n",
          mu_name,              // %s::Permute
          mu_name, mu_name,     // %s temp("Permute_%s"...
          maximum_size // j-for
          );
  fprintf(codefile,"};\n");
}

void arraytypedecl::generate_permute_function()
{
  if (already_generated_permute_function) return;
  else already_generated_permute_function = TRUE;  

  elementtype->generate_permute_function();
  
  fprintf(codefile,
          "void %s::Permute(PermSet& Perm, int i)\n"
          "{\n"
          "  static %s temp(\"Permute_%s\",-1);\n"
          "  int j;\n"
          "  for (j=0; j<%d; j++)\n"
          "    array[j].Permute(Perm, i);\n",
          mu_name,              // %s::Permute
          mu_name, mu_name,     // %s temp("Permute_%s"...
          indextype->getsize() // j-for
          );
  
  if ( indextype->gettypeclass() == typedecl::Scalarset
       && indextype->getsize() > 1)
    {
      if (elementtype->issimple())
	fprintf(codefile,
		"  temp = *this;\n"
		"  for (j=%d; j<=%d; j++)\n"
		"     (*this)[j].value(temp[Perm.revperm_%s[Perm.in_%s[i]][j-%d]].value());",
		indextype->getleft(), indextype->getright(),     // j-for
		indextype->mu_name, indextype->mu_name, indextype->getleft()
		);
      else
	fprintf(codefile,
		"  temp = *this;\n"
		"  for (j=%d; j<=%d; j++)\n"
		"    (*this)[j] = temp[Perm.revperm_%s[Perm.in_%s[i]][j-%d]];",
		indextype->getleft(), indextype->getright(),     // j-for
		indextype->mu_name, indextype->mu_name, indextype->getleft()
		);
    }
  
  if ( indextype->gettypeclass() == typedecl::Union
       && ((uniontypedecl *)indextype)->IsUnionWithScalarset() )
    {
      stelist * t;
      typedecl * d;  
      
      for(t=((uniontypedecl *)indextype)->getunionmembers();
	  t!= NULL; t=t->next)
	{
	  d = (typedecl *)t->s->getvalue();
	  if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
	    { 
	      if (elementtype->issimple())
		fprintf(codefile,
			"  temp = *this;\n"
			"  for (j=%d; j<=%d; j++)\n"
			"      (*this)[j].value(temp[Perm.revperm_%s[Perm.in_%s[i]][j-%d]].value());\n",
			d->getleft(), d->getright(),          // j-for
			d->mu_name, d->mu_name, d->getleft()
			);
	      else
		fprintf(codefile,
			"  temp = *this;\n"
			"  for (j=%d; j<=%d; j++)\n"
			"    (*this)[j] = temp[Perm.revperm_%s[Perm.in_%s[i]][j-%d]];\n",
			d->getleft(), d->getright(),          // j-for
			d->mu_name, d->mu_name, d->getleft()
			);
	    }
	}
    }
  fprintf(codefile,"};\n");
}

void recordtypedecl::generate_permute_function()
{
  ste *f;
  
  if (already_generated_permute_function) return;
  else already_generated_permute_function = TRUE;
  
  for (f = fields; f != NULL; f = f->next)
    f->getvalue()->gettype()->generate_permute_function();
  
  fprintf(codefile,
          "void %s::Permute(PermSet& Perm, int i)\n"
          "{\n",
          mu_name
          );
  for (f = fields; f != NULL; f = f->next)
    if (f->getvalue()->gettype()->getstructure() != NoScalarset
	&& f->getvalue()->gettype()->getstructure() != MultisetOfFree)
      fprintf(codefile,
              "  %s.Permute(Perm,i);\n",
              f->getvalue()->generate_code()
              );
  fprintf(codefile,"};\n");
  
}

/********************
  supporting procedure for arraytypedecl
  ********************/
void arraytypedecl::generate_simple_sort()
{
  stelist * t;
  typedecl * d;  
  int thisright;
  int thisleft;

  if (indextype->gettypeclass() == typedecl::Scalarset
      && indextype->getsize() > 1)
    generate_simple_sort(indextype->getsize(), indextype->mu_name, indextype->getleft());
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset())
    {
      thisright = indextype->getsize()-1;
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          thisleft = thisright-d->getsize()+1;
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
//            generate_simple_sort(d->getsize(), d->mu_name, thisleft);
            generate_simple_sort(d->getsize(), d->mu_name, d->getleft());
          thisright= thisleft-1;
        }
    }
}
void arraytypedecl::generate_simple_sort(int size, char * name, int left)
{
  // please make sure that i,j,k,z can be modified
  fprintf(codefile,
	  "  // sorting %s\n"
	  "  // only if there is more than 1 permutation in class\n"
	  "  if (Perm.MTO_class_%s())\n"
	  "    {\n"
	  "      for (i=0; i<%d; i++)\n"
	  "        for (j=0; j<%d; j++)\n"
	  "          pos_%s[i][j]=FALSE;\n"
	  "      count_%s = 0;\n",
	  name, // sorting %s
	  name, // MTO_class_%s
	  size, size, name, // i-for j-for pos_%s
	  name // count_%s
	  );

  if (elementtype->issimple())
    {
      fprintf(codefile,
	      "      for (i=0; i<%d; i++)\n"
	      "        {\n"
	      "          for (j=0; j<count_%s; j++)\n"
	      "            {\n" 
	      "              compare = CompareWeight(value[j],"
	      "(*this)[i+%d]);\n"
	      "              if (compare==0)\n"
	      "                {\n"
	      "                  pos_%s[j][i]= TRUE;\n"
	      "                  break;\n"
	      "                }\n",
	      size, // i-for
	      name, // count_%s i.e. j-for
	      left, // array[i+%d]
	      name // pos_%s
	      );
      fprintf(codefile,
	      "              else if (compare>0)\n"
	      "                {\n"
	      "                  for (k=count_%s; k>j; k--)\n"
	      "                    {\n" 
	      "                      value[k].value(value[k-1].value());\n"
	      "                      for (z=0; z<%d; z++)\n"
	      "                        pos_%s[k][z] = pos_%s[k-1][z];\n"
	      "                    }\n"
	      "                  value[j].value((*this)[i+%d].value());\n"
	      "                  for (z=0; z<%d; z++)\n"
	      "                    pos_%s[j][z] = FALSE;\n"
	      "                  pos_%s[j][i] = TRUE;\n"
	      "                  count_%s++;\n"
	      "                  break;\n"
	      "                }\n"
	      "            }\n",
	      name, // count_%s
	      size, // z-for
	      name, name, // pos_%s pos_%s
              left, // array[i+%d]
	      size, // z-for
	      name, // pos_%s
	      name, // pos_%s
	      name // count_%s
	      );
      fprintf(codefile,
	      "          if (j==count_%s)\n"
	      "            {\n"
	      "                value[j].value((*this)[i+%d].value());\n"
	      "              for (z=0; z<%d; z++)\n"
	      "                pos_%s[j][z] = FALSE;\n"
	      "              pos_%s[j][i] = TRUE;\n"
	      "              count_%s++;\n"
	      "            }\n"
	      "        }\n"
	      "    }\n",
	      name, // count_%s
	      left,
	      size, // z-for
	      name, // pos_%s
	      name, // pos_%s
	      name  // count_%s
	      );
    }
  else
    {    
      fprintf(codefile,
	      "      for (i=0; i<%d; i++)\n"
	      "        {\n"
	      "          for (j=0; j<count_%s; j++)\n"
	      "            {\n" 
	      "              compare = CompareWeight(value[j],"
	      "(*this)[i+%d]);\n"
	      "              if (compare==0)\n"
	      "                {\n"
	      "                  pos_%s[j][i]= TRUE;\n"
	      "                  break;\n"
	      "                }\n",
	      size, // i-for
	      name, // count_%s i.e. j-for
	      left,
	      name // pos_%s
	      );
      fprintf(codefile,
	      "              else if (compare>0)\n"
	      "                {\n"
	      "                  for (k=count_%s; k>j; k--)\n"
	      "                    {\n" 
	      "                      value[k] = value[k-1];\n"
	      "                      for (z=0; z<%d; z++)\n"
	      "                        pos_%s[k][z] = pos_%s[k-1][z];\n"
	      "                    }\n"
	      "                  value[j] = (*this)[i+%d];\n"
	      "                  for (z=0; z<%d; z++)\n"
	      "                    pos_%s[j][z] = FALSE;\n"
	      "                  pos_%s[j][i] = TRUE;\n"
	      "                  count_%s++;\n"
	      "                  break;\n"
	      "                }\n"
	      "            }\n",
	      name, // count_%s
	      size, // z-for
	      name, name, // pos_%s pos_%s
	      left,
	      size, // z-for
	      name, // pos_%s
	      name, // pos_%s
	      name // count_%s
	      );
      fprintf(codefile,
	      "          if (j==count_%s)\n"
	      "            {\n"
	      "              value[j] = (*this)[i+%d];\n"
	      "              for (z=0; z<%d; z++)\n"
	      "                pos_%s[j][z] = FALSE;\n"
	      "              pos_%s[j][i] = TRUE;\n"
	      "              count_%s++;\n"
	      "            }\n"
	      "        }\n"
	      "    }\n",
	      name, // count_%s
	      left,
	      size, // z-for
	      name, // pos_%s
	      name, // pos_%s
	      name  // count_%s
	      );
    }
}
void arraytypedecl::generate_limit()
{
  stelist * t;
  typedecl * d;  

  if (indextype->gettypeclass() == typedecl::Scalarset
      && indextype->getsize() > 1)
    generate_limit(indextype->getsize(), indextype->mu_name);
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
	    generate_limit(d->getsize(), d->mu_name);
        }
    }
}
void arraytypedecl::generate_limit(int size, char * name)
{

  // please make sure that i,j,k can be modified
  fprintf(codefile,
	  "  // if there is more than 1 permutation in class\n"
	  "  if (Perm.MTO_class_%s() && count_%s>1)\n"
	  "    {\n"
          "      // limit\n"
          "      for (j=0; j<%d; j++) // class priority\n"
          "        {\n"
          "          for (i=0; i<count_%s; i++) // for value priority\n"
          "            {\n"
          "              exists = FALSE;\n"
          "              for (k=0; k<%d; k++) // step through class\n"
          "                goodset_%s[k] = FALSE;\n"
          "              for (k=0; k<%d; k++) // step through class\n"
          "                if (pos_%s[i][k] && Perm.class_%s[k] == j)\n"
          "                  {\n"
          "                    exists = TRUE;\n"
          "                    goodset_%s[k] = TRUE;\n"
          "                    pos_%s[i][k] = FALSE;\n"
          "                  }\n"
          "              if (exists)\n"
          "                {\n"
          "                  split=FALSE;\n"
          "                  for (k=0; k<%d; k++)\n"
          "                    if ( Perm.class_%s[k] == j && !goodset_%s[k] ) \n"
          "                      split= TRUE;\n"
          "                  if (split)\n"
          "                    {\n"
          "                      for (k=0; k<%d; k++)\n"
          "                        if (Perm.class_%s[k]>j\n"
          "                            || ( Perm.class_%s[k] == j && !goodset_%s[k] ) )\n"
          "                          Perm.class_%s[k]++;\n"
          "                      Perm.undefined_class_%s++;\n"
          "                    }\n"
          "                }\n"
          "            }\n"
          "        }\n"
          "    }\n",
	  name, name, // if guard
          size, // j-for
          name, // count_%s
          size, name, // k-for goodset_%s
          size, // k-for
          name, name, // pos_%s, class_%s 
          name, // goodset_%s
          name, // pos_%s
          size, // k-for
          name, name,  // class_%s, goodset_%s
          size, // k-for
          name,   // class_%s
          name, name,  // class_%s, goodset_%s
          name,   // class_%s
          name    // undefined_class_%s
          );
}
void arraytypedecl::generate_limit2(char * var, typedecl * e)
{
  stelist * t;
  typedecl * d;  

  if (indextype->gettypeclass() == typedecl::Scalarset
      && indextype->getsize() > 1)
    generate_limit2(indextype->getsize(), indextype->mu_name, var, indextype->getleft());
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
            generate_limit2(d->getsize(), d->mu_name, var, d->getleft());
        }
    }
}
void arraytypedecl::generate_limit2(int size, char * name, char * var, int left)
{
  // please make sure that i,j,k can be modified
  fprintf(codefile,
	  "\n"
          "  // refinement -- check undefine\n"
	  "  // only if there is more than 1 permutation in class\n"
	  "  if (Perm.MTO_class_%s() && count_%s<%d)\n"
	  "    {\n"
	  "      exists = FALSE;\n"
	  "      split = FALSE;\n"
	  "      // if there exists both defined value and undefined value\n"
          "      for (k=0; k<%d; k++) // step through class\n"
	  "        if ((*this)[k+%d]%s.isundefined())\n"
	  "          exists = TRUE;\n"
	  "        else\n"
	  "          split = TRUE;\n"
	  "      if (exists && split)\n"
	  "        {\n"
          "          for (i=0; i<count_%s; i++) // for value priority\n"
          "            {\n"
          "              exists = FALSE;\n"
          "              for (k=0; k<%d; k++) // step through class\n"
          "                goodset_%s[k] = FALSE;\n"
          "              for (k=0; k<%d; k++) // step through class\n"
          "                if (pos_%s[i][k] \n"
          "                    && (*this)[k+%d]%s.isundefined())\n"
          "                  {\n"
          "                    exists = TRUE;\n"
          "                    goodset_%s[k] = TRUE;\n"
          "                  }\n"
          "              if (exists)\n"
          "                {\n"
          "                  split=FALSE;\n"
          "                  for (k=0; k<%d; k++)\n"
          "                    if ( pos_%s[i][k] && !goodset_%s[k] ) \n"
          "                          split= TRUE;\n"
          "                  if (split)\n"
          "                    {\n"
          "                      for (j=count_%s; j>i; j--)\n"
          "                        for (k=0; k<%d; k++)\n"
          "                          pos_%s[j][k] = pos_%s[j-1][k];\n"
          "                      for (k=0; k<%d; k++)\n"
          "                        {\n"
          "                          if (pos_%s[i][k] && !goodset_%s[k])\n"
          "                            pos_%s[i][k] = FALSE;\n"
          "                          if (pos_%s[i+1][k] && goodset_%s[k])\n"
          "                            pos_%s[i+1][k] = FALSE;             \n"
          "                        }\n"
          "                      count_%s++; i++;\n"
          "                    }\n"
          "                }\n"
          "            }\n"
          "        }\n"
          "    }\n",
	  name, name, size, // if guard
	  size, // k-for
	  left, var,  // (*this)[k+%d]%s
          name, // count_%s
          size, name, // k-for goodset_%s
          size, // k-for
          name, left, var, // pos_%s, [k+%d], %s.isundefined()
          name, // goodset_%s
          
          size, // k-for
          name, name, // pos_%s goodset_%s
          name, // count_%s
          size, // k-for
          name, name,  // pos_%s, pos_%s
          size, // k-for
          
          name, name,  // pos_%s, goodset_%s
          name, // pos_%s
          name, name,  // class_%s, goodset_%s
          name, // pos_%s
          name  // count_%s
          );
}

// added by Uli
int multisettypedecl::get_num_limit_locals(typedecl * e)
{
  int ret, temp; ste * f;

  switch (e->gettypeclass()) {
  case typedecl::Scalarset:
  case typedecl::Enum:
  case typedecl::Range:
  case typedecl::Union:
    return 0;
  case typedecl::Record:
    ret = 0;
    for (f = e->getfields(); f != NULL; f = f->next)
      {
        temp = get_num_limit_locals(f->getvalue()->gettype());
        if (ret < temp) ret = temp;
      }
    return ret;
  case typedecl::Array:
    return 1 + get_num_limit_locals(e->getelementtype());
  }
  return 0;
}

int arraytypedecl::get_num_limit_locals(typedecl * e)
{
  int ret, temp; ste * f;

  switch (e->gettypeclass()) {
  case typedecl::Scalarset:
  case typedecl::Enum:
  case typedecl::Range:
  case typedecl::Union:
    return 0;
  case typedecl::Record:
    ret = 0;
    for (f = e->getfields(); f != NULL; f = f->next)
      {
	temp = get_num_limit_locals(f->getvalue()->gettype());
	if (ret < temp) ret = temp;
      }
    return ret;
  case typedecl::Array:
    return 1 + get_num_limit_locals(e->getelementtype());
  }
  return 0;
}

void arraytypedecl::generate_limit_step1(int limit_strategy)
{
  stelist * t;
  typedecl * d;  

  if (indextype->gettypeclass() == typedecl::Scalarset
      && indextype->getsize() > 1)
    generate_limit_step2(indextype, "", elementtype,0,limit_strategy);
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
	    generate_limit_step2(d, "", elementtype,0,limit_strategy);
        }
    }
}

void arraytypedecl::generate_limit_step2(typedecl * d, char * var, typedecl * e, int local_num, int limit_strategy)
{
  stelist * t;
  ste * f;
  typedecl * e2, * d2;  

  // please check that scalarset of size 1 is handled by sort in the other routine

  if (e->gettypeclass() == typedecl::Scalarset
      && e->getsize() > 1)
    generate_limit_step3(d, var, e, limit_strategy, FALSE);
  else if (e->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)e)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)e)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d2 = (typedecl *)t->s->getvalue();
          if (d2->gettypeclass() == typedecl::Scalarset && d2->getsize() > 1 ) 
	    generate_limit_step3(d, var, d2, limit_strategy, TRUE);
        }
    }
  else if (e->gettypeclass() == typedecl::Array)
    {
      e2 = e->getindextype();
      switch (e2->gettypeclass()) {
      case typedecl::Scalarset:
	if (e2->getsize() == 1)
	  generate_limit_step2(d, 
			       tsprintf("%s[mu_%s]",var,((scalarsettypedecl *)e2)->getidvalues()->getname()->getname()),
			       e->getelementtype(), local_num, limit_strategy);
	break;
      case typedecl::Union:
	for(t=((uniontypedecl *)e2)->getunionmembers();
	    t!= NULL; t=t->next)
	  {
	    d2 = (typedecl *)t->s->getvalue();
	    switch (d2->gettypeclass()) {
	    case typedecl::Enum:
	      for (f = ((enumtypedecl *) d2)->getidvalues();
		   f!=NULL && type_equal( f->getvalue()->gettype(), d2);
		   f = f->getnext()) 
		generate_limit_step2(d, 
				      tsprintf("%s[mu_%s]",var,f->getname()->getname()),
				      e->getelementtype(), local_num, limit_strategy);
	      break;
	    case typedecl::Range:
	      fprintf(codefile,
		      "  // loop through elements of a array indexed by subrange(union)\n"
		      "  for (i%d = %d; i%d <= %d; i%d++)\n"
		      "  {\n",
		      local_num, d2->getleft(), local_num, d2->getright(), local_num);
	      generate_limit_step2(d, 
				    tsprintf("%s[i%d]",var,local_num),
				    e->getelementtype(), local_num+1, limit_strategy);
	      fprintf(codefile,
		      "  }\n");
	      break;
	    }
	  }
      case typedecl::Enum:
	for (f = ((enumtypedecl *)e2)->getidvalues();
	     f!=NULL && type_equal( f->getvalue()->gettype(), e2);
	     f = f->getnext()) 
	  generate_limit_step2(d, 
				tsprintf("%s[mu_%s]",var,f->getname()->getname()),
				e->getelementtype(), local_num, limit_strategy);
	break;
      case typedecl::Range:
	fprintf(codefile,
		"  // loop through elements of a array indexed by subrange\n"
		"  for (i%d = %d; i%d <= %d; i%d++)\n"
		"  {\n",
		local_num, e2->getleft(), local_num, e2->getright(), local_num);
	generate_limit_step2(d, 
			      tsprintf("%s[i%d]",var,local_num),
			      e->getelementtype(), local_num+1, limit_strategy);
	fprintf(codefile,
		"  }\n");
	break;
      }
    }
  else if (e->gettypeclass() == typedecl::Record)
    {
      for (f = e->getfields(); f != NULL; f = f->next)
	{
	  generate_limit_step2(d, 
				tsprintf("%s.mu_%s",var, f->getname()->getname()),
				f->getvalue()->gettype(), local_num, limit_strategy);
	}
    }
}
void arraytypedecl::generate_limit_step3(typedecl * d, char * var, typedecl * e, int limit_strategy, bool isunion)
{
  switch (limit_strategy) {
  case 3:
    if (e == d)
      generate_limit3(d->getsize(), d->mu_name, var, d->getleft());
    break;
  case 4:
    if (e == d)
      generate_limit4(d->getsize(), d->mu_name, var, d->getleft(), isunion);
    break;
  case 5:
    if (e != d && e->getsize() > 1)
      generate_limit5(d->mu_name, d->getsize(), d->getleft(), var, e->mu_name, e->getsize(), e->getleft(), isunion);
    break;
  }
}

void arraytypedecl::generate_limit3(char * var, typedecl * e)
{
  stelist * t;
  typedecl * d;  

  if (indextype->gettypeclass() == typedecl::Scalarset
      && indextype->getsize() > 1)
    generate_limit3(indextype, var, e);

  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
	    generate_limit3(d, var, e);
        }
    }
}

void arraytypedecl::generate_limit3(typedecl * d, char * var, typedecl * e)
{
  stelist * t;
  typedecl * d2;  
  
  if (e == d)
    generate_limit3(d->getsize(), d->mu_name, var, d->getleft());
  else if (e->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)e)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)e)->getunionmembers();
	  t!= NULL; t=t->next)
	{
	  d2 = (typedecl *)t->s->getvalue();
	  if (d2 == d)
	    generate_limit3(d->getsize(), d->mu_name, var, d2->getleft());
	}
    }
}
void arraytypedecl::generate_limit3(int size, char * name, char * var, int left)
{
  // please make sure that i,j,k can be modified
  fprintf(codefile,
	  "\n"
          "  // refinement -- check selfloop\n"
	  "  // only if there is more than 1 permutation in class\n"
	  "  if (Perm.MTO_class_%s() && count_%s<%d)\n"
	  "    {\n"
	  "      exists = FALSE;\n"
	  "      split = FALSE;\n"
	  "      // if there exists both self loop and non-self loop\n"
          "      for (k=0; k<%d; k++) // step through class\n"
	  "        if ((*this)[k+%d]%s.isundefined()\n"
	  "            ||(*this)[k+%d]%s!=k+%d)\n"
	  "          exists = TRUE;\n"
	  "        else\n"
	  "          split = TRUE;\n"
	  "      if (exists && split)\n"
	  "        {\n"
          "          for (i=0; i<count_%s; i++) // for value priority\n"
          "            {\n"
          "              exists = FALSE;\n"
          "              for (k=0; k<%d; k++) // step through class\n"
          "                goodset_%s[k] = FALSE;\n"
          "              for (k=0; k<%d; k++) // step through class\n"
          "                if (pos_%s[i][k] \n"
          "                    && !(*this)[k+%d]%s.isundefined()\n"
          "                    && (*this)[k+%d]%s==k+%d)\n"
          "                  {\n"
          "                    exists = TRUE;\n"
          "                    goodset_%s[k] = TRUE;\n"
          "                  }\n"
          "              if (exists)\n"
          "                {\n"
          "                  split=FALSE;\n"
          "                  for (k=0; k<%d; k++)\n"
          "                    if ( pos_%s[i][k] && !goodset_%s[k] ) \n"
          "                          split= TRUE;\n"
          "                  if (split)\n"
          "                    {\n"
          "                      for (j=count_%s; j>i; j--)\n"
          "                        for (k=0; k<%d; k++)\n"
          "                          pos_%s[j][k] = pos_%s[j-1][k];\n"
          "                      for (k=0; k<%d; k++)\n"
          "                        {\n"
          "                          if (pos_%s[i][k] && !goodset_%s[k])\n"
          "                            pos_%s[i][k] = FALSE;\n"
          "                          if (pos_%s[i+1][k] && goodset_%s[k])\n"
          "                            pos_%s[i+1][k] = FALSE;\n"
          "                        }\n"
          "                      count_%s++; i++;\n"
          "                    }\n"
          "                }\n"
          "            }\n"
          "        }\n"
          "    }\n",
          name, name, size, // MTO_class count_%s<%d
	  size, // k-for
	  left, var,  // (*this)[k]%s
	  left, var, left, // (*this)[k]%s!=k+%d

          name, // count_%s
          size, name, // k-for goodset_%s
          size, // k-for
          name, // pos_%s
          left, var, // [i+%d]%s.isundefined()
          left, var, left, // [i+%d]%s== i+%d
          name, // goodset_%s
          
          size, // k-for
          name, name, // pos_%s goodset_%s
          name, // count_%s
          size, // k-for
          name, name,  // pos_%s, pos_%s
          size, // k-for
          
          name, name,  // pos_%s, goodset_%s
          name,   // pos_%s
          name, name,  // class_%s, goodset_%s
          name, // pos_%s
          name  // count_%s
          );
}
// void arraytypedecl::generate_limit4(int size, char * name, char * var, int left)
// {
//   fprintf(codefile,
//           "      // limit\n"
//           "      // check priority in general\n"
//           "      oldcount_%s = count_%s-1;\n"
//           "      while (oldcount_%s != count_%s && count_%s<%d)\n"
//           "        {\n"
//           "          oldcount_%s = count_%s;\n"
//           "          for (i=0; i<count_%s; i++) // for value priority\n"
//           "            {\n"
//           "              for (j=0; j<count_%s; j++) // for value priority\n"
//           "                {\n"
//           "                  exists = FALSE;\n"
//           "                  for (k=0; k<%d; k++) // step through class\n"
//           "                    goodset_%s[k] = FALSE;\n"
//           "                  for (k=0; k<%d; k++) // step through class\n"
//           "                    if (pos_%s[i][k] \n"
//           "                        && !(*this)[k+%d]%s.isundefined()\n"
//           "                        && (*this)[k+%d]%s!=k+%d\n"
//           "                        && pos_%s[j][(*this)[k+%d]%s-%d])\n"
//           "                      {\n"
//           "                        exists = TRUE;\n"
//           "                        goodset_%s[k] = TRUE;\n"
//           "                      }\n"
//           "                  if (exists)\n"
//           "                    {\n"
//           "                      split=FALSE;\n"
//           "                      for (k=0; k<%d; k++)\n"
//           "                        if ( pos_%s[i][k] && !goodset_%s[k] ) \n"
//           "                          split= TRUE;\n"
//           "                      if (split)\n"
//           "                        {\n"
//           "                          for (j=count_%s; j>i; j--)\n"
//           "                            for (k=0; k<%d; k++)\n"
//           "                              pos_%s[j][k] = pos_%s[j-1][k];\n"
//           "                          for (k=0; k<%d; k++)\n"
//           "                            {\n"
//           "                              if (pos_%s[i][k] && !goodset_%s[k])\n"
//           "                                pos_%s[i][k] = FALSE;\n"
//           "                              if (pos_%s[i+1][k] && goodset_%s[k])\n"
//           "                                pos_%s[i+1][k] = FALSE;                  \n"
//           "                            }\n"
//           "                          count_%s++;\n"
//           "                        }\n"
//           "                    }\n"
//           "                }\n"
//           "            }\n"
//           "         }\n",
//           name, name, // oldcount_%s count_%s
//           name, name, name, size, // oldcount_%s count_%s count_%s<%d
//           name, name, // oldcount_%s count_%s
//           name, // count_%s
//           name, // count_%s
//           size, name, // k-for goodset_%s
//           size, // k-for
//           name, // pos_%s
//           left, var, // [i+%d]%s.isundefined()
//           left, var, left, // [i+%d]%s== i+%d
//           name, left, var, left, // pos_%s[j][array[k+%d]%s-%d]
//           name, // goodset_%s
//           
//           size, // k-for
//           name, name, // pos_%s goodset_%s
//           name, // count_%s
//           size, // k-for
//           name, name,  // pos_%s, pos_%s
//           size, // k-for
//           
//           name, name,  // pos_%s, goodset_%s
//           name, // pos_%s
//           name, name,  // class_%s, goodset_%s
//           name, // pos_%s
//           name  // count_%s
//           );
// }

void arraytypedecl::generate_limit4(char * var, typedecl * e)
{
  stelist * t;
  typedecl * d;  

  if (indextype->gettypeclass() == typedecl::Scalarset
      && indextype->getsize() > 1)
    generate_limit4(indextype, var, e);
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
	    generate_limit4(d, var, e);
        }
    }
}
void arraytypedecl::generate_limit4(typedecl * d, char * var, typedecl * e)
{
  stelist * t;
  typedecl * d2;  
  
  if (e->gettypeclass() == typedecl::Scalarset
      && e == d)
    generate_limit4(d->getsize(), d->mu_name, var, d->getleft(),FALSE);
  else if (e->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)e)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)e)->getunionmembers();
	  t!= NULL; t=t->next)
	{
	  d2 = (typedecl *)t->s->getvalue();
	  if (d2 == d)
	    fprintf(codefile,
		    "\n"
		    "      // refinement -- graph structure for a single scalarset\n"
		    "      //               as in array S1 of S1\n"
		    "      // only if there is more than 1 permutation in class\n"
		    "      if (Perm.MTO_class_%s() && count_%s<%d)\n"
		    "        {\n"
		    "          exists = FALSE;\n"
		    "          split = FALSE;\n"
		    "          for (k=0; k<%d; k++) // step through class\n"
		    "            if (!(*this)[k+%d]%s.isundefined()\n"
		    "                &&(*this)[k+%d]%s!=k+%d\n"
		    "                &&(*this)[k+%d]%s>=%d\n"
		    "                &&(*this)[k+%d]%s<=%d)\n"
		    "              exists = TRUE;\n"
		    "          if (exists)\n"
		    "            {\n"
		    "              for (i=0; i<count_%s; i++) // for value priority\n"
		    "                {\n"
		    "                  for (j=0; j<count_%s; j++) // for value priority\n"
		    "                    {\n"
		    "                      exists = FALSE;\n"
		    "                      for (k=0; k<%d; k++) // step through class\n"
		    "                        goodset_%s[k] = FALSE;\n"
		    "                      for (k=0; k<%d; k++) // step through class\n"
		    "                        if (pos_%s[i][k] \n"
		    "                            && !(*this)[k+%d]%s.isundefined()\n"
		    "                            && (*this)[k+%d]%s!=k+%d\n"
		    "                            && (*this)[k+%d]%s>=%d\n"          // extra checking
		    "                            && (*this)[k+%d]%s<=%d\n"           // extra checking
		    "                            && pos_%s[j][(*this)[k+%d]%s-%d])\n"
		    "                          {\n"
		    "                            exists = TRUE;\n"
		    "                            goodset_%s[k] = TRUE;\n"
		    "                          }\n"
		    "                      if (exists)\n"
		    "                        {\n"
		    "                          split=FALSE;\n"
		    "                          for (k=0; k<%d; k++)\n"
		    "                            if ( pos_%s[i][k] && !goodset_%s[k] ) \n"
		    "                              split= TRUE;\n"
		    "                          if (split)\n"
		    "                            {\n"
		    "                              for (j=count_%s; j>i; j--)\n"
		    "                                for (k=0; k<%d; k++)\n"
		    "                                  pos_%s[j][k] = pos_%s[j-1][k];\n"
		    "                              for (k=0; k<%d; k++)\n"
		    "                                {\n"
		    "                                  if (pos_%s[i][k] && !goodset_%s[k])\n"
		    "                                    pos_%s[i][k] = FALSE;\n"
		    "                                  if (pos_%s[i+1][k] && goodset_%s[k])\n"
		    "                                    pos_%s[i+1][k] = FALSE;                  \n"
		    "                                }\n"
		    "                              count_%s++;\n"
		    "                            }\n"
		    "                        }\n"
		    "                    }\n"
		    "                }\n"
		    "            }\n"
		    "        }\n",
		    d->mu_name, d->mu_name, d->getsize(), // MTO_class count_%s<%d
		    d->getsize(), // k-for
		    d->getleft(), var,  // (*this)[k]%s
		    d->getleft(), var, d->getleft(), // (*this)[k]%s==k+%d
		    d->getleft(), var, d->getleft(), // (*this)[k]%s<%d
		    d->getleft(), var, d->getright(), // (*this)[k]%s>%d

		    d->mu_name, // count_%s
		    d->mu_name, // count_%s
		    d->getsize(), d->mu_name, // k-for goodset_%s
		    d->getsize(), // k-for
		    d->mu_name, // pos_%s
		    d->getleft(), var, // [i+%d]%s.isundefined()
		    d->getleft(), var, d->getleft(), // [i+%d]%s== i+%d
		    d->getleft(), var, d->getleft(), // [i+%d]%s>=%d
		    d->getleft(), var, d->getright(), // [i+%d]%s<=%d
		    d->mu_name, d->getleft(), var, d->getleft(), // pos_%s[j][(*this)[k+%d]%s-%d]
		    d->mu_name, // goodset_%s
		    
		    d->getsize(), // k-for
		    d->mu_name, d->mu_name, // pos_%s goodset_%s
		    d->mu_name, // count_%s
		    d->getsize(), // k-for
		    d->mu_name, d->mu_name,  // pos_%s, pos_%s
		    d->getsize(), // k-for
		    
		    d->mu_name, d->mu_name,  // pos_%s, goodset_%s
		    d->mu_name, // pos_%s
		    d->mu_name, d->mu_name,  // class_%s, goodset_%s
		    d->mu_name, // pos_%s
		    d->mu_name  // count_%s
		    );
	}
    }
}
void arraytypedecl::generate_limit4(int size, char * name, char * var, int left, bool isunion)
{
  if (isunion) 
    fprintf(codefile,
	    "\n"
	    "      // refinement -- graph structure for a single scalarset\n"
	    "      //               as in array S1 of S1\n"
	    "      // only if there is more than 1 permutation in class\n"
	    "      if (Perm.MTO_class_%s() && count_%s<%d)\n"
	    "        {\n"
	    "          exists = FALSE;\n"
	    "          split = FALSE;\n"
	    "          for (k=0; k<%d; k++) // step through class\n"
	    "            if (!(*this)[k+%d]%s.isundefined()\n"
	    "                &&(*this)[k+%d]%s!=k+%d\n"
	    "                &&(*this)[k+%d]%s>=%d\n"
	    "                &&(*this)[k+%d]%s<=%d)\n"
	    "              exists = TRUE;\n"
	    "          if (exists)\n"
	    "            {\n"
	    "              for (i=0; i<count_%s; i++) // for value priority\n"
	    "                {\n"
	    "                  for (j=0; j<count_%s; j++) // for value priority\n"
	    "                    {\n"
	    "                      exists = FALSE;\n"
	    "                      for (k=0; k<%d; k++) // step through class\n"
	    "                        goodset_%s[k] = FALSE;\n"
	    "                      for (k=0; k<%d; k++) // step through class\n"
	    "                        if (pos_%s[i][k] \n"
	    "                            && !(*this)[k+%d]%s.isundefined()\n"
	    "                            && (*this)[k+%d]%s!=k+%d\n"
	    "                            && (*this)[k+%d]%s>=%d\n"          // extra checking
	    "                            && (*this)[k+%d]%s<=%d\n"           // extra checking
	    "                            && pos_%s[j][(*this)[k+%d]%s-%d])\n"
	    "                          {\n"
	    "                            exists = TRUE;\n"
	    "                            goodset_%s[k] = TRUE;\n"
	    "                          }\n"
	    "                      if (exists)\n"
	    "                        {\n"
	    "                          split=FALSE;\n"
	    "                          for (k=0; k<%d; k++)\n"
	    "                            if ( pos_%s[i][k] && !goodset_%s[k] ) \n"
	    "                              split= TRUE;\n"
	    "                          if (split)\n"
	    "                            {\n"
	    "                              for (j=count_%s; j>i; j--)\n"
	    "                                for (k=0; k<%d; k++)\n"
	    "                                  pos_%s[j][k] = pos_%s[j-1][k];\n"
	    "                              for (k=0; k<%d; k++)\n"
	    "                                {\n"
	    "                                  if (pos_%s[i][k] && !goodset_%s[k])\n"
	    "                                    pos_%s[i][k] = FALSE;\n"
	    "                                  if (pos_%s[i+1][k] && goodset_%s[k])\n"
	    "                                    pos_%s[i+1][k] = FALSE;                  \n"
	    "                                }\n"
	    "                              count_%s++;\n"
	    "                            }\n"
	    "                        }\n"
	    "                    }\n"
	    "                }\n"
	    "            }\n"
	    "        }\n",
	    name, name, size, // MTO_class count_%s<%d
	    size, // k-for
	    left, var,  // (*this)[k]%s
	    left, var, left, // (*this)[k]%s==k+%d
	    left, var, left, // (*this)[k]%s<%d
	    left, var, left+size-1, // (*this)[k]%s>%d
	    
	    name, // count_%s
	    name, // count_%s
	    size, name, // k-for goodset_%s
	    size, // k-for
	    name, // pos_%s
	    left, var, // [i+%d]%s.isundefined()
	    left, var, left, // [i+%d]%s== i+%d
	    left, var, left, // [i+%d]%s>=%d
	    left, var, left+size-1, // [i+%d]%s<=%d
	    name, left, var, left, // pos_%s[j][(*this)[k+%d]%s-%d]
	    name, // goodset_%s
	    
	    size, // k-for
	    name, name, // pos_%s goodset_%s
	    name, // count_%s
	    size, // k-for
	    name, name,  // pos_%s, pos_%s
	    size, // k-for
	    
	    name, name,  // pos_%s, goodset_%s
	    name, // pos_%s
	    name, name,  // class_%s, goodset_%s
	    name, // pos_%s
	    name  // count_%s
	    );
  else 
    fprintf(codefile,
	    "\n"
	    "      // refinement -- graph structure for a single scalarset\n"
	    "      //               as in array S1 of S1\n"
	    "      // only if there is more than 1 permutation in class\n"
	    "      if (Perm.MTO_class_%s() && count_%s<%d)\n"
	    "        {\n"
	    "          exists = FALSE;\n"
	    "          split = FALSE;\n"
	    "          // if there exists non-self/undefine loop\n"
	    "          for (k=0; k<%d; k++) // step through class\n"
	    "            if (!(*this)[k+%d]%s.isundefined()\n"
	    "                &&(*this)[k+%d]%s!=k+%d)\n"
	    "              exists = TRUE;\n"
	    "          if (exists)\n"
	    "            {\n"
	    "              for (i=0; i<count_%s; i++) // for value priority\n"
	    "                {\n"
	    "                  for (j=0; j<count_%s; j++) // for value priority\n"
	    "                    {\n"
	    "                      exists = FALSE;\n"
	    "                      for (k=0; k<%d; k++) // step through class\n"
	    "                        goodset_%s[k] = FALSE;\n"
	    "                      for (k=0; k<%d; k++) // step through class\n"
	    "                        if (pos_%s[i][k] \n"
	    "                            && !(*this)[k+%d]%s.isundefined()\n"
	    "                            && (*this)[k+%d]%s!=k+%d\n"
	    "                            && pos_%s[j][(*this)[k+%d]%s-%d])\n"
	    "                          {\n"
	    "                            exists = TRUE;\n"
	    "                            goodset_%s[k] = TRUE;\n"
	    "                          }\n"
	    "                      if (exists)\n"
	    "                        {\n"
	    "                          split=FALSE;\n"
	    "                          for (k=0; k<%d; k++)\n"
	    "                            if ( pos_%s[i][k] && !goodset_%s[k] ) \n"
	    "                              split= TRUE;\n"
	    "                          if (split)\n"
	    "                            {\n"
	    "                              for (j=count_%s; j>i; j--)\n"
	    "                                for (k=0; k<%d; k++)\n"
	    "                                  pos_%s[j][k] = pos_%s[j-1][k];\n"
	    "                              for (k=0; k<%d; k++)\n"
	    "                                {\n"
	    "                                  if (pos_%s[i][k] && !goodset_%s[k])\n"
	    "                                    pos_%s[i][k] = FALSE;\n"
	    "                                  if (pos_%s[i+1][k] && goodset_%s[k])\n"
	    "                                    pos_%s[i+1][k] = FALSE;                  \n"
	    "                                }\n"
	    "                              count_%s++;\n"
	    "                            }\n"
	    "                        }\n"
	    "                    }\n"
	    "                }\n"
	    "            }\n"
	    "        }\n",
	    name, name, size, // MTO_class count_%s<%d
	    size, // k-for
	    left, var,  // (*this)[k]%s
	    left, var, left, // (*this)[k]%s==k+%d
	    
	    name, // count_%s
	    name, // count_%s
	    size, name, // k-for goodset_%s
	    size, // k-for
	    name, // pos_%s
	    left, var, // [i+%d]%s.isundefined()
	    left, var, left, // [i+%d]%s== i+%d
	    name, left, var, left, // pos_%s[j][(*this)[k+%d]%s-%d]
	    name, // goodset_%s
	    
	    size, // k-for
	    name, name, // pos_%s goodset_%s
	    name, // count_%s
	    size, // k-for
	    name, name,  // pos_%s, pos_%s
	    size, // k-for
	    
	    name, name,  // pos_%s, goodset_%s
	    name, // pos_%s
	    name, name,  // class_%s, goodset_%s
	    name, // pos_%s
	    name  // count_%s
	    );
}
void arraytypedecl::generate_limit5(char * var, typedecl * e)
{
  stelist * t;
  typedecl * d;  

  if (indextype->gettypeclass() == typedecl::Scalarset
      && indextype->getsize() > 1)
    generate_limit5(indextype, var, e);
  else if (indextype->gettypeclass() == typedecl::Union
	  && ((uniontypedecl *)indextype)->IsUnionWithScalarset() )
    {
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1)
	    generate_limit5(d, var, e);
        }
    }
}
void arraytypedecl::generate_limit5(typedecl * d, char * var, typedecl * e)
{
  stelist * t;
  typedecl * d2;  
  int thisright;
  int thisleft;

  if (e->gettypeclass() == typedecl::Scalarset
      && e!=d
      && e->getsize() > 1)
    generate_limit5(d->mu_name, d->getsize(), d->getleft(),
		    var,
		    e->mu_name, e->getsize(), e->getleft(),FALSE);
  else if (e->gettypeclass() == typedecl::Union)
    {
      thisright = e->getsize()-1;
      for(t=((uniontypedecl *)e)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d2 = (typedecl *)t->s->getvalue();
          thisleft = thisright-d2->getsize()+1;
          if (d2->gettypeclass() == typedecl::Scalarset && d2->getsize() > 1 && d2!=d)
	    fprintf(codefile,
		    "\n"
		    "      // refinement -- graph structure for two scalarsets\n"
		    "      //               as in array S1 of S2\n"
		    "      // only if there is more than 1 permutation in class\n"
		    "      if ( (Perm.MTO_class_%s() && count_%s<%d)\n"
		    "           || ( Perm.MTO_class_%s() && count_%s<%d) )\n"
		    "        {\n"
		    "          exists = FALSE;\n"
		    "          split = FALSE;\n"
		    "          for (k=0; k<%d; k++) // step through class\n"
		    "            if ((*this)[k+%d]%s.isundefined()\n"
		    "                ||(*this)[k+%d]%s<%d\n"
		    "                ||(*this)[k+%d]%s>%d)\n"
		    "              exists = TRUE;\n"
		    "            else\n"
		    "              split = TRUE;\n"
		    "          if (exists && split)\n"
		    "            {\n"
		    "              for (i=0; i<count_%s; i++) // scan through array index priority\n"
		    "                for (j=0; j<count_%s; j++) //scan through element priority\n"
		    "                  {\n"
		    "                    exists = FALSE;\n"
		    "                    for (k=0; k<%d; k++) // initialize goodset\n"
		    "                      goodset_%s[k] = FALSE;\n"
		    "                    for (k=0; k<%d; k++) // initialize goodset\n"
		    "                      goodset_%s[k] = FALSE;\n"
		    "                    for (k=0; k<%d; k++) // scan array index\n"
		    "                      // set goodsets\n"
		    "                      if (pos_%s[i][k] \n"
		    "                          && !(*this)[k+%d]%s.isundefined()\n"
		    "                          && (*this)[k+%d]%s>=%d\n"          // extra checking
		    "                          && (*this)[k+%d]%s<=%d\n"           // extra checking
		    "                          && pos_%s[j][(*this)[k+%d]%s-%d])\n"
		    "                        {\n"
		    "                          exists = TRUE;\n"
		    "                          goodset_%s[k] = TRUE;\n"
		    "                          goodset_%s[(*this)[k+%d]%s-%d] = TRUE;\n"
		    "                        }\n"
		    "                    if (exists)\n"
		    "                      {\n"
		    "                        // set split for the array index type\n" 
		    "                        split=FALSE;\n"
		    "                        for (k=0; k<%d; k++)\n"
		    "                          if ( pos_%s[i][k] && !goodset_%s[k] )\n"
		    "                            split= TRUE;\n"
		    "                        if (split)\n"
		    "                          {\n"
		    "                            // move following pos entries down 1 step\n"
		    "                            for (z=count_%s; z>i; z--)\n"
		    "                              for (k=0; k<%d; k++)\n"
		    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
		    "                            // split pos\n"
		    "                            for (k=0; k<%d; k++)\n"
		    "                              {\n"
		    "                                if (pos_%s[i][k] && !goodset_%s[k])\n"
		    "                                  pos_%s[i][k] = FALSE;\n"
		    "                                if (pos_%s[i+1][k] && goodset_%s[k])\n"
		    "                                  pos_%s[i+1][k] = FALSE;\n"
		    "                              }\n"
		    "                            count_%s++;\n"
		    "                          }\n"
		    "                        // set split for the element type\n" 
		    "                        split=FALSE;\n"
		    "                        for (k=0; k<%d; k++)\n"
		    "                          if ( pos_%s[j][k] && !goodset_%s[k] )\n"
		    "                            split= TRUE;\n"
		    "                        if (split)\n"
		    "                          {\n"
		    "                            // move following pos entries down 1 step\n"
		    "                            for (z=count_%s; z>j; z--)\n"
		    "                              for (k=0; k<%d; k++)\n"
		    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
		    "                            // split pos\n"
		    "                            for (k=0; k<%d; k++)\n"
		    "                              {\n"
		    "                                if (pos_%s[j][k] && !goodset_%s[k])\n"
		    "                                  pos_%s[j][k] = FALSE;\n"
		    "                                if (pos_%s[j+1][k] && goodset_%s[k])\n"
		    "                                  pos_%s[j+1][k] = FALSE;\n"
		    "                              }\n"
		    "                            count_%s++;\n"
		    "                          }\n"
		    "                      }\n"
		    "                  }\n"
		    "            }\n"
		    "        }\n",
		    d->mu_name, d->mu_name, d->getsize(), // if guard1
		    d2->mu_name, d2->mu_name, d2->getsize(), // if guard2
		    d->getsize(), // k-for
		    d->getleft(), var, // (*this)[k_%d]%s
// 		    d->getleft(), var, thisleft, // (*this)[k_%d]%s<%d
// 		    d->getleft(), var, thisright, // (*this)[k_%d]%s>%d
		    d->getleft(), var, d2->getleft(), // (*this)[k_%d]%s<%d
		    d->getleft(), var, d2->getright(), // (*this)[k_%d]%s>%d
		    
		    d->mu_name, // i-f
		    d2->mu_name, // j-for
		    d->getsize(), d->mu_name, // initialize goodset
		    d2->getsize(), d2->mu_name, // initialize goodset
		    d->getsize(), // scan array index
		    d->mu_name, d->getleft(), var,               // if condition
// 		    d->getleft(), var, thisleft, // (*this)[k_%d]%s<%d
// 		    d->getleft(), var, thisright, // (*this)[k_%d]%s>%d
		    d->getleft(), var, d2->getleft(), // (*this)[k_%d]%s<%d
		    d->getleft(), var, d2->getright(), // (*this)[k_%d]%s>%d
		    d2->mu_name, d->getleft(), var, d2->getleft(), // if condition (cont)
		    d->mu_name, d2->mu_name, d->getleft(), var, d2->getleft(), // set goodsets
		    
		    d->getsize(), // k-for -- split
		    d->mu_name, d->mu_name, // if condition
		    d->mu_name, d->getsize(), d->mu_name, d->mu_name, // move pos down 1 step
		    d->getsize(), d->mu_name, d->mu_name, d->mu_name, // split pos
		    d->mu_name, d->mu_name, d->mu_name,               // split pos (cont)
		    d->mu_name, // count_%s
		    
		    d2->getsize(), // k-for -- split
		    d2->mu_name, d2->mu_name, // if condition
		    d2->mu_name, d2->getsize(), d2->mu_name, d2->mu_name, // move pos down 1 step
		    d2->getsize(), d2->mu_name, d2->mu_name, d2->mu_name, // split pos
		    d2->mu_name, d2->mu_name, d2->mu_name,               // split pos (cont)
		    d2->mu_name // count_%s
		    
		    );
          thisright= thisleft-1;
        }
    }
}
void arraytypedecl::generate_limit5(char * name1, int size1, int left1,
				    char * var,
				    char * name2, int size2, int left2, bool isunion)
{
  if (isunion)
    fprintf(codefile,
	    "\n"
	    "      // refinement -- graph structure for two scalarsets\n"
	    "      //               as in array S1 of S2\n"
	    "      // only if there is more than 1 permutation in class\n"
	    "      if ( (Perm.MTO_class_%s() && count_%s<%d)\n"
	    "           || ( Perm.MTO_class_%s() && count_%s<%d) )\n"
	    "        {\n"
	    "          exists = FALSE;\n"
	    "          split = FALSE;\n"
	    "          for (k=0; k<%d; k++) // step through class\n"
	    "            if ((*this)[k+%d]%s.isundefined()\n"
	    "                ||(*this)[k+%d]%s<%d\n"
	    "                ||(*this)[k+%d]%s>%d)\n"
	    "              exists = TRUE;\n"
	    "            else\n"
	    "              split = TRUE;\n"
	    "          if (exists && split)\n"
	    "            {\n"
	    "              for (i=0; i<count_%s; i++) // scan through array index priority\n"
	    "                for (j=0; j<count_%s; j++) //scan through element priority\n"
	    "                  {\n"
	    "                    exists = FALSE;\n"
	    "                    for (k=0; k<%d; k++) // initialize goodset\n"
	    "                      goodset_%s[k] = FALSE;\n"
	    "                    for (k=0; k<%d; k++) // initialize goodset\n"
	    "                      goodset_%s[k] = FALSE;\n"
	    "                    for (k=0; k<%d; k++) // scan array index\n"
	    "                      // set goodsets\n"
	    "                      if (pos_%s[i][k] \n"
	    "                          && !(*this)[k+%d]%s.isundefined()\n"
	    "                          && (*this)[k+%d]%s>=%d\n"          // extra checking
	    "                          && (*this)[k+%d]%s<=%d\n"           // extra checking
	    "                          && pos_%s[j][(*this)[k+%d]%s-%d])\n"
	    "                        {\n"
	    "                          exists = TRUE;\n"
	    "                          goodset_%s[k] = TRUE;\n"
	    "                          goodset_%s[(*this)[k+%d]%s-%d] = TRUE;\n"
	    "                        }\n"
	    "                    if (exists)\n"
	    "                      {\n"
	    "                        // set split for the array index type\n" 
	    "                        split=FALSE;\n"
	    "                        for (k=0; k<%d; k++)\n"
	    "                          if ( pos_%s[i][k] && !goodset_%s[k] )\n"
	    "                            split= TRUE;\n"
	    "                        if (split)\n"
	    "                          {\n"
	    "                            // move following pos entries down 1 step\n"
	    "                            for (z=count_%s; z>i; z--)\n"
	    "                              for (k=0; k<%d; k++)\n"
	    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
	    "                            // split pos\n"
	    "                            for (k=0; k<%d; k++)\n"
	    "                              {\n"
	    "                                if (pos_%s[i][k] && !goodset_%s[k])\n"
	    "                                  pos_%s[i][k] = FALSE;\n"
	    "                                if (pos_%s[i+1][k] && goodset_%s[k])\n"
	    "                                  pos_%s[i+1][k] = FALSE;\n"
	    "                              }\n"
	    "                            count_%s++;\n"
	    "                          }\n"
	    "                        // set split for the element type\n" 
	    "                        split=FALSE;\n"
	    "                        for (k=0; k<%d; k++)\n"
	    "                          if ( pos_%s[j][k] && !goodset_%s[k] )\n"
	    "                            split= TRUE;\n"
	    "                        if (split)\n"
	    "                          {\n"
	    "                            // move following pos entries down 1 step\n"
	    "                            for (z=count_%s; z>j; z--)\n"
	    "                              for (k=0; k<%d; k++)\n"
	    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
	    "                            // split pos\n"
	    "                            for (k=0; k<%d; k++)\n"
	    "                              {\n"
	    "                                if (pos_%s[j][k] && !goodset_%s[k])\n"
	    "                                  pos_%s[j][k] = FALSE;\n"
	    "                                if (pos_%s[j+1][k] && goodset_%s[k])\n"
	    "                                  pos_%s[j+1][k] = FALSE;\n"
	    "                              }\n"
	    "                            count_%s++;\n"
	    "                          }\n"
	    "                      }\n"
	    "                  }\n"
	    "            }\n"
	    "        }\n",
	    name1, name1, size1, // if guard1
	    name2, name2, size2, // if guard2
	    size1, // k-for
	    left1, var, // (*this)[k_%d]%s
	    left1, var, left2, // (*this)[k_%d]%s<%d
	    left1, var, left2+size2-1, // (*this)[k_%d]%s>%d
	    
	    name1, // i-f
	    name2, // j-for
	    size1, name1, // initialize goodset
	    size2, name2, // initialize goodset
	    size1, // scan array index
	    name1, left1, var,               // if condition
	    left1, var, left2, // (*this)[k_%d]%s<%d
	    left1, var, left2+size2-1, // (*this)[k_%d]%s>%d
	    name2, left1, var, left2, // if condition (cont)
	    name1, name2, left1, var, left2, // set goodsets
	    
	    size1, // k-for -- split
	    name1, name1, // if condition
	    name1, size1, name1, name1, // move pos down 1 step
	    size1, name1, name1, name1, // split pos
	    name1, name1, name1,               // split pos (cont)
	    name1, // count_%s
	    
	    size2, // k-for -- split
	    name2, name2, // if condition
	    name2, size2, name2, name2, // move pos down 1 step
	    size2, name2, name2, name2, // split pos
	    name2, name2, name2,               // split pos (cont)
	    name2 // count_%s
	    
	    );
  else
    fprintf(codefile,
	    "\n"
	    "      // refinement -- graph structure for two scalarsets\n"
	    "      //               as in array S1 of S2\n"
	    "      // only if there is more than 1 permutation in class\n"
	    "      if ( (Perm.MTO_class_%s() && count_%s<%d)\n"
	    "           || ( Perm.MTO_class_%s() && count_%s<%d) )\n"
	    "        {\n"
	    "          exists = FALSE;\n"
	    "          split = FALSE;\n"
	    "          for (k=0; k<%d; k++) // step through class\n"
	    "            if ((*this)[k+%d]%s.isundefined())\n"
	    "              exists = TRUE;\n"
	    "            else\n"
	    "              split = TRUE;\n"
	    "          if (exists && split)\n"
	    "            {\n"
	    "              for (i=0; i<count_%s; i++) // scan through array index priority\n"
	    "                for (j=0; j<count_%s; j++) //scan through element priority\n"
	    "                  {\n"
	    "                    exists = FALSE;\n"
	    "                    for (k=0; k<%d; k++) // initialize goodset\n"
	    "                      goodset_%s[k] = FALSE;\n"
	    "                    for (k=0; k<%d; k++) // initialize goodset\n"
	    "                      goodset_%s[k] = FALSE;\n"
	    "                    for (k=0; k<%d; k++) // scan array index\n"
	    "                      // set goodsets\n"
	    "                      if (pos_%s[i][k] \n"
	    "                          && !(*this)[k+%d]%s.isundefined()\n"
	    "                          && pos_%s[j][(*this)[k+%d]%s-%d])\n"
	    "                        {\n"
	    "                          exists = TRUE;\n"
	    "                          goodset_%s[k] = TRUE;\n"
	    "                          goodset_%s[(*this)[k+%d]%s-%d] = TRUE;\n"
	    "                        }\n"
	    "                    if (exists)\n"
	    "                      {\n"
	    "                        // set split for the array index type\n" 
	    "                        split=FALSE;\n"
	    "                        for (k=0; k<%d; k++)\n"
	    "                          if ( pos_%s[i][k] && !goodset_%s[k] )\n"
	    "                            split= TRUE;\n"
	    "                        if (split)\n"
	    "                          {\n"
	    "                            // move following pos entries down 1 step\n"
	    "                            for (z=count_%s; z>i; z--)\n"
	    "                              for (k=0; k<%d; k++)\n"
	    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
	    "                            // split pos\n"
	    "                            for (k=0; k<%d; k++)\n"
	    "                              {\n"
	    "                                if (pos_%s[i][k] && !goodset_%s[k])\n"
	    "                                  pos_%s[i][k] = FALSE;\n"
	    "                                if (pos_%s[i+1][k] && goodset_%s[k])\n"
	    "                                  pos_%s[i+1][k] = FALSE;\n"
	    "                              }\n"
	    "                            count_%s++;\n"
	    "                          }\n"
	    "                        // set split for the element type\n" 
	    "                        split=FALSE;\n"
	    "                        for (k=0; k<%d; k++)\n"
	    "                          if ( pos_%s[j][k] && !goodset_%s[k] )\n"
	    "                            split= TRUE;\n"
	    "                        if (split)\n"
	    "                          {\n"
	    "                            // move following pos entries down 1 step\n"
	    "                            for (z=count_%s; z>j; z--)\n"
	    "                              for (k=0; k<%d; k++)\n"
	    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
	    "                            // split pos\n"
	    "                            for (k=0; k<%d; k++)\n"
	    "                              {\n"
	    "                                if (pos_%s[j][k] && !goodset_%s[k])\n"
	    "                                  pos_%s[j][k] = FALSE;\n"
	    "                                if (pos_%s[j+1][k] && goodset_%s[k])\n"
	    "                                  pos_%s[j+1][k] = FALSE;\n"
	    "                              }\n"
	    "                            count_%s++;\n"
	    "                          }\n"
	    "                      }\n"
	    "                  }\n"
	    "            }\n"
	    "        }\n",
	    name1, name1, size1, // if guard1
	    name2, name2, size2, // if guard2
	    size1, // k-for
	    left1, var, // (*this)[k_%d]%s
	    
	    name1, // i-f
	    name2, // j-for
	    size1, name1, // initialize goodset
	    size2, name2, // initialize goodset
	    size1, // scan array index
	    name1, left1, var,               // if condition
	    name2, left1, var, left2, // if condition (cont)
	    name1, name2, left1, var, left2, // set goodsets
	    
	    size1, // k-for -- split
	    name1, name1, // if condition
	    name1, size1, name1, name1, // move pos down 1 step
	    size1, name1, name1, name1, // split pos
	    name1, name1, name1,               // split pos (cont)
	    name1, // count_%s
	    
	    size2, // k-for -- split
	    name2, name2, // if condition
	    name2, size2, name2, name2, // move pos down 1 step
	    size2, name2, name2, name2, // split pos
	    name2, name2, name2,               // split pos (cont)
	    name2 // count_%s
	    
	    );
}
void arraytypedecl::generate_range(int size, char * name)
{
  fprintf(codefile,
          "\n"
          "      // setup range for maping\n"
          "      start = 0;\n"
          "      for (j=0; j<=Perm.undefined_class_%s; j++) // class number\n"
          "        {\n"
          "          class_size = 0;\n"
          "          for (k=0; k<%d; k++) // step through class[k]\n"
          "            if (Perm.class_%s[k]==j)\n"
          "              class_size++;\n"
          "          for (k=0; k<%d; k++) // step through class[k]\n"
          "            if (Perm.class_%s[k]==j)\n"
          "              {\n"
          "                size_%s[k] = class_size;\n"
          "                start_%s[k] = start;\n"
          "              }\n"
          "          start+=class_size;\n"
          "        }\n",
          name,   // j-for
          size, // k-for
          name,   // class_%s
          size, // k-for
          name,   // class_%s
          name,   // size_%s
          name    // start_%s
          );
}
void arraytypedecl::generate_canonicalize()
{
  stelist * t;
  typedecl * d;  
  int thisright;
  int thisleft;

  if (indextype->gettypeclass() == typedecl::Scalarset)
    generate_canonicalize(indextype->getsize(), indextype->mu_name,
			  0, indextype->getleft());
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset())
    {
      thisright = indextype->getsize()-1;
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          thisleft = thisright-d->getsize()+1;
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
	    generate_canonicalize(d->getsize(), d->mu_name,
				  thisleft, d->getleft());
          thisright= thisleft-1;
        }
    }
}
void arraytypedecl::generate_canonicalize(int size,
                                          char * name, int left, int scalarsetleft)
{
  fprintf(codefile,
	  "  if (Perm.MTO_class_%s())\n"
	  "    {\n",
	  name
	  );
  generate_range(size, name);
  generate_slow_canonicalize(size, name, left, scalarsetleft);
  fprintf(codefile,
	  "    }\n"
	  "  else\n"
	  "    {\n"
	  );
  generate_fast_canonicalize(size, name, left, scalarsetleft);
  fprintf(codefile,
	  "    }\n"
	  );
}
void arraytypedecl::generate_slow_canonicalize(int size,
                                          char * name, int left, int scalarsetleft)
{
  if (elementtype->issimple())
    fprintf(codefile,
	    "\n"
	    "      // canonicalize\n"
	    "      temp = *this;\n"
	    "      for (i=0; i<%d; i++)\n"
	    "        for (j=0; j<%d; j++)\n"
	    "         if (i >=start_%s[j] \n"
	    "             && i < start_%s[j] + size_%s[j])\n"
	    "           {\n"
	    "             array[i+%d].value(temp[j+%d].value());\n"
	    "             break;\n"
	    "           }\n",
	    size, // i-for
	    size, // j-for
	    name,   // start_%s
	    name, name,   // start_%s, size_%s
	    left, scalarsetleft // array[i+%d], temp[j+%d]
	    );
  else
    fprintf(codefile,
	    "\n"
	    "      // canonicalize\n"
	    "      temp = *this;\n"
	    "      for (i=0; i<%d; i++)\n"
	    "        for (j=0; j<%d; j++)\n"
	    "         if (i >=start_%s[j] \n"
	    "             && i < start_%s[j] + size_%s[j])\n"
	    "           {\n"
	    "             array[i+%d] = temp[j+%d];\n"
	    "             break;\n"
	    "           }\n",
	    size, // i-for
	    size, // j-for
	    name,   // start_%s
	    name, name,   // start_%s, size_%s
	    left, scalarsetleft // array[i+%d], temp[j+%d]
	    );
}

void arraytypedecl::generate_fast_canonicalize(int size,
                                          char * name, int left, int scalarsetleft)
{
  if (elementtype->gettypeclass() == typedecl::Record
      ||elementtype->gettypeclass() == typedecl::Array)
    
    fprintf(codefile,
	    "\n"
	    "      // fast canonicalize\n"
	    "      temp = *this;\n"
	    "      for (j=0; j<%d; j++)\n"
	    "        array[Perm.class_%s[j]+%d] = temp[j+%d];\n",
	    size, // j-for
	    name, // class_%s
	    left, scalarsetleft // array[i+%d], temp[j+%d]
	    );
  else
    fprintf(codefile,
	    "\n"
	    "      // fast canonicalize\n"
	    "      temp = *this;\n"
	    "      for (j=0; j<%d; j++)\n"
	    "        array[Perm.class_%s[j]+%d].value(temp[j+%d].value());\n",
	    size, // j-for
	    name, left, scalarsetleft // class_%s array[i+%d], temp[j+%d]
	    );
}
void arraytypedecl::generate_while_guard(charlist * scalarsetlist)
{
  charlist * sl;

  fprintf(codefile, "      while_guard = FALSE;\n");
  for (sl=scalarsetlist; sl!=NULL; sl = sl->next)
    fprintf(codefile, 
	    "      while_guard = while_guard || (oldcount_%s!=count_%s);\n",
	    sl->e->mu_name, sl->e->mu_name
	    );
  fprintf(codefile, "      while_guard_temp = while_guard;\n");
  fprintf(codefile, "      while_guard = FALSE;\n");
  for (sl=scalarsetlist; sl!=NULL; sl = sl->next)
    fprintf(codefile,
	    "      while_guard = while_guard || count_%s<%d;\n",
	    sl->e->mu_name, sl->e->getsize()
	    );
  fprintf(codefile, "      while_guard = while_guard && while_guard_temp;\n");
}
void arraytypedecl::generate_while(charlist * scalarsetlist)
{
  charlist * sl;
  
  fprintf(codefile, 
	  "\n"
	  "  // refinement -- checking priority in general\n");
  
  // setup initial while guard
  fprintf(codefile, "  while_guard = FALSE;\n");
  for (sl=scalarsetlist; sl!=NULL; sl = sl->next)
    fprintf(codefile,
	    "  while_guard = while_guard || (Perm.MTO_class_%s() && count_%s<%d);\n",
	    sl->e->mu_name, 
	    sl->e->mu_name, sl->e->getsize()
	    );
  
  fprintf(codefile, 
	  "  while ( while_guard )\n"
	  "    {\n");
  
  // setup oldcount
  for (sl=scalarsetlist; sl!=NULL; sl = sl->next)
    fprintf(codefile, 
	    "      oldcount_%s = count_%s;\n",
	    sl->e->mu_name, sl->e->mu_name
	    );
  
}

/********************
  generate simple_canon() for each typedecl
  ********************/

void enumtypedecl::generate_simple_canon_function(symmetryclass& symmetry)
{
  if (already_generated_simple_canon_function) return;
  else already_generated_simple_canon_function = TRUE;
  
  if (strcmp(mu_name,"mu_0_boolean")!=0) 
    fprintf(codefile,
            "void %s::SimpleCanonicalize(PermSet& Perm) {};\n",
            mu_name);
};
void subrangetypedecl::generate_simple_canon_function(symmetryclass& symmetry)
{
  if (already_generated_simple_canon_function) return;
  else already_generated_simple_canon_function = TRUE;
  
  fprintf(codefile,
          "void %s::SimpleCanonicalize(PermSet& Perm) {};\n",
          mu_name);
};

void multisettypedecl::generate_simple_canon_function(symmetryclass& symmetry)
{
  if (already_generated_simple_canon_function) return;
  else already_generated_simple_canon_function = TRUE;
  
  elementtype->generate_simple_canon_function(symmetry);

  fprintf(codefile,
          "void %s::SimpleCanonicalize(PermSet& Perm)\n"
	  "{ Error.Error(\"Internal: calling SimpleCanonicalize for a multiset.\\n\"); };\n",
          mu_name);
}

void arraytypedecl::generate_simple_canon_function(symmetryclass& symmetry)
{
  if (already_generated_simple_canon_function) return;
  else already_generated_simple_canon_function = TRUE;
  
  elementtype->generate_simple_canon_function(symmetry);
  
  if (indextype->getstructure() != typedecl::ScalarsetVariable 
      && elementtype->HasScalarsetVariable())
    fprintf(codefile,
	    "void %s::SimpleCanonicalize(PermSet& Perm)\n"
	    "{\n"
	    "  for (int j=0; j<%d; j++)\n"
	    "    array[j].SimpleCanonicalize(Perm);\n"
	    "}\n",
	    mu_name,              // %s::Canonicalize
	    indextype->getsize()  // j-for
	    );
  else
    fprintf(codefile,
            "void %s::SimpleCanonicalize(PermSet& Perm)\n"
	    "{ Error.Error(\"Internal: Simple Canonicalization of Scalarset Array\\n\"); };\n",
            mu_name);
}

void recordtypedecl::generate_simple_canon_function(symmetryclass& symmetry)
{
  ste *f;
  
  if (already_generated_simple_canon_function) return;
  else already_generated_simple_canon_function = TRUE;
  
  for (f = fields; f != NULL; f = f->next)
    f->getvalue()->gettype()->generate_simple_canon_function(symmetry);
  
  if (HasScalarsetVariable())
    {
      
      fprintf(codefile,
	      "void %s::SimpleCanonicalize(PermSet& Perm)\n"
	      "{\n",
	      mu_name
	      );
      for (f = fields; f != NULL; f = f->next)
	if (f->getvalue()->gettype()->HasScalarsetVariable())
	  fprintf(codefile,
		  "  %s.SimpleCanonicalize(Perm);\n",
		  f->getvalue()->generate_code()
		  );
      fprintf(codefile,
	      "};\n");
    }
  else
    fprintf(codefile,
	    "void %s::SimpleCanonicalize(PermSet& Perm)\n"
	    "{ Error.Error(\"Internal: Simple Canonicalization of Record with no scalarset variable\\n\"); };\n",
	    mu_name
	    );
}
  
void scalarsettypedecl::generate_simple_canon_function(symmetryclass& symmetry)
{
  if (already_generated_simple_canon_function) return;
  else already_generated_simple_canon_function = TRUE;
  
  if (!useless && getsize() > 1) 
    fprintf(codefile,
            "void %s::SimpleCanonicalize(PermSet& Perm)\n"
            "{\n"
            "  int i, class_number;\n"
            "  if (Perm.Presentation != PermSet::Simple)\n"
            "    Error.Error(\"Internal Error: Wrong Sequence of Normalization\");\n" 
            "\n"
            "  if (defined())\n"
            "    if (Perm.class_%s[value()-%d]==Perm.undefined_class_%s) // value - base\n"
            "      {\n"
            "        // it has not been mapped to any particular value\n"
            "        for (i=0; i<%d; i++)\n"
            "          if (Perm.class_%s[i] == Perm.undefined_class_%s && i!=value()-%d)\n"
            "            Perm.class_%s[i]++;\n"
            "        value(%d + Perm.undefined_class_%s++);\n"
            "      }\n"
            "    else \n"
            "      {\n"
            "        value(Perm.class_%s[value()-%d]+%d);\n"
            "      }\n"
            "}\n",
            mu_name,                  // void %s::Canonicalize
            mu_name, left, mu_name,   // class_%s, value-%d, undefined_class_%s
            getsize(),                // i-for
            mu_name, mu_name, left,   // class_%s, undefined_class_%s, value-%d
            mu_name,                  // class_%s
            left, mu_name,            // %d + Perm.undefined_class_%s++
            mu_name, left, left       // class_%s[value-%d]+%d
            );
  else
    fprintf(codefile,
            "void %s::SimpleCanonicalize(PermSet& Perm) {}\n",
            mu_name);
}

void uniontypedecl::generate_simple_canon_function(symmetryclass& symmetry)
{
  if (already_generated_simple_canon_function) return;
  else already_generated_simple_canon_function = TRUE;
  
  if (useless) 
    fprintf(codefile,
	    "void %s::SimpleCanonicalize(PermSet& Perm) {}\n",
	    mu_name);
  else
    {
      fprintf(codefile,
              "void %s::SimpleCanonicalize(PermSet& Perm)\n"
              "{\n"
              "  int i, class_number;\n"
              "  if (Perm.Presentation != PermSet::Simple)\n"
              "    Error.Error(\"Internal Error: Wrong Sequence of Normalization\");\n" 
              "  if (defined()) {\n",
              mu_name);  
      
      stelist * t;
      typedecl * d;  
      int thisright = getsize();
      int thisleft;
      for(t=unionmembers; t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          thisleft = thisright-d->getsize()+1;
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1)
            fprintf(codefile,
                    "    if ( ( value() >= %d ) && ( value() <= %d ) )\n"
		    "      {\n" 
                    "        if (Perm.class_%s[value()-%d]==Perm.undefined_class_%s) // value - base\n"
                    "          {\n"
                    "            // it has not been mapped to any particular value\n"
                    "            for (i=0; i<%d; i++)\n"
                    "              if (Perm.class_%s[i] == Perm.undefined_class_%s && i!=value()-%d)\n"
                    "                Perm.class_%s[i]++;\n"
                    "            value(%d + Perm.undefined_class_%s++);\n"
                    "          }\n"
                    "        else \n"
                    "          {\n"
                    "            value(Perm.class_%s[value()-%d]+%d);\n"
                    "          }\n"
                    "      }\n",
//                    thisleft, thisright,      // if// fixing union permutation bug
		    d->getleft(), d->getright(),
//                    d->mu_name, thisleft, d->mu_name, // class_%s, value-%d, class_%s// fixing union permutation bug
		    d->mu_name, d->getleft(), d->mu_name,
                    d->getsize(),             // i-for
//                    d->mu_name, d->mu_name, thisleft,  // class_%s, undefined_class_%s, vlaue -%d// fixing union permutation bug
                    d->mu_name, d->mu_name, d->getleft(),  // class_%s, undefined_class_%s, vlaue -%d
                    d->mu_name,               // class_%s
//                    thisleft, d->mu_name,     //  %d + Perm.undefined_class_%s++// fixing union permutation bug
		    d->getleft(), d->mu_name,
//                    d->mu_name, thisleft, thisleft     // class_%s[value-%d]+%d// fixing union permutation bug
                    d->mu_name, d->getleft(), d->getleft()     // class_%s[value-%d]+%d
                    );
          thisright = thisleft-1;
        }
      fprintf(codefile,
              "  }\n"
              "}\n"
              );
    }
}

void typedecl::generate_simple_canon_function(symmetryclass& symmetry)
{ Error.Error("Internal: typedecl::generate_simple_canon_function()"); };

/********************
  generate canonicalize() for each typedecl
  ********************/

void enumtypedecl::generate_canonicalize_function(symmetryclass& symmetry)
{
  if (already_generated_canonicalize_function) return;
  else already_generated_canonicalize_function = TRUE;
  
  if (strcmp(mu_name,"mu_0_boolean")!=0) 
    fprintf(codefile,
            "void %s::Canonicalize(PermSet& Perm) {};\n",
            mu_name);
};
void subrangetypedecl::generate_canonicalize_function(symmetryclass& symmetry)
{
  if (already_generated_canonicalize_function) return;
  else already_generated_canonicalize_function = TRUE;
  
  fprintf(codefile,
          "void %s::Canonicalize(PermSet& Perm) {};\n",
          mu_name);
};
void multisettypedecl::generate_canonicalize_function(symmetryclass& symmetry)
{
  if (already_generated_canonicalize_function) return;
  else already_generated_canonicalize_function = TRUE;
  
  elementtype->generate_canonicalize_function(symmetry);

  fprintf(codefile,
          "void %s::Canonicalize(PermSet& Perm)\n"
	  "{ Error.Error(\"You cannot use this symmetry algorithm with Multiset.\\n\"); };\n",
          mu_name);
}

void arraytypedecl::generate_canonicalize_function(symmetryclass& symmetry)
{
  stelist * t;
  typedecl * d;  
  
  if (already_generated_canonicalize_function) return;
  else already_generated_canonicalize_function = TRUE;
  
  elementtype->generate_canonicalize_function(symmetry);
  
   if (indextype->getstructure() != typedecl::ScalarsetVariable
       && ( elementtype->getstructure() == typedecl::ScalarsetArrayOfFree
          || elementtype->getstructure() == typedecl::MultisetOfFree ) )
    {
      fprintf(codefile,
              "void %s::Canonicalize(PermSet& Perm)\n"
              "{\n"
              "  for (int j=0; j<%d; j++)\n"
              "    array[j].Canonicalize(Perm);\n"
              "}\n",
              mu_name,              // %s::Canonicalize
              indextype->getsize()  // j-for
              );
    }
  else if (indextype->gettypeclass() == typedecl::Scalarset
           && elementtype->getstructure() == typedecl::NoScalarset
           && indextype->getsize() > 1 )
    {
      fprintf(codefile,
              "void %s::Canonicalize(PermSet& Perm)\n"
              "{\n"
              "  // indexes\n"
              "  int i,j,k,z;\n"
              "  // sorting\n"
              "  int count_%s;\n"
	      "  int compare;\n"
              "  static %s value[%d];\n"
              "  // limit\n"
              "  bool exists;\n"
              "  bool split;\n"
              "  bool goodset_%s[%d];\n"
              "  bool pos_%s[%d][%d];\n"
              "  // range mapping\n"
              "  int start;\n"
              "  int class_size;\n"
              "  int size_%s[%d];\n"
              "  int start_%s[%d];\n"
              "  // canonicalization\n"
              "  static %s temp;\n",
              mu_name, // %s::Canonicalize
              indextype->mu_name, // count_%s
              elementtype->mu_name, indextype->getsize(), // %s value[%d]
              indextype->mu_name, indextype->getsize(), // bool goodset_%s[%d]
              indextype->mu_name, indextype->getsize(), indextype->getsize(), // pos
              indextype->mu_name, indextype->getsize(), // size_%s[%d]
              indextype->mu_name, indextype->getsize(), // start_%s[%d]
              mu_name  // static %s temp
              );
      generate_simple_sort();
      generate_limit();
      generate_canonicalize();
      fprintf(codefile, "}\n");
    }
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset()
           && elementtype->getstructure() == typedecl::NoScalarset)
    {
      fprintf(codefile,
              "void %s::Canonicalize(PermSet& Perm)\n"
              "{\n"
              "  // indexes\n"
              "  int i,j,k,z;\n"
              "  // sorting\n"
              "  static %s value[%d];\n"
	      "  int compare;\n"
              "  // limit\n"
              "  bool exists;\n"
              "  bool split;\n"
              "  // range mapping\n"
              "  int start;\n"
              "  int class_size;\n"
              "  // canonicalization\n"
              "  static %s temp;\n",
              mu_name, // %s::Canonicalize
              elementtype->mu_name, indextype->getsize(), // %s value[%d]
              mu_name  // static %s temp
              );
      
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
            {
              fprintf(codefile,
                      "  int count_%s;\n"
                      "  bool pos_%s[%d][%d];\n"
                      "  bool goodset_%s[%d];\n"
                      "  int size_%s[%d];\n"
                      "  int start_%s[%d];\n",
                      d->mu_name,
                      d->mu_name, d->getsize(), d->getsize(),
                      d->mu_name, d->getsize(),
                      d->mu_name, d->getsize(),
                      d->mu_name, d->getsize(),
                      d->mu_name
                      );
            }
        }
      generate_simple_sort();
      generate_limit();
      generate_canonicalize();
      fprintf(codefile,"}\n");
    }
  else
    fprintf(codefile,
            "void %s::Canonicalize(PermSet& Perm){};\n",
            mu_name);
}

void recordtypedecl::generate_canonicalize_function(symmetryclass& symmetry)
{
  ste *f;
  
  if (already_generated_canonicalize_function) return;
  else already_generated_canonicalize_function = TRUE;
  
  for (f = fields; f != NULL; f = f->next)
    f->getvalue()->gettype()->generate_canonicalize_function(symmetry);
  
//   if (getstructure() == ScalarsetArrayOfFree)
//     {
  fprintf(codefile,
	  "void %s::Canonicalize(PermSet& Perm)\n"
	  "{\n",
	  mu_name
	  );
  for (f = fields; f != NULL; f = f->next)
    if (f->getvalue()->gettype()->getstructure() == ScalarsetArrayOfFree)
//        if (f->getvalue()->gettype()->HasScalarsetArrayOfFree())
      fprintf(codefile,
	      "  %s.Canonicalize(Perm);\n",
	      f->getvalue()->generate_code()
	      );
  fprintf(codefile,
	  "};\n");
//     }
//   else
//     fprintf(codefile,
//             "void %s::Canonicalize(PermSet& Perm)\n"
//             "{\n"
//             "  Error.Error(\"Calling canonicalize() for non simple scalarsettype.\");\n"
//             "}\n",
//             mu_name);
  
}

void scalarsettypedecl::generate_canonicalize_function(symmetryclass& symmetry)
{
  if (already_generated_canonicalize_function) return;
  else already_generated_canonicalize_function = TRUE;
  
  fprintf(codefile,
	  "void %s::Canonicalize(PermSet& Perm)\n"
	  "{\n"
	  "  Error.Error(\"Calling canonicalize() for Scalarset.\");\n"
	  "}\n",
	  mu_name);
}

void uniontypedecl::generate_canonicalize_function(symmetryclass& symmetry)
{
  if (already_generated_canonicalize_function) return;
  else already_generated_canonicalize_function = TRUE;
  
  fprintf(codefile,
	  "void %s::Canonicalize(PermSet& Perm)\n"
	  "{\n"
	  "  Error.Error(\"Calling canonicalize() for Scalarset.\");\n"
	  "}\n",
	  mu_name);
}

void typedecl::generate_canonicalize_function(symmetryclass& symmetry)
{ Error.Error("Internal: typedecl::generate_canonicalize_function()"); };

/********************
  generate SimpleLimit() for each typedecl
  ********************/

void enumtypedecl::generate_simple_limit_function(symmetryclass& symmetry)
{
  if (already_generated_simple_limit_function) return;
  else already_generated_simple_limit_function = TRUE;
  
  if (strcmp(mu_name,"mu_0_boolean")!=0) 
    fprintf(codefile,
            "void %s::SimpleLimit(PermSet& Perm) {};\n",
            mu_name);
};
void subrangetypedecl::generate_simple_limit_function(symmetryclass& symmetry)
{
  if (already_generated_simple_limit_function) return;
  else already_generated_simple_limit_function = TRUE;
  
  fprintf(codefile,
          "void %s::SimpleLimit(PermSet& Perm) {};\n",
          mu_name);
};
void multisettypedecl::generate_simple_limit_function(symmetryclass& symmetry)
{
  if (already_generated_simple_limit_function) return;
  else already_generated_simple_limit_function = TRUE;
  
  elementtype->generate_simple_limit_function(symmetry);

  fprintf(codefile,
          "void %s::SimpleLimit(PermSet& Perm)\n"
	  "{ Error.Error(\"You cannot use this symmetry algorithm with Multiset.\\n\"); };\n",
          mu_name);
}
void arraytypedecl::generate_simple_limit_function(symmetryclass& symmetry)
{
  if (already_generated_simple_limit_function) return;
  else already_generated_simple_limit_function = TRUE;
  
  elementtype->generate_simple_limit_function(symmetry);
  
  if ( indextype->getstructure() != typedecl::ScalarsetVariable
       && elementtype->HasScalarsetVariable() )
    {
      fprintf(codefile,
              "void %s::SimpleLimit(PermSet& Perm)\n"
              "{\n"
              "  for (int j=0; j<%d; j++) {\n"
              "    array[j].SimpleLimit(Perm);\n"
              "  }\n"
              "}\n",
              mu_name,              // %s::Limit
              indextype->getsize()  // j-for
              );
    }
  else
    fprintf(codefile,
            "void %s::SimpleLimit(PermSet& Perm){}\n",
            mu_name);
}

void recordtypedecl::generate_simple_limit_function(symmetryclass& symmetry)
{
  ste *f;
  
  if (already_generated_simple_limit_function) return;
  else already_generated_simple_limit_function = TRUE;
  
  for (f = fields; f != NULL; f = f->next)
    f->getvalue()->gettype()->generate_simple_limit_function(symmetry);
  
  if (HasScalarsetVariable())
    {
      fprintf(codefile,
              "void %s::SimpleLimit(PermSet& Perm)\n"
              "{\n",
              mu_name
              );
      for (f = fields; f != NULL; f = f->next)
        if (f->getvalue()->gettype()->HasScalarsetVariable())
          fprintf(codefile,
                  "  %s.SimpleLimit(Perm);\n",
                  f->getvalue()->generate_code()
                  );
      fprintf(codefile,"};\n");
    }
  else
    fprintf(codefile,
            "void %s::SimpleLimit(PermSet& Perm){}\n",
            mu_name);
  
}

void scalarsettypedecl::generate_simple_limit_function(symmetryclass& symmetry)
{
  if (already_generated_simple_limit_function) return;
  else already_generated_simple_limit_function = TRUE;
  
  if (!useless && getsize() > 1) 
    fprintf(codefile,
            "void %s::SimpleLimit(PermSet& Perm)\n"
            "{\n"
            "  int i, class_number;\n"
            "  if (Perm.Presentation != PermSet::Simple)\n"
            "    Error.Error(\"Internal Error: Wrong Sequence of Normalization\");\n" 
            "\n"
            "  if (defined())\n"
            "    if (Perm.class_%s[value()-%d]==Perm.undefined_class_%s) // value - base\n"
            "      {\n"
            "        // it has not been mapped to any particular value\n"
            "        for (i=0; i<%d; i++)\n"
            "          if (Perm.class_%s[i] == Perm.undefined_class_%s && i!=value()-%d)\n"
            "            Perm.class_%s[i]++;\n"
            "        Perm.undefined_class_%s++;\n"
            "      }\n"
            "}\n",
            mu_name,                  // void %s::Canonicalize
            mu_name, left, mu_name,   // class_%s, value-%d, undefined_class_%s
            getsize(),                // i-for
            mu_name, mu_name, left,   // class_%s, undefined_class_%s, value-%d
            mu_name,                  // class_%s
            mu_name                   // Perm.undefined_class_%s++
            );
  else
    fprintf(codefile,
            "void %s::SimpleLimit(PermSet& Perm) {}\n",
            mu_name);
}

void uniontypedecl::generate_simple_limit_function(symmetryclass& symmetry)
{
  if (already_generated_simple_limit_function) return;
  else already_generated_simple_limit_function = TRUE;

  if (useless) 
   {
      fprintf(codefile,
              "void %s::SimpleLimit(PermSet& Perm) {}\n",
              mu_name);
   }
  else
    {
      fprintf(codefile,
              "void %s::SimpleLimit(PermSet& Perm)\n"
              "{\n"
              "  int i, class_number;\n"
              "  if (Perm.Presentation != PermSet::Simple)\n"
              "    Error.Error(\"Internal Error: Wrong Sequence of Normalization\");\n" 
              "  if (defined()) {\n",
              mu_name);  
      
      stelist * t;
      typedecl * d;  
      int thisright = getsize();
      int thisleft;
      for(t=unionmembers; t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          thisleft = thisright-d->getsize()+1;
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1)
            fprintf(codefile,
                    "    if ( ( value() >= %d ) && ( value() <= %d ) )\n"
                    "      if (Perm.class_%s[value()-%d]==Perm.undefined_class_%s) // value - base\n"
                    "        {\n"
                    "          // it has not been mapped to any particular value\n"
                    "          for (i=0; i<%d; i++)\n"
                    "            if (Perm.class_%s[i] == Perm.undefined_class_%s && i!=value()-%d)\n"
                    "              Perm.class_%s[i]++;\n"
                    "          Perm.undefined_class_%s++;\n"
                    "        }\n",
//                    thisleft, thisright,      // if// fixing union permutation bug
		    d->getleft(), d->getright(),
//                    d->mu_name, thisleft, d->mu_name, // class_%s, value-%d, class_%s// fixing union permutation bug
                    d->mu_name, d->getleft(), d->mu_name, // class_%s, value-%d, class_%s
                    d->getsize(),             // i-for
//                    d->mu_name, d->mu_name, thisleft,  // class_%s, undefined_class_%s, vlaue -%d// fixing union permutation bug
                    d->mu_name, d->mu_name, d->getleft(),  // class_%s, undefined_class_%s, vlaue -%d
                    d->mu_name,               // class_%s
                    d->mu_name                // Perm.undefined_class_%s++
                    );
          thisright = thisleft-1;
        }
      fprintf(codefile,
              "  }\n"
              "}\n"
              );
    }
}
void typedecl::generate_simple_limit_function(symmetryclass& symmetry)
{ Error.Error("Internal: typedecl::generate_simple_limit_function()"); };



/********************
  generate ArrayLimit() for each typedecl
  ********************/

void enumtypedecl::generate_array_limit_function(symmetryclass& symmetry)
{
  if (already_generated_array_limit_function) return;
  else already_generated_array_limit_function = TRUE;

  if (strcmp(mu_name,"mu_0_boolean")!=0) 
    fprintf(codefile,
            "void %s::ArrayLimit(PermSet& Perm) {};\n",
            mu_name);
};
void subrangetypedecl::generate_array_limit_function(symmetryclass& symmetry)
{
  if (already_generated_array_limit_function) return;
  else already_generated_array_limit_function = TRUE;
  
  fprintf(codefile,
          "void %s::ArrayLimit(PermSet& Perm) {};\n",
          mu_name);
};
void multisettypedecl::generate_array_limit_function(symmetryclass& symmetry)
{
  if (already_generated_array_limit_function) return;
  else already_generated_array_limit_function = TRUE;
  
  elementtype->generate_array_limit_function(symmetry);

  fprintf(codefile,
          "void %s::ArrayLimit(PermSet& Perm)\n"
	  "{ Error.Error(\"You cannot use this symmetry algorithm with Multiset.\\n\"); };\n",
          mu_name);
}

void arraytypedecl::generate_array_limit_function(symmetryclass& symmetry)
{
  stelist * t;
  typedecl * d;  
  
  if (already_generated_array_limit_function) return;
  else already_generated_array_limit_function = TRUE;
  
  elementtype->generate_array_limit_function(symmetry);
  
  if ( indextype->getstructure() != typedecl::ScalarsetVariable
       && elementtype->HasScalarsetArrayOfFree() )
    {
      fprintf(codefile,
              "void %s::ArrayLimit(PermSet& Perm)\n"
              "{\n"
              "  for (int j=0; j<%d; j++)\n"
              "    array[j].ArrayLimit(Perm);\n"
              "}\n",
              mu_name,              // %s::Limit
              indextype->getsize()  // j-for
              );
    }
  // no matter what the element is, we will generate the procedure
  // whether this procedure will be call depends on the high level routine
  else if (indextype->gettypeclass() == typedecl::Scalarset
           && indextype->getsize() > 1 )
    {
      fprintf(codefile,
              "void %s::ArrayLimit(PermSet& Perm)\n"
              "{\n"
              "  // indexes\n"
              "  int i,j,k,z;\n"
              "  // sorting\n"
              "  int count_%s;\n"
	      "  int compare;\n" 
              "  static %s value[%d];\n"
              "  // limit\n"
              "  bool exists;\n"
              "  bool split;\n"
              "  bool goodset_%s[%d];\n"
              "  bool pos_%s[%d][%d];\n",
              mu_name, // %s::Limit
              indextype->mu_name, // count_%s
              elementtype->mu_name, indextype->getsize(), // %s value[%d]
              indextype->mu_name, indextype->getsize(), // bool goodset_%s[%d]
              indextype->mu_name, indextype->getsize(), indextype->getsize() // pos
              );

      generate_simple_sort();
      generate_limit();
      fprintf(codefile, "}\n");
    }
  // no matter what the element is, we will generate the procedure
  // whether this procedure will be call depends on the high level routine
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset() )
    {
      fprintf(codefile,
              "void %s::ArrayLimit(PermSet& Perm)\n"
              "{\n"
              "  // indexes\n"
              "  int i,j,k,z;\n"
              "  // sorting\n"
	      "  int compare;\n"
              "  static %s value[%d];\n"
              "  // limit\n"
              "  bool exists;\n"
              "  bool split;\n",
              mu_name, // %s::Canonicalize
              elementtype->mu_name, indextype->getsize() // %s value[%d]
              );
      
      for(t=((uniontypedecl *)indextype)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d = (typedecl *)t->s->getvalue();
          if (d->gettypeclass() == typedecl::Scalarset && d->getsize() > 1 ) 
            {
              fprintf(codefile,
                      "  int count_%s;\n"
                      "  bool pos_%s[%d][%d];\n"
                      "  bool goodset_%s[%d];\n",
                      d->mu_name,
                      d->mu_name, d->getsize(), d->getsize(),
                      d->mu_name, d->getsize()
                      );
            }
        }
      
      generate_simple_sort();
      generate_limit();
      fprintf(codefile,"}\n");
    }
  else
    fprintf(codefile,
            "void %s::ArrayLimit(PermSet& Perm) {}\n",
            mu_name);
}

void recordtypedecl::generate_array_limit_function(symmetryclass& symmetry)
{
  ste *f;
  
  if (already_generated_array_limit_function) return;
  else already_generated_array_limit_function = TRUE;
  
  for (f = fields; f != NULL; f = f->next)
    f->getvalue()->gettype()->generate_array_limit_function(symmetry);
  
  if (HasScalarsetArrayOfFree())
    {
      fprintf(codefile,
              "void %s::ArrayLimit(PermSet& Perm)\n"
              "{\n",
              mu_name
              );
      for (f = fields; f != NULL; f = f->next)
        if (f->getvalue()->gettype()->HasScalarsetArrayOfFree())
          fprintf(codefile,
                  "  %s.ArrayLimit(Perm);\n",
                  f->getvalue()->generate_code()
                  );
      fprintf(codefile,"};\n");
    }
  else
    fprintf(codefile,
            "void %s::ArrayLimit(PermSet& Perm){}\n",
            mu_name);
  
}

void scalarsettypedecl::generate_array_limit_function(symmetryclass& symmetry)
{
  if (already_generated_array_limit_function) return;
  else already_generated_array_limit_function = TRUE;
  
  fprintf(codefile,
	  "void %s::ArrayLimit(PermSet& Perm) {}\n",
	  mu_name);
}

void uniontypedecl::generate_array_limit_function(symmetryclass& symmetry)
{
  if (already_generated_array_limit_function) return;
  else already_generated_array_limit_function = TRUE;

  fprintf(codefile,
	  "void %s::ArrayLimit(PermSet& Perm) {}\n",
	  mu_name);
}
void typedecl::generate_array_limit_function(symmetryclass& symmetry)
{ Error.Error("Internal: typedecl::generate_array_limit_function()"); };



/********************
  generate limit() for each typedecl
  ********************/

void enumtypedecl::generate_limit_function(symmetryclass& symmetry)
{
  if (already_generated_limit_function) return;
  else already_generated_limit_function = TRUE;
  
  if (strcmp(mu_name,"mu_0_boolean")!=0) 
    fprintf(codefile,
            "void %s::Limit(PermSet& Perm) {};\n",
            mu_name);
};
void subrangetypedecl::generate_limit_function(symmetryclass& symmetry)
{
  if (already_generated_limit_function) return;
  else already_generated_limit_function = TRUE;
  
  fprintf(codefile,
          "void %s::Limit(PermSet& Perm) {};\n",
          mu_name);
};
void multisettypedecl::generate_limit_function(symmetryclass& symmetry)
{
  if (already_generated_limit_function) return;
  else already_generated_limit_function = TRUE;
  
  elementtype->generate_limit_function(symmetry);

  fprintf(codefile,
          "void %s::Limit(PermSet& Perm)\n"
	  "{ Error.Error(\"You cannot use this symmetry algorithm with Multiset.\\n\"); };\n",
          mu_name);
}
void arraytypedecl::generate_limit_function(symmetryclass& symmetry)
{
  charlist * scalarsetlist;
  charlist * scalarsetvalue;
  charlist * sv;
  
  if (already_generated_limit_function) return;
  else already_generated_limit_function = TRUE;
  
  elementtype->generate_limit_function(symmetry);
  
  if ( indextype->getstructure() != typedecl::ScalarsetVariable
       && elementtype->HasScalarsetArrayOfS() )
    {
      fprintf(codefile,
              "void %s::Limit(PermSet& Perm)\n"
              "{\n"
              "  if (Perm.Presentation != PermSet::Simple)\n"
              "    Error.Error(\"Internal Error: Wrong Sequence of Normalization\");\n" 
              "\n"
              "  for (int j=0; j<%d; j++) {\n"
              "    array[j].Limit(Perm);\n"
              "  }\n"
              "}\n",
              mu_name,              // %s::Limit
              indextype->getsize()  // j-for
              );
    }
  else if (indextype->gettypeclass() == typedecl::Scalarset
           && elementtype->HasScalarsetVariable()
           && indextype->getsize() > 1 )
    {
      fprintf(codefile,
              "void %s::Limit(PermSet& Perm)\n"
              "{\n"
              "  // indexes\n"
              "  int i,j,k,z;\n"
	      "  // while guard\n"
	      "  bool while_guard, while_guard_temp;\n"
              "  // sorting\n"
              "  static %s value[%d];\n"
              "  // limit\n"
              "  bool exists;\n"
              "  bool split;\n",
              mu_name, // %s::Canonicalize
              elementtype->mu_name, indextype->getsize() // %s value[%d]
              );

      // get a list for all scalarsets that may appear
      scalarsetlist = generate_scalarset_list(NULL);


      fprintf(codefile, "  int i0");
      for (int i = get_num_limit_locals(elementtype)-1; i>0; i--)
	fprintf(codefile,",i%d",i);
      fprintf(codefile, ";\n");

      // declare for all scalarsets that may appear
      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  int count_%s, oldcount_%s;\n"
		"  bool pos_%s[%d][%d];\n"
		"  bool goodset_%s[%d];\n",
		sv->e->mu_name, sv->e->mu_name,
		sv->e->mu_name, sv->e->getsize(), sv->e->getsize(),
		sv->e->mu_name, sv->e->getsize()
		);

      // change to without simple sort
      // it has been done separately
      // you have to set it up for all scalarsets that may be reference
      // even though it may have only 1 permutation

      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  // count_ corresponds to the number of equivalence class within the\n"
		"  // scalarset value.  If count_ == size of the scalarset, then a unique\n"
		"  // permutation has been found.\n"
		"\n"
		"  // pos_ is a relation on a equivalence class number and a scalarset value.\n"
		"\n"
		"  // initializing pos array\n"
		"  for (i=0; i<%d; i++)\n"
		"    for (j=0; j<%d; j++)\n"
		"      pos_%s[i][j]=FALSE;\n"
		"  count_%s = 0;\n"
		"  while (1)\n"
		"    {\n"
		"      exists = FALSE;\n"
		"      for (i=0; i<%d; i++)\n"
		"       if (Perm.class_%s[i] == count_%s)\n"
		"         {\n"
		"           pos_%s[count_%s][i]=TRUE;\n"
		"           exists = TRUE;\n"
		"         }\n"
		"      if (exists) count_%s++;\n"
		"      else break;\n"
		"    }\n",
		sv->e->getsize(), sv->e->getsize(), sv->e->mu_name, // initialization
		sv->e->mu_name, // count_%s
		sv->e->getsize(), // i-for
		sv->e->mu_name, sv->e->mu_name, // if-guard 
		sv->e->mu_name, sv->e->mu_name, // pos_%s[count%s]
		sv->e->mu_name // count_%s
		);
      
      // refine according to the undefined scalarset value in the element
      // for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
      //   generate_limit2(sv->string, sv->e);
      
      // refine according to selfreference of scalarset in the element
//      for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
//	generate_limit3(sv->string, sv->e);
      generate_limit_step1(3);

      // have to generate the while loop
      generate_while(scalarsetlist);

      // refine accordipng to directed graph of Array S1 of S1
//       for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
// 	generate_limit4(indextype, sv->string, sv->e);
      generate_limit_step1(4);

      // refine according to directed graph of Array S1 of S2 where S1 != S2
//       for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
      generate_limit_step1(5);

      // set while guard
      generate_while_guard(scalarsetlist);

      // end while loop
      fprintf(codefile, "    } // end while\n"); 

      // enter result
      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  // enter the result into class\n"
		"  if (Perm.MTO_class_%s())\n"
		"    {\n"
		"      for (i=0; i<%d; i++)\n"
		"        for (j=0; j<%d; j++)\n"
		"          if (pos_%s[i][j])\n"
		"            Perm.class_%s[j] = i;\n"
		"      Perm.undefined_class_%s=0;\n"
		"      for (j=0; j<%d; j++)\n"
		"        if (Perm.class_%s[j]>Perm.undefined_class_%s)\n"
		"          Perm.undefined_class_%s=Perm.class_%s[j];\n"
		"    }\n",
		sv->e->mu_name,
		sv->e->getsize(), sv->e->getsize(), // i-for j-for
		sv->e->mu_name, // pos
		sv->e->mu_name, // class
		sv->e->mu_name, // undefined_class
		sv->e->getsize(), // i-for j-for
		sv->e->mu_name, sv->e->mu_name, // class_%s undefined_class_%s
		sv->e->mu_name, sv->e->mu_name  // undefined_class_%s class_%s
		);
      
      fprintf(codefile,"}\n");
    }
  else if (indextype->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)indextype)->IsUnionWithScalarset()
           && elementtype->HasScalarsetVariable() )
    {
      fprintf(codefile,
              "void %s::Limit(PermSet& Perm)\n"
              "{\n"
              "  // indexes\n"
              "  int i,j,k,z;\n"
	      "  // while guard\n"
	      "  bool while_guard, while_guard_temp;\n"
              "  // sorting\n"
              "  static %s value[%d];\n"
              "  // limit\n"
              "  bool exists;\n"
              "  bool split;\n",
              mu_name, // %s::Limit
              elementtype->mu_name, indextype->getsize() // %s value[%d]
              );

      // get a list for all scalarsets that may appear
      scalarsetlist = generate_scalarset_list(NULL);

      fprintf(codefile, "  int i0");
      for (int i = get_num_limit_locals(elementtype)-1; i>0; i--)
	fprintf(codefile,",i%d",i);
      fprintf(codefile, ";\n");

      // declare for all scalarsets that may appear
      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  int count_%s, oldcount_%s;\n"
		"  bool pos_%s[%d][%d];\n"
		"  bool goodset_%s[%d];\n",
		sv->e->mu_name, sv->e->mu_name,
		sv->e->mu_name, sv->e->getsize(), sv->e->getsize(),
		sv->e->mu_name, sv->e->getsize()
		);

      // change to without simple sort
      // you have to set it up for all scalarsets that may be reference
      // even though it may have only 1 permutation
      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  // initializing pos array\n"
		"  for (i=0; i<%d; i++)\n"
		"    for (j=0; j<%d; j++)\n"
		"      pos_%s[i][j]=FALSE;\n"
		"  count_%s = 0;\n"
		"  while (1)\n"
		"    {\n"
		"      exists = FALSE;\n"
		"      for (i=0; i<%d; i++)\n"
		"       if (Perm.class_%s[i] == count_%s)\n"
		"         {\n"
		"           pos_%s[count_%s][i]=TRUE;\n"
		"           exists = TRUE;\n"
		"         }\n"
		"      if (exists) count_%s++;\n"
		"      else break;\n"
		"    }\n",
		sv->e->getsize(), sv->e->getsize(), sv->e->mu_name, // initialization
		sv->e->mu_name, // count_%s
		sv->e->getsize(), // i-for
		sv->e->mu_name, sv->e->mu_name, // if-guard 
		sv->e->mu_name, sv->e->mu_name, // pos_%s[count%s]
		sv->e->mu_name // count_%s
		);
      
      // sorting according to constants in the element
      // generate_simple_sort();
              
      // refine according to the undefined scalarset value in the element
      // for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
      //   generate_limit2(sv->string, sv->e);
      
      // refine according to selfreference of scalarset in the element
//       for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
// 	generate_limit3(sv->string, sv->e);
      generate_limit_step1(3);
             
      // prepare while loop for further refinement
      generate_while(scalarsetlist);

      // refine according to direted graph of Array S1 of S1
//       for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
// 	generate_limit4(sv->string, sv->e);
      generate_limit_step1(4);

      // refine according to direted graph of Array S1 of S2 where S1 != S2
//       for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
// 	generate_limit5(sv->string, sv->e);
      generate_limit_step1(5);

      // set while guard
      generate_while_guard(scalarsetlist);

      // end while loop
      fprintf(codefile, "    } // end while\n"); 

      // enter result
      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  // enter the result into class\n"
		"  if (Perm.MTO_class_%s())\n"
		"    {\n"
		"      for (i=0; i<%d; i++)\n"
		"        for (j=0; j<%d; j++)\n"
		"          if (pos_%s[i][j])\n"
		"            Perm.class_%s[j] = i;\n"
		"      Perm.undefined_class_%s=0;\n"
		"      for (j=0; j<%d; j++)\n"
		"        if (Perm.class_%s[j]>Perm.undefined_class_%s)\n"
		"          Perm.undefined_class_%s=Perm.class_%s[j];\n"
		"    }\n",
		sv->e->mu_name,
		sv->e->getsize(), sv->e->getsize(), // i-for j-for
		sv->e->mu_name, // pos
		sv->e->mu_name, // class
		sv->e->mu_name, // undefined_class
		sv->e->getsize(), // i-for
		sv->e->mu_name, sv->e->mu_name, // class_%s undefined_class_%s
		sv->e->mu_name, sv->e->mu_name  // undefined_class_%s class_%s
		);
      
      fprintf(codefile,"}\n");
    }
  else
    fprintf(codefile,
            "void %s::Limit(PermSet& Perm){}\n",
            mu_name);
}

void recordtypedecl::generate_limit_function(symmetryclass& symmetry)
{
  ste *f;
  
  if (already_generated_limit_function) return;
  else already_generated_limit_function = TRUE;
  
  for (f = fields; f != NULL; f = f->next)
    f->getvalue()->gettype()->generate_limit_function(symmetry);
  
  fprintf(codefile,
	  "void %s::Limit(PermSet& Perm)\n"
	  "{\n",
	  mu_name
	  );
  for (f = fields; f != NULL; f = f->next)
    if (f->getvalue()->gettype()->getstructure() != typedecl::NoScalarset
	&& f->getvalue()->gettype()->getstructure() != typedecl::ScalarsetVariable
	&& f->getvalue()->gettype()->getstructure() != typedecl::ScalarsetArrayOfFree
	&& f->getvalue()->gettype()->getstructure() != typedecl::MultisetOfFree)
      fprintf(codefile,
	      "  %s.Limit(Perm);\n",
	      f->getvalue()->generate_code()
	      );
  fprintf(codefile,"};\n");
}

void scalarsettypedecl::generate_limit_function(symmetryclass& symmetry)
{
  if (already_generated_limit_function) return;
  else already_generated_limit_function = TRUE;
  
  fprintf(codefile,
	  "void %s::Limit(PermSet& Perm) {}\n",
	  mu_name);
}

void uniontypedecl::generate_limit_function(symmetryclass& symmetry)
{
  if (already_generated_limit_function) return;
  else already_generated_limit_function = TRUE;

  fprintf(codefile,
	  "void %s::Limit(PermSet& Perm) {}\n",
	  mu_name);
}
void typedecl::generate_limit_function(symmetryclass& symmetry)
{ Error.Error("Internal: typedecl::generate_limit_function()"); };

/********************
  generate MultisetLimit() for each typedecl
  ********************/

void enumtypedecl::generate_multisetlimit_function(symmetryclass& symmetry)
{
  if (already_generated_multisetlimit_function) return;
  else already_generated_multisetlimit_function = TRUE;
  
  if (strcmp(mu_name,"mu_0_boolean")!=0) 
    fprintf(codefile,
            "void %s::MultisetLimit(PermSet& Perm)\n"
	    "{ Error.Error(\"Internal: calling MultisetLimit for enum type.\\n\"); };\n",
            mu_name);
};
void subrangetypedecl::generate_multisetlimit_function(symmetryclass& symmetry)
{
  if (already_generated_multisetlimit_function) return;
  else already_generated_multisetlimit_function = TRUE;
  
  fprintf(codefile,
          "void %s::MultisetLimit(PermSet& Perm)\n"
	  "{ Error.Error(\"Internal: calling MultisetLimit for subrange type.\\n\"); };\n",
          mu_name);
};

void multisettypedecl::generate_multiset_simple_sort()
{
  // similar to MultisetSort
  // but use CompareWeight instead of Compare,
  // ignoring the part of elements with scalarset
  fprintf(codefile, 	      
	  "  %s temp;\n"
	  "\n"   
	  "  // compact\n"
	  "  for (i = 0, j = 0; i < %d; i++)\n"
	  "    if (valid[i].value())\n"
	  "      {\n"
	  "        if (j!=i)\n"
	  "          array[j++] = array[i];\n"
	  "        else\n"
	  "          j++;\n"
	  "      }\n"
	  "  if (j != current_size) current_size = j;\n"
	  "  for (i = j; i < %d; i++)\n"
	  "    array[i].undefine();\n"
	  "  for (i = 0; i < j; i++)\n"
	  "    valid[i].value(TRUE);\n"
	  "  for (i = j; i < %d; i++)\n"
	  "    valid[i].value(FALSE);\n"
	  "\n"
	  "  // bubble sort\n"
	  "  for (i = 0; i < current_size; i++)\n"
	  "    for (j = i+1; j < current_size; j++)\n"
	  "      if (CompareWeight(array[i],array[j])>0)\n"
	  "        {\n"
	  "          temp = array[i];\n"
	  "          array[i] = array[j];\n"
	  "          array[j] = temp;\n"
	  "        }\n",
	  elementtype->generate_code(),
	  maximum_size,
	  maximum_size,
	  maximum_size
	  );

  fprintf(codefile,
	  "  // initializing pos array\n"
	  "    for (i=0; i<current_size; i++)\n"
	  "      for (j=0; j<current_size; j++)\n"
	  "        pos_multisetindex[i][j]=FALSE;\n"
	  "    count_multisetindex = 1;\n"
	  "    pos_multisetindex[0][0] = TRUE;\n"
	  "    for (i = 1, j = 0 ; i < current_size; i++)\n"
	  "      if (CompareWeight(array[i-1],array[i])==0)\n"
	  "        pos_multisetindex[j][i] = TRUE;\n"
	  "      else\n"
	  "        { count_multisetindex++; pos_multisetindex[++j][i] = TRUE; }\n"
	  );
}
void multisettypedecl::generate_multiset_while(charlist * scalarsetlist)
{
  // similar to arraytypedecl::generate_while, except that we check
  // the index array explicitly

  charlist * sl;
  
  fprintf(codefile, 
	  "\n"
	  "  // refinement -- checking priority in general\n");
  
  // setup initial while guard
  fprintf(codefile, "  while_guard = (count_multisetindex < current_size);\n");
  for (sl=scalarsetlist; sl!=NULL; sl = sl->next)
    fprintf(codefile,
	    "  while_guard = while_guard || (Perm.MTO_class_%s() && count_%s<%d);\n",
	    sl->e->mu_name, 
	    sl->e->mu_name, sl->e->getsize()
	    );
  
  fprintf(codefile, 
	  "  while ( while_guard )\n"
	  "    {\n"
	  "      oldcount_multisetindex = count_multisetindex;\n"
	  );
  
  // setup oldcount
  for (sl=scalarsetlist; sl!=NULL; sl = sl->next)
    fprintf(codefile, 
	    "      oldcount_%s = count_%s;\n",
	    sl->e->mu_name, sl->e->mu_name
	    );
}
void multisettypedecl::generate_multiset_while_guard(charlist * scalarsetlist)
{
  charlist * sl;

  fprintf(codefile, "      while_guard = oldcount_multisetindex!=count_multisetindex;\n");
  for (sl=scalarsetlist; sl!=NULL; sl = sl->next)
    fprintf(codefile, 
	    "      while_guard = while_guard || (oldcount_%s!=count_%s);\n",
	    sl->e->mu_name, sl->e->mu_name
	    );
  fprintf(codefile, "      while_guard_temp = while_guard;\n");
  fprintf(codefile, "      while_guard = (count_multisetindex < current_size);\n");
  for (sl=scalarsetlist; sl!=NULL; sl = sl->next)
    fprintf(codefile,
	    "      while_guard = while_guard || count_%s<%d;\n",
	    sl->e->mu_name, sl->e->getsize()
	    );
  fprintf(codefile, "      while_guard = while_guard && while_guard_temp;\n");
}

void multisettypedecl::generate_limit_step1(int limit_strategy)
{
  generate_limit_step2("",elementtype,0,limit_strategy);
}
void multisettypedecl::generate_limit_step2(char * var, typedecl * e, int local_num, int limit_strategy)
{
  stelist * t;
  ste * f;
  typedecl * e2, * d2;  

  // please check that scalarset of size 1 is handled by sort in the other routine

  if (e->gettypeclass() == typedecl::Scalarset
      && e->getsize() > 1)
    generate_limit_step3(var, e, limit_strategy, FALSE);
  else if (e->gettypeclass() == typedecl::Union
	   && ((uniontypedecl *)e)->IsUnionWithScalarset())
    {
      for(t=((uniontypedecl *)e)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d2 = (typedecl *)t->s->getvalue();
          if (d2->gettypeclass() == typedecl::Scalarset && d2->getsize() > 1 ) 
	    generate_limit_step3(var, d2, limit_strategy, TRUE);
        }
    }
  else if (e->gettypeclass() == typedecl::Array)
    {
      e2 = e->getindextype();
      switch (e2->gettypeclass()) {
      case typedecl::Scalarset:
	if (e2->getsize() == 1)
	  generate_limit_step2(tsprintf("%s[mu_%s]",var,((scalarsettypedecl *)e2)->getidvalues()->getname()->getname()),
			       e->getelementtype(), local_num, limit_strategy);
	break;
      case typedecl::Union:
	for(t=((uniontypedecl *)e2)->getunionmembers();
	    t!= NULL; t=t->next)
	  {
	    d2 = (typedecl *)t->s->getvalue();
	    switch (d2->gettypeclass()) {
	    case typedecl::Enum:
	      for (f = ((enumtypedecl *) d2)->getidvalues();
		   f!=NULL && type_equal( f->getvalue()->gettype(), d2);
		   f = f->getnext()) 
		generate_limit_step2(tsprintf("%s[mu_%s]",var,f->getname()->getname()),
				     e->getelementtype(), local_num, limit_strategy);
	      break;
	    case typedecl::Range:
	      fprintf(codefile,
		      "  // loop through elements of a array indexed by subrange(union)\n"
		      "  for (i%d = %d; i%d <= %d; i%d++)\n"
		      "  {\n",
		      local_num, d2->getleft(), local_num, d2->getright(), local_num);
	      generate_limit_step2(tsprintf("%s[i%d]",var,local_num),
				   e->getelementtype(), local_num+1, limit_strategy);
	      fprintf(codefile,
		      "  }\n");
	      break;
	    }
	  }
      case typedecl::Enum:
	for (f = ((enumtypedecl *)e2)->getidvalues();
	     f!=NULL && type_equal( f->getvalue()->gettype(), e2);
	     f = f->getnext()) 
	  generate_limit_step2(tsprintf("%s[mu_%s]",var,f->getname()->getname()),
				e->getelementtype(), local_num, limit_strategy);
	break;
      case typedecl::Range:
	fprintf(codefile,
		"  // loop through elements of a array indexed by subrange\n"
		"  for (i%d = %d; i%d <= %d; i%d++)\n"
		"  {\n",
		local_num, e2->getleft(), local_num, e2->getright(), local_num);
	generate_limit_step2(tsprintf("%s[i%d]",var,local_num),
			      e->getelementtype(), local_num+1, limit_strategy);
	fprintf(codefile,
		"  }\n");
	break;
      }
    }
  else if (e->gettypeclass() == typedecl::Record)
    {
      for (f = e->getfields(); f != NULL; f = f->next)
	{
	  generate_limit_step2(tsprintf("%s.mu_%s",var, f->getname()->getname()),
				f->getvalue()->gettype(), local_num, limit_strategy);
	}
    }
}
void multisettypedecl::generate_limit_step3(char * var, typedecl * e, int limit_strategy, bool isunion)
{
  if (limit_strategy == 5)
    generate_multiset_limit5(var, e->mu_name, e->getsize(), e->getleft(), isunion);
}

void multisettypedecl::generate_multiset_limit5(char * var, typedecl * e)
{
  stelist * t;
  typedecl * d2;  
  int thisright;
  int thisleft;
  
  if (e->gettypeclass() == typedecl::Scalarset
      && e->getsize() > 1)
    generate_multiset_limit5(var,e->mu_name, e->getsize(), e->getleft(),FALSE);
  else if (e->gettypeclass() == typedecl::Union)
    {
      thisright = e->getsize()-1;
      for(t=((uniontypedecl *)e)->getunionmembers();
          t!= NULL; t=t->next)
        {
          d2 = (typedecl *)t->s->getvalue();
          thisleft = thisright-d2->getsize()+1;
          if (d2->gettypeclass() == typedecl::Scalarset && d2->getsize() > 1)
	    fprintf(codefile,
		    "\n"
		    "      // refinement -- graph structure for two scalarsets\n"
		    "      //               as in array S1 of S2\n"
		    "      // only if there is more than 1 permutation in class\n"
		    "      if ( (count_multisetindex<current_size)\n"
		    "           || ( Perm.MTO_class_%s() && count_%s<%d) )\n"
		    "        {\n"
		    "          exists = FALSE;\n"
		    "          split = FALSE;\n"
		    "          for (k=0; k<current_size; k++) // step through class\n"
		    "            if ((!(*this)[k]%s.isundefined())\n"
		    "                && (*this)[k]%s>=%d\n"
		    "                && (*this)[k]%s<=%d)\n"
		    "              split = TRUE;\n"
		    "          if (split)\n"
		    "            {\n"
		    "              for (i=0; i<count_multisetindex; i++) // scan through array index priority\n"
		    "                for (j=0; j<count_%s; j++) //scan through element priority\n"
		    "                  {\n"
		    "                    exists = FALSE;\n"
		    "                    for (k=0; k<current_size; k++) // initialize goodset\n"
		    "                      goodset_multisetindex[k] = FALSE;\n"
		    "                    for (k=0; k<%d; k++) // initialize goodset\n"
		    "                      goodset_%s[k] = FALSE;\n"
		    "                    for (k=0; k<current_size; k++) // scan array index\n"
		    "                      // set goodsets\n"
		    "                      if (pos_multisetindex[i][k] \n"
		    "                          && !(*this)[k]%s.isundefined()\n"
		    "                          && (*this)[k]%s>=%d\n"          // extra checking
		    "                          && (*this)[k]%s<=%d\n"           // extra checking
		    "                          && pos_%s[j][(*this)[k]%s-%d])\n"
		    "                        {\n"
		    "                          exists = TRUE;\n"
		    "                          goodset_multisetindex[k] = TRUE;\n"
		    "                          goodset_%s[(*this)[k]%s-%d] = TRUE;\n"
		    "                        }\n"
		    "                    if (exists)\n"
		    "                      {\n"
		    "                        // set split for the array index type\n" 
		    "                        split=FALSE;\n"
		    "                        for (k=0; k<current_size; k++)\n"
		    "                          if ( pos_multisetindex[i][k] && !goodset_multisetindex[k] )\n"
		    "                            split= TRUE;\n"
		    "                        if (split)\n"
		    "                          {\n"
		    "                            // move following pos entries down 1 step\n"
		    "                            for (z=count_multisetindex; z>i; z--)\n"
		    "                              for (k=0; k<current_size; k++)\n"
		    "                                pos_multisetindex[z][k] = pos_multisetindex[z-1][k];\n"
		    "                            // split pos\n"
		    "                            for (k=0; k<current_size; k++)\n"
		    "                              {\n"
		    "                                if (pos_multisetindex[i][k] && !goodset_multisetindex[k])\n"
		    "                                  pos_multisetindex[i][k] = FALSE;\n"
		    "                                if (pos_multisetindex[i+1][k] && goodset_multisetindex[k])\n"
		    "                                  pos_multisetindex[i+1][k] = FALSE;\n"
		    "                              }\n"
		    "                            count_multisetindex++;\n"
		    "                          }\n"
		    "                        // set split for the element type\n" 
		    "                        split=FALSE;\n"
		    "                        for (k=0; k<%d; k++)\n"
		    "                          if ( pos_%s[j][k] && !goodset_%s[k] )\n"
		    "                            split= TRUE;\n"
		    "                        if (split)\n"
		    "                          {\n"
		    "                            // move following pos entries down 1 step\n"
		    "                            for (z=count_%s; z>j; z--)\n"
		    "                              for (k=0; k<%d; k++)\n"
		    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
		    "                            // split pos\n"
		    "                            for (k=0; k<%d; k++)\n"
		    "                              {\n"
		    "                                if (pos_%s[j][k] && !goodset_%s[k])\n"
		    "                                  pos_%s[j][k] = FALSE;\n"
		    "                                if (pos_%s[j+1][k] && goodset_%s[k])\n"
		    "                                  pos_%s[j+1][k] = FALSE;\n"
		    "                              }\n"
		    "                            count_%s++;\n"
		    "                          }\n"
		    "                      }\n"
		    "                  }\n"
		    "            }\n"
		    "        }\n",
		    d2->mu_name, d2->mu_name, d2->getsize(), // if guard2
		    var, // (*this)[k]%s
// 		    var, thisleft, // (*this)[k]%s<%d
// 		    var, thisright, // (*this)[k]%s>%d
		    var, d2->getleft(), // (*this)[k]%s<%d
		    var, d2->getright(), // (*this)[k]%s>%d
		    
		    d2->mu_name, // j-for
		    d2->getsize(), d2->mu_name, // initialize goodset

		    var,               // if condition
// 		    var, thisleft, // (*this)[k_%d]%s<%d
// 		    var, thisright, // (*this)[k_%d]%s>%d
		    var, d2->getleft(), // (*this)[k]%s<%d
		    var, d2->getright(), // (*this)[k]%s>%d
		    d2->mu_name, var, d2->getleft(), // if condition (cont)
		    d2->mu_name, var, d2->getleft(), // set goodsets
		    
		    d2->getsize(), // k-for -- split
		    d2->mu_name, d2->mu_name, // if condition
		    d2->mu_name, d2->getsize(), d2->mu_name, d2->mu_name, // move pos down 1 step
		    d2->getsize(), d2->mu_name, d2->mu_name, d2->mu_name, // split pos
		    d2->mu_name, d2->mu_name, d2->mu_name,               // split pos (cont)
		    d2->mu_name // count_%s
		    
		    );
          thisright= thisleft-1;
        }
    }
}
void multisettypedecl::generate_multiset_limit5(char *var, char * name2, int size2, int left2, bool isunion)
{
  if (isunion)
    fprintf(codefile,
	    "\n"
	    "      // refinement -- graph structure for two scalarsets\n"
	    "      //               as in array S1 of S2\n"
	    "      // only if there is more than 1 permutation in class\n"
	    "      if ( (count_multisetindex<current_size)\n"
	    "           || ( Perm.MTO_class_%s() && count_%s<%d) )\n"
	    "        {\n"
	    "          exists = FALSE;\n"
	    "          split = FALSE;\n"
	    "          for (k=0; k<current_size; k++) // step through class\n"
	    "            if ((!(*this)[k]%s.isundefined())\n"
	    "                && (*this)[k]%s>=%d\n"
	    "                && (*this)[k]%s<=%d)\n"
	    "              split = TRUE;\n"
	    "          if (split)\n"
	    "            {\n"
	    "              for (i=0; i<count_multisetindex; i++) // scan through array index priority\n"
	    "                for (j=0; j<count_%s; j++) //scan through element priority\n"
	    "                  {\n"
	    "                    exists = FALSE;\n"
	    "                    for (k=0; k<current_size; k++) // initialize goodset\n"
	    "                      goodset_multisetindex[k] = FALSE;\n"
	    "                    for (k=0; k<%d; k++) // initialize goodset\n"
	    "                      goodset_%s[k] = FALSE;\n"
	    "                    for (k=0; k<current_size; k++) // scan array index\n"
	    "                      // set goodsets\n"
	    "                      if (pos_multisetindex[i][k] \n"
	    "                          && !(*this)[k]%s.isundefined()\n"
	    "                          && (*this)[k]%s>=%d\n"          // extra checking
	    "                          && (*this)[k]%s<=%d\n"           // extra checking
	    "                          && pos_%s[j][(*this)[k]%s-%d])\n"
	    "                        {\n"
	    "                          exists = TRUE;\n"
	    "                          goodset_multisetindex[k] = TRUE;\n"
	    "                          goodset_%s[(*this)[k]%s-%d] = TRUE;\n"
	    "                        }\n"
	    "                    if (exists)\n"
	    "                      {\n"
	    "                        // set split for the array index type\n" 
	    "                        split=FALSE;\n"
	    "                        for (k=0; k<current_size; k++)\n"
	    "                          if ( pos_multisetindex[i][k] && !goodset_multisetindex[k] )\n"
	    "                            split= TRUE;\n"
	    "                        if (split)\n"
	    "                          {\n"
	    "                            // move following pos entries down 1 step\n"
	    "                            for (z=count_multisetindex; z>i; z--)\n"
	    "                              for (k=0; k<current_size; k++)\n"
	    "                                pos_multisetindex[z][k] = pos_multisetindex[z-1][k];\n"
	    "                            // split pos\n"
	    "                            for (k=0; k<current_size; k++)\n"
	    "                              {\n"
	    "                                if (pos_multisetindex[i][k] && !goodset_multisetindex[k])\n"
	    "                                  pos_multisetindex[i][k] = FALSE;\n"
	    "                                if (pos_multisetindex[i+1][k] && goodset_multisetindex[k])\n"
	    "                                  pos_multisetindex[i+1][k] = FALSE;\n"
	    "                              }\n"
	    "                            count_multisetindex++;\n"
	    "                          }\n"
	    "                        // set split for the element type\n" 
	    "                        split=FALSE;\n"
	    "                        for (k=0; k<%d; k++)\n"
	    "                          if ( pos_%s[j][k] && !goodset_%s[k] )\n"
	    "                            split= TRUE;\n"
	    "                        if (split)\n"
	    "                          {\n"
	    "                            // move following pos entries down 1 step\n"
	    "                            for (z=count_%s; z>j; z--)\n"
	    "                              for (k=0; k<%d; k++)\n"
	    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
	    "                            // split pos\n"
	    "                            for (k=0; k<%d; k++)\n"
	    "                              {\n"
	    "                                if (pos_%s[j][k] && !goodset_%s[k])\n"
	    "                                  pos_%s[j][k] = FALSE;\n"
	    "                                if (pos_%s[j+1][k] && goodset_%s[k])\n"
	    "                                  pos_%s[j+1][k] = FALSE;\n"
	    "                              }\n"
	    "                            count_%s++;\n"
	    "                          }\n"
	    "                      }\n"
	    "                  }\n"
	    "            }\n"
	    "        }\n",
	    name2, name2, size2, // if guard2
	    var, // (*this)[k]%s
	    var, left2, // (*this)[k]%s<%d
	    var, left2+size2-1, // (*this)[k]%s>%d
	    
	    name2, // j-for
	    size2, name2, // initialize goodset
	    
	    var,               // if condition
	    var, left2, // (*this)[k]%s<%d
	    var, left2+size2-1, // (*this)[k]%s>%d
	    name2, var, left2, // if condition (cont)
	    name2, var, left2, // set goodsets
	    
	    size2, // k-for -- split
	    name2, name2, // if condition
	    name2, size2, name2, name2, // move pos down 1 step
	    size2, name2, name2, name2, // split pos
	    name2, name2, name2,               // split pos (cont)
	    name2 // count_%s
	    
	    );
  else
    fprintf(codefile,
	    "\n"
	    "      // refinement -- graph structure for two scalarsets\n"
	    "      //               as in array S1 of S2\n"
	    "      if ( ( count_multisetindex<current_size)\n"
	    "           || ( Perm.MTO_class_%s() && count_%s<%d) )\n"
	    "        {\n"
	    "          exists = FALSE;\n"
	    "          split = FALSE;\n"
	    "          for (k=0; k<current_size; k++) // step through class\n"
	    "            if (!(*this)[k]%s.isundefined())\n"
	    "              split = TRUE;\n"
	    "          if (split)\n"
	    "            {\n"
	    "              for (i=0; i<count_multisetindex; i++) // scan through array index priority\n"
	    "                for (j=0; j<count_%s; j++) //scan through element priority\n"
	    "                  {\n"
	    "                    exists = FALSE;\n"
	    "                    for (k=0; k<current_size; k++) // initialize goodset\n"
	    "                      goodset_multisetindex[k] = FALSE;\n"
	    "                    for (k=0; k<%d; k++) // initialize goodset\n"
	    "                      goodset_%s[k] = FALSE;\n"
	    "                    for (k=0; k<current_size; k++) // scan array index\n"
	    "                      // set goodsets\n"
	    "                      if (pos_multisetindex[i][k] \n"
	    "                          && !(*this)[k]%s.isundefined()\n"
	    "                          && pos_%s[j][(*this)[k]%s-%d])\n"
	    "                        {\n"
	    "                          exists = TRUE;\n"
	    "                          goodset_multisetindex[k] = TRUE;\n"
	    "                          goodset_%s[(*this)[k]%s-%d] = TRUE;\n"
	    "                        }\n"
	    "                    if (exists)\n"
	    "                      {\n"
	    "                        // set split for the array index type\n" 
	    "                        split=FALSE;\n"
	    "                        for (k=0; k<current_size; k++)\n"
	    "                          if ( pos_multisetindex[i][k] && !goodset_multisetindex[k] )\n"
	    "                            split= TRUE;\n"
	    "                        if (split)\n"
	    "                          {\n"
	    "                            // move following pos entries down 1 step\n"
	    "                            for (z=count_multisetindex; z>i; z--)\n"
	    "                              for (k=0; k<current_size; k++)\n"
	    "                                pos_multisetindex[z][k] = pos_multisetindex[z-1][k];\n"
	    "                            // split pos\n"
	    "                            for (k=0; k<current_size; k++)\n"
	    "                              {\n"
	    "                                if (pos_multisetindex[i][k] && !goodset_multisetindex[k])\n"
	    "                                  pos_multisetindex[i][k] = FALSE;\n"
	    "                                if (pos_multisetindex[i+1][k] && goodset_multisetindex[k])\n"
	    "                                  pos_multisetindex[i+1][k] = FALSE;\n"
	    "                              }\n"
	    "                            count_multisetindex++;\n"
	    "                          }\n"
	    "                        // set split for the element type\n" 
	    "                        split=FALSE;\n"
	    "                        for (k=0; k<%d; k++)\n"
	    "                          if ( pos_%s[j][k] && !goodset_%s[k] )\n"
	    "                            split= TRUE;\n"
	    "                        if (split)\n"
	    "                          {\n"
	    "                            // move following pos entries down 1 step\n"
	    "                            for (z=count_%s; z>j; z--)\n"
	    "                              for (k=0; k<%d; k++)\n"
	    "                                pos_%s[z][k] = pos_%s[z-1][k];\n"
	    "                            // split pos\n"
	    "                            for (k=0; k<%d; k++)\n"
	    "                              {\n"
	    "                                if (pos_%s[j][k] && !goodset_%s[k])\n"
	    "                                  pos_%s[j][k] = FALSE;\n"
	    "                                if (pos_%s[j+1][k] && goodset_%s[k])\n"
	    "                                  pos_%s[j+1][k] = FALSE;\n"
	    "                              }\n"
	    "                            count_%s++;\n"
	    "                          }\n"
	    "                      }\n"
	    "                  }\n"
	    "            }\n"
	    "        }\n",
	    name2, name2, size2, // if guard2
	    var, // (*this)[k_%d]%s
	    
	    name2, // j-for
	    size2, name2, // initialize goodset
	    var,               // if condition
	    name2, var, left2, // if condition (cont)
	    name2, var, left2, // set goodsets
	    
	    size2, // k-for -- split
	    name2, name2, // if condition
	    name2, size2, name2, name2, // move pos down 1 step
	    size2, name2, name2, name2, // split pos
	    name2, name2, name2,               // split pos (cont)
	    name2 // count_%s
	    
	    );
}
void multisettypedecl::generate_multisetlimit_function(symmetryclass& symmetry)
{
  charlist * scalarsetlist;
  charlist * scalarsetvalue;
  charlist * sv;
  
  if (already_generated_multisetlimit_function) return;
  else already_generated_multisetlimit_function = TRUE;
  
  elementtype->generate_multisetlimit_function(symmetry);

  if (!elementtype->HasScalarsetVariable())
    fprintf(codefile,
	    "void %s::MultisetLimit(PermSet& Perm)\n"
	    "{ Error.Error(\"Internal: calling MultisetLimit for multiset type of scalarset-free elements.\\n\"); };\n",
	    mu_name);
  else
    {
      fprintf(codefile,
	      "void %s::MultisetLimit(PermSet& Perm)\n"
	      "{\n"
              "  // indexes\n"
              "  int i,j,k,z;\n"
	      "  // while guard\n"
	      "  bool while_guard, while_guard_temp;\n"
              "  // sorting\n"
              "  static %s value[%d];\n"
              "  // limit\n"
              "  bool exists;\n"
              "  bool split;\n",
              mu_name, // %s::Canonicalize
              elementtype->mu_name, maximum_size // %s value[%d]
              );

      // get a list for all scalarsets that may appear
      scalarsetlist = generate_scalarset_list(NULL);

      // added by Uli
      fprintf(codefile, "  int i0");
      for (int i = get_num_limit_locals(elementtype)-1; i>0; i--)
        fprintf(codefile,",i%d",i);
      fprintf(codefile, ";\n");

      // declare for all scalarsets that may appear
      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  int count_%s, oldcount_%s;\n"
		"  bool pos_%s[%d][%d];\n"
		"  bool goodset_%s[%d];\n",
		sv->e->mu_name, sv->e->mu_name,
		sv->e->mu_name, sv->e->getsize(), sv->e->getsize(),
		sv->e->mu_name, sv->e->getsize()
		);
      // declare for the indexes of the multiset
      fprintf(codefile,
	      "  int count_multisetindex, oldcount_multisetindex;\n"
	      "  bool pos_multisetindex[%d][%d];\n"
	      "  bool goodset_multisetindex[%d];\n",
	      maximum_size,
	      maximum_size,
	      maximum_size
	      );
      
      generate_multiset_simple_sort();

      fprintf(codefile,
	      "  if (current_size == 1)\n"
	      "    {\n"
	      );
      if (elementtype->HasScalarsetVariable())
	fprintf(codefile,
		"      array[0].SimpleLimit(Perm);\n"
		);
      if (elementtype->HasScalarsetArrayOfFree())
	fprintf(codefile,
		"      array[0].ArrayLimit(Perm);\n"
		);
      if (elementtype->HasScalarsetArrayOfS())
	fprintf(codefile,
		"      array[0].Limit(Perm);\n"
		);
      fprintf(codefile,
	      "    }\n"
	      "  else\n"
	      "    {\n"
	      "\n"
	      );

      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  // initializing pos array\n"
		"  for (i=0; i<%d; i++)\n"
		"    for (j=0; j<%d; j++)\n"
		"      pos_%s[i][j]=FALSE;\n"
		"  count_%s = 0;\n"
		"  while (1)\n"
		"    {\n"
		"      exists = FALSE;\n"
		"      for (i=0; i<%d; i++)\n"
		"       if (Perm.class_%s[i] == count_%s)\n"
		"         {\n"
		"           pos_%s[count_%s][i]=TRUE;\n"
		"           exists = TRUE;\n"
		"         }\n"
		"      if (exists) count_%s++;\n"
		"      else break;\n"
		"    }\n",
		sv->e->getsize(), sv->e->getsize(), sv->e->mu_name, // initialization
		sv->e->mu_name, // count_%s
		sv->e->getsize(), // i-for
		sv->e->mu_name, sv->e->mu_name, // if-guard 
		sv->e->mu_name, sv->e->mu_name, // pos_%s[count%s]
		sv->e->mu_name // count_%s
		);
      
      // have to generate the while loop
      generate_multiset_while(scalarsetlist);

      // refine according to directed graph of Array S1 of S2 where S1 != S2
//       for (sv=scalarsetvalue; sv!=NULL; sv = sv->next)
// 	if (sv->e->getsize() > 1)
// 	  generate_multiset_limit5(sv->string, sv->e);

      generate_limit_step1(5);
      
      // set while guard
      generate_multiset_while_guard(scalarsetlist);

      // end while loop
      fprintf(codefile, "    } // end while\n"); 

      // enter result
      for (sv=scalarsetlist; sv!=NULL; sv = sv->next)
	fprintf(codefile,
		"  // enter the result into class\n"
		"  if (Perm.MTO_class_%s())\n"
		"    {\n"
		"      for (i=0; i<%d; i++)\n"
		"        for (j=0; j<%d; j++)\n"
		"          if (pos_%s[i][j])\n"
		"            Perm.class_%s[j] = i;\n"
		"      Perm.undefined_class_%s=0;\n"
		"      for (j=0; j<%d; j++)\n"
		"        if (Perm.class_%s[j]>Perm.undefined_class_%s)\n"
		"          Perm.undefined_class_%s=Perm.class_%s[j];\n"
		"    }\n",
		sv->e->mu_name,
		sv->e->getsize(), sv->e->getsize(), // i-for j-for
		sv->e->mu_name, // pos
		sv->e->mu_name, // class
		sv->e->mu_name, // undefined_class
		sv->e->getsize(), // i-for j-for
		sv->e->mu_name, sv->e->mu_name, // class_%s undefined_class_%s
		sv->e->mu_name, sv->e->mu_name  // undefined_class_%s class_%s
		);
      
      fprintf(codefile,
	      "  }\n"
	      "}\n");
    }
}

void arraytypedecl::generate_multisetlimit_function(symmetryclass& symmetry)
{
  if (already_generated_multisetlimit_function) return;
  else already_generated_multisetlimit_function = TRUE;
  
  elementtype->generate_multisetlimit_function(symmetry);

  if ( indextype->getstructure() != typedecl::ScalarsetVariable
       && elementtype->HasMultisetOfScalarset() )
    {
      fprintf(codefile,
              "void %s::MultisetLimit(PermSet& Perm)\n"
              "{\n"
              "  for (int j=0; j<%d; j++) {\n"
              "    array[j].MultisetLimit(Perm);\n"
              "  }\n"
              "}\n",
              mu_name,              // %s::Fast...
              indextype->getsize()          // j-for
              );
    }
  else
    fprintf(codefile,
	    "void %s::MultisetLimit(PermSet& Perm)\n"
	    "{ Error.Error(\"Internal: calling MultisetLimit for scalarset array.\\n\"); };\n",
	    mu_name);
};
void recordtypedecl::generate_multisetlimit_function(symmetryclass& symmetry)
{
  if (already_generated_multisetlimit_function) return;
  else already_generated_multisetlimit_function = TRUE;

  ste *f;
  
  for (f = fields; f != NULL; f = f->next)
    f->getvalue()->gettype()->generate_limit_function(symmetry);
  
  fprintf(codefile,
          "void %s::MultisetLimit(PermSet& Perm)\n"
	  "{\n",
	  mu_name
	  );
  for (f = fields; f != NULL; f = f->next)
    if (f->getvalue()->gettype()->HasMultisetOfScalarset())
      fprintf(codefile,
	      "  %s.MultisetLimit(Perm);\n",
	      f->getvalue()->generate_code()
	      );
  fprintf(codefile,"};\n");
};
void scalarsettypedecl::generate_multisetlimit_function(symmetryclass& symmetry)
{
  if (already_generated_multisetlimit_function) return;
  else already_generated_multisetlimit_function = TRUE;
  
  fprintf(codefile,
          "void %s::MultisetLimit(PermSet& Perm)\n"
	    "{ Error.Error(\"Internal: calling MultisetLimit for scalarset type.\\n\"); };\n",
          mu_name);
};

void uniontypedecl::generate_multisetlimit_function(symmetryclass& symmetry)
{
  if (already_generated_multisetlimit_function) return;
  else already_generated_multisetlimit_function = TRUE;
  
  fprintf(codefile,
          "void %s::MultisetLimit(PermSet& Perm)\n"
	  "{ Error.Error(\"Internal: calling MultisetLimit for union type.\\n\"); };\n",
          mu_name);
};

void typedecl::generate_multisetlimit_function(symmetryclass& symmetry)
{ Error.Error("Internal: typedecl::generate_multisetlimit_function()"); };

/****************************************
  * 20 Dec 93 Norris Ip: 
  basic structures
  * 31 Jan 94 Norris Ip:
  Only generate useful scalarset/scalarsetunion functions
  * 16 Feb 94 Norris Ip:
  Break down arraytypedecl into a series of procedures
  * 23 Feb 94 Norris Ip:
  debuged limit, limit2, limit3, and limit4
  expanding limit4 to more general case
  * 1 March 94 Norris Ip: 
  added
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  ******
  * Subject: Extension for {multiset, undefined value, general union}
  * 25 May 94 Norris Ip:
  change all scalarsetunion to union
  handled enum in union
beta2.9S released
  * 14 July 95 Norris Ip:
  Tidying/removing old algorithm
  adding small memory algorithm for larger scalarsets
****************************************/

/********************
 $Log: cpp_sym_decl.C,v $
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
