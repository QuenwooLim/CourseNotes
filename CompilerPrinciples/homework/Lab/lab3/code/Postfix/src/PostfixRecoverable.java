import java.io.*;

/**
 * Enhanced Parser for infix expressions with Error Recovery.
 * <p>
 * This parser attempts to find all syntax and lexical errors in a single pass
 * by recovering after an error is encountered.
 */
class ParserRecoverable {
    int lookahead;
    int pos;
    boolean errorOccurred = false;

    public ParserRecoverable() throws IOException {
        pos = 0;
        readChar();
    }

    private void readChar() throws IOException {
        int next = System.in.read();
        if (next != -1) pos++;
        lookahead = next;
    }

    private String show(int c) {
        if (c == -1) return "EOF";
        if (c == '\n') return "\\n";
        if (c == '\r') return "\\r";
        return "'" + (char) c + "'";
    }

    private void reportError(String message) {
        errorOccurred = true;
        System.err.println("Error at position " + pos + ": " + message);
    }

    /**
     * Error Recovery: Synchronize to the next operator or EOF.
     */
    private void synchronize() throws IOException {
        while (lookahead != -1 && lookahead != '+' && lookahead != '-' && lookahead != '\n' && lookahead != '\r') {
            readChar();
        }
    }

    public void expr() throws IOException {
        term();
        rest();
        if (lookahead != -1 && lookahead != '\n' && lookahead != '\r') {
            reportError("unexpected character " + show(lookahead));
        }
    }

    void rest() throws IOException {
        while (true) {
            if (lookahead == '+') {
                match('+');
                term();
                System.out.write('+');
            } else if (lookahead == '-') {
                match('-');
                term();
                System.out.write('-');
            } else if (lookahead == -1 || lookahead == '\n' || lookahead == '\r') {
                break;
            } else {
                reportError("expected operator (+ or -), found " + show(lookahead));
                synchronize(); // Try to find next operator
            }
        }
    }

    void term() throws IOException {
        if (lookahead == -1 || lookahead == '\n' || lookahead == '\r') {
            reportError("expected digit, found " + show(lookahead));
            return;
        }
        
        if (Character.isWhitespace((char) lookahead)) {
            reportError("unexpected whitespace");
            readChar();
            term(); // try to skip and find digit
            return;
        }

        if (Character.isDigit((char) lookahead)) {
            System.out.write((char) lookahead);
            match(lookahead);
        } else {
            reportError("expected digit, found " + show(lookahead));
            // recovery: skip this invalid char and try to find operator in rest/expr
            readChar();
        }
    }

    void match(int t) throws IOException {
        if (lookahead == t) {
            readChar();
        } else {
            reportError("expected " + show(t) + ", found " + show(lookahead));
        }
    }
}

/**
 * Entry point for Recoverable Postfix.
 */
public class PostfixRecoverable {
    /**
     * Main entry; reads an expression from standard input and runs the recoverable
     * parser, printing a summary status message at the end.
     *
     * @param args command-line arguments (ignored)
     * @throws IOException on I/O error
     */
    public static void main(String[] args) throws IOException {
        System.out.println("Input an infix expression (Recoverable Mode):");
        ParserRecoverable p = new ParserRecoverable();
        p.expr();
        if (p.errorOccurred) {
            System.out.println("\nFinished with errors.");
        } else {
            System.out.println("\nFinished successfully.");
        }
    }
}
