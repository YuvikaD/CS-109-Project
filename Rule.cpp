#include "common.h"
#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	//title = rule;
	if (log == "OR"){logop = 0;}
	else if (log == "AND"){logop = 1;}
	else {cout << " constructor error: log was not AND or OR" << endl;}
	vector<string> vstring;
}

ostream& operator<< (std::ostream &os, Rule* rule)
{
	for(string i : rule->vstring){
	//for (vector<string>::iterator it = rule->vstring.begin() ; it != rule->vstring.end(); ++it){ 
		//cout << *it << " ";
		cout << i << " ";
	}
    return os;
}



void Rule::printRule(){
	//cout << title << ": ";
	for (vector<string>::iterator it = vstring.begin() ; it != vstring.end(); ++it){ 
		cout << *it << " ";
	}
	cout << endl;
}


Rule::~Rule(){}	//  destructor
