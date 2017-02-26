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
	map<string,Fact*> Fact_map;
	map<string,Rule*> Rule_map;
	void dumpFacts(ostream &os);
	void dumpRules(ostream &os);
	void dump();
	void load(string filename);
	Manipulator();
	~Manipulator();
};
#endif
