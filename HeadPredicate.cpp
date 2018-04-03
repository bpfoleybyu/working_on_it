#include "HeadPredicate.h"



HeadPredicate::HeadPredicate()
{
}

HeadPredicate::HeadPredicate(Lexer &myLex)
{
	headId = Id(myLex);
	myLex.getNextToken("LEFT_PAREN");
	columnNames.push_back(Id(myLex));

	while (myLex.getTop()->getTokenType() == "COMMA")
	{
		myLex.getNextToken("COMMA");
		columnNames.push_back(Id(myLex));
	}

	myLex.getNextToken("RIGHT_PAREN");
	printHeadStr = printHead();
	//cout << "printHeadStr: " << printHeadStr << endl;
}

string HeadPredicate::printHead()
{
	string headStr;
	headStr += headId.getMyId() + "(";

	//cout << "headId: " << headId.getMyId() << endl << "vs headStr: " << headStr << endl;
        unsigned int i = 0;

	while (true)
	{
		headStr += columnNames[i].getMyId();
		//cout << "headStr: " << columnNames[i]->getMyId() << endl;
		i++;
		if (i == columnNames.size()) {
			break;
		}
		headStr += ",";
	}
	headStr += ")";
	//cout << "what about this headStr: " << headStr << endl;
	return headStr;
}

string HeadPredicate::getHeadId()
{
    return headId.getMyId();
}

string HeadPredicate::getPrint()
{
	return printHeadStr;
}

vector<string> HeadPredicate::getColNames()
{
    vector<string> predicates;
    for(auto p: columnNames){
            predicates.push_back(p.getMyId()); //converts the Id to string
        }
    return predicates;
}

Header HeadPredicate::toHeader()
{
    Header t;
    for(auto p: columnNames){
            t.push_back(p.getMyId()); //converts the Id to string for the Header
        }
    return t;

}

HeadPredicate::~HeadPredicate()
{
	/*for (auto p : columnNames)
	{
		cout << "delete colName: " << this << endl;
		delete p;
	}*/
}
