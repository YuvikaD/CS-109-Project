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
	while(getline(readFile,line)){	// read from input file, put contents into 'line' string
		stringstream iss(line);		// put contents of line into a ss object
		getline(iss,leftHandSide,')');	// read from ss object, put contents into 'name' string until '(' char is read
		getline(iss,separator,' ');
		if(separator == ":-"){
			cout  << " rule ";
			stringstream iss2(line);
			getline(iss2,name,'(');	
			getline(iss,logop,' ');	// get logop
			Rule * r = new Rule(logop);
			Rule_map[name] = r;
			while(getline(iss,data,' ')){ // add contents to the Fact object's vector, separated by ','
				Rule_map[name]->vstring.push_back(data);
			}
			cout 
			<< "name: " << name
			<< " operator (0 is OR 1 is AND): " << Rule_map[name]->get_logop() << endl;
		} else {
			cout  << " fact" << endl;
			stringstream iss2(leftHandSide);
			getline(iss2,name,'(');
			Fact * f = new Fact(name);		// make a fact object, it's parameter is what we read in, ex: "Father"
			Fact_map[name] = f;
			while(getline(iss2,data,',')){	// add contents to the Fact object's vector, separated by ','
				f->vstring.push_back(data);
			}
			vFact.push_back(*f);
		}
	}
	for(Fact i: vFact){
		i.printFact();
		}

	for(map <string,Rule*>::iterator it = Rule_map.begin(); it != Rule_map.end(); ++it) {
		cout << " " << it->first << ": " << it->second << endl;
    }	
}
