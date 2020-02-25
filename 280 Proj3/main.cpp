#include "parse.h"
#include <fstream>
#include <sstream>

int main(int argc, char *argv[]){
	int n = 0;
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

	bool isErr = false;
	ParseTree *s = Prog(*stream, lineNum);
	map<string, bool> id_map;
	map<string, Val> symbols;

	if(s){
		isErr = s->CheckLetBeforeUse(id_map);

		if(!isErr)
			exit(0);

		try{
			s->Eval(symbols);
		}
		catch(runtime_error e){
			cout << e.what() << endl;
		}
	}

}

void runtime_err(int lineNum, string msg){
	std::ostringstream strBuild;
	strBuild << "RUNTIME ERROR at " << lineNum << ": " << msg;
	throw runtime_error(strBuild.str());
}