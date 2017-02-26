#include "common.h"
#include "Fact.cpp"
#include "Rule.cpp"
#include "Manipulator.cpp"
using namespace std;
int main(){
	Manipulator M;
	M.load();
	// print out facts and rules for debugging
	dumpFacts(M);
	//dumpRules(M);
	for(map<string,Rule*>::iterator it = M.Rule_map.begin(); it != M.Rule_map.end(); ++it) {
		cout << " " 
		<< it->second->get_logop() << " " << it->second << endl;
    }

	//query(M.Fact_map,"Father");
	//query(M.Fact_map,"Mother");
	
}
