#include "Table.h"



Table::Table()
{
}

//pulls header. adds rows from facts.
Table::Table(Header header, string name, set<vector<string>> &rows)
{
	this->header = header;
	this->name = name;
	this->rows = rows;
}

void Table::addRow(const vector<string> &row)
{
	rows.insert(row);
}

string Table::getName() const
{
	return name;
}

Header Table::getHeader() const
{
	return header;
}

set<vector<string>> Table::getRows() const
{
	return rows;
}

/*checks the queries against the Table. i.e if your fact is AB('a','b') and the query is AB(X, 'b') it will
go through the table, and see if it can find rows have anything in col 1, and a b in col 2.

the check after and delete is not working.

+++ IF IT FINDS THIS, then it will pull the row into a new table and return that table.*/
Table Table::select(vector<pair<int, string>> selects) const
{
        set<vector<string>>tempRows;
        if (selects.size() == 0) {
                tempRows = rows;
        }

        else{
            set<vector<string>>::iterator it;
            for (it = rows.begin(); it != rows.end(); it++)
            {
                    //for each row in the table.. check the colToSearch for the valToFind.
                    vector<string> temp = *it;
                    unsigned int matches = 0; //count the number of tests it passes.


                            for (unsigned int i = 0; i < selects.size(); i++)
                            {
                                    int colToSeach = selects[i].first;
                                    string valToFind = selects[i].second;
                                    if (temp[colToSeach] == valToFind)
                                    {
                                            matches++;
                                    }
                            }

                            if (matches == selects.size())
                            {
                                    tempRows.insert(temp);
                            }
            }
        }

        return Table(header, name, tempRows);
}


//having problems if the first pari of match pairs dont actually match
Table Table::matchSelect(vector<pair<int, int>> matches) const
{
	set<vector<string>> tempRows;
	if (matches.size() == 0) { //if no matches, keep the whole row, as that means there are no variables.
		tempRows = rows;
	}
	//bool aBreak = false;
	for (unsigned int i = 0; i < matches.size(); i++)
	{
		//bool toBreak = false;
		int matchFirst = matches[i].first;
		int matchSec = matches[i].second;

			set<vector<string>> ::iterator it;
			//bool noMatch = true;
			//if the variable at both places match, add the row, to be checked by the next elements.
			if (i == 0) {
                            matchSelectHelper(matchFirst, matchSec, tempRows);
			}
			//now check each of the new ones against this.
			else
			{
				vector<string> eraseIt;
				bool erase = false;
				for (it = tempRows.begin(); it != tempRows.end(); it++)
				{
					vector<string> temp2 = *it;

					if (temp2[matchFirst] != temp2[matchSec]) {
						erase = true;
						eraseIt = temp2;
					}
				}
				if (erase == true) {
					tempRows.erase(eraseIt);
				}
			}

	}
	return Table(header, name, tempRows);
}

void Table::matchSelectHelper(int &matchFirst, int &matchSec, set<vector<string>> &tempRows) const{

    set<vector<string>> ::iterator it;
    for (it = rows.begin(); it != rows.end(); it++)
    {
            const vector<string> &temp = *it;
            if (temp[matchFirst] == temp[matchSec]) {
                    vector<string> tempRow = temp;
                    tempRows.insert(tempRow);
            }
    }
}

/*pulls in selectedTables. uses the projects(cols to Keep) to build new tables.*/
Table Table::project(vector<int> projects) const
{
	set<vector<string>>newRows;
	set<vector<string>>::iterator it;
	Header newHead;
	if (projects.size() == 0) { //if nothing to project.. then just pass the whole thing
		newHead = header;
		newRows = rows;
	}

	else {
		for (unsigned int i = 0; i < projects.size(); i++)
		{
			newHead.push_back(header[projects[i]]); //newHead is given col names from header.
		}

		for (it = rows.begin(); it != rows.end(); it++)
		{

			const vector<string> &temp = *it;
			vector<string> newRow;
			for (unsigned int k = 0; k < projects.size(); k++)
                        {
                                newRow.push_back(temp[projects[k]]);
			}
			newRows.insert(newRow);
		}
	}
	return Table(newHead, name, newRows);
}

/*this pulls the projectedTable. renames the Columns to be the variables from the Query*/
Table Table::rename(vector<string> renames) const
{
	Header newHead;
	if (renames.size() == 0) {
		newHead = header;
	}
	for (unsigned int i = 0; i < renames.size(); i++)
	{
		bool add = true;
		for (unsigned int j = 0; j < newHead.size(); j++)
		{
			//if there are duplicates in the header it eliminates them.
                        if (newHead[j] == renames[i] && i != j) {
				add = false;
                        }
		}
		if (add == true) {
			newHead.push_back(renames[i]);
		}
	}

	set<vector<string>> tempRows = rows;
	return Table(newHead, name, tempRows);
}

//FOR RULES
Table Table::ruleProject(vector<int> &projects) const //this also need to preserve the order
{
        set<vector<string>>newRows;
        set<vector<string>>::iterator it;
        set<int>::iterator intIt;
        Header newHead;
        //unsigned int thisSize = size;

                for (unsigned int i= 0; i <projects.size(); i++)
                {
                        //cout << "p at i: " << projects[i];
                        newHead.push_back(header[projects[i]]); //newHead is given col names from header.
                }

                for (it = rows.begin(); it != rows.end(); it++)
                {
                        const vector<string> &temp = *it;
                        vector<string> newRow;
                        for (unsigned int i= 0; i <projects.size(); i++)
                        {
                            newRow.push_back(temp[projects[i]]); //pull the int from k and inset into the newRow
                        }
                        newRows.insert(newRow);
                }

                if (projects.size() == 0) { //if nothing to project.. then just pass the whole thing
                        newHead = header;
                        newRows = rows;
                }

        return Table(newHead, name, newRows);
}

Table::~Table()
{
}
