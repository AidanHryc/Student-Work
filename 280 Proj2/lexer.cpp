#include "lex.h"
#include <iostream>
#include <string>
#include <fstream>

Lex getNextToken(istream& in, int& linenum){
	string tempStr = "";
	bool inState = false;
	int tok = 0;
	char ch = '\0';
	while(in.good()){
		ch = in.get();
		if(!inState){  // initial check of the character

		}
		else{	// All the states of the machine will be here

		}
	}
	return Lex(ERR, "", 0); //Placeholder
}


