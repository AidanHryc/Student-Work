//Aidan Hryc
//CS 280
#include "lex.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <set>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]){
	vector<int> commands;
	ifstream file;
	ifstream &f = file;
	int line = 0;
	int& lineNum = line;
	bool fileName = false;
	bool verbose = false;
	istream *stream;

	for(int i=1; i < argc; i++){
		if(strcmp(argv[i], "-v") == 0){
			commands.push_back(0);
			verbose = true;
		}
		else if(strcmp(argv[i], "-consts") == 0){
			commands.push_back(1);
		}
		else if(strcmp(argv[i], "-ids") == 0){
			commands.push_back(2);
		}
		else if(argv[i][0] == '-'){
			cout << "UNRECOGNIZED FLAG " << argv[i] << endl;
			exit(0);
		}
		else{
			f.open(argv[i]);
			if(fileName){
				cout << "ONLY ONE FILE NAME ALLOWED" << endl;
				exit(0);
			}
			fileName = true;
		}
	}
	if(!f && fileName){
		cout << "CANNOT OPEN " << (argv[argc-1]) << endl;
		exit(0);
	}

	stream = fileName? &f : &cin; //Choosing stream to put in the istream pointer

	Lex lex;
	Token tok;
	string lexeme;
	string temp;
	set<string> strSet;
	set<int> intSet;
	set<string> idSet;
	int tokenCount = 0;
	const char *token_str[]={"PRINT","LET","IF","LOOP","BEGIN","END","ID","INT","STR",
 							  "PLUS","MINUS","STAR","SLASH","BANG","LPAREN","RPAREN","SC",
						      "ERR","DONE"};

	while((lex = getNextToken(*stream, lineNum)) != ERR && lex != DONE){
		tok = lex.GetToken();
		lexeme = lex.GetLexeme();
		tokenCount++;

		if(verbose){
			if(tok <= 5 || (9 <= tok && tok <= 16)){
				cout << token_str[tok] << endl;
			}
			else{
				cout << token_str[tok] << "(" << lexeme << ")" << endl;
			}
		}

		if(tok == 8){ //Only putting unique lexemes in the sets
			strSet.insert(lexeme);
		}
		else if(tok == 7){
			intSet.insert(stoi(lexeme));
		}
		else if(tok == 6){
			idSet.insert(lexeme);
		}
	}

	if(lex == ERR){
		cout << "Error on line " << lineNum << " (" << lex.GetLexeme()  << ")" << endl;
		exit(0);
	}

	for(int i=0; i < commands.size(); i++){
		switch(commands[i]){
			case 1:	// For the -consts flag
				if(!strSet.empty()){
					cout << "STRINGS:" << endl;
					for(string x : strSet){
						cout << x << endl;
					}
				}
				if(!intSet.empty()){
					cout << "INTEGERS:" << endl;
					for(int x : intSet){
						cout << x << endl;
					}
				}
				break;
			case 2: // For the -ids flag
				int index = 0;
				if(!idSet.empty()){
					cout << "IDENTIFIERS: ";
					for(string x : idSet){
						index++;
						if(index == idSet.size()){
							cout << x << endl;
						}
						else{
							cout << x << ", ";
						}
					}
				}
		}
	}

	if(lineNum == 0){
		cout << "Lines: " << 0 << endl;
	}
	else{
		cout << "Lines: " << lineNum << endl;
		cout << "Tokens: " << tokenCount << endl;;
	}
}

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