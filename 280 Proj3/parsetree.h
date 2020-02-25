/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE , INTTYPE, STRTYPE };

// a "forward declaration" for a class to hold values
class Value;

//****PARSETREE****
class ParseTree {
protected:
	int 		linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree(){
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }

	int MaxDepth(ParseTree *root) {
		//Implement a recursive depth checker
		if(root == 0) {
		    return 0;
	    }
		if(root->left == 0 && root->right == 0){
			return 1;
		}
	    else{
	   		int l = MaxDepth(root->left);
	    	int r = MaxDepth(root->right);

			return (1 + ((l > r)? l : r));
	   	}
	}

	virtual bool IsIdent() const { return false; }
	virtual bool IsVar() const { return false; }
	virtual string GetId() const { return ""; }
    virtual int IsBang() const { return 0; }
    virtual bool IsLet() const { return false; }

	int BangCount() const {
		//This recursively checks the number of bang nodes
		int count = 0;
		if(left)
			count += left->BangCount();
		if(right)
			count += right->BangCount();
		if(IsBang())
			return count + 1;
		return count;
	}

	void checkId(){
		//This recursively adds Idents to the boolean map
		//This also checks if any Idents have not been initialized
		static map<string, bool> id_map;
		if(IsLet()){
			id_map[GetId()] = 1;
		}
		if(IsIdent()){
			if(id_map[GetId()] == 0){
				cout << "UNDECLARED VARIABLE " << GetId() << endl;
			}
		}
		if(left)
			left->checkId();
		if(right)
			right->checkId();
		return;
	}


	virtual void CheckLetBeforeUse(map<string,bool>& var) {}


};

//****STMTLIST****
class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
};


//****LET****
class Let : public ParseTree {
	string id;

public:
	Let(Lex& t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {
	}

	string GetId() const { return id; }
	bool IsLet() const { return true; }
};


//****PRINT****
class Print : public ParseTree {
	string id;
public:
	Print(Lex& t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}
};

//****LOOP****
class Loop : public ParseTree {
public:
	Loop(Lex& beg, ParseTree *l, ParseTree *r) : ParseTree(beg.GetLinenum(), l, r) {}
};

//****IF****
class If : public ParseTree {
public:
	If(Lex& beg, ParseTree *l, ParseTree *r) : ParseTree(beg.GetLinenum(), l, r) {}
};

//****PLUSEXPR****
class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

//****MINUSEXPR****
class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

//****TIMESEXPR****
class TimesExpr : public ParseTree {
};

//****DIVIDEEXPR****
class DivideExpr : public ParseTree {
};

//****BANGEXPR****
class BangExpr : public ParseTree {
	string id;

public:
	BangExpr(Lex& t, ParseTree *e)
		: ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {
	}

	int IsBang() const { return 1; }
};

//****ICONST****
class IConst : public ParseTree {
	int val;

public:
	IConst(Lex& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
};

//****SCONST****
class SConst : public ParseTree {
	string val;

public:
	SConst(Lex& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}
};

//****IDENT****
class Ident : public ParseTree {
	string id;

public:
	Ident(Lex& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
	string GetId() const { return id; }
};

#endif /* PARSETREE_H_ */
