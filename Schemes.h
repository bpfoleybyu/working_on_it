/*calls the passed in lexer obj. creates a vector of schemes, and fills it with schemes if they exist, if not throws an error.*/
//SCHEMES COLON scheme schemelist
#pragma once
#include<vector>
#include"Lexer.h"
#include "scheme.h"
#include"String.h"
#include<set>

using namespace std;

class Schemes
{
String result;

public:
    Schemes(){};
	Schemes(Lexer &myLex);
	vector<scheme> schemes;
	string printSchemes();
	void destructor();
    set<vector<string>> getHeader();
	~Schemes();
};

