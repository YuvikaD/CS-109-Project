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
	string rightHandSide;
	string predName;
	string separator;
	string logop;	
	ifstream readFile("input.txt");	// open the input file
	string name;		// the name of the Fact or Rule, like "Father"
	string data;		// the params of the Fact, like "Allen"
	string preds;
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
			int i=0;
			char a = 'a';
			while(getline(iss,data,' ')){
				stringstream iss4(data);
				i++;
				getline(iss4,predName,'(');
				vector<string> first_vector;
				first_vector.push_back(predName);
				(getline(iss4,rightHandSide,')'));
				stringstream iss5(rightHandSide);
				while(getline(iss5,preds,',')){
					first_vector.push_back(preds);
				}
				r->paramVector.push_back(first_vector);
			}
			cout<< i << endl;
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
