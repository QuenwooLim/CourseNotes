/**
 * @Copyright(C) 2008 Software Engineering Laboratory (SELAB), Department of Computer 
 * Science, SUN YAT-SEN UNIVERSITY. All rights reserved.
**/

import callgraph.*;

/**
 * A demonstrative main program for the usage of the CallGraph class. The example 
 * shows that function a() calls b() and c(), and funciton c() calls b() and d(). 
 * 
 * @author Dr. Wen-jun LI
 * @author Da LUO
 * @version 1.01 (Last update: June 4, 2008)
**/
public class CallGraphDemo2 {
	public static void main(String[] args) throws Exception {
		// Create a call graph
		CallGraph graph = new CallGraph();

		// Add procedure nodes
		graph.addProcedure("a", "a()");
		graph.addProcedure("b", "b(INTEGER, INTEGER)");
		graph.addProcedure("c", "c(INTEGER, INTEGER)");
		graph.addProcedure("d", "d(INTEGER, INTEGER)");

		// Add call sites
		graph.addCallSite("a1", "a()", "b(x, 1);");
		graph.addCallSite("a2", "a()", "c(y, 2);");
		graph.addCallSite("c1", "c()", "b(x + i, 2 * j);");
		graph.addCallSite("c2", "c()", "d(2 * i, y + j);");

		// Add procedure calls
		graph.addEdge("a1", "b");
		graph.addEdge("a2", "c");
		graph.addEdge("c1", "b");
		graph.addEdge("c2", "d");

		// Show the graph
		graph.show();
	}
}
