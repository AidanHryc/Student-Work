#include "parse.h"
#include <fstream>

int main(int argc, char *argv[]){
	int n = 1;
	int &lineNum = n;
	ifstream file;
	ifstream &f = file;
	istream *stream;
	switch(argc){
		case 1:
			stream = &cin;
			break;
		case 2:
			f.open(argv[1]);
			if(!f){
				cout << "COULD NOT OPEN " << (argv[1]) << endl;
				exit(0);
			}
			stream = &f;
			break;
		default:
			cout << "TOO MANY FILENAMES" << endl;
			exit(0);
	}


	ParseTree *s = Prog(*stream, lineNum);

	if(s){
		s->checkId();
		int bangs = s->BangCount();
		cout << "BANG COUNT: " << bangs << endl;
		int depth = s->MaxDepth(s);
		cout << "MAX DEPTH: " << depth << endl;
	}

}
