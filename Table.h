/*Table class. contains the set of rows, a constructor, and 
  the three functions, select, project and rename.

  general process.
  1. generate params.
  2. run select matches
  3. run select
  4. project
  5. rename

select: two different functions. first handles the matching variables, second handles other ones.

include links other .h files.*/

#pragma once
#include<string>
#include<set>
#include<utility>
#include "Header.h"
#include"Row.h"
#include"datalogProgram.h"

using namespace std;

class Table
{
	private:
		string name; //the id pulled from the header.
		Header header;// a Schemes. each scheme is a col.
		set<vector<string>> rows; //each row is facts. each fact into a col, matching with scheme.

	public:
		Table();
		Table(Header header, string name, set<vector<string>> &rows); //pass in name and header
		
		bool operator <(const Table &rhs) const //have to include this for sets to work.
			{
				return name < rhs.name;
			}
			
		void addRow(vector<string> row);//need an add row function. addRow(Row row);
		void matchSelectHelper(int &matchFirst, int &matchSec, set<vector<string>> &tempRows) const; //fix complexity

		string getName() const;
		Header getHeader() const;
		set<vector<string>> getRows() const;
		Table select(vector<pair<int, string>> selects) const; //select function. int is the column number. string what is in that col in the row.
		Table matchSelect(vector<pair<int, int>> matches) const; //select function for matches. int is column numbers with the same var.
		Table project(vector<int> projects)const;//project function. project(set<int> columnsToKeep)
		Table rename(vector<string> renames) const;//rename function. takes the strings, those replace the col names from project.
		
		Table ruleProject(vector<int> &projects) const; //pass in projects AND the number of rows.
        
		~Table();
};

