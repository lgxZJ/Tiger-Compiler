#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../myMunch.h"

#include <memory>
#include <algorithm>

using namespace lgxZJ::IS;
using namespace std;

class MunchTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(MunchTest);

        CPPUNIT_TEST(testMunchState_LabelStatement_PutLabel);
        CPPUNIT_TEST(testMunchState_JumpState_PutJmp);
        CPPUNIT_TEST(testMunchState_RightRegisterCJump_PutCmpAndJe);
        CPPUNIT_TEST(testMunchState_RightConstCJump_PutCmpAndJl);
        CPPUNIT_TEST(testMunchState_LeftMemoryRightRegisterMove_PutMove);
        CPPUNIT_TEST(testMunchState_LeftMemoryRightConstMove_PutMove);
        CPPUNIT_TEST(testMunchState_BothOperandRegistersMove_PutMove);
        CPPUNIT_TEST(testMunchState_LeftRegisterRightConstMove_PutMove);

        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp()
        {
            Munch::Reset();
        }

        ////////////////////////////////////////////////////////////////////
        //                          test helper

        #define DEFINE_AAI_CHECKER(CHECKER_NAME, strToFind)             \
            bool CHECKER_NAME(shared_ptr<AAI> ptr)                      \
            {                                                           \
                return string::npos != ptr->ToString().find(strToFind); \
            }

        DEFINE_AAI_CHECKER(isAAILabel, ":");
        DEFINE_AAI_CHECKER(isAAIJmp, "jmp");
        DEFINE_AAI_CHECKER(isAAIJe, "je");
        DEFINE_AAI_CHECKER(isAAICmp, "cmp")
        DEFINE_AAI_CHECKER(isAAIJl, "jl")

        bool isAAIContainsConst(shared_ptr<AAI> ptr, int constValue)
        {
            return string::npos != ptr->ToString().find(to_string(constValue));
        }

        bool isAAIContainsString(shared_ptr<AAI> ptr, string str)
        {
            return string::npos != ptr->ToString().find(str);
        }

        ////////////////////////////////////////////////////////////////////
        //                          tests

        void testMunchState_LabelStatement_PutLabel()
        {
            myLabel label = Temp_newLabel();

            Munch::State(IR_makeLabel(label));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAILabel(result.at(0)));
        }

        void testMunchState_JumpState_PutJmp()
        {
            myLabel label = Temp_newLabel();
        
            Munch::State(IR_makeJump(IR_makeName(label), Temp_makeLabelList(label, nullptr)));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT(isAAIJmp(result.at(0)));
        }

        /////////////

        void testMunchState_RightRegisterCJump_PutCmpAndJe()
        {
            myLabel trueLabel = Temp_newLabel();
            myTemp rightReg = Temp_newTemp();

            Munch::State(IR_makeCJump(IR_Equal,
                IR_makeTemp(Temp_newTemp()), IR_makeTemp(rightReg),
                Temp_newLabel(), trueLabel));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAICmp(result.at(0)));
            CPPUNIT_ASSERT_EQUAL(
                true,
                isAAIContainsString(result.at(0), to_string(Temp_getTempNum(rightReg))));
            CPPUNIT_ASSERT_EQUAL(true, isAAIJe(result.at(1)));
        }

        void testMunchState_RightConstCJump_PutCmpAndJl()
        {
            myLabel trueLabel = Temp_newLabel();
            int constValue = 12;

            Munch::State(IR_makeCJump(IR_LessThan,
                IR_makeTemp(Temp_newTemp()), IR_makeConst(constValue),
                trueLabel, Temp_newLabel()));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAICmp(result.at(0)));
            CPPUNIT_ASSERT_EQUAL(
                true,
                isAAIContainsConst(result.at(0), constValue));
            CPPUNIT_ASSERT_EQUAL(true, isAAIJl(result.at(1)));
            CPPUNIT_ASSERT_EQUAL(true, isAAIContainsString(result.at(1), MySymbol_GetName(trueLabel)));
        }

        //////////////

        void testMunchState_LeftMemoryRightRegisterMove_PutMove()
        {
            Munch::State(IR_makeMove(
                IR_makeMem(IR_makeTemp(Temp_newTemp())), IR_makeTemp(Temp_newTemp())));

            Instructions result = Munch::GetIns();

            string insStr = result.at(0)->ToString();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(1, (int)count(insStr.begin(), insStr.end(), ']'));
            CPPUNIT_ASSERT_EQUAL(1, (int)count(insStr.begin(), insStr.end(), '[' ));
        }

        void testMunchState_LeftMemoryRightConstMove_PutMove()
        {
            int constValue = 7834532;
            Munch::State(IR_makeMove(
                IR_makeMem(IR_makeTemp(Temp_newTemp())), IR_makeConst(constValue)));

            Instructions result = Munch::GetIns();

            string insStr = result.at(0)->ToString();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(1, (int)count(insStr.begin(), insStr.end(), ']'));
            CPPUNIT_ASSERT_EQUAL(1, (int)count(insStr.begin(), insStr.end(), '[' ));
            CPPUNIT_ASSERT_EQUAL(true, isAAIContainsConst(result.at(0), constValue));
        }

        void testMunchState_BothOperandRegistersMove_PutMove()
        {
            Munch::State(IR_makeMove(
                IR_makeTemp(Temp_newTemp()), IR_makeTemp(Temp_newTemp())));

            Instructions result = Munch::GetIns();

            string insStr = result.at(0)->ToString();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(0, (int)count(insStr.begin(), insStr.end(), ']'));
            CPPUNIT_ASSERT_EQUAL(0, (int)count(insStr.begin(), insStr.end(), '[' ));
        }

        void testMunchState_LeftRegisterRightConstMove_PutMove()
        {
            int constValue = 12344321;
            Munch::State(IR_makeMove(
                IR_makeTemp(Temp_newTemp()), IR_makeConst(constValue)));

            Instructions result = Munch::GetIns();

            string insStr = result.at(0)->ToString();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(0, (int)count(insStr.begin(), insStr.end(), ']'));
            CPPUNIT_ASSERT_EQUAL(0, (int)count(insStr.begin(), insStr.end(), '[' ));
            CPPUNIT_ASSERT_EQUAL(true, isAAIContainsConst(result.at(0), constValue));
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(MunchTest::suite());
    runner.run();
    return 0;
}