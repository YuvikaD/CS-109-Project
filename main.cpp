#include "common.h"
#include "Fact.cpp"
#include "Rule.cpp"
#include "Manipulator.cpp"
using namespace std;
int main(){
	Manipulator M;
	// load facts and rules from file "input.txt"
	M.load();
	// dump facts and rules to   file "output.txt"
	M.dump();
	//query(M.Fact_map,"Father");
	//query(M.Fact_map,"Mother");
	
}
