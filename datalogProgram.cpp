//source code for the files.
#include "datalogProgram.h"

datalogProgram::datalogProgram()
{
}

// calls all of schemes, rules, facts, queries, and the call the print functions for each of them.
datalogProgram::datalogProgram(Lexer &myLex)
{

    try{
        schemes_ = Schemes(myLex);
        facts_ = Facts(myLex);
        rules_ = Rules(myLex);
        queries_ = Queries(myLex);

        //Domain myDomain(myFacts);

        printString = schemes_.printSchemes() + facts_.printFacts() + rules_.printString()
                + queries_.toString(); //+ myDomain.printDomain();

        header = schemes_.getHeader();
        rows = facts_.getRows();
        queries = queries_.getQueries();
        rules = rules_.getRules();

        destructor(rules_, queries_);

        //cout << "Success!\n";
        //cout << getPrint();
    }

    catch (const string &actual){
        cout << "Failure!\n";
        cout << actual;
        destructor(rules_, queries_);
        }


}

/*string datalogProgram::getToThrow(Lexer &myLex){
        return myLex.getThrown();
}*/

string datalogProgram::getPrint()
{
	return printString;
}

set<vector<string>> datalogProgram::getHeader()
{
        return header;
}

set<vector<string>> datalogProgram::getRows()
{
        return rows;
}

vector<vector<string>> datalogProgram::getQueries()
{
        return queries;
}

vector<rule> datalogProgram::getRules()
{
        return rules;
}

void datalogProgram::outsideDest(){
    //cout <<"called Dest\n";
        destructor(rules_, queries_);
}

void datalogProgram::destructor(Rules &r, Queries &q)
{
    //cout << "----\n";
        r.destructor();
        q.destructor();
}

datalogProgram::~datalogProgram()
{
    //destructor(mySchemes, myFacts, myRules, myQueries);
}
