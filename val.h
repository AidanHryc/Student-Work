/*
    val.h
*/

#ifndef VAL_H
#define VAL_H

#include <string>

using namespace std;

class Val {
    int i;
    string s;
    enum ValType { ISINT, ISSTR, ISERR } vt;

public:
    Val() : i(0), vt(ISERR) {}
    Val(int i) : i(i), vt(ISINT) {}
    Val(string s) : i(0), s(s), vt(ISSTR) {}
    Val(ValType vt, string errmsg) : i(0), s(errmsg), vt(ISERR) {}

    ValType getVt() const { return vt; }

    bool isErr() const { return vt == ISERR; }
    bool isInt() const { return vt == ISINT; }
    bool isStr() const { return vt == ISSTR; }

    int ValInt() const {
        if( isInt() ) return i;
        throw "This Val is not an Int";
    }
    string ValString() const {
        if( isStr() ) return s;
        throw "This Val is not a Str";
    }

    friend ostream& operator<<(ostream& out, const Val& v) {
        if(v.isInt())
            out << v.ValInt();
        else if(v.isStr())
            out << v.ValString();
        return out;
    }

    string GetErrMsg() const {
        if( isErr() ) return s;
        throw "This Val is not an Error";
    }

    Val operator+(const Val& op) const {
        if(isInt() && op.isInt())
            return ValInt() + op.ValInt();
        if(isStr() && op.isStr())
            return ValString() + op.ValString();
        return Val(ISERR, "Type mismatch on operands of +");
    }

    Val operator-(const Val& op) const {
        if(isInt() && op.isInt())
            return ValInt() - op.ValInt();
        return Val(ISERR, "Type mismatch on operands of -");
    }

    Val operator*(const Val& op) const {
        int num = 0;
        string str = "";
        string accum = "";
        if(isInt() && op.isInt()){
            return ValInt() * op.ValInt();
        }
        if((isStr() && op.isInt()) || (isInt() && op.isStr())){
            if(isInt()){
                if(ValInt() < 0)
                    return Val(ISERR, "Cannot multiply string by negative int");
                num = ValInt();
                str = op.ValString();
            }
            else if(isStr()){
                if(op.ValInt() < 0)
                    return Val(ISERR, "Cannot multiply string by negative int");
                str = ValString();
                num = op.ValInt();
            }
            if(!num){
                return accum;
            }
            for(int i = 0; i < num; i++)
                accum+=str;
            return accum;
        }
        return Val(ISERR, "Type mismatch on operands of *");
    }

    Val operator/(const Val& op) const {
        if(isInt() && op.isInt()){
            if(op.ValInt())
                return ValInt() / op.ValInt();
            return Val(ISERR, "Divide by zero error");
        }
        return Val(ISERR, "Type mismatch on operands of /");
    }

    Val operator!() const {
        int tot = 0;
        int val = 0;
        string accm = "";
        if(isInt()){
            val = ValInt();
    		while(val!=0){
    			tot *= 10;
    			tot += val%10;
    			val = val/10;
    		}
    		return tot;
        }
        if(isStr()){
        	for(int i = s.length()-1; i >= 0; i--){
        		accm += s[i];
        	}
            return accm;
        }
        return Val(ISERR, "Type mismatch on operands of !");
    }
};

#endif