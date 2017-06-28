#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../AAI/myMovable.h"

using namespace std;
using namespace lgxZJ::IS;

class MoveTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( MoveTest );
    CPPUNIT_TEST( testReplaceReg_ContentReg_ReplaceIt );
    CPPUNIT_TEST( testReplaceReg_MemoryReg_ReplaceIt );
    CPPUNIT_TEST( testReplaceReg_NoReg_DoNothing );
    CPPUNIT_TEST_SUITE_END();

    public:
        void testReplaceReg_ContentReg_ReplaceIt()
        {
            myTemp regDst = Temp_newTemp(), regSrc = Temp_newTemp();
            Move move(regDst, regSrc);

            move.ReplaceReg(regDst, regSrc);

            CPPUNIT_ASSERT_EQUAL( 1, (int)move.GetDstRegs().size() );
            CPPUNIT_ASSERT_EQUAL( regSrc, move.GetDstRegs().at(0) );
        }

        void testReplaceReg_MemoryReg_ReplaceIt()
        {
            myTemp regDst = Temp_newTemp(), regSrc = Temp_newTemp(), regOther = Temp_newTemp();
            Move move(regDst, regSrc, Move::OperandType::Memory, Move::OperandType::Memory);

            move.ReplaceReg(regSrc, regOther);
            move.ReplaceReg(regDst, regSrc);

            CPPUNIT_ASSERT_EQUAL( 2, (int)move.GetSrcRegs().size() );
            CPPUNIT_ASSERT_EQUAL( regSrc, move.GetSrcRegs().at(0) );
            CPPUNIT_ASSERT_EQUAL( regOther, move.GetSrcRegs().at(1) );
        }

        void testReplaceReg_NoReg_DoNothing()
        {
            myTemp regDst = Temp_newTemp(), regOther = Temp_newTemp();
            Move move(regDst, Temp_newTemp());

            move.ReplaceReg(regOther, regDst);

            CPPUNIT_ASSERT_EQUAL( 1, (int)move.GetDstRegs().size() );
            CPPUNIT_ASSERT_EQUAL( regDst, move.GetDstRegs().at(0) );
        }
};


int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(MoveTest::suite());
    runner.run();
    return 0;
}