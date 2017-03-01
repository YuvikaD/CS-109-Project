#include "common.h"
#include "Fact.cpp"
#include "Rule.cpp"
using namespace std;
int main(){
	map<string,Rule*> Rule_map;
	map<string,Fact*> Fact_map;
	//vector<Fact> vFact; // map of vectors that hold objects, multiple mother objects will go into one vector.
	string line;		// empty string to load the input file's lines into
	string leftHandSide;
	string rightHandSide;
	string predName;
	string leftPredName;
	string leftPreds;
	string separator;
	string logop;	
	ifstream readFile("input.txt");	// open the input file
	string name;		// the name of the Fact or Rule, like "Father"
	string name2;
	string name3;
	string temp;
	string temp2;
	string temp3;
	const string space="|";
	string data;		// the params of the Fact, like "Allen"
	string preds;
	vector<string> predicates; 
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
	
	// check if a key exists
	//if(Rule_map.count("Parent") == 1){cout<< " did found parent" << endl;}
	//if(Rule_map.count("Parent") == 0){cout<< " did not  found parent" << endl;}
	
	// print out facts and rules for debugging
	
	/*for(map<string,Fact*>::iterator it = Fact_map.begin(); it != Fact_map.end(); ++it) {
		cout << " " 
		<< it->first << ": " 
		<< it->second << endl;
    }*/
	
	/*for(map<string,Rule*>::iterator it = Rule_map.begin(); it != Rule_map.end(); ++it) {
		cout //<< " " 
		<< it->first << ": " 
		<< it->second->get_logop() << " " << it->second << endl;
    }*/

	//query(Fact_map,"Father");
	//query(Fact_map,"Mother");
	
	cout << "-----------------------------------" << endl;
	// Rquery(Rule_map,"GrandFather");
	vector<string> argVec={"Carl","Ryan"};
	Rule_map["GrandFather"]->check(Rule_map,Fact_map,argVec);
	
}
