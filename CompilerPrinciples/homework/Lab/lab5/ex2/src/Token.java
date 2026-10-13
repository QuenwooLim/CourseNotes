/**
 * Represents a token produced by the Oberon-0 lexical analyzer.
 * Each token has a type, lexeme text, and source position.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
public class Token {
    /** Enumeration of all Oberon-0 token types. */
    public enum Type {
        // Reserved words
        MODULE, END, PROCEDURE, BEGIN, CONST, TYPE, VAR,
        ARRAY, OF, RECORD, IF, THEN, ELSIF, ELSE, WHILE, DO,
        DIV_KW, MOD_KW, OR, RETURN,

        // Identifiers and literals
        IDENTIFIER, INTEGER_LITERAL,

        // Operators
        PLUS, MINUS, STAR, SLASH,         // + - * /
        EQ, NE, LT, LE, GT, GE,           // = # < <= > >=
        AND, NOT,                          // & ~
        ASSIGN,                           // :=

        // Delimiters
        LPAREN, RPAREN,                   // ( )
        LBRACKET, RBRACKET,               // [ ]
        DOT, COMMA, SEMICOLON, COLON,     // . , ; :

        // End of file
        EOF
    }

    private final Type type;
    private final String lexeme;
    private final int line;
    private final int column;

    public Token(Type type, String lexeme, int line, int column) {
        this.type = type;
        this.lexeme = lexeme;
        this.line = line;
        this.column = column;
    }

    public Type getType() { return type; }
    public String getLexeme() { return lexeme; }
    public int getLine() { return line; }
    public int getColumn() { return column; }

    @Override
    public String toString() {
        return String.format("Token{%s, '%s', line=%d, col=%d}",
                type, lexeme, line, column);
    }
}
