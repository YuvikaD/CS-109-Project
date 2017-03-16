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
	bool filter = false;	
	string forFilter, temp, vars; // temp for storing stringstream data temporarily, vars for getting the default or user inputted vars $X $Y
	vector<string> varVec; // for filtering
	string leftHandSide = "";	// leftHandSide will have the name of the rule/fact we inference, like "GrandFather"
	ifstream readFile(filename);
	getline(readFile,line);
 while ( line.find ("\r") != string::npos ){ // erase invisible carriage return characters
        line.erase ( line.find ("\r"), 1 );
    }
	string edited = line;
	edited.erase(std::remove(edited.begin(), edited.end(), ' '), edited.end()); // erases spaces????
	replace( edited.begin(), edited.end(), ')', ','); // replace the last bracket with a comma to simplify parsing
	
	stringstream iss2(edited);
	getline(iss2,temp,'(');
	forFilter = "";
	varVec.clear();
	getline(iss2,forFilter);
	iss2.clear();
	iss2.str(forFilter);
	vars = "";
	while(getline(iss2,vars,',')){
			if(vars != "\n" && vars != "\r" && vars != "" && vars != "\0" && vars !="\r\n"){
				//cout << vars << endl; // debugging
				varVec.push_back(vars);
				if(vars[0] != '$'){
					filter = true;
				}
			}
	}
	edited.erase(std::remove(edited.begin(), edited.end(), '$'), edited.end());
	stringstream iss(edited);
	getline(iss,leftHandSide,'('); // leftHandSide now has the name of the rule/fact we inference, like "GrandFather"
	//cout << "line " << line  << "edited " << edited << "LHS " << leftHandSide << endl; // DEBUGGING
	
	if(filter){
		if(Fact_map.count(leftHandSide) == 1){ // if its a fact
			factFilter(leftHandSide, Fact_map, varVec);
			return;
		} else { // if its a rule
			ruleFilter(leftHandSide, Rule_map, Fact_map, varVec);
		}
	}
	if(!filter){
			nofilter(leftHandSide, edited, varVec);	// Inferencing FUNCTION for ($X,$Y) type rules and facts
	}
}

void Manipulator::nofilter(string leftHandSide, string edited, vector<string> varVec){	// Inferencing FUNCTION for ($X,$Y) type rules and facts
	int threadID = tCount;
	//cout<<"THREAD COUNT: "<< tvec.capacity() << endl;
	if(threadID > 0) cout << "Thread " << threadID << " started" << endl;
	//cout << "leftHandSide: " << leftHandSide << "edited: " << edited << endl; // debugging
	//cout << "printImmediately: " << printImmediately << endl;	// debugging
	if(Fact_map.count(leftHandSide) == 1){
		//cout << "pushed back " << leftHandSide << endl; // debugging
		Fact_map[leftHandSide]->userArgs = varVec;	// load user or default vars like $X $Y
	}

	if(Rule_map.count(leftHandSide) == 1){	//cout << leftHandSide << " is a rule." << endl;
		recursions++; // keep track of how many Rules deep we have recursed into inference
		if (Rule_map[leftHandSide]->firstInference == true){
			infRules.push_back(leftHandSide);
			//cout << "pushed back " << leftHandSide << endl; // debugging
			Rule_map[leftHandSide]->userArgs = varVec;
			Rule_map[leftHandSide]->firstInference = false;
		}
	}
	///for(auto it= infRules.begin(); it != infRules.end(); ++it){cout << "iterating infRules " << *it << endl;}// debugging
	stringstream iss(edited); // edited is an edited version of the thing we are inferencing
	getline(iss,leftHandSide,'(');
	if(Fact_map.count(leftHandSide) == 1){
		if(Fact_map[leftHandSide]->printed == true){
			//cout << "alrady printed :P" << endl;
		} // debugging
		else{
		string subjects="";
		while(getline(iss,subjects,',')){ // subjects has the vars like  X Y for Father
			//cout << "SUBJECTS: " << subjects << " "; // debugging
			Fact_map[leftHandSide]->countVec.push_back(subjects); // countVec  holds subjects, like X Y for Father
			//cout << "pushed " << subjects << " into countVec" << endl; // debugging
			subjects="";	// reset the subjects string so we can reuse it
		}
		int count=0; // 
		int total=0;  //
		int res=0;
		bool stopCount = false; // stop the count when we've reached a number equal to the number of vars like $X $Y is 2
		mtx.lock();
		//cout<< "locked!"<<endl;
		for(auto iter = Fact_map[leftHandSide]->vstring.begin(); iter != Fact_map[leftHandSide]->vstring.end(); ++iter){
			if(*iter == "|"){	// the separator between facts tells us when to stop counting vars
				if(printImmediately){ // printImmediately is it's a fact or an OR rule
					cout << endl;
				}
				stopCount=true;
			} else {
				if(!stopCount){++count;}
				++total;// total is how many fact results we have that are not a '|' separator 
				// printing out facts
				if(total%count !=0) {	// decide which varaible we want to go with our fact output, default vars like $X $Y or user inputted vars like $vArS
					if(infRules.size() >= recursions){ // infRules holds rules.if true, we ran it on a rule with custom args
						//cout << "seg fault? 5" << endl;
						if(Rule_map[infRules[0]]->userArgs[res][0] == '$'){ // if it's a variable
							if(printImmediately){
								cout << Rule_map[infRules[0]]->userArgs[(total%res)] << ": " << *iter << " ";
							} else{
							//cout << "added in 1" << endl;
							if(!Fact_map[leftHandSide]->readFacts){
								Fact_map[leftHandSide]->andVars.push_back(Fact_map[leftHandSide]->userArgs[(total%res)]);
								Fact_map[leftHandSide]->andVars.push_back(*iter);
							}
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[(total%res)]);
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
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
							}
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[(total%res)]);
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
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
								//cout << "added in 3" << endl;
							if(!Fact_map[leftHandSide]->readFacts){
								Fact_map[leftHandSide]->andVars.push_back(Fact_map[leftHandSide]->userArgs[count-1]);
								Fact_map[leftHandSide]->andVars.push_back(*iter);
	
							}
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[count-1]);
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
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
							}
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[count-1]);
							Rule_map[infRules[recursions]]->andVars.push_back(" "); // ADDING SPCACE
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
						}
					}else{
						//cout << " seg falut incoming? 2" << endl;
						if(printImmediately){
							cout << Fact_map[leftHandSide]->countVec[count-1] << ": " << *iter<<" "; 
						} else{
							Rule_map[infRules[recursions]]->andVars.push_back(Rule_map[infRules[recursions]]->userArgs[count-1]);
							Rule_map[infRules[recursions]]->andVars.push_back(" ");
							Rule_map[infRules[recursions]]->andVars.push_back(*iter);
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
		//		cout<<endl<<"unlocked!";
		mtx.unlock();
		if(printImmediately){
			cout << endl;
		}
		Fact_map[leftHandSide]->printed = true;
		Fact_map[leftHandSide]->readFacts = true;
		}
	}

	if(Rule_map.count(leftHandSide) == 1){ //calls inference on rule's preds
		if (Rule_map[leftHandSide]->get_logop() == "OR"){	// OR rule
			//cout << "GOT LOGOP: " << Rule_map[leftHandSide]->get_logop() << endl;
				ofstream fstor;
				//	iterating thru infVector which holds the Rule's right hand predicates, like Father($X,$Z) Parent($Z,$Y)
				for(auto iter = Rule_map[leftHandSide]->infVector.begin(); iter != Rule_map[leftHandSide]->infVector.end(); ++iter){
					//cout << "infVector " << *iter << endl;
					////////////////////////////// The Following block lets us print the same fact multiple times if it
					string resetPrint = "";		// appears multiple times in a rule. 
					stringstream iss (*iter);
					getline(iss,resetPrint,'(');
					if (Fact_map.count(resetPrint) ==1){
						Fact_map[resetPrint]->printed = false;			
					}
					//////////////////////////////
					
					fstor.open("write.txt");
					fstor << *iter;	///cout << *iter << endl;	// put the predicates into a file
					fstor << '\n';
					fstor.close();
					//inference("write.txt");	//call inference recursively on the file
					tvec.push_back(thread(&Manipulator::inference,this,"write.txt"));
					tCount++;
					tvec.back().join();
				}
			}
			
			else if (Rule_map[leftHandSide]->get_logop() == "AND"){ // AND rule
				printImmediately = false; // in this case only we will not print the results Immediately
				// iterate thru RuleVector of this rule, We wanna get $X's and $Y's 
				for(auto iter = Rule_map[leftHandSide]->RuleVector.begin(); iter != Rule_map[leftHandSide]->RuleVector.end(); ++iter){
					for(auto iter2 = iter->begin(); iter2 != iter->end(); ++iter2){ // iterating thru p_vector
						vector<string> dollarVars;
						bool factGroup = false; // if we inference Parent, we dont want the vars of both father and mother, only of parent
						for(auto iter3 = iter2->begin(); iter3 != iter2->end(); ++iter3){ // iterating thru strings
						if (iter3 == iter2->begin()){
							if(Rule_map.count(*iter3) == 1 || Fact_map.count(*iter3) == 1){factGroup = true;}
						}
						//cout << *iter3 << " ";
							//else if(Rule_map.count(*iter3) == 1){factGroup = true;} // if it's a rule
							if(((*iter3)[0] == '$') && factGroup){ // if the string is a '$' variable     && (factGroup==true)
								//cout << *iter3 << " ";
								dollarVars.push_back(*iter3);
							}
							//factGroup = false;
						}
						Rule_map[leftHandSide]->dollarVarsVec.push_back(dollarVars); // put little vectors in big one
						dollarVars.clear();	// clear little vector for next loop
					}
				}
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
					fstor.open("write.txt");
					fstor << *iter; //cout << *iter << endl;
					fstor << '\n';
					fstor.close();
					inference("write.txt");
				}

				
				vector<string> VARVECTOR; // temporary vector to store VARSTRING's, this VARVECTOR will get pushed into a rule object's Results vector of vectors
				string VARSTRING; // temporary string that stores the data from a fact/rule's andVar
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
					Rule_map[infRules[0]]->Results.push_back(VARVECTOR); // VARVECTOR pushed into a rule object's Results vector of vectors
					VARVECTOR.clear();	// MAKE SURE TO CLEAR VARVECTOR FOR EACH NEW LOOOOPER on iter.
				}

				int rVecIndex = 0;
				for (auto it = Rule_map[infRules[0]]->Results.begin(); it != Rule_map[infRules[0]]->Results.end()-1; ++it){
					// starts at second position, stays 1 ahead
					rVecIndex++;
					int rowLoop = 0;
					for(auto it1 = Rule_map[infRules[0]]->Results.begin() + 1; it1 != Rule_map[infRules[0]]->Results.end(); ++it1){
						// moves across 'it' row
						
						for (auto it2 = it->begin(); it2 != it->end(); ++it2){
							// moves across 'it1' row
							//if(){ // if this row has the variable we are looking for
								
							//} else {
								bool MatchVar = false;
								for (auto it3 = it1->begin(); it3 != it1->end(); ++it3){
									if((*it2)[1] == (*it3)[1]){
										MatchVar = true;
									}
									//if(*it2 == *it3) { cout << " ITS A MATCH " << *it2 << " == " << *it3 << endl;}
									
								}
								bool varNeeded = false;
								for (auto it3 = it1->begin(); it3 != it1->end(); ++it3){
									if(MatchVar){ // if this vector has the $ variable we want
										if((*it2)[1] == (*it3)[1]){
											if((*it2) == (*it3)){
												//cout <<  (*it2) << " "<<(*it3)<< "--";
												//cout << (*it2)[1] << " is in vector"<<endl;
												varNeeded = true;
												
											}
										} 
										
									}
									
								}
							
						}
						rowLoop++;
					}
				}
				vector<string> tVar = Rule_map[leftHandSide]->RuleVector[0][0];
				tVar.erase(tVar.begin());
				/*cout<<"Is this the vector? ";
				for(string s : tVar){
					cout<< s << " ";
				}cout<<endl;*/
				/// Searching for matches
				//vector<vector<string>> printResults;
				
				int groupSize = 1;
				char t = Rule_map[infRules[0]]->Results[0][0][1];
				int itt = 1;
				while(t != Rule_map[infRules[0]]->Results[0][itt][1]){
					
					itt++;
					groupSize++;
				}
				//cout << "groupSize before: "<<groupSize<<endl;
				//for each group in row 0:
				cout << "---RESULTS---"<<endl;
				for(int index = 0; index <= Rule_map[infRules[0]]->Results[0].size()-groupSize; index+=groupSize){
					//cout << index << "/" << Rule_map[infRules[0]]->Results[0].size()-groupSize << endl;
					vector<string> group;
					for(int oS = 0; oS < groupSize; oS++){
						group.push_back(Rule_map[infRules[0]]->Results[0][index+oS]);
						//cout<<"pushing back: " << Rule_map[infRules[0]]->Results[0][index+oS] << endl;
					}
					AND(tVar, group, Rule_map[infRules[0]]->Results, 1,0);
					//cout << endl;
				}
				
				
				Rule_map[leftHandSide]->dollarVarsVec.clear(); // clear the vector for next inf of same rule
				
			}
		}
	
	if(threadID > 0) cout << "Thread " << threadID << " completes" << endl;
	
}
void Manipulator::AND(vector<string> tVar, vector<string> group, vector<vector<string>> Results, int row, int ind){
	vector<string> tVarSave = tVar;
	for (int inc = row; inc < Results.size(); inc++){ // going through each main row
	vector<string> output;
		int groupSize = 1;
		char var = Results[inc][0][1];
		int it = 1;
		while(var != Results[inc][it][1]){ // finds the size of the group (X,Y) = a groupsize of 2
			it++;
			groupSize++;
		}
		//going through each smaller row:
		for(int index = ind; index <= Results[inc].size()-groupSize; index+=groupSize){
			
			bool correct = true;
			// look for things from our group (fist one would be Allen,Margret)
			for (int compare = 0; compare < group.size(); compare++){ // for each thing the group,
				correct = true;
				for(int var = 0; var < tVar.size(); var++){
					if (tVar[var][1] == group[compare][1]){ // if its something we want to print (if its X or Y - Allen is an X
						stringstream ss;
						string pushing;
						char c = tVar[var][1];
						ss << c;
						ss >> pushing;
						output.push_back(pushing); 			// add Allen to the output vector
						output.push_back(": ");
						output.push_back(group[compare]);
						tVar.erase(tVar.begin() + var);
						
					}
				}
				for(int offset = 0; offset < groupSize; offset++){
					if(Results[inc][offset+index][1] == group[compare][1]){ // if the variables match
						// AND if the STRINGS match
						if(Results[inc][offset+index] == group[compare]){
							// do nothing i guess
						} else {
							bool ok = true;
							for(string s : tVar){ // checks if all the strings for a particular variable match,
							// so we dont get a group with $ZRalph and $ZSarah
								if(s[1] == Results[inc][offset+index][1]){
									ok = false;
								}
							}
							if(ok){
								correct = false;
							}
						}
					}
				}
				
			}
			if(correct){ // if the variables match and have the same strings,
				if(tVar.size()>0){ // we want to add this stuff to group and run AND again on the next row
					for (int offset = 0; offset < groupSize; offset++){
						bool ok = true;
						for(string s : tVar){ // this checks if the variable is one we have already
							if(s[1] == Results[inc][offset+index][1]){
								ok = false;
							}
						}
						if(ok){ // if we don't have it, add it to the vector
							group.push_back(Results[inc][offset+index]);
						}
						for(int var = 0; var < tVar.size(); var++){ // if its X or Y,
							if (tVar[var][1] == Results[inc][offset+index][1]){
								stringstream ss;
								string pushing;
								char c=tVar[var][1];
								ss << c;
								ss >> pushing;// pretty sure this is where $YBob will get added
								output.push_back(pushing); // push it into the output vector
								output.push_back(": ");	
								output.push_back(Results[inc][offset+index]);
								tVar.erase(tVar.begin() + var);
							}
						}
						if(tVar.size() == 0){ // if the variable vector is empty (both X and Y have been found)
							if(output.size()/3 %groupSize == 0){ //print it out
								int tracker = 0;
								for(int s = 0; s < output.size(); s++){
									if(output[s].size() == 1){
										cout << output[s];
									}else if(output[s] == ": "){
										cout << output[s];
									} else{
										for(int c = 2; c < output[s].size();c++){
											cout << output[s][c];
										}
										cout << " ";
									}
									
									if(tracker%groupSize == 2){
										cout << endl;
									}
									tracker++;
								}
								cout << endl;
							}
							output.clear();
							tVar = tVarSave; // reset the vector for the next group
						}
						int temp = row+1;
						AND(tVar, group, Results, temp,ind); // continue searching, recursively
						
					}
					
				} else {
					// if we are done, newline
					cout <<endl;
				}
				
			}
			
		
		}
	}
	
	/*
	parameters: tVar (X, Y) what we want to print, group to look for, row to begin looking in
	(all things are by value not reference)
	find groupSize
	iterating through the following rows, compare our vector with each group in the row
	if any of the things dont match what we have (a divverent $X) skip this group, continue
	if any of them DO match,  add them to the group to look for
	if they are X or Y, print and remove the printed variable from tvar.
	if tVar isn't empty AND(tvar, new group, next row)
	*/
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
void Manipulator::clean(){
	/// resets variables and containters for the next call to inference
	for (auto it = Fact_map.begin(); it != Fact_map.end();++it){
		it->second->printed = false;
		it->second->readFacts = false;
		it->second->andVars.clear();
	}
	for (auto it = Rule_map.begin(); it != Rule_map.end();++it){
		it->second->firstInference = true;
		it->second->andVars.clear();
		it->second->Results.clear();
	}
	infRules.erase(infRules.begin(), infRules.end());
	infRules.clear();
	printImmediately = true;
	recursions = -1;
}
