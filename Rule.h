#include "common.h"
#ifndef Rule_H
#define Rule_H
using namespace std;
class Rule {
	string logop;
	
	public:
	vector<vector<vector<string>>> RuleVector;

	string get_logop();
	void Rquery(map<string,Rule*> rmap,string s);
	void yuvi_Function(string line, Rule * rule);
	//bool recFunc(map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> variables, vector<string> factNames, vector<string> argVec);
	//void check(map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> argVec,int vecIndex);
	void printRule();
	friend ostream& operator<< (ostream &os, Rule* rule);
	friend ostream& operator<< (ostream &os, vector<string> vstring);
	Rule(string log);	// constructor
	~Rule();
};
#endif
