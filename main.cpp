#include "Fact.cpp"
#include "Rule.cpp"
#include "Manipulator.cpp"
using namespace std;
int main(){
	Manipulator M;
	// load facts and rules from file "input.txt"
	//M.load();
	// dump facts and rules to   file "output.txt"
	//M.dump();
	//query(M.Fact_map,"Father");
	//query(M.Fact_map,"Mother");

	string str;
	string command;
	string k;
	stringstream iss(str);//create ss object
	ofstream fstor;//create output file
	fstor.open("write.txt");//open file
	string s = "write.txt";
	bool done = false;
	
	while(!done){
		cout << "Type a command: " << endl;
		getline(cin, str);
		stringstream iss(str);//create ss object
		getline(iss, command, ' ');//parse first input for command
		getline(iss, k);//parses rest of string and stores in k
		char c = str[2];
		switch(c){
		case 'L':	// if(command == "RULE")
				cout << "Rule" << k << endl;
				fstor << k;//puts rest in file
				M.load(s);//calls load to check if R or F and puts data where it should go
			break;
		case 'C':	// if(command == "FACT")
				std::cout << "Fact " << k << endl;
				fstor << k;
				M.load(s);
			break;
		case 'A':	// if(command == "LOAD")
				cout << "LOAD" << endl;
				M.load(k);
			break;
		case 'O':	// if(command == "DROP")
				cout << "drop" << endl;
			break;
		case 'F':	// if(command == "INFERENCE")
				cout << "issuing query" << endl;
			break;
		case 'M':	// if(command == "DUMP")
				M.dump();
				cout << "~KB and RB dumped~" << endl;
			break;
		case 'I':	// if(command == "EXIT")
			done = true;
			break;
		default: 
				cout << "command not found" << endl;
			break;
		}
	}
return 0;
}
