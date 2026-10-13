/**
 * @Copyright(C) 2008 Software Engineering Laboratory (SELAB), Department of Computer 
 * Science, SUN YAT-SEN UNIVERSITY. All rights reserved.
**/

import callgraph.*;

/**
 * A demonstrative main program for the usage of the CallGraph class. The example 
 * comes from the new version of dragon book (DBv2), pp.904-906. 
 * 
 * Note that the procedures and call sites may be inserted in any order, but the 
 * edges must be added only after all referenced procedures and call sites are added. 
 * 
 * @author Dr. Wen-jun LI
 * @author Da LUO
 * @version 1.01 (Last update: June 4, 2008)
**/
public class CallGraphDemo1 {
	public static void main(String[] args) throws Exception {
		// Create a call graph
		CallGraph graph = new CallGraph();

		// Add procedure nodes
		graph.addProcedure("f1", "fun1");
		graph.addProcedure("f2", "fun2");
		graph.addProcedure("main", "main");

		// Add call sites
		graph.addCallSite("c1", "fun1", "return (*pf)(x+1)");
		graph.addCallSite("c2", "fun2", "return (*pf)(y)");
		graph.addCallSite("c3", "main", "(*pf)(5)");

		// Add procedure calls
		graph.addEdge("c1", "f1");
		graph.addEdge("c1", "f2");
		graph.addEdge("c2", "f1");
		graph.addEdge("c2", "f2");
		graph.addEdge("c3", "f1");
		graph.addEdge("c3", "f2");

		// Show the graph
		graph.show();
	}
}
