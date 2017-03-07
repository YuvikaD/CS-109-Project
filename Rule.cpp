#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	logop = log;
	vector<vector<vector<string>>> RuleVector;
	vector<string> infVector;
	vector<vector<vector<string>>> savedResultsVector;
}
ostream& operator<< (std::ostream &os, Rule* rule){
	int increment=0;
	bool no = false;
	//bool pipe = false;
	for(int a=0; a<rule->RuleVector.size();++a){
		//pipe =false;
		//if (rule->RuleVector[a][0][0] == "|"){;pipe=true;}
		bool left=true;
		for (auto vvsiter = rule->RuleVector.begin(); vvsiter != rule->RuleVector.end(); ++vvsiter){
			int pos = vvsiter - rule->RuleVector.begin();
			if(pos>=increment){
				increment++;
				bool rool=true;
				for(auto vsiter = vvsiter->begin(); vsiter!=vvsiter->end(); ++vsiter){
					bool first2 = true;
					bool paren = true;
					bool second= true;	
					for (auto jit = vsiter->begin(); jit != vsiter->end(); ++jit){
						int jpos = jit - vsiter->begin();	
						no=false;
						if (*jit=="|"){no=true;os<<endl;break;} // if multiple of same rule, new line, break
							if(no){break;}
							string stt = *jit;
							stt.erase(remove(stt.begin(), stt.end(), ' '), stt.end());
							if(rool){rool=false;if(paren){os << *jit << "("; paren=false;}}
							else if(paren)	{os << " " << stt << "("; 	paren=false;}
							else if(second)	{os << stt; 				second=false;}
							else if(first2)	{os << ',' << stt; }
					}
				if(no){break;} 	// if multiple of same rule, break
				if(left) {os << "):- " << rule->get_logop(); left=false;} // print separator and Logical Operator
				else{os << ')';}
				if(no){break;}		// if multiple of same rule, break
				}
			if(no){break;}			// if multiple of same rule, break
			}
		}
	}
	if(!no){os<<endl;}
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
	cout << "Got into check function" << endl;
	vector<string> variables;
	vector<string> factNames;
	this->makeVecs(variables, factNames);
	/*cout << "variables vector: ";
	for(auto i : variables){
		cout << i << " ";
	}
	cout << endl;
	cout << "factNames vector: ";
	for(auto i : factNames){
		cout << i << " ";
	}
	cout<< endl;*/
		// OR ------------------------------- OR-----------------------------------OR-------------------------------
	if(get_logop() == "OR"){ // is it OR
		cout<< "got into OR" << endl;
		bool numOrString = 0;	// keeps track of if you're parsing the number or the fact name
		bool vars = false;
		int RuleVal = 0;	// the rule's predicate #
		string FactInQ = ""; // Fact in Question - fact we are going to look at
		int FactsNum = 1; // the number of predicates the Fact in Question has 
		bool ready = false; // bool for if its ready to iterate through the fact
		for(int i = 0; i < factNames.size(); i++){ // for each: 3 Father 2 Mother 2
			if(i == 0){ // if its the first number in factNames
				RuleVal = atoi(factNames[0].c_str()); // then it is the Fact's predicate amount
				for(int VarC = RuleVal; VarC < variables.size(); VarC++){
					if(variables[VarC][0] == '$'){
						vars = true;
					}
				}
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
					 //cout << "FactInQ: " << FactInQ << endl << "FactsNum: " << FactsNum<< endl << "RuleVal: "<< RuleVal << endl << "Rule: "<< paramVector[0][0]<<endl;
					bool going = true;
					while(going){
						for(int it = RuleVal; it<RuleVal+FactsNum; it++){ // checks if variables match (eg. X,Y X,Y)
							if(variables[it] != variables[it+RuleVal]){
								cout<< "variables don't match"<<endl;
								going = false;
								break;
							} // if variables match:
						}
							bool resultFound = true;
								cout << fmap[FactInQ]<<endl;
								for(int i = 0; i < fmap[FactInQ]->vstring.size(); i++){ // checks if strings match, iterating through facts vstring
									for(int varLimit = 0; varLimit < RuleVal; varLimit++){// for marcie, ryan
										cout << "i = " << i << " --- " << fmap[FactInQ]->vstring[i] << " vs "<<argVec[varLimit]<<endl;
										// if they match, and its the last string to check 
										if(fmap[FactInQ]->vstring[i] == argVec[varLimit] && resultFound){
											if( vars || (varLimit+1==RuleVal)){
												cout<< "FOUND: ";
												for(int res = 0; res < argVec.size(); res++){
													cout << fmap[FactInQ]->vstring[i - varLimit + res] << ", ";
												}
												/*for(string ss : argVec){
													cout<<ss<<", ";
												}*/
												cout <<"in " << FactInQ <<endl;
												//cout<<"ending "<< FactInQ <<" fact search"<<endl;
												if(!vars){return;}
											}
										} else if(fmap[FactInQ]->vstring[i] != argVec[varLimit] && !vars){
											resultFound = false;
										} else if(fmap[FactInQ]->vstring[i] == argVec[varLimit] || vars){
											//i++;
										}
									}
									resultFound = true;
								}
								////cout <<"ENDING"<<endl;
						//}
						// if its not found:
						//cout<< paramVector[0][0]<<" NOT FOUND: ";
						//for(string preds : argVec){
						//	cout<<preds << ", ";
						//}
						//cout << " in "<< FactInQ <<endl;
						//cout<<"ending "<< FactInQ <<" fact search"<<endl;
						FactsNum = 0; // resets these for the next Fact Type
						numOrString = 1;
						going = false;
					}
					ready = false;
				} else if (rmap.count(FactInQ) == 1){ // if its another rule
					rmap[FactInQ]->check(rmap,fmap,argVec);
				} else {
					cout << "Error with Fact/Rule " << FactInQ << endl;
				}
			}
		}
	}
	cout << endl;
}

void Rule::makeVecs(vector<string> &variables, vector<string> &factNames){
	cout << "got into MakeVecs" << endl;
	int loops = 0;
	int maincount = 0;
	int count = 0; // count for dealing with rule's stuff
	int varcounter = 0; // counter of # of facts params
	for(vector<vector<string>> j :RuleVector){
		for(vector<string> i : j){ // adds vars to the var vector
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
						if(loops == i.size() &&  maincount == j.size()){
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
	}
}

string Rule::get_logop(){
	return logop;
}

Rule::~Rule(){}	//  destructor