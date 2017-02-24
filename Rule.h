#include "common.h"
#ifndef Rule_H
#define Rule_H
using namespace std;
class Rule {
	public:
	//string title;
	vector<string> vstring;	// data vector
	bool logop;
	void printRule();
	Rule(string log);	// constructor
	~Rule();
};
#endif