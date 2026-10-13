/**
 * Thrown when a type incompatibility is detected, such as using
 * an INTEGER value where a BOOLEAN is expected, or vice versa.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class TypeMismatchedException extends SemanticException {
    public TypeMismatchedException(int line, int column) {
        super("Type mismatched", line, column);
    }

    public TypeMismatchedException(String detail, int line, int column) {
        super("Type mismatched: " + detail, line, column);
    }
}
