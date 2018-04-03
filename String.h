#pragma once
#include "Lexer.h"
#include "Parameters.h"
#include<string>

class String :public Parameters
{
public:
	String();
	String(Lexer &myLex);
	string toString();
	string getMyString();
    void destructor();
	~String();

private:
	Token myString;
};
