import java.io.*;

/**
 * Parser for simple infix expressions consisting of single-digit operands
 * and the binary operators '+' and '-'.
 * <p>
 * This parser converts a single-line infix expression read from standard
 * input into its postfix form written to standard output. The parser treats
 * any whitespace as a lexical error (per lab specification) and reports
 * error position information when a syntax/lexical error is detected.
 */
class Parser {
	int lookahead; // current input character (or -1 for EOF)
	int pos;       // 1-based position of the last-read character

	public Parser() throws IOException {
		pos = 0;
		readChar();
	}

	/** Read next character from stdin into {@code lookahead} and update {@code pos}. */
	private void readChar() throws IOException {
		int next = System.in.read();
		if (next != -1) pos++;
		lookahead = next;
	}

	private String show(int c) {
		return c == -1 ? "EOF" : "'" + (char) c + "'";
	}

	void expr() throws IOException {
		term();
		rest();
		if (lookahead != -1 && lookahead != '\n' && lookahead != '\r')
			throw new RuntimeException("syntax error: unexpected character " + show(lookahead) + " at position " + pos);
	}

	/**
	 * Eliminate tail recursion by looping over sequences of + / - operators.
	 */
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
			} else {
				break;
			}
		}
	}

	void term() throws IOException {
		if (lookahead == -1)
			throw new RuntimeException("syntax error: unexpected EOF at position " + pos);
		if (Character.isWhitespace((char) lookahead))
			throw new RuntimeException("lexical error: unexpected whitespace at position " + pos);
		if (Character.isDigit((char) lookahead)) {
			System.out.write((char) lookahead);
			match(lookahead);
		} else
			throw new RuntimeException("syntax error: expected digit at position " + pos + ", found " + show(lookahead));
	}

	void match(int t) throws IOException {
		if (lookahead == t) {
			readChar();
		} else {
			throw new RuntimeException("syntax error: expected " + show(t) + " at position " + pos + ", found " + show(lookahead));
		}
	}
}

/**
 * Main class for the Postfix converter program.
 *
 * <p>Reads a single-line infix expression from standard input and writes
 * its postfix representation to standard output. On error the program prints
 * any partial postfix followed by " (error)"; detailed diagnostics are
 * printed to standard error.
 */
public class Postfix {
	/**
	 * Program entry point.
	 *
	 * @param args command-line arguments (ignored)
	 * @throws IOException if an I/O error occurs while reading input
	 */
	public static void main(String[] args) throws IOException {
		System.out.println("Input an infix expression and output its postfix notation:");
		// Use the recoverable parser implementation to report all errors in one pass
		ParserRecoverable p = new ParserRecoverable();
		p.expr();
		if (p.errorOccurred) {
			System.out.println(" (error)");
		} else {
			System.out.println("\nEnd of program.");
		}
	}
}
