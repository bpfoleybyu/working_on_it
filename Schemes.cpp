#include "Schemes.h"


//checks to see if the keywords are right. SCHEMES COLON scheme schemelist
Schemes::Schemes(Lexer &myLex)
{
	myLex.getNextToken("SCHEMES");
	myLex.getNextToken("COLON");

	do
	{
		schemes.push_back(scheme(myLex));

	} while (myLex.getTop()->getTokenType() == "ID");

	//cout << printSchemes();
}

/*this prints out all of the Schemes in the proper format.*/
string Schemes::printSchemes()
{
	string result;
	result = result + "Schemes(" + to_string(schemes.size()) + "):\n";
	for (unsigned int i = 0; i < schemes.size(); i++)
	{
		result += schemes[i].toString() + "\n";
	}
	return result;
}

void Schemes::destructor()
{
	for (auto p : schemes) {
		//p-destructor();
	}
}

set<vector<string>> Schemes::getHeader()
{
    set<vector<string>> temp;
        for (auto p : schemes) {
                temp.insert(p.getHeader());
                //p-destructor();
        }
        return temp;
}

Schemes::~Schemes()
{
	/*for (auto p : schemes) {
		delete p;
	}*/
}
