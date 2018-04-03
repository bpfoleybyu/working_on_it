//creates the actual Id from the lexer.
#pragma once
#include "Lexer.h"
#include "Token.h"
#include "Parameters.h"

class Id:public Parameters
{
public:
	Id();
	Id(Lexer &myLex);
	Token getId();

	string toString();
	string getMyId();
    void destructor();
	~Id();


private:
	Token id;
	
};

