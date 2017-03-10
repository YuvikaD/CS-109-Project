#include "common.h"
#include "Manipulator.h"
#ifndef Fact_H
#define Fact_H

using namespace std;
class Fact {
	string title;
	
	public:
	bool printed;
	bool readFacts;
	vector<string> vstring;
	vector<string> countVec;
	vector<string> userArgs;
	vector<string> andVars;
	//vector<vector<string>> savedResultsVector;
	//vector<vector<string>> rawPredicates;
	vector<string> subject;
	vector<string> get_vstring();
	void dumpFacts(Manipulator M);
	void query(string s);
	friend ostream& operator<<(ostream &os,Fact * fact);
	
	Fact(string name);
	~Fact();
};
#endif
