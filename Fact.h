#include "common.h"
#include "Manipulator.h"
#ifndef Fact_H
#define Fact_H

using namespace std;
class Fact {
	string title;
	public:
	vector<string> vstring;
	
	vector<string> get_vstring();
	void dumpFacts(Manipulator M);
	void query(string s);
	friend ostream& operator<<(ostream &os,Fact * fact);
	
	Fact(string name);
	~Fact();
};
#endif
