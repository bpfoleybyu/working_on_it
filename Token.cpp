/*Token cpp class. very simple. constructor and to string.*/

#include "Token.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include<istream>
#include<string>


using namespace std;

Token::Token() {
}

Token::Token(string tokenType, string myToken, int line_num)
{
	this->tokenType = tokenType;
	this->myToken = myToken;
	this->line_num = line_num;
}

string Token::getTokenType()
{
	return tokenType;
}

string Token::getMyToken()
{
	return myToken;
}

int Token::getLineNum()
{
	return line_num;
}


string Token::toString()
{
	ostringstream ss;
	ss << "(" << tokenType << "," << '"' << myToken << '"' << "," << line_num << ")";

	return ss.str();
}

Token::~Token()
{
    for(auto p: tokens){
        //delete p;
    }
}
