/**
 * Thrown when an integer constant exceeds the maximum allowed
 * number of digits (12 digits).
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class IllegalIntegerRangeException extends LexicalException {
    public IllegalIntegerRangeException(int line, int column) {
        super("Integer constant exceeds maximum range (12 digits)", line, column);
    }
}
