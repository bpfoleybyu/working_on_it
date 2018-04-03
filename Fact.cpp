#include "Fact.h"

/*Id(string,string...) It can also be empty*/

Fact::Fact()
{
}

Fact::Fact(Lexer &myLex)
{
	//disclaimer here, that if there are no facts, but just a RULES then dont push anything to strings. also forces exit of the while.
	if (myLex.getTop()->getTokenType() != "RULES") {

		factId = Id(myLex);
		myLex.getNextToken("LEFT_PAREN");
                strings.push_back(String(myLex));

		while (myLex.getTop()->getTokenType() == "COMMA")
		{
			myLex.getNextToken("COMMA");
                        strings.push_back(String(myLex));
		}
		myLex.getNextToken("RIGHT_PAREN");
		myLex.getNextToken("PERIOD");
	}
	//fillDomain();
}


string Fact::toString()
{
	string factStr;
	factStr += "  " + factId.getMyId() + "(";
        unsigned int i = 0;
	while (true)
	{
		
                factStr += strings[i].getMyString();
		i++;
		if (i == strings.size()) {
			break;
		}

		factStr += ",";
	}
	factStr += ").";
	return factStr;
}

/*pulls each fact and inserts it into a vector. vector wil be used to populate the set(domains)*/
set<string> Fact::toSet()
{
	set<string> domainId;
        unsigned int i = 0;
	while (true)
	{

                domainId.insert(strings[i].getMyString());
		i++;
		if (i == strings.size()) {
			break;
		}
	}

	return domainId;
}

vector<string> Fact::getRows()
{
    vector<string> row;
    unsigned int i = 0;
    row.push_back(factId.getMyId());
    while(true)
    {
        row.push_back(strings[i].getMyString());
        i++;
        if(i == strings.size()) break;
    }
    return row;
}

void Fact::destructor()
{
        /*for (auto p : strings) {
		delete p;
        }*/
}

Fact::~Fact()
{
}
