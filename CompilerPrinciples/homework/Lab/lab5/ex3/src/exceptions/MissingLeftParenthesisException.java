/**
 * Thrown when a left parenthesis '(' is missing (unmatched right parenthesis).
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class MissingLeftParenthesisException extends SyntacticException {
    public MissingLeftParenthesisException(int line, int column) {
        super("Missing left parenthesis '('", line, column);
    }
}
