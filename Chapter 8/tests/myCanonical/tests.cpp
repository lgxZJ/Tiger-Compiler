#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>

#include "../../myCanonical.h"

////////////////////////////////////////////

using namespace lgxZJ::Canonical;

class CanonicalTest: public CppUnit::TestFixture
{
    public:
        void testLinearize_NonSeqStatement_CollectInResult()
        {
            IR_myStatement nonSeqState = IR_makeLabel((myLabel)12);

            Statements result = Canonical::Linearize(nonSeqState);

            CPPUNIT_ASSERT_EQUAL(result.size(), (size_t)1);
            CPPUNIT_ASSERT_EQUAL(result.at(0), nonSeqState);
        }
};

//////////////////////////////////////////

int main()
{
    CppUnit::TestCaller<CanonicalTest> test(
        "testLinearize", &CanonicalTest::testLinearize_NonSeqStatement_CollectInResult);
    CppUnit::TestResult result;
    test.run(&result);
    return 0;
}