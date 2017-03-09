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
	bool printImmediately = true;
	vector<string> infRules;
	vector<vector<string>> rawPredicates;
	map<string,Fact*> Fact_map;
	map<string,Rule*> Rule_map;
	void drop(string name);
	void dumpFacts(ostream &os);
	void dumpRules(ostream &os);
	void dump();
	void load(string filename);
	void inference(string filename);
	void nofilter(string leftHandSide,string edited, vector<string> varVec);
	void evaluate(string line, Rule * rule, map<string,Fact*> fmap, map<string,Rule*> rmap);
	void factFilter(string FactInQ, map<string,Fact*> fmap, vector<string> argVec);
	void ruleFilter(string rule, map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> argVec);
	void makeVecs(string rule, vector<string> &variables, vector<string> &factNames,map<string,Rule*> rmap);
	Manipulator();
	~Manipulator();
};
#endif
