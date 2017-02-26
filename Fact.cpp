#include "common.h"
#include "Fact.h"
using namespace std;
Fact:: Fact(string name){	// constructor 
	title = name;
	vector<string> vstring;
}	

vector<string> Fact::get_vstring(){
	return vstring;
}

void Fact::printFact(){
	//cout << title << ": ";
	for (vector<string>::iterator it = vstring.begin() ; it != vstring.end(); ++it){ 
		cout << *it << " ";
	}
	cout << endl;
}

ostream& operator<< (std::ostream &os, Fact* fact)
{
	for(string i : fact->vstring){
			cout << i << " ";
	}
    return os;
}

void query(map<string,Fact*> fmap,string s){
	if(fmap.count(s) == 1){
		cout << s << ": " << endl
		<< fmap[s] << endl;
	}
	else {
		cout << "The fact named " << s << " is not in here" << endl;
	}
}

Fact::~Fact(){}	//  destructor