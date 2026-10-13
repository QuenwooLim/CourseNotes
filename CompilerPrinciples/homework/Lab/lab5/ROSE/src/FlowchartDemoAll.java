/**
 * @Copyright(C) 2008 Software Engineering Laboratory (SELAB), Department of Computer 
 * Science, SUN YAT-SEN UNIVERSITY. All rights reserved.
**/

import flowchart.*;

/**
 * A demonstrative program for the usage of the API of flowchart package. 
 * 
 * @author Dr. Wen-jun LI
 * @author Da LUO
 * @version 1.00 (Last update: June 8, 2008)
**/
public class FlowchartDemoAll {
	public static void main(String[] args) throws Exception {
		/**
		 * Local variables. A stack of variables is preferred if you generate a 
		 * flowchart while parsing. 
		**/
		Module testModule = null;              // instance of a module
		Procedure proc = null;                 // instance of a procedure
		WhileStatement whileStmt = null;       // instance of a WHILE statement
		IfStatement ifStmt = null;             // instance of a IF statement
		StatementSequence loopBody = null;     // instance of a loop body

		// Create a module
		testModule = new Module("Test");

		// Procedure with only one statement
		proc = testModule.add("1. Simple procedure");
		proc.add(new PrimitiveStatement("ptr := 0;"));

		// Procedure with only one statement
		proc = testModule.add("2. Two statements");
		proc.add(new PrimitiveStatement("ptr := 0;"));
		proc.add(new PrimitiveStatement("total := 0;"));

		// An IF-THEN statement
		proc = testModule.add("3. IF-THEN statement");
		ifStmt = new IfStatement("BooleanExpression");
		proc.add(ifStmt);
		{
			ifStmt.getTrueBody().add(new PrimitiveStatement("total := total + price[ptr];"));
			ifStmt.getTrueBody().add(new PrimitiveStatement("ptr := ptr + 1;"));
		}

		// An IF-THEN-ELSE statement
		proc = testModule.add("4. IF-THEN-ELSE statement with wide left");
		ifStmt = new IfStatement("tryAVeryLongBooleanExpressionAsCondition");
		proc.add(ifStmt);
		{
			ifStmt.getTrueBody().add(new PrimitiveStatement("total := total + price[ptr] * number[ptr];"));
			ifStmt.getTrueBody().add(new PrimitiveStatement("ptr := ptr + 1;"));
		}{
			ifStmt.getFalseBody().add(new PrimitiveStatement("sleep(ptr);"));
		}

		// An IF-THEN-ELSE statement
		proc = testModule.add("5. IF-THEN-ELSE statement with wide right");
		ifStmt = new IfStatement("tryAVeryLongBooleanExpressionAsCondition");
		proc.add(ifStmt);
		{
			ifStmt.getTrueBody().add(new PrimitiveStatement("sleep(ptr);"));
		}{
			ifStmt.getFalseBody().add(new PrimitiveStatement("total := total + price[ptr] * number[ptr];"));
			ifStmt.getFalseBody().add(new PrimitiveStatement("ptr := ptr + 1;"));
		}

		// A WHILE statement
		proc = testModule.add("6. WHILE statement");
		whileStmt = new WhileStatement("ptr &lt= 100");    // use HTML escapes
		proc.add(whileStmt);
		{
			whileStmt.getLoopBody().add(new PrimitiveStatement("total := total + price[ptr];"));
			whileStmt.getLoopBody().add(new PrimitiveStatement("ptr := ptr + 1;"));
		}

		// A WHILE statement with an IF statement
		proc = testModule.add("7. WHILE and IF statement");
		whileStmt = new WhileStatement("ptr &lt= 100");
		proc.add(whileStmt);
		{
			ifStmt = new IfStatement("ptr &lt= 100");
			whileStmt.getLoopBody().add(ifStmt);
			{
				ifStmt.getTrueBody().add(new PrimitiveStatement("total := total + price[ptr];"));
				ifStmt.getTrueBody().add(new PrimitiveStatement("ptr := ptr + 1;"));
			}{
				ifStmt.getFalseBody().add(new PrimitiveStatement("sleep(ptr);"));
			}
		}

		// An IF statement with a WHILE statement
		proc = testModule.add("8. IF and WHILE statement");
		ifStmt = new IfStatement("ptr &lt= 100");
		proc.add(ifStmt);
		{
			ifStmt.getTrueBody().add(new PrimitiveStatement("total := total + price[ptr];"));
		}{
			whileStmt = new WhileStatement("TRUE and FALSE or TRUE");
			ifStmt.getFalseBody().add(whileStmt);
			{
				whileStmt.getLoopBody().add(new PrimitiveStatement("sleep(ptr + 100);"));
			}
		}

		// An IF statement with a WHILE statement
		proc = testModule.add("9. Example: Greated Common Divisor");
		proc.add(new PrimitiveStatement("Input <em>a</em> and <em>b</em>"));
		proc.add(new PrimitiveStatement("Let <em>r</em> = <em>a</em> <strong>mod</strong> <em>b</em>"));
		whileStmt = new WhileStatement("<em>r</em> &lt&gt 0");
		proc.add(whileStmt);
		{
			whileStmt.getLoopBody().add(new PrimitiveStatement("Let <em>a</em> = <em>b</em>"));
			whileStmt.getLoopBody().add(new PrimitiveStatement("Let <em>b</em> = <em>r</em>"));
			whileStmt.getLoopBody().add(new PrimitiveStatement("Let <em>r</em> = <em>a</em> <strong>mod</strong> <em>b</em>"));
		}
		proc.add(new PrimitiveStatement("Output <em>b</em>"));

		// A complex procedure
		proc = testModule.add("A1. Complex procedure");
		proc.add(new PrimitiveStatement("initialize();"));
		whileStmt = new WhileStatement("ptr &lt= 10");
		proc.add(whileStmt);
		{
			loopBody = whileStmt.getLoopBody();
			loopBody.add(new PrimitiveStatement("total := total + prices[ptr];"));
			loopBody.add(new PrimitiveStatement("time : = 0;"));
			whileStmt = new WhileStatement("time &gt= 100 AND time &lt= 1000");
			loopBody.add(whileStmt);
			{
				whileStmt.getLoopBody().add(new PrimitiveStatement("sleep();<br>time := time + 1;"));
				ifStmt = new IfStatement("booleanExpression1");
				whileStmt.getLoopBody().add(ifStmt);
				{
					ifStmt.getTrueBody().add(new PrimitiveStatement("total := total + price[ptr];"));
					ifStmt.getTrueBody().add(new PrimitiveStatement("ptr := ptr + 1;"));
				}
			}
			loopBody.add(new PrimitiveStatement("ptr := ptr + 1;"));
		}
		ifStmt = new IfStatement("aVeryLongBooleanExpression");
		proc.add(ifStmt);
		{
			ifStmt.getTrueBody().add(new PrimitiveStatement("total := total + price[ptr];"));
			ifStmt.getTrueBody().add(new PrimitiveStatement("ptr := ptr + 1;"));
		}{
			ifStmt.getFalseBody().add(new PrimitiveStatement("sleep(ptr);"));
		}
		proc.add(new PrimitiveStatement("writeln(\"Total is: \" + total);"));

		// Empty THEN branch
		proc = testModule.add("E1. Empty THEN branch");
		ifStmt = new IfStatement("a = b");
		proc.add(ifStmt);
		ifStmt.getFalseBody().add(new PrimitiveStatement("ptr := 0;"));

		// Empty THEN and ELSE branch
		proc = testModule.add("E2. Empty THEN and ELSE branch");
		proc.add(new IfStatement("a = b"));

		// Empty loop body
		proc = testModule.add("E3. Empty loop body");
		proc.add(new WhileStatement("a = b"));

		// Show the flowchart for each procedure in the module
		testModule.show();
	}
}
