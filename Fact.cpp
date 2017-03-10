#include "Fact.h"
using namespace std;
Fact:: Fact(string name){	// constructor 
	title = name;
	bool printed = false;
	bool readFacts = false;
	vector<string> vstring;
	vector<string> countVec;
	vector<string> subject;
	vector<string> andVars;
	//vector<string> fromvectorRaw;
	//vector<vector<string>> savedResultsVector;
	//vector<vector<string>> rawPredicates;
}	

vector<string> Fact::get_vstring(){
	return vstring;
}

ostream& operator<< (ostream &os, Fact* fact)
{
	os << fact->title << "(";
	for(int n=0; n<fact->vstring.size(); n++){
		string i = fact->vstring[n];
		string m;
		if(n+1 != (fact->vstring.size())) m = fact->vstring[n+1];
		if(!i.compare("|")){
			os << ")" << endl << fact->title << "(";
		}else if(!m.compare("|") || n+1 == fact->vstring.size()){
			os << i;
		}else os << i << ",";
	}
	os << ")" << endl;
    return os;
}

void query(map<string,Fact*> fmap,string s){
	if(fmap.count(s) == 1){
		cout << s << ": " << fmap[s] << endl;
	}
	else {
		cout << "The fact named " << s << " is not in here" << endl;
	}
}

Fact::~Fact(){}	//  destructor
