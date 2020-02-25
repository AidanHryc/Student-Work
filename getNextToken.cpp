#include "lex.h"

Lex getNextToken(istream& in, int& linenum){
	string lexeme = "";
	bool inState = false;
	int tok = 0;
	char ch = '\0';
    while(in.get(ch)){
		if(!inState){         // Checks if the program is in a current state
			if(ch == '\n'){
				linenum++;
			}
			if(isspace(ch)){
				continue;
			}
			if(ch == '+'){
				return Lex(PLUS, "+", linenum);
			}
			else if(ch == '*'){
				return Lex(STAR, "*", linenum);
			}
			else if(ch == '!'){
				return Lex(BANG, "!", linenum);
			}
			else if(ch == '('){
				return Lex(LPAREN, "(", linenum);
			}
			else if(ch == ')'){
				return Lex(RPAREN, ")", linenum);
			}
			else if(ch == ';'){
				return Lex(SC, ";", linenum);
			}
			else if(ch == '-'){
				return Lex(MINUS, "-", linenum);
			}
			else if(ch == '/'){
				if(in.peek() == '/'){
					inState = true;
					tok = STAR; //this goes to the comment state
				}
				else{
					return Lex(SLASH, "/", linenum);
				}
			}
			else if(isdigit(ch)){
				lexeme += ch;
				inState = true;
				tok = INT;
			}
			else if(isalpha(ch)){
				lexeme += ch;
				inState = true;
				tok = ID;
			}
			else if(ch == '"'){ //String
				inState = true;
				tok = STR;
			}
			else if(ch == EOF){
				return Lex(DONE, "", linenum);
			}
			else{
				lexeme += ch;
                linenum++;
				return Lex(ERR, lexeme, linenum);
			}
		}
		else{	// All the states of the machine will be here
			switch(tok){
				case INT:
					if(isdigit(ch)){
						lexeme += ch;
					}
					else if(ch == '\n'){
						linenum++;
						return Lex(INT, lexeme, linenum);
					}
					else{
						in.putback(ch);
						return Lex(INT, lexeme, linenum);
					}
					break;
				case ID:
					if(lexeme == "print"){	//	PRINT, LET, IF, LOOP, BEGIN, END,
						in.putback(ch);
						return Lex(PRINT, lexeme, linenum);
					}
					if(lexeme == "let"){
						in.putback(ch);
						return Lex(LET, lexeme, linenum);
					}
					if(lexeme == "if"){
						in.putback(ch);
						return Lex(IF, lexeme, linenum);
					}
					if(lexeme == "loop"){
						in.putback(ch);
						return Lex(LOOP, lexeme, linenum);
					}
					if(lexeme == "begin"){
						in.putback(ch);
						return Lex(BEGIN, lexeme, linenum);
					}
					if(lexeme == "end"){
						in.putback(ch);
						return Lex(END, lexeme, linenum);
					}
					if(isalnum(ch)){
						lexeme += ch;
					}
					else if(ch == '\n'){
						linenum++;
						return Lex(ID, lexeme, linenum);
					}
					else{
						in.putback(ch);
						return Lex(ID, lexeme, linenum);
					}
					break;
				case STR:
					if(ch == '\n'){
						lexeme += '\n';
						lexeme.insert(0,"\"");
                        linenum++;
						return Lex(ERR, lexeme, linenum);
					}
					else if(ch == '\\'){
						tok = SLASH;
					}
					else if(ch == '"'){
						return Lex(STR, lexeme, linenum);
					}
					else if(in.peek() == EOF){
						lexeme += ch;
						lexeme.insert(0,"\"");
                        linenum++;
						return Lex(ERR, lexeme, linenum);
					}
					else{
						lexeme += ch;
					}
					break;
				case SLASH:
					if(ch == 'n'){
						lexeme += '\n';
					}
					else{
						lexeme += ch;
					}
					tok = STR;
					break;
				case STAR: //This is the comment state
					if(ch == '\n'){
						linenum++;
						inState = false;
					}
			}
		}
	}
	return Lex(DONE, "", linenum);
}