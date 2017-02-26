#include "common.h"
#ifndef Rule_H
#define Rule_H
using namespace std;
class Rule {
	bool logop;
	public:
	vector<vector<string>> paramVector;
	//vector<string> get_vstring();
	bool get_logop();
	void printRule();
	Rule(string log);	// constructor
	~Rule();
};
#endif
