#include "common.h"
#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	if (log == "OR"){logop = 0;}
	else if (log == "AND"){logop = 1;}
	else {cout << " constructor error: log was not AND or OR" << endl;}
	vector<vector<string>> paramVector(100);
	
}

ostream& operator<< (std::ostream &os, Rule* rule)
{
	for(vector<string> i : rule->paramVector){
		for(string s : i){
			cout << s << " ";
		}
	}
    return os;
}

void Rquery(map<string,Rule*> rmap,string s){
	if(rmap.count(s) == 1){
		cout << s << ": " << endl
		<< rmap[s] << endl;
	} else {
		cout << "The fact named " << s << " is not in here" << endl;
	}
}

void Rule::check(string x, string y){
		//parent is the rule
		string X = x;
		string Y = y;
		vector<string> variables;
		vector<string> factNames;
		int loops = 0;
		int maincount = 0;
		int count = 0; // count for dealing with rule's stuff
		int varcounter = 0; // counter of # of facts params
		for(vector<string> i : paramVector){ // adds vars to the var vector
			maincount++;
			loops = 0;
			if (count == 0) {// goes through each fact
				loops++;
				count = 1;
			} 
			for(string s : i) { // in each fact, check name X Y...
				loops++;
				// cout << "loop: " << loops << endl;
				//cout << "String:" << s << endl; //debugging
				if (count == 1){
					count++;
					//cout << "not added" << endl;
				} else {
					if (s[0] == '$'){
						varcounter ++;
						variables.push_back(s);
						if(loops == i.size() &&  maincount == paramVector.size()){
							factNames.push_back(std::to_string(varcounter));
						}
					} else {
						factNames.push_back(std::to_string(varcounter));
						factNames.push_back(s);
						//cout << "Reset Var" << endl;
						varcounter = 0; // resets counter for next fact
					}
				}
			}
		}
		cout << "variables array: ";
		for(auto i : variables){
			cout << i << " ";
		}
		cout << endl;
		cout << "factNames array: ";
		for(auto i : factNames){
			cout << i << " ";
		}
		cout << endl;
		bool numOrString = 0; // 0 is num
		int value;
		//char* str;
		// cout << "String or Int?" << endl;
		int RuleVal = 0;
		string FactInQ = "";
		int FactsNum = 0;
		int rounds = 0;
		for(int i = 0; i < factNames.size(); i++){ // for each: 3 Father 2 Mother 2
			rounds++;
			if(i == 0){
				RuleVal = atoi(factNames[0].c_str());
				//cout << RuleVal << " ";						//RuleVal == 3
				//cout << "int "; 
				numOrString = 1;
			} else if(rounds <= RuleVal+1) {
					if (numOrString == 0){ // if its a number
						value = atoi(factNames[i].c_str());	// value == 2
						//cout << "VALUE: " <<value << " ";
						//cout << "int "; 
						numOrString = 1;
						if(FactsNum == 0){FactsNum = value;}
					} else  if (numOrString == 1) {				// if its a name
						//cout << factNames[i] << " ";
						//cout << "string ";
						FactInQ = factNames[i];				// FactInQ == Father
						numOrString = 0;
						
					}
			} else {
				cout << "FactInQ: " << FactInQ << endl << "FactsNum: " << FactsNum<< endl << "RuleVal: "<< RuleVal << endl << "Rule: "<< paramVector[0][0]<<endl;
				
				/*if in variables[i<RuleVal]
					matches variables[i = Ruleval -> i < ruleVal+factsNum]
						in FactInQ if i = x and i+1 = y
						found
				*/
			}
			
			
		}
		cout << endl;
		
		
		/*in fact array - save the number
			iterate through the fact
				if i == marcie and the next val == ryan
					marcie is a parent
					return;
		cout << "Not found" << endl;
		*/
}
	
bool Rule::get_logop(){
	return logop;
}

Rule::~Rule(){}	//  destructorr
