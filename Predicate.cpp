//predicate->ID LEFT_PAREN parameter parameterList RIGHT_PAREN

#include "Predicate.h"

Predicate::Predicate()
{
}

Predicate::Predicate(Lexer &myLex)
{
    try{
	predicateId = Id(myLex);

	myLex.getNextToken("LEFT_PAREN");
	
	parameters.push_back(Parameters::createParam(myLex));

	while (myLex.getTop()->getTokenType()== "COMMA") //while its a list.
	{
		myLex.getNextToken("COMMA");
		parameters.push_back(Parameters::createParam(myLex));
	}

	myLex.getNextToken("RIGHT_PAREN");
	printParams();
    }

    catch(const string &thrown){
        //cout << "thrown";
        destructor();
        throw;
    }
}

string Predicate::printParams()
{
	string paramStr;
	paramStr = paramStr + predicateId.getMyId() + "(";
	//paramStr = "stub";
	for (unsigned int i = 0; i < parameters.size(); i++)
	{
		if (i > 0) {
			paramStr += ",";
		}
                paramStr = paramStr + parameters[i]->toString();
	}
	paramStr += ")";
	return paramStr;
}

int Predicate::getParamSize()
{
	return parameters.size();
}

void Predicate::destructor()
{
    //cout << "called *****DEST\n";
       for(auto p: parameters){
		predicateId.~Id();
                delete p;
        }

        parameters.clear();
}

vector<Parameters*> Predicate::getParams()
{
    return parameters;
}

vector<string> Predicate::getQuery()
{
    vector<string> gotQ;
    for(unsigned int i = 0; i< parameters.size(); i++)
    {
        gotQ.push_back(parameters[i]->toString());
    }
    return gotQ;
}

Predicate::~Predicate()
{

}
