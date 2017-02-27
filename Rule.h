#include "common.h"
#ifndef Rule_H
#define Rule_H
using namespace std;
class Rule {
		// private data member
	bool logop;
	public:
	vector<vector<string>> paramVector;
	bool get_logop();
	void printRule();
	void Rquery(map<string,Rule*> rmap,string s);
	Rule(string log);	// constructor
	~Rule();
	void check(map<string,Rule*> rmap,map<string,Fact*> fmap, string x, string y);
};
#endif
