#include "Fact.cpp"
#include "Rule.cpp"
#include "Manipulator.cpp"
using namespace std;

void LineIn(Manipulator *M, string str, bool *done){
		string command;
		string rest;
		string k;
		string s;
		ofstream fstor;//create output file
		stringstream iss(str);//create ss object
		getline(iss, command, ' ');//parse first input for command
		getline(iss, k);//parses rest of string and stores in rest
		char c = str[2];
		switch(c){
		case 'L':	// if(command == "RULE")
				cout << "Rule" << k << endl;
				fstor.open("write.txt");
				s = "write.txt";
				fstor << k;//puts rest in file
				fstor.close();
				M->load(s);//calls load to check if R or F and puts data where it should go
			break;
		case 'C':	// if(command == "FACT")
				std::cout << "Fact " << k << endl;
				fstor.open("write.txt");
				s = "write.txt";
				fstor << k;
				fstor.close();
				M->load(s);
			break;
		case 'A':	// if(command == "LOAD")
				M->load(k);
				cout << "Loaded" << endl;
			break;
		case 'O':	// if(command == "DROP")
				M->drop(k);
				cout << "Dropped" << endl;
			break;
		case 'F':	// if(command == "INFERENCE")
				fstor.open("write.txt");
				s = "write.txt";
				fstor << k;
				fstor.close();
				M->inference(s);
				cout << "~query issued~" <<endl;
			break;
		case 'M':	// if(command == "DUMP")
				M->dump();
				cout << "~KB and RB dumped~" << endl;
			break;
		case 'I':	// if(command == "EXIT")
			*done = true;
			break;
		default: 
				cout << "not a valid command" << endl;
			break;
		}
}

int main(int argc, char *argv[]){
	Manipulator M;
	bool done = false;
	string welcome = "\n-----------Welcome to the SIMPLE RULE-BASE INFERENCE ENGINE!------------\n\n"
			"Let's get started.\n" 
			"Below are a list of user commands & what they do.\n\n"
			"	LOAD - type the file, fact or rule you'd like to load\n"
			"	FACT - define a fact to add to the Knowlegde Base (KB)\n"
			"	RULE - define a rule to add to the Rule Base (RB) \n"
			"	INFERENCE - issue a query\n"
			"	DROP - used to delete a fact from the KB and RB\n"
			"	DUMP - prints the contents of KB and RB to an .sri file\n"
			"	EXIT - exit the SRI\n";
	cout << welcome <<endl;
	if(argc == 2){
		string line;
		ifstream readInput;
		readInput.open(argv[1]);
		while(getline(readInput,line)){
			LineIn(&M, line, &done);
		}
	}
	done = false;
	while(!done){
		string inp;
		cout << "Type a command: " << endl;
		getline(cin, inp);
		LineIn(&M, inp, &done);
	}

	return 0;
}
	

