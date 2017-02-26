#include "common.h"
#include "Fact.cpp"
#include "Rule.cpp"
#include "Manipulator.cpp"
using namespace std;
int main(){ 
	Manipulator M;
	string str;
	string command;
	string rest;
    
	std::cout << "Type a command to begin: " <<endl;
	
    getline(std::cin, str); //get input and store in str
    stringstream iss(str);//create ss object
	getline(iss, command, ' ');//parse first input for command
	string k = getline(iss, rest);//parses rest of string and stores in rest
	ofstream fout;//create output file
	fout.open("write.txt");//open file
	string s = "write.txt"
	if(command == "RULE"){	
		std::cout << "Rule" << k << endl;
			fout << k;//puts rest in file
			M.load(s);//calls load to check if R or F and puts data where it should go
		}
		else if(command == "FACT"){
			std::cout << "Fact" << k << endl;
			fout << k;
			M.load(s);
		}		
		else if(command == "LOAD"){std::cout << "load"; M.load();}
		else if(command == "DROP"){
			std::cout << "drop";
			if()
				Fact_Map.find(name)->
				else if ()
					Rule_map.find(name)->
					
			 determine f or r. find data in vector and delete
			//Fact_map.find(name)->second->vstring.push_back(data);
		}
		
		else if(command == "INFERENCE"){std::cout << "issuing query";}
		else if(command == "DUMP"){M.dump();std::cout << "~KB and RB dumped~";}
		else {std::cout << "command not found";}
	return 0;
}
	
}