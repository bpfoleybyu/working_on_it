#pragma once
#include <set>

using namespace std;

class Node
{
	private:
		Node();
		bool visited;
		bool selfDependant;
		set<int> dependants;
	public:
		~Node();
		void setVisitedTrue();
		void setSelfDependantTrue();
		void insertDependant(int &toInsert);
};
