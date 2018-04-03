/*this class is to define a string, id or expression

parameter	->	STRING | ID | expression
if string assign it.
if id assign it.
if expression call expression(myLex).
*/

#pragma once
#include "Lexer.h"


class Parameters
{
public:
	Parameters();
	static Parameters* createParam(Lexer &myLex);
    virtual string toString()=0;
	virtual void destructor()=0;
	//virtual ~Parameters()=0;
   virtual ~Parameters() {};
};

