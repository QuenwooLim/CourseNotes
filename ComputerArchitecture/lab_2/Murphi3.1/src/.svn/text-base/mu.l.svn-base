%{ 

/* -*- C++ -*- */

/*
 * mu.l
 * @(#) lex (flex, really) source for the lexer for Murphi.
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
// #include "y.tab.h"
// #include "lextable.h"

// default yywrap function - always treat EOF as an EOF
// added by Uli
#define yywrap() 1

%}

%x	        comment lrcomment

LETTER		[a-zA-Z_]
DIGIT		[0-9]
ID		({LETTER}({LETTER}|{DIGIT})*)
INTEGER		({DIGIT}+)
STRING          (\"[^"\n]*\")

%%
[ \t\f]		;
[\n]		{ gLineNum++; }



".."		{ return(DOTDOT); }
"==>"		{ return(LONGARROW); }
":="		{ return(ASSIGN); }
"<="		{ return(LEQ); }
"!="		{ return(NEQ); }
">="		{ return(GEQ); }
"!"		{ return( NOT ); }
"->"		{ return( IMPLIES ); }

{ID}		{ /* also catches reserved words. rlm. */ 
        lexid *temp;
	if( strlen(yytext) > IDLEN ) {
          Error.Error( "Identifier ", yytext, " too long." );
        }
	if( yytext[0] == '_' )
	  {
	    Error.Error("Identifiers beginning with an underscore are reserved by the system.");
	  }
        temp = ltable.enter(yytext);
	yylval.lex = temp;
	if ( temp->getlextype() == OLDEND )
	  {
	    Error.Error("All types of End* have been replaced with just 'End'.");
	    return (END);
	  }
        else return( temp->getlextype() );
      }

{INTEGER}		{	sscanf( yytext, "%d", &(yylval.integer) );
        return(INTCONST);
     }

{STRING}		{
		    yylval.string = new char[ yyleng ];
		    strcpy(yylval.string,yytext + 1); /* Dup w/o leading ". */
		    yylval.string [ yyleng - 2 ] = '\0';
		    return(STRING);
		  }

[-;,.()\+=:\[\]\"<>*/|&\{\}?\%]	{ return( yytext[0] ); }

"--"		{	BEGIN comment;
			}
<comment>.		;
<comment>\n		{	gLineNum++;
				BEGIN (INITIAL);
			}


"/*"		{	BEGIN lrcomment;
   /* Only active in normal state, because nested comments not allowed. */
   /* Comment-handling code copied from the \'flexdoc\' man page. */
        }


<lrcomment>[^*\n]*	/* eat anything that\'s not a '*' */
<lrcomment>"*"+[^*/\n]* /* eat up *\'s not followed by '/'s */
<lrcomment>\n		{	gLineNum++; }
<lrcomment>"*"+"/"	{	BEGIN(INITIAL);	}


.			{	/* No other rule applies, so it's a bad token */
 		 		Error.Error( "Bad input character`", yytext, "'." );
 			}

%%

void
LexInit( int listingSize ) {
/*  BEGIN norm; */
};

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
****************************************/

/********************
 $Log: mu.l,v $
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
