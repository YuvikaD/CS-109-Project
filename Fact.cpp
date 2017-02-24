#include "common.h"
#include "Fact.h"
using namespace std;
Fact:: Fact(string name){	// constructor 
	title = name;
	std::vector<std::string> vstring;
}	

void Fact::printFact(){
	cout << title << ": ";
	for (vector<string>::iterator it = vstring.begin() ; it != vstring.end(); ++it){ 
		cout << *it << " ";
	}
	cout << endl;
}


std::ostream& operator<< (std::ostream &os, Fact* fact)
{
	for (vector<string>::iterator it = fact->vstring.begin() ; it != fact->vstring.end(); ++it){ 
		cout << *it << " ";
	}
    return os;
}
Fact::~Fact(){}	//  destructor
