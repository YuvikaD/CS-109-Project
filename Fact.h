#include "common.h"
#ifndef Fact_H
#define Fact_H
using namespace std;
class Fact {
	string title;	
		// private data member
	public:
	vector<string> vstring;
	
	vector<string> get_vstring();
	void printFact();
	friend ostream& operator<<(ostream &os,Fact * fact);
	
	Fact(string name);
	~Fact();
};
#endif