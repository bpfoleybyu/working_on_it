#include "Queries.h"



Queries::Queries()
{
}

Queries::Queries(Lexer & myLex)
{
    try{
	myLex.getNextToken("QUERIES");
	myLex.getNextToken("COLON");
	do
	{
		queries.push_back(Query(myLex));

	} while (myLex.getTop()->getTokenType() == "ID");
       }
        catch(const string &actual){
            destructor();
            throw;
        }
}


string Queries::getPrint()
{
	return printStr;
}

string Queries::toString()
{
	string printStr;
	printStr = "Queries(" + to_string(queries.size()) + "):\n";

	for (unsigned int i = 0; i < queries.size(); i++)
	{
		printStr += queries[i].getPrintStr();
		printStr += "\n";
	}

	return printStr;
}

void Queries::destructor()
{
	for (auto p : queries) {
                p.destructor();
	}
}

vector<vector<string>> Queries::getQueries()
{
    vector<vector<string>> temp;
    for(auto p: queries)
    {
        temp.push_back(p.getQueries());
    }
    return temp;
}

Queries::~Queries()
{
}
