/**
 * Thrown when a right parenthesis ')' is missing (unmatched left parenthesis).
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class MissingRightParenthesisException extends SyntacticException {
    public MissingRightParenthesisException(int line, int column) {
        super("Missing right parenthesis ')'", line, column);
    }
}
