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
                "testCtor", &TwoComputeTest::testAddGetDstRegs_ByDefault_ReturnDstRegs));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testAddGetSrcRegs_ConstSrc_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testAddGetSrcRegs_ReigsterSrc_ReturnRegisters));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testAddGetLabel_ByDefault_ReturnNull));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubCtor_LeftRegRightConstValue_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubGetDstRegs_ByDefault_ReturnDstRegs));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubGetSrcRegs_ConstSrc_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubGetSrcRegs_ReigsterSrc_ReturnRegisters));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubGetLabel_ByDefault_ReturnNull));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulCtor_RegSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulCtor_ValueSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulGetDstRegs_ByDefault_ReturnDstRegs));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulGetSrcRegs_ConstSrc_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulGetSrcRegs_ReigsterSrc_ReturnRegisters));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulGetLabel_ByDefault_ReturnNull));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivCtor_RegSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivCtor_ValueSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivGetDstRegs_ByDefault_ReturnDstRegs));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivGetSrcRegs_ConstSrc_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivGetSrcRegs_ReigsterSrc_ReturnRegisters));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivGetLabel_ByDefault_ReturnNull));       
                
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorCtor_LeftRegRightConstValue_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorGetDstRegs_ByDefault_ReturnDstRegs));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorGetSrcRegs_ConstSrc_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorGetSrcRegs_ReigsterSrc_ReturnRegisters));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorGetLabel_ByDefault_ReturnNull));
                
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

        void testAddGetDstRegs_ByDefault_ReturnDstRegs()
        {
            testTwoOperandGetDstRegs_ByDefault_ReturnDstRegs<Add>();
        }

        void testAddGetSrcRegs_ConstSrc_ReturnDstReg()
        {
            testTwoOperandGetSrcRegs_ConstSrc_ReturnDstReg<Add>();
        }

        void testAddGetSrcRegs_ReigsterSrc_ReturnRegisters()
        {
            testTwoOperandGetSrcRegs_RegisterSrc_ReturnRegisters<Add>();
        }

        void testAddGetLabel_ByDefault_ReturnNull()
        {
            Add add(Temp_newTemp(), Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), add.GetLabel());
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

        void testSubGetDstRegs_ByDefault_ReturnDstRegs()
        {
            testTwoOperandGetDstRegs_ByDefault_ReturnDstRegs<Sub>();
        }

        void testSubGetSrcRegs_ConstSrc_ReturnDstReg()
        {
            testTwoOperandGetSrcRegs_ConstSrc_ReturnDstReg<Sub>();
        }

        void testSubGetSrcRegs_ReigsterSrc_ReturnRegisters()
        {
            testTwoOperandGetSrcRegs_RegisterSrc_ReturnRegisters<Sub>();
        }

        void testSubGetLabel_ByDefault_ReturnNull()
        {
            Sub sub(Temp_newTemp(), Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), sub.GetLabel());
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

        void testIMulGetDstRegs_ByDefault_ReturnDstRegs()
        {
            testOneOperandGetDstRegs_ByDefault_ReturnDstRegs<IMul>();
        }

        void testIMulGetSrcRegs_ConstSrc_ReturnDstReg()
        {
            testOneOperandGetSrcRegs_ConstSrc_ReturnDstReg<IMul>();
        }

        void testIMulGetSrcRegs_ReigsterSrc_ReturnRegisters()
        {
            testOneOperandGetSrcRegs_RegisterSrc_ReturnRegisters<IMul>();
        }

        void testIMulGetLabel_ByDefault_ReturnNull()
        {
            IMul mul(Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), mul.GetLabel());
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

        void testIDivGetDstRegs_ByDefault_ReturnDstRegs()
        {
            testOneOperandGetDstRegs_ByDefault_ReturnDstRegs<IDiv>();
        }

        void testIDivGetSrcRegs_ConstSrc_ReturnDstReg()
        {
            testOneOperandGetSrcRegs_ConstSrc_ReturnDstReg<IDiv>();
        }

        void testIDivGetSrcRegs_ReigsterSrc_ReturnRegisters()
        {
            testOneOperandGetSrcRegs_RegisterSrc_ReturnRegisters<IDiv>();
        }

        void testIDivGetLabel_ByDefault_ReturnNull()
        {
            IDiv div(Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), div.GetLabel());
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

        void testXorGetDstRegs_ByDefault_ReturnDstRegs()
        {
            testTwoOperandGetDstRegs_ByDefault_ReturnDstRegs<Xor>();
        }

        void testXorGetSrcRegs_ConstSrc_ReturnDstReg()
        {
            testTwoOperandGetSrcRegs_ConstSrc_ReturnDstReg<Xor>();
        }

        void testXorGetSrcRegs_ReigsterSrc_ReturnRegisters()
        {
            testTwoOperandGetSrcRegs_RegisterSrc_ReturnRegisters<Xor>();
        }

        void testXorGetLabel_ByDefault_ReturnNull()
        {
            Xor xorr(Temp_newTemp(), Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), xorr.GetLabel());
        }
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(TwoComputeTest::suite());
    runner.run();
    return 0;
}