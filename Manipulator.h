#include "common.h"
class Rule;
class Fact;
#ifndef Manipulator_H
#define Manipulator_H
using namespace std;
class Manipulator {
	string line;		// empty string to load the input file's lines into
	string leftHandSide;
	string rightHandSide;
	string predName;
	string separator;
	string logop;	
	string name;		// the name of the Fact or Rule, like "Father"
	string name2;
	string name3;
	string temp;
	string temp2;
	const string space="|";
	string data;		// the params of the Fact, like "Allen"
	string preds;
	
	public:
	int recursions=-1;
	string resultsInString;
	vector<vector<string>> andVarsVec;
	char outputArray [];
	
	vector<thread> tvec;
	mutex mtx;
	bool joined;
	int tCount=0;
	
	bool printImmediately = true;
	vector<string> infRules;
	vector<vector<string>> rawPredicates;
	map<string,Fact*> Fact_map;
	map<string,Rule*> Rule_map;
	void drop(string name);
	void dumpFacts(ostream &os);
	void dumpRules(ostream &os);
	void dump();
	void LineIn(string str, bool *done);
	void load(string filename);
	void inference(string filename);
	void nofilter(string leftHandSide,string edited, vector<string> varVec);
	void factFilter(string FactInQ, map<string,Fact*> fmap, vector<string> argVec);
	void ruleFilter(string rule, map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> argVec);
	void makeVecs(string rule, vector<string> &variables, vector<string> &factNames,map<string,Rule*> rmap);
	void AND(vector<string> tVar, vector<string> group, vector<vector<string>> Results, int row, int ind);
	void clean();
	void thread_OR();
	Manipulator();
	~Manipulator();
};
#endif
