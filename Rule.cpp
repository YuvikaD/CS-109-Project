#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	logop = log;
	vector<vector<string>> paramVector(100);	// hardcoded max amount of rule predicates
}
ostream& operator<< (std::ostream &os, Rule* rule)
{
	for(vector<string> i : rule->paramVector){	// print first vector ex "GrandFather($X,$Y)"
			bool first2 = true;
			bool paren = true;
			bool second= true;
			for(string s : i){
				if(paren)		{os << s << '('; paren=false;}
				else if(second)	{os << s; 		second=false;}
				else if(first2)	{os << ',' << s;}
			}
			first2=false;
			os << "):- " << rule->get_logop() << " ";	// print separator and Logical Operator
			break;
		}
	
	bool first = true;
	for(vector<string> i : rule->paramVector){	// print the rest of the vector
		if(first){first=false;}
		else{
			bool first2 = true;
			bool paren = true;
			bool second= true;
			for(string s : i){
				if(paren)		{os << s << '('; paren=false;}
				else if(second)	{os << s; 		second=false;}
				else if(first2)	{os << ',' << s;}
			}
			os << ") ";
		}
	}
    return os;
}

ostream& operator<< (std::ostream &os, vector<string> vstring)
{
	//os << rule->title << ": ";
	for(string i : vstring){
			os << i << endl << ": ";
	}
	os << endl;
    return os;
}

string Rule::get_logop(){
	return logop;
}

Rule::~Rule(){}	//  destructor
