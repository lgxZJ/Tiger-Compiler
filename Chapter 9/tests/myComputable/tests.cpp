#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../AAI/myComputable.h"

#include "../TestFixture.h"

using namespace lgxZJ::IS;

class TwoComputeTest : public CppUnit::TestFixture
{
    public:
        static CppUnit::Test* suite()
        {
            CppUnit::TestSuite* suite = new CppUnit::TestSuite("AddTest");

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testAddCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testAddCtor_LeftRegRightConstValue_SetWhatPassed));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubCtor_LeftRegRightConstValue_SetWhatPassed));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulCtor_RegSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulCtor_ValueSource_SetWhatPassed));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivCtor_RegSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivCtor_ValueSource_SetWhatPassed));
                
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorCtor_LeftRegRightConstValue_SetWhatPassed));
                
            return suite;
        }
        
        ///////////////////////////////////////////////////////////////////////
        //                              tests

        void testAddCtor_BothOperandsReg_SetWhatPassed()
        {
            testTwoOperandCtor_BothOperandsReg_SetWhatPassed<Add>();
        }

        void testAddCtor_LeftRegRightConstValue_SetWhatPassed()
        {
            testTwoOperandCtor_LeftRegRightConstValue_SetWhatPassed<Add>();
        }

        /////////////////////////////

        void testSubCtor_BothOperandsReg_SetWhatPassed()
        {
            testTwoOperandCtor_BothOperandsReg_SetWhatPassed<Sub>();
        }

        void testSubCtor_LeftRegRightConstValue_SetWhatPassed()
        {
            testTwoOperandCtor_LeftRegRightConstValue_SetWhatPassed<Sub>();
        }

        //////////////////////////////

        void testIMulCtor_RegSource_SetWhatPassed()
        {
            testOneOperandCtor_RegSource_SetWhatPassed<IMul>();
        }

        void testIMulCtor_ValueSource_SetWhatPassed()
        {
            testOneOperandCtor_ValueSource_SetWhatPassed<IMul>();
        }

        ///////////////////////////////

        void testIDivCtor_RegSource_SetWhatPassed()
        {
            testOneOperandCtor_RegSource_SetWhatPassed<IMul>();
        }

        void testIDivCtor_ValueSource_SetWhatPassed()
        {
            testOneOperandCtor_ValueSource_SetWhatPassed<IMul>();
        }

        /////////////////////////////

        void testXorCtor_BothOperandsReg_SetWhatPassed()
        {
            testTwoOperandCtor_BothOperandsReg_SetWhatPassed<Xor>();
        }

        void testXorCtor_LeftRegRightConstValue_SetWhatPassed()
        {
            testTwoOperandCtor_LeftRegRightConstValue_SetWhatPassed<Xor>();
        }
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(TwoComputeTest::suite());
    runner.run();
    return 0;
}