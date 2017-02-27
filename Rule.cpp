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
// called like: Rule_map["Parent"]->check(Rule_map,Fact_map,"Marcie","Ryan");
void Rule::check(map<string,Rule*> rmap,map<string,Fact*> fmap, string x, string y){
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
	cout << "variables vector: ";
	for(auto i : variables){
		cout << i << " ";
	}
	cout << endl;
	cout << "factNames vector: ";
	for(auto i : factNames){
		cout << i << " ";
	}
	cout << endl;
	bool numOrString = 0;	// keeps track of if you're parsing the number or the fact name
	int RuleVal = 0;	// the rule's predicate #
	string FactInQ = ""; // Fact in Question - fact we are going to look at
	int FactsNum = 1; // the number of predicates the Fact in Question has 
	bool ready = false; // bool for if its ready to iterate through the fact
	for(int i = 0; i < factNames.size(); i++){ // for each: 3 Father 2 Mother 2
		if(i == 0){ // if its the first number in factNames
			RuleVal = atoi(factNames[0].c_str()); // then it is the Fact's predicate amount
			numOrString = 1; // next val will be a string
		} else if(!ready) { // if its still not ready to iterate through the fact...
			if (numOrString == 1) {				// if its a string
				FactInQ = factNames[i];	// fact in question = the facts name
				numOrString = 0; // next thing will be a number
			} else if (numOrString == 0){ 					// if its a number
				FactsNum= atoi(factNames[i].c_str()); // fact num = facts num of predicates
				numOrString = 1; // next thing will be a string
				ready = true;
			} 
		}  if (ready){ // when you have a fact type and it's param #
			cout<<"---starting "<< FactInQ <<" fact search---"<<endl;
			if (fmap.count(FactInQ) == 1){
				// cout << "FactInQ: " << FactInQ << endl << "FactsNum: " << FactsNum<< endl << "RuleVal: "<< RuleVal << endl << "Rule: "<< paramVector[0][0]<<endl;
				bool going = true;
				while(going){
					for(int it = 0; it<RuleVal+FactsNum; it++){ // checks if variables match (eg. X,Y X,Y)
						if(variables[it] != variables[it+RuleVal]){
							cout<< "variables don't match"<<endl;
						going = false;
							break;
						} // if variables match:
						for(int i = 0; i < fmap[FactInQ]->vstring.size(); i++){ // checks if strings match
							if(fmap[FactInQ]->vstring[i] == X && fmap[FactInQ]->vstring[i+1] == Y){
								cout<< "FOUND: " << x << " is a " << FactInQ <<" and a "<< paramVector[0][0]<<endl;
								//cout<<"ending "<< FactInQ <<" fact search"<<endl;
								return;
							}
						}
					}
						// if its not found:
						//cout<< paramVector[0][0]<<" NOT FOUND: " << X << ", " << Y<< " in "<< FactInQ<<endl;
						//cout<<"ending "<< FactInQ <<" fact search"<<endl;
					FactsNum = 0; // resets these for the next Fact Type
					numOrString = 1;
					going = false;
					}
			} else if (rmap.count(FactInQ) == 1){ // if its another rule
				rmap[FactInQ]->check(rmap,fmap, X, Y);
			}
		}
		ready = false;
	}
	cout << endl;
}

bool Rule::get_logop(){
	return logop;
}

Rule::~Rule(){}	//  destructor
