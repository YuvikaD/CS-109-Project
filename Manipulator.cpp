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
	//vector<string> subjectsVec;
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
	forFilter = "";
	varVec.clear();
	getline(iss2,forFilter);
	iss2.clear();
	iss2.str(forFilter);
	vars = "";
	//cout << "forFilter: " << forFilter << endl;
	while(getline(iss2,vars,',')){
			if(vars != "\n" && vars != "\r" && vars != "" && vars != "\0" && vars !="\r\n"){
				cout << vars << endl;
				varVec.push_back(vars);
				if(vars[0] != '$'){
					filter = true;
				}
			}
	}
	edited.erase(std::remove(edited.begin(), edited.end(), '$'), edited.end());
	stringstream iss(edited);
	getline(iss,leftHandSide,'(');
	///cout << "line " << line  << "edited " << edited << "LHS " << leftHandSide << endl; // DEBUGGING
	
	if(filter){
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
		//if((Rule_map.count(leftHandSide) == 1 && Rule_map[leftHandSide]->get_logop() == "OR") || Fact_map.count(leftHandSide) == 1){
			nofilter(leftHandSide, edited, varVec);
		//}
	}
}

void Manipulator::nofilter(string leftHandSide, string edited, vector<string> varVec){
	cout << "leftHandSide: " << leftHandSide << "edited: " << edited << endl;
	//cout << "printImmediately: " << printImmediately << endl;
	if(Fact_map.count(leftHandSide) == 1){
		//printImmediately = true;
		//cout << "pushed back " << leftHandSide << endl;
		Fact_map[leftHandSide]->userArgs = varVec;
	}

	if(Rule_map.count(leftHandSide) == 1){
		recursions++;
		//cout << leftHandSide << " is a rule." << endl;
		if (Rule_map[leftHandSide]->firstInference == true){
			infRules.push_back(leftHandSide);
			cout << "pushed back " << leftHandSide << endl;
			Rule_map[leftHandSide]->userArgs = varVec;
			Rule_map[leftHandSide]->firstInference = false;
		}
	}
	///for(auto it= infRules.begin(); it != infRules.end(); ++it){cout << "iterating infRules " << *it << endl;}// DEBUGGING CODE
	string it3;
	int count;
	int total;
	bool stopCount;
	bool gotVar;
	vector<string> varPred;
	stringstream iss(edited);
	getline(iss,leftHandSide,'(');
	//vector<string> countVec;
	if(Fact_map.count(leftHandSide) == 1){
		if(Fact_map[leftHandSide]->printed == true){cout << "alrady printed :P" << endl;}
		else{
		string subjects="";
		while(getline(iss,subjects,',')){
			//cout << "SUBJECTS: " << subjects << " ";
			Fact_map[leftHandSide]->countVec.push_back(subjects); // countVec X Y
			//cout << "pushed " << subjects << " into countVec" << endl;
			subjects="";
		}
		int inc=0;
		count=0; // 
		total=0;  //
		int res=0;
		stopCount = false;
		for(auto iter = Fact_map[leftHandSide]->vstring.begin(); iter != Fact_map[leftHandSide]->vstring.end(); ++iter){
			if(*iter == "|"){
				if(printImmediately){
					cout << endl;
				}
				stopCount=true;
			} else {
				if(!stopCount){++count;}
				++total; // printing out facts
				//cout << "total: " << total << "count: " << count << endl;
				if(total%count !=0) {
					//cout << "total%count !=0" << endl;
					//cout << "infRules " << infRules[0] << endl;
					//cout << "infRule.size() " << infRules.size() << " recursions: " << recursions << endl;
					if(infRules.size() >= recursions){ // infRules holds rules.if true, we ran it on a rule with custom args
						//cout << "infRules " << infRules[0] << endl;
						//cout << "seg fault? 5" << endl;
						if(Rule_map[infRules[0]]->userArgs[res][0] == '$'){ // if it's a variable
							if(printImmediately){
								cout << Rule_map[infRules[0]]->userArgs[(total%res)] << ": " << *iter << " ";
							} else{
							//cout << "added in 1" << endl;
							if(!Fact_map[leftHandSide]->readFacts){
								Fact_map[leftHandSide]->andVars.push_back(Fact_map[leftHandSide]->userArgs[(total%res)]);
								Fact_map[leftHandSide]->andVars.push_back(*iter);
								///Fact_map[leftHandSide]->andVars.push_back(",");
							}
							//cout << "recursions: " << recursions << endl;
							//cout << "rule: " << infRules[recursions] << endl;
							//cout << "pushing " << Fact_map[leftHandSide]->userArgs[total%res] << endl;
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[(total%res)]);
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
							///Rule_map[infRules[recursions]]->andVars.push_back(",");
							}
						}
						//cout << "not 5" << endl; // this is where grandfather stuff is done
						// if userArgs has anyhting in it, custom variables like $T,$y
					}else if(Fact_map[leftHandSide]->userArgs.size()>0){	// ran it on a fact with custom args
						//cout << " seg falut incoming? 4" << endl;
						if(printImmediately){
							cout << Fact_map[leftHandSide]->userArgs[(total%res)] << ": " << *iter << " ";
						} else{
							//cout << "added in 2" << endl;
							if(!Fact_map[leftHandSide]->readFacts){
								Fact_map[leftHandSide]->andVars.push_back(Fact_map[leftHandSide]->userArgs[(total%res)]);
								Fact_map[leftHandSide]->andVars.push_back(*iter);
								///Fact_map[leftHandSide]->andVars.push_back(",");
							}
							cout << "recursions: " << recursions << endl;
							cout << "rule: " << infRules[recursions] << endl;
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[(total%res)]);
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
							///Rule_map[infRules[recursions]]->andVars.push_back(",");
						}
						//cout << " no 4" << endl;
					}			
					else{
						//cout << " seg falut incoming? 1" << endl;
						if(printImmediately){
							cout << Fact_map[leftHandSide]->countVec[(total%count)-1] << ": " << *iter << " "; 
						} else{
							///Fact_map[leftHandSide]->andVars.push_back(Fact_map[leftHandSide]->countVec[(total%count)-1]);
							///Fact_map[leftHandSide]->andVars.push_back(*iter);
							
						}
						//cout << " no 1" << endl;
					}
				} else { // if total%count == 0
					if(infRules.size() >= recursions){ // was > 0
						//cout << "infRules " << infRules[0] << endl;
						if (Rule_map[infRules[0]]->userArgs[res][0] == '$'){
							if(printImmediately){
								cout << Rule_map[infRules[0]]->userArgs[res] << ": " << *iter << " ";
							} else{
								//cout << "pushing to vector...LHS: " << leftHandSide << endl;
								//cout << "added in 3" << endl;
							if(!Fact_map[leftHandSide]->readFacts){
								Fact_map[leftHandSide]->andVars.push_back(Fact_map[leftHandSide]->userArgs[count-1]);
								Fact_map[leftHandSide]->andVars.push_back(*iter);
								///Fact_map[leftHandSide]->andVars.push_back(",");
							}
							//cout << "recursions: " << recursions << endl;
							//cout << "rule: " << infRules[recursions] << endl;
							//cout << "pushing " << Fact_map[leftHandSide]->userArgs[count-1] << endl;
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[count-1]);
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
							///Rule_map[infRules[recursions]]->andVars.push_back(",");
							//cout << "succ suceess" << endl;
							}
						}
					}else if(Fact_map[leftHandSide]->userArgs.size()>0){	// ran it on a fact with custom args
						//cout << " seg falut incoming? 3" << endl;
						if(printImmediately){
							cout << Fact_map[leftHandSide]->userArgs[count-1] << ": " << *iter << " ";
						} else{
							//cout << "added in 4" << endl;
							if(!Fact_map[leftHandSide]->readFacts){
								Fact_map[leftHandSide]->andVars.push_back(Fact_map[leftHandSide]->userArgs[count-1]);
								Fact_map[leftHandSide]->andVars.push_back(*iter);
								////Fact_map[leftHandSide]->andVars.push_back(",");
							}
							//cout << "recursions: " << recursions << endl;
							//cout << "rule: " << infRules[recursions] << endl;
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[count-1]);
							Rule_map[infRules[recursions]]->andVars.push_back(" "); // ADDING SPCACE
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
							///Rule_map[infRules[recursions]]->andVars.push_back(",");
						}
						//cout << " no 3" << endl;
					}else{
						//cout << " seg falut incoming? 2" << endl;
						if(printImmediately){
							cout << Fact_map[leftHandSide]->countVec[count-1] << ": " << *iter<<" "; 
						} else{
							//cout << "added in 5" << endl;
							///Fact_map[leftHandSide]->andVars.push_back(Fact_map[leftHandSide]->userArgs[count-1]);
							///Fact_map[leftHandSide]->andVars.push_back(*iter);
							//cout << "recursions: " << recursions << endl;
							//cout << "rule: " << infRules[recursions] << endl;
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[count-1]);
							Rule_map[infRules[recursions]]->andVars.push_back(" ");
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
							///Rule_map[infRules[recursions]]->andVars.push_back(",");
						}
						//cout << " no 2" << endl;
					}
				}
			}
			if(infRules.size() > 0){
				if (res+1 < Rule_map[infRules[0]]->userArgs.size()){	// for a rule
				++res;	// increment which custom user argument we will look at, $X -> $Y
				}
			} else if(res+1 < Fact_map[leftHandSide]->userArgs.size()){ // for a fact
				++res;	// increment which custom user argument we will look at, $X -> $Y
				}
		}
		if(printImmediately){
			cout << endl;
		}
		Fact_map[leftHandSide]->printed = true;
		Fact_map[leftHandSide]->readFacts = true;
		}
	}

	if(Rule_map.count(leftHandSide) == 1){ //calls inference on rule's preds
		if (Rule_map[leftHandSide]->get_logop() == "OR"){
			cout << "GOT LOGOP: " << Rule_map[leftHandSide]->get_logop() << endl;
				ofstream fstor;
				for(auto iter = Rule_map[leftHandSide]->infVector.begin(); iter != Rule_map[leftHandSide]->infVector.end(); ++iter){
					cout << "infVector " << *iter << endl;
					////////////////////////////// The Following block lets us print the same fact multiple times if it
					string resetPrint = "";		// appears multiple times in a rule. 
					stringstream iss (*iter);
					getline(iss,resetPrint,'(');
					//cout << "printed? " << resetPrint << endl;
					if (Fact_map.count(resetPrint) ==1){
						Fact_map[resetPrint]->printed = false;			
					}
					//////////////////////////////
					
					fstor.open("write.txt");
					fstor << *iter;	///cout << *iter << endl;
					fstor << '\n';
					fstor.close();
					inference("write.txt");
				}
			}
			else if (Rule_map[leftHandSide]->get_logop() == "AND"){
				printImmediately = false;
				cout << "GOT LOGOP: " << Rule_map[leftHandSide]->get_logop() << endl;
				// call a new function that does cool stuff
				// iterate thru RuleVector of this rule, We wanna get $X's and $Y's 
				cout << "adding stuff to vector" << endl;
				for(auto iter = Rule_map[leftHandSide]->RuleVector.begin(); iter != Rule_map[leftHandSide]->RuleVector.end(); ++iter){
					for(auto iter2 = iter->begin(); iter2 != iter->end(); ++iter2){ // iterating thru p_vector
						vector<string> dollarVars;
						for(auto iter3 = iter2->begin(); iter3 != iter2->end(); ++iter3){ // iterating thru strings
							//dollarVars.clear();
							if((*iter3)[0] == '$'){ // if the string is a '$' variable
								cout << *iter3 << " ";
								dollarVars.push_back(*iter3);
							}
						}
						Rule_map[leftHandSide]->dollarVarsVec.push_back(dollarVars); // put little vectors in big one
						dollarVars.clear();	// clear little vector for next loop
					cout << '\t';
					}
				}
				cout << endl;
				ofstream fstor;
				for(auto iter = Rule_map[leftHandSide]->infVector.begin(); iter != Rule_map[leftHandSide]->infVector.end(); ++iter){
					////////////////////////////// The Following block lets us print the same fact multiple times if it
					string resetPrint = "";		// appears multiple times in a rule. 
					stringstream iss (*iter);
					getline(iss,resetPrint,'(');
					if (Fact_map.count(resetPrint) ==1){
						Fact_map[resetPrint]->printed = false;
					}
					//////////////////////////////
					cout << "gonna inf this  " << *iter << endl;
					fstor.open("write.txt");
					fstor << *iter; //cout << *iter << endl;
					fstor << '\n';
					fstor.close();
					inference("write.txt");
				}
				
				cout << endl << "printing from newly made dollarVarsVec:" << endl;
				for(auto iter = Rule_map[leftHandSide]->dollarVarsVec.begin(); iter != Rule_map[leftHandSide]->dollarVarsVec.end(); ++iter){
					for(auto iter2 = iter->begin(); iter2 != iter->end(); ++iter2)
						cout << *iter2 << " ";
				}
				cout << endl;
				//cout << "LHS at end: " << leftHandSide << endl; // GrandFather
				//for(auto iter = Rule_map[leftHandSide]->infVector.begin(); iter != Rule_map[leftHandSide]->infVector.end(); ++iter){
				//}
				///cout << "predNames of " << Rule_map[leftHandSide];
				
				for(auto iter = Rule_map[leftHandSide]->predNames.begin(); iter != Rule_map[leftHandSide]->predNames.end(); ++iter){
					 cout << "Rule or Fact andVars vector: " << *iter << '\t';
					 // print out Father's andvars
					 if (Rule_map.count(*iter) ==1){
						 //cout << "rule" << endl;
						for (auto iter2 = Rule_map[*iter]->andVars.begin(); iter2 != Rule_map[*iter]->andVars.end(); ++iter2){
							cout << *iter2;
						}
					 } else if (Fact_map.count(*iter) ==1){
						 //cout << "fact" << endl;
						for (auto iter2 = Fact_map[*iter]->andVars.begin(); iter2 != Fact_map[*iter]->andVars.end(); ++iter2){
							cout << *iter2;
						}
					 }
					 cout << endl;
				}
				cout << endl;
				vector<string> VARVECTOR;
				//vector<string> VARVECTOR2;
				string VARSTRING;
				string VARSTRING2;
				string VAR = "";
				int i =0;
				for(auto iter = Rule_map[leftHandSide]->predNames.begin(); iter != Rule_map[leftHandSide]->predNames.end(); ++iter){
					if (Rule_map.count(*iter) == 1){ // rule
						for (auto iter2 = (Rule_map[*iter])->andVars.begin(); iter2 != (Rule_map[*iter])->andVars.end(); ++iter2){
							++i;
							VARSTRING += (*iter2);
							if (i==2){
								VARVECTOR.push_back(VARSTRING);
								i=0;
								VARSTRING = "";
							}
						}
					 } else if (Fact_map.count(*iter) == 1){ // fact
						for (auto iter2 = Fact_map[*iter]->andVars.begin(); iter2 != Fact_map[*iter]->andVars.end(); ++iter2){
							++i;
							VARSTRING += (*iter2);
							if (i==2){
								VARVECTOR.push_back(VARSTRING);
								i=0;
								VARSTRING = "";
							}
						}
					 }

					if(VARVECTOR.size() > 0) {cout << "VARVECTOR: ";}
					for( auto iter = VARVECTOR.begin(); iter != VARVECTOR.end(); ++iter){
						cout << *iter;
					}
					cout << endl;
					//<< "infRules[0]: " << infRules[0] << endl;
					Rule_map[infRules[0]]->Results.push_back(VARVECTOR);
					VARVECTOR.clear();	// MAKE SURE TO CLEAR VARVECTOR FOR EACH NEW LOOOOPER
				}
				cout << "segg" << endl;
				 
				int j = 0;
				cout << "infRules[0]: " << infRules[0] << endl;;
				for(auto it = Rule_map[infRules[0]]->Results.begin(); it != Rule_map[infRules[0]]->Results.end(); ++it){
					//cout << "predname: " << Rule_map[infRules[0]]->predNames[j] << endl;
					if(Rule_map.count(Rule_map[infRules[0]]->predNames[j]) == 1 || Fact_map.count(Rule_map[infRules[0]]->predNames[j]) == 1){
						cout << Rule_map[infRules[0]]->predNames[j] << ": ";
					}
					for(auto it2 = it->begin(); it2 != it->end(); ++it2){
						cout << *it2 << " ";
					}
					++j;
					cout << endl;
				}
				
				for (auto it = Rule_map[infRules[0]]->Results.begin(); it != Rule_map[infRules[0]]->Results.end(); ++it){
					for(auto it1 = Rule_map[infRules[0]]->Results.begin() + 1; it1 != Rule_map[infRules[0]]->Results.end(); ++it1){
						for (auto it2 = it->begin(); it2 != it->end(); ++it2){
							for (auto it3 = it1->begin(); it3 != it1->end(); ++it3){
								if(*it2 == *it3) { cout << " ITS A MATCHJH " << *it2 << " == " << *it3 << endl;}
								cout << "seggyloop" << endl;
							}
						}
					}
				}
				
				/*
				for( auto iter = VARVECTOR.begin(); iter != VARVECTOR.end(); ++iter){
					cout << "SEARCHING FOR " << *iter << " IN VECTOR2" << endl;
					for (auto iter2 = VARVECTOR2.begin(); iter2 != VARVECTOR2.end(); ++iter2){
						if (*iter == *iter2) {
							cout << "OH SHIT " << *iter << " == " << *iter2 << endl;
							--iter;
							++iter2;
							cout << "FINAL RESULTS OF THE RULE: " << endl;
							
							string editString="";
							for (int c = 0; c < (*iter).length(); c++){
								if(c==0){
									//cout<<(*iter)[c];
									editString += (*iter)[c];
								} else if (c==1){
									//cout<<(*iter)[c]<<": ";	
									editString += ((*iter)[c]);
									editString += (": ");
								}
								else {
									//cout<<(*iter)[c];
									editString += (*iter)[c];
								}
								
							}cout<<'\t'<< editString << '\t';
							
							editString = "";
							for (int c = 0; c < (*iter2).length(); c++){ // change when we remove hardcoded make a loops
								if(c==0){
									//cout<<(*iter2)[c];
									editString += (*iter2)[c];
								} else if (c==1){
									//cout<<(*iter2)[c]<<": ";
									editString += ((*iter2)[c]);
									editString += (": ");
								}
								else {
									//cout<<(*iter2)[c];
									editString += (*iter2)[c];
								}
								
							}cout<<'\t'<< editString << '\t' << endl;
							
							
							//cout << *iter << '\t' << *iter2 << endl;
							++iter;
							--iter2;
							}
						else {}//{cout << "no findo";}
					}
				}
				*/
				
				
				cout << "seggy" << endl;
				//auto itsearch = VARVECTOR.begin();
				/*
				for (auto ittarget = VARVECTOR.begin(); ittarget != VARVECTOR.end(); ++ittarget){
					if(*ittarget != ""){
					if((*ittarget)[0] == '$'){
						cout << "ittarget: " << *ittarget << " ";
						//if(*itsearch != "|"){itsearch = find(VARVECTOR.begin(),VARVECTOR.end(),"|");}
						auto itsearch = find(VARVECTOR2.begin(),VARVECTOR2.end(),*ittarget);
						if (itsearch != VARVECTOR2.end() && *(++itsearch) == *(++ittarget)){
							cout << "itsearch found" << *ittarget << endl;
							//++ittarget;
							//auto pos = itsearch - VARVECTOR2.begin() ;
							//cout << "pos " << pos << endl;
							//itsearch = find(VARVECTOR2.begin()+pos,VARVECTOR2.end(),*ittarget);
							if(itsearch != VARVECTOR2.end()){
								cout << *ittarget << " matches something" << endl;
								ittarget -= 3;
								cout << *ittarget << ": ";
								++ittarget;
								cout << *ittarget;
								ittarget += 2;
								
								++itsearch;
								cout << *itsearch << ": ";
								++itsearch;
								cout << *itsearch << ": ";
								cout << endl;
							}
						} else{ cout << endl;}
					if(itsearch != VARVECTOR.end()){cout << "WOW" << *ittarget << "==" << *itsearch << endl;}
					
					}
					}
				}
				*/
				cout << endl << "seggy" << endl;
				
				
				
				
				
				Rule_map[leftHandSide]->dollarVarsVec.clear(); // clear the vector for next inf of same rule
				
			}
		}
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
				while(getline(iss,data,' ')){	// data is stuff like Father($X,$Y)
					//cout << "data" << data << endl;
					Rule_map[name3]->infVector.push_back(data); ///NEW, do for if rule exists!!!
				stringstream iss4(data);
				getline(iss4,predName,'(');
					Rule_map[name3]->predNames.push_back(predName); /// NEW
				vector<string> second_vector;
				second_vector.push_back(predName);
				///cout << "predName " << predName << endl;
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
					Rule_map[name3]->predNames.push_back(predName); /// NEW
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
}
