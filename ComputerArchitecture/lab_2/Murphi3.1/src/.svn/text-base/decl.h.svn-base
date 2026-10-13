/* -*- C++ -*-
 * decl.h
 * @(#) interfaces for Murphi declarations.
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

/* do not #include "mu.h"; mu.h includes this. */

/* 
 * Original Author: Ralph Melton
 * 
 * Update:
 *
 * C. Norris Ip 
 * Subject: symmetry extension
 * First version: April 93 
 * Transfer to Liveness Version:
 * First modified: November 8, 93
 *
 * C. Norris Ip
 * Subject: Extension for {multiset, undefined value, general union}
 * First modified: May 24, 94
 *
 */ 

#define BYTES(x) ((x)>0 ? ((x)>8?32:8) : 0)

/********************
  class decl
  ********************/
class decl: public TNode
{
protected:
public:
  // variables
  bool declared; // whether the item has been declared in the generated code.
  char *name;    // What the object\'s name is
  char *mu_name; // the object's name in the generated code
  
  // initializer
  decl();
  decl(char *name);
  
  // subclass identifier
  enum decl_class {Type, Const, Var, Alias, Quant, Choose, Param, Proc, Func, Error_decl};
  
  // supporting routines
  virtual bool isdecl() const { return TRUE; };
  virtual decl_class getclass() const { return Error_decl; }; 
  virtual typedecl *gettype(void) const; // every type of decl has a type defined. 
  virtual designator *getdesignator(ste *origin) const;
  
  // code generation
  virtual char *generate_code();
  virtual char *generate_decl();
  
  // base routines declarations for subclass routines
  virtual int getoffset() const // for vardecl
  { Error.Error("Getting offset from nonvariable.\n"); return -1; };
  virtual int getvalue() const // for constdecl
  { Error.Error("Getting value from nonconstant.\n"); return -1; };
  virtual expr *getexpr() const // for aliasdecl
  { Error.Error("Getting expr from nonalias.\n"); return error_expr; } 
  virtual int getsize() const // for scalarsettypedecl
  { Error.Error("Getting size from nonscalarset.\n"); return -1; };
};

/********************
  class typedecl
  -- the type of a typedecl is its ancestor type;
  -- two types are compatible iff their types are the same.
  ********************/

class typedecl: public decl
{
protected:
  int tNum;       /* it\'s distinct for each type. */
                  /* used to give different anonymous type a different name */
  int numbits;    /* the size of the type. */
  int bitsalloc;  /* number of bits allocated in the compacted world state */
  typedecl *next; /* linked list of typedecls */
  const char *mu_type;  /* murphi type from which it will inherit, */
                  /* only applicable for enumtype, subrange, multiset, */
                  /* scalarset and union. */
                  // Uli: made this const

  // scalarsets involved
  stelist *scalarsetlist; 

public:
  // origin of the list of typedecls
  static typedecl *origin; 

  // initializer
  typedecl();
  typedecl(char *name);
  
  // subclass identifier
  enum typeclass {Enum, Range, Array, MultiSet, MultiSetID, Record, Scalarset, Union, Error_type };
  
  // supporting routines
  virtual decl_class getclass() const { return Type; };
  virtual typeclass gettypeclass() const { return Error_type; };
  virtual typedecl *gettype() const { return (typedecl *) this; };
  virtual int getnumbits() const { return numbits; };
  virtual int getbitsalloc() const { return bitsalloc; };
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
  void generate_all_decl();
  void generate_all_symmetry_decl(symmetryclass& symmetry);
  
  // base routines declarations for subclass routines
  virtual bool issimple() const
  { Error.Error("Internal: typedecl::issimple()"); return FALSE; };
  virtual int getsize() const
  { Error.Error("Internal: typedecl::getsize()"); return 0; };
  virtual int getleft() const
  { Error.Error("Internal: typedecl::getleft()"); return 0; };
  virtual int getright() const
  { Error.Error("Internal: typedecl::getright()"); return 1; };
  virtual designator *getdesignator(ste *origin) const
  {
    Error.Error("Internal: bad call to typedecl::getdesignator()");
    return error_designator;
  }

  virtual typedecl *getindextype() const; // for arraytypedecl
  virtual typedecl *getelementtype() const; // for arraytypedecl
  virtual ste *getfields() const // for recordtypedecl
  { Error.Error("Getting fields from nonrecord.\n"); return NULL; };

  // scalarset structure
  enum structureclass
  {  NoScalarset, ScalarsetVariable, 
       ScalarsetArrayOfFree, ScalarsetArrayOfScalarset, 
       MultisetOfFree, MultisetOfScalarset, Complex };

  structureclass structure;
  stelist * getscalarsetlist() const { return scalarsetlist; } ;
  structureclass getstructure() const { return structure; };
  virtual bool HasConstant() const
  { Error.Error("Internal: typedecl::HasConstant().\n"); return FALSE; };
  virtual bool HasScalarsetVariable() const
  { Error.Error("Internal: typedecl::HasScalarsetVariable().\n"); return FALSE; };
  virtual bool HasScalarsetArrayOfFree() const
  { Error.Error("Internal: typedecl::HasScalarsetArrayOfFree().\n"); return FALSE; };
  virtual bool HasScalarsetArrayOfS() const
  { Error.Error("Internal: typedecl::HasScalarsetArrayOfS().\n"); return FALSE; };
  virtual bool HasScalarsetLeaf() const
  { Error.Error("Internal: typedecl::HasScalarsetLeaf().\n"); return FALSE; };
  virtual bool HasMultisetOfFree() const
  { Error.Error("Internal: typedecl::HasMultisetOfFree().\n"); return FALSE; };
  virtual bool HasMultisetOfScalarset() const
  { Error.Error("Internal: typedecl::HasMultisetOfScalarset().\n"); return FALSE; };
  
  char * structurestring() const 
  {
    switch (structure) {
    case NoScalarset:
      return "NoScalarset";
    case ScalarsetVariable:
      return "ScalarsetVariable";
    case ScalarsetArrayOfFree:
      return "ScalarsetArrayOfFree";
    case ScalarsetArrayOfScalarset:
      return "ScalarsetArrayOfScalarset";
    case Complex:
      return "Complex";
    default:
      return "Complex";
    };
  };
  virtual void setusefulness()
  { Error.Error("Setting Usefulness for typedecl.\n"); };

  // canonicalization code for symmetry
  bool already_generated_permute_function;
  virtual void generate_permute_function()
  { Error.Error("Internal: typedecl::generate_permute_function()"); };

  bool already_generated_simple_canon_function;
  virtual void generate_simple_canon_function(symmetryclass& symmetry);

  bool already_generated_canonicalize_function;
  virtual void generate_canonicalize_function(symmetryclass& symmetry);

  bool already_generated_simple_limit_function;
  virtual void generate_simple_limit_function(symmetryclass& symmetry);

  bool already_generated_array_limit_function;
  virtual void generate_array_limit_function(symmetryclass& symmetry);

  bool already_generated_limit_function;
  virtual void generate_limit_function(symmetryclass& symmetry);

  bool already_generated_multisetlimit_function;
  virtual void generate_multisetlimit_function(symmetryclass& symmetry);

  virtual charlist * generate_scalarset_list(charlist * sl)
  { Error.Error("Internal: typedecl::generate_scalarset_list()"); return NULL; }
};

extern typedecl * voidtype;

/********************
  class enumtypedecl
  -- in order to allow easy implementation
  -- of disjoint union of scalarset/enum, the range
  -- of integer for each scalarset/enum is disjoint.
  ********************/
class enumtypedecl: public typedecl
{
  /* enum values are all represented as integers. */
  int left, right; /* least value, greatest. */
  // int shift;
  ste * idvalues;  /* used to get names of enums. */
public:
  // initializer
  enumtypedecl(int left, int right);

  // supporting routines
  virtual typeclass gettypeclass() const { return Enum; };
  virtual int getsize() const { return right - left + 1; };
  virtual int getleft() const { return left; };
  virtual int getright() const { return right; };
  virtual void setright( int rt ) { 
    // right = rt-shift; 
    right = rt;
    numbits = CeilLog2(right - left + 2);
    if (args->no_compression) {
	bitsalloc = BYTES(numbits);
	if (numbits>8 || right > 254) {
	  mu_type = "mu__long";
	  bitsalloc = 32;
	}
      }
    else
	bitsalloc = numbits;
  };
  virtual ste * getidvalues() const { return idvalues; };
  virtual void setidvalues( ste * vs ) { idvalues = vs; };
  virtual bool issimple() const { return TRUE; }
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();

  // classify scalarset types as useful or useless
  virtual void setusefulness() {}

  // classify variable type
  virtual bool HasConstant() const { return TRUE; }
  virtual bool HasScalarsetVariable() const { return FALSE; }
  virtual bool HasScalarsetArrayOfFree() const { return FALSE; }
  virtual bool HasScalarsetArrayOfS() const { return FALSE; }
  virtual bool HasScalarsetLeaf() const { return FALSE; }
  virtual bool HasMultisetOfFree() const  { return FALSE; }
  virtual bool HasMultisetOfScalarset() const  { return FALSE; }

  // canonicalization code for symmetry
  virtual void generate_permute_function();
  virtual void generate_simple_canon_function(symmetryclass& symmetry);
  virtual void generate_canonicalize_function(symmetryclass& symmetry);
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  virtual void generate_limit_function(symmetryclass& symmetry);
  virtual void generate_multisetlimit_function(symmetryclass& symmetry);

  virtual charlist * generate_scalarset_list(charlist * sl);
};

/********************
  class subrangetypedecl
  subrange of ints or enums. 
  -- can it be enums? Norris.
  ********************/
class subrangetypedecl: public typedecl 
{
  int left, right;   /* least value, greatest. */
  typedecl * parent; /* parent type. */
public:
  // initializer
  subrangetypedecl(int left, int right, typedecl * parent);
  subrangetypedecl(expr *left, expr *right);
  
  // supporting routines
  virtual int getsize() const { return right - left + 1; };
  virtual int getleft() const { return left; };
  virtual int getright() const { return right; };
  virtual typedecl* gettype() const { return parent->gettype(); };
  virtual typeclass gettypeclass() const { return Range; };
  virtual bool issimple() const { return TRUE; }
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();

  // classify scalarset types as useful or useless
  virtual void setusefulness() {}

  // classify variable type
  virtual bool HasConstant() const { return TRUE; }
  virtual bool HasScalarsetVariable() const { return FALSE; }
  virtual bool HasScalarsetArrayOfFree() const { return FALSE; }
  virtual bool HasScalarsetArrayOfS() const { return FALSE; }
  virtual bool HasScalarsetLeaf() const { return FALSE; }
  virtual bool HasMultisetOfFree() const  { return FALSE; }
  virtual bool HasMultisetOfScalarset() const  { return FALSE; }

  // canonicalization code for symmetry
  virtual void generate_permute_function();
  virtual void generate_simple_canon_function(symmetryclass& symmetry);
  virtual void generate_canonicalize_function(symmetryclass& symmetry);
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  virtual void generate_limit_function(symmetryclass& symmetry);
  virtual void generate_multisetlimit_function(symmetryclass& symmetry);
  virtual charlist * generate_scalarset_list(charlist * sl);
};

/********************
  class arraytypedecl
  ********************/
class arraytypedecl: public typedecl
{
  typedecl *indextype;
  typedecl *elementtype;
  bool interleaved;      // for EVER use -- BDD 
  
  virtual void generate_simple_sort();
  virtual void generate_simple_sort(int size, char * name, int left);

  virtual void generate_limit();
  virtual void generate_limit(int size, char * name);

  virtual void generate_limit2(char * var, typedecl * e);
  virtual void generate_limit2(int size, char * name, char * var, int left);


  virtual int get_num_limit_locals(typedecl * e);
  virtual void generate_limit_step1(int limit_strategy);
  virtual void generate_limit_step2(typedecl * d, char * var, typedecl * e, int local_num, int limit_strategy);
  virtual void generate_limit_step3(typedecl * d, char * var, typedecl * e, int limit_strategy, bool isunion);

  virtual void generate_limit3(char * var, typedecl * e);
  virtual void generate_limit3(typedecl * d, char * var, typedecl * e);
  virtual void generate_limit3(int size, char * name, char * var, int left);

  virtual void generate_while(charlist * scalarsetlist);
  virtual void generate_while_guard(charlist * scalarsetlist);

  virtual void generate_limit4(char * var, typedecl * e);
  virtual void generate_limit4(typedecl * d, char * var, typedecl * e);
  virtual void generate_limit4(int size, char * name, char * var, int left, bool isunion );

  virtual void generate_limit5(char * var, typedecl * e);
  virtual void generate_limit5(typedecl * d, char * var, typedecl * e);
  virtual void generate_limit5(char * name1, int size1, int left1,
			       char * var,
			       char * name2, int size2, int left2, bool isunion);

  virtual void generate_range(int size, char * name);
  virtual void generate_canonicalize();
  virtual void generate_canonicalize(int size,
 				     char * name, int left, int scalarsetleft);
  virtual void generate_slow_canonicalize(int size,
 				     char * name, int left, int scalarsetleft);
  virtual void generate_fast_canonicalize(int size,
 				     char * name, int left, int scalarsetleft);

public:
  // initializer
  arraytypedecl(bool interleaved,
		typedecl *indextype,
		typedecl *elementtype);
  
  // supporting routines
  typedecl *getindextype() const { return indextype; };
  typedecl *getelementtype() const { return elementtype; };
  virtual bool issimple() const { return FALSE; };
  virtual typeclass gettypeclass() const { return Array; };
  virtual int getsize() const 
  { Error.Error ("Internal: arraytypedecl::getsize()"); return 0; };
  virtual int getleft() const
  { Error.Error ("Internal: arraytypedecl::getleft()"); return 0; };
  virtual int getright() const
  { Error.Error ("Internal: arraytypedecl::getright()"); return 0; };
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
  virtual void generate_assign();

  // classify scalarset types as useful or useless
  virtual void setusefulness() 
  { indextype->setusefulness(); elementtype->setusefulness(); }

  // classify variablt type
  virtual bool HasConstant() const 
  { 
    if (elementtype->HasConstant()
	&& indextype->getstructure() != typedecl::ScalarsetVariable)
      return TRUE;
    return FALSE;
  }
  virtual bool HasScalarsetVariable() const 
  {
    if (elementtype->HasScalarsetVariable()
	&& indextype->getstructure() != typedecl::ScalarsetVariable)
      return TRUE;
    else
      return FALSE;
  }
  virtual bool HasScalarsetArrayOfFree() const
  {
    if (elementtype->HasConstant()
	&& indextype->getstructure() == typedecl::ScalarsetVariable)
      return TRUE;
    else if (elementtype->HasScalarsetArrayOfFree()
	     && indextype->getstructure() != typedecl::ScalarsetVariable)
      return TRUE;
    else
      return FALSE;
  }
  virtual bool HasScalarsetArrayOfS() const
  {
    if (elementtype->HasScalarsetVariable()
	&& indextype->getstructure() == typedecl::ScalarsetVariable)
      return TRUE;
    else if (elementtype->HasScalarsetArrayOfS()
	     && indextype->getstructure() != typedecl::ScalarsetVariable)
      return TRUE;
    else
      return FALSE;
  }
  virtual bool HasScalarsetLeaf() const 
  {
    return elementtype->HasScalarsetLeaf();
  }
  virtual bool HasMultisetOfFree() const 
  {
    if (elementtype->HasMultisetOfFree()
	&& !indextype->getstructure() == typedecl::ScalarsetVariable)
      return TRUE;
    return FALSE;
  }
  virtual bool HasMultisetOfScalarset() const 
  {
    if (elementtype->HasMultisetOfScalarset()
	&& !indextype->getstructure() == typedecl::ScalarsetVariable)
      return TRUE;
    return FALSE;
  }

  // canonicalization code for symmetry
  virtual void generate_permute_function();
  virtual void generate_simple_canon_function(symmetryclass& symmetry);
  virtual void generate_canonicalize_function(symmetryclass& symmetry);
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  virtual void generate_limit_function(symmetryclass& symmetry);
  virtual void generate_multisetlimit_function(symmetryclass& symmetry);
  virtual charlist * generate_scalarset_list(charlist * sl);
};

/********************
  class multisettypedecl
  ********************/
class multisetcount;

class multisetcountlist
{
  multisetcount *mscount;
  multisetcountlist *next;
public:
  multisetcountlist(multisetcount *mscount, multisetcountlist *next)
  : mscount(mscount), next(next) {};
  void generate_decl(multisettypedecl * mset);
  void generate_procedure();
};

class multisetremovestmt;

class multisetremovelist
{
  multisetremovestmt *msremove;
  multisetremovelist *next;
public:
  multisetremovelist(multisetremovestmt *msremove, multisetremovelist *next)
  : msremove(msremove), next(next) {};
  void generate_decl(multisettypedecl * mset);
  void generate_procedure();
};

class multisettypedecl: public typedecl
{
  int maximum_size;
  typedecl *elementtype;
  bool interleaved;      // for EVER use -- BDD 
  multisetcountlist * msclist;  
  multisetremovelist * msrlist;  

  void generate_multiset_simple_sort();
  void generate_multiset_while(charlist * scalarsetlist);

  int get_num_limit_locals(typedecl * e);   // added by Uli
  void generate_limit_step1(int limit_strategy);
  void generate_limit_step2(char * var, typedecl * e, int local_num, int limit_strategy);
  void generate_limit_step3(char * var, typedecl * e, int limit_strategy, bool isunion);
  void generate_multiset_limit5(char *var, typedecl * e);
  void generate_multiset_limit5(char *var, char * name2, int size2, int left2, bool isunion);

  void generate_multiset_while_guard(charlist * scalarsetlist);

public:
  // initializer
  multisettypedecl(bool interleaved,
		expr * e,
		typedecl *elementtype);

  // add multisetcount
  void addcount(multisetcount * mscount) 
  { msclist = new multisetcountlist(mscount, msclist); }
  
  // add multisetremove
  void addremove(multisetremovestmt * msremove) 
  { msrlist = new multisetremovelist(msremove, msrlist); }
  
  // supporting routines
  typedecl *getindextype() const 
  { Error.Error ("Internal: multisettypedecl::getindextype()"); return 0; };
  int getindexsize() const { return maximum_size; };
  typedecl *getelementtype() const { return elementtype; };
  virtual bool issimple() const { return FALSE; };
  virtual typeclass gettypeclass() const { return MultiSet; };

  virtual int getsize() const 
  { Error.Error ("Internal: multisettypedecl::getsize()"); return 0; };
  virtual int getleft() const
  { Error.Error ("Internal: multisettypedecl::getleft()"); return 0; };
  virtual int getright() const
  { Error.Error ("Internal: multisettypedecl::getright()"); return 0; };
  
  // classify variable type
  virtual bool HasMultisetOfFree() const
  {
    if (elementtype->HasConstant())
      return TRUE;
    return FALSE;
  }
  virtual bool HasMultisetOfScalarset() const
  {
    if (elementtype->HasScalarsetVariable())
      return TRUE;
    return FALSE;
  }

  virtual bool HasConstant() const 
  { 
    return FALSE;
  }
  virtual bool HasScalarsetVariable() const 
  {
    return FALSE;
  }
  virtual bool HasScalarsetArrayOfFree() const
  {
    return FALSE;
  }
  virtual bool HasScalarsetArrayOfS() const
  {
    return FALSE;
  }
  virtual bool HasScalarsetLeaf() const 
  {
    return elementtype->HasScalarsetLeaf();
  }

  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
  virtual void generate_assign();

  // classify scalarset types as useful or useless
  virtual void setusefulness() 
  { elementtype->setusefulness(); }

  // canonicalization code for symmetry
  virtual void generate_permute_function();
  virtual void generate_simple_canon_function(symmetryclass& symmetry);
  virtual void generate_canonicalize_function(symmetryclass& symmetry);
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  virtual void generate_limit_function(symmetryclass& symmetry);
  virtual void generate_multisetlimit_function(symmetryclass& symmetry);
  virtual charlist * generate_scalarset_list(charlist * sl);
};

class multisetidtypedecl: public typedecl
{
  int left, right;   /* least value, greatest. */
  designator * parent; /* parent type. */

public:
  // initializer
  multisetidtypedecl(designator * parent)
  : typedecl(), parent(parent) 
  { left = 0; right = ((multisettypedecl *) parent->gettype())->getindexsize()-1; }
  
  // supporting routines
  virtual int getsize() const { return right - left + 1; };
  virtual int getleft() const { return left; };
  virtual int getright() const { return right; };
  virtual typedecl* getparenttype() const { return parent->gettype(); };
  virtual char * getparentname() const { return parent->generate_code(); };
  virtual typeclass gettypeclass() const { return MultiSetID; };
  virtual bool issimple() const { return TRUE; }
  
  // code generation
  virtual char *generate_decl();

  // classify scalarset types as useful or useless
  virtual void setusefulness() {}

  // classify variable type
  virtual bool HasConstant() const { return TRUE; }
  virtual bool HasScalarsetVariable() const { return FALSE; }
  virtual bool HasScalarsetArrayOfFree() const { return FALSE; }
  virtual bool HasScalarsetArrayOfS() const { return FALSE; }
  virtual bool HasScalarsetLeaf() const { return FALSE; }

  // canonicalization code for symmetry
  virtual void generate_permute_function(){};
  virtual void generate_simple_canon_function(symmetryclass& symmetry){};
  virtual void generate_canonicalize_function(symmetryclass& symmetry){};
  virtual void generate_simple_limit_function(symmetryclass& symmetry){};
  virtual void generate_array_limit_function(symmetryclass& symmetry){};
  virtual void generate_limit_function(symmetryclass& symmetry){};
  virtual void generate_multisetlimit_function(symmetryclass& symmetry){};
  virtual charlist * generate_scalarset_list(charlist * sl){ return NULL;};
};


/********************
  class recordtypedecl
  ********************/
class recordtypedecl: public typedecl
{
  ste *fields;       /* list of component fields. */
  bool interleaved;  /* for ever use -- BDD */
  
public:
  // initializer
  recordtypedecl(bool interleaved, ste *fields);
  
  // supporting routines
  virtual typeclass gettypeclass() const { return Record; };
  virtual bool issimple() const { return FALSE; };
  ste *getfields() const { return fields; };
  virtual int getsize() const
  { Error.Error ("Internal: arraytypedecl::getsize()"); return 0; };
  virtual int getleft() const
  { Error.Error ("Internal: recordtypedecl::getleft()"); return 0;};
  virtual int getright() const
  { Error.Error ("Internal: recordtypedecl::getright()"); return 1;};
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
  virtual void generate_assign();

  // classify scalarset types as useful or useless
  virtual void setusefulness() 
  { 
    for (ste *field = fields; field != NULL; field = field->next)
      field->getvalue()->gettype()->setusefulness();
  }

  // classify variablt type
  virtual bool HasConstant() const 
  {
    bool ret = FALSE;
    for (ste *field = fields; field != NULL; field = field->next)
      if (field->getvalue()->gettype()->HasConstant())
	ret = TRUE;
    return ret;
  }
  virtual bool HasScalarsetVariable() const 
  {
    bool ret = FALSE;
    for (ste *field = fields; field != NULL; field = field->next)
      if (field->getvalue()->gettype()->HasScalarsetVariable())
	ret = TRUE;
    return ret;
  }
  virtual bool HasScalarsetArrayOfFree() const 
  {
    bool ret = FALSE;
    for (ste *field = fields; field != NULL; field = field->next)
      if (field->getvalue()->gettype()->HasScalarsetArrayOfFree())
	ret = TRUE;
    return ret;
  }
  virtual bool HasScalarsetArrayOfS() const 
  {
    bool ret = FALSE;
    for (ste *field = fields; field != NULL; field = field->next)
      if (field->getvalue()->gettype()->HasScalarsetArrayOfS())
	ret = TRUE;
    return ret;
  }
  virtual bool HasScalarsetLeaf() const 
  {
    for (ste *field = fields; field != NULL; field = field->next)
      if (field->getvalue()->gettype()->HasScalarsetLeaf())
	return TRUE;
    return FALSE;
  }
  virtual bool HasMultisetOfFree() const 
  {
    for (ste *field = fields; field != NULL; field = field->next)
      if (field->getvalue()->gettype()->HasMultisetOfFree())
	return TRUE;
    return FALSE;
  }
  virtual bool HasMultisetOfScalarset() const 
  {
    for (ste *field = fields; field != NULL; field = field->next)
      if (field->getvalue()->gettype()->HasMultisetOfScalarset())
	return TRUE;
    return FALSE;
  }

  // canonicalization code for symmetry
  virtual void generate_permute_function();
  virtual void generate_simple_canon_function(symmetryclass& symmetry);
  virtual void generate_canonicalize_function(symmetryclass& symmetry);
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  virtual void generate_limit_function(symmetryclass& symmetry);
  virtual void generate_multisetlimit_function(symmetryclass& symmetry);
  virtual charlist * generate_scalarset_list(charlist * sl);
};

/********************
  class scalarsettypedecl
  -- set of permutable ids 
  --
  -- in order to allow easy implementation
  -- of disjoint union of scalarset/enum, the range
  -- of integer for each scalarset/enum is disjoint.
  ********************/
class scalarsettypedecl: public typedecl 
{
  bool named;  // true if it is explicitly given name
  lexid * lexname; // declared name of the scalarset
  int left, right; // lb and ub of the representing integer.
  ste * idvalues;  // ids and the corresponding values
  bool useless; // whether we should wast time in canonicalizing w.r.t. it.

public:
  // initializer
  scalarsettypedecl(expr *left, int lb);
  
  // supporting routines
  void setname( lexid * n ) { named = TRUE; lexname = n;  };
  bool isnamed() const { return named; };
  lexid * getlexname() const { return lexname;  };
  virtual typeclass gettypeclass() const { return Scalarset; };
  virtual bool issimple() const { return TRUE; }
  virtual int getsize() const { return right - left +1; }
  virtual int getleft() const { return left; }
  virtual int getright() const { return right;}
  ste * getidvalues() { return idvalues; };
  void setidvalues( ste * vs ) { idvalues = vs; };
  void setupid(lexid *n);
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();

  // classify scalarset types as useful or useless
  virtual void setusefulness() { if (named) useless = FALSE; }
  virtual bool isuseless() { return useless; }

  // classify variable type
  // scalarset of size 1 is regarded as constant.
  virtual bool HasConstant() const { if (left==right) return TRUE; else return FALSE; }
  virtual bool HasScalarsetVariable() const { if (left==right) return FALSE; else return TRUE; }
  virtual bool HasScalarsetArrayOfFree() const { return FALSE; }
  virtual bool HasScalarsetArrayOfS() const { return FALSE; }
  virtual bool HasScalarsetLeaf() const { return TRUE; }
  virtual bool HasMultisetOfFree() const  { return FALSE; }
  virtual bool HasMultisetOfScalarset() const  { return FALSE; }

  // canonicalization code for symmetry
  virtual void generate_permute_function();
  virtual void generate_simple_canon_function(symmetryclass& symmetry);
  virtual void generate_canonicalize_function(symmetryclass& symmetry);
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  virtual void generate_limit_function(symmetryclass& symmetry);
  virtual void generate_multisetlimit_function(symmetryclass& symmetry);
  virtual charlist * generate_scalarset_list(charlist * sl);
};

/********************
  class uniontypedecl
  union of sets of permutable ids and/or enum
  ********************/
class uniontypedecl: public typedecl
{
  int size;              // totol size of the union
  stelist *unionmembers;  // list of scalarset member
  bool useless; // whether we should wast time in canonicalizing w.r.t. it.
  bool unionwithscalarset;

public:
  // initializer
  uniontypedecl(stelist *unionmembers);
  
  // supporting routines
  virtual typeclass gettypeclass() const { return Union; };
  virtual bool issimple() const { return TRUE; }
  virtual int getsize() const { return size; }
  virtual int getleft() const
  { Error.Error("Getting lower bound from union.\n"); return 0; };
  virtual int getright() const 
  { Error.Error("Getting upper bound from union.\n"); return 0; };
  virtual stelist * getunionmembers() const { return unionmembers;}
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();

  // classify scalarset types as useful or useless
  virtual void setusefulness() 
  {
    useless = FALSE;
    for (stelist *member = unionmembers; member != NULL; member = member->next)
      if (((typedecl *)member->s->getvalue())->gettypeclass() == typedecl::Scalarset)
	((scalarsettypedecl *) member->s->getvalue())->setusefulness();
  }
  virtual bool isuseless() { return useless; }
  virtual bool IsUnionWithScalarset() { return unionwithscalarset; } 

  // classify variable type
  virtual bool HasConstant() const { return FALSE; }
  virtual bool HasScalarsetVariable() const { return unionwithscalarset; }
  virtual bool HasScalarsetArrayOfFree() const { return FALSE; }
  virtual bool HasScalarsetArrayOfS() const { return FALSE; }
  virtual bool HasScalarsetLeaf() const { return unionwithscalarset; }
  virtual bool HasMultisetOfFree() const  { return FALSE; }
  virtual bool HasMultisetOfScalarset() const  { return FALSE; }

  // canonicalization code for symmetry
  virtual void generate_permute_function();
  virtual void generate_simple_canon_function(symmetryclass& symmetry);
  virtual void generate_canonicalize_function(symmetryclass& symmetry);
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  virtual void generate_limit_function(symmetryclass& symmetry);
  virtual void generate_multisetlimit_function(symmetryclass& symmetry);
  virtual charlist * generate_scalarset_list(charlist * sl);
};


/********************
  class errortypedecl
  ********************/
class errortypedecl: public typedecl
{
public:
  errortypedecl(char *name) : typedecl(name) {} ;

  virtual typeclass gettypeclass() const { return Error_type; };
  virtual bool issimple() const { return TRUE; /* don\'t report an error. */ };
};

extern errortypedecl * errortype; // For things that have gone wrong.

/********************
  class constdecl
  ********************/
class constdecl: public decl
{
  /* if it\'s a const, it must be a simple type, so the value can be shown
   * as an int. */
  int value;
  typedecl *type;
  
public:
  // initializer
  constdecl(int value, typedecl * type);
  constdecl(expr * e);
  
  // supporting routines
  virtual decl_class getclass() const { return Const; };
  virtual typedecl *gettype() const { return type; }
  virtual int getvalue() const { return value; }
  virtual designator *getdesignator(ste * origin) const
  { return new designator(origin, type, FALSE, TRUE, FALSE, value); }
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};

/********************
  class vardecl
  ********************/
class vardecl: public decl
{
  int offset; /* in bits. */
  
protected:
  typedecl * type;
  
public:
  // initializer
  vardecl(typedecl * type);
  
  // supporting routines
  virtual decl_class getclass() const { return Var; }
  virtual typedecl *gettype() const { return type; }
  virtual int getoffset() const { return offset; };
  virtual designator *getdesignator (ste *origin) const
  { return new designator(origin, type, TRUE, FALSE, TRUE); }
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};


/********************
  class aliasdecl
  a declaration for an alias.
  ********************/
class aliasdecl: public decl
{
  expr * ref;
  
public:
  // initializer
  aliasdecl(expr *ref);
  
  // supporting routines
  virtual decl_class getclass(void) const { return Alias; };
  virtual typedecl *gettype() const { return ref->gettype(); }
  virtual designator *getdesignator (ste *origin) const
  { return new designator(origin,
			  ref->gettype(),
			  ref->islvalue(),
			  ref->hasvalue(),
			  ref->checkundefined(),
			  ref->getvalue()); }
  virtual expr *getexpr() const { return ref; };
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};

/********************
  class choosedecl
  ********************/
class choosedecl: public decl
{
  //  friend ruleset; /* so a ruleset can check the type of quantifier. */
  
public:
  typedecl * type;
  
public:
  // initializer
  choosedecl(typedecl *type);
  
  // supporting routines  
  virtual decl_class getclass() const { return Choose; };
  virtual typedecl *gettype() const { return type; }
  virtual designator *getdesignator( ste *origin ) const
  { return new designator(origin, type, FALSE, FALSE, FALSE);
  };
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};

/********************
  class quantdecl
  a quantified variable, for a ruleset or FORALL.
  ********************/
class quantdecl: public decl
{
  //  friend ruleset; /* so a ruleset can check the type of quantifier. */
  
public:
  typedecl * type;
  expr * left, * right;
  int by;
  
public:
  // initializer
  quantdecl(typedecl *type);
  quantdecl(expr *lb, expr *ub, int by);
  
  // supporting routines  
  virtual decl_class getclass() const { return Quant; };
  virtual typedecl *gettype() const { return type; }
  virtual designator *getdesignator( ste *origin ) const
  { return new designator(origin, type, FALSE, FALSE, FALSE);
  };
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
  void make_for();  // make the for statement for the 
  //evaluation of the quantifier. in cpp_code.C.
  
};

/********************
  class param
  parameter to a procedure or function.
  ********************/
class param: public vardecl
{
public:
  // class identifier
  enum param_class { Value, Var, Const, Name, Error_param };
  
  // initializer
  param(typedecl *type);
  
  // supporting routines
  virtual decl_class getclass() const { return Param; };
  virtual param_class getparamclass( void ) const { return Error_param; };
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};

/********************
  class valparam
  ********************/
class valparam: public param
{
public:
  valparam(typedecl *type);
  virtual param_class getparamclass( void ) const { return Value; };
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};

/********************
  class varparam
  ********************/
class varparam: public param
{
public:
  varparam(typedecl *type);
  virtual param_class getparamclass( void ) const { return Var; };
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};

/********************
  class constparam
  ********************/
class constparam: public param
{
public:
  constparam(typedecl *type);
  virtual param_class getparamclass( void ) const { return Const; };
  virtual designator *getdesignator( ste *origin ) const
  { return new designator(origin, type, FALSE, FALSE, TRUE);
  };
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};


/********************
  class procdecl
  -- a definition of a procedure. 
  ********************/
struct procdecl: public decl
{
  // variables
  ste *params;
  ste *decls;
  stmt *body;
  
  // initializer
  procdecl(ste *params, ste *decls, stmt *body);
  procdecl();
  
  // supportine routines
  virtual decl_class getclass() const { return Proc; };
  virtual typedecl * gettype() const { return voidtype; };
  virtual ste * getparams() const { return params; };
  virtual stmt * getbody() const { return body; };
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};


/********************
  class funcdecl
  -- a function definition.
  ********************/
struct funcdecl: public procdecl
{
  // variables
  typedecl *returntype;
  bool sideeffects;
  
  // initializer
  funcdecl(ste *params,
	   ste *decls,
	   stmt *body,
	   typedecl *returntype,
	   bool sideeffects);
  funcdecl();
  
  // supporting routines
  virtual decl_class getclass(void) const { return Func; };
  virtual typedecl *gettype(void) const { return returntype; };
  virtual bool has_side_effects(void) const { return sideeffects; };
  
  // code generation
#ifdef GENERATE_DECL_CODE
  // used in no_code.C
  virtual char *generate_code();
#endif
  virtual char *generate_decl();
};


/********************
  class error_decl
  ********************/
class error_decl: public decl
{
protected:
public:
  error_decl(char *name) : decl(name) {};
  virtual decl_class getclass() const { return Error_decl; }; 
  virtual typedecl *gettype(void) const; /* every type of decl
					    has a type defined. */
  virtual designator *getdesignator(ste *origin) const;
};

/********************
  external variables
  ********************/
extern typedecl * booltype; /* boolean type. */
extern typedecl * inttype;  /* not used directly; parent type
			       of integer subranges. */
extern error_decl * errordecl;
extern param * errorparam;

/****************************************
  * 18 Nov 93 Norris Ip: 
  added scalarsetlist to typedecl
  added scalarset structure class to typedecl
  * 7 Dec 93 Norris Ip: 
  added generate_permute_function() to every typedecl
  *** completed the old simple/fast exhaustive canonicalization
  *** for one scalarset
  * 8 Dec 93 Norris Ip: 
  added generate_canonicalize_function() to every typedecl
  * 14 Dec 93 Norris Ip:
  added bool useless to scalarsettypedecl so that
  we can restrict the canonicalization to useful scalarset
  * 20 Dec 93 Norris Ip: 
  added generate_limit_function() to every typedecl
  * 26 Jan 94 Norris Ip:
  add getsize()
  changed all right-left+1 to getsize()
  * 31 jan 94 Norris Ip:
  add setusefulness() to typedecls
  * 28 Feb 94 Norris Ip:
  add HasScalarsetVariable() and HasScalarsetArrayOfFree() to typedecls
  * 28 Feb 94 Norris Ip: 
  added generate_simple_canon_function() to every typedecl
  * 1 March 94 Norris Ip: 
  added
  virtual void generate_simple_limit_function(symmetryclass& symmetry);
  virtual void generate_array_limit_function(symmetryclass& symmetry);
  * 3 March 93 Norris Ip:
  add HasScalarsetArrayOfS() to typedecls
  * 7 March 93 Norris Ip:
  add HasConstant() to typedecls
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 5 April 94 Norris Ip:
  added HasScalarsetLeaf() to forbid clearing a scalarset
  ******
  * Subject: Extension for {multiset, undefined value, general union}
  * 25 May 94 Norris Ip:
  designator:: added maybeundefined field
  change scalarsettypedecl initializer
  rename all scalarsetunion to union
  * 7 OCT 94 Norris Ip:
  added multisettypedecl
  * 11 OCT 94 Norris Ip:
  added multisetidtypedecl
  changed to use different semantic
beta2.9S released
  * 14 July 95 Norris Ip:
  Tidying/removing old algorithm
  adding small memory algorithm for larger scalarsets
****************************************/

/********************
 $Log: decl.h,v $
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
