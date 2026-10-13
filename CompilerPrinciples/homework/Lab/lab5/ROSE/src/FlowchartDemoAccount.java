/**
 * @Copyright(C) 2008 Software Engineering Laboratory (SELAB), Department of Computer 
 * Science, SUN YAT-SEN UNIVERSITY. All rights reserved.
**/

import flowchart.*;

/**
 * A demonstrative main program for the usage of the API of flowchart package. The 
 * example shows how a flowchart can be generated based on a typical bank account 
 * module. A stack of AbstractStatement variables is preferred if you generate a 
 * flowchart while parsing. 
 * 
 * @author Dr. Wen-jun LI
 * @author Da LUO
 * @version 1.00 (Last update: June 8, 2008)
**/
public class FlowchartDemoAccount {
	public static void main(String[] args) throws Exception {
		String text = null;           // text of statements

		// Create a module
		Module accModule = new Module("Account");

		// Add statements to the 1st procedure
		Procedure proc = accModule.add("deposit(amount: INTEGER)");
		text = "balance := balance + amount;";
		proc.add(new PrimitiveStatement(text));
		text = "log(\"Perform deposit.\");";
		proc.add(new PrimitiveStatement(text));

		// Add statements to the 2nd procedure
		proc = accModule.add("withdraw(amount: INTEGER)");
		IfStatement ifStmt = new IfStatement("balance &gt= amount");
		proc.add(ifStmt);
		{
			text = "balance := balance - amount;";
			ifStmt.getTrueBody().add(new PrimitiveStatement(text));
			text = "writeln(\"Not enough money.\");<br>writeln(\"Don't try it again.\");";
			ifStmt.getFalseBody().add(new PrimitiveStatement(text));
		}
		text = "log(\"Perform withdraw.\");";
		proc.add(new PrimitiveStatement(text));

		// Add statements to the 3rd procedure
		proc = accModule.add("getBalance(var amount: INTEGER)");
		text = "amount := balance;";
		proc.add(new PrimitiveStatement(text));
		text = "log(\"Perform getBalance.\");";
		proc.add(new PrimitiveStatement(text));

		// Show the flowchart for each procedure in the module
		accModule.show();
	}
}
