#include "common.h"
#include "Fact.cpp"
#include "Rule.cpp"
using namespace std;
int main(){
	map<string,Rule*> Rule_map;
	map<string,Fact*> Fact_map;
	vector<Fact> vFact; // use map instead? map of vectors that hold objects, multiple mother objects will go into one vector.
	string line;		// empty string to load the input file's lines into
	string leftHandSide;
	string separator;
	string logop;	
	ifstream readFile("input.txt");	// open the input file
	string name;		// the name of the Fact or Rule, like "Father"
	string data;		// the params of the Fact, like "Allen"
	vector<string> predicates; 
	while(getline(readFile,line)){	// read from input file, put contents into 'line' string
		stringstream iss(line);		// put contents of line into a ss object
		getline(iss,leftHandSide,')');	// read from ss object, put contents into 'name' string until '(' char is read
		getline(iss,separator,' ');
		stringstream iss2(line);
		stringstream iss3(leftHandSide);
		
		if(separator == ":-"){	// Rule
			getline(iss2,name,':');	
			getline(iss,logop,' ');	// get logop
			Rule * r = new Rule(logop);
			Rule_map[name] = r;
			while(getline(iss,data,' ')){ // add contents to the Fact object's vector, separated by ','
				stringstream iss4(data);
				getline(iss4,SOMETHING)
			
				predicates.push_back(data);
				//Rule_map[name]->vstring.push_back(data);
				Rule_map[name]->paramVector.push_back(predicates);
				
			}
		} else {	// Fact
			getline(iss3,name,'(');	
			Fact * f = new Fact(name);		// make a fact object, it's parameter is what we read in, ex: "Father"
			Fact_map[name] = f;
			while(getline(iss3,data,',')){	// add contents to the Fact object's vector, separated by ','
				f->vstring.push_back(data);
			}
			vFact.push_back(*f);
		}
	}
	for(Fact i: vFact){
		i.printFact();
		}

	for(map<string,Rule*>::iterator it = Rule_map.begin(); it != Rule_map.end(); ++it) {
		cout << " " << it->first << ": " << it->second->get_logop() << " " << it->second << endl;
    }	
}