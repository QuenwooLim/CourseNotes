/**
 * Thrown when an octal constant (starting with 0) contains digits
 * outside the range 0-7 (i.e., 8 or 9).
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class IllegalOctalException extends LexicalException {
    public IllegalOctalException(int line, int column) {
        super("Illegal octal constant: digits 8 and 9 not allowed", line, column);
    }
}
