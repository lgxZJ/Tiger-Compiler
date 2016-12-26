#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../AAI/myMovable.h"

#include <algorithm>

using namespace std;
using namespace lgxZJ::IS;

class MoveFaker : public Move
{
    public:
        explicit MoveFaker(Move move) : Move(move)  {}

        myTemp GetDstReg() const        { return dstReg; }
        myTemp GetSrcReg() const        { return srcRep.u.reg;; }
        OperandType GetDstType() const  { return dstType; }
        OperandType GetSrcType() const  { return srcType; }
        int GetSrcValue() const         { return srcRep.u.value; }
};

class MoveTest : public CppUnit::TestFixture
{
    public:
        static CppUnit::Test* suite()
        {
            CppUnit::TestSuite* suite = new CppUnit::TestSuite("MoveTest");

            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testCtor_TwoContentOperands_SetAsItIs));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testCtor_TwoMemoryOperands_SetAsItIs));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testCtor_LeftMemoryRightValue_SetAsItIs));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testToString_LeftMemory_ContainsOnePairSquareBrackets));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testToString_LeftContentRightValue_NoSquareBrackets));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testGetDstRegs_DstRegsAsContent_ReturnLeftReg));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testGetDstRegs_DstRegsAsMemory_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testGetSrcRegs_LeftRegRightRegBothAsContent_ReturnRightReg));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testGetSrcRegs_LeftRegAsMemoryRightRegAsContent_ReturnLeftAndRightReg));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testGetSrcRegs_LeftRegAsContentRightRegAsMemory_ReturnRightReg));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testGetSrcRegs_LeftRegAsMemoryRightRegAsMemory_ReturnLeftAndRightReg));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testGetSrcRegs_RightOperandConst_ReturnLeftWhenRegAsMemoryElseEmpty));
            suite->addTest(new CppUnit::TestCaller<MoveTest>(
                "testCtor", &MoveTest::testGetDstLabel_BeDefault_ReturnNull));
            
            return suite;
        }

        /////////////////////////////////////////////////////////////////
        //                          tests

        void testCtor_TwoContentOperands_SetAsItIs()
        {
            myTemp dstReg = Temp_newTemp();
            myTemp srcReg = Temp_newTemp();
            Move one(dstReg, srcReg);

            MoveFaker moveFaker(one);
            CPPUNIT_ASSERT_EQUAL(dstReg, moveFaker.GetDstReg());
            CPPUNIT_ASSERT_EQUAL(srcReg, moveFaker.GetSrcReg());
            CPPUNIT_ASSERT_EQUAL(Move::OperandType::Content, moveFaker.GetDstType());
            CPPUNIT_ASSERT_EQUAL(Move::OperandType::Content, moveFaker.GetSrcType());
        }

        void testCtor_TwoMemoryOperands_SetAsItIs()
        {
            myTemp dstReg = Temp_newTemp();
            myTemp srcReg = Temp_newTemp();
            Move one(dstReg, srcReg, Move::OperandType::Memory, Move::OperandType::Memory);

            MoveFaker moveFaker(one);
            CPPUNIT_ASSERT_EQUAL(dstReg, moveFaker.GetDstReg());
            CPPUNIT_ASSERT_EQUAL(srcReg, moveFaker.GetSrcReg());
            CPPUNIT_ASSERT_EQUAL(Move::OperandType::Memory, moveFaker.GetDstType());
            CPPUNIT_ASSERT_EQUAL(Move::OperandType::Memory, moveFaker.GetSrcType());
        }

        void testCtor_LeftMemoryRightValue_SetAsItIs()
        {
            myTemp dstReg = Temp_newTemp();
            int srcValue = 1;
            Move one(dstReg, Move::OperandType::Memory, srcValue);

            MoveFaker moveFaker(one);
            CPPUNIT_ASSERT_EQUAL(dstReg, moveFaker.GetDstReg());
            CPPUNIT_ASSERT_EQUAL(srcValue, moveFaker.GetSrcValue());
            CPPUNIT_ASSERT_EQUAL(Move::OperandType::Memory, moveFaker.GetDstType());
        }

        //////////////////////////////////

        void testToString_LeftMemory_ContainsOnePairSquareBrackets()
        {
            myTemp dstReg = Temp_newTemp();
            myTemp srcReg = Temp_newTemp();
            Move one(dstReg, srcReg, Move::OperandType::Content, Move::OperandType::Memory);

            string result = one.ToString();

            CPPUNIT_ASSERT_EQUAL(1, (int)count(result.begin(), result.end(), '['));
            CPPUNIT_ASSERT_EQUAL(1, (int)count(result.begin(), result.end(), ']'));
        }

        void testToString_LeftContentRightValue_NoSquareBrackets()
        {
            myTemp dstReg = Temp_newTemp();
            Move one(dstReg, Move::OperandType::Content, 1);

            string result = one.ToString();

            CPPUNIT_ASSERT_EQUAL(0, (int)count(result.begin(), result.end(), '['));
            CPPUNIT_ASSERT_EQUAL(0, (int)count(result.begin(), result.end(), ']'));
        }

        void testGetDstRegs_DstRegsAsContent_ReturnLeftReg()
        {
            myTemp dstReg = Temp_newTemp();
            Move one(dstReg, Move::OperandType::Content, 1);

            Registers result = one.GetDstRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(dstReg, result.at(0));
        }

        void testGetDstRegs_DstRegsAsMemory_ReturnEmpty()
        {
            myTemp dstReg = Temp_newTemp();
            Move one(dstReg, Move::OperandType::Memory, 1);

            Registers result = one.GetDstRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)0, result.size());
        }

        void testGetSrcRegs_LeftRegRightRegBothAsContent_ReturnRightReg()
        {
            myTemp dstReg = Temp_newTemp();
            myTemp srcReg = Temp_newTemp();
            Move one(dstReg, srcReg);

            Registers result = one.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(srcReg, result.at(0));
        }

        void testGetSrcRegs_LeftRegAsMemoryRightRegAsContent_ReturnLeftAndRightReg()
        {
            myTemp dstReg = Temp_newTemp();
            myTemp srcReg = Temp_newTemp();
            Move one(dstReg, srcReg, Move::OperandType::Memory);

            Registers result = one.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
            CPPUNIT_ASSERT_EQUAL(dstReg, result.at(0));
            CPPUNIT_ASSERT_EQUAL(srcReg, result.at(1));
        }

        void testGetSrcRegs_LeftRegAsContentRightRegAsMemory_ReturnRightReg()
        {
            myTemp dstReg = Temp_newTemp();
            myTemp srcReg = Temp_newTemp();
            Move one(dstReg, srcReg, Move::OperandType::Content, Move::OperandType::Memory);

            Registers result = one.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(srcReg, result.at(0));
        }

        void testGetSrcRegs_LeftRegAsMemoryRightRegAsMemory_ReturnLeftAndRightReg()
        {
            myTemp dstReg = Temp_newTemp();
            myTemp srcReg = Temp_newTemp();
            Move one(dstReg, srcReg, Move::OperandType::Memory, Move::OperandType::Memory);

            Registers result = one.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
            CPPUNIT_ASSERT_EQUAL(dstReg, result.at(0));
            CPPUNIT_ASSERT_EQUAL(srcReg, result.at(1));
        }

        void testGetSrcRegs_RightOperandConst_ReturnLeftWhenRegAsMemoryElseEmpty()
        {
            myTemp dstReg = Temp_newTemp();
            Move one(dstReg, Move::OperandType::Content, 1);
            Move two(dstReg, Move::OperandType::Memory, 1);

            Registers oneResult = one.GetSrcRegs();
            Registers twoResult = two.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)0, oneResult.size());
            CPPUNIT_ASSERT_EQUAL((size_t)1, twoResult.size());
            CPPUNIT_ASSERT_EQUAL(dstReg, twoResult.at(0));
        }

        void testGetDstLabel_BeDefault_ReturnNull()
        {
            Move move(Temp_newTemp(), Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), move.GetDstLabel());
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(MoveTest::suite());
    runner.run();
    return 0;
}