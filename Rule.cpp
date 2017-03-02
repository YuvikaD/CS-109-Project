#include "common.h"
#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	//if (log == "OR"){logop = 0;}
	//else if (log == "AND"){logop = 1;}
	//else {cout << " constructor error: log was not AND or OR" << endl;}
	logop = log;
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
		cout << "The rule named " << s << " is not in here" << endl;
	}
}

void Rule::check(map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> argVec){
	//cout << "0" << endl;
	vector<string> variables;
	vector<string> factNames;
	//cout << "0.1" << endl;
	int loops = 0;
	int maincount = 0;
	int count = 0; // count for dealing with rule's stuff
	int varcounter = 0; // counter of # of facts params
	for(vector<string> i : paramVector){ // adds vars to the var vector
		//cout << "1" << endl;
		maincount++;
		loops = 0;
		if (count == 0) {// goes through each fact
			loops++;
			count = 1;
		}
		for(string s : i) { // in each fact, check name X Y...
		//cout << "2" << endl;
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
	cout<< endl;
		// OR ------------------------------- OR-----------------------------------OR-------------------------------
	if(get_logop() == "OR"){ // is it OR
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
				if(fmap.count(FactInQ) == 1){
					cout<<"---starting "<< FactInQ <<" fact search---"<<endl;
					// cout << "FactInQ: " << FactInQ << endl << "FactsNum: " << FactsNum<< endl << "RuleVal: "<< RuleVal << endl << "Rule: "<< paramVector[0][0]<<endl;
					bool going = true;
					while(going){
						for(int it = 0; it<RuleVal+FactsNum; it++){ // checks if variables match (eg. X,Y X,Y)
							if(variables[it] != variables[it+RuleVal]){
								cout<< "variables don't match"<<endl;
								going = false;
								break;
							} // if variables match:
							bool resultFound = true;
							//for(int varLimit = 0; varLimit < RuleVal; varLimit++){// for 0, 1, marcie ryan
								for(int i = 0; i < fmap[FactInQ]->vstring.size(); i++){ // checks if strings match
									for(int varLimit = 0; varLimit < RuleVal; varLimit++){// for 0, 1, marcie ryan
										if(fmap[FactInQ]->vstring[i] == argVec[varLimit] && resultFound && (varLimit+1==RuleVal)){
											cout<< "FOUND: " << argVec[0]<< " is a " << FactInQ <<" and a "<< paramVector[0][0]<<endl;
											cout<<"ending "<< FactInQ <<" fact search"<<endl;
											return;
										} else if(fmap[FactInQ]->vstring[i] != argVec[varLimit]){
											resultFound = false;
										} else if(fmap[FactInQ]->vstring[i] == argVec[varLimit]){
											i++;
										}
									}
									resultFound = true;
								}
								
							//}
						}
						// if its not found:
						cout<< paramVector[0][0]<<" NOT FOUND: ";
						for(string preds : argVec){
							cout<<preds << ", ";
						}
						cout << " in "<< FactInQ <<endl;
						cout<<"ending "<< FactInQ <<" fact search"<<endl;
						FactsNum = 0; // resets these for the next Fact Type
						numOrString = 1;
						going = false;
					}
					ready = false;
				} else if (rmap.count(FactInQ) == 1){ // if its another rule
					rmap[FactInQ]->check(rmap,fmap, X, Y);
				} else {
					cout << "Error with Fact/Rule " << FactInQ << endl;
				}
			}
		}
	}
	cout << endl;

	

	// ------------------------------------AND -----------------------------------AND----------------------------------------
	if(get_logop()=="AND"){
		bool worked;
		worked = recFunc(rmap,fmap,variables,factNames,argVec);
		cout << worked<< endl;
	}
}

bool Rule::recFunc(map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> variables, vector<string> factNames, vector<string> argVec){
	int RuleVal = atoi(factNames[0].c_str()); 
	
		int FactsNum = 0;
		//bool ready = true
		//if(ready){
			cout << "variables vector before: "<<endl;
			for(auto i : variables){
				cout << i << " ";
			}
			cout<<endl;
			for(int i = 0; i < RuleVal; i++){ // for the rules predicates
				string temp = variables[i];
				for(int it = RuleVal; it<variables.size(); it++){
					if(variables[i] == variables[it]){
						variables[it] = argVec[i];	// check if they match
					}
				}
				variables[i]=argVec[i];					// if they do, plug in the strings
			}
			cout << "variables vector after: "<<endl;
			//variables.erase(variables.begin() + 2);
			for(auto i : variables){
				cout << i << " ";
			}
			cout<<endl;
			vector<string> temp;
			vector<string> tempVars = variables;
			for(int i = 1; i < factNames.size(); i+=2){ // for each fact in factnames
				FactsNum= atoi(factNames[i+1].c_str());
				string FactInQ = factNames[i];
				int factsIndex = i;
				cout<<"FactInQ: "  <<FactInQ<<endl;
				cout<<factNames[i]<<" "<< FactsNum<<endl;
				//if(fmap.count(factNames[i]) == 1){ // if its a fact
					for(int it = RuleVal; it<RuleVal+FactsNum; it++){
						temp.push_back(variables[it]);
						cout<<"Pushed "<< variables[it]<<endl;
					}
					int offset = 0;
					for(int off = 0; off < temp.size(); off++){
						if(temp[off][0] != '$'){
							offset = off;
							break;
						}
					}
					int keeper = 0;
					bool kept = false;
				if(fmap.count(factNames[i]) == 1){ // if its a fact
					bool temps = false;
					for (int finder = offset; finder <fmap[FactInQ]->vstring.size(); finder+= (FactsNum +1)){ // for all the facts
						if(fmap[FactInQ]->vstring[finder] == temp[offset]){
							for(int parser = 0; parser < temp.size(); parser++){
								if(fmap[FactInQ]->vstring[finder + parser] == temp[parser]){ // if the strings match
									cout << "match: " << fmap[FactInQ]->vstring[finder + parser]  << endl;
								} if(temp[parser][0] == '$'){
									temps = true;
									string tempval = tempVars[RuleVal + parser];
									cout << "tempval: " << tempval << endl;
									//tempVars[RuleVal + parser] =  fmap[FactInQ]->vstring[finder + parser]; // replaces $variable
									for(int replacer = 0; replacer < tempVars.size(); replacer++){
										if(tempVars[replacer] == tempval){
											if(kept == false){keeper = replacer; kept = true;}
											cout << "replacing " << tempVars[replacer] << " " <<  replacer<< " with "<< fmap[FactInQ]->vstring[finder + parser] <<endl;
											tempVars[replacer]  = fmap[FactInQ]->vstring[finder + parser];
										}
									}
									// cout << "variable" << endl;
								}
							}
						}
					}
					cout << "variables vector after replacement: "<<endl;
					//variables.erase(variables.begin() + 2);
					for(auto i : tempVars){
						cout << i << " ";
					}
					cout<<endl;
					if(temps == false){ //no $signs
						for (int it = 0; it < FactsNum; it++){ // remove it from the vector
							variables.erase(variables.begin() + RuleVal + keeper);
						}
						factNames.erase(factNames.begin() + i);
						factNames.erase(factNames.begin() + i+1);
						//cout << "temps is false"<<endl;
						//return true;
						//rmap[FactInQ]->check(rmap,fmap,temp);
					}
					//cout<<endl;

					//bool done = recFunc(rmap,fmap, tempVars, factNames, argVec);
					//f(done){
						
					//}

				}else if (rmap.count(FactInQ) == 1){ // if its another rule
					//rmap[FactInQ]->check(rmap,fmap,tempVars);
					recFunc(rmap,fmap,tempVars,factNames,argVec);
				} else {
					cout << FactInQ <<" isn't a fact or rule"<<endl;
					return false; 
				}
			cout<<endl;
			return true;
			}
}
string Rule::get_logop(){
	return logop;
}

Rule::~Rule(){}	//  destructor