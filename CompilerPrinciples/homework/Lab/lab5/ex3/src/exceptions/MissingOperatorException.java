/**
 * Thrown when an operator is missing, or when a predefined function
 * call lacks required arguments.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class MissingOperatorException extends SyntacticException {
    public MissingOperatorException(int line, int column) {
        super("Missing operator", line, column);
    }

    public MissingOperatorException(String detail, int line, int column) {
        super("Missing operator: " + detail, line, column);
    }
}
