/**
 * Oberon-0 Recursive Descent Predictive Parser for Experiment 4.
 *
 * This parser implements a top-down recursive descent analysis of
 * Oberon-0 programs based on a translation scheme designed from the
 * Oberon-0 EBNF grammar. It generates flowcharts using the flowchart API.
 *
 * Translation Scheme Design:
 * -------------------------
 * Each non-terminal maps to a recursive method. Inherited attributes
 * are passed as parameters; synthesized attributes are returned.
 * Semantic actions (flowchart node construction) are embedded at
 * the corresponding positions in the grammar productions.
 *
 * Grammar Adaptation for LL(1):
 * Oberon-0 EBNF uses {repetition} and [optional] extensively,
 * making it naturally suitable for recursive descent without
 * explicit left-recursion elimination.
 *
 * @author Lin Hongyu
 * @version 1.00
 */
import java.io.*;
import java.util.*;
import exceptions.*;
import flowchart.Procedure;
import flowchart.StatementSequence;
import flowchart.IfStatement;
import flowchart.WhileStatement;
import flowchart.PrimitiveStatement;

public class OberonParser {
    private OberonScanner scanner;
    private Token lookahead;
    private List<Exception> errors;

    // Flowchart generation
    private flowchart.Module currentModule;
    private Procedure currentProcedure;
    private StatementSequence currentBody;

    // For translation scheme: track statement text
    private StringBuilder stmtBuilder;
    private int indentLevel;

    public OberonParser(OberonScanner scanner) {
        this.scanner = scanner;
        this.errors = new ArrayList<>();
        this.stmtBuilder = new StringBuilder();
        this.indentLevel = 0;
    }

    public flowchart.Module getModule() { return currentModule; }
    public List<Exception> getErrors() { return errors; }

    // --- Main entry point ---
    public void parse() throws Exception {
        nextToken();
        module();
        if (lookahead.getType() != Token.Type.EOF) {
            errors.add(new SyntacticException("Unexpected tokens after module end",
                lookahead.getLine(), lookahead.getColumn()));
        }
    }

    // --- Scanner interface ---
    private void nextToken() throws IOException, LexicalException {
        lookahead = scanner.nextToken();
        while (lookahead.getType() == Token.Type.EOF && false) {
            // handled by caller
        }
    }

    private boolean check(Token.Type t) { return lookahead.getType() == t; }

    private void match(Token.Type expected) throws Exception {
        if (check(expected)) {
            nextToken();
        } else {
            throw new SyntacticException(
                "Expected " + expected + " but found " + lookahead.getLexeme()
                + " (" + lookahead.getType() + ")",
                lookahead.getLine(), lookahead.getColumn());
        }
    }

    private String getLexeme() { return lookahead.getLexeme(); }
    private int getLine() { return lookahead.getLine(); }
    private int getCol() { return lookahead.getColumn(); }

    // --- Translation Scheme: Grammar Rules with Semantic Actions ---

    /**
     * module = "MODULE" identifier ";" declarations
     *          ["BEGIN" statement_sequence] "END" identifier ".".
     *
     * Semantic action: Create a Module object for flowchart generation.
     */
    private void module() throws Exception {
        match(Token.Type.MODULE);
        String moduleName = getLexeme();
        match(Token.Type.IDENTIFIER);
        match(Token.Type.SEMICOLON);

        // Semantic action: create Module
        currentModule = new flowchart.Module(moduleName);

        declarations();

        if (check(Token.Type.BEGIN)) {
            match(Token.Type.BEGIN);

            // Semantic action: create module body as a procedure
            currentProcedure = currentModule.add(moduleName + "(body)");
            currentBody = new StatementSequence();

            statementSequence();

            currentProcedure.add(currentBody);

            match(Token.Type.END);
            String endName = getLexeme();
            match(Token.Type.IDENTIFIER);
            if (!endName.equalsIgnoreCase(moduleName)) {
                errors.add(new SemanticException("Module name mismatch: "
                    + moduleName + " vs " + endName, getLine(), getCol()));
            }
        } else {
            match(Token.Type.END);
            String endName = getLexeme();
            match(Token.Type.IDENTIFIER);
            if (!endName.equalsIgnoreCase(moduleName)) {
                errors.add(new SemanticException("Module name mismatch: "
                    + moduleName + " vs " + endName, getLine(), getCol()));
            }
        }
        match(Token.Type.DOT);
    }

    /**
     * declarations = ["CONST" {identifier "=" expression ";"}]
     *                ["TYPE" {identifier "=" type ";"}]
     *                ["VAR" {identifier_list ":" type ";"}]
     *                {procedure_declaration ";"}.
     *
     * Semantic action: None directly; declarations set up symbol table context.
     */
    private void declarations() throws Exception {
        // CONST section
        if (check(Token.Type.CONST)) {
            match(Token.Type.CONST);
            while (check(Token.Type.IDENTIFIER)) {
                String constName = getLexeme();
                match(Token.Type.IDENTIFIER);
                match(Token.Type.EQ);
                String value = expressionText();
                match(Token.Type.SEMICOLON);
            }
        }
        // TYPE section
        if (check(Token.Type.TYPE)) {
            match(Token.Type.TYPE);
            while (check(Token.Type.IDENTIFIER)) {
                match(Token.Type.IDENTIFIER);
                match(Token.Type.EQ);
                String typeText = typeText();
                match(Token.Type.SEMICOLON);
            }
        }
        // VAR section
        if (check(Token.Type.VAR)) {
            match(Token.Type.VAR);
            while (check(Token.Type.IDENTIFIER)) {
                String idList = identifierListText();
                match(Token.Type.COLON);
                String varType = typeText();
                match(Token.Type.SEMICOLON);
            }
        }
        // Procedure declarations
        while (check(Token.Type.PROCEDURE)) {
            procedureDeclaration();
            match(Token.Type.SEMICOLON);
        }
    }

    /**
     * procedure_declaration = procedure_heading ";" procedure_body.
     *
     * Semantic action: Create a Procedure object and add to Module.
     */
    private void procedureDeclaration() throws Exception {
        String procName = procedureHeading();
        match(Token.Type.SEMICOLON);
        procedureBody(procName);
    }

    /**
     * procedure_heading = "PROCEDURE" identifier [formal_parameters].
     *
     * @return the procedure name (synthesized attribute)
     */
    private String procedureHeading() throws Exception {
        match(Token.Type.PROCEDURE);
        String procName = getLexeme();
        match(Token.Type.IDENTIFIER);

        // Semantic action: create Procedure node for flowchart
        currentProcedure = currentModule.add(procName);
        currentBody = new StatementSequence();

        if (check(Token.Type.LPAREN)) {
            formalParameters();
        }
        return procName;
    }

    /**
     * procedure_body = declarations ["BEGIN" statement_sequence]
     *                  "END" identifier.
     *
     * Inherited attribute: procName (for END name verification)
     */
    private void procedureBody(String procName) throws Exception {
        declarations();

        if (check(Token.Type.BEGIN)) {
            match(Token.Type.BEGIN);
            statementSequence();
            match(Token.Type.END);
            String endName = getLexeme();
            match(Token.Type.IDENTIFIER);
            if (!endName.equalsIgnoreCase(procName)) {
                errors.add(new SemanticException("Procedure name mismatch: "
                    + procName + " vs " + endName, getLine(), getCol()));
            }
        } else {
            // Procedure with no body (just declarations)
            match(Token.Type.END);
            String endName = getLexeme();
            match(Token.Type.IDENTIFIER);
            if (!endName.equalsIgnoreCase(procName)) {
                errors.add(new SemanticException("Procedure name mismatch: "
                    + procName + " vs " + endName, getLine(), getCol()));
            }
        }

        // Semantic action: add collected statements to procedure
        if (currentBody.count() > 0) {
            currentProcedure.add(currentBody);
        }
    }

    /**
     * formal_parameters = "(" [fp_section {";" fp_section}] ")".
     */
    private void formalParameters() throws Exception {
        match(Token.Type.LPAREN);
        if (!check(Token.Type.RPAREN)) {
            fpSection();
            while (check(Token.Type.SEMICOLON)) {
                match(Token.Type.SEMICOLON);
                fpSection();
            }
        }
        match(Token.Type.RPAREN);
    }

    private void fpSection() throws Exception {
        if (check(Token.Type.VAR)) {
            match(Token.Type.VAR);
        }
        identifierListText();
        match(Token.Type.COLON);
        typeText();
    }

    /**
     * type = identifier | array_type | record_type.
     *
     * @return type text representation (synthesized attribute)
     */
    private String typeText() throws Exception {
        if (check(Token.Type.ARRAY)) {
            match(Token.Type.ARRAY);
            String sizeExpr = expressionText();
            match(Token.Type.OF);
            String elemType = typeText();
            return "ARRAY " + sizeExpr + " OF " + elemType;
        } else if (check(Token.Type.RECORD)) {
            match(Token.Type.RECORD);
            StringBuilder sb = new StringBuilder("RECORD ");
            sb.append(fieldListText());
            while (check(Token.Type.SEMICOLON)) {
                match(Token.Type.SEMICOLON);
                sb.append("; ").append(fieldListText());
            }
            match(Token.Type.END);
            return sb.append(" END").toString();
        } else {
            String id = getLexeme();
            match(Token.Type.IDENTIFIER);
            return id;
        }
    }

    private String fieldListText() throws Exception {
        if (check(Token.Type.IDENTIFIER)) {
            String ids = identifierListText();
            match(Token.Type.COLON);
            String ft = typeText();
            return ids + ": " + ft;
        }
        return "";
    }

    /**
     * identifier_list = identifier {"," identifier}.
     *
     * @return comma-separated identifier list text
     */
    private String identifierListText() throws Exception {
        StringBuilder sb = new StringBuilder(getLexeme());
        match(Token.Type.IDENTIFIER);
        while (check(Token.Type.COMMA)) {
            match(Token.Type.COMMA);
            sb.append(", ").append(getLexeme());
            match(Token.Type.IDENTIFIER);
        }
        return sb.toString();
    }

    /**
     * statement_sequence = statement {";" statement}.
     *
     * Semantic action: Statements are added to currentBody (inherited attribute).
     */
    private void statementSequence() throws Exception {
        statement();
        while (check(Token.Type.SEMICOLON)) {
            match(Token.Type.SEMICOLON);
            statement();
        }
    }

    /**
     * statement = [assignment | procedure_call | if_statement | while_statement].
     *
     * Semantic action: Construct flowchart statement nodes.
     */
    private void statement() throws Exception {
        if (check(Token.Type.IDENTIFIER)) {
            // Save position
            String name = getLexeme();
            int line = getLine(), col = getCol();
            match(Token.Type.IDENTIFIER);

            // Collect selector chain for assignment
            StringBuilder selectorText = new StringBuilder();
            while (check(Token.Type.DOT) || check(Token.Type.LBRACKET)) {
                if (check(Token.Type.DOT)) {
                    match(Token.Type.DOT);
                    selectorText.append(".").append(getLexeme());
                    match(Token.Type.IDENTIFIER);
                } else {
                    match(Token.Type.LBRACKET);
                    selectorText.append("[");
                    selectorText.append(expressionText());
                    selectorText.append("]");
                    match(Token.Type.RBRACKET);
                }
            }

            if (check(Token.Type.ASSIGN)) {
                // Assignment statement
                match(Token.Type.ASSIGN);
                String rhs = expressionText();
                String stmtText = name + selectorText + " := " + rhs;

                // Semantic action: add PrimitiveStatement
                currentBody.add(new PrimitiveStatement(stmtText));
            } else if (check(Token.Type.LPAREN)) {
                // Procedure call with parameters
                String params = actualParametersText();
                String stmtText = name + params;

                // Semantic action: add PrimitiveStatement
                currentBody.add(new PrimitiveStatement(stmtText));
            } else {
                // Procedure call without parameters
                // Semantic action: add PrimitiveStatement
                currentBody.add(new PrimitiveStatement(name));
            }
        } else if (check(Token.Type.IF)) {
            ifStatement();
        } else if (check(Token.Type.WHILE)) {
            whileStatement();
        }
        // epsilon: empty statement
    }

    /**
     * while_statement = "WHILE" expression "DO" statement_sequence "END".
     *
     * Semantic action: Create WhileStatement with loop body.
     * Inherited attribute: parent StatementSequence (for adding the WhileStatement)
     */
    private void whileStatement() throws Exception {
        match(Token.Type.WHILE);
        String condition = expressionText();
        match(Token.Type.DO);

        // Semantic action: create WhileStatement, save context
        WhileStatement wstmt = new WhileStatement(condition);
        StatementSequence savedBody = currentBody;
        currentBody = wstmt.getLoopBody();

        statementSequence();

        // Restore context and add WhileStatement
        currentBody = savedBody;
        currentBody.add(wstmt);

        match(Token.Type.END);
    }

    /**
     * if_statement = "IF" expression "THEN" statement_sequence
     *                {"ELSIF" expression "THEN" statement_sequence}
     *                ["ELSE" statement_sequence] "END".
     *
     * Semantic action: Create IfStatement with true/false bodies.
     */
    private void ifStatement() throws Exception {
        match(Token.Type.IF);
        String condition = expressionText();
        match(Token.Type.THEN);

        // Semantic action: create IfStatement for first IF
        IfStatement ifStmt = new IfStatement(condition);
        StatementSequence savedBody = currentBody;

        // Parse THEN body
        currentBody = ifStmt.getTrueBody();
        statementSequence();

        // Parse ELSIF chains (each ELSIF creates a nested IfStatement in falseBody)
        IfStatement currentIf = ifStmt;
        while (check(Token.Type.ELSIF)) {
            match(Token.Type.ELSIF);
            String elsifCond = expressionText();
            match(Token.Type.THEN);

            // Create nested IfStatement in falseBody for ELSIF
            IfStatement elsifStmt = new IfStatement(elsifCond);
            currentIf.getFalseBody().add(elsifStmt);

            currentBody = elsifStmt.getTrueBody();
            statementSequence();
            currentIf = elsifStmt;
        }

        // Parse ELSE body (final falseBody if present)
        if (check(Token.Type.ELSE)) {
            match(Token.Type.ELSE);
            currentBody = currentIf.getFalseBody();
            statementSequence();
        }

        // Restore context and add IfStatement
        currentBody = savedBody;
        currentBody.add(ifStmt);

        match(Token.Type.END);
    }

    /**
     * expression = simple_expression
     *              [("=" | "#" | "<" | "<=" | ">" | ">=") simple_expression].
     *
     * @return expression text (synthesized attribute)
     */
    private String expressionText() throws Exception {
        StringBuilder sb = new StringBuilder(simpleExpressionText());
        if (check(Token.Type.EQ) || check(Token.Type.NE) || check(Token.Type.LT)
            || check(Token.Type.LE) || check(Token.Type.GT) || check(Token.Type.GE)) {
            String op = getLexeme();
            match(lookahead.getType());
            sb.append(" ").append(op).append(" ").append(simpleExpressionText());
        }
        return sb.toString();
    }

    private String simpleExpressionText() throws Exception {
        StringBuilder sb = new StringBuilder();
        if (check(Token.Type.PLUS)) { match(Token.Type.PLUS); sb.append("+"); }
        else if (check(Token.Type.MINUS)) { match(Token.Type.MINUS); sb.append("-"); }
        sb.append(termText());
        while (check(Token.Type.PLUS) || check(Token.Type.MINUS) || check(Token.Type.OR)) {
            String op = getLexeme();
            match(lookahead.getType());
            sb.append(" ").append(op).append(" ").append(termText());
        }
        return sb.toString();
    }

    private String termText() throws Exception {
        StringBuilder sb = new StringBuilder(factorText());
        while (check(Token.Type.STAR) || check(Token.Type.SLASH) || check(Token.Type.DIV_KW)
            || check(Token.Type.MOD_KW) || check(Token.Type.AND)) {
            String op = getLexeme();
            match(lookahead.getType());
            sb.append(" ").append(op).append(" ").append(factorText());
        }
        return sb.toString();
    }

    private String factorText() throws Exception {
        if (check(Token.Type.IDENTIFIER)) {
            StringBuilder sb = new StringBuilder(getLexeme());
            match(Token.Type.IDENTIFIER);
            while (check(Token.Type.DOT) || check(Token.Type.LBRACKET)) {
                if (check(Token.Type.DOT)) {
                    match(Token.Type.DOT);
                    sb.append(".").append(getLexeme());
                    match(Token.Type.IDENTIFIER);
                } else {
                    match(Token.Type.LBRACKET);
                    sb.append("[");
                    sb.append(expressionText());
                    sb.append("]");
                    match(Token.Type.RBRACKET);
                }
            }
            return sb.toString();
        } else if (check(Token.Type.INTEGER_LITERAL)) {
            String val = getLexeme();
            match(Token.Type.INTEGER_LITERAL);
            return val;
        } else if (check(Token.Type.LPAREN)) {
            match(Token.Type.LPAREN);
            String expr = expressionText();
            match(Token.Type.RPAREN);
            return "(" + expr + ")";
        } else if (check(Token.Type.NOT)) {
            match(Token.Type.NOT);
            return "~" + factorText();
        } else {
            throw new MissingOperandException(getLine(), getCol());
        }
    }

    /**
     * actual_parameters = "(" [expression {"," expression}] ")".
     *
     * @return actual parameters text including parentheses
     */
    private String actualParametersText() throws Exception {
        StringBuilder sb = new StringBuilder("(");
        match(Token.Type.LPAREN);
        if (!check(Token.Type.RPAREN)) {
            sb.append(expressionText());
            while (check(Token.Type.COMMA)) {
                match(Token.Type.COMMA);
                sb.append(", ").append(expressionText());
            }
        }
        match(Token.Type.RPAREN);
        sb.append(")");
        return sb.toString();
    }

    // --- Safe main ---
    public static void main(String[] args) {
        if (args.length < 1) {
            System.err.println("Usage: java OberonParser <source-file> [--no-gui]");
            System.exit(1);
        }
        String filename = args[0];
        boolean showGui = (args.length < 2 || !args[1].equals("--no-gui"));

        System.out.println("=== Recursive Descent Parsing: " + filename + " ===");

        try {
            FileReader reader = new FileReader(filename);
            OberonScanner scanner = new OberonScanner(reader);
            OberonParser parser = new OberonParser(scanner);
            parser.parse();

            List<Exception> errors = parser.getErrors();
            flowchart.Module module = parser.getModule();

            System.out.println("Module: " + (module != null ? module.getName() : "null"));
            System.out.println("Procedures: " + (module != null ? module.count() : 0));

            if (errors.isEmpty()) {
                System.out.println("RESULT: SUCCESS");
            } else {
                System.out.println("Warnings/errors (" + errors.size() + "):");
                for (Exception e : errors) {
                    System.out.println("  " + e.getClass().getSimpleName() + ": " + e.getMessage());
                }
            }

            if (showGui && module != null && !module.isEmpty()) {
                module.show();
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
