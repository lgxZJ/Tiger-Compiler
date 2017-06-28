#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../AAI/myComputable.h"
#include "../../CXX_myFrame.h"

using namespace std;
using namespace lgxZJ::IS;

template <typename T>
class TwoComputeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( TwoComputeTest<T> );
    CPPUNIT_TEST( testReplaceReg_HaveSuchRegs_ReplaceIt );
    CPPUNIT_TEST( testReplace_NoSuchRegs_DoNothing );
    CPPUNIT_TEST_SUITE_END();

    public:
        void testReplaceReg_HaveSuchRegs_ReplaceIt()
        {
            myTemp regDst = Temp_newTemp(), regSrc = Temp_newTemp(), regOther = Temp_newTemp();
            T oneObject(regDst, regSrc);

            oneObject.ReplaceReg(regDst, regOther);
            oneObject.ReplaceReg(regSrc, regOther);

            CPPUNIT_ASSERT_EQUAL( 1, (int)oneObject.GetDstRegs().size() );
            CPPUNIT_ASSERT_EQUAL( regOther, oneObject.GetDstRegs().at(0) );
            CPPUNIT_ASSERT_EQUAL( 2, (int)oneObject.GetSrcRegs().size() );
            CPPUNIT_ASSERT_EQUAL( regOther, oneObject.GetSrcRegs().at(0) );
        }

        void testReplace_NoSuchRegs_DoNothing()
        {
            myTemp regDst = Temp_newTemp(), regOther = Temp_newTemp();

            T oneObject(regDst, 12);

            oneObject.ReplaceReg(regOther, regDst);

            CPPUNIT_ASSERT_EQUAL( 1, (int)oneObject.GetDstRegs().size() );
            CPPUNIT_ASSERT_EQUAL( regDst, oneObject.GetDstRegs().at(0) );
            CPPUNIT_ASSERT_EQUAL( 1, (int)oneObject.GetSrcRegs().size() );
            CPPUNIT_ASSERT_EQUAL( regDst, oneObject.GetSrcRegs().at(0) );
        }
};

template <typename T>
class OneComputeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( OneComputeTest<T> );
    CPPUNIT_TEST( testReplaceReg_HaveSuchRegs_ReplaceIt );
    CPPUNIT_TEST( testReplace_NoSuchRegs_DoNothing );
    CPPUNIT_TEST_SUITE_END();

    public:
        void testReplaceReg_HaveSuchRegs_ReplaceIt()
        {
            myTemp regSrc = Temp_newTemp(), regOther = Temp_newTemp();
            T oneObject(regSrc);

            oneObject.ReplaceReg(regSrc, regOther);

            CPPUNIT_ASSERT_EQUAL( 2, (int)oneObject.GetSrcRegs().size() );
            CPPUNIT_ASSERT_EQUAL( regOther, oneObject.GetSrcRegs().at(1) );
        }

        void testReplace_NoSuchRegs_DoNothing()
        {
            myTemp regSrc = Temp_newTemp(), regOther = Temp_newTemp();

            T oneObject(regOther);

            oneObject.ReplaceReg(regSrc, regOther);

            CPPUNIT_ASSERT_EQUAL( 2, (int)oneObject.GetSrcRegs().size() );
            CPPUNIT_ASSERT_EQUAL( Frame_EAX(), oneObject.GetSrcRegs().at(0) );
            CPPUNIT_ASSERT_EQUAL( regOther, oneObject.GetSrcRegs().at(1) );
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(TwoComputeTest<Add>::suite());
    runner.addTest(TwoComputeTest<Sub>::suite());
    runner.addTest(TwoComputeTest<Xor>::suite());
    runner.addTest(OneComputeTest<IMul>::suite());
    runner.addTest(OneComputeTest<IDiv>::suite());
    runner.run();
    return 0;
}