/* Do not use C++ mode in emacs, but it is still a C++ code
 * -*- C++ -*-
 *
 * This is yacc, but the actions are C++.  Even so, using C++-mode on
 * this code won\'t work, unless C++-mode has been fixed.
 */

/*
 * mu.y
 * @(#) The (Berkeley) yacc grammar for Murphi, with actions.
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
 * Subject: symmetry syntax
 * First version: April 6, 93 -- June 11, 93
 * Transfer to Liveness Version:
 * First modified: November 8, 93
 *
 * C. Norris Ip
 * Subject: Extension for {multiset, undefined value, general union}
 * First modified: May 24, 94
 *
 */ 

%{

/* include files */
#include "mu.h"

/* global variables */
int enumval = 1;               /* current value for enum. */
int shiftenum=0;
enumtypedecl *enumdecl = NULL; /* old enumeration decl. */
bool hasreturn = FALSE;        /* whether currently-defined function has
				  a return statement yet. */
int localscope = 0;            /* all scopes within localscope will have
				* a scope value greater than localscope. */
bool sideeffects = FALSE;
typedecl *returntype = NULL;
static ste *curfields = NULL;
extern typedecl *switchtype;
extern int offset;
lexlist *not_yet_declared = NULL;
int paramclass = 0;
bool hasrule = FALSE;
bool has_startstate = FALSE;

/* variables for union */
static stelist *unionmembers = NULL;

%}


%start prog

/* standard token */
%token          END PROGRAM PROCESS /* Do we use Program and Process? */
%token          PROCEDURE ENDPROCEDURE FUNCTION ENDFUNCTION
%token          RULE ENDRULE RULESET ENDRULESET ALIAS ENDALIAS
%token          IF THEN ELSIF ELSE ENDIF SWITCH CASE ENDSWITCH
%token		FOR FORALL EXISTS IN DO ENDFOR ENDFORALL ENDEXISTS 
%token		WHILE ENDWHILE RETURN /* These added by rlm. */
%token		TO bEGIN BY /* added by rlm. */
%token		OLDEND /* for all the old types of END*. */
%token          LONGARROW DOTDOT ASSIGN CLEAR ERROR ASSERT PUT
%token          CONST TYPE VAR
%token          ENUM /* BOOLEAN */ INTERLEAVED RECORD ARRAY OF ENDRECORD
%token          STARTSTATE ENDSTARTSTATE INVARIANT TRACEUNTIL
%token<string>  STRING
%token<lex>	ID
%token<integer> INTCONST

/* liveness token */
%token          FAIRNESS LIVENESS ALWAYS EVENTUALLY UNTIL FAILED UNFAIR
%token          LIVENESSSET ENDLIVENESSSET FAIRNESSSET ENDFAIRNESSSET

/* scalarset token */
%token          SCALARSET ISMEMBER 

/* "undefined" token */
%token          UNDEFINE ISUNDEFINED UNDEFINED

/* general union */
%token          UNION

/* multiset */
%token          MULTISET MULTISETREMOVE MULTISETREMOVEPRED MULTISETADD MULTISETCOUNT
%token          CHOOSE ENDCHOOSE

/* associativity */
%nonassoc	ASSIGN
%nonassoc	'?' ':'			/* C-style conditional. */
%nonassoc	IMPLIES			/* Boolean implication. */
%left		'|'			/* boolean disjunction. */
%left 		'&'			/* boolean conjunction. */
%left		NOT			/* Boolean negation. */
%nonassoc	'<' LEQ '=' NEQ GEQ '>' /* Comparisons. */
%left		'+' '-'			/* integer addition and subtraction. */
%left		'*' '/' '%'		/* integer mult, div, and mod. */
					/* mod added by rlm. */

/* standard type */
%type <typedecl_p> vardecltail formaltail fieldtail
%type <typedecl_p> typeid typeExpr enumtype recordtype subrangetype arraytype
%type <typedecl_p> multisettype
%type <ste_p>  enums /* optenums */ fields formals formal quantifier
%type <ste_p>  alias
%type <expr_p> expr optretexpr optCondition
%type <desig_p> designator
%type <exprlist_p> actuals exprlist
%type <stmt_p> stmt stmts optstmts ifstmt assignment whilestmt forstmt
%type <stmt_p> proccall clearstmt errorstmt assertstmt putstmt aliasstmt
%type <stmt_p> switchstmt returnstmt optElse optElses
%type <stmt_p> multisetaddstmt multisetremovestmt
%type <rule_p> rules rule simplerule aliasrule ruleset startstate invariant
%type <rule_p> choose onerule
%type <elsifs>  /* optElsifs */ elsif
%type <cases>  optCases case
%type <boolean> optInterleaved
%type <integer> optBy
%type <string> optString

/* Vitaly */
%type <integer> optPriority

/* type for liveness */
%type <rule_p> fairness liveness liveU liveE liveAE liveEA liveAIU liveAIE
%type <boolean> optFair

/* type for scalarset, union, undefine */
%type <typedecl_p> scalarsettype uniontype
%type <stmt_p> undefinestmt

%%
/*************************************************************
 * The structure of this file is loosely a depth-first preorder
 * traversal of the tree of the grammar.
 */

/* The whole thing. */
prog		: 
			{ symtab->pushscope(); }
		  decls 
			{ theprog->globals = symtab->getscope(); }
 		  procDecls
			{ theprog->procedures = symtab->getscope(); }
		  rules
			{
			  /* normally, the popscope() changes the offset
			   * so we have to save and restore the offset we want. */
			  theprog->bits_in_world = offset;

			  theprog->rules = $6;
			  Error.CondError( !hasrule,
					  "Program has no rules.");
			  Error.CondError( !has_startstate,
					  "Program has no startstates.");

			  symtab->popscope();
			  Error.CondError( symtab->topscope() != 0,
					  "Internal: Didn't pop enough scopes.");
			}
		;


/* general strongly-typed declarations. */
decls		: decls decl
                | /* empty */
		;


/* A single declaration. */
decl		: CONST constDecls
		| TYPE typeDecls
		| VAR varDecls
		;


/* constant declarations. */
constDecls	: constDecls constDecl semi
		| /* empty */
		;


/* an optional semicolon. */
/* BUG:  please decide whether to have empty or not */
semi		: ';'
/*		| /* empty */
		;


/* a single constant declaration. */
constDecl	: ID ':' expr
			{
			  decl *tconst = new constdecl($3);
			  symtab->declare( $1, tconst );
			}
		;


/* type declarations. */
typeDecls	: typeDecls typeDecl semi
		| /* empty */
		;


/* a single type declaration. */
typeDecl	: ID ':' typeExpr
			{
			  // declare new type
			  ste * s = symtab->declare($1,$3);
			  // Should this be declare_global?

			  // scalarset extension
  			  // if the new type is a scalarset, 
			  // reset id strings and put it into scalarsetlist
			  if ($3->gettypeclass() == typedecl::Scalarset)
			    {	
			      // reset id strings
 			      ((scalarsettypedecl *) $3)->setupid($1);

			      // put into scalarsetlist
 			      if (((scalarsettypedecl *)$3)->getsize() > 1) 
				theprog->symmetry.add(s);
			    }
			}
		;


/* a type expression, using the default $$ = $1 action. */
typeExpr	: typeid /* An already defined type. */
		| enumtype
		| subrangetype
		| recordtype
		| arraytype
                | scalarsettype
                | uniontype
		| multisettype
		;


/* an already defined type. */
typeid		: ID
			{
			  decl *t = (typedecl *)symtab->lookup($1)->getvalue();
			  if ( Error.CondError( t->getclass() != decl::Type,
					 "%s is not a type name.",
					 $1->getname() ) )
			    $$ = errortype;
			  else $$ = (typedecl *) t;
			}
		;


/* an enumerated type. */
enumtype	: ENUM 
			{
			  enumdecl = new enumtypedecl( enumval, enumval);
			  shiftenum = enumval;
			}
		  '{' enums '}' /* should this be optenums? */
			{
			  enumdecl->setright( enumval - 1 );
			  enumdecl->setidvalues( symtab->getscope() );
			  $$ = enumdecl;
			}
		;


/* a possibly-empty comma-separated list. */
/* optenums	: enums
			{ $$ = $1; } */
/*		| /* empty */
/*			{ $$ = NULL; }
		;
*/


/* a comma-separated list of words. */
enums		: enums ',' ID
			{
			  $$ = symtab->declare($3,
					       new constdecl(enumval++, /* -
							     shiftenum+1, */
							     enumdecl));
			}
		| enums ID
			{
			  $$ = symtab->declare($2,
					       new constdecl(enumval++, /* -
							     shiftenum+1, */
							     enumdecl));
			  Error.Error("Murphi 2 requires elements of enums to be separated by commas.");
			}
		| ID
			{
			  $$ = symtab->declare($1,
					       new constdecl(enumval++, /* -
							     shiftenum+1, */
							     enumdecl));
			}
		;


/* a subrange of an enumerated type */
subrangetype	: expr DOTDOT expr /* must be constants. */
			{
			  $$ = new subrangetypedecl($1, $3);
			}
		;


/* a record type, like the records of every Wirth language. */
recordtype	: RECORD optInterleaved 
			{
			  /* we _can_ have record declarations within
			   * record declarations, so we _have_ to save
			   * and restore the list. */
			  $<ste_p>$ = curfields;
			  curfields = NULL;
			}
			  /* Yea, verily, _two_ null nonterminals--we need
			   * to save both the curfields and the offset. */
			{
			  $<integer>$ = offset;
			  offset = 0;
			}
		  fields endrecord
			{
			  $$ = new recordtypedecl($2,
						  curfields->reverse() );
			  curfields = $<ste_p>3;
			  offset = $<integer>4;
			}
		;


/* End of a record. */
endrecord	: ENDRECORD
		| END
		;


/* optional keyword INTERLEAVED. */
optInterleaved	: /* INTERLEAVED  
			{ $$ = TRUE; }		
		| */ /* empty */
			{ $$ = FALSE; }
		;


/* a list of fields, with ; as a separator. */
fields		: fields semi field
			{}
		| fields semi
		  /* Why doesn't this cause a shift-reduce conflict?*/
		  /* Oh, I see, it only considers the expansions that
		   * can go into a valid parse. I am filled with a new
		   * respect for LALR parsing. */
			{}
		| field
			{}
		;


/* a single field entry. */
field		: 
			{
			  $<lexlist_p>$ = not_yet_declared;
			  not_yet_declared = NULL;
			}
		  ID fieldtail
			{ 
			  for (lexlist *ll = new lexlist($2, not_yet_declared);
			       ll != NULL;
			       ll = ll->next)
			    { /* 'hand-declare' the field. */
			      ste *tmpste = new ste( ll->l, 0, new vardecl($3) );
			      tmpste->next = curfields;
			      curfields = tmpste;
			    }
			  not_yet_declared = $<lexlist_p>1;
			}
		;


/* end of a field--this is useful because it allows multiple fields
 * to be declared on a single line. */
fieldtail	: ',' ID fieldtail
			{
			  not_yet_declared = new lexlist( $2, not_yet_declared );
			  $$ = $3;
			}

		| ':' typeExpr /* the associated type. */
			{
			  $$ = $2;

 			  // scalarset warning
 			  if ( $2->gettypeclass() == typedecl::Scalarset && !( (scalarsettypedecl *) $2)->isnamed())
 			    Error.Warning("The anonymous scalarset type in the field of a record is useless.");
			}
		;


/* an array. */
arraytype	: ARRAY optInterleaved '[' typeExpr ']' OF typeExpr
		  /* $4 must be a simple type, not an array or record.*/
			{
			  $$ = new arraytypedecl($2,$4,$7);

			  // scalarset warning
			  if ( $4->gettypeclass() == typedecl::Scalarset && !( (scalarsettypedecl *) $4)->isnamed())
			    Error.Warning("The anonymous scalarset type in the index of an array is useless.");
			  if ( $7->gettypeclass() == typedecl::Scalarset && !( (scalarsettypedecl *) $7)->isnamed())
			    Error.Warning("The anonymous scalarset type as type of the elements of an array is useless.");
			}
		;

/* an multiset array. */
multisettype	: MULTISET optInterleaved '[' expr ']' OF typeExpr
		  /* $4 must be an integer, signifying the maximium size.*/
			{
			  $$ = new multisettypedecl($2,$4,$7);

			  // scalarset warning
			  if ( $7->gettypeclass() == typedecl::Scalarset && !( (scalarsettypedecl *) $7)->isnamed())
			    Error.Warning("The anonymous scalarset type as type of the elements of an array is useless.");
			}
		;

/* scalarset extension */
scalarsettype   : SCALARSET "(" expr ")" // a single scalarset
			{
			  $$ = new scalarsettypedecl($3, enumval);
			  enumval += $3->getvalue();
			}

/* union extension */
uniontype       : UNION "{" unionlist "}"  // a union of scalarsets and/or enum
			{
 			  $$ = new uniontypedecl(unionmembers);
			  unionmembers = NULL;                      
			}
                ;

unionlist       : unionlist "," unionlistelt
                | unionlistelt "," unionlistelt
                ; 

unionlistelt 
                : ID
			{
			  // lookup id in symtable and check whether it is a typedecl
			  ste *s = symtab->lookup($1);
			  Error.CondError( s->getvalue()->getclass() != decl::Type,
					 "%s is not a scalarset type name.",
					 $1->getname() );

			  // check whether it is a scalarsettypedecl
			  typedecl *t = (typedecl *) s->getvalue();
			  Error.CondError( ( t->gettypeclass() != typedecl::Scalarset
					     && t->gettypeclass() != typedecl::Enum ),
					 "%s is not a scalarset/enum type name.",
					 $1->getname() );

			  // check whether it has been used in this union before
			  for (stelist * elt = unionmembers; elt != NULL; elt=elt->next)
			    Error.CondError( ( elt->s->name != NULL && elt->s->name == $1 ),
					    "%s is used more than once in a union.",
					    $1->getname() );

			  // enter into list
			  unionmembers = new stelist(s, unionmembers);
			}
                | scalarsettype
			{
			  Error.Warning("The anonymous scalarset type in the union is useless.");
			}
                | enumtype
			{
			  ste * s = new ste(NULL,0,$1); 

			  // enter into list
			  unionmembers = new stelist(s, unionmembers);
			}
                ;



/* var declarations. */
varDecls	: varDecls varDecl semi
			{ ; }
		| /* empty */
			{ ; }
		;


/* a single var declaration. */
varDecl		: 
			{ 
			  $<lexlist_p>$ = not_yet_declared;
			  not_yet_declared = NULL;
			}
		  ID vardecltail
			{ 
			  for (lexlist *ll = new lexlist($2, not_yet_declared);
			       ll != NULL;
			       ll = ll->next)
			    symtab->declare( ll->l,
					    new vardecl($3) );
			  not_yet_declared = $<lexlist_p>1;
			}
		;


/* end of a vardecl--this is useful because it allows multiple vars
 * to be declared on a single line. */
vardecltail	: ',' ID vardecltail
			{
			  not_yet_declared = new lexlist($2, not_yet_declared);
			  $$ = $3;
			}
		| ':' typeExpr /* the associated type. */
			{
			  $$ = $2; 

			  // scalarset warning
			  if ( $2->gettypeclass() == typedecl::Scalarset && !( (scalarsettypedecl *) $2)->isnamed())
			    Error.Warning("The anonymous scalarset type in variable declaration is useless.");
			}
		;


/* a list of procedures. */
procDecls	: procDecls procDecl
		| procDecls funcDecl
		| /* empty */
		;


/* a single procdure. */
procDecl	: PROCEDURE ID 
			{
			  symtab->pushscope();
			}
		  '(' optformals ')' semi
			{
			  $<decl_p>$ = new procdecl();
			  ((procdecl *) $<decl_p>$)->params =
			    symtab->dupscope()->reverse();
			  symtab->pushscope();
			  returntype = voidtype;
			}
		    optdecls
			{
			  symtab->declare_global($2, $<decl_p>8);
			}
		    optstmts endprocedure semi
			{
			  ste *decls = symtab->popscope();
			  symtab->popscope(); /* Clear parameters. */
			  procdecl *proc = (procdecl *) $<decl_p>8;
			  proc->decls = decls;
			  proc->body = $11;
			}	      
		;


endprocedure	: ENDPROCEDURE
		| END
		;


/* a single function. */
funcDecl	: FUNCTION ID 
			{
			  symtab->pushscope();
			}
		  '(' optformals ')' 
			{
			  $<integer>$ = localscope;
			}
  		  ':' typeExpr semi
			{
			  funcdecl * func = new funcdecl;
			  $<decl_p>$ = func;
			  func->params = symtab->dupscope()->reverse();
			  if ($9->name == NULL)
			    symtab->declare_global(
						   ltable.enter(
								tsprintf("_type_%d",
									 new_int()
									 )
								),
						   $9 );
			  func->returntype = $9;
			  localscope = symtab->pushscope();

			  // scalarset warning
			  if ( $9->gettypeclass() == typedecl::Scalarset && !( (scalarsettypedecl *) $9)->isnamed())
			    Error.Warning("The anonymous scalarset type as return type of a function is useless.");
			}			  
		  optdecls
			{
			  symtab->declare_global($2, $<decl_p>11);
			  returntype = $9;
			  $<boolean>$ = hasreturn;
			  hasreturn = FALSE;
			  sideeffects = FALSE;
			}
		  optstmts endfunction semi /*decls and BEGIN added by rlm.*/
			{
			  ste *decls = symtab->popscope();
			  symtab->popscope(); /* remove parameters. */
			  funcdecl *func = (funcdecl *) $<decl_p>11;
			  func->decls = decls;
			  func->body = $14;
			  func->sideeffects = sideeffects;
			  Error.CondError( !hasreturn,
					  "function %s never returns a value.",
					  $2->getname() );
			  localscope = $<integer>7;
			  hasreturn = $<boolean>13;
			}
		;


endfunction	: ENDFUNCTION
		| END
		;


/* an optional set of formals */
optformals	: formals
			{}
		| /* empty */
		;


/* a nonempty list of formals */
formals		: formals semi formal
		| formals semi
		| formal
 		| formals ',' formal
			{
			  Error.Error("Murphi 2 requires formal parameters to be separated by semi-colons.");
			}
		;


/* a single formal. */
/* To add a type of parameter, you
 * must add a line that sets up paramclass correctly,
 * and add a case to the switch on paramclass in formaltail. */
formal		: VAR 
			{
			  paramclass = VAR;
			  $<lexlist_p>$ = not_yet_declared;
			  not_yet_declared = NULL;
			}
		  formalrest
			{ not_yet_declared = $<lexlist_p>2; }
		| 
			{
			  paramclass = CONST;
			  $<lexlist_p>$ = not_yet_declared;
			  not_yet_declared = NULL;
			}
		  formalrest
			{ not_yet_declared = $<lexlist_p>2; }
		;


formalrest	: ID formaltail
			{
			  for ( lexlist *ll = new lexlist($1, not_yet_declared);
			       ll != NULL;
			       ll = ll->next )
			    {
			      param *p = NULL;
			      switch ( paramclass ) {
			      case VAR:
				p = new varparam($2);
				break;
			      case CONST:
				p = new constparam($2);
				break;
			      default:
				p = errorparam;
				break;
			      }
			      symtab->declare(ll->l, p);
			    }
			}
		;


/* the end of a formal declaration. */
formaltail	: ',' ID formaltail
			{
			  not_yet_declared = new lexlist($2, not_yet_declared);
			  $$ = $3;
			}
		| ':' typeExpr
                        /* was a TypeExpr, but I had to require param\'s types to be defined before. */
			{
			  $$ = $2;

			  // scalarset warning
			  if ( $2->gettypeclass() == typedecl::Scalarset && !( (scalarsettypedecl *) $2)->isnamed())
			    Error.Warning("The anonymous scalarset type in formal is useless.");
			}
		;


/* an optional declaration section and BEGIN keyword. */
optdecls	: decls bEGIN
		| /* empty */
		;



/* expression syntax. */
/* a designator is an lvalue. */
designator	: ID
			{
			  ste *tmpste = symtab->lookup($1);
			  Error.CondError( ( tmpste->getvalue()->getclass() == decl::Type || 
					     tmpste->getvalue()->getclass() == decl::Proc ),
					   "Non Variable found when variable is expected.");
			  $$ = tmpste->getvalue()->getdesignator(tmpste);
			}
		| designator '[' expr ']'
		  /* array reference. */
			{ $$ = new designator ( $1, $3 ); };
		| designator '.' ID
		  /* record reference. */
			{ 
			  $$ = new designator ( $1, $3 );
			}
		;


/* general expression. */
expr		: expr '?' expr ':' expr
			{
			  $$ = new condexpr($1,$3,$5);
			}
  		| expr IMPLIES expr
			{
			  $$ = new boolexpr(IMPLIES, $1, $3); }
		| expr '|' expr 
			{
			  $$ = new boolexpr('|', $1, $3);
			}
		| expr '&' expr
			{ 
			  $$ = new boolexpr('&', $1, $3);
			}
		| NOT expr
			{
			  $$ = new notexpr($2);
			}
		| expr '<' expr
			{ 
			  $$ = new compexpr('<', $1, $3);
			}
		| expr LEQ expr
			{ 
			  $$ = new compexpr(LEQ, $1, $3);
			}
		| expr '>' expr
			{ 
			  $$ = new compexpr('>', $1, $3);
			}
		| expr GEQ expr
			{
			  $$ = new compexpr(GEQ, $1, $3);
			}
		| expr '=' expr
			{
			  $$ = new equalexpr('=', $1, $3);
			}
		| expr NEQ expr
			{ 
			  $$ = new equalexpr(NEQ, $1, $3);
			}
		| expr '+' expr
			{ 
			  $$ = new arithexpr('+', $1, $3);
			}
		| expr '-' expr
			{ 
			  $$ = new arithexpr('-', $1, $3);
			}
		| '+' expr	%prec '*' 
			{
			  $$ = new unexpr('+',$2);
			}
 		| '-' expr	%prec '*' 
			{
			  $$ = new unexpr('-',$2);
			}
		| expr '*' expr
			{ 
			  $$ = new mulexpr('*', $1, $3);
			}
		| expr '/' expr
			{ 
			  $$ = new mulexpr('/', $1, $3);
			}
		| expr '%' expr /* extension to V1.6. by rlm. */
			{
			  $$ = new mulexpr('%', $1, $3);
			}
		| INTCONST 
			{ 
			  $$ = new expr( $1, inttype );
			}
		| designator
			{ 
			  $$ = $1;
			} 
		| ID actuals /* a function call. */
			{
			  $$ = new funccall ( symtab->lookup($1), $2 );
			}
		| ISUNDEFINED '(' designator ')'	// scalarset extension
			{ 
			  $$ = new isundefined ( $3 );
			}
                | ISMEMBER '(' designator ',' typeExpr ')' // scalarset extension
			{ 
			  $$ = new ismember ( $3 , $5 );
			} 	
		| '(' expr ')'
			{ $$ = $2; }
		| FORALL
			{ symtab->pushscope(); }
		   quantifiers do expr endforall
			{
			  $$ = new quantexpr( FORALL, symtab->popscope(), $5 );
			}
		| EXISTS
			{ symtab->pushscope(); }
		  quantifiers do expr endexists
			{
			  $$ = new quantexpr( EXISTS, symtab->popscope(), $5 );
			}
		| error
			{
			  $$ = error_expr;
			  /* perform magic--I don't understand this well. */
			}
                | MULTISETCOUNT '(' 
			{ symtab->pushscope(); }
		  ID ':' designator ','
			{
			  Error.CondError( $6->gettype()->gettypeclass() != typedecl::MultiSet,
					   "Parameter for choose must be a multiset");
			  multisetidtypedecl *t = new multisetidtypedecl($6);
			  decl *tvar = new quantdecl(t);
			  symtab->declare( $4, tvar );
			} 	
		  expr ')'
                        {
			  $$ = new multisetcount( symtab->popscope(FALSE), $6, $9 );
			} 	
		;


endforall	: ENDFORALL
		| END
		;


endexists	: ENDEXISTS
		| END
		;


/* actual parameters for a function or procedure. */
actuals 	: '(' exprlist ')'
			{ $$ = $2->reverse(); }
		| '(' ')'
			{ $$ = NULL; }
		;


/* a nonempty list of expressions. */
/* generated in reverse order. */
exprlist	: exprlist ',' expr
			{ $$ = new exprlist($3, $1); }
		| expr
			{ $$ = new exprlist($1); }
		| exprlist ',' UNDEFINED
			{ $$ = new exprlist(TRUE, $1); }
		| UNDEFINED
			{ $$ = new exprlist(TRUE); }
		;


/* the two types of FOR construct, marked off by braces in the following
 * examples:
 * 1) FOR {i := 1 to N BY 2} DO ...
 * 2) FOR {i : ind_t } DO ...
 */
quantifier	: ID ':' typeExpr
			{
			  quantdecl *q = new quantdecl( $3 );
			  $$ = symtab->declare( $1, q );

			  // scalarset warning
			  if ( $3->gettypeclass() == typedecl::Scalarset && !( (scalarsettypedecl *) $3)->isnamed())
			    Error.Warning("The anonymous scalarset type in a quantifier is useless.");
			}
		| ID ASSIGN expr TO expr optBy
			{
			  quantdecl *q = new quantdecl( $3, $5, $6);
			  $$ = symtab->declare( $1, q);
			}
		;


/* a semi-colon separated list of quantifiers. Only used for rulesets. */
quantifiers	: quantifier ';' quantifiers
			{
			  ste *p = $1;
			  ste *q = p->getnext()->search( p->getname() );
			  Error.CondError( q != NULL &&
					  (q->getvalue()->getclass() == decl::Quant ||
					   q->getvalue()->getclass() == decl::Alias),
					   "Ruleset declaration shadows enclosing Ruleset or Alias.");
			}
		| quantifier
			{
			  ste *p = $1;
			  ste *q = p->getnext()->search( p->getname() );
			  Error.CondError( q != NULL &&
					  (q->getvalue()->getclass() == decl::Quant ||
					   q->getvalue()->getclass() == decl::Alias),
					   "Ruleset declaration shadows enclosing Ruleset or Alias.");
			}
		;


/* We've decided to try to let a colon substitute for the word 'do'. */
do		: DO
		| ':'
		;


/* an optional BY phrase. */
optBy		: BY expr
			{ 
			  if (!Error.CondError (!$2->hasvalue(),
						"BY value must be constant."))
			    {
			      $$ = $2->getvalue();
			      Error.CondError ( $$ == 0,
					       "BY value must not be 0.");
			    }
			}
		| /* empty */
			{ $$ = 1; }
		;



/* statement rules. */
/* an optional list of statements. */
optstmts	:
			{ $<stmt_p>$ = NULL; }
		stmts
			{ $$ = $2; }
		| /* empty */
			{ $$ = NULL; }
		;


/* a list of statements, with ; as a separator. */
stmts		: stmts semi stmt
			{ $<stmt_p>0->next = $3;
			  $<stmt_p>0 = $3;
			  $$ = $1;
			}
		| stmts semi /* allow extra semi-colons. */
			{ $$ = $1; }
		| stmt
		  	{
			  $<stmt_p>0 = $1;
			  $$ = $1;
			}
		;


/* a ;-separated statement. */
stmt		: assignment
		| ifstmt
		| whilestmt
		| switchstmt
		| forstmt
		| proccall
		| clearstmt
		| errorstmt
		| assertstmt
		| putstmt 	
		| aliasstmt 	
		| returnstmt 	
		| undefinestmt // scalarset extension
                | multisetaddstmt
                | multisetremovestmt
		;

/* an assignment statement. */
assignment	: designator ASSIGN expr
			{
			  Error.CondWarning($1->getbase() != NULL && 
					    $1->getbase()->getscope() < localscope,
					    "Assignment makes function have side effects.");
			  $$ = new assignment ($1, $3);
			}
                | designator ASSIGN UNDEFINED // "undefined" extension
			{
			  Error.CondWarning($1->getbase() != NULL &&
					    $1->getbase()->getscope() < localscope,
					    "Undefine makes function have side effects.");
			  $$ = new undefinestmt ( $1 );
			}
		;


/* if statement. */
ifstmt		: IF expr THEN
		    optstmts
		  optElses
		  endif
			{ $$ = new ifstmt ($2,$4, $5); }
		;


endif		: ENDIF
		| END
		;


/* elsif parts. */
optElses	: elsif optElses
			{
			  ((ifstmt *) $1)->elsecode = $2;
			  $$ = $1;
			}
		| optElse
			{ $$ = $1; }
		;


/* a single elsif. */
elsif		: ELSIF expr THEN
		    optstmts
			{ $$ = new ifstmt ($2, $4); }
		;


/* an optional else clause. */
optElse		: ELSE optstmts
			{ $$ = $2; }
		| /* empty */
			{ $$ = NULL; }
		;


/* while statment. */
whilestmt	: WHILE expr do
		    optstmts
		  endwhile
			{ $$ = new whilestmt ( $2, $4); }
		;


endwhile	: ENDWHILE
		| END
		;


/* switch statement. */
switchstmt 	: SWITCH expr
			{
			  $<typedecl_p>$ = switchtype;
			  switchtype = $2->gettype();
			  Error.CondError(!$2->gettype()->issimple(),
					  "Switch quantity must be of simple type.");
			}
			{ $<cases>$ = NULL; }
		    optCases
		    optElse
		  endswitch
			{
			  $$ = new switchstmt ( $2, $5, $6);
			  switchtype = $<typedecl_p>3;
			}
		;


endswitch	: ENDSWITCH
		| END
		;


/* an optional list of cases. */
optCases	: optCases case
			{
			  if ( $<cases>0 != NULL )
			    {
			      $<cases>0->next = $2;
			      $$ = $1;
			    }
			  else $$ = $2;
			  $<cases>0 = $2;
			}
		| /* empty */
			{ $$ = NULL; }
		;


/* a single case. */
case		: CASE exprlist ':' optstmts
			{ $$ = new caselist ( $2, $4); }
		;


/* for statement. */
forstmt		: FOR 
			{ symtab->pushscope(); }
		  quantifiers do
		    optstmts
		  endfor
			{
			  $$ = new forstmt (symtab->popscope(), $5);
			}
		;


endfor		: ENDFOR
		| END
		;


/* procedure call. */
proccall	:  ID actuals
			{
			  ste *proc = symtab->lookup($1);
			  $$ = new proccall ( proc, $2);
			}
		;


/* set all elements of basic type to their minima. */
clearstmt	: CLEAR designator
			{
			  Error.CondWarning($2->getbase() != NULL &&
					    $2->getbase()->getscope() < localscope,
					    "CLEAR makes function have side effects.");
			  $$ = new clearstmt ( $2 );
			}
		;

/* undefine statement */
undefinestmt	: UNDEFINE designator
			{
			  Error.CondWarning($2->getbase() != NULL &&
					    $2->getbase()->getscope() < localscope,
					    "Undefine makes function have side effects.");
			  $$ = new undefinestmt ( $2 );
			}
		;

/* multiset add statement */
multisetaddstmt : MULTISETADD '(' designator ',' designator ')'
			{
			  $$ = new multisetaddstmt ( $3, $5 );
			}
		;

/* multiset remove statement */
multisetremovestmt : MULTISETREMOVEPRED '(' 
			{ symtab->pushscope(); }
		  ID ':' designator 
			{
			  Error.CondError( $6->gettype()->gettypeclass() != typedecl::MultiSet,
					   "Parameter for choose must be a multiset");
			  multisetidtypedecl *t = new multisetidtypedecl($6);
			  decl *tvar = new quantdecl(t);
			  symtab->declare( $4, tvar );
			}
		  ',' expr ')'
                        {
			  $$ = new multisetremovestmt( symtab->popscope(FALSE), $6, $9 );
			} 	
		| MULTISETREMOVE '(' expr ',' designator ')'
		        {
			  $$ = new multisetremovestmt ($3,$5);
			}
		;

/* raise an error. */
errorstmt	: ERROR STRING
			{ $$ = new errorstmt ($2); }
		;


/* check an assertion, error if it\'s not so. */
assertstmt	: ASSERT expr optString
			{ $$ = new assertstmt ( $2, $3 ); }
		;


/* Print a message to output, I assume. */
putstmt		: PUT expr
			{ $$ = new putstmt( $2 ); }
		| PUT STRING
			{ $$ = new putstmt( $2 ); }
		;


/* simplify use of common expressions--sort of like a with statement. */
aliasstmt	: ALIAS
			{
			  symtab->pushscope();
			}
		  aliases do optstmts endalias
			{ $$ = new aliasstmt ( symtab->popscope(TRUE),
					      $5 ); }
		;


endalias	: ENDALIAS
		| END
		;


/* a set of aliases. */
aliases		: aliases semi alias
		| aliases semi /* I still don't see why this doesn't create conflicts. */
		  /* Oh, I see, it only considers the expansions that
		   * can go into a valid parse. I am filled with a new
		   * respect for LALR parsing. */
		| aliases ',' alias
			{
			  Error.Error("Murphi 2 separates aliases with semicolons.");
			}
		| alias
  			{}
		;


/* a single alias. */
alias		: ID ':' expr
			{
			  aliasdecl *adcl = new aliasdecl ( $3 );
			  $$ = symtab->declare ( $1, adcl );
			}
		;


/* exit from a procedure, function or rule and return a value for a function */
returnstmt	: RETURN optretexpr
			{ 
			  if ( returntype == voidtype )
			    {
			      Error.CondError($2 != NULL,
					      "Can't return a value from here.");
			    }
			  else
			    {
			      Error.CondError(!type_equal(returntype,
							  $2->gettype() ),
					      "Invalid type for return value.");
			      hasreturn = TRUE;
			    }
			  $$ = new returnstmt ( $2 );
			}
		;


/* an optional expression to be returned. */
optretexpr	: expr
			{ $$ = $1; }
		| /* empty */
			{ $$ = NULL; }
		;

/* Murphi-specific stuff. */
/* A list of rules to be applied nondeterministically. */
/* Okay, so it\'s right-recursive. Forgive me, please. */
rules		: rule semi rules
			{
			  $1->next = $3;
			  $$ = $1;
			}
		| rule semi
			{ $$ = $1; }
		| rule
			{ $$ = $1; }
		;

/* a single rule--in essence, a hunk of code to execute nondeterministically.*/
/* Now, it could be a startstate or an invariant, too. rlm. */
rule		: simplerule
		| aliasrule
		| ruleset
		| startstate
		| invariant
//		| fairness	// liveness extension	
//              | liveness 	// liveness extension
		| error
			{ /* perform magic--I don't understand this well. */ }
                | choose
		;


/* a basic rule. */
simplerule	: RULE optFair optPriority optString optCondition
			{
			  $<ste_p>$ = symtab->topste();
			  symtab->pushscope();
			  returntype = voidtype;
			}
		  optdecls optstmts endrule
			/*decls, BEGIN, and END added by rlm.*/
			{
			  stmt *stmts = $8 != NULL ? $8 : nullstmt;
			  $$ = new simplerule($<ste_p>6,
					      $5,
					      symtab->popscope(),
			 		      stmts, $2, $3);
			  $$->set_name($4);
			  hasrule = TRUE;
			}
                ;


// liveness extension
optFair         : UNFAIR { $$ = TRUE; }
                | { $$ = FALSE; }
                ;

endrule		: ENDRULE
		| END
		;


/* an optional condition on a rule. */
optCondition	: expr LONGARROW
			{
			  Error.CondError(!type_equal($1->gettype(),
						      booltype),
					  "Condition for rule must be a boolean expression.");
			  $$ = $1;
			}
		| /* empty */
			{ $$ = true_expr; }
		;


/* an optional name. */
optString	: STRING
			{ $$ = $1; }
		| /* empty */
			{ $$ = NULL; }
/*		| ID
			{ $$ = $1->getname(); }
*/
		;

/* Vitaly */

/* an optional priority */

optPriority	: INTCONST
			{ $$ = $1; }
		| /* empty */
			{ $$ = 0; }
		;


/* an aliased ruleset. */
aliasrule	: ALIAS
			{ symtab->pushscope(); }
		  aliases do rules endalias
			{ $$ = new aliasrule( symtab->popscope(FALSE), $5); }
		;

			  
/* a parametrized ruleset. */
ruleset		: RULESET
			{ symtab->pushscope(); }
		  quantifiers
		  do rules endruleset
			{ $$ = new quantrule( symtab->popscope(FALSE), $5 ); }
		;


endruleset	: ENDRULESET
		| END
		;


/* choosing from a multiset */
choose		: CHOOSE 
			{ symtab->pushscope(); }
                  ID ':' designator
                        {
			  Error.CondError( $5->gettype()->gettypeclass() != typedecl::MultiSet,
					   "Parameter for choose must be a multiset");
			  multisetidtypedecl *t = new multisetidtypedecl($5);
			  decl *tvar = new choosedecl(t);
			  symtab->declare( $3, tvar );
			}
		  do rules endchoose
			{
			  $$ = new chooserule( symtab->popscope(FALSE), $5, $8 );
			}
		;

onerule		: rule semi
			{ $$ = $1; }
		| rule
			{ $$ = $1; }
		;


endchoose	: ENDCHOOSE
		| END
		;


/* a single start state. */
startstate	: STARTSTATE
			{
			  $<ste_p>$ = symtab->topste();
			  symtab->pushscope();
			}
  		  optString optdecls optstmts endstartstate
			{
			  stmt *body = $5 != NULL ? $5 : nullstmt;
			  $$ = new startstate($<ste_p>2,
					      symtab->popscope(),
					      body);
			  $$->set_name($3);
			  has_startstate = TRUE;
			}
		;


endstartstate	: ENDSTARTSTATE
  		| END
		;


/* a list of invariants. */
/* optInvariants	: optInvariants invariant */
/*		| /* empty */
/*		;


/* a single invariant. */
invariant	: INVARIANT optString expr
			{
			  $$ = new invariant(symtab->topste(), $3);
			  $$->set_name($2);
			}
		;


/* a single fairness. */
fairness	: FAIRNESS optString expr
			{
			  $$ = new fairness(symtab->topste(), $3);
			  $$->set_name($2);
			}
		;

liveness        : liveU
                | liveE
                | liveAE
                | liveEA
                | liveAIU
                | liveAIE
                ;

liveU           : LIVENESS optString expr UNTIL expr
                  {
                    $$ = new liveness(symtab->topste(), 
				      NULL, $3, $5, U);
                    $$->set_name($2);
		  }
                ;

liveE           : LIVENESS optString EVENTUALLY expr
                  {
                    $$ = new liveness(symtab->topste(),
                                      NULL, NULL, $4, E);
                    $$->set_name($2);
                  }
                ;

liveEA          : LIVENESS optString EVENTUALLY ALWAYS expr
                  {
                    $$ = new liveness(symtab->topste(), 
				      $5, NULL, NULL, EA);
                    $$->set_name($2);
		  }
                ;

liveAE          : LIVENESS optString ALWAYS EVENTUALLY expr
                  {
                    $$ = new liveness(symtab->topste(), 
				      NULL, NULL, $5, AE);
                    $$->set_name($2);
		  }
                ;

liveAIE         : LIVENESS optString ALWAYS expr IMPLIES EVENTUALLY expr 
                  {
                    $$ = new liveness(symtab->topste(), 
				      $4, NULL, $7, AIE);
                    $$->set_name($2);
		  }
                ;

liveAIU         : LIVENESS optString ALWAYS expr IMPLIES expr UNTIL expr
                  {
                    $$ = new liveness(symtab->topste(), 
				      $4, $6, $8, AIU);
                    $$->set_name($2);
		  }
                ;

/****************************************
  * 18 Nov 93 Norris Ip: 
  don't add scalarset of size 1 or less to scalarsetlist
  * 7 Dec 93 Norris Ip: 
  changed using
    stelist *scalarsetlist;
  in program class to using
    symmetryclass symmetry; 
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 13 April 94 Norris Ip:
  added type checking for designator ID
  * 28 April 94 Norris Ip:
  Forbid liveness extension syntax to avoid confusion
  ******
  * Subject: Extension for {multiset, undefined value, general union}
  * 24 May 94 Norris Ip:
  exprlist::exprlist(bool b; ...) to include UNDEFINED keyword
  assignmentstmts to include UNDEFINED keyword
  * 25 May 94 Norris Ip:
  rename all scalarsetunion to union
  change all enum/scalarset values to be disjoint and distinct from 0
  * 7 OCT 94 Norris Ip:
  added multisettype parsing
****************************************/

/********************
 $Log: mu.y,v $
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
