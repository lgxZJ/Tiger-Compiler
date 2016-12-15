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
                "testCtor", &myControlableTest::testLabelGetDstRegsAndGetSrcRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetDstRegs_ByDefault_ReturnDstRegs));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetSrcRegs_ConstSrc_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetSrcRegs_RegSrc_ReturnSrcRegs));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJmpGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJeGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJneGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJgGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJgeGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJlGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJleGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCallGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty));
            
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testLabelGetLabel_ByDefault_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetLabel_ByDefault_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCallGetLabel_ByDefault_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJmpGetLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJeGetLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJneGetLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJgGetLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJgeGetLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJlGetLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJleGetLabel_ByDefault_ReturnDstLabel));
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

        void testLabelGetLabel_ByDefault_ReturnEmpty()
        {
            Label label(Temp_newLabel());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), label.GetLabel());
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

        void testCmpGetLabel_ByDefault_ReturnEmpty()
        {
            Cmp cmp(Temp_newTemp(), Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), cmp.GetLabel());
        }

        ///////////////////////////////

        void testCallTrashedRegs_ByDefault_EAX()
        {
            Call call(Temp_newLabel(), nullptr);

            CPPUNIT_ASSERT_EQUAL(Frame_RV(), call.TrashedRegs()->head);
            CPPUNIT_ASSERT_EQUAL((myTempList)nullptr, call.TrashedRegs()->tail);
        }

        void testCallGetLabel_ByDefault_ReturnEmpty()
        {
            Call call(Temp_newLabel(), nullptr);

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), call.GetLabel());
        }

        ////////////////////////////////

        void testLabelGetDstRegsAndGetSrcRegs_ByDefault_BothReturnEmpty()
        {
            Label label(Temp_newLabel());

            CPPUNIT_ASSERT_EQUAL((size_t)0, label.GetDstRegs().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, label.GetSrcRegs().size());
        }

        ////////////////////////////////

        void testCmpGetDstRegs_ByDefault_ReturnDstRegs()
        {
            myTemp dstReg = Temp_newTemp();
            Cmp cmp(dstReg, 1);

            Registers result = cmp.GetDstRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(dstReg, result.at(0));
        }

        /////////////////////////////////

        void testCmpGetSrcRegs_ConstSrc_ReturnEmpty()
        {
            Cmp cmp(Temp_newTemp(), 1);

            Registers result = cmp.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)0, result.size());
        }

        //////////////////////////////////

        void testCmpGetSrcRegs_RegSrc_ReturnSrcRegs()
        {
            myTemp srcReg = Temp_newTemp();
            Cmp cmp(Temp_newTemp(), srcReg);

            Registers result = cmp.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(srcReg, result.at(0));
        }

        //////////////////////////////////

        #define TEST_JUMPABLE_GETSRCANDDST(CLSNAME)                                 \
                CLSNAME one(Temp_newLabel());                                       \
                                                                                    \
                CPPUNIT_ASSERT_EQUAL((size_t)0, one.GetDstRegs().size());           \
                CPPUNIT_ASSERT_EQUAL((size_t)0, one.GetSrcRegs().size());

        ///////////////

        void testJmpGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jmp);
        }

        ///////////////////////////////

        void testJeGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty()
        {
            TEST_JUMPABLE_GETSRCANDDST(Je);
        }

        ///////////////////////////////

        void testJneGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jne);
        }

        ///////////////////////////////

        void testJgGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jg);
        }

        ///////////////////////////////

        void testJgeGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jge);
        }

        ///////////////////////////////

        void testJlGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jl);
        }

        ///////////////////////////////

        void testJleGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty()
        {
            TEST_JUMPABLE_GETSRCANDDST(Jle);
        }

        //////////////////////////////

        void testCallGetSrcRegsAndGetDstRegs_ByDefault_BothReturnEmpty()
        {
            Call call(Temp_newLabel(), Temp_makeTempList(Temp_newTemp(), nullptr));

            CPPUNIT_ASSERT_EQUAL((size_t)0, call.GetDstRegs().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, call.GetSrcRegs().size());
        }

        ////////////////////////////////
        ////////////////////////////////

        template <typename T>
        void testOneGetLabel_ByDefault_ReturnDstLabel()
        {
            myLabel label = Temp_newLabel();
            T one(label);

            CPPUNIT_ASSERT_EQUAL(label, one.GetLabel());
        }

        ////////////////

        void testJmpGetLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetLabel_ByDefault_ReturnDstLabel<Jmp>();
        }

        ////////////////

        void testJeGetLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetLabel_ByDefault_ReturnDstLabel<Je>();
        }

        ////////////////

        void testJneGetLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetLabel_ByDefault_ReturnDstLabel<Jne>();
        }

        ////////////////

        void testJgGetLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetLabel_ByDefault_ReturnDstLabel<Jg>();
        }

        ////////////////

        void testJgeGetLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetLabel_ByDefault_ReturnDstLabel<Jge>();
        }

        ////////////////

        void testJlGetLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetLabel_ByDefault_ReturnDstLabel<Jl>();
        }

        ////////////////

        void testJleGetLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetLabel_ByDefault_ReturnDstLabel<Jle>();
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(myControlableTest::suite());
    runner.run();
    return 0;
}