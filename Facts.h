/*
Facts class. 
FACTS COLON factlist
factlist-> fact factlist
fact -> ID ( STRING stringlist ).
*/

#pragma once
#include<vector>
#include"Lexer.h"
#include "Fact.h"
#include<vector>
#include<set>
#include<string>

using namespace std;

class Facts
{
private:
	set<string> domains;

public:
    Facts(){};
	Facts(Lexer &myLex);
	vector<Fact> facts;
	string printFacts();
	void fillDomain();
	set<string> getDomain();
	void destructor();
    
    set<vector<string>> getRows();
	~Facts();
};

