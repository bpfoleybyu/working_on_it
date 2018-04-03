#pragma once
#include "Lexer.h"
#include "String.h"
#include "Id.h"
#include<string>
#include<set>

class Fact
{
private:
	set<string> domain;

public:
	Fact();
	Fact(Lexer &myLex);
	vector<String> strings;
	Id factId;

    
    vector<string> getRows();
	string toString();
	set<string> toSet(); //this pulls the fact ids into a vector, so they can be stored in domain.
	void destructor();

	~Fact();
};

