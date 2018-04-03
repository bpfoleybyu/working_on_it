/*Branden Foley bpfoley2001@yahoo.com CS 236 Datalog Parser, Section 2

The purpose of this project is to create a Database, using the parser and lexer.

search the table by certain params and return if they exist. if not return no.

select, project, rename.
*/

#include "Lexer.h"
#include "Token.h"
#include "datalogProgram.h"
#include "Database.h"
#include <iostream>
#include<string>
#include<fstream>
#include<sstream>

using namespace std;

int main(int argc, char* argv[]) {

                Lexer myLex(argv[1]);
                datalogProgram data_program(myLex);
                Database myDatabase(data_program);

	return 0;
}
