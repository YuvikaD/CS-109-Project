#include "Manipulator.h"
using namespace std;
Manipulator:: Manipulator(){}	// constructor
Manipulator::~Manipulator(){}	// destructor 

void Manipulator::drop(string name){
	if(Fact_map.find(name)!=Fact_map.end()) Fact_map.erase(name);
	if(Rule_map.find(name)!=Rule_map.end()) Rule_map.erase(name);
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
		//cout << Rule_map[leftHandSide];
		//Yuvi's code 
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
				(getline(iss4,rightHandSide,')'));
				stringstream iss5(rightHandSide);
				while(getline(iss5,preds,',')){
					second_vector.push_back(preds);
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
				while (getline(iss5,temp,',')){				
					first_vector.push_back(temp);			
				}
				p_vector.push_back(first_vector);
				while(getline(iss,data,' ')){
				stringstream iss4(data);
				getline(iss4,predName,'(');
				vector<string> second_vector;
				second_vector.push_back(predName);
				(getline(iss4,rightHandSide,')'));
				stringstream iss5(rightHandSide);
				while(getline(iss5,preds,',')){
					second_vector.push_back(preds);
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
