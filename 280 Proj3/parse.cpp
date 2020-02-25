/*
 * parse.cpp
 */

#include "parse.h"
#include <iostream>

namespace Parser {
	bool pushed_back = false;
	Lex	pushed_token;

	static Lex GetNextToken(istream& in, int& line) {
		if( pushed_back ){
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(Lex& t) {
		if( pushed_back ) {
			abort();
		}

		pushed_back = true;
		pushed_token = t;
	}
}

static int error_count = 0;

void ParseError(int line, string msg){
	++error_count;
	cout << line << ": " << msg << endl;
}
/*
Prog := Slist
Slist := SC { Slist } | Stmt SC { Slist }
Stmt := IfStmt | PrintStmt | LetStmt | LoopStmt
IfStmt := IF Expr BEGIN Slist END
LetStmt := LET ID Expr
LoopStmt := LOOP Expr BEGIN Slist END
PrintStmt := PRINT Expr
Expr := Prod { (PLUS | MINUS) Prod }
Prod := Rev { (STAR | SLASH) Rev }
Rev := BANG Rev | PRIMARY
Primary := ID | INT | STR | LPAREN Expr RPAREN
*/

//Prog := Slist
ParseTree *Prog(istream& in, int& line){

	ParseTree *sl = Slist(in, line);

	if( sl == 0 )
		ParseError(line, "No statements in program");

	//If there is an error, it is a bad program with no parse tree.
	if( error_count )
		return 0;

	return sl;
}

// Slist is a Statement followed by a Statement List
//Slist := SC { Slist } | Stmt SC { Slist }
ParseTree *Slist(istream& in, int& line) {		//Finished?
	Lex t = Parser::GetNextToken(in, line);
	ParseTree *s;

	if(t != SC){
		Parser::PushBackToken(t);
		s = Stmt(in, line);

		if( s == 0 ){
			return 0;
		}

		t = Parser::GetNextToken(in, line);
		if(t != SC){
			ParseError(line, "Expected ';' before " + t.GetLexeme() + " token");
			return 0;
		}
	}

	return new StmtList(s, Slist(in,line));
}

//Stmt := IfStmt | PrintStmt | LetStmt | LoopStmt
ParseTree *Stmt(istream& in, int& line) {
	Lex t = Parser::GetNextToken(in, line);

	if(t == IF){
		return IfStmt(in, line);
	}
	else if(t == PRINT){
		Parser::PushBackToken(t);
		return PrintStmt(in, line);
	}
	else if(t == LET){
		return LetStmt(in, line);
	}
	else if(t == LOOP){
		return LoopStmt(in, line);
	}
	else if(t == END){
		Parser::PushBackToken(t);
		return 0;
	}
	else if(t != DONE){
		ParseError(line, "Expected 'if', 'print', 'let', or 'loop'");
		return 0;
	}
	return 0;
}


//LetStmt := LET ID Expr
ParseTree *LetStmt(istream& in, int& line) {	
	Lex t = Parser::GetNextToken(in, line);

	if(t == ID){
		ParseTree *p = Expr(in, line);
		if(p == 0){
			ParseError(line, "Missing expression after identifier");
			return 0;
		}
		return new Let(t, p);
	}
	else{
		ParseError(line, "Missing identifier after LET");
		return 0;
	}
}

//PrintStmt := PRINT Expr
ParseTree *PrintStmt(istream& in, int& line) {
	Lex t = Parser::GetNextToken(in, line);
	ParseTree *p = Expr(in, line);
	if(!p){
		ParseError(line, "Missing expression after 'print' token");
		return 0;
	}
	return new Print(t, p);
}

//LoopStmt := LOOP Expr BEGIN Slist END
ParseTree *LoopStmt(istream& in, int& line) {
	ParseTree *expr, *slist;

	expr = Expr(in, line);
	if(expr){
		Lex t = Parser::GetNextToken(in, line);
		if(t == BEGIN){
			slist = Slist(in, line);
			if(slist){
				t = Parser::GetNextToken(in, line);
				if(t == END){
					return new Loop(t, expr, slist);
				}
				else{
					ParseError(line, "Expected 'end' token after statement list");
				}
			}
			else{
				ParseError(line, "Missing statment list after 'begin' token");
			}
		}
		else{
			ParseError(line, "Expected 'begin' token");
		}
	}
	else{
		ParseError(line, "Missing expression after 'loop' token");
	}
	return 0;
}

//IfStmt := IF Expr BEGIN Slist END
ParseTree *IfStmt(istream& in, int& line) {
	ParseTree *expr, *slist;

	expr = Expr(in, line);
	if(expr){
		Lex t = Parser::GetNextToken(in, line);
		if(t == BEGIN){
			slist = Slist(in, line);
			if(slist){
				t = Parser::GetNextToken(in, line);
				if(t == END){
					return new If(t, expr, slist);
				}
				else{
					ParseError(line, "Expected 'end' token after statement list");
				}
			}
			else{
				ParseError(line, "Missing statment list after 'begin' token");
			}
		}
		else{
			ParseError(line, "Expected 'begin' token");
		}
	}
	else{
		ParseError(line, "Missing expression after 'if' token");
	}
	return 0;
}


//Expr := Prod { (PLUS | MINUS) Prod }
ParseTree *Expr(istream& in, int& line) {
	ParseTree *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Lex t = Parser::GetNextToken(in, line);

		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS )
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
}

//Prod := Rev { (STAR | SLASH) Rev }
ParseTree *Prod(istream& in, int& line) {
	ParseTree *p1 = Rev(in, line);

	if(p1 == 0){
		return 0;
	}

	while ( true ) {
		Lex t = Parser::GetNextToken(in, line);

		if( t != STAR && t != SLASH ) {
			Parser::PushBackToken(t);
			return p1;
		}

		ParseTree *p2 = Rev(in, line);
		if( p2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == STAR )
			p1 = new PlusExpr(t.GetLinenum(), p1, p2);
		else
			p1 = new MinusExpr(t.GetLinenum(), p1, p2);
	}

}


//Rev := BANG Rev | PRIMARY
ParseTree *Rev(istream& in, int& line) {
	Lex t = Parser::GetNextToken(in, line);

	if(t == BANG){
		return new BangExpr(t, Rev(in, line));
	}
	Parser::PushBackToken(t);

	return Primary(in, line);
}


//Primary := ID | INT | STR | LPAREN Expr RPAREN
ParseTree *Primary(istream& in, int& line) {
	Lex t = Parser::GetNextToken(in, line);

	if( t == ID ) {
		return new Ident(t);
	}
	else if( t == INT ) {
		return new IConst(t);
	}
	else if( t == STR ) {
		return new SConst(t);
	}
	else if( t == LPAREN ) {
		ParseTree *ex = Expr(in, line);
		if( ex == 0 ) {
			ParseError(line, "Missing expression after (");
			return 0;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;

		ParseError(line, "Missing ) after expression");
		return 0;
	}

	ParseError(line, "Primary expected");
	return 0;
}

