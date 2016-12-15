#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../AAI/myControlable.h"
#include "../../CXX_myFrame.h"

#include "../TestFixture.h"

using namespace lgxZJ::IS;

//////////////////////////////////////////////////////////////////////////////
//                          Fakers

class LabelFaker : public Label
{
    public:
        explicit LabelFaker(Label label) : Label(label)  {}

        myLabel GetLabel() const { return label; }
};

class myControlableTest : public CppUnit::TestFixture
{
    public:
        /////////////////////////////////////////////////////////////////////
        //                          Suite Maker

        static CppUnit::Test* suite()
        {
            CppUnit::TestSuite* suite = new CppUnit::TestSuite("myControlableTest");

            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testLabelCtor_ByDefault_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpCtor_BothOperandsRegs_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpCtor_LeftRegRightValue_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCallTrashedRegs_ByDefault_EAX));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testLabelGetDstRegAndGetSrcReg_ByDefault_BothReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetDstReg_ByDefault_ReturnDstReg));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetSrcReg_ConstSrc_ReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetSrcReg_RegSrc_ReturnSrcReg));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJmpGetSrcRegAndGetDstReg_ByDefault_BothReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJeGetSrcRegAndGetDstReg_ByDefault_BothReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJneGetSrcRegAndGetDstReg_ByDefault_BothReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJgGetSrcRegAndGetDstReg_ByDefault_BothReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJgeGetSrcRegAndGetDstReg_ByDefault_BothReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJlGetSrcRegAndGetDstReg_ByDefault_BothReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJleGetSrcRegAndGetDstReg_ByDefault_BothReturnNull));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCallGetSrcRegAndGetDstReg_ByDefault_BothReturnNull));

            return suite;
        }

        ////////////////////////////////////////////////////////////////////
        //                          tests

        void testLabelCtor_ByDefault_SetWhatPassed()
        {
            myLabel label = Temp_newLabel();
            Label one(label);

            LabelFaker faker(one);
            CPPUNIT_ASSERT_EQUAL(label, faker.GetLabel());
        }

        ////////////////////////////////

        void testCmpCtor_BothOperandsRegs_SetWhatPassed()
        {
            testTwoOperandCtor_BothOperandsReg_SetWhatPassed<Cmp>();
        }

        void testCmpCtor_LeftRegRightValue_SetWhatPassed()
        {
            testTwoOperandCtor_LeftRegRightConstValue_SetWhatPassed<Cmp>();
        }

        ///////////////////////////////

        void testCallTrashedRegs_ByDefault_EAX()
        {
            Call call(Temp_newLabel(), nullptr);

            CPPUNIT_ASSERT_EQUAL(Frame_RV(), call.TrashedRegs()->head);
            CPPUNIT_ASSERT_EQUAL((myTempList)nullptr, call.TrashedRegs()->tail);
        }

        ////////////////////////////////

        void testLabelGetDstRegAndGetSrcReg_ByDefault_BothReturnNull()
        {
            Label label(Temp_newLabel());

            CPPUNIT_ASSERT_EQUAL(static_cast<myTemp>(nullptr), label.GetDstReg());
            CPPUNIT_ASSERT_EQUAL(static_cast<myTemp>(nullptr), label.GetSrcReg());
        }

        ////////////////////////////////

        void testCmpGetDstReg_ByDefault_ReturnDstReg()
        {
            myTemp dstReg = Temp_newTemp();
            Cmp cmp(dstReg, 1);

            myTemp result = cmp.GetDstReg();

            CPPUNIT_ASSERT_EQUAL(dstReg, result);
        }

        /////////////////////////////////

        void testCmpGetSrcReg_ConstSrc_ReturnNull()
        {
            Cmp cmp(Temp_newTemp(), 1);

            myTemp result = cmp.GetSrcReg();

            CPPUNIT_ASSERT_EQUAL(static_cast<myTemp>(nullptr), result);
        }

        //////////////////////////////////

        void testCmpGetSrcReg_RegSrc_ReturnSrcReg()
        {
            myTemp srcReg = Temp_newTemp();
            Cmp cmp(Temp_newTemp(), srcReg);

            myTemp result = cmp.GetSrcReg();

            CPPUNIT_ASSERT_EQUAL(srcReg, result);
        }

        //////////////////////////////////

        #define TEST_JUMPABLE_GETSRCANDDST(CLSNAME)                                 \
                CLSNAME one(Temp_newLabel());                                       \
                                                                                    \
                CPPUNIT_ASSERT_EQUAL(static_cast<myTemp>(nullptr), one.GetDstReg());\
                CPPUNIT_ASSERT_EQUAL(static_cast<myTemp>(nullptr), one.GetSrcReg());

        ///////////////

        void testJmpGetSrcRegAndGetDstReg_ByDefault_BothReturnNull()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jmp);
        }

        ///////////////////////////////

        void testJeGetSrcRegAndGetDstReg_ByDefault_BothReturnNull()
        {
            TEST_JUMPABLE_GETSRCANDDST(Je);
        }

        ///////////////////////////////

        void testJneGetSrcRegAndGetDstReg_ByDefault_BothReturnNull()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jne);
        }

        ///////////////////////////////

        void testJgGetSrcRegAndGetDstReg_ByDefault_BothReturnNull()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jg);
        }

        ///////////////////////////////

        void testJgeGetSrcRegAndGetDstReg_ByDefault_BothReturnNull()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jge);
        }

        ///////////////////////////////

        void testJlGetSrcRegAndGetDstReg_ByDefault_BothReturnNull()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jl);
        }

        ///////////////////////////////

        void testJleGetSrcRegAndGetDstReg_ByDefault_BothReturnNull()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jle);
        }

        //////////////////////////////

        void testCallGetSrcRegAndGetDstReg_ByDefault_BothReturnNull()
        {
            Call call(Temp_newLabel(), Temp_makeTempList(Temp_newTemp(), nullptr));

            CPPUNIT_ASSERT_EQUAL(static_cast<myTemp>(nullptr), call.GetDstReg());
            CPPUNIT_ASSERT_EQUAL(static_cast<myTemp>(nullptr), call.GetSrcReg());
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(myControlableTest::suite());
    runner.run();
    return 0;
}