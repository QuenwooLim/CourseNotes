/**
 * Thrown when an operand is missing in an expression.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class MissingOperandException extends SyntacticException {
    public MissingOperandException(int line, int column) {
        super("Missing operand", line, column);
    }
}
