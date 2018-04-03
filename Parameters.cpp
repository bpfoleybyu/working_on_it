//parameter	->	STRING | ID | expression

#include "Parameters.h"
#include "Expression.h"
#include "Id.h"
#include "String.h"

Parameters::Parameters()
{
}

Parameters* Parameters::createParam(Lexer & myLex)
{

        if (myLex.getTop()->getTokenType() == "ID") {return new Id(myLex);}

	if (myLex.getTop()->getTokenType() == "STRING") { return new String(myLex); }
	
        return new Expression(myLex);
}
