/**
 * JFlex specification for the Oberon-0 lexical analyzer.
 * Generates OberonScanner.java which tokenizes Oberon-0 source code.
 *
 * Usage: jflex oberon.flex
 * Output: OberonScanner.java
 *
 * @author Lin Hongyu
 * @version 1.00
 */

import java.io.*;
import exceptions.*;

%%

%public
%class OberonScanner
%unicode
%line
%column
%type Token
%function nextToken
%yylexthrow LexicalException

%{
    private StringBuilder stringBuilder = new StringBuilder();

    private Token token(Token.Type type) {
        return new Token(type, yytext(), yyline + 1, yycolumn + 1);
    }

    private Token token(Token.Type type, String lexeme) {
        return new Token(type, lexeme, yyline + 1, yycolumn + 1);
    }

    private void checkIdentifierLength() throws IllegalIdentifierLengthException {
        if (yylength() > 24) {
            throw new IllegalIdentifierLengthException(yytext(), yyline + 1, yycolumn + 1);
        }
    }
%}

/* --- Macro Definitions --- */

LineTerminator  = \r|\n|\r\n
WhiteSpace      = {LineTerminator} | [ \t\f]

Letter          = [a-zA-Z]
Digit           = [0-9]

Identifier      = {Letter}({Letter}|{Digit})*
Integer         = {Digit}+
OctalInteger    = 0[0-7]*

/* Comment: (* ... *) with non-nesting behavior */
CommentContent  = [^*] | "*"[^)]

%state COMMENT

%%

/* --- Lexical Rules --- */

/* Whitespace (skip) */
<YYINITIAL> {
    {WhiteSpace}    { /* ignore whitespace */ }
}

/* Comments */
<YYINITIAL> {
    "(*"            { yybegin(COMMENT); stringBuilder.setLength(0); }
}

<COMMENT> {
    "*)"            { yybegin(YYINITIAL); }
    {CommentContent} { /* consume comment content */ }
    <<EOF>>         { throw new MismatchedCommentException(yyline + 1, yycolumn + 1); }
}

/* Reserved Words (case-insensitive matching) */
<YYINITIAL> {
    [Mm][Oo][Dd][Uu][Ll][Ee]    { return token(Token.Type.MODULE); }
    [Ee][Nn][Dd]                 { return token(Token.Type.END); }
    [Pp][Rr][Oo][Cc][Ee][Dd][Uu][Rr][Ee] { return token(Token.Type.PROCEDURE); }
    [Bb][Ee][Gg][Ii][Nn]         { return token(Token.Type.BEGIN); }
    [Cc][Oo][Nn][Ss][Tt]         { return token(Token.Type.CONST); }
    [Tt][Yy][Pp][Ee]             { return token(Token.Type.TYPE); }
    [Vv][Aa][Rr]                 { return token(Token.Type.VAR); }
    [Aa][Rr][Rr][Aa][Yy]         { return token(Token.Type.ARRAY); }
    [Oo][Ff]                     { return token(Token.Type.OF); }
    [Rr][Ee][Cc][Oo][Rr][Dd]     { return token(Token.Type.RECORD); }
    [Ii][Ff]                     { return token(Token.Type.IF); }
    [Tt][Hh][Ee][Nn]             { return token(Token.Type.THEN); }
    [Ee][Ll][Ss][Ii][Ff]         { return token(Token.Type.ELSIF); }
    [Ee][Ll][Ss][Ee]             { return token(Token.Type.ELSE); }
    [Ww][Hh][Ii][Ll][Ee]         { return token(Token.Type.WHILE); }
    [Dd][Oo]                     { return token(Token.Type.DO); }
    [Dd][Ii][Vv]                 { return token(Token.Type.DIV_KW); }
    [Mm][Oo][Dd]                 { return token(Token.Type.MOD_KW); }
    [Oo][Rr]                     { return token(Token.Type.OR); }
    [Rr][Ee][Tt][Uu][Rr][Nn]     { return token(Token.Type.RETURN); }
}

/* Operators */
<YYINITIAL> {
    ":="     { return token(Token.Type.ASSIGN); }
    "<="     { return token(Token.Type.LE); }
    ">="     { return token(Token.Type.GE); }
    "#"      { return token(Token.Type.NE); }
    "<"      { return token(Token.Type.LT); }
    ">"      { return token(Token.Type.GT); }
    "="      { return token(Token.Type.EQ); }
    "+"      { return token(Token.Type.PLUS); }
    "-"      { return token(Token.Type.MINUS); }
    "*"      { return token(Token.Type.STAR); }
    "/"      { return token(Token.Type.SLASH); }
    "&"      { return token(Token.Type.AND); }
    "~"      { return token(Token.Type.NOT); }
}

/* Delimiters */
<YYINITIAL> {
    "("      { return token(Token.Type.LPAREN); }
    ")"      { return token(Token.Type.RPAREN); }
    "["      { return token(Token.Type.LBRACKET); }
    "]"      { return token(Token.Type.RBRACKET); }
    "."      { return token(Token.Type.DOT); }
    ","      { return token(Token.Type.COMMA); }
    ";"      { return token(Token.Type.SEMICOLON); }
    ":"      { return token(Token.Type.COLON); }
}

/* Integer followed by identifier (no whitespace separator) — illegal */
<YYINITIAL> {
    /* Catch integer immediately followed by letter: e.g. "25id" */
    {Digit}+{Letter}({Letter}|{Digit})* {
        throw new IllegalIntegerException(yyline + 1, yycolumn + 1);
    }
}

/* Integer Literals */
<YYINITIAL> {
    /* Octal: 0 followed by digits - check for illegal 8/9 */
    0[0-9]* {
        String text = yytext();
        // Check for illegal octal digits (8 or 9)
        for (int i = 1; i < text.length(); i++) {
            if (text.charAt(i) == '8' || text.charAt(i) == '9') {
                throw new IllegalOctalException(yyline + 1, yycolumn + 1);
            }
        }
        // Check length limit (12 digits max)
        if (text.length() > 12) {
            throw new IllegalIntegerRangeException(yyline + 1, yycolumn + 1);
        }
        return token(Token.Type.INTEGER_LITERAL);
    }

    /* Decimal: non-zero digit followed by digits */
    [1-9]{Digit}* {
        String text = yytext();
        // Check length limit (12 digits max)
        if (text.length() > 12) {
            throw new IllegalIntegerRangeException(yyline + 1, yycolumn + 1);
        }
        return token(Token.Type.INTEGER_LITERAL);
    }
}

/* Identifier (after reserved words to give reserved words priority) */
<YYINITIAL> {
    {Identifier} {
        checkIdentifierLength();
        return token(Token.Type.IDENTIFIER);
    }
}

/* Error fallback - catch illegal symbols */
<YYINITIAL> {
    [^] {
        throw new IllegalSymbolException(yytext(), yyline + 1, yycolumn + 1);
    }
}

<<EOF>> {
    return token(Token.Type.EOF, "EOF");
}
