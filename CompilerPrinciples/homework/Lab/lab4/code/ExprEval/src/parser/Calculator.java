/**
 * @Copyright(C) 2008 Software Engineering Laboratory (SELAB), Department of Computer
 * Science, SUN YAT-SEN UNIVERSITY. All rights reserved.
**/

package parser;

import exceptions.*;
import java.util.*;

/**
 * Main program of the expression based calculator ExprEval.
 * Implements a complete lexer, operator-precedence parser, and semantic
 * evaluator for arithmetic/boolean expressions.
 *
 * @author [PENDING your name]
 * @version 1.00 (Last update: [PENDING the last update])
**/
public class Calculator
{
	// ── Token types ────────────────────────────────────────────────────
	private enum TokenType {
		NUMBER, BOOL, FUNC,
		PLUS, MINUS, MUL, DIV, POWER,
		EQ, NE, LT, LE, GT, GE,
		AND, OR, NOT,
		QUEST, COLON, COMMA,
		LPAREN, RPAREN,
		END
	}

	// ── Token ─────────────────────────────────────────────────────────
	private static class Token {
		TokenType type;
		String   text;
		double   numVal;
		boolean  boolVal;
		String   funcName;

		Token(TokenType t, String s) { type = t; text = s; }
	}

	// ── Value with type tag ───────────────────────────────────────────
	private static class Val {
		double  v;
		boolean isBool;
		Val(double vv, boolean b) { v = vv; isBool = b; }
	}

	// ── Operator stack entry ──────────────────────────────────────────
	private static class OpEntry {
		Token   tok;
		boolean isUnary;
		int     valBase;   // valStack size when FUNC's '(' was pushed
		OpEntry(Token t, boolean u) { tok = t; isUnary = u; valBase = -1; }
		OpEntry(Token t, boolean u, int vb) {
			tok = t; isUnary = u; valBase = vb;
		}
	}

	// ── Ternary state ─────────────────────────────────────────────────
	private static class TernState {
		Val cond;
		Val trueBranch;
		TernState(Val c) { cond = c; }
	}

	// ── Precedence (higher = tighter) ─────────────────────────────────
	// Level 11 (tightest): ( )   — handled specially
	// Level 10: sin cos max min — handled specially
	// Level  9: unary -
	// Level  8: ^
	// Level  7: * /
	// Level  6: + -
	// Level  5: = <> < <= > >=
	// Level  4: !
	// Level  3: &
	// Level  2: |
	// Level  1 (loosest): ?:
	private static int prec(TokenType t, boolean unary) {
		if (unary && t == TokenType.MINUS) return 9;
		switch (t) {
			case QUEST: case COLON: return 1;
			case OR:               return 2;
			case AND:              return 3;
			case NOT:              return 4;
			case EQ: case NE:
			case LT: case LE:
			case GT: case GE:      return 5;
			case PLUS: case MINUS: return 6;
			case MUL: case DIV:    return 7;
			case POWER:            return 8;
			default:               return 0;
		}
	}

	private static boolean rightAssoc(TokenType t) {
		return t == TokenType.POWER || t == TokenType.QUEST;
	}

	// ══════════════════════════════════════════════════════════════════
	//  LEXER
	// ══════════════════════════════════════════════════════════════════

	private List<Token> lex(String expr) throws LexicalException {
		List<Token> tokens = new ArrayList<>();
		int i = 0, n = expr.length();
		while (i < n) {
			char c = expr.charAt(i);
			if (c == ' ') { i++; continue; }

			if (c >= '0' && c <= '9' || c == '.')
				i = lexNumber(expr, i, tokens);
			else if (Character.isLetter(c))
				i = lexIdent(expr, i, tokens);
			else
				i = lexSymbol(expr, i, tokens);
		}
		tokens.add(new Token(TokenType.END, ""));
		return tokens;
	}

	private int lexNumber(String s, int i, List<Token> out)
			throws IllegalDecimalException {
		int start = i, n = s.length();

		// Number starting with '.' is invalid (e.g. ".11" — missing integer part)
		if (s.charAt(i) == '.')
			throw new IllegalDecimalException();

		while (i < n && Character.isDigit(s.charAt(i))) i++;

		if (i < n && s.charAt(i) == '.') {
			i++;
			if (i >= n || !Character.isDigit(s.charAt(i)))
				throw new IllegalDecimalException();
			while (i < n && Character.isDigit(s.charAt(i))) i++;
		}

		if (i < n && (s.charAt(i) == 'e' || s.charAt(i) == 'E')) {
			i++;
			if (i < n && (s.charAt(i) == '+' || s.charAt(i) == '-')) i++;
			if (i >= n || !Character.isDigit(s.charAt(i)))
				throw new IllegalDecimalException();
			while (i < n && Character.isDigit(s.charAt(i))) i++;
		}

		// Trailing garbage: if next char is '.' or letter, the decimal is malformed
		// e.g. "3.3e3.3" or "12.3Emax" or "4.E+3"
		if (i < n && (s.charAt(i) == '.' || Character.isLetter(s.charAt(i))))
			throw new IllegalDecimalException();

		Token tok = new Token(TokenType.NUMBER, s.substring(start, i));
		tok.numVal = Double.parseDouble(tok.text);
		out.add(tok);
		return i;
	}

	private int lexIdent(String s, int i, List<Token> out)
			throws IllegalIdentifierException {
		int start = i, n = s.length();
		while (i < n && Character.isLetter(s.charAt(i))) i++;
		String word = s.substring(start, i);
		String lo = word.toLowerCase();

		if (lo.equals("true")) {
			Token tok = new Token(TokenType.BOOL, word);
			tok.boolVal = true; tok.numVal = 1.0; out.add(tok);
		} else if (lo.equals("false")) {
			Token tok = new Token(TokenType.BOOL, word);
			tok.boolVal = false; tok.numVal = 0.0; out.add(tok);
		} else if (lo.equals("sin") || lo.equals("cos") ||
		           lo.equals("max") || lo.equals("min")) {
			Token tok = new Token(TokenType.FUNC, word);
			tok.funcName = lo; out.add(tok);
		} else {
			throw new IllegalIdentifierException();
		}
		return i;
	}

	private int lexSymbol(String s, int i, List<Token> out)
			throws IllegalSymbolException {
		char c = s.charAt(i);
		switch (c) {
		case '+': out.add(new Token(TokenType.PLUS,  "+")); i++; break;
		case '*': out.add(new Token(TokenType.MUL,   "*")); i++; break;
		case '/': out.add(new Token(TokenType.DIV,   "/")); i++; break;
		case '^': out.add(new Token(TokenType.POWER, "^")); i++; break;
		case '&': out.add(new Token(TokenType.AND,   "&")); i++; break;
		case '|': out.add(new Token(TokenType.OR,    "|")); i++; break;
		case '!': out.add(new Token(TokenType.NOT,   "!")); i++; break;
		case '(': out.add(new Token(TokenType.LPAREN,"(")); i++; break;
		case ')': out.add(new Token(TokenType.RPAREN,")")); i++; break;
		case ',': out.add(new Token(TokenType.COMMA, ",")); i++; break;
		case '?': out.add(new Token(TokenType.QUEST, "?")); i++; break;
		case ':': out.add(new Token(TokenType.COLON, ":")); i++; break;
		case '-': out.add(new Token(TokenType.MINUS, "-")); i++; break;
		case '=': out.add(new Token(TokenType.EQ,    "=")); i++; break;
		case '<':
			i++;
			if (i < s.length() && s.charAt(i) == '=') {
				out.add(new Token(TokenType.LE, "<=")); i++;
			} else if (i < s.length() && s.charAt(i) == '>') {
				out.add(new Token(TokenType.NE, "<>")); i++;
			} else {
				out.add(new Token(TokenType.LT, "<"));
			}
			break;
		case '>':
			i++;
			if (i < s.length() && s.charAt(i) == '=') {
				out.add(new Token(TokenType.GE, ">=")); i++;
			} else {
				out.add(new Token(TokenType.GT, ">"));
			}
			break;
		default:
			throw new IllegalSymbolException();
		}
		return i;
	}

	// ══════════════════════════════════════════════════════════════════
	//  PARSER & EVALUATOR
	// ══════════════════════════════════════════════════════════════════

	private List<Token>      tokens;
	private int              pos;
	private Stack<Val>       valStack;
	private Stack<OpEntry>   opStack;
	private Stack<TernState> ternStack;

	public double calculate(String expression) throws ExpressionException {
		tokens    = lex(expression);
		pos       = 0;
		valStack  = new Stack<>();
		opStack   = new Stack<>();
		ternStack = new Stack<>();

		boolean expectOpnd = true;
		boolean sawComma   = false;

		while (pos < tokens.size()) {
			Token t = tokens.get(pos);

			switch (t.type) {

			case NUMBER:
				if (!expectOpnd) throw new MissingOperatorException();
				valStack.push(new Val(t.numVal, false));
				expectOpnd = false;
				sawComma = false;
				pos++;
				break;

			case BOOL:
				if (!expectOpnd) throw new MissingOperatorException();
				valStack.push(new Val(t.numVal, true));
				expectOpnd = false;
				sawComma = false;
				pos++;
				break;

			case FUNC:
				if (!expectOpnd) throw new MissingOperatorException();
				opStack.push(new OpEntry(t, false));
				expectOpnd = true;
				sawComma = false;
				pos++;
				if (pos >= tokens.size()
						|| tokens.get(pos).type != TokenType.LPAREN)
					throw new FunctionCallException();
				break;

			case LPAREN:
				if (!expectOpnd) throw new MissingOperatorException();
				if (!opStack.isEmpty()
						&& opStack.peek().tok.type == TokenType.FUNC)
					opStack.push(new OpEntry(t, false, valStack.size()));
				else
					opStack.push(new OpEntry(t, false));
				expectOpnd = true;
				sawComma = false;
				pos++;
				break;

			case RPAREN:
				if (expectOpnd) {
					// If a comma was just seen, the comma expected an operand
					if (sawComma) throw new MissingOperandException();
					if (!opStack.isEmpty()
							&& opStack.peek().tok.type == TokenType.LPAREN) {
						OpEntry lp = opStack.pop();
						if (!opStack.isEmpty()
								&& opStack.peek().tok.type == TokenType.FUNC) {
							OpEntry fe = opStack.pop();
							handleFunc(fe.tok, lp.valBase);
						} else {
							throw new EmptyExpressionException();
						}
					} else {
						throw new MissingOperandException();
					}
				} else {
					evalUntil(TokenType.LPAREN);
					if (opStack.isEmpty()
							|| opStack.peek().tok.type != TokenType.LPAREN)
						throw new MissingLeftParenthesisException();
					OpEntry lp = opStack.pop();
					if (!opStack.isEmpty()
							&& opStack.peek().tok.type == TokenType.FUNC) {
						OpEntry fe = opStack.pop();
						handleFunc(fe.tok, lp.valBase);
					}
				}
				expectOpnd = false;
				sawComma = false;
				pos++;
				break;

			case COMMA:
				if (expectOpnd) throw new MissingOperandException();
				evalUntil(TokenType.LPAREN);
				if (opStack.isEmpty()
						|| opStack.peek().tok.type != TokenType.LPAREN)
					throw new MissingLeftParenthesisException();
				expectOpnd = true;
				sawComma = true;
				pos++;
				break;

			case NOT:
				if (!expectOpnd) throw new MissingOperatorException();
				opStack.push(new OpEntry(t, false));
				expectOpnd = true;
				sawComma = false;
				pos++;
				break;

			case MINUS:
				if (expectOpnd) {
					opStack.push(new OpEntry(t, true));
					sawComma = false;
				} else {
					evalBeforePush(t, false);
					opStack.push(new OpEntry(t, false));
					expectOpnd = true;
				}
				pos++;
				break;

			case PLUS: case MUL: case DIV: case POWER:
			case EQ: case NE: case LT: case LE: case GT: case GE:
			case AND: case OR:
				if (expectOpnd) throw new MissingOperandException();
				evalBeforePush(t, false);
				opStack.push(new OpEntry(t, false));
				expectOpnd = true;
				pos++;
				break;

			case QUEST:
				if (expectOpnd) throw new MissingOperandException();
				evalBeforePush(t, false);
				{
					Val c = valStack.pop();
					ternStack.push(new TernState(c));
				}
				opStack.push(new OpEntry(t, false));
				expectOpnd = true;
				pos++;
				break;

			case COLON:
				if (expectOpnd) throw new MissingOperandException();
				evalUntil(TokenType.QUEST);
				if (opStack.isEmpty()
						|| opStack.peek().tok.type != TokenType.QUEST)
					throw new TrinaryOperationException();
				opStack.pop();
				{
					Val tv = valStack.pop();
					TernState ts = ternStack.pop();
					ts.trueBranch = tv;
					ternStack.push(ts);
				}
				opStack.push(new OpEntry(t, false));
				expectOpnd = true;
				pos++;
				break;

			case END:
				if (expectOpnd) {
					if (valStack.isEmpty() && opStack.isEmpty())
						throw new EmptyExpressionException();
					throw new MissingOperandException();
				}
				while (!opStack.isEmpty()) {
					OpEntry oe = opStack.pop();
					if (oe.tok.type == TokenType.LPAREN)
						throw new MissingRightParenthesisException();
					evalOp(oe);
				}
				pos++;
				break;

			default:
				pos++;
			}
		}

		if (valStack.size() != 1)
			throw new MissingOperatorException();

		Val result = valStack.pop();
		if (result.isBool)
			throw new TypeMismatchedException();
		return result.v;
	}

	// ── OPP stack helpers ───────────────────────────────────────────

	private void evalUntil(TokenType marker) throws ExpressionException {
		while (!opStack.isEmpty()) {
			TokenType top = opStack.peek().tok.type;
			if (top == marker) break;
			if (top == TokenType.LPAREN) break;
			OpEntry oe = opStack.pop();
			evalOp(oe);
		}
	}

	private void evalBeforePush(Token incoming, boolean isUnary)
			throws ExpressionException {
		int pIn = prec(incoming.type, isUnary);
		while (!opStack.isEmpty()) {
			OpEntry top = opStack.peek();
			TokenType tt = top.tok.type;
			if (tt == TokenType.LPAREN || tt == TokenType.QUEST) break;
			int pTop = prec(tt, top.isUnary);
			if (rightAssoc(incoming.type)) {
				if (pTop <= pIn) break;
			} else {
				if (pTop < pIn) break;
			}
			opStack.pop();
			evalOp(top);
		}
	}

	// ── Semantic actions ────────────────────────────────────────────

	private void evalOp(OpEntry oe) throws ExpressionException {
		Token op = oe.tok;

		// Ternary ':' completion
		if (op.type == TokenType.COLON) {
			Val falseVal = valStack.pop();
			TernState ts = ternStack.pop();
			if (!ts.cond.isBool) throw new TypeMismatchedException();
			Val res = (ts.cond.v != 0) ? ts.trueBranch : falseVal;
			valStack.push(res);
			return;
		}

		// Unary minus
		if (op.type == TokenType.MINUS && oe.isUnary) {
			Val a = valStack.pop();
			if (a.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(-a.v, false));
			return;
		}

		// Unary NOT
		if (op.type == TokenType.NOT) {
			Val a = valStack.pop();
			if (!a.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v != 0 ? 0.0 : 1.0, true));
			return;
		}

		// Binary operators
		Val b = valStack.pop();
		Val a = valStack.pop();

		switch (op.type) {
		case PLUS:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v + b.v, false));
			break;
		case MINUS:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v - b.v, false));
			break;
		case MUL:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v * b.v, false));
			break;
		case DIV:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			if (b.v == 0.0) throw new DividedByZeroException();
			valStack.push(new Val(a.v / b.v, false));
			break;
		case POWER:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(Math.pow(a.v, b.v), false));
			break;
		case EQ:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v == b.v ? 1.0 : 0.0, true));
			break;
		case NE:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v != b.v ? 1.0 : 0.0, true));
			break;
		case LT:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v < b.v ? 1.0 : 0.0, true));
			break;
		case LE:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v <= b.v ? 1.0 : 0.0, true));
			break;
		case GT:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v > b.v ? 1.0 : 0.0, true));
			break;
		case GE:
			if (a.isBool || b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val(a.v >= b.v ? 1.0 : 0.0, true));
			break;
		case AND:
			if (!a.isBool || !b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val((a.v != 0 && b.v != 0) ? 1.0 : 0.0, true));
			break;
		case OR:
			if (!a.isBool || !b.isBool) throw new TypeMismatchedException();
			valStack.push(new Val((a.v != 0 || b.v != 0) ? 1.0 : 0.0, true));
			break;
		default:
			break;
		}
	}

	private void handleFunc(Token func, int base) throws ExpressionException {
		String name = func.funcName;
		int nArgs = valStack.size() - base;

		if (name.equals("sin") || name.equals("cos")) {
			if (nArgs < 1) throw new MissingOperandException();
			if (nArgs > 1) throw new FunctionCallException();
			Val a = valStack.pop();
			if (a.isBool) throw new TypeMismatchedException();
			double r = name.equals("sin") ? Math.sin(a.v) : Math.cos(a.v);
			valStack.push(new Val(r, false));
		} else { // max or min
			if (nArgs < 2) throw new MissingOperandException();
			// Args are on stack in order [..., arg1, arg2, ... argN]
			// Pop them in reverse
			double[] args = new double[nArgs];
			for (int i = nArgs - 1; i >= 0; i--) {
				Val a = valStack.pop();
				if (a.isBool) throw new TypeMismatchedException();
				args[i] = a.v;
			}
			double result = args[0];
			if (name.equals("max")) {
				for (int i = 1; i < nArgs; i++)
					if (args[i] > result) result = args[i];
			} else {
				for (int i = 1; i < nArgs; i++)
					if (args[i] < result) result = args[i];
			}
			valStack.push(new Val(result, false));
		}
	}
}
