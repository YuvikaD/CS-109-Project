#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	logop = log;
	vector<vector<vector<string>>> RuleVector;
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
	vector<string> variables;
	vector<string> factNames;
	this->makeVecs(variables, factNames);
	/*cout << "argVec: " <<endl;
	for(string i : argVec){
		cout << i << " ";
	}
	cout << endl;*/
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
		bool numOrString = 0;	// keeps track of if you're parsing the number or the fact name
		bool vars = false;
		int RuleVal = 0;	// the rule's predicate #
		string FactInQ = ""; // Fact in Question - fact we are going to look at
		int FactsNum = 1; // the number of predicates the Fact in Question has 
		bool ready = false; // bool for if its ready to iterate through the fact
		for(int i = 0; i < factNames.size(); i++){ // for each: 3 Father 2 Mother 2
			if(i == 0){ // if its the first number in factNames
				RuleVal = atoi(factNames[0].c_str()); // then it is the Fact's predicate amount
				for(int VarC = 0; VarC < argVec.size(); VarC++){
					if(argVec[VarC][0] == '$'){
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
				//cout<<"---starting "<< FactInQ <<" fact search---"<<endl;
					// cout << "FactInQ: " << FactInQ << endl << "FactsNum: " << FactsNum<< endl << "RuleVal: "<< RuleVal << endl << "Rule: "<< paramVector[0][0]<<endl;
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
								//cout << fmap[FactInQ]<<endl;
								for(int i = 0; i < fmap[FactInQ]->vstring.size(); i++){ // checks if strings match, iterating through facts vstring
									for(int varLimit = 0; varLimit < RuleVal; varLimit++){// for marcie, ryan
										//cout << "i = " << i << " --- " << fmap[FactInQ]->vstring[i] << " vs "<<argVec[varLimit]<<endl;
										// if they match, and its the last string to check 
										bool alright = (vars || (varLimit+1==RuleVal));
										bool equals = (fmap[FactInQ]->vstring[i] == argVec[varLimit]);
										bool sizes = (i-varLimit+argVec.size() <=  fmap[FactInQ]->vstring.size());
										
										//if(i>5){cout << "Equals: "<<equals << " ResultFound: " << resultFound << " sizes: " << sizes <<" alright: "<<alright<<endl;}
										if(fmap[FactInQ]->vstring[i] == argVec[varLimit] && resultFound && i-varLimit+argVec.size() <=  fmap[FactInQ]->vstring.size()  && alright){
											//if(i-varLimit+argVec.size() <=  fmap[FactInQ]->vstring.size()){
												//cout << "vars: "<<vars<<endl;
												//if( vars || (varLimit+1==RuleVal)){
													bool legit = true;
													for(int res = 0; res < argVec.size(); res++){
														if(fmap[FactInQ]->vstring[i - varLimit + res] == "|"){
															legit = false; // maybe seg fault causing
														}
													}
													if(legit){
														//cout<< "FOUND: ";
														/*for(string bla : fmap[FactInQ]->vstring){
															cout << bla << " ";
														} cout <<endl;*/
														for(int res = 0; res < argVec.size(); res++){
															//cout << "index "<<i - varLimit + res<<endl;
															if(argVec[res][0]=='$'){
																cout << argVec[res] << ": ";
															} else {
																cout << variables[res][1] << ": ";
															}
															cout << fmap[FactInQ]->vstring[i - varLimit + res];
															if(res+1!=argVec.size()){cout<< ", ";}
														}
														cout << endl;
														/*for(string ss : argVec){
															cout<<ss<<", ";
														}*/
														//cout <<"in " << FactInQ <<endl;
														//cout<<"ending "<< FactInQ <<" fact search"<<endl;
														if(!vars){
															cout<<endl;
															return;
														}
													}
													
												//}
											//}
											
										} else if(fmap[FactInQ]->vstring[i] == "|"){
											resultFound = false;
										}else if(fmap[FactInQ]->vstring[i] != argVec[varLimit] && !vars && equals){
											//if(i>5){cout<<"setting resultFound to false, "<<fmap[FactInQ]->vstring[i] << " != " << argVec[varLimit]<<endl;}
											resultFound = false;
										} else if(fmap[FactInQ]->vstring[i] == argVec[varLimit] || vars){
											//i++; // this causes seg fault
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
