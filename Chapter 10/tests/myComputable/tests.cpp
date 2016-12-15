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
                "testCtor", &TwoComputeTest::testAddGetDstReg_ByDefault_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testAddGetSrcReg_ConstSrc_ReturnNull));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testAddGetSrcReg_ReigsterSrc_ReturnRegister));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testAddGetLabel_ByDefault_ReturnNull));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubCtor_LeftRegRightConstValue_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubGetDstReg_ByDefault_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubGetSrcReg_ConstSrc_ReturnNull));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubGetSrcReg_ReigsterSrc_ReturnRegister));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubGetLabel_ByDefault_ReturnNull));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulCtor_RegSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulCtor_ValueSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulGetDstReg_ByDefault_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulGetSrcReg_ConstSrc_ReturnNull));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulGetSrcReg_ReigsterSrc_ReturnRegister));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulGetLabel_ByDefault_ReturnNull));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivCtor_RegSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivCtor_ValueSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivGetDstReg_ByDefault_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivGetSrcReg_ConstSrc_ReturnNull));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivGetSrcReg_ReigsterSrc_ReturnRegister));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivGetLabel_ByDefault_ReturnNull));       
                
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorCtor_LeftRegRightConstValue_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorGetDstReg_ByDefault_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorGetSrcReg_ConstSrc_ReturnNull));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorGetSrcReg_ReigsterSrc_ReturnRegister));
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

        void testAddGetDstReg_ByDefault_ReturnDstReg()
        {
            testTwoOperandGetDstReg_ByDefault_ReturnDstReg<Add>();
        }

        void testAddGetSrcReg_ConstSrc_ReturnNull()
        {
            testTwoOperandGetSrcReg_ConstSrc_ReturnNull<Add>();
        }

        void testAddGetSrcReg_ReigsterSrc_ReturnRegister()
        {
            testTwoOperandGetSrcReg_RegisterSrc_ReturnRegister<Add>();
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

        void testSubGetDstReg_ByDefault_ReturnDstReg()
        {
            testTwoOperandGetDstReg_ByDefault_ReturnDstReg<Sub>();
        }

        void testSubGetSrcReg_ConstSrc_ReturnNull()
        {
            testTwoOperandGetSrcReg_ConstSrc_ReturnNull<Sub>();
        }

        void testSubGetSrcReg_ReigsterSrc_ReturnRegister()
        {
            testTwoOperandGetSrcReg_RegisterSrc_ReturnRegister<Sub>();
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

        void testIMulGetDstReg_ByDefault_ReturnDstReg()
        {
            testOneOperandGetDstReg_ByDefault_ReturnDstReg<IMul>();
        }

        void testIMulGetSrcReg_ConstSrc_ReturnNull()
        {
            testOneOperandGetSrcReg_ConstSrc_ReturnNull<IMul>();
        }

        void testIMulGetSrcReg_ReigsterSrc_ReturnRegister()
        {
            testOneOperandGetSrcReg_RegisterSrc_ReturnRegister<IMul>();
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

        void testIDivGetDstReg_ByDefault_ReturnDstReg()
        {
            testOneOperandGetDstReg_ByDefault_ReturnDstReg<IDiv>();
        }

        void testIDivGetSrcReg_ConstSrc_ReturnNull()
        {
            testOneOperandGetSrcReg_ConstSrc_ReturnNull<IDiv>();
        }

        void testIDivGetSrcReg_ReigsterSrc_ReturnRegister()
        {
            testOneOperandGetSrcReg_RegisterSrc_ReturnRegister<IDiv>();
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

        void testXorGetDstReg_ByDefault_ReturnDstReg()
        {
            testTwoOperandGetDstReg_ByDefault_ReturnDstReg<Xor>();
        }

        void testXorGetSrcReg_ConstSrc_ReturnNull()
        {
            testTwoOperandGetSrcReg_ConstSrc_ReturnNull<Xor>();
        }

        void testXorGetSrcReg_ReigsterSrc_ReturnRegister()
        {
            testTwoOperandGetSrcReg_RegisterSrc_ReturnRegister<Xor>();
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