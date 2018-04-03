/*Query class. 

It is a single predicate.

query -> predicate Q_MARK
queryList ->query queryList | lambda

*/

#pragma once
#include"Lexer.h"
#include"Predicate.h"

class Query
{
private:
	string printString;
    vector<string> gotQuery;
    
public:
	Query(Lexer &myLex);
	vector<Predicate>predicates;
	string printQuery();
	string getPrintStr();
	void destructor();
    
    
    vector<string> fillQuery();
    vector<string> getQueries();
    
	~Query();
};

