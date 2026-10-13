/**
 * Test driver for the Oberon-0 lexical analyzer (OberonScanner).
 * Reads an Oberon-0 source file and prints all tokens found,
 * or reports lexical errors.
 *
 * Usage: java ScannerTest <source-file>
 *
 * @author Lin Hongyu
 * @version 1.00
 */
import java.io.*;
import exceptions.*;

public class ScannerTest {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.err.println("Usage: java ScannerTest <source-file>");
            System.exit(1);
        }

        String filename = args[0];
        System.out.println("=== Scanning: " + filename + " ===");

        try {
            FileReader reader = new FileReader(filename);
            OberonScanner scanner = new OberonScanner(reader);
            int tokenCount = 0;

            while (true) {
                Token token = scanner.nextToken();
                if (token.getType() == Token.Type.EOF) {
                    System.out.println("--- EOF ---");
                    break;
                }
                System.out.println(token);
                tokenCount++;
            }

            System.out.println("=== Scan complete: " + tokenCount + " tokens found ===");
            System.out.println("RESULT: SUCCESS");

        } catch (LexicalException e) {
            System.err.println("LEXICAL ERROR: " + e.toString());
            System.err.println("RESULT: FAILED - " + e.getClass().getSimpleName());
        } catch (FileNotFoundException e) {
            System.err.println("ERROR: File not found: " + filename);
            System.err.println("RESULT: FAILED - FileNotFound");
        } catch (IOException e) {
            System.err.println("ERROR: I/O error: " + e.getMessage());
            System.err.println("RESULT: FAILED - IOError");
        }
    }
}
