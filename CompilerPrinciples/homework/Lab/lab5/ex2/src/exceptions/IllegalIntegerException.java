/**
 * Thrown when an integer constant is not separated from a following
 * identifier by whitespace (e.g., "25id").
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class IllegalIntegerException extends LexicalException {
    public IllegalIntegerException(int line, int column) {
        super("Illegal integer constant: missing whitespace before identifier", line, column);
    }
}
