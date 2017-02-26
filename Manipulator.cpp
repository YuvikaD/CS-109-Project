#include "common.h"
#include "Manipulator.h"
using namespace std;
Manipulator:: Manipulator(){}	// constructor
Manipulator::~Manipulator(){}	// destructor 

void Manipulator::drop(string k){
	string name;
	stringstream ss(k);
	getline(ss,name,'(');//get name
	Fact_map.erase(name);
	Rule_map.erase(name);
	cout <<'dropped' << endl;		
}

void Manipulator::dumpFacts(ostream &os){
for(auto it = Fact_map.begin(); it != Fact_map.end(); ++it) {
		os << it->second;
    }
}
void Manipulator::dumpRules(ostream &os){
	bool first = 1;
	auto it = Rule_map.begin();
	os << it->second << endl;
	for(it = Rule_map.begin(); it != Rule_map.end(); ++it) {
		os << it->second->get_logop() << " " << it->second << endl;
    }
}
void Manipulator::dump(){
	ofstream fout;
	fout.open("output.txt");
	dumpFacts(fout);
	dumpRules(fout);
}	
void Manipulator::load(){
ifstream readFile("input.txt");
while(getline(readFile,line)){	// read from input file, put contents into 'line' string
		stringstream iss(line);		// put contents of line into a ss object
		getline(iss,leftHandSide,')');	// read from ss object, put contents into 'leftHandSide' string until ')' char is read
		getline(iss,separator,' ');
		stringstream iss2(line);
		stringstream iss3(leftHandSide);
		
		if(separator == ":-"){	// Rule
			getline(iss2,name,':');	
			getline(iss,logop,' ');	// get logop
			vector<string> first_vector;
			stringstream iss6(name);
			getline(iss6,name3,'(');
			first_vector.push_back(name3);
			//cout << name3 << endl;
			Rule * r = new Rule(logop);	
			Rule_map[name3] = r;
			
			getline(iss6,temp2,')');
			stringstream iss5(temp2);
			while (getline(iss5,temp,',')){
				first_vector.push_back(temp);
			}
			
			
			r->paramVector.push_back(first_vector); // put vector in big vector
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
				r->paramVector.push_back(second_vector);
			}
			
		} else {	// Fact
			getline(iss3,name,'(');	
			if(Fact_map.count(name) != 1){ 	// If this Fact is not in the map
				//cout<< " did not  found " << name << endl;
				Fact * f = new Fact(name);	// make fact object, parameter ex: "Father"
				Fact_map[name] = f;
				while(getline(iss3,data,',')){	// add contents to the Fact object's vector, separated by ','
					f->vstring.push_back(data);
				}
			}
			
			else if(Fact_map.count(name) == 1){ // If this Fact name already exists in the map
				//cout<< " did found " << name << endl;
				Fact_map.find(name)->second->vstring.push_back(space);
				while(getline(iss3,data,',')){	// add contents to the Fact object's vector, separated by ','
					Fact_map.find(name)->second->vstring.push_back(data);
				}
			}
		}
	}
}
