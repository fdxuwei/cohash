#include <iostream>
#include <stdlib.h>
#include "CoHash.h"

using namespace std;

int main(int ac, char *av[])
{
	//
	if(ac < 2)
	{
		cout << "Usage: " << av[0] << " replica" << endl;
		return 1;
	}
	int replica = atoi(av[1]);
	//
	CoHash ch(replica);
	cout << "----add node 1,2,3 : " << endl;
	ch.addMachine(1);
	ch.addMachine(2);
	ch.addMachine(3);
	cout << ch.getNodesAsString() << endl;	
	cout << "----remove node 1 : " << endl;
	ch.removeMachine(1);
	cout << ch.getNodesAsString() << endl;
	cout << "----remove node 2 : " << endl;
	ch.removeMachine(2);
	cout << ch.getNodesAsString() << endl;
	cout << "----remove node 3 : " << endl;
	ch.removeMachine(3);
	cout << ch.getNodesAsString() << endl;

	//
	return 0;
}