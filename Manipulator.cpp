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
	vector<string> subjectsVec;
	string leftHandSide = "";
	ifstream readFile(filename);
	getline(readFile,line);
	string edited = line;
	edited.erase(std::remove(edited.begin(), edited.end(), ' '), edited.end()); // erases spaces????
	replace( edited.begin(), edited.end(), ')', ',');
	edited.erase(std::remove(edited.begin(), edited.end(), '$'), edited.end());
	stringstream iss(edited);
	getline(iss,leftHandSide,'(');
	///cout << "line " << line << endl;
	///cout << "edited " << edited << endl;
	///cout << "LHS " << leftHandSide << endl;
	
	string it3;
	int count;
	int total;
	bool stopCount;
	bool gotVar;
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
		count=0;
		total=0;
		stopCount = false;
		for(auto iter = Fact_map[leftHandSide]->vstring.begin(); iter != Fact_map[leftHandSide]->vstring.end(); ++iter){
			if(*iter == "|"){
				cout << endl;
				stopCount=true;
			}
			else{
				if(!stopCount){++count;}
				++total;
				if(total%count ==0) {
					///cout << "total: " << total << "count: " << count << endl;
					///cout << "total%count ==0" << endl;
					cout << Fact_map[leftHandSide]->countVec[count -1] 			<< ": " << *iter << '\t' << endl;
					//Fact_map[it3]->subject.push_back(Fact_map[it3]->countVec[count]);
					//Fact_map[it3]->subject.push_back(s);	// this vector will have like $X, Allen, $Z, Marget
					}
				else {
					///cout << "total: " << total << "count: " << count << endl;
					///cout << "total%count !=0" << endl;
					cout << Fact_map[leftHandSide]->countVec[(total%count)-1] 	<< ": " << *iter << '\t' << endl;
					//Fact_map[it3]->subject.push_back(Fact_map[it3]->countVec[(total%count)]);
					//Fact_map[it3]->subject.push_back(s);
				}
			}
		}
		cout << endl;
		Fact_map[leftHandSide]->printed = true;
		}
	}
	
	if(Rule_map.count(leftHandSide) == 1){
		ofstream fstor;
		for(auto iter = Rule_map[leftHandSide]->infVector.begin(); iter != Rule_map[leftHandSide]->infVector.end(); ++iter){
			///cout << *iter << endl;
			fstor.open("write.txt");
			fstor << *iter;
			fstor.close();
			inference("write.txt");
		}
		
		/*
		for (auto iter = Rule_map[leftHandSide]->RuleVector.begin();iter != Rule_map[leftHandSide]->RuleVector.end(); ++iter){
			for(auto iter2 = iter->begin(); iter2 != iter->end();++iter2){
				if(iter2 == iter->begin()){}
				else{
					for(auto iter3 = iter2->begin(); iter3 != iter2->end(); ++iter2){
					 //store Father($X,$Y) in a vector
					}
				}
			}
		}
		*/
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
}
