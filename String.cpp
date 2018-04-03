#include "String.h"

String::String()
{
	//cout << "wrong string const \n";
}

String::String(Lexer &myLex)
{
	myString = myLex.getNextToken("STRING");
}

string String::toString()
{
	return getMyString();
}


string String::getMyString()
{
	return myString.getMyToken();
}

void String::destructor(){
    //~String();"
}

String::~String()
{
    //cout << "called ~String() \n";
}
