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
    //CPPUNIT_TEST( testCtor_SecondIns_GetCorrectResult );
    CPPUNIT_TEST( testCtor_ThridIns_GetCorrectResult );
    CPPUNIT_TEST( testCtor_FourthIns_GetCorrectResult );
    CPPUNIT_TEST( testCtor_FifthIns_GetCorrectResult );
    CPPUNIT_TEST( testCtor_InsWithPrecoloredRegs_GetCorrectResult );
    CPPUNIT_TEST( testCtor_OnlyNeedCoalesce_DoCorrectCoalesce );
    CPPUNIT_TEST( testCtor_CoalesceButConstrained_DoCorrectCoalesce );
    CPPUNIT_TEST( testCtor_NeedFreeze_DocorrectFreeze );

    CPPUNIT_TEST( testCtor_SimplifyWithPrecolored_DocorrectSimplify );
    CPPUNIT_TEST( testCtor_CoalesceWithPrecolored_DoCorrectCoalesce );
    CPPUNIT_TEST( testCtor_CoalesceButConstrainedWithPrecolored_DoCorrectCoalesce );
    CPPUNIT_TEST( testCtor_FreezeWithPrecolored_DoCorrectFreeze );
    CPPUNIT_TEST( testCtor_MulAndDivIns_DoCorrectAllocation );

    CPPUNIT_TEST( testCtor_NeedSpill_DoCorrectSpill );
    CPPUNIT_TEST_SUITE_END();

    public:
        //  Interference graph should be:
        //      eax --- ecx
        //      ebx --- ecx
        //  The move pairs should be:
        //      ebx ---- eax
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

        //  Interference graph should be:
        //      eax --- ebx
        //      eax --- ecx
        //      eax --- edx
        //      ebx --- ecx
        //      ebx --- edx
        //  The move pairs should be:
        //      null
        Instructions makeSecondIns(myTemp regEAX, myTemp regEBX)
        {
            // myLabel label = Temp_newLabel();

            // return Instructions{
            //     make_shared<Move>(regEAX, Move::OperandType::Content, 0),
            //     make_shared<IMul>(2),
            //     make_shared<Jmp>(label),
            //     make_shared<Call>(Temp_newLabel(), nullptr),
            //     make_shared<IDiv>(regEBX),
            //     make_shared<Label>(label),
            // };
        }

        //  Interference graph should be:
        //      eax --- ecx
        //      edx --- ebx
        //  The move pairs should be:
        //      eax --- ebx
        Instructions makeThirdIns(myTemp regA, myTemp regB, myTemp regC, myTemp regD)
        {
            return Instructions{
                make_shared<Move>(regA, regB), 
                make_shared<Add>(regC, regA), 
                make_shared<Add>(regD, regB),
            };
        }

        //  Interference graph should be:
        //      ebx --- ecx
        //  The move pairs should be:
        //      null
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

        //  regA != EAX
        Instructions makeFifthIns(myTemp reg32, myTemp reg20, myTemp reg33, myTemp reg36, myTemp reg11,
                                myTemp reg37, myTemp reg34, myTemp reg35, myTemp reg38)
	    {
            return Instructions {
                make_shared<Move>(reg32, reg20, Move::OperandType::Memory, Move::OperandType::Content),
                make_shared<Move>(reg33, reg32, Move::OperandType::Content, Move::OperandType::Memory),
                make_shared<Move>(reg36, reg11, Move::OperandType::Content, Move::OperandType::Content),
                make_shared<Add>(reg36, -4),
                make_shared<Move>(reg37, reg36, Move::OperandType::Content, Move::OperandType::Memory),
                make_shared<Move>(reg34, Move::OperandType::Content, 1),
                make_shared<Move>(reg35, Move::OperandType::Content, 4),
                make_shared<Move>(Frame_EAX(), reg34, Move::OperandType::Content, Move::OperandType::Content),
                make_shared<IMul>(reg35),
                make_shared<Add>(reg37, Frame_EAX()),
                make_shared<Move>(reg38, reg37, Move::OperandType::Content, Move::OperandType::Memory)
            };
	    }

        Instructions makeInsWithPrecoloredRegs(myTemp regA, myTemp regB, myTemp regC)
        {
            return makeFourthIns(regA, regB, regC);
        }

        //  interference graph should be:
        //      empty
        //  The move pairs should be:
        //      regA -- regB
        //      regC -- regB
        //      regD -- regB
        //  four regs should be coalesced into one reg
        Instructions makeInsOnlyNeedCoalesce(myTemp regA, myTemp regB, myTemp regC, myTemp regD)
        {
            return Instructions{
            	 make_shared<Move>(regA, regB) ,
            	 make_shared<Move>(regC, regB) ,
            	 make_shared<Move>(regD, regB) ,
            };
        }

        //  interference graph should be:
        //      regA -- regB
        //  The move pairs should be:
        //      regA -- regB
        Instructions makeInsCoalesceButConstrained(myTemp regA, myTemp regB)
        {
            return Instructions{
            	 make_shared<Move>(regA, regB) ,
            	 make_shared<Add>(regA, regB) ,
                 make_shared<Add>(regB, regA) ,
            };
        }

        //  The interference graph should be:
        //      regA -- regB
        //      regA -- regC
        //      regA -- regD
        //      regA -- regE
        //      regA -- regF
        //      regA -- regG
        //      regA -- regH
        //      regB -- regC
        //      regB -- regD
        //      regB -- regE
        //      regB -- regF
        //      regB -- regG
        //      regB -- regH
        //      regC -- regD
        //      regC -- regE
        //      regC -- regF
        //      regC -- regG
        //      regC -- regH
        //  The move pairs should be:
        //      regA -- regB
        //      regA -- regC
        //      regA -- regD
        //      regA -- regE
        //      regA -- regF
        //      regA -- regG
        //      regA -- regH
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

        //  interference graph:
        //      regA -- regB
        //      regA -- regC
        //      regB -- regC
        //  move pairs:
        //      empty
        Instructions makeInsActualSpill(
            myTemp regA, myTemp regB, myTemp regC)
        {
            return Instructions{
            	 make_shared<Add>(regA, regB) ,
                 make_shared<Add>(regA, regC) ,

                 make_shared<Add>(regB, regA) ,
                 make_shared<Add>(regB, regC) ,

                 make_shared<Add>(regC, regA) ,
                 make_shared<Add>(regC, regB) ,
            };
        }

        //  interference graph should be:
        //      EAX --- regB
        //      EAX --- EBX
        //      EAX --- ECX
        //      EAX --- EDX
        //      EDX --- regB
        //      EDX --- EBX
        //      EDX --- ECX
        //  move pairs should be:
        //      null
        Instructions makeInsWithMulAndDiv(
            myTemp regA, myTemp regB, myTemp regEBX, myTemp regECX)
        {
            return Instructions{
                make_shared<IMul>(regA),
                make_shared<IMul>(regB),
                make_shared<IDiv>(regEBX),
                make_shared<IMul>(regECX),
            };
        }

        ///////////////////////////////////////////////////////////////////////
        //                              tests

        void testCtor_FirstIns_GetCorrectResult()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = Temp_newTemp();

            RegisterAllocation ra(
                makeFirstIns(regA, regB, regC), Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL ( maps[regA], maps[regB] );
            CPPUNIT_ASSERT ( maps[regB] != maps[regC] );
            CPPUNIT_ASSERT ( maps[regA] != maps[regC] );
        }

        void testCtor_SecondIns_GetCorrectResult()
        {
            // myTemp regEAX = Frame_EAX(), regEBX = Frame_EBX();

            // RegisterAllocation ra(
            //     makeSecondIns(regEAX, regEBX), Trans_outermostLevel(), 6);

            // RegisterMap maps = ra.GetRegisterMap();
            // CPPUNIT_ASSERT_EQUAL( 4, (int)maps.size() );
            // CPPUNIT_ASSERT( maps[regEAX] != maps[regEBX] );
            // CPPUNIT_ASSERT( maps[regEAX] != maps[Frame_ECX()] );
            // CPPUNIT_ASSERT( maps[regEAX] != maps[Frame_EDX()] );
            // CPPUNIT_ASSERT( maps[regEBX] != maps[Frame_ECX()] );
            // CPPUNIT_ASSERT( maps[regEBX] != maps[Frame_EDX()] );
        }

        void testCtor_ThridIns_GetCorrectResult()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp();
            myTemp regC = Temp_newTemp(), regD = Temp_newTemp();

            RegisterAllocation ra(
                makeThirdIns(regA, regB, regC, regD), Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regB] );
            CPPUNIT_ASSERT( maps[regA] != maps[regC] );
            CPPUNIT_ASSERT( maps[regB] != maps[regD] );
        }

        void testCtor_FourthIns_GetCorrectResult()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = Temp_newTemp();

            RegisterAllocation ra(
                makeFourthIns(regA, regB, regC), Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT( maps[regB] != maps[regC] );
        }

        void testCtor_FifthIns_GetCorrectResult()
        {
            myTemp reg32 = Temp_newTemp(), reg20 = Temp_newTemp(), reg33 = Temp_newTemp();
            myTemp reg36 = Temp_newTemp(), reg11 = Temp_newTemp(), reg37 = Temp_newTemp();
            myTemp reg34 = Temp_newTemp(), reg35 = Temp_newTemp(), reg38 = Temp_newTemp();

            RegisterAllocation ra(makeFifthIns(reg32, reg20, reg33, reg36, reg11, reg37, reg34, reg35, reg38),
                                  Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT( maps[reg37] != Frame_EAX() );
        }

        void testCtor_InsWithPrecoloredRegs_GetCorrectResult()
        {
            myTemp regEAX = Frame_EAX(), regEBX = Frame_EBX(), regECX = Frame_ECX();

            RegisterAllocation ra(
                makeInsWithPrecoloredRegs(regEAX, regEBX, regECX), Trans_outermostLevel(), 6);
            
            RegisterMap maps = ra.GetRegisterMap();
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
                makeInsOnlyNeedCoalesce(regA, regB, regC, regD), Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL( 4, (int)maps.size() );
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regB] );
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regC] );
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regD] );
        }

        void testCtor_CoalesceButConstrained_DoCorrectCoalesce()
        {
            myTemp  regA = Temp_newTemp(), regB = Temp_newTemp();

            RegisterAllocation ra(
                makeInsCoalesceButConstrained(regA, regB), Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
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
                Trans_outermostLevel(),
                6);
            
            RegisterMap maps = ra.GetRegisterMap();
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

        void testCtor_NeedSpill_DoCorrectSpill()
        {
            /*
            myTemp  regA = Temp_newTemp(), regB = Temp_newTemp(),
                    regC = Temp_newTemp(), regD = Temp_newTemp(),
                    regE = Temp_newTemp(), regF = Temp_newTemp(),
                    regG = Temp_newTemp(), regH = Temp_newTemp();

            RegisterAllocation ra(
                makeInsActualSpill(regA, regB, regC), Trans_outermostLevel(), 2);
            
            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL( 3, (int)maps.size() );*/
        }

        void testCtor_SimplifyWithPrecolored_DocorrectSimplify()
        {
            myTemp regA = Temp_newTemp(), regEBX = Frame_EBX(),
                    regECX = Frame_ECX();

            RegisterAllocation ra(
                makeFirstIns(regA, regEBX, regECX), Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL ( maps[regA], maps[regEBX] );
            CPPUNIT_ASSERT ( maps[regEBX] != maps[regECX] );
            CPPUNIT_ASSERT ( maps[regA] != maps[regECX] );
            CPPUNIT_ASSERT_EQUAL ( maps[regEBX],  regEBX );
            CPPUNIT_ASSERT_EQUAL ( maps[regECX],  regECX );
        }

        void testCtor_CoalesceWithPrecolored_DoCorrectCoalesce()
        {
            myTemp  regA = Temp_newTemp(), regB = Temp_newTemp(),
                    regECX = Frame_ECX(), regEDX = Frame_EDX();

            RegisterAllocation ra(
                makeInsOnlyNeedCoalesce(regA, regB, regECX, regEDX), Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL( 4, (int)maps.size() );
            CPPUNIT_ASSERT_EQUAL( maps[regA], maps[regB] );
            CPPUNIT_ASSERT_EQUAL( regECX, maps[regECX] );
            CPPUNIT_ASSERT_EQUAL( regEDX, maps[regEDX] );
        }

        void testCtor_CoalesceButConstrainedWithPrecolored_DoCorrectCoalesce()
        {
            myTemp  regA = Temp_newTemp(), regEBX = Frame_EBX();

            RegisterAllocation ra(
                makeInsCoalesceButConstrained(regA, regEBX), Trans_outermostLevel(), 6);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL( 2, (int)maps.size() );
            CPPUNIT_ASSERT( maps[regA] != maps[regEBX] );
            CPPUNIT_ASSERT_EQUAL( regEBX, maps[regEBX] );
        }

        void testCtor_FreezeWithPrecolored_DoCorrectFreeze()
        {
            myTemp  regEAX = Frame_EAX(), regEBX = Frame_EBX(),
                    regECX = Frame_ECX(), regEDX = Frame_EDX(),
                    regE = Temp_newTemp(), regF = Temp_newTemp(),
                    regG = Temp_newTemp(), regH = Temp_newTemp();

            RegisterAllocation ra(
                makeInsNeedFreeze(regEAX, regEBX, regECX, regEDX, regE, regF, regG, regH),
                Trans_outermostLevel(),
                6);
            
            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL( 8, (int)maps.size() );
            CPPUNIT_ASSERT_EQUAL( regEAX, maps[regEAX] );
            CPPUNIT_ASSERT( maps[regEAX] != maps[regECX] );
            CPPUNIT_ASSERT( maps[regEAX] != maps[regEDX] );            
            CPPUNIT_ASSERT( maps[regEAX] != maps[regE] );
            CPPUNIT_ASSERT( maps[regEAX] != maps[regF] );
            CPPUNIT_ASSERT( maps[regEAX] != maps[regG] );
            CPPUNIT_ASSERT( maps[regEAX] != maps[regH] );

            CPPUNIT_ASSERT_EQUAL( regEBX, maps[regEBX] );
            CPPUNIT_ASSERT( maps[regEBX] != maps[regECX] );
            CPPUNIT_ASSERT( maps[regEBX] != maps[regEDX] );
            CPPUNIT_ASSERT( maps[regEBX] != maps[regE] );
            CPPUNIT_ASSERT( maps[regEBX] != maps[regF] );
            CPPUNIT_ASSERT( maps[regEBX] != maps[regG] );
            CPPUNIT_ASSERT( maps[regEBX] != maps[regH] );

            CPPUNIT_ASSERT_EQUAL( regECX, maps[regECX] );
            CPPUNIT_ASSERT( maps[regECX] != maps[regEDX] );
            CPPUNIT_ASSERT( maps[regECX] != maps[regE] );
            CPPUNIT_ASSERT( maps[regECX] != maps[regF] );
            CPPUNIT_ASSERT( maps[regECX] != maps[regG] );
            CPPUNIT_ASSERT( maps[regECX] != maps[regH] );

            CPPUNIT_ASSERT_EQUAL( regEDX, maps[regEDX] );
        }

        void testCtor_MulAndDivIns_DoCorrectAllocation()
        {
            myTemp  regEBX = Frame_EBX(),  regECX = Frame_ECX(),
                    regA = Temp_newTemp(), regB = Temp_newTemp();

            RegisterAllocation ra(
                makeInsWithMulAndDiv(regA, regB, regEBX, regECX),
                Trans_outermostLevel(),
                4);

            RegisterMap maps = ra.GetRegisterMap();
            CPPUNIT_ASSERT_EQUAL( 6, (int)maps.size() );
            CPPUNIT_ASSERT( maps[Frame_EAX()] != maps[regB] );
            CPPUNIT_ASSERT( maps[Frame_EAX()] != maps[Frame_EBX()] );
            CPPUNIT_ASSERT( maps[Frame_EAX()] != maps[Frame_EDX()] );
            CPPUNIT_ASSERT( maps[Frame_EAX()] != maps[Frame_ECX()] );
            CPPUNIT_ASSERT( maps[Frame_EDX()] != maps[regB] );
            CPPUNIT_ASSERT( maps[Frame_EDX()] != maps[Frame_EBX()] );
            CPPUNIT_ASSERT( maps[Frame_EDX()] != maps[Frame_ECX()] );
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(RegisterAllocationTest::suite());
    runner.run();
    return 0;
}
