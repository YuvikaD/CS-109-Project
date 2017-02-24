#include "common.h"
#ifndef Rule_H
#define Rule_H
using namespace std;
class Rule {
		// private data member
	bool logop;
	public:
	vector<string> vstring;
	vector<string> get_vstring();
	bool get_logop();
	void printRule();
	Rule(string log);	// constructor
	~Rule();
};
#endif