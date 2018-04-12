#include "Database.h"

/*
Adding rule optimizing to the lab

1. Build the dependency graph.
2. Build the reverse dependency graph. (same time as #1).
3. Run DFS-Forest on the reverse dependency graph.
4. Find the strongly connected components (SCCs).
5. Evaluate the rules in each component.

Modifying The Database to evaluate the rules a little bit better.
evaluate the rules based off of the

*/

//HANDLE BACKWARDS EDGES!!!


/* this is what will fill up the set of tables, and also pull the queries to evaluate!*/
Database::Database(datalogProgram &datalog_program)
{
//fillTables
	set<vector<string>> headers = datalog_program.getHeader();
	set<vector<string>> rows = datalog_program.getRows();
	vector<vector<string>> vecQueries = datalog_program.getQueries();
        vector<rule> rules = datalog_program.getRules();
        fillDatabase(headers, rows, vecQueries); //builds the tables.

//Rule Opt
        fillGraphs(rules);
        printGraphs();
        fillStack();
        fillSCC();
        runOptRules(rules);

//Run the Queries
        cout << "\nQuery Evaluation\n";
        for (unsigned int i = 0; i < vecQueries.size(); i++)
	{
		vector<string> tempQ = vecQueries[i];

		vector <pair<int, int>> matchPairs; //for matches
		vector<pair<int, string>> pairs;
		vector<int>projects;
		vector<string>renames;
		generateParams(vecQueries, matchPairs, pairs, projects, renames, tempQ);

		set<Table> selectedTable = runSelect(matchPairs, pairs, tempQ); //select
		set<Table> projectedTable = runProject(projects, selectedTable);//project
		set<Table> renamedTable = runRename(renames, projectedTable); //rename

		//then print here, will be easier then trying to iterate through again at the end
                bool printIt = true;
		set<Table>::iterator tempIt;
		tempIt = selectedTable.begin();
		set<vector<string>> tempRows = tempIt->getRows(); //check if any matches (by what was selected
		if (tempRows.size() == 0)
		{
			printIt = false;
		}
                bool var = true;

		if (renames.size() == 0)
		{
			var = false;
                        //cout << "\n **** \n";
		}
                if(printIt == var) cout << "";
                print(renamedTable, vecQueries, i, printIt, var);
        }
}
/*
Eval the first rule, if there is more than one, join the next with first.
*/
void Database::evalRules(rule &rule_)
{
        rule temp = rule_;                                          //i.e cn(c,n):-snap(s,n,a,p), csg(c,s,g).
        HeadPredicate tempHead = temp.getHead();                    // i.e cn(c,n)
        string newName = tempHead.getHeadId();                      // i.e cn
        vector<string> tempCols = tempHead.getColNames();           // i.e. (c,n)
        cout << temp.ruleToString();                                // this **PRINT** the cn(c,n):-snap(s,n,a,p), csg(c,s,g).

        vector<Predicate> tempPreds = temp.getPred();               //gets (c,n) but in predicate form instead of string.
        Predicate firstPred = tempPreds[0];                         // first rel
        string firstName = firstPred.getPredId();                   // name of rel
        vector<string> firstQueries = firstPred.getQuery();         // query of rel
        Header firstHead = buildHeader(firstQueries);               // builds header from queries.

        set<Table> firstRel = doRule(firstName, firstQueries, tempCols);
        Table firstT = *firstRel.begin();                           //pulls first table its all you need.


//join- pull the first. join the second to it, update the first with that joined Rel, keep joining as needed.
        for(unsigned int k = 1; k < tempPreds.size(); k++)                      // if more than one, keep joining it to first.
        {
            Predicate tPred = tempPreds[k];                                     // first rel
            string tName = tPred.getPredId();                                   // name of rel
            vector<string> tQueries = tPred.getQuery();                         //query of rel
            Table second = *doRule(tName, tQueries, tempCols).begin();          //Table you will join

            firstT = join(firstT, second, newName);                             // join the first Table, with the next.
        }

//project & rename
        Header toProject = firstT.getHeader();
        vector<int> colsToProject = doProject(tempCols, toProject);             // get the colsToProject
        Table projectedRule = firstT.ruleProject(colsToProject);                // project needed cols
        Table renamedRule = renameForUnion(projectedRule, newName);             //rename

//union
        unionTables(renamedRule);
}

void Database::unionTables(Table &newTs) //for each new relation that matches an old, add all the new rows to the old.
{
    string name = newTs.getName();
    map<string, Table>::iterator mapIt = mapOfTables.find(name);

    set<vector<string>> getRowsHere= newTs.getRows();
    for(auto a: getRowsHere)
    {
        int size = (*mapIt).second.getRows().size();
            (*mapIt).second.addRow(a);
        int addedSize = (*mapIt).second.getRows().size();

        if(size != addedSize) // if you added something, print that thing.
        {
            setAddedTrue();
            printVecRules((*mapIt).second.getHeader(), a);
        }
    }
}

int Database::getSizeOfTables()
{
    int size;
    for(auto a: mapOfTables) //for every Table, pull the row size and add it together.
    {
        size += a.second.getRows().size();
    }
    return size;
}

//find table in tables, rename accordingly
Table Database::renameForUnion(Table &projectedTable, string name)
{
        map<string, Table>::iterator it;
	Table renamed;

        it = mapOfTables.find(name);
        Table temp = (*it).second;
        if(it != mapOfTables.end())
        {
            set<vector<string>> rows = projectedTable.getRows();
            renamed = Table(temp.getHeader(), temp.getName(), rows);
        }

	return renamed;
}

bool Database::addedRow(set<vector<string>> t1, vector<string> t2)
{
    set<vector<string>>::iterator it;
    for(it = t1.begin(); it != t1.end(); it++)
    {
        vector<string> temp = *it;
        if(temp == t2) return false; //if the row is already in there, ret false (dont add)
    }
    return true;
}

set<Table> Database::doRule(string name, vector<string> &queries, vector<string> &newPreds)
{
    vector<pair<int, int>> matchPairs;
    vector<pair<int, string>> pairs;
    vector<int> projects;
    vector<string> renames;

    ruleParam(matchPairs, pairs, projects, renames, queries, name);

    set<Table> selected = runRuleSelect(matchPairs, pairs, queries, name);
    set<Table> projected = runProject(projects, selected);
    set<Table> renamed = runRename(renames, projected);

    return renamed;
}

vector<int> Database::doProject(vector<string> &tempCols, Header &jHeader)
{
    vector<int> projectCols;
    for(unsigned int i = 0; i < tempCols.size(); i++) //for each tempCol
    {
        for(unsigned int j = 0; j< jHeader.size(); j++) //for each header
        {
            if(tempCols[i] == jHeader[j]) projectCols.push_back(j); //if the colToKeep matches the header, push_back the location of the header.
        }
    }
    return projectCols;
}

void Database::fillDatabase(set<vector<string>> &headers, set<vector<string>> &rows, vector<vector<string>> &vecQueries){

    set<vector<string>>::iterator it; //for headers
    set<vector<string>>::iterator it2; //for rows
    for (it = headers.begin(); it != headers.end(); ++it)
    {
            const vector<string>& i = *it; //new vector i, given data from the vector inside the set..
            string name = i[0];

            //pulls the header, inserts it into a new table, which is then stored in tables.
            Header tempH;
            for (unsigned int j = 1; j < i.size(); j++)
            {
                    tempH.push_back(i[j]);
            }

            set<vector<string>> tempRows;
            for (it2 = rows.begin(); it2 != rows.end(); ++it2)
            {
                    vector<string> k = *it2; //gets data from the row in the set.
                    string id = k[0];
                    //store the rows that match in a set, then pass that set into the new table.
                    if (id == name) {
                            k.erase(k.begin()); //erase the name it doesnt get stored in the row.
                            for (unsigned int l = 0; l < rows.size(); l++)
                            {
                                    tempRows.insert(k);
                            }
                    }
            }
            string tempName = name;
            Table newT = Table(tempH, tempName, tempRows);
            mapOfTables.insert(pair<string, Table>(tempName, newT));
    }
}

Header Database::getSchema(Table left, Table right, vector<pair<int,int>> &matchingCols, vector<int> & uniqueCols)
{
    Header leftH = left.getHeader();
    Header rightH = right.getHeader();

    for(unsigned int i = 0; i < rightH.size(); i++){ // if things match, add them to matchingCols
        for(unsigned int j = 0; j < leftH.size(); j++)
        {
            if(leftH[j] == rightH[i]){
                matchingCols.push_back(make_pair(j,i));
            }
        }

        Header::iterator p = find(leftH.begin(), leftH.end(), rightH[i]); // check for unique vals (Things to add)
        if(p == leftH.end()) uniqueCols.push_back(i);
    }

    for(unsigned int i = 0; i < uniqueCols.size(); i++) //add ToAdd to the name
    {
        leftH.push_back(rightH[uniqueCols[i]]);
    }

    return leftH;
}

set<Table> Database::runSelect(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs, vector<string> tempQ)
{
	set<Table>tempTables;
        set<Table>selectedTables;

        map<string, Table>::iterator it;
        it = mapOfTables.find(tempQ[0]); //get the map w/ matching name.
        Table tempTable = (*it).second;
        tempTables.insert((tempTable).matchSelect(matchPairs)); //matchSel, then Sel

        for(auto a: tempTables)
        {
            selectedTables.insert((a.select(pairs)));
        }


	return selectedTables;
}

set<Table> Database::runRuleSelect(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs, vector<string> tempQ, string newName)
{
        set<Table>tempTables;
        set<Table>selectedTables;

        map<string, Table>::iterator mapIt;

        mapIt= mapOfTables.find(newName); //get the map w/ matching name.
        Table tempTable = (*mapIt).second;

        tempTables.insert((tempTable).matchSelect(matchPairs)); //matchSel, then Sel

        for(auto a: tempTables)
        {
            selectedTables.insert((a.select(pairs)));
        }

        return selectedTables;
}

set<Table> Database::runProject(vector<int>& projects, set<Table> &selectedTables)
{
	set<Table>::iterator it;
	set<Table>projectedTables; //pulls in the selectedTables and Projects them into projected.
	for (it = selectedTables.begin(); it != selectedTables.end(); ++it)
	{
		projectedTables.insert((*it).project(projects));
	}

	return projectedTables;
}

Table Database::runRuleProject(vector<int> &projects, Table &selectedTables)
{
    return selectedTables.project(projects); //project from selectedTables
}

set<Table> Database::runRename(vector<string>& renames, set<Table>& projectedTables)
{
	set<Table>::iterator it;
	set<Table> renamedTables;
	for (it = projectedTables.begin(); it != projectedTables.end(); it++)
	{
		renamedTables.insert((*it).rename(renames));
	}

	return renamedTables;
}

/*Pass in a Query. pass out Params.
 */
void Database::generateParams(vector<vector<string>> &queries, vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs,
		vector<int> &projects, vector<string> &renames, vector<string> &tempQ)
{

    Table tempTable = (*mapOfTables.find(tempQ[0])).second;
    for (unsigned int i = 0; i < tempQ.size(); i++)
    {
        for (unsigned int i = 1; i < tempQ.size(); i++) //ignore the first item of vector, as it is the header.
        {
                if (tempQ[i].at(0) != '\'')
                {

                        if(i ==1){
                                projects.push_back(i - 1);
                                renames.push_back(tempQ[i]);
                        }

                        else{
                                bool add = true;
                                generatorHelper(tempQ, projects, add, renames, i); //fills projects, renames
                        }

                        generatorHelperSecond(tempQ, matchPairs, i); //fillsMatchPairs
                }
                else if (tempQ[i].at(0) == '\'') {
                        pairs.push_back(make_pair(i-1, tempQ[i]));
                }
        }
    }
}

void Database::ruleParam(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs,
                         vector<int> &projects, vector<string> &renames, vector<string> &tempQ, string name) //before join
{

    map<string, Table>::iterator it;
    it =  mapOfTables.find(name);
    Table tempTable = (*it).second;
    for (unsigned int i = 0; i < tempQ.size(); i++)
    {
        if(tempQ[i].at(0)!= '\'')
        {
            if(i == 0){ //if first time
                projects.push_back(i);
                renames.push_back(tempQ[i]);
            }

                bool add = true;
                ruleGenHelper(tempQ, projects, add, renames, i); //fills projects, renames
                ruleGenHelperSecond(tempQ, matchPairs, i); //fillsMatchPairs
        }
        else
        {

            pairs.push_back(make_pair(i, tempQ[i]));
        }
    }
}

void Database::generatorHelper(vector<string> &tempQ, vector<int> &projects, bool &add, vector<string> &renames, unsigned int i){
    for (unsigned int j = 0; j < projects.size(); j++)
    {
            if(tempQ[projects[j] + 1] == tempQ[i])
            {
                    add = false;
            }
    }
    if(add == true){
            projects.push_back(i - 1);
            renames.push_back(tempQ[i]);
    }
}

void Database::ruleGenHelper(vector<string> &tempQ, vector<int> &projects, bool &add, vector<string> &renames, unsigned int i){
    for (unsigned int j = 0; j < projects.size(); j++)
    {
            if(tempQ[projects[j]] == tempQ[i])
            {
                    add = false;
            }
    }
    if(add == true){
            projects.push_back(i);
            renames.push_back(tempQ[i]);
    }
}


void Database::generatorHelperSecond(vector<string> &tempQ, vector<pair<int, int>> &matchPairs, unsigned int i){
    for (unsigned int j = 1; j < tempQ.size(); j++)
    {
            if (tempQ[i] == tempQ[j] && i < j) {
                    matchPairs.push_back(make_pair(i-1, j-1)); //if the variables match then make it a match p
            }
    }
}

void Database::ruleGenHelperSecond(vector<string> &tempQ, vector<pair<int, int>> &matchPairs, unsigned int i){
    for (unsigned int j = 0; j < tempQ.size(); j++)
    {
            if (tempQ[i] == tempQ[j] && i < j) {
                    matchPairs.push_back(make_pair(i, j)); //if the variables match then make it a match p
            }
    }
}

Table Database::join(Table left, Table right, string newName)
{
    //join Name. take the whole L header, add to it anything from R that is different
    vector<pair<int,int>> matchingCols;
    vector<int> uniqueCols;
    Header newHeader = getSchema(left, right, matchingCols, uniqueCols); //joins Rel name, and fills matchingCols and uniqueCols

    //join Rel
    set<vector<string>> rows;
    set<vector<string>> lRows = left.getRows();
    set<vector<string>> rRows = right.getRows();
    Table joined = Table(newHeader, newName, rows); // this creates a Table off of the first Scheme (left)

    //for each row in left, for each row in right, if they can join, join the rows. and add that row to joined.
    for(set<vector<string>>::iterator Lit = lRows.begin(); Lit != lRows.end(); Lit++){
        for(set<vector<string>>::iterator Rit = rRows.begin(); Rit != rRows.end(); Rit++)
        {
            vector<string> left = *Lit;
            vector<string> right = *Rit;
            if(joinable(left, right, matchingCols))
            {
                vector<string> toAdd = combineRows(left, right, uniqueCols);
                joined.addRow(toAdd); //add to relation.
            }
        }
    }

    return joined;
}

vector<string> Database::combineRows(vector<string> &left, vector<string> &right, vector<int> &uniqueCols) //joins the rows.
{
    //start with left. add to it all of the right that in the uniqueCol spots.
   vector<string> combined = left;

   for(unsigned int i = 0; i < uniqueCols.size(); i++)
   {
       combined.push_back(right[uniqueCols[i]]);
   }
   return combined;
}

bool Database::joinable(vector<string> &left, vector<string> &right, vector<pair<int, int>> &matchPairs) //NOT FUNCTIONAL YET
{
    //check if left and right match at each of the match Pairs. if they dont, then it is not joinable

   for(auto a: matchPairs)
   {
       if(left[a.first] != right[a.second]) return false;
   }
    return true;
}

Header Database::buildHeader(vector<string> strings)
{
    Header head;
    for(auto a: strings)
    {
        head.push_back(a);
    }
    return head;
}

void Database::print(set<Table> &renamedTable, vector<vector<string>> &vecQueries, int j, bool printIt, bool var)
{
	//example X(X,X)? Yes(2)
	//   X = '1', X = '2'
	vector<string> query = vecQueries[j];
	set<Table>::iterator tableIt;

	tableIt = renamedTable.begin();
	Header gotHeader = tableIt->getHeader();

        printHelper(query);

        if (printIt == false) //check if the table name matches, or if you previously said no.
	{
		cout << "No" <<endl;
	}
	else
	{
		if (renamedTable.size() > 0)
		{
			set<vector<string>> gotRows = tableIt->getRows();
			set<vector<string>>::iterator rowIt;
			cout << "Yes(" << gotRows.size() << ")\n";
			if (var == true)
			{
				for (rowIt = gotRows.begin(); rowIt != gotRows.end(); rowIt++)
				{
					vector<string> temp = *rowIt;
					int counter = 0;
                                        cout << "  ";
					for (unsigned int i = 0; i < gotHeader.size(); i++)
					{
						if (counter > 0) {
							cout << ", ";
						}
						cout << gotHeader[i] << "=" << temp[i];
						counter++;
					}
					cout << endl;
				}
			}
		}
	}
}

void Database::printVecRules(Header printHeader, vector<string> printVec)
{
    int counter = 0;
    cout << "  ";
            for(unsigned int j = 0; j < printHeader.size(); j++)
            {
                if(counter > 0) cout << ", ";
                cout << printHeader[j] << "=" << printVec[j];
                counter++;
            }
            cout << endl;
}

void Database::printHelper(vector<string> &query)
{
    cout << query[0] << "(";
    for (unsigned int i = 1; i < query.size(); i++)
    {
            if (i > 1) cout << ",";
            cout << query[i];
    }
    cout << ")? ";
}


//FUNCTIONS FOR RULEOPT

/*for each rule, check if the predicates headers match any of the following rules headers
if they match none, then insert an empty set?
*/
void Database::fillGraphs(vector<rule> &rules)
{
    for(unsigned int i = 0; i < rules.size(); i++)
    {
        //initializeGraphDependents for each graph (Needed so it prints properly).
        forwardGraph[i];
        reverseGraph[i];
        vector<Predicate> tempPreds = rules[i].getPred();
        for(unsigned int j = 0; j < tempPreds.size(); j++ )
        {
            for(unsigned int k = 0; k < rules.size(); k++)
            {
		string headId = rules[k].getHead().getHeadId();
                if(tempPreds[j].getPredId() == headId) //i depends on k.. & k depends on i
			{
                           if(rules[i].getHead().getHeadId() == headId) //then i depends on itself
                                {
                                    forwardGraph[i].setSelfDependantTrue();
                                    reverseGraph[j].setSelfDependantTrue();
                                }
                            forwardGraph[i].dependants.insert(k);
                            reverseGraph[k].dependants.insert(i);
			}
            }
        }
    }
}

void Database::printGraphs()
{
    cout << "Dependency Graph\n";
    int counter = 0;

    for(auto a: forwardGraph)
    {
        cout << "R" << counter << ":";
        set<int> depends = a.second.dependants;
        int comma = 0;
        for(auto b:depends)
        {
            if(comma >0) cout << ",";
            cout << "R"<< b;
            comma++;
        }
        cout <<endl;
        counter++;
    }
    cout << endl;

}

void Database::fillStack()
{
    for(auto a: reverseGraph) //this will fill the stack for the first part
    {
        dfsReverse(a.first);
    }
}

void Database::fillSCC()
{
    //cout << "fGraph size: "<<forwardGraph.size()<<endl;
    while(!postOrderStack.empty())
    {
        dfsSet.clear(); //empties the data member
        int a = postOrderStack.top();
        dfsForward(a);
        postOrderStack.pop();
        if(dfsSet.size() != 0) scc.push_back (dfsSet); // push to SCC
    }
}

void Database::dfsReverse(int node) //run the first dfs, ths runs on the reverse graph to fill stack
{
    if(!reverseGraph[node].getVisited())
    {
        reverseGraph[node].setVisitedTrue();
        for(auto a: reverseGraph[node].dependants)
        {
            dfsReverse(a);
        }
        postOrderStack.push(node);
    }
}

void Database::dfsForward(int node) //this runs off the order from the stack. fills the scc vec<set<int>>
{
    if(!forwardGraph[node].getVisited())
    {

        forwardGraph[node].setVisitedTrue();

        for(auto a: forwardGraph[node].dependants)
        {
            dfsForward(a);

        }
        dfsSet.insert(node);
        //scc.push_back (dfsSet); // push to SCC
    }
}

void Database::runOptRules(vector<rule>& rules)
{
    int counter = 0;
    cout << "Rule Evaluation\n";
    for(auto a: scc)
    {
        int passes = 0;
        int whichRule = 0;
        set<int> temp = a;
        whichRule = *(temp.begin());
        cout << "SCC: ";
        int checker = 0;
            for(auto b: temp)
            {
                if(checker != 0) cout << ",";
                cout << "R"<<b;
                checker++;
            }
             cout << endl;

        bool selfDependant = forwardGraph[whichRule].getSelfDependant();
        if(temp.size() == 1 && selfDependant == false)       //only run once.
        {
            evalRules(rules[whichRule]);
            passes++;
        }

        else
            {
            runDoWhile(temp, rules, passes);
        }

        cout << passes << " passes: ";
        int comma = 0;
        for(auto b: temp)
        {
            if(comma != 0) cout << ",";
            cout << "R"<<b;
            comma++;
        }
        cout<<endl;
        counter++;
    }
}

void Database::runDoWhile(set<int> &temp, vector<rule> &rules, int &passes)
{
    do {
        setAddedFalse();
        for(auto b: temp)
        {
            evalRules(rules[b]);
        }
        passes++;
    } while(getAdded() == true);
}

Database::~Database()
{
}
