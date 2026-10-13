/**
 * Represents syntactic errors detected during parsing.
 * All syntactic error exceptions derive from this class.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class SyntacticException extends OberonException {
    public SyntacticException(String message) {
        super(message);
    }

    public SyntacticException(String message, int line, int column) {
        super(message, line, column);
    }
}
