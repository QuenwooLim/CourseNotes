/* -*- C++ -*-
 * mu.h
 * @(#) General declarations for the Murphi compiler.
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
 * Subject: liveness extension
 * Last modified: Novemeber  5, 93
 *
 * C. Norris Ip 
 * Subject: symmetry extension
 * First version: Jan 14 93 - June 2 93
 * Transfer to Liveness Version:
 * First modified: November 8, 93
 *
 * Denis Leroy
 * Subject: 'no compression' option
 * Last modified: May 13, 95
 */ 

/********************
  general constants
  ********************/
#define MURPHI_VERSION "Murphi Release 3.1"
#define LAST_DATE "Jan 29 1999"

#ifndef __DATE__  // To accomodate DEC's CC
#define __DATE__ "On or after "LAST_DATE
#endif

/*  Define this macro for pre 2.x G++ versions.
    It controls whether delete has a size argument,
    which is required by old g++ and generates a
    warning in new g++ compilers */

/* #define OLD_GPP(arg) arg */
/* #define OLD_GPP(arg) : we don't need this : spark */

#define MAX_NO_BITS 1024   // maximium number of bits in a single state
#define MAX_NO_RULES 1024  // maximium number of rules in the description
#define BUFFER_SIZE 1024   // maximium size of temporary buffer in byte
#define MAX_SCALARSET_PERM 600000 // maximium number of permutations for explicit storage


#ifndef _mu_h
#define _mu_h

/* #define this if you need different types of code generation
 * for different types of decls in a backend. If you don\'t know
 * what it does, don\'t do it. */
#undef GENERATE_DECL_CODE
#undef TEST

/********************
  Include file headers I
  ********************/
#include <stdio.h>
#include <stdlib.h>
// #include <strings.h>   // Uli: not available on all systems
#include <string.h>
#include <stdarg.h>
#include "y.tab.h"

/********************
  boolean constants
  ********************/
//typedef char bool; /* just for my sake. */
#define TRUE 1
#define FALSE 0

/********************
  class TNode
  ********************/
class TNode
{
 public:
  TNode() { };
  virtual char *generate_code(void) { return NULL; };
//  virtual ~TNode() {};
};

/********************
  class declarations
  ********************/
class decl;
class typedecl;
class expr;
class designator;
class stmt;
class rule;

// liveness extension
enum  live_type { E, AE, EA, U, AIE, AIU };

/********************
  extern variables
  ********************/
extern TNode error_obj; /* created for error handling. */

/********************
  class Error_handler 
  ********************/
class Error_handler /* error-reporting mechanism. */
{
  int numerrors;
  int numwarnings;

 private:
  void vError(const char *fmt, va_list argp);
  void vWarning(const char *fmt, va_list argp);

 public:
  // initializer
  Error_handler(void);

  // These next five each take printf-like arguments. 
  void Error(const char* fmt, ...);  
          // print out message and record an error.
  void FatalError(const char* fmt, ...); 
          // print out message and quit.
  bool CondError(const bool test, const char* fmt, ...);
          // if test is true, print strings and record error. Return test. 
  void Warning(const char* fmt, ...); 
          // print a warning. 
  bool CondWarning(const bool test, const char* fmt, ...);
          // if test is true, print strings and record warning. Return test. 

  // supporting routines
  int getnumerrors(void) { return numerrors; };
  int getnumwarnings(void) { return numwarnings; };

};

/********************
  extern variable
  ********************/
extern Error_handler Error;

/********************
  class argclass 
  -- command line argument handling
  -- a registry of command-line options. 
  -- Argclass inspired by Andreas\' code.
  ********************/
class argclass
{
  int argc;
  char **argv;

public:
  // variables
  char *filename;	// name of the input file
  bool print_license;   // whether to print license 
  bool help;		// whether to print option
  bool no_compression;  // whether not to bitpack the state vector
  bool hash_compression; // whether not to hashcompact states in hash tbl
  const bool checking;	/* runtime checking? */
  bool remove_deadrule; // suppress code generation for dead rules
  int symmetry_algorithm_number;// symmetry algorithm number 

  // initializer
  argclass(int ac, char** av);

  // supporting routines
  char *NextFile();  // ??
  bool Flag(char *arg);  // ??
  void PrintInfo( void );
  void PrintOptions( void );
  void PrintLicense( void );
};

/********************
  extern variable
  ********************/
extern argclass *args;

/********************
  Utilities Declaration
  ********************/
int CeilLog2(int n); 
/* number of bits required to hold n values. */

char *tsprintf (const char *fmt, ...);
/* sprintf's the arguments into dynamically allocated memory.  Returns the
 * dynamically allocated string. */

int new_int();
/* returns a different integer each time. Used for creating distinct names.*/

/********************
  extern variable
  ********************/
extern char *gFileName;  /* file support. */
extern FILE *codefile;   /* name of the file currently being compiled. */

/********************
  Class lexid and lexlist
  -- entry in lex's name table / entry list
  ********************/
class lexid {
  char *name;
  int lextype;
 public:
  lexid(char *name, int lextype);
  char *getname() const { return name; };
  int getlextype() const { return lextype; };
};

/* a linked list of lexids. */
struct lexlist
{
  lexid *l;
  lexlist *next;
  lexlist(lexid *l, lexlist *next)
  :l(l), next(next) {}
};

/********************
  Symbol table stuff. 
  -- The symbol table needs to be a persistent structure; the processing
  -- stage needs to be able to find symbols to access names.  However,
  -- when we're in that stage, we'll have a pointer to each symbol
  -- we need, so finding symbols is not such a problem.
  -- The current implementation uses a hash table to find identifiers,
  -- with each bucket a linked list of symbol table entries ( ste's ).
  ********************/

#define MAXSCOPES 64 /* number of saved scopes. */
#define SYMTABSIZE 211 /* should be a prime. */

/********************
  class ste
  -- a symbol table entry. 
  --
  -- Note: since we use lexids instead of char *'s for names, we can
  -- use pointer comparison to check for equality, and if we decided to move
  -- to a different conception of type sensitivity, all we have to do
  -- is make sure that lextable::enter() maps words that are considered
  -- equivalent to the same lexid.
  ********************/
struct ste
{ 
  friend class symboltable;

  // basic data 
  lexid *name; /* name of object. */
  int scope; /* scope in which it's declared. */
  decl *value; /* thing associated with name. */

  // linkage in lists
  ste *next; /* previous thing declared. */

public:
  // basic interface
  ste(lexid *name, int scope, decl *value);
  void setname( lexid * name ) {this->name = name; };
  lexid *getname() const { return name; };
  int getscope() const { return scope; };
  decl *getvalue() { return value; };
  
  // list interface
  ste *getnext() const {return next; }; 
  void setnext(ste *next) { this->next = next; };
  ste *reverse();   /* reverse a list of ste\'s. */
  
  // search a list of ste's for one with lexid name.
  ste *search(lexid *name);
  
  // code generation
  // generate\'s declarations for all the things in a list of ste\'s,
  // in reverse order.
  char *generate_decls();
};

/********************
  class symboltable
  -- the symbol table, recording (name, scope, value) triples.
  ********************/
class symboltable
{ 
private:
  // variables
  static const int globalscope;
  int scopes[MAXSCOPES];          /* stack of numbers of currently active scopes. */
  ste *scopestack[MAXSCOPES];     /* stack of top element of active scopes. */
  int offsets[MAXSCOPES];         /* Saved variable offsets. */
  int curscope;                   /* integer number of scopes created. */
  int scopedepth;                 /* index of highest scope in use. */
  ste *find(lexid *name) const;   /* return the ste corresponding to name
				     in current table, or NULL if none. */

 public:
  // initializer
  symboltable(); 

  // supportine routines
  int getscopedepth() const { return scopedepth; };
  ste *lookup(lexid *name);
          /* find ste corresponding to name in current scope.*/
          /* if not there, report error and declare as error_decl. */
  ste *declare(lexid *name, decl *value);
          /* associate lexid with object value. If already declared in scope,
           * report error and do nothing. */
  ste *declare_global(lexid *name, decl *value);
          /* declare in global scope. */
  int pushscope();
          /* push a new scope and return its number. */
  ste *popscope(bool cut = TRUE);
          /* pop a scope, returning the list of ste's popped.
           * IF cut is true, clear the last link in the returned scope. */
  ste *getscope() const; 
          /* return to a pointer to the linked list of ste\'s
          * at the top of the stack. */
  ste *dupscope() const; 
          /* return a copy of the ste\'s in the top scope. */
  ste *topste() const {return scopestack[scopedepth]; };
  int topscope() const { return scopes[ scopedepth ]; };
  int getcurscope() const { return curscope; };
};

/********************
  extern variable
  ********************/
extern symboltable *symtab;

/********************
  ste list
  ********************/
struct stelist
{
  ste *s;
  stelist *next;
public:
  stelist(ste *s, stelist *next )
  :s(s), next(next) {}
};

/********************
  ste collection
  (based on ste list but support union)

  Vitaly hacked this
  ********************/

class stecoll
{
  stelist *first;
  stelist *last;
public:
  stecoll(ste *s) {
    first = last = new stelist (s, NULL); }
  stecoll() {
    first = last = NULL; }

  void append(stecoll *sc) {
    if(last) {
      // This collection is not empty
      if(sc->last) {
        // sc is not empty
        last->next = sc->first;
        last       = sc->last; } }
    else {
      // This collection is empty
      first = sc->first;
      last  = sc->last;
    }
    delete sc;
  }

  bool includes(ste *s) {
    stelist *p = first;
    while (p) {
      if (p->s == s) return TRUE;
      p = p->next;
    }
    return FALSE; 
  }

  void output() {
    stelist *p = first;
    while (p) {
      printf("%s ", p->s->getname()->getname());
      p = p->next;
    }
    printf("\n");
  }
};

/********************
  Include file headers II
  ********************/
#include "cpp_sym.h"
#include "lextable.h"
#include "expr.h"
#include "decl.h"
#include "stmt.h"
#include "rule.h"

/********************
  procedure declarations
  -- declared at parse.C
  ********************/
bool matchparams(char *name, ste *formals,exprlist *actuals);
bool type_equal(typedecl* a, typedecl *b);
        /* whether two types should be considered compatible. */

/********************
  rulerec for storing a list of summary for
  * rules
  * startstates
  * invariants
  * fairnesses
  ********************/
struct rulerec
{
  char *rulename;
  char *conditionname;
  char *bodyname;
  bool unfair;
  rulerec *next;
  rulerec(char * rulename,
	  char * conditionname,
	  char * bodyname,
	  bool unfair)
  : rulename(rulename), conditionname(conditionname), bodyname(bodyname),
    next(NULL), unfair(unfair) {};
  int print_rules(); /* print out a list of rulerecs. */
};

/********************
  liverec for storing a list of summary 
  for liveness properties
********************/
struct liverec
{
  char *rulename;
  char *preconditionname;
  char *leftconditionname;
  char *rightconditionname;
  live_type  livetype;
  liverec *next;
  liverec(char * rulename,
          char * preconditionname,
          char * leftconditionname,
          char * rightconditionname,
          live_type livetype)
  : rulename(rulename), preconditionname(preconditionname),
    livetype(livetype), leftconditionname(leftconditionname),
    rightconditionname(rightconditionname), next(NULL) {};
  int print_liverules();
};



/********************
  Class program
  -- for storing parsed tree
  ********************/
struct program :TNode
{
  int bits_in_world;
  ste *globals;
  ste *procedures;
  rule *rules;
  rulerec *rulelist;
  rulerec *startstatelist;
  rulerec *invariantlist;
  rulerec *fairnesslist;
  liverec *livenesslist;
  symmetryclass symmetry;

  program( void )
  : globals(NULL), procedures(NULL),
    rules(NULL), rulelist(NULL), startstatelist(NULL),
    invariantlist(NULL), fairnesslist(NULL), livenesslist(NULL)
  {}
  void make_state(int bits_in_world);
  virtual char *generate_code();
};

/********************
  extern variable
  ********************/
extern program *theprog;

/********************
  Declarations for parser and lexer
  ********************/
#define IDLEN 256           /* longest length of an identifier. */
extern int gLineNum;        /* current line number in file. */
extern FILE *yyin;          /* the file from which yylex() reads. */
int yylex( void );          /* the lexer, provided by 'flex mu.l' */
void yyerror(char *s);      /* stuff for yacc. yacc's yyerror. */
extern int offset;          /* offset for variables in current scope. */
extern int yyparse( void ); /* shouldn't this get declared in y.tab.h? */

/********************
  Yacc\'s YYSTYPE
  -- Yacc\'s YYSTYPE, done here instead of in a %union in yacc so that we
  -- don\'t get redefinitions when y.tab.c #include\'s this which #include\'s
  -- y_tab.h.
  ********************/
union YYSTYPE { /* sloppy, sloppy, sloppy. Bleah. */
  TNode *node;
  expr *expr_p;		// an expression.
  designator *desig_p;  // a designator (l-value).
  decl *decl_p;		// a declaration.
  typedecl *typedecl_p; // a type declaration.
  ste *ste_p;		// a record field list or formal parameter list.
  stmt *stmt_p;		// a statement or sequence of statements.
  rule *rule_p;		// a sequence of rules.
  ifstmt *elsifs;	// a sequence of elsif clauses.
  caselist *cases;	// a sequence of cases of a switch statement.
  alias *aliases;	// a sequence of alias bindings.
  int integer;		// an integer constant.
  bool boolean;		// a boolean value--used with INTERLEAVED.
  lexid *lex;		// a lexeme, most usefully an ID.
  lexlist *lexlist_p;	// a list of lexemes.
  char *string;		// a string value.
  exprlist *exprlist_p;	// a list of expressions.
};

typedef union YYSTYPE YYSTYPE;

extern YYSTYPE yylval;

#endif

/****************************************
  * Nov 93 Norris Ip: 
  added struct stelist
  added scalarsetlist to program class
  * 7 Dec 93 Norris Ip:
  changed 
    stelist *scalarsetlist;
  in program class to
    symmetryclass symmetry; 
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 8 April 94 Norris Ip:
  fixed ordering in aliasstmt decl
****************************************/

/********************
 $Log: mu.h,v $
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
