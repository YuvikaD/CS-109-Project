#include "Rule.h"
using namespace std;
Rule::Rule(string log){// constructor 
	logop = log;
	vector<vector<vector<string>>> RuleVector;
	vector<vector<vector<string>>> savedResultsVector;
	
	vector<vector<string>> dollarVarsVec;
	vector<string> userArgs;
	vector<vector<string>> infFactsVector;
}
ostream& operator<< (std::ostream &os, Rule* rule){
	int increment=0;
	bool no = false;
	//bool pipe = false;
	for(int a=0; a<rule->RuleVector.size();++a){
		//pipe =false;
		//if (rule->RuleVector[a][0][0] == "|"){;pipe=true;}
		bool left=true;
		for (auto vvsiter = rule->RuleVector.begin(); vvsiter != rule->RuleVector.end(); ++vvsiter){
			int pos = vvsiter - rule->RuleVector.begin();
			if(pos>=increment){
				increment++;
				bool rool=true;
				for(auto vsiter = vvsiter->begin(); vsiter!=vvsiter->end(); ++vsiter){
					bool first2 = true;
					bool paren = true;
					bool second= true;	
					for (auto jit = vsiter->begin(); jit != vsiter->end(); ++jit){
						int jpos = jit - vsiter->begin();	
						no=false;
						if (*jit=="|"){no=true;os<<endl;break;} // if multiple of same rule, new line, break
							if(no){break;}
							string stt = *jit;
							stt.erase(remove(stt.begin(), stt.end(), ' '), stt.end());
							if(rool){rool=false;if(paren){os << *jit << "("; paren=false;}}
							else if(paren)	{os << " " << stt << "("; 	paren=false;}
							else if(second)	{os << stt; 				second=false;}
							else if(first2)	{os << ',' << stt; }
					}
				if(no){break;} 	// if multiple of same rule, break
				if(left) {os << "):- " << rule->get_logop(); left=false;} // print separator and Logical Operator
				else{os << ')';}
				if(no){break;}		// if multiple of same rule, break
				}
			if(no){break;}			// if multiple of same rule, break
			}
		}
	}
	if(!no){os<<endl;}
    return os;
}



void Rquery(map<string,Rule*> rmap,string s){
	if(rmap.count(s) == 1){
		cout << s << ": " << endl
		<< rmap[s] << endl;
	} else {
		cout << "The rule named " << s << " is not in here" << endl;
	}
}


string Rule::get_logop(){
	return logop;
}

Rule::~Rule(){}	//  destructor
