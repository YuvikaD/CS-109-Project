#include "common.h"
#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	//title = rule;
	if (log == "OR"){logop = 0;}
	else if (log == "AND"){logop = 1;}
	else {cout << " constructor error: log was not AND or OR" << endl;}
	vector<vector<string>> paramVector(10);
	
}

ostream& operator<< (std::ostream &os, Rule* rule)
{
	for(vector<string> i : rule->paramVector){
		for(string s : i){
			cout << s << " ";
		}
	}
	/*for(string i : rule->get_vstring()){
	//for (vector<string>::iterator it = rule->vstring.begin() ; it != rule->vstring.end(); ++it){ 
		//cout << *it << " ";
		cout << i << " ";
	}*/
    return os;
}

/*
vector<string> Rule::get_vstring(){
	return vstring;
}
*/
bool Rule::get_logop(){
	return logop;
}
/*
void Rule::printRule(){
	//cout << title << ": ";
	for (vector<string>::iterator it = vstring.begin() ; it != vstring.end(); ++it){ 
		cout << *it << " ";
	}
	cout << endl;
}
*/

Rule::~Rule(){}	//  destructor