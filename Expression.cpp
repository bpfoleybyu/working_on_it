#include "Expression.h"

Expression::Expression()
{
}

Expression::Expression(Lexer &myLex)
{
    try{
		myLex.getNextToken("LEFT_PAREN");
		paramL = createParam(myLex);

	if (myLex.top().getMyToken() == "+")
	{
		myOperator = myLex.getNextToken("ADD");
	}

	else  {
		myOperator = myLex.getNextToken("MULTIPLY");
	}
		paramR = createParam(myLex);
		toString();
		myLex.getNextToken("RIGHT_PAREN");
}
    catch(const string &actual)
    {
        destructor();
        //~Expression();
        throw;
    }
    }

//concatenate all three things to the end of the string. the virtual toString from params should point each part one to its needed toString.
string Expression::toString()
{
	//string printStr;
	return "(" + paramL->toString() + myOperator.getMyToken() + paramR->toString() + ")";
	//cout << "printStr: " << printStr;
	//return printStr;
}

void Expression::destructor(){
    if(paramL != NULL){
        delete paramL;
    }
    if(paramR != NULL){
        delete  paramR;
    }
    //myOperator.~Token();
   // cout << "stuff";
}

Expression::~Expression()
{
    //cout << "called ~Exp \n";
    if(paramL != NULL){
        delete paramL;
    }
    if(paramR != NULL){
        delete  paramR;
    }
    //myOperator.~Token();
}
