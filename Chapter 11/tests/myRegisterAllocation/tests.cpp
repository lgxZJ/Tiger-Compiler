#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../AAI/myMovable.h"
#include "../../AAI/myComputable.h"
#include "../../AAI/myControlable.h"
#include "../../myRegisterAllocation.h"

#include <memory>   //  for make_shared()

using namespace std;
using namespace lgxZJ::IS;
using namespace lgxZJ::RA;

class RegisterAllocationTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( RegisterAllocationTest );
    CPPUNIT_TEST( testCtor_FirstIns_GetCorrectResult );
    CPPUNIT_TEST( testCtor_SecondIns_GetCorrectResult );
    CPPUNIT_TEST( testCtor_ThridIns_GetCorrectResult );
    CPPUNIT_TEST( testCtor_FourthIns_GetCorrectResult );
    CPPUNIT_TEST( testCtor_InsWithPrecoloredRegs_GetCorrectResult );
    CPPUNIT_TEST( testCtor_OnlyNeedCoalesce_DoCorrectCoalesce );
    CPPUNIT_TEST( testCtor_CoalesceButConstrained_DoCorrectCoalesce );
    CPPUNIT_TEST( testCtor_NeedFreeze_DocorrectFreeze );
    CPPUNIT_TEST_SUITE_END();

    public:
        Instructions makeFirstIns(myTemp regA, myTemp regB, myTemp regC)
        {
            myLabel label = Temp_newLabel();

            return Instructions{
                make_shared<Move>(regA, Move::OperandType::Content, 0),
                make_shared<Label>(label),
                make_shared<Add>(regA, 1),
                make_shared<Move>(regB, regA),
                make_shared<Add>(regC, regB),
                make_shared<Add>(regB, 2),
                make_shared<Move>(regA, regB),
                make_shared<Cmp>(regA, 12),
                make_shared<Jl>(label),
            };
        }

        Instructions makeSecondIns(myTemp regA, myTemp regB)
        {
            myLabel label = Temp_newLabel();

            return Instructions{
                make_shared<Move>(regA, Move::OperandType::Content, 0),
                make_shared<IMul>(2),
                make_shared<Jmp>(label),
                make_shared<Call>(Temp_newLabel(), nullptr),
                make_shared<IDiv>(regB),
                make_shared<Label>(label),
            };
        }

        Instructions makeThirdIns(myTemp regA, myTemp regB, myTemp regC, myTemp regD)
        {
            return Instructions{
                make_shared<Move>(regA, regB), 
                make_shared<Add>(regC, regA), 
                make_shared<Add>(regD, regB),
            };
        }

        Instructions makeFourthIns(myTemp regA, myTemp regB, myTemp regC)
        {
            return Instructions {
                make_shared<Move>(regA, regB, Move::OperandType::Memory),
                make_shared<Add>(regC, regA),
                make_shared<Move>(
                    regC, regB, Move::OperandType::Memory, Move::OperandType::Memory),
                make_shared<Move>(
                    regC, regB, Move::OperandType::Content, Move::OperandType::Memory),
            };
        }

        Instructions makeInsWithPrecoloredRegs(myTemp regA, myTemp regB, myTemp regC)
        {
            return Instructions{
            	 make_shared<Move>(regA, regB, Move::OperandType::Memory) ,
            	 make_shared<Add>(regC, regA) ,
            	 make_shared<Move>(
                    regC, regB, Move::OperandType::Memory, Move::OperandType::Memory) ,
            	 make_shared<Move>(
                    regC, regB, Move::OperandType::Content, Move::OperandType::Memory) ,
            };
        }

        Instructions makeInsOnlyNeedCoalesce(myTemp regA, myTemp regB, myTemp regC, myTemp regD)
        {
            return Instructions{
            	 make_shared<Move>(regA, regB) ,
            	 make_shared<Move>(regC, regB) ,
            	 make_shared<Move>(regD, regB) ,
            };
        }

        Instructions makeInsCoalesceButConstrained(myTemp regA, myTemp regB)
        {
            return Instructions{
            	 make_shared<Move>(regA, regB) ,
            	 make_shared<Add>(regA, regB) ,
                 make_shared<Add>(regB, regA) ,
            };
        }

        Instructions makeInsNeedFreeze(
            myTemp regA, myTemp regB, myTemp regC, myTemp regD,
            myTemp regE, myTemp regF, myTemp regG, myTemp regH)
        {
            return Instructions{
            	 make_shared<Add>(regA, regB) ,
                 make_shared<Add>(regA, regC) ,
                 make_shared<Add>(regA, regD) ,
                 make_shared<Add>(regA, regE) ,
                 make_shared<Add>(regA, regF) ,
                 make_shared<Add>(regA, regG) ,
                 make_shared<Add>(regA, regH) ,

                 make_shared<Add>(regB, regA) ,
                 make_shared<Add>(regB, regC) ,
                 make_shared<Add>(regB, regD) ,
                 make_shared<Add>(regB, regE) ,
                 make_shared<Add>(regB, regF) ,
                 make_shared<Add>(regB, regG) ,
                 make_shared<Add>(regB, regH) ,

                 make_shared<Add>(regC, regB) ,
                 make_shared<Add>(regC, regD) ,
                 make_shared<Add>(regC, regE) ,
                 make_shared<Add>(regC, regF) ,
                 make_shared<Add>(regC, regG) ,
                 make_shared<Add>(regC, regH) ,

                 make_shared<Move>(regA, regB),
                 make_shared<Move>(regA, regC) ,
                 make_shared<Move>(regA, regD) ,
                 make_shared<Move>(regA, regE) ,
                 make_shared<Move>(regA, regF) ,
                 make_shared<Move>(regA, regG) ,
                 make_shared<Move>(regA, regH) ,
            };
        }

        ///////////////////////////////////////////////////////////////////////
        //                              tests

        void testCtor_FirstIns_GetCorrectResult()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = Temp_newTemp();

            RegisterAllocation ra(
                makeFirstIns(regA, regB, regC), Trans_outermostLevel());

            RegisterMaps maps = ra.GetRegisterMaps();
            CPPUNIT_ASSERT_EQUAL ( maps[regA], maps[regB] );
            CPPUNIT_ASSERT ( maps[regB] != maps[regC] );
            CPPUNIT_ASSERT ( maps[regA] != maps[regC] );
        }

        void testCtor_SecondIns_GetCorrectResult()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp();

            RegisterAllocation ra(
                makeSecondIns(regA, regB), Trans_outermostLevel());

            RegisterMaps maps = ra.GetRegisterMaps();
            CPPUNIT_ASSERT( maps[regA] != maps[regB] );
        }

        void testCtor_ThridIns_GetCorrectResult()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp();
            myTemp regC = Temp_newTemp(), regD = Temp_newTemp();

            RegisterAllocation ra(
                makeThirdIns(regA, regB, regC, regD), Trans_outermostLevel());

            RegisterMaps maps = ra.GetRegisterMaps();
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regB] );
            CPPUNIT_ASSERT( maps[regA] != maps[regC] );
            CPPUNIT_ASSERT( maps[regB] != maps[regD] );
        }

        void testCtor_FourthIns_GetCorrectResult()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = Temp_newTemp();

            RegisterAllocation ra(
                makeFourthIns(regA, regB, regC), Trans_outermostLevel());

            RegisterMaps maps = ra.GetRegisterMaps();
            CPPUNIT_ASSERT( maps[regB] != maps[regC] );
        }

        void testCtor_InsWithPrecoloredRegs_GetCorrectResult()
        {
            myTemp regEAX = Frame_EAX(), regEBX = Frame_EBX(), regECX = Frame_ECX();

            RegisterAllocation ra(
                makeInsWithPrecoloredRegs(regEAX, regEBX, regECX), Trans_outermostLevel());
            
            RegisterMaps maps = ra.GetRegisterMaps();
            CPPUNIT_ASSERT_EQUAL( 3, (int)maps.size() );
            CPPUNIT_ASSERT_EQUAL( regEBX, maps[regEBX] );
            CPPUNIT_ASSERT_EQUAL( regEAX, maps[regEAX] );
            CPPUNIT_ASSERT_EQUAL( regECX, maps[regECX] );
        }

        void testCtor_OnlyNeedCoalesce_DoCorrectCoalesce()
        {
            myTemp  regA = Temp_newTemp(), regB = Temp_newTemp(),
                    regC = Temp_newTemp(), regD = Temp_newTemp();

            RegisterAllocation ra(
                makeInsOnlyNeedCoalesce(regA, regB, regC, regD), Trans_outermostLevel());

            RegisterMaps maps = ra.GetRegisterMaps();
            CPPUNIT_ASSERT_EQUAL( 4, (int)maps.size() );
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regB] );
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regC] );
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regD] );
        }

        void testCtor_CoalesceButConstrained_DoCorrectCoalesce()
        {
            myTemp  regA = Temp_newTemp(), regB = Temp_newTemp();

            RegisterAllocation ra(
                makeInsCoalesceButConstrained(regA, regB), Trans_outermostLevel());

            RegisterMaps maps = ra.GetRegisterMaps();
            CPPUNIT_ASSERT_EQUAL( 2, (int)maps.size() );
            CPPUNIT_ASSERT( maps[regA] != maps[regB] );
        }

        void testCtor_NeedFreeze_DocorrectFreeze()
        {
            myTemp  regA = Temp_newTemp(), regB = Temp_newTemp(),
                    regC = Temp_newTemp(), regD = Temp_newTemp(),
                    regE = Temp_newTemp(), regF = Temp_newTemp(),
                    regG = Temp_newTemp(), regH = Temp_newTemp();

            RegisterAllocation ra(
                makeInsNeedFreeze(regA, regB, regC, regD, regE, regF, regG, regH),
                Trans_outermostLevel());
            
            RegisterMaps maps = ra.GetRegisterMaps();
            CPPUNIT_ASSERT_EQUAL( 8, (int)maps.size() );
            CPPUNIT_ASSERT( maps[regA] != maps[regB] );
            CPPUNIT_ASSERT( maps[regA] != maps[regC] );
            CPPUNIT_ASSERT( maps[regA] != maps[regD] );            
            CPPUNIT_ASSERT( maps[regA] != maps[regE] );
            CPPUNIT_ASSERT( maps[regA] != maps[regF] );
            CPPUNIT_ASSERT( maps[regA] != maps[regG] );
            CPPUNIT_ASSERT( maps[regA] != maps[regH] );

            CPPUNIT_ASSERT( maps[regB] != maps[regC] );
            CPPUNIT_ASSERT( maps[regB] != maps[regD] );
            CPPUNIT_ASSERT( maps[regB] != maps[regE] );
            CPPUNIT_ASSERT( maps[regB] != maps[regF] );
            CPPUNIT_ASSERT( maps[regB] != maps[regG] );
            CPPUNIT_ASSERT( maps[regB] != maps[regH] );

            CPPUNIT_ASSERT( maps[regC] != maps[regD] );
            CPPUNIT_ASSERT( maps[regC] != maps[regE] );
            CPPUNIT_ASSERT( maps[regC] != maps[regF] );
            CPPUNIT_ASSERT( maps[regC] != maps[regG] );
            CPPUNIT_ASSERT( maps[regC] != maps[regH] );
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(RegisterAllocationTest::suite());
    runner.run();
    return 0;
}