/*This is the Lexer class header file. The lexer does the majority of the operations for this program.
This includes all of the Machine functions, a function to read the file, add tokens to the vector, and print out the tokens
*/



#pragma once
#include "Token.h"
#include<vector>
#include<string>
#include<stack>
#include<memory>

using namespace std;

class Lexer
{
private:
	vector<Token*> tokens;
	//stack<Token*> tokenStack;
	int currentLine = 1;
	int next = 0;
    string thrown;
	bool keep = true;
    
public:
	Lexer(char* in_file);
	string getTokenType();
	Token getNextToken(string tokenType);
	Token* getTop();
	Token top();
	void rainDropPopTop(); //this erases the first element in the vector
    
    /*string getThrown();
	bool getKeep(){
		return keep;
	}*/
	void read_file(string fileName);
	void addToken(string tokenType, string mystr);
	void addSpecLineToken(string tokenType, string mystr, int lineNum);
	ifstream ifs;

	//token machines
	void commaMachine();
	void periodMachine();
	void questionMachine();
	void leftParenthMachine();
	void rightParenthMachine();
	void colonMachine();
	void multiplyMachine();
	void addMachine();
	void colonDashMachine();

	//keyword machines
	void idMachine(string &id);
	void schemesMachine(char &mychar);
	void factsMachine(char &mychar);
	void rulesMachine(char &mychar);
	void queriesMachine(char &mychar);
	void stringMachine(char &mychar);
	void commentMachine(char &mychar);
	void multiCommentMachine(char &mychar);

	//default machine and undefined
	void undefinedMachine(string &id, int lineNum);
	void defaultMachine(char mychar);


	//Misc functions
	void incrementLineNum();
	void print(vector<Token*> &tokens);

	//remove comment tokens.


	~Lexer();
};

