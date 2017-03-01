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
		cout << "The rule named " << s << " is not in here" << endl;
	}
}

void Rule::check(map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> argVec){
	//parent is the rule
	/*va_list argList;
	va_start(argList, num);
	vector<string> argVec;
	string str;
	//cout<<"here"<<endl;
	for(int i = 1; i < num; i++){
		str = vsscanf(va_arg(argList,string*), "%s", argList);
		argVec.push_back(str);
	}*/
	
	string X = argVec[0];
	string Y = argVec[1];
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
	if(get_logop() == 0){
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
										cout<< "FOUND: " << X << " is a " << FactInQ <<" and a "<< paramVector[0][0]<<endl;
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
					cout<< paramVector[0][0]<<" NOT FOUND: " << X << ", " << Y<< " in "<< FactInQ <<endl;
					cout<<"ending "<< FactInQ <<" fact search"<<endl;
					FactsNum = 0; // resets these for the next Fact Type
					numOrString = 1;
					going = false;
				}
				ready = false;
			}
		}
	}
	cout << endl;
	// OR ------------------------------- OR-----------------------------------OR-------------------------------
	

	// ------------------------------------AND -----------------------------------AND----------------------------------------
	if(get_logop()==1){
		int RuleVal = atoi(factNames[0].c_str()); 
		int FactsNum = 0;
		//bool ready = true
		//if(ready){
			for(int i = 0; i < RuleVal; i++){ // for the rules predicates
				string temp = variables[i];
				for(int it = RuleVal; it<variables.size(); it++){
					if(variables[i] == variables[it]){
						variables[it] = argVec[i];
					}
					
					/*if(variables[i] == variables[it]){ // checks if the $Values match
						if(variables[i] == "$X"){ // this will have to be changed to be the vector of names given
							variables[it]=X;					// if they do, plug in the strings
						}else if(variables[i] == "$Y"){
							variables[it]=Y;
						} 
					}*/
				}
				//if(variables[i] == "$X"){ // this will have to be changed to be the vector of names given
				variables[i]=argVec[i];					// if they do, plug in the strings
				/*}else if(variables[i] == "$Y"){
					variables[i]=Y;
				} */
			}
			cout << "variables vector after: "<<endl;
			//variables.erase(variables.begin() + 2);
			for(auto i : variables){
				cout << i << " ";
			}
			//cout << endl;
			//for(auto i : factNames){
			//	cout << i << " ";
			//}
			cout<<endl;
			vector<string> temp;
			vector<string> tempVars = variables;
			for(int i = 1; i < factNames.size(); i+=2){ // for each fact in factnames
				FactsNum= atoi(factNames[i+1].c_str());
				string FactInQ = factNames[i];
				cout<<"FactInQ: "  <<FactInQ<<endl;
				cout<<factNames[i]<<" "<< FactsNum<<endl;
				if(fmap.count(factNames[i]) == 1){ // if its a fact
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
					for (int finder = offset; finder <fmap[FactInQ]->vstring.size(); finder+= (FactsNum +1)){
						if(fmap[FactInQ]->vstring[finder] == temp[offset]){
							for(int parser = 0; parser < temp.size(); parser++){
								if(fmap[FactInQ]->vstring[finder + parser] == temp[parser]){ // if the strings match
									cout << fmap[FactInQ]->vstring[finder + parser]  << endl;
								}
							}
						}
					}
					//for(int Fi = 0; Fi < fmap[FactInQ]->vstring.size(); Fi+= (FactsNum+1)){ // iterating through this fact's facts
						//if(fmap[FactInQ]->vstring[Fi] == temp[0]){	// if the first one matches			
							//cout << fmap["Father"];
							/*bool correct = true;
							for(int Itemp = 0; Itemp < temp.size(); Itemp++){ // for the things in temp 	carl $Z
								correct = true;
								for(int Fi = 0; Fi < fmap[FactInQ]->vstring.size(); Fi+= (FactsNum+1)){ 
								// cout << Itemp<<"/"<<temp.size() << endl;
									cout << fmap[FactInQ]->vstring[Fi]<< " vs " << temp[Itemp]<< endl;
									if(fmap[FactInQ]->vstring[Fi] != temp[Itemp]){
										if(temp[Itemp][0] == '$' && correct == true){
											cout <<"found "<< temp[Itemp]<<endl;
											tempVars.push_back(temp[Itemp]);
											// cout<<FactInQ<<endl;
										}
									} else if(fmap[FactInQ]->vstring[Fi] == temp[Itemp]){ // if it matches
										cout<< "found " << temp[Itemp]<< endl;
										tempVars.push_back(temp[Itemp]);
									} else{
										correct = false;
									}
								}
							}*/
						//}
					//}
				}
				
			}
			
			/*
			make a copy of the variables vector
			for each fact in factnames vector
				if its a fact
					capture the variables vectors section for THIS FACT into a new temporary vector <Carl Z>
					compare the non $ values to the facts vector of all the facts		Carl in Father
					if theres a $, save it in a vector?		$Z
					for each of the (Carl, Z)s, 
						plug Z's string into the variables vector where ever there is a Zs 	
									---only z: marcie. after plugging: carl, marcie, marcie, ryan
						
						for each fact in factnames
							evaluate with THIS array ^^
							if you find a fact to be true
								delete it from the array
							if you find a fact to be  false, 
								break?    get back to the for each of the (Carl, Z)s,  loop
					
			*/
			
			
			/*cout << endl;
			if (fmap.count(FactInQ) == 1){ // if its a fact
				for(int i = 1; i < factNames.size(); i+=2){ // for each of the facts
					if(i+2 >= factNames.size()){break;}
					for(int it = 0; it < fmap[FactInQ]->vstring.size();it++){ // for the strings in the facts vstring
						if(variables[RuleVal+it][0] != '$' && fmap[FactInQ]->vstring[it] == variables[RuleVal+it]){ // if its a string and it matches
							// carl and second carl
							
						} else if(variables[RuleVal+it][0] != '$' && fmap[FactInQ]->vstring[it] != variables[RuleVal+it]){
							// for all the remaining facts, check if they have z, and if they do, check if there is that fact for carls child and child,ryan
							
							
						} else if(variables[RuleVal+it][0] == '$'){// if its a variable
							
						}
					}
					
					/*
					iterate through factinQs facts for x carl
						if find x carl
							if variables factnum+i = $var
								variables += factnames[i+1 atoi]
								factnames += 2
								for this new fact
								
								variables vec: carl ryan carl z z ryan
								Carl, Ryan
								example: GrandFather
								factNames vector: 2 Father 2 Parent 2
					*/
				//}
				
			/*} else if (rmap.count(FactInQ) == 1){ // if its another rule
					rmap[FactInQ]->check(rmap,fmap,/*vector of new args);
			} else {
					cout << FactInQ <<" isn't a fact"<<endl;
					return; 
			}*/
		///ready = false;
		cout<<endl;
	}
	
}
	// variables vector:Carl, Ryan, Carl, $Z, $Z, Ryan
	
	/*
	Carl, Ryan
	example: GrandFather
	variables vector: $X $Y $X $Z $Z $Y
	factNames vector: 2 Father 2 Parent 2
	
	check the strings in question, match them to the $Variables ./
	plug them into the facts matching $Variables ./
	check if any of the unassigned $Variables among the facts match ./
	for the matching $Variables, check if the string values match 
		check each fact, same way as in 'or' ./
		if the value of a pred begins with a $ we want to know if its in any other fact we need 
		for the fact vector, if we find strings matching ours, skipping if ours is $ ./
			for those strings,
				for the other facts with $ matching our $
					if they match, return true
	for the solo $Variables, return true if there is a fact with a value in that spot
	if all of those are true, the checker found a result
	
	
	vector rulepreds
	for i < factnames[0]
		rulepreds[i] = variables [i]		// copies rule stuff into a new vector <$X,$Y>
	
	
    */
/*
	for(int i = RuleVal; i<variables.size(); i++){ // for the variables vector (iterating through)
		for(int it = i; it < variables.size(); it++){
			if(variables[i][0] == '$'){ // if its one of the variables
				if(variables[i] == variables[it]){ 	// if they match ($Z and $Z)
					variables[it].erase(0,1);
				}
			}
		}
		if(variables[i][0] == '$'){
			variables[i].erase(0,1);
		}
	}
	cout << "variables vector after: "<<endl;
	for(auto i : variables){
		cout << i << " ";
	}*/
bool Rule::get_logop(){
	return logop;
}

Rule::~Rule(){}	//  destructor
