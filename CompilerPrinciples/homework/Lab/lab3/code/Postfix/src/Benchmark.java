import java.io.*;

/**
 * Original version of Parser with static lookahead and tail recursion.
 */
class ParserOriginal {
    static int lookahead;
    static InputStream input;

    public ParserOriginal(InputStream s) throws IOException {
        input = s;
        lookahead = input.read();
    }

    void expr() throws IOException {
        term();
        rest();
    }

    void rest() throws IOException {
        if (lookahead == '+') {
            match('+');
            term();
            // System.out.write('+'); // Disabled output for clean timing
            rest();
        } else if (lookahead == '-') {
            match('-');
            term();
            // System.out.write('-');
            rest();
        }
    }

    void term() throws IOException {
        if (Character.isDigit((char) lookahead)) {
            match(lookahead);
        }
    }

    void match(int t) throws IOException {
        if (lookahead == t)
            lookahead = input.read();
    }
}

/**
 * Improved version of Parser withInstance lookahead and LOOP (no recursion).
 */
class ParserImproved {
    int lookahead;
    InputStream input;

    public ParserImproved(InputStream s) throws IOException {
        input = s;
        read();
    }

    void read() throws IOException {
        lookahead = input.read();
    }

    void expr() throws IOException {
        term();
        rest();
    }

    void rest() throws IOException {
        while (true) {
            if (lookahead == '+') {
                match('+');
                term();
            } else if (lookahead == '-') {
                match('-');
                term();
            } else {
                break;
            }
        }
    }

    void term() throws IOException {
        if (Character.isDigit((char) lookahead)) {
            match(lookahead);
        }
    }

    void match(int t) throws IOException {
        if (lookahead == t)
            read();
    }
}

/**
 * Micro-benchmark comparing the original recursive parser and the improved loop parser.
 *
 * <p>Usage: run `java -cp bin Benchmark [length]` where <code>length</code> is the
 * number of `+1` repetitions used to build the test expression. The benchmark
 * prints average milliseconds per operation for both implementations.</p>
 */
public class Benchmark {
    /**
     * Run the benchmark.
     *
     * @param args if present, <code>args[0]</code> is the length (number of "+1" occurrences)
     * @throws IOException on I/O error
     */
    public static void main(String[] args) throws IOException {
        int length = 5000; // Adjust for different test sizes
        if (args.length > 0) length = Integer.parseInt(args[0]);

        StringBuilder sb = new StringBuilder();
        sb.append("5");
        for (int i = 0; i < length; i++) {
            sb.append("+1");
        }
        String expr = sb.toString();
        byte[] bytes = expr.getBytes();

        System.out.println("Benchmark with expression length: " + (length * 2 + 1));

        // Warmup
        for(int i=0; i<100; i++) {
            new ParserOriginal(new ByteArrayInputStream(bytes)).expr();
            new ParserImproved(new ByteArrayInputStream(bytes)).expr();
        }

        int iterations = 1000;
        
        long startOrig = System.nanoTime();
        for (int i = 0; i < iterations; i++) {
            new ParserOriginal(new ByteArrayInputStream(bytes)).expr();
        }
        long endOrig = System.nanoTime();

        long startImp = System.nanoTime();
        for (int i = 0; i < iterations; i++) {
            new ParserImproved(new ByteArrayInputStream(bytes)).expr();
        }
        long endImp = System.nanoTime();

        double avgOrig = (endOrig - startOrig) / 1e6 / iterations;
        double avgImp = (endImp - startImp) / 1e6 / iterations;

        System.out.println("Original (Recursion): " + String.format("%.4f", avgOrig) + " ms/op");
        System.out.println("Improved (Loop):      " + String.format("%.4f", avgImp) + " ms/op");
        System.out.println("Speedup:             " + String.format("%.2f", (avgOrig / avgImp)) + "x");
    }
}
