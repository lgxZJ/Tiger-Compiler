///////////////////////////////////////////////////////////////////////////////
//
//  This tests test whether typical tiger sources can be processed successfully.
//  
///////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdlib.h>

#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
//#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../CXX_frontTester.h"
#include "../../myCanonical.h"
#include "../../myMunch.h"


using namespace lgxZJ::Canonical;
using namespace lgxZJ::IS;

class IntegrationTest: public CppUnit::TestFixture
{
    public:
        static CppUnit::Test* suite()
        {
            CppUnit::TestSuite* suiteOfTests = new CppUnit::TestSuite("CanonicalTest");
            
            suiteOfTests->addTest(new CppUnit::TestCaller<IntegrationTest>(
                "testFlatten", &IntegrationTest::test_ByDefault_CanTranslateSources));
            
            return suiteOfTests;
        }

        ///////////////////////     test helpers    ///////////////////////

        //  test all phases completed so far.
        bool testOneFile(const char* filename)
        {
            IR_myStatement resultState = frontTester(filename);

            Statements states = Canonical::Linearize(resultState);
            Blocks blocks = Canonical::ToBlocks(states);
            Blocks blocksTraced = Canonical::Trace(blocks);
            Statements statesFlattened = Canonical::Flatten(blocksTraced);

            Munch::All(statesFlattened);
            return true;
        }

        void test_ByDefault_CanTranslateSources()
        {
            for (int i = 1; i <= 22; ++i)
            {
                char buffer[256] = {0};
                sprintf(buffer, "../testcases/correct/test%d.tig", i);
                testOneFile(buffer);
            }

            testOneFile("../testcases/correct/queens.tig");
            testOneFile("../testcases/correct/merge.tig");
        }

};

//////////////////////////////////////////

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(IntegrationTest::suite());
    runner.run();
    return 0;
}