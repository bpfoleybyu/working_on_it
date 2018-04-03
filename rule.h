/*rule -> headPredicate COLON_DASH predicate predicateList PERIOD
headPredicate calls the same thing as scheme. ID (id list)

contains the HeadPredicate head, and the vector of pred* predicates.
contructor gives head a value, and pushes back predicates into the vector
*/

#pragma once

#include "Lexer.h"
#include "HeadPredicate.h"
#include"Predicate.h"
class rule
{
private:
	HeadPredicate head;
	vector<Predicate> predicates;
    string printStr;

public:
	rule();
	rule(Lexer &myLex);
	string toString();
    string ruleToString();
    vector<Predicate> getPred();
    HeadPredicate getHead();
	void destructor();
	~rule();
};

