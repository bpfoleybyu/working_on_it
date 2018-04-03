/*RULES COLON rulelist
rulelist->rule ruleList | lambda	
rule -> headPredicate COLON_DASH predicate predicateList PERIOD

contains a vector of rule* rules. The constructor calls get next on RULES and COLON to start it off. then pushes back a new rule into rules.
*/

#pragma once

#include "Lexer.h"
#include "rule.h"
#include "HeadPredicate.h"
#include<vector>

class Rules
{
private:
	vector<rule> rules;
public:
	Rules();
	Rules(Lexer &myLex);
	string printString();
    vector<rule> getRules();
	void destructor();
	~Rules();
};

