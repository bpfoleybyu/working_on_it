#include "Database.h"


/*
Adding rule evaluation to the Lab
steps for this
    pull rules into a vector
        for each rule, build relations on the right side, then join(if needed) to make the relation on the left.
        get those relations by pulling the preds from the rule, then select, project, rename on it.
        only once you have this can you run the queries.

        eval first rule, and for everyone after that, join it with r.

        Printing (from 1st example)
        Rule Evalutaion
        cn(c,n) :- snap(S,n,A,P),csg(c,S,G)
            C='CS101', N='C. Brown'
            C='CS101', N='P. Patty'
        etc.

        Schemes populated after _ passes through the Rules.

        Query Evaluation
        cn('CS101',Name)? Yes(3)
            Name='C. Brown'
            Name='P. Patty'
            Name='Snoopy'
        ncg('Snoopy',Course,Grade)? Yes(1)
            Course='CS101', Grade='C'
        

*/


/* this is what will fill up the set of tables, and also pull the queries to evaluate!*/
Database::Database(datalogProgram &datalog_program)
{
	set<vector<string>> headers = datalog_program.getHeader();
	set<vector<string>> rows = datalog_program.getRows();
	vector<vector<string>> vecQueries = datalog_program.getQueries();
        vector<rule> rules = datalog_program.getRules(); //fill rules to eval later

        fillDatabase(headers, rows, vecQueries); //builds the tables.

        cout << "Rule Evaluation \n";
        set<Table> ruleTables = evalRules(rules);
        //tables.insert(ruleTables.begin(), ruleTables.end()); // union table and rule Tables
        tables = ruleTables;

        //Run the Queries
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
		print(renamedTable, vecQueries, i, printIt, var);
        }
}
/*
Eval the first rule, if there is more than one, join the next with first.
*/
set<Table> Database::evalRules(vector<rule> &rules)
{
    set<Table> ruleTables;
    for(unsigned int i = 0; i < rules.size(); i++)
    {
        rule temp = rules[i];                                   //this is an individual rule. i.e rule(a,b):- la(n,b), na(a,c).
        HeadPredicate tempHead = temp.getHead();
        string newName = tempHead.getHeadId();                  //new name for the rel
        vector<string> tempCols = tempHead.getColNames();       //columns of headPread i.e. new(a,b,c)
        cout << temp.ruleToString();                        // this **PRINT** the cn(c,n):-snap(s,n,a,p), csg(c,s,g).

        vector<Predicate> tempPreds = temp.getPred();
        vector<pair<int,int>> matchingCols;

        Predicate firstPred = tempPreds[0];                     // first rel
        string firstName = firstPred.getPredId();               // name of rel
        vector<string> firstQueries = firstPred.getQuery();     //query of rel
        Header firstHead = buildHeader(firstQueries);           // builds header from queries.

        set<Table> firstRel = doRule(firstName, firstQueries);
        Table firstT = *firstRel.begin();                       //pulls first table its all you need.

        Header toProject;
//join
        for(unsigned int k = 1; k < tempPreds.size(); k++)      // if more than one, keep joining it to first.
        {
            Predicate tPred = tempPreds[k];                     // first rel
            string tName = tPred.getPredId();                   // name of rel
            vector<string> tQueries = tPred.getQuery();         //query of rel
            Table second = *doRule(tName, tQueries).begin();    //Table you will join

            firstT = join(firstT, second, newName);             // join the first Table, with the next.
            toProject = firstT.getHeader();
        }

//project & rename
        vector<int> colsToProject = doProject(tempCols, toProject);             // get the colsToProject
        vector<string> colsToRename = getRuleRenames(tempCols, toProject);

        Table projectedRule = firstT.ruleProject(colsToProject);// project needed cols
        Table renamedRule = runRuleRename(colsToRename, projectedRule);
        printRules(renamedRule);

        //tables.insert(renamedRule);

//print
        cout << endl << "Schemes populated after " << " " <<" passes through the Rules.\n";
        set<vector<string>> thisStuff = firstT.getRows();
        set<vector<string>>::iterator printIt;

       /* for(printIt = thisStuff.begin(); printIt != thisStuff.end(); printIt++)//print rows of rel
        {
            vector<string> row = *printIt;
            cout << "new row: ";
            for(auto a: row)
            {
                cout<< a << " ";
            }
            cout <<endl;
        }*/
    }
    return ruleTables;
}

set<Table> Database::doRule(string name, vector<string> &queries)
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

vector<string> Database::getRuleRenames(vector<string> &tempCols, Header &tHeader)
{
    vector<string> renames;
    //cout << "tH size: " << tHeader.size() << endl;
    for(auto a: tHeader)
    {
        for(unsigned int k = 0; k < tempCols.size(); k++)
        {
            if(a == tempCols[k]) renames.push_back(tempCols[k]);
        }
    }
    return renames;
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

    /*cout << "proj cols size: " <<projectCols.size() << "***";
    for(auto b: projectCols)
    {
        cout << " " << b;
    }*/
    //here sort projectCols
    //print is not working because it is printing things in the wrong order, need to reorder the params, and the header accordingly.

    //sort(projectCols.begin(), projectCols.end()); //need header to be reordered too if I do this.
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
            tables.insert(Table(tempH, tempName, tempRows));
    }
}

Header Database::getSchema(Table left, Table right, vector<pair<int,int>> &matchingCols, vector<int> & uniqueCols)
{
    Header leftH = left.getHeader();
    Header rightH = right.getHeader();
    set<string> toAdd;

    for(unsigned int i = 0; i < rightH.size(); i++){ // if things match, add them to matchingCols
        for(unsigned int j = 0; j < leftH.size(); j++)
        {
            if(leftH[j] == rightH[i]){
                matchingCols.push_back(make_pair(j,i));
            }
        }
        Header::iterator p = find(leftH.begin(), leftH.end(), rightH[i]); // check for unique vals (Things to add)
        if(p == leftH.end())
        uniqueCols.push_back(i);
    }

    set<string>::iterator it;
    for(unsigned int i = 0; i < uniqueCols.size(); i++) //add ToAdd to the name
    {
        leftH.push_back(rightH[uniqueCols[i]]);
    }

    //cout << "newSchema: ";
    Header newHeader = leftH;
    /*for(auto a: newHeader) //print out
    {
        cout << a << " ";
    }
    cout <<endl;*/

    return newHeader;
}

set<Table> Database::runSelect(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs, vector<string> tempQ)
{
	set<Table>tempTables;
	set<Table>selectedTables;
	set<Table>::iterator it;
	set<Table>::iterator tempIt;
	//set<Table>::iterator selectIt;
	for (it = tables.begin(); it != tables.end(); it++)
	{
		if((*it).getName() == tempQ[0]){
                    //cout << "running select \n";
                        tempTables.insert((*it).matchSelect(matchPairs));
			tempIt = tempTables.begin();

			for (tempIt = tempTables.begin(); tempIt != tempTables.end(); tempIt++)
			{
				selectedTables.insert((*tempIt).select(pairs));
			}
		}
        }

	return selectedTables;
}

set<Table> Database::runRuleSelect(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs, vector<string> tempQ, string newName)
{
        set<Table>tempTables;
        set<Table>selectedTables;
        set<Table>::iterator it;
        set<Table>::iterator tempIt;
        //set<Table>::iterator selectIt;
        for (it = tables.begin(); it != tables.end(); it++)
        {
                if((*it).getName() == newName){
                        tempTables.insert((*it).matchSelect(matchPairs));
                        tempIt = tempTables.begin();

                        for (tempIt = tempTables.begin(); tempIt != tempTables.end(); tempIt++)
                        {
                                selectedTables.insert((*tempIt).select(pairs));
                        }
                }
        }

        return selectedTables;
}

Table Database::runRuleRename(vector<string> &renames, Table &projectedTable)
{
    return projectedTable.rename(renames);
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
	set<Table>::iterator headIt;
	for (headIt = tables.begin(); headIt != tables.end(); ++headIt) //checking headers
	{
		Table tempTable = *headIt;
		if (tempTable.getName() == tempQ[0]) //if table header matches header on Query then pull stuff from it.
		{
			//then check if matches, if not a string, check for matches.
                        for (unsigned int i = 1; i < tempQ.size(); i++) //ignore the first item of vector, as it is the header.
			{
                            //cout << "i: " << i <<endl << "tempQ: " << tempQ[i] << endl;
				if (tempQ[i].at(0) != '\'')
				{

					if(i ==1){
						projects.push_back(i - 1);
						renames.push_back(tempQ[i]);
                                                //cout << "renames has something : " << renames.size() << " passed this: " <<tempQ[i] <<endl;
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
        //cout << "renames size: " << renames.size() << "\n";
}

void Database::ruleParam(vector<pair<int, int>> &matchPairs, vector<pair<int, string>> &pairs,
                         vector<int> &projects, vector<string> &renames, vector<string> &tempQ, string name) //RM all the +- 1 for header.
{
    set<Table>::iterator headIt;
    for (headIt = tables.begin(); headIt != tables.end(); ++headIt) //checking headers
    {
            Table tempTable = *headIt;
            if (tempTable.getName() == name) //if table header matches header on Query then pull stuff from it.
            {
                    //then check if matches, if not a string, check for matches.
                    for (unsigned int i = 0; i < tempQ.size(); i++)
                    {
                        //cout << "i: " << i <<endl << "tempQ: " << tempQ[i] << endl;
                            if (tempQ[i].at(0) != '\'')
                            {

                                    if(i == 0){
                                            projects.push_back(i);
                                            renames.push_back(tempQ[i]);
                                            //cout << "renames has something : " << renames.size() << " passed this: " <<tempQ[i] <<endl;
                                    }

                                            bool add = true;
                                            ruleGenHelper(tempQ, projects, add, renames, i); //fills projects, renames


                                    ruleGenHelperSecond(tempQ, matchPairs, i); //fillsMatchPairs
                            }
                            else if (tempQ[i].at(0) == '\'') {
                                    pairs.push_back(make_pair(i-1, tempQ[i]));
                                    //cout << "pushBack \n";
                            }
                    }
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
                //cout << "f\n";
                    add = false;
            }
    }
    if(add == true){
        //cout << "t\n";
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
    Header newHeader = getSchema(left, right, matchingCols, uniqueCols); //joins Rel name, and fills matchingCols

    //join Rel
    set<vector<string>> rows;
    set<vector<string>> lRows = left.getRows();
    set<vector<string>> rRows = right.getRows();
    set<vector<string>>::iterator Lit;
    set<vector<string>>::iterator Rit;
    Table joined = Table(newHeader, newName, rows); // this creates a Table off of the first Scheme (left)
    //for each row in left, for each row in right, if they can join, join the rows. and add that row to joined.
    for(Lit = lRows.begin(); Lit != lRows.end(); Lit++){
        for(Rit = rRows.begin(); Rit != rRows.end(); Rit++)
        {
            vector<string> left = *Lit;
            vector<string> right = *Rit;
            if(joinable(left, right, matchingCols))
            {
                vector<string> combinedRow = combineRows(left, right, uniqueCols); //join rows
                joined.addRow(combinedRow); //add to relation.
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

void Database::printRules(Table &renamedTable)
{

    Header gotHeader = renamedTable.getHeader();
    set<vector<string>> gotRows = renamedTable.getRows();
    set<vector<string>>::iterator rowIt;

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

void Database::print(set<Table> &renamedTable, vector<vector<string>> &vecQueries, int j, bool printIt, bool var)
{
	//example X(X,X)? Yes(2)
	//   X = '1', X = '2'
	vector<string> query = vecQueries[j];
	set<Table>::iterator tableIt;

	tableIt = renamedTable.begin();
	Header gotHeader = tableIt->getHeader();

        printHelper(query);

	if (printIt == false /*|| (tableIt)->getName() != query[0]*/) //check if the table name matches, or if you previously said no.
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

Database::~Database()
{
}
