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
	ifstream readFile(filename);
	
	getline(readFile,line);
	stringstream iss(line);
	getline(iss,leftHandSide,'(');
	if(Fact_map.count(leftHandSide) == 1){
		cout << Fact_map[leftHandSide];
	}
	if(Rule_map.count(leftHandSide) == 1){
		evaluate(line,Rule_map[leftHandSide],Fact_map,Rule_map);
		
		for (auto it = rawPredicates.begin(); it != rawPredicates.end();++it){
			for (auto it2 = it->begin(); it2 != it->end();++it2){
				//cout << "it2: " << *it2 << " ";
			
			string it3 = *(it->begin());
			
			cout << "SAVEDRESULTSVECTOR: " << endl;
			for(vector<string> i : Fact_map[it3]->savedResultsVector){
				for(string s : i){
					cout << s << " ";
				}
			}
			
			cout << endl;	
			}
		}
		//cout << line;	
		//Yuvi's code 
	}
	
}

void Manipulator::evaluate(string line, Rule * rule, map<string,Fact*> fmap, map<string,Rule*> rmap){
  
	bool filter = false;
	string rulename,nobanana,vars;
	vector<string> varVec;
	stringstream iss(line);
	getline(iss,rulename,'(');
	getline(iss,nobanana,')');
	stringstream iss2(nobanana);
	while(getline(iss2,vars,',')){
		varVec.push_back(vars);
		if(vars[0] != '$'){
		  filter = true;
		}
	}
  
   if(filter){
    rule->check(rmap,fmap, varVec);
  }
  
  if(!filter){
  if(rule->get_logop() == "OR"){
  //vector<string> results;
     for(auto vvsiter = rule->RuleVector.begin(); vvsiter != rule->RuleVector.end(); ++vvsiter){ // go thru RuleVector
       for(auto vsiter = vvsiter->begin(); vsiter!=vvsiter->end(); ++vsiter){ // go thru p_vector
          if(vsiter == vvsiter->begin()){} // do nothing for the rule itself to avoid infinite recursion
         else{
         string fromvector="";
         string name;
         string rules;
		 bool skip=true;
          for(auto jit = vsiter->begin(); jit !=vsiter->end();++jit){
            if(*jit == "|"){}           // If the Rule has multiple of the same name, skip the separator
            else if(jit==vsiter->begin()){name=*jit;fromvector+=(*jit + "(");rules = *jit;}
            else {
				if(skip){fromvector+=*jit;skip=false;}
				else{fromvector+=("," + *jit);}         // get the name of the fact or rule, eg "Father"
            }
          } 
            if(fromvector!=""){
				fromvector+=")"; // dont really need this since I parse it off at begin of evaluate
             if(fmap.count(name) == 1){    // if it's a fact
               cout << "fromvector: " << fromvector << endl;
               cout << "name: " << name << endl;
              cout << fmap[name];  // prints all the factual results
               
             }else if (rmap.count(rules) == 1){ // if it's a rule
				
				cout << "fromvector: " << fromvector << endl;
				cout << "rules: " << rules << endl;
                evaluate(fromvector,rmap[rules],fmap,rmap); // prints all the rule results
             }
            }       
          }
       }   
     }
	}
	if(rule->get_logop() == "AND"){
	//vector<string> results;
		 for(auto vvsiter = rule->RuleVector.begin(); vvsiter != rule->RuleVector.end(); ++vvsiter){ // go thru RuleVector
		   for(auto vsiter = vvsiter->begin(); vsiter!=vvsiter->end(); ++vsiter){ // go thru p_vector
			  if(vsiter == vvsiter->begin()){} // do nothing for the rule itself to avoid infinite recursion
			 else{
			 string fromvector="";
			 vector<string> fromvectorRaw;
			 string name;
			 string rules;
			 bool skip=true;
			  for(auto jit = vsiter->begin(); jit !=vsiter->end();++jit){
				if(*jit == "|"){}           // If the Rule has multiple of the same name, skip the separator
				else if(jit==vsiter->begin()){
					name=*jit;fromvector+=(*jit + "(");
					fromvectorRaw.push_back(*jit);
					rules = *jit;
					}
				else {
					if(skip){
						fromvector+=*jit;
						fromvectorRaw.push_back(*jit);
						skip=false;
					}
					else{
						fromvector+=("," + *jit);
						fromvectorRaw.push_back(*jit);
						}         // get the name of the fact or rule, eg "Father"
				}
			  }
				rawPredicates.push_back(fromvectorRaw);
				if(fromvector!=""){
					fromvector+=")"; // dont really need this since I parse it off at begin of evaluate
				if(fmap.count(name) == 1){    // if it's a fact
					cout << "fromvector: " << fromvector << endl;
					cout << "name: " << name << endl;
					cout << fmap[name];  // prints all the factual results
				    fmap[name]->savedResultsVector.push_back(fmap[name]->vstring);
					
				 }else if (rmap.count(rules) == 1){ // if it's a rule
					cout << "fromvector: " << fromvector << endl;
					cout << "rules: " << rules << endl;
					evaluate(fromvector,rmap[rules],fmap,rmap); // prints all the rule results
				 }
				}   
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
				while(getline(iss,data,' ')){
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
}
