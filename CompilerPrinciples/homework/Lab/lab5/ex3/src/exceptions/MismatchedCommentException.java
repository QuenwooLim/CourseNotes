/**
 * Thrown when a comment starting with "(*" is not properly closed
 * with "*)", i.e., the closing delimiter is missing.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class MismatchedCommentException extends LexicalException {
    public MismatchedCommentException(int line, int column) {
        super("Mismatched comment: missing closing '*)'", line, column);
    }
}
