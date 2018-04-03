#include "scheme.h"

//pulls the ( then puts in the ID. then a comma, followed by each additional ID.
scheme::scheme(Lexer &myLex)
{
	//cout << "called scheme \n";
	schemeId = Id(myLex);
	myLex.getNextToken("LEFT_PAREN");
	columnNames.push_back(Id(myLex));

	//if there is a comma(meaning there is more then one id), erase the comma, pull the id, add it to the vector, keep going until no more comma.
	while (myLex.getTop()->getTokenType() == "COMMA")
	{
		myLex.getNextToken("COMMA");
		columnNames.push_back(Id(myLex));
	}
	myLex.getNextToken("RIGHT_PAREN");
}


string scheme::toString()
{
	string schemeStr;

	schemeStr += "  " + schemeId.getMyId() + "(";
        unsigned int i = 0;

	while (true)
	{
		schemeStr += columnNames[i].getMyId();
		i++;
		if (i == columnNames.size()) {
			break;
		}
		schemeStr += ",";
	}
	schemeStr += ")";

	return schemeStr;
}

vector<string> scheme::getHeader()
{
    vector<string> temp;

    unsigned int i = 0;
    temp.push_back(schemeId.getMyId()); //pulls the id and puts it in the vector too!
    while (true)
    {
            temp.push_back(columnNames[i].getMyId());
            i++;
            if (i == columnNames.size()) {
                    break;
            }
    }

        return temp;
}

void scheme::destructor()
{
	for (auto p : columnNames) {
		//delete p;
	}
}

scheme::~scheme()
{
	/*for (auto p : columnNames) {
		delete p;
	}*/
}
