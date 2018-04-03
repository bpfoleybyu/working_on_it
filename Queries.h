#pragma once
#include"Lexer.h"
#include"Query.h"
#include <vector>
#include<string>
class Queries
{
private:
	string printStr;
public:
	Queries();
	Queries(Lexer &myLex);
	vector<Query> queries;
	string getPrint();
	string toString();
	void destructor();
    
    vector<vector<string>> getQueries();
	~Queries();
};

