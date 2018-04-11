#include "Node.h"

Node::Node()
{
	visited = false;
	selfDependant = false;
}

void Node::setVisitedTrue()
{
	visited = true;
}

void Node::setSelfDependantTrue()
{
	selfDependant = true;
}

void Node::insertDependant(int &toInsert)
{
        dependants.insert(toInsert);
}

Node::~Node()
{
}

