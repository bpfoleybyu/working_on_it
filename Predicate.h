/*pulls the id with the predId.
pushes back parameters into the vector 

predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
*/

#pragma once

#include "Lexer.h"
#include"Predicate.h"
#include "Id.h"
#include "Parameters.h"
#include "Expression.h"
#include<string>

class Predicate
{
private:
	Id predicateId;
	vector<Parameters*> parameters;

public:
	Predicate();
	Predicate(Lexer &myLex);
    
    vector<Parameters*> getParams();
    vector<string> getQuery(); //pulls the Query for the ruleEval part.
    string getPredId(){return predicateId.getMyId();}
	string printParams();
	int getParamSize();
	void destructor();
	~Predicate();
};

