#include "common.h"
#ifndef Fact_H
#define Fact_H
class Fact {
	public:
	std::string title;
	std::vector<std::string> vstring;
	
	void printFact();
	friend std::ostream& operator<<(std::ostream &os,Fact * fact);
	
	Fact(std::string name);
	~Fact();
};
#endif