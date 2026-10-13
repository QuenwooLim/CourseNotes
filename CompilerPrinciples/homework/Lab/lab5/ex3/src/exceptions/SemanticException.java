/**
 * Represents semantic errors detected during analysis.
 * All semantic error exceptions derive from this class.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class SemanticException extends OberonException {
    public SemanticException(String message) {
        super(message);
    }

    public SemanticException(String message, int line, int column) {
        super(message, line, column);
    }
}
