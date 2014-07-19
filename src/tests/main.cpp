/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */

#include <Core.hpp>

#include <AvlTree.hpp>
#include <AvlTests.hpp>

#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdexcept>

using namespace std;

int defaultTestSize = 2048;

typedef struct __options_t {
    bool checkIntegrity;
    int testSize;
} options_t;

int parseArgs(int argc, char * argv[], options_t& opts);
void printUsage(const char * progName);

int main(int argc, char * argv[])
{
	int rc = 0;

    try 
	{
        logdbg << "Debugging output is enabled (NDEBUG is NOT defined)" << endl;
        logtrace << "Tracing output is enabled (TRACE is defined)" << endl;

        options_t opts;
        if(parseArgs(argc, argv, opts) == -1) {
            printUsage(argv[0]);
            return 1;
        }

		AvlTests tester(opts.checkIntegrity, opts.testSize);
		clock_t begin = clock(), end;

		tester.testComparator();
		tester.testHeight();
		tester.testRandomInserts();
		tester.testRemoves();

		end = clock();
		double time = (double)(end - begin) / CLOCKS_PER_SEC;
		logdbg << "Done! Inserted " << opts.testSize << " items" << endl;
		logdbg << "Test succeeded in " << time << " seconds!" << endl;

        //long maxDigits = ceil(log10(static_cast<double>(range)));
        //tester.printTree(cout, maxDigits);

        loginfo << "Test finished successfully!" << endl;
	}
	catch(exception * e)
	{
		logerror << "Exception caught: " << e->what() << endl;
        logerror << "Testing failed!" << endl;
        delete e;
		rc = -1;
	}
	
    loginfo << "Exited gracefully!" << endl;
	return rc;
}

int parseArgs(int argc, char * argv[], options_t& opts)
{
    memset(&opts, 0, sizeof(opts));
    opts.testSize = defaultTestSize;
#ifndef NDEBUG
    logdbg << "Enforcing integrity check. Please compile with -DNDEBUG to disable this." << endl;
    opts.checkIntegrity = true;
#endif

    int i  = 1;
    while(i < argc)
    {
        string arg(argv[i]);
        
        if(arg == "-i" || arg == "--check-integritty") {
            opts.checkIntegrity = true;
        } else if(arg == "-s" || arg == "--test-size") {
            if(i + 1 < argc && (opts.testSize = atoi(argv[i+1])) != 0) {
                // We're good.
                i++;
            } else {
                logerror << "Expecting a numeric argument after -s or --test-size. Got '" << argv[i+1] << "'" << endl << endl;
                return -1;
            }
        } else if(arg == "-h" || arg == "--help") {
            return -1;
        } else {
            logerror << "Unknown argument: '" << arg << "'" << endl << endl;
            return -1;
        }

        i++;
    }

    logdbg << "Arguments parsed: " << endl
        << "\tIntegrity check: " << boolalpha << opts.checkIntegrity << endl
        << "\tTest size: " << opts.testSize << endl << endl;

    return 0;
}

void printUsage(const char * progName)
{
    cout << "Usage: " << progName << " [OPTIONS]" << endl;
    cout << endl;
    cout << "OPTIONS:" << endl;
    cout << "   -i, --check-integrity    enables AVL integrity checks after every insertion (O(n) work at each insert, slows down tester)" << endl;
    cout << "   -s, --test-size <size>   change the default test size (" << defaultTestSize << ")" << endl;
    cout << endl;
}
