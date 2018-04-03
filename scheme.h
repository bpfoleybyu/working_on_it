//creates the Scheme from the id, and stores it in the scheme vector
#pragma once
#include <vector>
#include <string>
#include "Id.h"

class scheme
{
public:
	scheme(Lexer &myLex);
	Id schemeId;
	vector<Id> columnNames;
	string toString();
	void destructor();
    vector<string> getHeader();
    
	~scheme();
};

