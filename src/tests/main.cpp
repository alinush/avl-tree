/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */

#include <AvlTree.h>
#include <AvlTest.h>

#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
	try 
	{
		AvlTreeTester tester;
		tester.testRandom();
	}
	catch(std::exception * e)
	{
		cout << "Exception caught:\n";
		cout << "\t" << e->what() << endl;
		return -1;
	}
	
	return 0;
}

