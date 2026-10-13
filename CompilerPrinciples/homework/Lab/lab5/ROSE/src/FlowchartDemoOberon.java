/**
 * @Copyright(C) 2008 Software Engineering Laboratory (SELAB), Department of Computer 
 * Science, SUN YAT-SEN UNIVERSITY. All rights reserved.
**/

import flowchart.*;

/**
 * A demonstrative main program for the usage of the API of flowchart package. The 
 * example comes from N. Wirth, <em>Theory and Techniques of Compiler Construction: 
 * An Introduction</em>, Addison-Wesley, 1996, ISBN 0-201-40353-6. 
 * 
 * @author Dr. Wen-jun LI
 * @author Da LUO
 * @version 1.00 (Last update: June 8, 2008)
**/
public class FlowchartDemoOberon {
	public static void main(String[] args) throws Exception {
		Module sampleModule = new Module("Sample");
		{
			Procedure proc = sampleModule.add("Multiply");
			proc.add(new PrimitiveStatement("Read(x); Read(y)"));
			proc.add(new PrimitiveStatement("z := 0"));
			WhileStatement wstmt = new WhileStatement("x &gt 0");
			proc.add(wstmt);
			{
				IfStatement istmt = new IfStatement("x MOD 2 = 1");
				wstmt.getLoopBody().add(istmt);
				{
					istmt.getTrueBody().add(new PrimitiveStatement("z := z + y"));
				}
				wstmt.getLoopBody().add(new PrimitiveStatement("y := 2 * y"));
				wstmt.getLoopBody().add(new PrimitiveStatement("x := x DIV 2"));
			}
			proc.add(new PrimitiveStatement("Write(x); Write(y)<br>Write(z); WriteLn"));
		}{
			Procedure proc = sampleModule.add("Divide");
			proc.add(new PrimitiveStatement("Read(x); Read(y)"));
			proc.add(new PrimitiveStatement("r := x; q := 0; w := y"));
			WhileStatement wstmt = new WhileStatement("w &lt= r");
			proc.add(wstmt);
			{
				wstmt.getLoopBody().add(new PrimitiveStatement("w := 2 * w"));
			}
			wstmt = new WhileStatement("w &gt y");
			proc.add(wstmt);
			{
				wstmt.getLoopBody().add(new PrimitiveStatement("q := 2 * q"));
				wstmt.getLoopBody().add(new PrimitiveStatement("w := w DIV 2"));
				IfStatement istmt = new IfStatement("w &lt= r");
				wstmt.getLoopBody().add(istmt);
				{
					istmt.getTrueBody().add(new PrimitiveStatement("r := r - w"));
					istmt.getTrueBody().add(new PrimitiveStatement("q := q + 1"));
				}
			}
			proc.add(new PrimitiveStatement("Write(x); Write(y)<br>Write(q); Write(r); WriteLn"));
		}{
			Procedure proc = sampleModule.add("BinSearch");
			proc.add(new PrimitiveStatement("Read(n)"));
			proc.add(new PrimitiveStatement("k := 0"));
			WhileStatement wstmt = new WhileStatement("k &lt n");
			proc.add(wstmt);
			{
				wstmt.getLoopBody().add(new PrimitiveStatement("Read(a[k])"));
				wstmt.getLoopBody().add(new PrimitiveStatement("k := k + 1"));
			}
			proc.add(new PrimitiveStatement("Read(x)"));
			proc.add(new PrimitiveStatement("i := 0; j := n"));
			wstmt = new WhileStatement("i &lt j");
			proc.add(wstmt);
			{
				wstmt.getLoopBody().add(new PrimitiveStatement("k := (i + j) DIV 2"));
				IfStatement istmt = new IfStatement("x &lt a[k]");
				wstmt.getLoopBody().add(istmt);
				{
					istmt.getTrueBody().add(new PrimitiveStatement("j := k"));
					istmt.getFalseBody().add(new PrimitiveStatement("i := k + 1"));
				}
			}
			proc.add(new PrimitiveStatement("Write(i); Write(j)<br>Write(a[j]); WriteLn"));
		}
		sampleModule.show();
	}
}
