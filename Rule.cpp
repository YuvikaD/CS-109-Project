#include "common.h"
#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	//title = rule;
	if (log == "OR"){logop = 0;}
	else if (log == "AND"){logop = 1;}
	else {cout << " constructor error: log was not AND or OR" << endl;}
	vector<vector<string>> paramVector(100);
	
}

ostream& operator<< (std::ostream &os, Rule* rule)
{
	for(vector<string> i : rule->paramVector){
		for(string s : i){
			cout << s << " ";
		}
	}
    return os;
}

bool Rule::get_logop(){
	return logop;
}

Rule::~Rule(){}	//  destructor