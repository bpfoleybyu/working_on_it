/*expression class.

exp-> LEFT_P param op param RIGHT_P
op -> ADD | MULT
*/

#pragma once

#include"Lexer.h"
#include "Parameters.h"
#include<string>

class Expression: public Parameters
{

private:
	Parameters* paramL = NULL;
	Parameters* paramR = NULL;
	Token myOperator;

public:
	Expression();
	Expression(Lexer &myLex);
	string toString();
    void destructor();
	~Expression();
};

