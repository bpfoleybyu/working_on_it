/*creates stores the set of tables. the constructor builds each table with the information from the datalog_program.*/

#pragma once
#include "Table.h"
#include"Header.h"
#include <set>
#include <map>
#include "datalogProgram.h"
#include <algorithm>
#include "Node.h"
#include <stack>


using namespace std;

class Database
{
	private:
		map<string, Table> mapOfTables;
        map<int, Node> forwardGraph;
        map<int, Node> reverseGraph;
        stack<int> postOrderStack;
        vector<set<int>> scc;
        set<int> dfsSet;
        bool added;
        
	public:
		Database(datalogProgram &datalog_program);
		void fillDatabase(set<vector<string>> &headers, set<vector<string>> &rows, vector<vector<string>> &vecQueries); //just to fix complexity in constructor.
		void printHelper(vector<string> &query); //fix complexity in print
		void generatorHelper(vector<string> &tempQ, vector<int> &projects, bool &add, vector<string> &renames, unsigned int i); //fix complexity in genParams
		void generatorHelperSecond(vector<string> &tempQ, vector<pair<int, int>> &matchPairs, unsigned int i);

		set<Table> runSelect(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs, vector<string> tempQ); //pass in tables, return selectedTables.
		set<Table> runRuleSelect(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs, vector<string> tempQ, string newName); 
		set<Table> runProject(vector<int> &projects, set<Table> &selectedTables);
		Table runRuleProject(vector<int> &projects, Table &selectedTables); //for Rules, need to be able to switch order
		Table renameForUnion(Table &projectedTable, string name);

		set<Table> runRename(vector<string> &renames, set<Table> &projectedTables);
		void evalRules(rule &rule_); //this builds a relation for each rule on the rhs. preps them to be joined.
		vector<int> doProject(vector<string> &tempCols, Header &jHeader); //sets up projects, in a set to maintain order!

		set<Table> doRule(string name, vector<string> &queries, vector<string> &newPreds); // does all of the work for the first rule

		Header getSchema(Table left, Table right, vector<pair<int,int>> &matchingCols, vector<int> &uniqueCols);
		Table join(Table left, Table right, string newName); // 1. join header 2. test 3. join
                vector<string> combineRows(vector<string> &left, vector<string> &right, vector<int> &uniqueCols); //joins the rows.

		bool joinable(vector<string> &left, vector<string> &right, vector<pair<int, int>> &matchPairs); //returns true if you should join it
		void generateParams(vector<vector<string>> &queries, vector<pair<int, int>> &matchPairs,
				vector<pair<int, string>> &pairs, vector<int> &projects, vector<string> &renames, vector<string> &tempQ); //this will fill matchPairs, pairs, projects, renames, etc.

		void ruleParam(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs, vector<int> &projects, vector<string> &renames, vector<string> &tempQ, string name); //this is for normal project

		bool addedRow(set<vector<string>> t1, vector<string> t2); //check if the row exists in the thing already.
		int getSizeOfTables();
		void addAllRows(Table readyToUnionTable, map<string, Table>::iterator thisTableInTables); // for use in Union

		void ruleGenHelper(vector<string> &tempQ, vector<int> &projects, bool &add, vector<string> &renames, unsigned int i);
		void ruleGenHelperSecond(vector<string> &tempQ, vector<pair<int, int>> &matchPairs, unsigned int i);
		Header buildHeader(vector<string> strings);

		void unionTables(Table &newTs); //checks name, adds new rows, keeps track of things added.

		void print(set<Table> &renamedTable, vector<vector<string>> &vecQueries, int j, bool printIt, bool var);
		void printVecRules(Header printHeader, vector<string> printVec); //prints right after adding

		//ruleOpt functions
		void fillGraphs(vector<rule>& rules); //fills the graphs
		void dfsReverse(int node); //run the first dfs, ths runs on the reverse graph to fill stack
		void dfsForward(int node); //runs off the input from the stack and the forward graph to fill SCC.
        void printGraphs(); //to check if filling correctly
        void fillStack(); //runs dfsReverse
        void fillSCC(); //runs dfsForward
        void setAddedFalse() {added = false;}
        void setAddedTrue() {added = true;}
        void runOptRules(vector<rule>& rules);
        bool getAdded() {return added;}
        void runDoWhile(set<int> &temp, vector<rule> &rules, int &passes);
		~Database();
};
