#include "Id.h"

Id::Id()
{
}

//constrcutor for the id. creates an expected, assigns it to ID, if the tokens match. if not, the get next throws an exception.
Id::Id(Lexer &myLex)
{
	id = myLex.getNextToken("ID");
}

Token Id::getId()
{
	return id;
}

string Id::toString()
{
	return getMyId();
}

string Id::getMyId() //pulls the id string from the token.
{
	return id.getMyToken();
}

void Id::destructor(){
    //~Id();
    id.~Token();
}

Id::~Id()
{
    //cout <<"called ~ID \n";
}
