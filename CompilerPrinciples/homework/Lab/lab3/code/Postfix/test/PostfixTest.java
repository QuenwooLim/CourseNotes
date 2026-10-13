import org.junit.Test;
import static org.junit.Assert.*;
import java.io.*;

/**
 * Unit tests for the `Postfix` parser and its error-reporting behavior.
 */
public class PostfixTest {

    private String runParser(String input) {
        InputStream originalIn = System.in;
        PrintStream originalOut = System.out;
        PrintStream originalErr = System.err;

        try {
            ByteArrayInputStream in = new ByteArrayInputStream(input.getBytes());
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            ByteArrayOutputStream err = new ByteArrayOutputStream();
            System.setIn(in);
            System.setOut(new PrintStream(out));
            System.setErr(new PrintStream(err));

            Postfix.main(new String[0]);

            String output = out.toString().replace("Input an infix expression and output its postfix notation:", "").replace("\nEnd of program.", "").trim();
            String error = err.toString().trim();

            if (!error.isEmpty()) {
                return output + " " + error;
            }
            return output;
        } catch (IOException e) {
            return "IOException: " + e.getMessage();
        } finally {
            System.setIn(originalIn);
            System.setOut(originalOut);
            System.setErr(originalErr);
        }
    }

    /** Simple expression conversion: "9-5+2" -> "95-2+" */
    @Test
    public void testSimpleExpression() {
        assertEquals("95-2+", runParser("9-5+2"));
    }

    /** Longer alternating +/- expression conversion */
    @Test
    public void testLongExpression() {
        assertEquals("12-3+4-5+6-7+8-9+0-", runParser("1-2+3-4+5-6+7-8+9-0"));
    }

    /** Missing operator should produce an error marker */
    @Test
    public void testMissingOperator() {
        assertTrue(runParser("95+2").contains("(error)"));
    }

    /** Adjacent operators like "+-" should be detected as error */
    @Test
    public void testAdjacentOperators() {
        assertTrue(runParser("9-5+-2").contains("(error)"));
    }

    /** Leading operator without left operand should be an error */
    @Test
    public void testLeadingOperator() {
        assertTrue(runParser("+1-2").contains("(error)"));
    }

    /** Trailing operator without right operand should be an error */
    @Test
    public void testTrailingOperator() {
        assertTrue(runParser("1-2+").contains("(error)"));
    }

    /** Invalid character (e.g. '*') should be reported as error */
    @Test
    public void testInvalidCharacter() {
        assertTrue(runParser("1*2").contains("(error)"));
    }

    /** Whitespace in expression is treated as error in this parser */
    @Test
    public void testWhitespace() {
        assertTrue(runParser("1 - 2").contains("(error)"));
    }
}
