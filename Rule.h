#include "common.h"
#ifndef Rule_H
#define Rule_H
using namespace std;
class Rule {
	string logop;
	
	public:
	vector<vector<vector<string>>> RuleVector;
	vector<string> infVector;
	vector<vector<vector<string>>> savedResultsVector;
	string get_logop();
	void Rquery(map<string,Rule*> rmap,string s);
	//void evaluate(string line,map<string,Fact*> fmap, map<string,Rule*> rmap);
	//bool recFunc(map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> variables, vector<string> factNames, vector<string> argVec);
	void check(map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> argVec);
	void printRule();
	friend ostream& operator<< (ostream &os, Rule* rule);
	friend ostream& operator<< (ostream &os, vector<string> vstring);
	void makeVecs(vector<string> &variables, vector<string> &factNames);
	Rule(string log);	// constructor
	~Rule();
};
#endif
