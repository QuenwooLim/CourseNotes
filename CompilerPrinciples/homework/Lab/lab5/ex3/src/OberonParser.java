/**
 * Oberon-0 parser for Experiment 3 (JavaCUP-style parser).
 * Parses Oberon-0 source, detects syntax/semantic errors,
 * performs full type checking, and generates call graphs.
 *
 * @author Lin Hongyu
 * @version 3.00
 */
import java.io.*;
import java.util.*;
import exceptions.*;
import callgraph.*;

public class OberonParser {

    // --- Type system ---
    private enum OberonType { INTEGER, BOOLEAN }
    private static class TypedExpr {
        final String text;
        final OberonType type;
        TypedExpr(String t, OberonType ty) { text = t; type = ty; }
    }

    // --- Parser state ---
    private OberonScanner scanner;
    private Token lookahead;
    private CallGraph callGraph;
    private String currentModule, currentProcedure;
    private int callSiteCounter;
    private List<Exception> errors;

    // Symbol tables
    private String moduleName;
    private Map<String, String> typeAliases;        // alias → base type name
    private Scope scope;            // nested scope for variables and constants
    private Map<String, Integer> procedureParamCounts;
    private Set<String> declaredProcedures;
    private List<String[]> pendingEdges;
    private static final String[] BUILTIN_PROCS = {"Read", "Write", "WriteLn"};

    public OberonParser(OberonScanner scanner) {
        this.scanner = scanner;
        this.callGraph = new CallGraph();
        this.callSiteCounter = 0;
        this.errors = new ArrayList<>();
        this.typeAliases = new HashMap<>();
        this.scope = new Scope();
        this.procedureParamCounts = new HashMap<>();
        this.declaredProcedures = new HashSet<>();
        this.pendingEdges = new ArrayList<>();
        for (String bp : BUILTIN_PROCS) {
            callGraph.addProcedure(bp, bp);
            declaredProcedures.add(bp);
            procedureParamCounts.put(bp, bp.equals("WriteLn") ? 0 : 1);
        }
    }

    public CallGraph getCallGraph() { return callGraph; }
    public List<Exception> getErrors() { return errors; }
    public String getModuleName() { return moduleName; }

    // --- Type helpers ---
    private String resolveType(String name) {
        String cur = name;
        int limit = 20; // prevent infinite loop via circular aliases
        while (typeAliases.containsKey(cur) && limit-- > 0)
            cur = typeAliases.get(cur);
        return cur;
    }
    private boolean isIntegerType(String name) {
        String r = resolveType(name);
        return r.equalsIgnoreCase("INTEGER");
    }
    private boolean isBooleanType(String name) {
        String r = resolveType(name);
        return r.equalsIgnoreCase("BOOLEAN");
    }
    private String lookupVar(String name) {
        String t = scope.lookup(name);
        return t != null ? t : "INTEGER";
    }

    // --- Main ---
    public void parse() throws Exception {
        nextToken();
        module();
        if (lookahead.getType() != Token.Type.EOF)
            errors.add(new SyntacticException("Unexpected tokens after module end",
                lookahead.getLine(), lookahead.getColumn()));
        for (String[] edge : pendingEdges) {
            try { callGraph.addEdge(edge[0], edge[1]); }
            catch (Exception e) { errors.add(e); }
        }
    }

    // --- Scanner interface ---
    private void nextToken() throws IOException, LexicalException { lookahead = scanner.nextToken(); }
    private void match(Token.Type expected) throws Exception {
        if (lookahead.getType() == expected) nextToken();
        else throw new SyntacticException("Expected " + expected + " but found "
            + lookahead.getLexeme() + " (" + lookahead.getType() + ")",
            lookahead.getLine(), lookahead.getColumn());
    }
    private boolean check(Token.Type t) { return lookahead.getType() == t; }
    private String lex() { return lookahead.getLexeme(); }
    private int line() { return lookahead.getLine(); }
    private int col() { return lookahead.getColumn(); }

    // --- Phase 1 helpers ---
    private void registerCall(String procName, String stmt) {
        String callId = "c" + (++callSiteCounter);
        callGraph.addCallSite(callId, currentProcedure, stmt);
        if (declaredProcedures.contains(procName)) {
            try { callGraph.addEdge(callId, procName); }
            catch (Exception e) { errors.add(e); }
        } else pendingEdges.add(new String[]{callId, procName});
    }
    private void declareProcedure(String procName, int formalParamCount) {
        declaredProcedures.add(procName);
        procedureParamCounts.put(procName, formalParamCount);
        callGraph.addProcedure(procName, procName);
    }

    // ==================== GRAMMAR RULES ====================

    private void module() throws Exception {
        match(Token.Type.MODULE); moduleName = lex(); match(Token.Type.IDENTIFIER);
        match(Token.Type.SEMICOLON);
        declarations();
        if (check(Token.Type.BEGIN)) {
            match(Token.Type.BEGIN);
            currentProcedure = moduleName;
            if (!declaredProcedures.contains(moduleName)) {
                callGraph.addProcedure(moduleName, moduleName + " (module body)");
                declaredProcedures.add(moduleName);
            }
            statementSequence();
            match(Token.Type.END);
            String endName = lex(); match(Token.Type.IDENTIFIER);
            if (!endName.equalsIgnoreCase(moduleName))
                errors.add(new SemanticException("Module name mismatch: " + moduleName + " vs " + endName, line(), col()));
        } else {
            match(Token.Type.END);
            String endName = lex(); match(Token.Type.IDENTIFIER);
            if (!endName.equalsIgnoreCase(moduleName))
                errors.add(new SemanticException("Module name mismatch: " + moduleName + " vs " + endName, line(), col()));
        }
        match(Token.Type.DOT);
    }

    private void declarations() throws Exception {
        // CONST — track constant values (not types, just skip for now)
        if (check(Token.Type.CONST)) {
            match(Token.Type.CONST);
            while (check(Token.Type.IDENTIFIER)) {
                String cn = lex(); match(Token.Type.IDENTIFIER); match(Token.Type.EQ);
                TypedExpr e = expression(); match(Token.Type.SEMICOLON);
                scope.define(cn, e.type == OberonType.INTEGER ? "INTEGER" : "BOOLEAN");
            }
        }
        // TYPE aliases
        if (check(Token.Type.TYPE)) {
            match(Token.Type.TYPE);
            while (check(Token.Type.IDENTIFIER)) {
                String alias = lex(); match(Token.Type.IDENTIFIER); match(Token.Type.EQ);
                String base = typeName(); match(Token.Type.SEMICOLON);
                typeAliases.put(alias, base);
            }
        }
        // VAR declarations
        if (check(Token.Type.VAR)) {
            match(Token.Type.VAR);
            while (check(Token.Type.IDENTIFIER)) {
                List<String> ids = identifierList();
                match(Token.Type.COLON);
                String tn = typeName(); match(Token.Type.SEMICOLON);
                for (String id : ids) scope.define(id, tn);
            }
        }
        while (check(Token.Type.PROCEDURE)) {
            Scope savedScope = scope;
            procedureDeclaration();
            match(Token.Type.SEMICOLON);
            scope = savedScope;
        }
    }

    private void procedureDeclaration() throws Exception {
        match(Token.Type.PROCEDURE);
        String procName = lex(); match(Token.Type.IDENTIFIER);
        String savedProc = currentProcedure; currentProcedure = procName;
        scope = scope.child();
        int paramCount = 0;
        if (check(Token.Type.LPAREN)) paramCount = formalParameters();
        declareProcedure(procName, paramCount);
        match(Token.Type.SEMICOLON);
        procedureBody(procName);
        currentProcedure = savedProc;
    }

    private int formalParameters() throws Exception {
        int count = 0; match(Token.Type.LPAREN); count += fpSection();
        while (check(Token.Type.SEMICOLON)) { match(Token.Type.SEMICOLON); count += fpSection(); }
        match(Token.Type.RPAREN); return count;
    }

    private int fpSection() throws Exception {
        if (check(Token.Type.VAR)) match(Token.Type.VAR);
        List<String> ids = identifierList(); match(Token.Type.COLON);
        String tn = typeName();
        for (String id : ids) scope.define(id, tn);
        return ids.size();
    }

    private String typeName() throws Exception {
        if (check(Token.Type.ARRAY)) {
            match(Token.Type.ARRAY); expression(); match(Token.Type.OF);
            return typeName();
        } else if (check(Token.Type.RECORD)) {
            match(Token.Type.RECORD); fieldList();
            while (check(Token.Type.SEMICOLON)) { match(Token.Type.SEMICOLON); fieldList(); }
            match(Token.Type.END);
            return "RECORD";
        } else { String id = lex(); match(Token.Type.IDENTIFIER); return id; }
    }

    private void fieldList() throws Exception {
        if (check(Token.Type.IDENTIFIER)) { identifierList(); match(Token.Type.COLON); typeName(); }
    }

    private List<String> identifierList() throws Exception {
        List<String> ids = new ArrayList<>();
        ids.add(lex()); match(Token.Type.IDENTIFIER);
        while (check(Token.Type.COMMA)) { match(Token.Type.COMMA); ids.add(lex()); match(Token.Type.IDENTIFIER); }
        return ids;
    }

    private void procedureBody(String procName) throws Exception {
        declarations();
        if (check(Token.Type.BEGIN)) {
            match(Token.Type.BEGIN); statementSequence();
            match(Token.Type.END);
            String endName = lex(); match(Token.Type.IDENTIFIER);
            if (!endName.equalsIgnoreCase(procName))
                errors.add(new SemanticException("Proc name mismatch: " + procName + " END " + endName, line(), col()));
        } else {
            match(Token.Type.END);
            String endName = lex(); match(Token.Type.IDENTIFIER);
            if (!endName.equalsIgnoreCase(procName))
                errors.add(new SemanticException("Proc name mismatch: " + procName + " END " + endName, line(), col()));
        }
    }

    // --- Statements ---
    private void statementSequence() throws Exception {
        statement(); while (check(Token.Type.SEMICOLON)) { match(Token.Type.SEMICOLON); statement(); }
    }

    private void statement() throws Exception {
        if (check(Token.Type.IDENTIFIER)) {
            String name = lex(); match(Token.Type.IDENTIFIER);
            // Consume selector chain
            while (check(Token.Type.DOT) || check(Token.Type.LBRACKET)) selector();
            if (check(Token.Type.ASSIGN)) {
                match(Token.Type.ASSIGN);
                TypedExpr rhs = expression();
                // Type check: LHS must match RHS (simplified: check INTEGER vs BOOLEAN)
                String lhsType = lookupVar(name);
                if (rhs.type == OberonType.BOOLEAN && isIntegerType(lhsType))
                    errors.add(new TypeMismatchedException("Cannot assign BOOLEAN to " + lhsType, line(), col()));
            } else if (check(Token.Type.LPAREN)) {
                int actualCount = actualParameters();
                Integer expectedCount = procedureParamCounts.get(name);
                if (expectedCount != null && expectedCount != actualCount)
                    errors.add(new ParameterMismatchedException(expectedCount, actualCount, line(), col()));
                registerCall(name, name + "(...)");
            } else {
                Integer expectedCount = procedureParamCounts.get(name);
                if (expectedCount != null && expectedCount != 0)
                    errors.add(new ParameterMismatchedException(expectedCount, 0, line(), col()));
                registerCall(name, name);
            }
        } else if (check(Token.Type.IF)) ifStatement();
        else if (check(Token.Type.WHILE)) whileStatement();
    }

    private void ifStatement() throws Exception {
        match(Token.Type.IF);
        TypedExpr cond = expression();
        if (cond.type != OberonType.BOOLEAN)
            errors.add(new TypeMismatchedException("IF condition must be BOOLEAN", line(), col()));
        match(Token.Type.THEN); statementSequence();
        while (check(Token.Type.ELSIF)) {
            match(Token.Type.ELSIF);
            TypedExpr ec = expression();
            if (ec.type != OberonType.BOOLEAN)
                errors.add(new TypeMismatchedException("ELSIF condition must be BOOLEAN", line(), col()));
            match(Token.Type.THEN); statementSequence();
        }
        if (check(Token.Type.ELSE)) { match(Token.Type.ELSE); statementSequence(); }
        match(Token.Type.END);
    }

    private void whileStatement() throws Exception {
        match(Token.Type.WHILE);
        TypedExpr cond = expression();
        if (cond.type != OberonType.BOOLEAN)
            errors.add(new TypeMismatchedException("WHILE condition must be BOOLEAN", line(), col()));
        match(Token.Type.DO); statementSequence(); match(Token.Type.END);
    }

    private int actualParameters() throws Exception {
        int count = 0; match(Token.Type.LPAREN);
        if (!check(Token.Type.RPAREN)) { expression(); count++; while (check(Token.Type.COMMA)) { match(Token.Type.COMMA); expression(); count++; } }
        match(Token.Type.RPAREN); return count;
    }

    private void selector() throws Exception {
        if (check(Token.Type.DOT)) { match(Token.Type.DOT); match(Token.Type.IDENTIFIER); }
        else if (check(Token.Type.LBRACKET)) { match(Token.Type.LBRACKET); expression(); match(Token.Type.RBRACKET); }
    }

    // ==================== EXPRESSIONS WITH TYPE CHECKING ====================

    private TypedExpr expression() throws Exception {
        TypedExpr left = simpleExpression();
        if (check(Token.Type.EQ) || check(Token.Type.NE) || check(Token.Type.LT) ||
            check(Token.Type.LE) || check(Token.Type.GT) || check(Token.Type.GE)) {
            Token.Type op = lookahead.getType(); String opText = lex(); match(op);
            TypedExpr right = simpleExpression();
            // Relational ops: both operands must be INTEGER, result is BOOLEAN
            if (left.type != OberonType.INTEGER || right.type != OberonType.INTEGER)
                errors.add(new TypeMismatchedException(opText + " requires INTEGER operands", line(), col()));
            return new TypedExpr(left.text + " " + opText + " " + right.text, OberonType.BOOLEAN);
        }
        return left;
    }

    private TypedExpr simpleExpression() throws Exception {
        String prefix = "";
        if (check(Token.Type.PLUS)) { prefix = "+"; match(Token.Type.PLUS); }
        else if (check(Token.Type.MINUS)) { prefix = "-"; match(Token.Type.MINUS); }
        TypedExpr left = term();
        if (!prefix.isEmpty()) {
            if (left.type != OberonType.INTEGER)
                errors.add(new TypeMismatchedException("Unary " + prefix + " requires INTEGER", line(), col()));
            left = new TypedExpr(prefix + left.text, OberonType.INTEGER);
        }

        while (check(Token.Type.PLUS) || check(Token.Type.MINUS) || check(Token.Type.OR)) {
            Token.Type op = lookahead.getType(); String opText = lex(); match(op);
            TypedExpr right = term();
            if (op == Token.Type.OR) {
                if (left.type != OberonType.BOOLEAN || right.type != OberonType.BOOLEAN)
                    errors.add(new TypeMismatchedException("OR requires BOOLEAN operands", line(), col()));
                left = new TypedExpr(left.text + " OR " + right.text, OberonType.BOOLEAN);
            } else {
                if (left.type != OberonType.INTEGER || right.type != OberonType.INTEGER)
                    errors.add(new TypeMismatchedException(opText + " requires INTEGER operands", line(), col()));
                left = new TypedExpr(left.text + " " + opText + " " + right.text, OberonType.INTEGER);
            }
        }
        return left;
    }

    private TypedExpr term() throws Exception {
        TypedExpr left = factor();
        while (check(Token.Type.STAR) || check(Token.Type.SLASH) || check(Token.Type.DIV_KW) ||
               check(Token.Type.MOD_KW) || check(Token.Type.AND)) {
            Token.Type op = lookahead.getType(); String opText = lex(); match(op);
            TypedExpr right = factor();
            if (op == Token.Type.AND) {
                if (left.type != OberonType.BOOLEAN || right.type != OberonType.BOOLEAN)
                    errors.add(new TypeMismatchedException("& requires BOOLEAN operands", line(), col()));
                left = new TypedExpr(left.text + " & " + right.text, OberonType.BOOLEAN);
            } else {
                if (left.type != OberonType.INTEGER || right.type != OberonType.INTEGER)
                    errors.add(new TypeMismatchedException(opText + " requires INTEGER operands", line(), col()));
                left = new TypedExpr(left.text + " " + opText + " " + right.text, OberonType.INTEGER);
            }
        }
        return left;
    }

    private TypedExpr factor() throws Exception {
        String text;
        OberonType type;
        if (check(Token.Type.IDENTIFIER)) {
            String name = lex(); match(Token.Type.IDENTIFIER);
            // Check if TRUE/FALSE → BOOLEAN
            if (name.equalsIgnoreCase("TRUE") || name.equalsIgnoreCase("FALSE")) {
                text = name; type = OberonType.BOOLEAN;
            } else {
                text = name; type = isBooleanType(lookupVar(name)) ? OberonType.BOOLEAN : OberonType.INTEGER;
            }
            while (check(Token.Type.DOT) || check(Token.Type.LBRACKET)) {
                if (check(Token.Type.DOT)) {
                    match(Token.Type.DOT); text += "." + lex(); match(Token.Type.IDENTIFIER);
                    // Record field access → still compound type (INTEGER in practice)
                    type = OberonType.INTEGER;
                } else {
                    match(Token.Type.LBRACKET); TypedExpr e = expression(); match(Token.Type.RBRACKET);
                    if (e.type != OberonType.INTEGER)
                        errors.add(new TypeMismatchedException("Array index must be INTEGER", line(), col()));
                    text += "[" + e.text + "]";
                    type = OberonType.INTEGER; // array element
                }
            }
            return new TypedExpr(text, type);
        } else if (check(Token.Type.INTEGER_LITERAL)) {
            text = lex(); match(Token.Type.INTEGER_LITERAL);
            return new TypedExpr(text, OberonType.INTEGER);
        } else if (check(Token.Type.LPAREN)) {
            match(Token.Type.LPAREN); TypedExpr e = expression(); match(Token.Type.RPAREN);
            return new TypedExpr("(" + e.text + ")", e.type);
        } else if (check(Token.Type.NOT)) {
            match(Token.Type.NOT); TypedExpr e = factor();
            if (e.type != OberonType.BOOLEAN)
                errors.add(new TypeMismatchedException("~ requires BOOLEAN operand", line(), col()));
            return new TypedExpr("~" + e.text, OberonType.BOOLEAN);
        } else throw new MissingOperandException(line(), col());
    }

    // --- Main ---
    public static void main(String[] args) {
        if (args.length < 1) { System.err.println("Usage: java OberonParser <source-file> [--no-gui]"); System.exit(1); }
        String filename = args[0];
        boolean showGui = (args.length < 2 || !args[1].equals("--no-gui"));
        System.out.println("=== Parsing: " + filename + " ===");
        try {
            FileReader reader = new FileReader(filename);
            OberonScanner scanner = new OberonScanner(reader);
            OberonParser parser = new OberonParser(scanner);
            parser.parse();
            List<Exception> errors = parser.getErrors();
            System.out.println("Module: " + parser.getModuleName());
            System.out.println("Procedures: " + parser.getCallGraph().countProcedures());
            System.out.println("Call sites: " + parser.getCallGraph().countCallSites());
            System.out.println("Call edges: " + parser.getCallGraph().countEdges());
            if (errors.isEmpty()) {
                System.out.println("RESULT: SUCCESS");
                if (showGui) parser.getCallGraph().show();
            } else {
                System.out.println("Errors/warnings (" + errors.size() + "):");
                for (Exception e : errors)
                    System.out.println("  " + e.getClass().getSimpleName() + ": " + e.getMessage());
                if (showGui && parser.getCallGraph().countProcedures() > 0)
                    parser.getCallGraph().show();
            }
        } catch (LexicalException e) {
            System.err.println("LEXICAL ERROR: " + e);
            System.err.println("RESULT: FAILED - " + e.getClass().getSimpleName());
        } catch (SyntacticException e) {
            System.err.println("SYNTAX ERROR: " + e);
            System.err.println("RESULT: FAILED - " + e.getClass().getSimpleName());
        } catch (FileNotFoundException e) {
            System.err.println("ERROR: File not found: " + filename);
        } catch (Exception e) {
            System.err.println("ERROR: " + e.getClass().getSimpleName() + ": " + e.getMessage());
            e.printStackTrace();
        }
    }
}
