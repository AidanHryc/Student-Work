/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
#include "val.h"
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE , INTTYPE, STRTYPE };

// a "forward declaration" for a class to hold values
class Value;

// Declaring the runtime_err method as an extern and implementing it in main
extern void runtime_err(int lineNum, string msg);

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
	virtual Val Eval(map<string,Val>& syms) const = 0;

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

	virtual bool CheckLetBeforeUse(map<string, bool>& id_map){
		//This recursively adds Idents to the boolean map
		//This also checks if any Idents have not been initialized
		bool err = true;
		bool L = true;
		bool R = true;
		if(IsLet()){
			//This checks if an undeclared ident is being defined by an undeclared ident
			err = left->CheckLetBeforeUse(id_map);
			id_map[GetId()] = 1;

		}
		if(IsIdent()){
			if(!id_map[GetId()]){
				err = false;
				cout << "UNDECLARED VARIABLE " << GetId() << endl;
			}
		}
		if(left)
			L = left->CheckLetBeforeUse(id_map);
		if(right)
			R = right->CheckLetBeforeUse(id_map);
		return (L & R & err);
	}

};

//****STMTLIST****
class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}

	Val Eval(map<string,Val>& symbols) const {
        left->Eval(symbols);
        if( right )
            right->Eval(symbols);
        return Val();
    }
};


//****LET****
class Let : public ParseTree {
	string id;

public:
	Let(Lex& t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {
	}

	string GetId() const { return id; }
	bool IsLet() const { return true; }


	Val Eval(map<string,Val>& symbols) const {
		auto L = left->Eval(symbols);
		symbols[id] = L;
		return Val();
    }

};


//****PRINT****
class Print : public ParseTree {
	string id;
public:
	Print(Lex& t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}

	Val Eval(map<string,Val>& symbols) const {
		auto L = left->Eval(symbols);
		if(L.isErr()){
			cout << "ERROR" << endl;
		}
		if(L.isInt()){
			cout << L.ValInt();
		}
		else{
			cout << L.ValString();
		}
		return Val();
    }
};

//****LOOP****
class Loop : public ParseTree {
public:
	Loop(Lex& beg, ParseTree *l, ParseTree *r) : ParseTree(beg.GetLinenum(), l, r) {}

	Val Eval(map<string,Val>& symbols) const{
		auto L = left->Eval(symbols);
		if(L.isInt()){
			while(L.ValInt()){
				right->Eval(symbols);
				L = left->Eval(symbols);
			}
		}
		else{
			runtime_err(left->GetLineNumber(), "Expression is not an integer");
		}
		return Val();
	}
};

//****IF****
class If : public ParseTree {
public:
	If(Lex& beg, ParseTree *l, ParseTree *r) : ParseTree(beg.GetLinenum(), l, r) {}

	Val Eval(map<string,Val>& symbols) const {
		auto L = left->Eval(symbols);
		if(L.isStr())
			runtime_err(left->GetLineNumber(), "Expression is not an integer");
		if(L.isInt()){
			if(L.ValInt()){
				right->Eval(symbols);
			}
		}
		return Val();
    }
};

//****PLUSEXPR****
class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	Val Eval(map<string,Val>& symbols) const {
        auto L = left->Eval(symbols);
        if( L.isErr() )
            runtime_err(linenum, L.GetErrMsg());
        auto R = right->Eval(symbols);
        if( R.isErr() )
            runtime_err(linenum, R.GetErrMsg());

        auto answer = L + R;
        if( answer.isErr() )
            runtime_err(linenum, answer.GetErrMsg());

        return answer;
    }

};

//****MINUSEXPR****
class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	Val Eval(map<string,Val>& symbols) const {
        auto L = left->Eval(symbols);
        if( L.isErr() )
            runtime_err(linenum, L.GetErrMsg());
        auto R = right->Eval(symbols);
        if( R.isErr() )
            runtime_err(linenum, R.GetErrMsg());

        auto answer = L - R;
        if( answer.isErr() )
            runtime_err(linenum, answer.GetErrMsg());

        return answer;
    }

};

//****TIMESEXPR****
class TimesExpr : public ParseTree {
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	Val Eval(map<string,Val>& symbols) const {
		auto L = left->Eval(symbols);
		if( L.isErr() )
			runtime_err(linenum, L.GetErrMsg());
		auto R = right->Eval(symbols);
		if( R.isErr() )
			runtime_err(linenum, R.GetErrMsg());

		auto answer = L * R;
		if( answer.isErr() )
			runtime_err(linenum, answer.GetErrMsg());

		return answer;
	}

};

//****DIVIDEEXPR****
class DivideExpr : public ParseTree {
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	Val Eval(map<string,Val>& symbols) const {
        auto L = left->Eval(symbols);
        if( L.isErr() )
            runtime_err(linenum, L.GetErrMsg());
        auto R = right->Eval(symbols);
        if( R.isErr() )
            runtime_err(linenum, R.GetErrMsg());

        auto answer = L / R;
        if( answer.isErr() )
            runtime_err(linenum, answer.GetErrMsg());

        return answer;
    }

};

//****BANGEXPR****
class BangExpr : public ParseTree {
	string id;

public:
	BangExpr(Lex& t, ParseTree *e)
		: ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {
	}

	int IsBang() const { return 1; }

	Val Eval(map<string,Val>& symbols) const {
		auto L = left->Eval(symbols);
		if( L.isErr() )
            runtime_err(linenum, L.GetErrMsg());
		auto answer = !L;
        return answer;
    }

};

//****ICONST****
class IConst : public ParseTree {
	int val;

public:
	IConst(Lex& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}

	Val Eval(map<string,Val>& symbols) const {
        return Val(val);
    }

};

//****SCONST****
class SConst : public ParseTree {
	string val;

public:
	SConst(Lex& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}


 	Val Eval(map<string,Val>& symbols) const {
        return Val(val);
    }

};

//****IDENT****
class Ident : public ParseTree {
	string id;

public:
	Ident(Lex& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
	string GetId() const { return id; }


	Val Eval(map<string,Val>& symbols) const {
		return symbols[id];
    }

};

#endif /* PARSETREE_H_ */
