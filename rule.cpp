/*first calls the headPred. then checks for COLON_DASH

HeadPred COLON_DASH predicate predList PERIOD

HeadPred :- predicate,predicate.*/

#include "rule.h"

rule::rule()
{
}

rule::rule(Lexer &myLex)
{
    try{
	head = HeadPredicate(myLex);
	myLex.getNextToken("COLON_DASH");
        predicates.push_back(Predicate(myLex));

	//if there is a comma, there is more than one, so you repeat. 
	while (myLex.getTop()->getTokenType() == "COMMA")
	{
		myLex.getNextToken("COMMA");

                predicates.push_back(Predicate(myLex));
	}
	myLex.getNextToken("PERIOD");
    }
    catch(const string &actual){
     destructor();
     throw;
    }
        //printStr = toString();
}

string rule::toString()
{
	string ruleStr;

	ruleStr = ruleStr + "  " + head.getPrint() +" :- ";
        unsigned int i = 0;

	while (true)
	{
                ruleStr += predicates[i].printParams();
		i++;

		if (i == predicates.size()) {
			break;
		}
		ruleStr += ",";

	}
	ruleStr = ruleStr + "." + "\n";
	return ruleStr;
}

string rule::ruleToString()
{
        string ruleStr;

        ruleStr = ruleStr + head.getPrint() +" :- ";
        unsigned int i = 0;

        while (true)
        {
                ruleStr += predicates[i].printParams();
                i++;

                if (i == predicates.size()) {
                        break;
                }
                ruleStr += ",";

        }
        ruleStr = ruleStr + "." + "\n";
        return ruleStr;
}


vector<Predicate> rule::getPred()
{
    return predicates;
}

HeadPredicate rule::getHead()
{
    return head;
}

void rule::destructor()
{
       /* for (auto p : predicates) {
            //cout <<"rule desrrtuct\n";
                p.destructor();
        }*/
}

rule::~rule()
{
}

