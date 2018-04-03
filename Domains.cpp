#include "Domains.h"



Domain::Domain(Facts &myFact)
{
	fillDomain(myFact);
	//cout << printDomain();
}

void Domain::fillDomain(Facts &myFact)
{
	set<string> tempDom;
	tempDom = myFact.getDomain();
	domain.insert(tempDom.begin(), tempDom.end());
}

string Domain::printDomain()
{
	string domPrint;
	domPrint = "Domain(" + to_string(domain.size()) + "):\n";

	set<string>::iterator it;
	for (it = domain.begin(); it != domain.end(); ++it) {
		domPrint += "  " + *it + "\n";
	}
	return domPrint;
}


Domain::~Domain()
{

}
