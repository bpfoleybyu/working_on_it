/*Domain class, contains a set that stores indiviual facts. and outputs them as called. this makes sure there are no repeats.

CALLED IT DOMAINS BC DOMAIN IS IN THE STD LIB ALREADY

pulls each String from facts.
2 options. each time you add a string to fact, also add it to domain... OR go through after and grab it.
*/

#pragma once
#include "Schemes.h"
#include"Lexer.h"
#include"Facts.h"
#include<set>

class Domain
{	
private:
	set<string> domain;

public:
	Domain(Facts &myFact);
	void fillDomain(Facts &myFact);
	string printDomain();

	~Domain();
};

