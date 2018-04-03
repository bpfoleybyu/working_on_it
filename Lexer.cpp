/*This is the cpp file for the Lexer class. it calls the read file function, which then passes the char through
  the switch statements until it reaches the proper machine. most machines are self explanatory,
  but there are some comments for the more confusing ones.*/


#include "Lexer.h"

Lexer::Lexer(char* in_file)
{
	read_file(in_file);
}

string Lexer::getTokenType()
{
	Token mytoken;
	return mytoken.getTokenType();
}


Token Lexer::getNextToken(string tokenType)
{
	Token actual = top();
	string actualStr = tokens[0]->getTokenType();

	if (actualStr != tokenType || actualStr == "EOF")
	{
		string actual;
		string lineNum = to_string(currentLine);
		actual = "(" + tokens[0]->getTokenType() + ",\"" + tokens[0]->getMyToken() + "\"," + to_string(tokens[0]->getLineNum()) + ")";
                //then delete
                throw actual;
	}

	rainDropPopTop();
	return actual;
}

/*string Lexer::getThrown(){
    return thrown;
}*/

Token* Lexer::getTop()
{
	return tokens[0];
}

Token Lexer::top()
{
	return *tokens[0]; //calls what tokens[0] is pointing at.
}

/*deletes the first element of the vector*/
void Lexer::rainDropPopTop()
{
	//make sure it is not trying to delete and/or erase nothing.
	delete tokens[0];
	tokens.erase(tokens.begin());
}

/*reads from the file.includes the int line_num, which increments each time you hit the "\n" symbol
  Use a while stream and a switch statement to go through each character and run it through each machine as necessary*/
void Lexer::read_file(string fileName)
{
	ifs.open(fileName);

	if (ifs.is_open()) {

		//use this to look at the next char and make sure it is not EOF
		//Iterate through pulling one char at a time.
		while (ifs.peek() != EOF)
		{
			char mychar;
			//this pulls the first char and sets it to mychar.
			ifs.get(mychar);

			//this is for the keywords
			if (isalnum(mychar))
			{
				if (isdigit(mychar)) {
					defaultMachine(mychar);
				}

				else {
					switch (mychar)
					{
						case 'S': schemesMachine(mychar); break;
						case 'F': factsMachine(mychar); break;
						case 'R': rulesMachine(mychar); break;
						case 'Q': queriesMachine(mychar); break;

						default: string id; id += mychar; idMachine(id); break;
					}
				}
			}

			//this is the switch for all of the tokens/ nonalphas
			if (!isalnum(mychar))
				switch (mychar)
				{
					case ',': commaMachine(); break;
					case '.': periodMachine(); break;
					case '?': questionMachine(); break;
					case '(': leftParenthMachine(); break;
					case ')': rightParenthMachine();  break;
					case ':': colonMachine();  break;
					case '*': multiplyMachine();  break;
					case '+': addMachine(); break;
					case '\n': incrementLineNum(); break;
					case '\'': stringMachine(mychar); break;
					case '#': commentMachine(mychar); break;
					default: defaultMachine(mychar); break;
				}

		}

		//this adds the default EOF file
		string myEOF = "EOF"; string myToken = "";
		addToken(myEOF, myToken);
		//print(tokens);
	}

	else cout << "file didn't open \n";
}

//default, checks if the char is whitespace or undefined. if it is not whitespace it is turned into an undefined token
void Lexer::defaultMachine(char mychar)
{
	if (isspace(mychar)) {
		return;
	}
	else {
		int lineNum = currentLine;
		string id;
		id += mychar;
		undefinedMachine(id, lineNum);
	}
}

//machines for all of the tokens
void Lexer::commaMachine()
{
	string tokenType = "COMMA";
	string token = ",";
	addToken(tokenType, token);
}
void Lexer::periodMachine()
{
	string tokenType = "PERIOD";
	string token = ".";
	addToken(tokenType, token);
}
void Lexer::questionMachine()
{
	string tokenType = "Q_MARK";
	string token = "?";
	addToken(tokenType, token);
}
void Lexer::leftParenthMachine()
{
	string tokenType = "LEFT_PAREN";
	string token = "(";
	addToken(tokenType, token);
}
void Lexer::rightParenthMachine()
{
	string tokenType = "RIGHT_PAREN";
	string token = ")";
	addToken(tokenType, token);
}

//pulls the next char from the ifstream, if there is a colon after the dash, send to the colonDashMachine.
void Lexer::colonMachine()
{

	if (ifs.peek() == '-') {
		char next;
		ifs.get(next);
		colonDashMachine(); return;
	}

	string tokenType = "COLON";
	string token = ":";
	addToken(tokenType, token);
}
void Lexer::colonDashMachine()
{
	string tokenType = "COLON_DASH";
	string token = ":-";
	addToken(tokenType, token);
}
void Lexer::multiplyMachine()
{
	string tokenType = "MULTIPLY";
	string token = "*";
	addToken(tokenType, token);
}
void Lexer::addMachine()
{
	string tokenType = "ADD";
	string token = "+";
	addToken(tokenType, token);
}

void Lexer::idMachine(string &id)
{
	string myToken = id;

	while (isalnum(ifs.peek()))
	{
		char next;
		ifs.get(next);
		myToken += next;
	}

	string tokenType = "ID";
	addToken(tokenType, myToken);
}

//keyword machines, peek and check, if not send to IdMachine
void Lexer::schemesMachine(char &mychar)
{
	string id;
	id += mychar;
	if (ifs.peek() == 'c') {
		char next;
		ifs.get(next);
		id += next;
		if (ifs.peek() == 'h') {
			char next;
			ifs.get(next);
			id += next;
			if (ifs.peek() == 'e') {
				char next;
				ifs.get(next);
				id += next;
				if (ifs.peek() == 'm') {
					char next;
					ifs.get(next);
					id += next;
					if (ifs.peek() == 'e') {
						char next;
						ifs.get(next);
						id += next;
						if (ifs.peek() == 's') {
							char next;
							ifs.get(next);
							id += next;

							if (!isalpha(ifs.peek())) {
								string tokenType = "SCHEMES";
								string myToken = "Schemes";
								addToken(tokenType, myToken);
								return;
							}

						}
					}
				}
			}
		}
	}
	idMachine(id);
}

void Lexer::factsMachine(char &mychar)
{
	string id;
	id += mychar;
	if (ifs.peek() == 'a') {
		char next;
		ifs.get(next);
		id += next;
		if (ifs.peek() == 'c') {
			char next;
			ifs.get(next);
			id += next;
			if (ifs.peek() == 't') {
				char next;
				ifs.get(next);
				id += next;
				if (ifs.peek() == 's') {
					char next;
					ifs.get(next);
					id += next;

					if (!isalpha(ifs.peek())) {
						string tokenType = "FACTS";
						string myToken = "Facts";
						addToken(tokenType, myToken);
						return;
					}
				}
			}
		}
	}
	idMachine(id);
}

void Lexer::rulesMachine(char &mychar)
{
	string id;
	id += mychar;
	if (ifs.peek() == 'u') {
		char next;
		ifs.get(next);
		id += next;
		if (ifs.peek() == 'l') {
			char next;
			ifs.get(next);
			id += next;
			if (ifs.peek() == 'e') {
				char next;
				ifs.get(next);
				id += next;
				if (ifs.peek() == 's') {
					char next;
					ifs.get(next);
					id += next;

					if (!isalpha(ifs.peek())) {
						string tokenType = "RULES";
						string myToken = "Rules";
						addToken(tokenType, myToken);
						return;
					}

				}
			}
		}
	}
	idMachine(id);
}

void Lexer::queriesMachine(char &mychar)
{
	string id;
	id += mychar;
	if (ifs.peek() == 'u') {
		char next;
		ifs.get(next);
		id += next;
		if (ifs.peek() == 'e') {
			char next;
			ifs.get(next);
			id += next;
			if (ifs.peek() == 'r') {
				char next;
				ifs.get(next);
				id += next;
				if (ifs.peek() == 'i') {
					char next;
					ifs.get(next);
					id += next;
					if (ifs.peek() == 'e') {
						char next;
						ifs.get(next);
						id += next;
						if (ifs.peek() == 's') {
							char next;
							ifs.get(next);
							id += next;

							if (!isalpha(ifs.peek())) {
								string tokenType = "QUERIES";
								string myToken = "Queries";
								addToken(tokenType, myToken);
								return;
							}
						}
					}
				}
			}
		}
	}
	idMachine(id);
}

//if there is a ' then read until the next dash. if that dash has a dash after it, then keep reading as normal.
//calls the bool, is followed by string func.
void Lexer::stringMachine(char &mychar)
{
	string myString;
	myString += mychar;
	int myLine = currentLine;

	while (ifs.peek()) {

		if (ifs.peek() == '\n') {
			incrementLineNum();
		}
		if (ifs.peek() == '\'') {
			char next;
			ifs.get(next);
			myString += next;
			if (ifs.peek() != '\'') {
				string tokenType = "STRING";
				addSpecLineToken(tokenType, myString, myLine);
				break;
			}
		}

		char next;
		ifs.get(next);
		myString += next;

		if (ifs.peek() == EOF) {
			undefinedMachine(myString, myLine);
			break;
		}

	}
}

void Lexer::commentMachine(char &mychar)
{
	string myToken;
	myToken += mychar;

	if (ifs.peek() == '|') {
		//cout << "to the multi- M\n";
		multiCommentMachine(mychar);
		return;
	}

	while (ifs.peek() != '\n' && ifs.peek() != EOF)
	{
		char next;
		ifs.get(next);
		myToken += next;
	}

	if (ifs.peek() == '#')
	{
		char next;
		ifs.get(next);
		myToken += next;
	}

	string tokenType = "COMMENT";
	//addToken(tokenType, myToken);
}

void Lexer::multiCommentMachine(char &mychar)
{
	string myToken;
	myToken += mychar;
	int myLine = currentLine;


	while (ifs.peek() != EOF)
	{
		if (ifs.peek() == '|') {
			char next;
			ifs.get(next);
			myToken += next;
			if (ifs.peek() == '#')
			{
				char next;
				ifs.get(next);
				myToken += next;
				break;
			}
		}
		if (ifs.peek() == '\n') {
			incrementLineNum();
		}

		char next;
		ifs.get(next);
		myToken += next;
	}

	if (ifs.peek() == EOF) {
		undefinedMachine(myToken, myLine);
	}
	else {
		string tokenType = "Comment";
		//addToken(tokenType, myToken, myLine);
	}
}

void Lexer::undefinedMachine(string & id, int lineNum)
{
	string tokenType = "UNDEFINED";
	string myToken = id;
	addSpecLineToken(tokenType, myToken, lineNum);
}

void Lexer::incrementLineNum() {
	currentLine++;
}
void Lexer::print(vector<Token*>& tokens)
{
	//ofstream out(out_file);
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		cout << tokens[i]->toString() << endl;
	}
	cout << "Total Tokens = " << tokens.size() << endl;
}

//creates a new token object and adds it to the vector of tokens.
void Lexer::addToken(string tokenType, string myToken)
{
	//Token* mytoken = new Token;
	tokens.push_back(new Token(tokenType, myToken, currentLine));
}

void Lexer::addSpecLineToken(string tokenType, string mystr, int lineNum)
{
	tokens.push_back(new Token(tokenType, mystr, lineNum));
}

Lexer::~Lexer()
{
        for (unsigned int i = 0; i < tokens.size(); i++)
	{
		delete tokens[i];
        }
}
