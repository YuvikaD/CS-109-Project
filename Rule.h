#include "common.h"
#ifndef Rule_H
#define Rule_H
using namespace std;
class Rule {
	string logop;
	
	public:
	vector<vector<vector<string>>> RuleVector;
	vector<string> infVector;
	vector<string> userArgs;
	vector<string> andVars;
	vector<string> predNames;
	vector<vector<vector<string>>> savedResultsVector;
	
	bool firstInference;
	vector<vector<string>> dollarVarsVec;
	vector<vector<string>> infFactsVector;
	vector<vector<string>> Results;
	
	string get_logop();
	void Rquery(map<string,Rule*> rmap,string s);
	//void evaluate(string line,map<string,Fact*> fmap, map<string,Rule*> rmap);
	//bool recFunc(map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> variables, vector<string> factNames, vector<string> argVec);
	void printRule();
	friend ostream& operator<< (ostream &os, Rule* rule);
	friend ostream& operator<< (ostream &os, vector<string> vstring);
	Rule(string log);	// constructor
	~Rule();
};
#endif
