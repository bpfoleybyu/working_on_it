#include "Rules.h"

Rules::Rules(){}

Rules::Rules(Lexer &myLex)
{
    try{
	myLex.getNextToken("RULES");
	myLex.getNextToken("COLON");

	if (myLex.getTop()->getTokenType() != "QUERIES")
	{
		do
		{
			//auto_ptr<rule> newRule(new rule(myLex)); //smart pointer eliminates need for deconstructor
			//rules.push_back(&*newRule);
                        rules.push_back(rule(myLex));

                } while (myLex.getTop()->getTokenType() == "ID");
	}
    }
    catch(const string &actual){
        destructor();
        throw;
    }
	//cout << printString();
}

string Rules::printString()
{
	HeadPredicate head;
	string printStr;
	printStr += "Rules(" + to_string(rules.size()) + "):\n"; 

	for (unsigned int i = 0; i < rules.size(); i++)
	{
                printStr+= rules[i].toString();
			//printStr += head.getPrint();
	}

	return printStr;
}

void Rules::destructor()
{
        for (auto p : rules) {
                p.destructor();
        }
}

vector<rule> Rules::getRules()
{
        return rules;
}


Rules::~Rules()
{
	/*for (auto p : rules) {
		//cout << "deleting rules: " << this << endl;
		delete p;
	}*/
}
