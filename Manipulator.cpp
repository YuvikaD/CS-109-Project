#include "Manipulator.h"
using namespace std;
Manipulator:: Manipulator(){}	// constructor
Manipulator::~Manipulator(){}	// constructor 

void Manipulator::drop(string name){
	if(Fact_map.find(name)!=Fact_map.end()) Fact_map.erase(name);
	if(Rule_map.find(name)!=Rule_map.end()) Rule_map.erase(name);
	cout << "dropped" << endl;		
}

void Manipulator::dumpFacts(ostream &os){
for(auto it = Fact_map.begin(); it != Fact_map.end(); ++it) {
		os << it->second;
    }
}

void Manipulator::dumpRules(ostream &os){
	for(auto it = Rule_map.begin(); it != Rule_map.end(); ++it) {
		os << it->second;
    }
}
void Manipulator::dump(){
	ofstream fout;
	fout.open("output.txt");
	dumpFacts(fout);
	dumpRules(fout);
}

void Manipulator::inference(string filename){
	// make LHS empty or itll mess stuff up later
	bool filter = false;
	string forFilter, temp, vars;
	vector<string> varVec; // for filtering
	vector<string> subjectsVec;
	string leftHandSide = "";	
	ifstream readFile(filename);
	getline(readFile,line);
 while ( line.find ("\r") != string::npos )
    {
        line.erase ( line.find ("\r"), 1 );
    }
	string edited = line;
	edited.erase(std::remove(edited.begin(), edited.end(), ' '), edited.end()); // erases spaces????
	//edited.erase(std::remove(edited.begin(), edited.end(), ')'), edited.end()); // erases spaces????
	replace( edited.begin(), edited.end(), ')', ',');
	
	//stringstream iss2(edited);
	stringstream iss2(edited);
	getline(iss2,temp,'(');
	//iss2.clear();
	//iss2.str(temp);
	//getline(iss2,forFilter,',');
	
	forFilter = "";
	varVec.clear();
	getline(iss2,forFilter);
	iss2.clear();
	iss2.str(forFilter);
	vars = "";
	//cout << "forFilter: " << forFilter << endl;
	while(getline(iss2,vars,',')){
		//cout << "vars: " << vars << " " << endl;
			if(vars != "\n" && vars != "\r" && vars != "" && vars != "\0" && vars !="\r\n"){
				varVec.push_back(vars);
				if(vars[0] != '$'){
					filter = true;
				}
			}
	}
	//cout << endl;
	edited.erase(std::remove(edited.begin(), edited.end(), '$'), edited.end());
	stringstream iss(edited);
	getline(iss,leftHandSide,'(');
	///cout << "line " << line << endl;
	///cout << "edited " << edited << endl;
	///cout << "LHS " << leftHandSide << endl;
	
	if(filter){ //if its filtered
		if(Fact_map.count(leftHandSide) == 1){ // if its a fact
			factFilter(leftHandSide, Fact_map, varVec);
			return;
		} else { // if its a rule
			//Rule_map[leftHandSide]->check(Rule_map,Fact_map, varVec);
			ruleFilter(leftHandSide, Rule_map, Fact_map, varVec);
		}
	}
	if(!filter){
		//// RYAN FUNCTION for ($X,$Y) type stuff
		nofilter(leftHandSide, edited);
		cout<<endl;
	}
	
}

void Manipulator::nofilter(string leftHandSide, string edited){
	string it3;
	int count;
	int total;
	bool stopCount;
	bool gotVar;
	stringstream iss(edited);
	getline(iss,leftHandSide,'(');
	//vector<string> countVec;
	if(Fact_map.count(leftHandSide) == 1){
		if(Fact_map[leftHandSide]->printed == true){}
		else{
			string subjects="";
			while(getline(iss,subjects,',')){
				///cout << "SUBJECTS: " << subjects << " ";
				Fact_map[leftHandSide]->countVec.push_back(subjects); // countVec X Y
				///cout << "pushed " << subjects << " into countVec" << endl;
				subjects="";
			}
			int inc=0;
			count=0; // 
			total=0;  // 
			stopCount = false;
			for(auto iter = Fact_map[leftHandSide]->vstring.begin(); iter != Fact_map[leftHandSide]->vstring.end(); ++iter){
				if(*iter == "|"){
					cout << endl;
					stopCount=true;
				} else {
					if(!stopCount){++count;}
					++total;
					//cout << "total: " << total << "count: " << count << endl;
					if(total%count !=0) {
						///cout << "total%count !=0" << endl;
						cout << Fact_map[leftHandSide]->countVec[(total%count)-1] 	<< ": " << *iter << " ";
						//Fact_map[it3]->subject.push_back(Fact_map[it3]->countVec[(total%count)]);
						//Fact_map[it3]->subject.push_back(s);
					} else {
						//cout << "total: " << total << "count: " << count << endl;
						/////cout << "total%count ==0" << endl;
						cout << Fact_map[leftHandSide]->countVec[count -1] 			<< ": " << *iter<<" ";
						//Fact_map[it3]->subject.push_back(Fact_map[it3]->countVec[count]);
						//Fact_map[it3]->subject.push_back(s);	// this vector will have like $X, Allen, $Z, Marget
					}
				}
			}
			//cout << endl;
			Fact_map[leftHandSide]->printed = true;
		}
		//cout<<endl;
	}
	
	if(Rule_map.count(leftHandSide) == 1){
		
		ofstream fstor;
		for(auto iter = Rule_map[leftHandSide]->infVector.begin(); iter != Rule_map[leftHandSide]->infVector.end(); ++iter){
			///cout << *iter << endl;
			fstor.open("write.txt");
			fstor << *iter;
			fstor << '\n';
			fstor.close();
			inference("write.txt");
		}
	}
}

void Manipulator::evaluate(string line, Rule * rule, map<string,Fact*> fmap, map<string,Rule*> rmap){
}

void Manipulator::load(string filename){
ifstream readFile(filename);
while(getline(readFile,line)){	// read from input file, put contents into 'line' string
		stringstream iss(line);		// put contents of line into a ss object
		getline(iss,leftHandSide,')');	// read from ss object, put contents into 'leftHandSide' string until ')' char is read
		getline(iss,separator,' ');
		stringstream iss2(line);
		stringstream iss3(leftHandSide);
		
		if(separator == ":-"){	// Rule
			getline(iss2,name,':');
			getline(iss,logop,' ');	// get logop
			vector<vector<string>> p_vector;
			stringstream iss6(name);
			getline(iss6,name3,'(');
			getline(iss6,temp2,')');
			stringstream iss5(temp2);

			if(Rule_map.count(name3) != 1){			
				vector<string> first_vector;
				Rule * r = new Rule(logop);	
				Rule_map[name3] = r;
				first_vector.push_back(name3);				
				while (getline(iss5,temp,',')){				
					first_vector.push_back(temp);			
				}
				p_vector.push_back(first_vector);
				while(getline(iss,data,' ')){
					Rule_map[name3]->infVector.push_back(data); ///NEW, do for if rule exists!!!
				stringstream iss4(data);
				getline(iss4,predName,'(');
				vector<string> second_vector;
				second_vector.push_back(predName);
				//cout << "predName " << predName << endl;
				(getline(iss4,rightHandSide,')'));
				stringstream iss5(rightHandSide);
				while(getline(iss5,preds,',')){
					second_vector.push_back(preds);
					//cout << "preds " << preds << endl;
				}	
				p_vector.push_back(second_vector);			
			}
			r->RuleVector.push_back(p_vector);
			}
			
			else if(Rule_map.count(name3) == 1){
				vector<string> first_vector;
				vector<string> space_vector;
				vector<vector<string>> space_vector2;
				space_vector.push_back(space);
				space_vector2.push_back(space_vector);
				
				
				first_vector.push_back(name3);				
				//cout << "name3 " << name3 << endl <<endl;
				while (getline(iss5,temp,',')){				
					first_vector.push_back(temp);			
				}
				p_vector.push_back(first_vector);
				while(getline(iss,data,' ')){
					Rule_map[name3]->infVector.push_back(data); /// NEW
				stringstream iss4(data);
				getline(iss4,predName,'(');
				vector<string> second_vector;
				second_vector.push_back(predName);
				//cout << "predName " << predName << endl;
				(getline(iss4,rightHandSide,')'));
				stringstream iss5(rightHandSide);
				while(getline(iss5,preds,',')){
					second_vector.push_back(preds);
					//cout << "preds " << preds << endl;
				}
				
				
				p_vector.push_back(second_vector);
			}

				Rule_map.find(name3)->second->RuleVector.push_back(space_vector2);
				Rule_map.find(name3)->second->RuleVector.push_back(p_vector);
				//cout << "HELLO" << endl;
		}
		
		} else {	// Fact
			getline(iss3,name,'(');	
			if(Fact_map.count(name) != 1){ 	// If this Fact is not in the map
				Fact * f = new Fact(name);	// make fact object, parameter ex: "Father"
				Fact_map[name] = f;
				while(getline(iss3,data,',')){	// add contents to the Fact object's vector, separated by ','
					f->vstring.push_back(data);
				}
			}
			
			else if(Fact_map.count(name) == 1){ // If this Fact name already exists in the map
				Fact_map.find(name)->second->vstring.push_back(space);
				while(getline(iss3,data,',')){	// add contents to the Fact object's vector, separated by ','
					Fact_map.find(name)->second->vstring.push_back(data);
				}
			}
		}
	}
	cout<<endl<<endl;
}

void Manipulator::factFilter(string FactInQ, map<string,Fact*> fmap, vector<string> argVec){
	bool resultFound = true;
	//cout << fmap[FactInQ]<<endl;
	for(int i = 0; i < fmap[FactInQ]->vstring.size(); i++){ // checks if strings match, iterating through facts vstring
		for(int varLimit = 0; varLimit < argVec.size(); varLimit++){// for marcie, ryan
			//cout << "i = " << i << " --- " << fmap[FactInQ]->vstring[i] << " vs "<<argVec[varLimit]<<endl;
			// if they match, and its the last string to check 
			bool vars = false;
			for(int VarC = 0; VarC < argVec.size(); VarC++){
					if(argVec[VarC][0] == '$'){
						vars = true; // if theres at least one $, its paritally filtered
					}
				}
			bool alright = (vars || (varLimit+1==argVec.size()));
			bool equals = (fmap[FactInQ]->vstring[i] == argVec[varLimit]);
			//bool sizes = (i-varLimit+argVec.size() <=  fmap[FactInQ]->vstring.size());	
			if(fmap[FactInQ]->vstring[i] == argVec[varLimit] && resultFound && i-varLimit+argVec.size() <=  fmap[FactInQ]->vstring.size()  && alright){
				bool legit = true; // keeps track of if the fact is correct
				for(int res = 0; res < argVec.size(); res++){
					if(fmap[FactInQ]->vstring[i - varLimit + res] == "|"){
						legit = false; // maybe seg fault causing
					}
				}
				if(legit){ // PRINTING: 
					for(int res = 0; res < argVec.size(); res++){ // for each thing in argvec
						//cout << "index "<<i - varLimit + res<<endl;
						if(argVec[res][0]=='$'){						// if its a variable, print as is
							cout << argVec[res] << ": ";
						} else {
							cout << "$" << ": ";		// otherwise, use the predefined word
						}
						cout << fmap[FactInQ]->vstring[i - varLimit + res];
						if(res+1!=argVec.size()){cout<< ", ";}	// comma to separate - just following karims example
					}
					cout << endl;
					//cout<<"ending "<< FactInQ <<" fact search"<<endl;
					if(!vars){ // if its completely filtered, return now (to ignore duplicates)
						cout<<endl;
						return;
					}
				}							
			} else if(fmap[FactInQ]->vstring[i] == "|"){
				resultFound = false;
			} else if(fmap[FactInQ]->vstring[i] != argVec[varLimit] && !vars && equals){
				//if(i>5){cout<<"setting resultFound to false, "<<fmap[FactInQ]->vstring[i] << " != " << argVec[varLimit]<<endl;}
				resultFound = false;
			} else if(fmap[FactInQ]->vstring[i] == argVec[varLimit] || vars){
				//i++; // this causes seg fault
			} 
		}
		resultFound = true;
	}
}

void Manipulator::ruleFilter(string rule, map<string,Rule*> rmap,map<string,Fact*> fmap, vector<string> argVec){
	vector<string> variables;
	vector<string> factNames;
	makeVecs(rule, variables, factNames, rmap);
	//some debugging code: 
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
	if(rmap[rule]->get_logop() == "OR"){ // is it OR
		bool numOrString = 0;	// keeps track of if you're parsing the number or the fact name
		bool vars = false; // checks if its completely filtered or partially filtered
		int RuleVal = 0;	// the rule's predicate #
		string FactInQ = ""; // Fact in Question - fact we are going to look at
		int FactsNum = 1; // the number of predicates the Fact in Question has 
		bool ready = false; // bool for if its ready to iterate through the fact
		for(int i = 0; i < factNames.size(); i++){ // for each: 3 Father 2 Mother 2
			if(i == 0){ // if its the first number in factNames
				RuleVal = atoi(factNames[0].c_str()); // then it is the Fact's predicate amount
				for(int VarC = 0; VarC < argVec.size(); VarC++){
					if(argVec[VarC][0] == '$'){
						vars = true; // if theres at least one $, its paritally filtered
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
			}   if (ready){ // when you have a fact type and it's param #
				if(fmap.count(FactInQ) == 1){ // if its a fact
					// more debugging:
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
								//bool sizes = (i-varLimit+argVec.size() <=  fmap[FactInQ]->vstring.size());	
								if(fmap[FactInQ]->vstring[i] == argVec[varLimit] && resultFound && i-varLimit+argVec.size() <= fmap[FactInQ]->vstring.size()  && alright){
									bool legit = true; // keeps track of if the fact is correct
									for(int res = 0; res < argVec.size(); res++){
										if(fmap[FactInQ]->vstring[i - varLimit + res] == "|"){
											legit = false; // maybe seg fault causing
										}
									}
									if(legit){ // PRINTING: 
										for(int res = 0; res < argVec.size(); res++){ // for each thing in argvec
											//cout << "index "<<i - varLimit + res<<endl;
											if(argVec[res][0]=='$'){						// if its a variable, print as is
												cout << argVec[res] << ": ";
											} else {
												cout << variables[res][1] << ": ";		// otherwise, use the predefined word
											}
											cout << fmap[FactInQ]->vstring[i - varLimit + res];
											if(res+1!=argVec.size()){cout<< ", ";}	// comma to separate - just following karims example
										}
										cout << endl;
										//cout<<"ending "<< FactInQ <<" fact search"<<endl;
										if(!vars){ // if its completely filtered, return now (to ignore duplicates)
											cout<<endl;
											return;
										}
									}							
								} else if(fmap[FactInQ]->vstring[i] == "|"){
									resultFound = false;
								} else if(fmap[FactInQ]->vstring[i] != argVec[varLimit] && !vars && equals){
									//if(i>5){cout<<"setting resultFound to false, "<<fmap[FactInQ]->vstring[i] << " != " << argVec[varLimit]<<endl;}
									resultFound = false;
								} else if(fmap[FactInQ]->vstring[i] == argVec[varLimit] || vars){
									//i++; // this causes seg fault
								} 
							}
							resultFound = true;
						}
						// if its not found:
						//cout<<"ending "<< FactInQ <<" fact search"<<endl;
						FactsNum = 0; // resets these for the next Fact Type
						numOrString = 1;
						going = false;
					}
					ready = false;
				} else if (rmap.count(FactInQ) == 1){ // if its another rule
					//rmap[FactInQ]->check(rmap,fmap,argVec);
					ruleFilter(FactInQ,rmap,fmap,argVec);
				} else {
					cout << "Error with Fact/Rule " << FactInQ << endl;
				}
			}
		}
	}
	cout << endl;
}

void Manipulator::makeVecs(string rule, vector<string> &variables, vector<string> &factNames,map<string,Rule*> rmap){
	int loops = 0;
	int maincount = 0;
	int count = 0; // count for dealing with rule's stuff
	int varcounter = 0; // counter of # of facts params
	for(vector<vector<string>> j : rmap[rule]->RuleVector){
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
					if (s[0] == '$'){ // if its a variable
						varcounter ++;
						variables.push_back(s); // add to variables vector
						if(loops == i.size() &&  maincount == j.size()){
							factNames.push_back(std::to_string(varcounter));
						}
					} else { // if its not a variable
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