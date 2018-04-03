#include "Facts.h"


Facts::Facts(Lexer &myLex)
{
	myLex.getNextToken("FACTS");
	myLex.getNextToken("COLON");

	if (myLex.getTop()->getTokenType() != "RULES") {

		do
		{
                        Fact newFact = Fact(myLex);
			facts.push_back(newFact);
			
		} while (myLex.getTop()->getTokenType() == "ID"); //repeats while there is stuff to repeat

		//cout << printFacts();
	}	
	fillDomain();
}

string Facts::printFacts()
{
		//cout << "print facts" << endl;
		string result;
		result = result + "Facts(" + to_string(facts.size()) + "):\n";
		if (facts.size() != 0)
		{
			for (unsigned int i = 0; i < facts.size(); i++)
			{
                                result += facts[i].toString() + "\n";
			}
		}
		
		return result;
}

void Facts::fillDomain()
{
	//for each fact. pull the set of facts, and append it to the existing domain.
	for (unsigned int i = 0; i < facts.size(); i++)
	{
		set<string> tempSet;
                tempSet=facts[i].toSet();
		domains.insert(tempSet.begin(), tempSet.end());
	}
}


set<vector<string>> Facts::getRows()
{
    set<vector<string>> rows;
    for(auto p: facts){
        rows.insert(p.getRows());
    }
    return rows;
}

set<string> Facts::getDomain()
{
	return domains;
}

void Facts::destructor()
{
        /*for (auto p : facts) {
		p->destructor();
        }*/
}

//pulls from facts, inserts to the set <string>domain



Facts::~Facts()
{
}
