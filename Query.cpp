#include "Query.h"


Query::Query(Lexer &myLex)
{
    try{
        predicates.push_back(Predicate(myLex));
        //Predicate myPred(myLex);

	myLex.getNextToken("Q_MARK");

        printString = printQuery();
        gotQuery = fillQuery();
        //myPred.destructor();
    }
    catch(const string &actual){
           destructor();
           throw;
    }
    }


string Query::printQuery()
{
    string printStr = "  ";
    unsigned int i= 0;

    while(true){

    printStr += predicates[i].printParams();
    i++;

    if (i == predicates.size()) {
            break;
    }
    printStr += ",";
}
    printStr += "?";
    return printStr;
}

string Query::getPrintStr()
{
	return printString;
}

void Query::destructor()
{
	for (auto p : predicates) {
                p.destructor();
		//delete p;
	}
}

vector<string> Query::fillQuery()
{
    vector<string> temp;
    vector<Parameters*> params = predicates[0].getParams();
    temp.push_back(predicates[0].getPredId());

    for(int i = 0; i < predicates[0].getParamSize(); i++)
    {
        temp.push_back(params[i]->toString());
    }
    return temp;
}

vector<string> Query::getQueries()
{
    return gotQuery;
}

Query::~Query()
{
}
