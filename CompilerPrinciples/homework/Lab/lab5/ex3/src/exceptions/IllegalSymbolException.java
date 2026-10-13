/**
 * Thrown when an illegal input symbol (e.g., @, $) is encountered.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class IllegalSymbolException extends LexicalException {
    public IllegalSymbolException(String symbol, int line, int column) {
        super("Illegal symbol '" + symbol + "'", line, column);
    }
}
