#include "common.h"
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
    while(true){
 	string str;
	string command;
	string k;
    
	cout << "Type a command to begin: " <<endl;
	
    	getline(cin, str); //get input and store in str
    	stringstream iss(str);//create ss object
	getline(iss, command, ' ');//parse first input for command
	getline(iss, k);//parses rest of string and stores in rest
	ofstream fstor;//create output file
	fstor.open("write.txt");//open file
	string s = "write.txt";
	if(command == "RULE"){	
		std::cout << "Rule" << k << endl;
			fstor << k;//puts rest in file
			M.load(s);//calls load to check if R or F and puts data where it should go
		}
		else if(command == "FACT"){
			std::cout << "Fact" << k << endl;
			fstor << k;
			M.load(s);
		}		
		else if(command == "LOAD"){cout << "load" << endl; M.load(k);}
		else if(command == "DROP"){
			cout << "drop" <<endl;
		/*	if()
				Fact_Map.find(name)->
				else if ()
					Rule_map.find(name)->
					
			 determine f or r. find data in vector and delete
			Fact_map.find(name)->second->vstring.push_back(data);*/
		}
		
		else if(command == "INFERENCE"){cout << "issuing query" <<endl;}
		else if(command == "DUMP"){M.dump(); cout << "~KB and RB dumped~" <<endl;}
	    	else if(command =='exit'){break;}
		else {cout << "command not found" <,endl;}
    }
	return 0;
	
}
