/**
 * Thrown when an identifier exceeds the maximum allowed length of
 * 24 characters.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class IllegalIdentifierLengthException extends LexicalException {
    public IllegalIdentifierLengthException(String id, int line, int column) {
        super("Identifier '" + id + "' exceeds maximum length (24 characters)", line, column);
    }
}
