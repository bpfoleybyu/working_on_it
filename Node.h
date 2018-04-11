#pragma once
#include <set>

using namespace std;

class Node
{
	private:
		bool visited;
		bool selfDependant;

	public:
		Node();
        ~Node();
        
        set<int> dependants;
		void setVisitedTrue();
		void setSelfDependantTrue();
		void insertDependant(int &toInsert);
        bool getVisited() {return visited;}
        bool getSelfDependant() {return selfDependant;}
};
