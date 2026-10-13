import java.io.*;
import java.util.*;
import exceptions.*;

public class CupTest {
    public static void main(String[] args) throws Exception {
        if (args.length < 1) { System.out.println("Usage: java CupTest <file> [--no-gui]"); return; }
        String file = args[0]; boolean gui = args.length < 2 || !args[1].equals("--no-gui");
        FileReader fr = new FileReader(file);
        OberonScanner scanner = new OberonScanner(fr);
        ScannerAdapter adapter = new ScannerAdapter(scanner);
        Parser parser = new Parser(adapter);
        try {
            parser.parse();
            List<Exception> errs = parser.getErrors();
            System.out.println("Parse complete. Errors: " + errs.size());
            for (Exception e : errs) System.out.println("  " + e.getClass().getSimpleName() + ": " + e.getMessage());
            System.out.println("Procedures: " + parser.getCallGraph().countProcedures());
            System.out.println("Call sites: " + parser.getCallGraph().countCallSites());
            System.out.println("Call edges: " + parser.getCallGraph().countEdges());
            System.out.println("RESULT: " + (errs.isEmpty() ? "SUCCESS" : "FAILED"));
            if (gui && parser.getCallGraph().countProcedures() > 0) parser.getCallGraph().show();
        } catch (Exception e) {
            System.out.println("RESULT: FAILED - " + e.getClass().getSimpleName() + ": " + e.getMessage());
        }
    }
}
