/*Purpose of the Token class is to hold the constructor for the tokens, and have a tostring function to use while printing.*/

#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

#pragma once

using namespace std;
//sets the TokeType for all possible TokenTypes

class Token
{
private:
	string tokenType;
	string myToken;
	int line_num = 1;
	

public:
	Token();
	Token(string tokenType, string myToken, int line_num);

	vector<Token> tokens;

	string toString();
	
	string getTokenType();
	string getMyToken();
	int getLineNum();
	//TokenType pullTokenType();

	~Token();
};

