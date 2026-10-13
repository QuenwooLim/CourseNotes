/**
 * The root class of all exceptions in the ROSE project.
 * All lexical, syntactic, and semantic exceptions derive from this class.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
package exceptions;

public class OberonException extends Exception {
    private int line;
    private int column;

    public OberonException(String message) {
        super(message);
    }

    public OberonException(String message, int line, int column) {
        super(message);
        this.line = line;
        this.column = column;
    }

    public int getLine() {
        return line;
    }

    public int getColumn() {
        return column;
    }

    @Override
    public String toString() {
        String loc = (line > 0) ? " at line " + line + ", column " + column : "";
        return getClass().getSimpleName() + ": " + getMessage() + loc;
    }
}
