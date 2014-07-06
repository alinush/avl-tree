/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */

#include <Core.hpp>

#include <AvlTree.hpp>
#include <AvlTest.hpp>

#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdexcept>

using namespace std;

int defaultTestSize = 16384;

typedef struct __options_t {
    bool checkIntegrity;
    int testSize;
} options_t;

int parseArgs(int argc, char * argv[], options_t& opts);
void printUsage(const char * progName);

int main(int argc, char * argv[])
{
    try 
	{
        logdbg << "Debugging output is enabled (NDEBUG is NOT defined)" << endl;
        logtrace << "Tracing output is enabled (TRACE is defined)" << endl;

        options_t opts;
        if(parseArgs(argc, argv, opts) == -1) {
            printUsage(argv[0]);
            return 1;
        }

		AvlTreeTester tester(opts.checkIntegrity);

		loginfo << "Inserting " << opts.testSize << " random numbers..." << endl;
		tester.testRandom(opts.testSize);
        loginfo << "Test finished successfully!" << endl;
	}
	catch(exception * e)
	{
		logerror << "Exception caught: " << endl;
		logerror << "\t" << e->what() << endl;
        logerror << "Test failed" << endl;
		return -1;
	}
	
    loginfo << "Exited gracefully!" << endl;
	return 0;
}

int parseArgs(int argc, char * argv[], options_t& opts)
{
    memset(&opts, 0, sizeof(opts));
    opts.testSize = defaultTestSize;

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

    logtrace << "Arguments parsed: " << endl
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
