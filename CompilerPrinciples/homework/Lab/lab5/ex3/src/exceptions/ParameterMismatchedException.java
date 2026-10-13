/**
 * Thrown when the number of actual parameters in a procedure call
 * does not match the number of formal parameters in the procedure
 * declaration.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class ParameterMismatchedException extends SemanticException {
    public ParameterMismatchedException(int expected, int actual, int line, int column) {
        super("Parameter count mismatched: expected " + expected + ", got " + actual, line, column);
    }
}
