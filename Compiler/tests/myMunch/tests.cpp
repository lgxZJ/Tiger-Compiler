#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../myMunch.h"
#include "../../CXX_myFrame.h"

#include <memory>
#include <algorithm>

using namespace lgxZJ::IS;
using namespace lgxZJ::Canonical;
using namespace std;

class MunchTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(MunchTest);

        CPPUNIT_TEST(testMunchState_NullState_DoNothing);
        CPPUNIT_TEST(testMunchState_LabelStatement_PutLabel);
        CPPUNIT_TEST(testMunchState_JumpState_PutJmp);
        CPPUNIT_TEST(testMunchState_RightRegisterCJump_PutCmpAndJe);
        CPPUNIT_TEST(testMunchState_RightConstCJump_PutCmpAndJl);
        CPPUNIT_TEST(testMunchState_NotEqualCJump_PutJne);
        CPPUNIT_TEST(testMunchState_LessThanCJump_PutJle);
        CPPUNIT_TEST(testMunchState_LessEqualCJump_PutJle);
        CPPUNIT_TEST(testMunchState_GreaterThanCJump_PutJg);
        CPPUNIT_TEST(testMunchState_GreaterEqualCJump_PutJge);
        CPPUNIT_TEST(testMunchState_LeftMemoryRightRegisterMove_PutMove);
        CPPUNIT_TEST(testMunchState_LeftMemoryRightConstMove_PutMove);
        CPPUNIT_TEST(testMunchState_BothOperandRegistersMove_PutMove);
        CPPUNIT_TEST(testMunchState_LeftRegisterRightConstMove_PutMove);
        CPPUNIT_TEST(testMunchState_Exp_MunchExp);

        CPPUNIT_TEST(testMunchExp_NullExp_PutNothingAndReturnNull);
        CPPUNIT_TEST(testMunchExp_BinOperation_ReturnLeftOperandReg);
        CPPUNIT_TEST(testMunchExp_MinusRightRegisterBinOperation_PutSub);
        CPPUNIT_TEST(testMunchExp_MinusRightConstBinOperation_PutSub);
        CPPUNIT_TEST(testMunchExp_MultiplyRightRegisterBinOperation_PutIMul);
        CPPUNIT_TEST(testMunchExp_DivideRightRegisterBinOperation_PutIDiv);
        CPPUNIT_TEST(testMunchExp_XorRightRegisterBinOperation_PutSub);
        CPPUNIT_TEST(testMunchExp_XorRightConstBinOperation_PutSub);
        CPPUNIT_TEST(testMunchExp_MemWithTempInside_PutMove);
        CPPUNIT_TEST(testMunchExp_Temp_NoPut);
        CPPUNIT_TEST(testMunchExp_Temp_ReturnThisTemp);
        CPPUNIT_TEST(testMunchExp_ESeq_MunchState);
        CPPUNIT_TEST(testMunchExp_ESeq_ReturnEffectTemp);
        CPPUNIT_TEST(testMunchExp_Const_PutMove);
        CPPUNIT_TEST(testMunchExp_Call_PutCall);
        CPPUNIT_TEST(testMunchExp_Call_ReturnEax);

        CPPUNIT_TEST(testMunchAll_ByDefault_MunchEveryOne);

        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp()
        {
            Munch::Reset();
        }

        ////////////////////////////////////////////////////////////////////
        //                          test helper

        #define DEFINE_AAI_CHECKER(CHECKER_NAME, strToFind)             \
            static bool CHECKER_NAME(shared_ptr<AAI> ptr)               \
            {                                                           \
                return string::npos != ptr->ToString().find(strToFind); \
            }

        DEFINE_AAI_CHECKER(isAAILabel, ":");
        DEFINE_AAI_CHECKER(isAAIJmp, "jmp");
        DEFINE_AAI_CHECKER(isAAIJe, "je");
        DEFINE_AAI_CHECKER(isAAIJne, "jne");
        DEFINE_AAI_CHECKER(isAAICmp, "cmp");
        DEFINE_AAI_CHECKER(isAAIJl, "jl");
        DEFINE_AAI_CHECKER(isAAIJle, "jle");
        DEFINE_AAI_CHECKER(isAAIJg, "jg");
        DEFINE_AAI_CHECKER(isAAIJge, "jge");
        DEFINE_AAI_CHECKER(isAAIAdd, "add");
        DEFINE_AAI_CHECKER(isAAISub, "sub");
        DEFINE_AAI_CHECKER(isAAIIMul, "imul");
        DEFINE_AAI_CHECKER(isAAIIDiv, "idiv");
        DEFINE_AAI_CHECKER(isAAIMov, "mov");
        DEFINE_AAI_CHECKER(isAAIXor, "xor");
        DEFINE_AAI_CHECKER(isAAICall, "call");

        bool isAAIContainsConst(shared_ptr<AAI> ptr, int constValue)
        {
            return string::npos != ptr->ToString().find(to_string(constValue));
        }

        bool isAAIContainsString(shared_ptr<AAI> ptr, string str)
        {
            return string::npos != ptr->ToString().find(str);
        }

        ////////////////////////////////////////////////////////////////////
        //                          Munch::State
        ////////////////////////////////////////////////////////////////////

        void testMunchState_NullState_DoNothing()
        {
            Munch::State(nullptr);

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL(true, result.empty());
        }

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

        /////////////////////////////////////////////////////////////
        //                      parameterized tests

        void testMunchState_OneCJump_PutOneAAI(
            IR_RelOperator op, bool (checkFunc)(shared_ptr<AAI> ptr))
        {
            Munch::State(IR_makeCJump(op,
                IR_makeTemp(Temp_newTemp()), IR_makeConst(1),
                Temp_newLabel(), Temp_newLabel()));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL(true, checkFunc(result.at(1)));
        }

        ////////////////

        void testMunchState_NotEqualCJump_PutJne()
        {
            testMunchState_OneCJump_PutOneAAI(IR_NotEqual, MunchTest::isAAIJne);
        }

        void testMunchState_LessThanCJump_PutJle()
        {
            testMunchState_OneCJump_PutOneAAI(IR_LessThan, MunchTest::isAAIJl);
        }

        void testMunchState_LessEqualCJump_PutJle()
        {
            testMunchState_OneCJump_PutOneAAI(IR_LessEqual, MunchTest::isAAIJle);
        }

        void testMunchState_GreaterThanCJump_PutJg()
        {
            testMunchState_OneCJump_PutOneAAI(IR_GreaterThan, MunchTest::isAAIJg);
        }

        void testMunchState_GreaterEqualCJump_PutJge()
        {
            testMunchState_OneCJump_PutOneAAI(IR_GreaterEqual, MunchTest::isAAIJge);
        }

        //////////////

        void testMunchState_LeftMemoryRightRegisterMove_PutMove()
        {
            Munch::State(IR_makeMove(
                IR_makeMem(IR_makeTemp(Temp_newTemp())), IR_makeTemp(Temp_newTemp())));

            Instructions result = Munch::GetIns();

            string insStr = result.at(0)->ToString();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(1, (int)count(insStr.begin(), insStr.end(), ')'));
            CPPUNIT_ASSERT_EQUAL(1, (int)count(insStr.begin(), insStr.end(), '(' ));
        }

        void testMunchState_LeftMemoryRightConstMove_PutMove()
        {
            int constValue = 7834532;
            Munch::State(IR_makeMove(
                IR_makeMem(IR_makeTemp(Temp_newTemp())), IR_makeConst(constValue)));

            Instructions result = Munch::GetIns();

            string insStr = result.at(0)->ToString();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(1, (int)count(insStr.begin(), insStr.end(), ')'));
            CPPUNIT_ASSERT_EQUAL(1, (int)count(insStr.begin(), insStr.end(), '(' ));
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

        void testMunchState_Exp_MunchExp()
        {
            Munch::State(IR_makeExp(IR_makeMem(IR_makeTemp(Temp_newTemp()))));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAIMov(result.at(0)));
        }

        /////////////////////////////////////////////////////////////////////////////
        //                          Munch::Exp
        /////////////////////////////////////////////////////////////////////////////

        void testMunchExp_NullExp_PutNothingAndReturnNull()
        {
            myTemp ret = Munch::Exp(nullptr);

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL(true, result.empty());
            CPPUNIT_ASSERT_EQUAL((myTemp)nullptr, ret);
        }

        void testMunchExp_BinOperation_ReturnLeftOperandReg()
        {
            myTemp reg = Temp_newTemp();

            myTemp resultTwo = Munch::Exp(
                IR_makeBinOperation(IR_Xor, IR_makeTemp(reg), IR_makeTemp(Temp_newTemp())));
            myTemp resultOne = Munch::Exp(
                IR_makeBinOperation(IR_Xor, IR_makeTemp(reg), IR_makeConst(2)));


            CPPUNIT_ASSERT_EQUAL(reg, resultOne);
            CPPUNIT_ASSERT_EQUAL(reg, resultTwo);
        }

        /////////////////////////////////////////////////////////////////////////////
        //          parameterized tests

        void testAndOrMinusOrXor_RightRegisterBinOperation_PutOne(
            IR_BinOperator op, bool(checkFunc)(shared_ptr<AAI> ptr))
        {
            myTemp rightReg = Temp_newTemp();

            Munch::Exp(IR_makeBinOperation(
                op, IR_makeTemp(Temp_newTemp()), IR_makeTemp(rightReg)));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(true, checkFunc(result.at(0)));
            CPPUNIT_ASSERT_EQUAL(
                true,
                isAAIContainsConst(result.at(0), Temp_getTempNum(rightReg)));
        }

        void testAndOrMinusOrXor_RightConstBinOperation_PutOne(
            IR_BinOperator op, bool(checkFunc)(shared_ptr<AAI> ptr))
        {
            int constValue = 123445;

            Munch::Exp(IR_makeBinOperation(
                op, IR_makeTemp(Temp_newTemp()), IR_makeConst(constValue)));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(true, checkFunc(result.at(0)));
            CPPUNIT_ASSERT_EQUAL(true, isAAIContainsConst(result.at(0), constValue));
        }

        ///////////////////

        void testMunchExp_MinusRightRegisterBinOperation_PutSub()
        {
            testAndOrMinusOrXor_RightRegisterBinOperation_PutOne(IR_Minus, isAAISub);
        }

        void testMunchExp_MinusRightConstBinOperation_PutSub()
        {
            testAndOrMinusOrXor_RightConstBinOperation_PutOne(IR_Minus, isAAISub);
        }

        void testMunchExp_XorRightRegisterBinOperation_PutSub()
        {
            testAndOrMinusOrXor_RightRegisterBinOperation_PutOne(IR_Xor, isAAIXor);
        }

        void testMunchExp_XorRightConstBinOperation_PutSub()
        {
            testAndOrMinusOrXor_RightConstBinOperation_PutOne(IR_Xor, isAAIXor);
        }

        /////////////////////////////////////////////////////////////
        //              parameterized tests

        void testMultiplyOrDivide_RightRegisterBinOperation_PutOne(
            IR_BinOperator op, bool(checkFunc)(shared_ptr<AAI> ptr))
        {
            myTemp leftReg = Temp_newTemp();
            myTemp rightReg = Temp_newTemp();

            Munch::Exp(IR_makeBinOperation(
                op, IR_makeTemp(leftReg), IR_makeTemp(rightReg)));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAIMov(result.at(0)));
            CPPUNIT_ASSERT_EQUAL(
                true,
                isAAIContainsString(result.at(0), "register" + to_string(Temp_getTempNum(Frame_EAX()))));
            CPPUNIT_ASSERT_EQUAL(
                true,
                isAAIContainsString(result.at(0), "register" + to_string(Temp_getTempNum(leftReg))));
            CPPUNIT_ASSERT_EQUAL(true, checkFunc(result.at(1)));
            CPPUNIT_ASSERT_EQUAL(
                true,
                isAAIContainsString(result.at(1), "register" + to_string(Temp_getTempNum(rightReg))));
        }

        ////////////////////////////////////////
        
        void testMunchExp_MultiplyRightRegisterBinOperation_PutIMul()
        {
            testMultiplyOrDivide_RightRegisterBinOperation_PutOne(IR_Multiply, isAAIIMul);
        }

        void testMunchExp_DivideRightRegisterBinOperation_PutIDiv()
        {
            testMultiplyOrDivide_RightRegisterBinOperation_PutOne(IR_Divide, isAAIIDiv);
        }

        ///////////////////////////////////////////

        void testMunchExp_MemWithTempInside_PutMove()
        {
            myTemp reg = Temp_newTemp();

            Munch::Exp(IR_makeMem(IR_makeTemp(reg)));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAIMov(result.at(0)));
            CPPUNIT_ASSERT_EQUAL(
                true,
                isAAIContainsString(result.at(0), "(register" + to_string(Temp_getTempNum(reg)) + ")" ));
        }

        void testMunchExp_Temp_NoPut()
        {
            Munch::Exp(IR_makeTemp(Temp_newTemp()));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL(true, result.empty());
        }

        void testMunchExp_Temp_ReturnThisTemp()
        {
            myTemp reg = Temp_newTemp();

            myTemp result = Munch::Exp(IR_makeTemp(reg));

            CPPUNIT_ASSERT_EQUAL(reg, result);
        }

        void testMunchExp_ESeq_MunchState()
        {
            myLabel label = Temp_newLabel();

            Munch::Exp(IR_makeESeq(IR_makeLabel(label), IR_makeTemp(Temp_newTemp())));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAILabel(result.at(0)));
        }

        void testMunchExp_ESeq_ReturnEffectTemp()
        {
            myTemp reg = Temp_newTemp();

            myTemp result = Munch::Exp(IR_makeESeq(nullptr, IR_makeTemp(reg)));

            CPPUNIT_ASSERT_EQUAL(reg, result);
        }

        void testMunchExp_Const_PutMove()
        {
            int constValue = 123456;

            Munch::Exp(IR_makeConst(constValue));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAIMov(result.at(0)));
        }

        void testMunchExp_Call_PutCall()
        {
            myString funcName = (myString)"func";
            myTemp secondReg = Temp_newTemp();

            Munch::Exp(IR_makeCall(
                funcName,
                    IR_makeExpList(IR_makeTemp(Temp_newTemp()),
                        IR_makeExpList(IR_makeTemp(secondReg), nullptr))));

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAICall(result.at(0)));
            CPPUNIT_ASSERT_EQUAL(true, isAAIContainsString(result.at(0), funcName));
            CPPUNIT_ASSERT_EQUAL(
                true,
                isAAIContainsString(result.at(0), "register" + to_string(Temp_getTempNum(secondReg))));
        }

        void testMunchExp_Call_ReturnEax()
        {
            myTemp ret = Munch::Exp(IR_makeCall(
                (myString)"func", nullptr));

            CPPUNIT_ASSERT_EQUAL(Frame_EAX() ,ret);
        }

        ////////////////////////////////////////////////////////////////////
        //                          Munch::All
        ////////////////////////////////////////////////////////////////////

        void testMunchAll_ByDefault_MunchEveryOne()
        {
            Statements statements;
            statements.push_back(IR_makeLabel(Temp_newLabel()));
            statements.push_back(IR_makeMove(IR_makeTemp(Temp_newTemp()), IR_makeTemp(Temp_newTemp())));
            statements.push_back(IR_makeExp(IR_makeConst(1)));

            Munch::All(statements);

            Instructions result = Munch::GetIns();
            CPPUNIT_ASSERT_EQUAL((size_t)3, result.size());
            CPPUNIT_ASSERT_EQUAL(true, isAAILabel(result.at(0)));
            CPPUNIT_ASSERT_EQUAL(true, isAAIMov(result.at(1)));
            CPPUNIT_ASSERT_EQUAL(true, isAAIMov(result.at(2)));
        }

};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(MunchTest::suite());
    runner.run();
    return 0;
}