/*starting point for the grammar. defines it
  datalogParser ->SCHEMES COLON scheme schemeList ,FACTS COLON factList, RULES COLON ruleList, QUERIES COLON query queryList

  passes in the lexer obj called myLexer
  also have private vectors in here that you can use to fill it up.
 */

#pragma once

#include "Lexer.h"
#include "Schemes.h"
#include "Facts.h"
#include "Rules.h"
#include "Queries.h"
#include "Domains.h"
#include "Fact.h"
#include<iostream>
#include<vector>
#include<string>
#include<set>


class datalogProgram
{
	private:
		string printString;
		Schemes schemes_;
		Facts facts_;
		Rules rules_;
		Queries queries_;
		set<vector<string>> header;
		set<vector<string>> rows;
		vector<vector<string>> queries;
        vector<rule> rules;

	public:
		datalogProgram();
		datalogProgram(Lexer &myLex);

		set<vector<string>> getHeader();
		set<vector<string>> getRows();
		vector<vector<string>> getQueries();
        vector<rule> getRules();

		string getPrint();
		void destructor(Rules &r, Queries &q);
		void outsideDest();
		~datalogProgram();
};

