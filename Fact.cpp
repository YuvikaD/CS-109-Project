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

ostream& operator<< (std::ostream &os, Fact* fact)
{
	os << fact->title << ": ";
	for(string i : fact->vstring){
		if(!i.compare("|")){
			os << endl << fact->title << ": ";
		} else	os << i << " ";
	}
	os << endl;
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
