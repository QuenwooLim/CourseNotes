/**
 * Represents lexical errors detected during scanning.
 * All lexical error exceptions derive from this class.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class LexicalException extends OberonException {
    public LexicalException(String message) {
        super(message);
    }

    public LexicalException(String message, int line, int column) {
        super(message, line, column);
    }
}
