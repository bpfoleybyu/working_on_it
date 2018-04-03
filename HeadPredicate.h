/*This is the same as the scheme class. it just calls the schemeList*/

#pragma once

#include "Lexer.h"
#include "Id.h"
#include<vector>
#include "Header.h"

class HeadPredicate
{
private:
	vector<Id> columnNames;
	Id headId;
	string printHeadStr;

public:
	HeadPredicate();
	HeadPredicate(Lexer &myLex);
	string printHead();
	string getPrint();
    string getHeadId();
    vector<string> getColNames();
    Header toHeader();
	
	~HeadPredicate();
};

