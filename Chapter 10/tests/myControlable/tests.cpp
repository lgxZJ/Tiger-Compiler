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

        myLabel GetDstLabel() const { return label; }
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
                "testCtor", &myControlableTest::testLabelGetDstRegsAndGetSrcRegs_ByDefault_BothReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetDstRegs_ByDefault_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetSrcRegs_ConstSrc_ReturnLeftReg));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetSrcRegs_RegSrc_ReturnLeftAndRightRegs));
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
                "testCtor", &myControlableTest::testCallGetDstReg_ByDefault_ReturnCallerSaveRegs));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCallGetSrcRegs_ByDefault_ReturnEmpty));
            
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testLabelGetDstLabel_ByDefault_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCmpGetDstLabel_ByDefault_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testCallGetDstLabel_ByDefault_ReturnEmpty));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJmpGetDstLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJeGetDstLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJneGetDstLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJgGetDstLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJgeGetDstLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJlGetDstLabel_ByDefault_ReturnDstLabel));
            suite->addTest(new CppUnit::TestCaller<myControlableTest>(
                "testCtor", &myControlableTest::testJleGetDstLabel_ByDefault_ReturnDstLabel));
            return suite;
        }

        ////////////////////////////////////////////////////////////////////
        //                          tests

        void testLabelCtor_ByDefault_SetWhatPassed()
        {
            myLabel label = Temp_newLabel();
            Label one(label);

            LabelFaker faker(one);
            CPPUNIT_ASSERT_EQUAL(label, faker.GetDstLabel());
        }

        void testLabelGetDstLabel_ByDefault_ReturnEmpty()
        {
            Label label(Temp_newLabel());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), label.GetDstLabel());
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

        void testCmpGetDstLabel_ByDefault_ReturnEmpty()
        {
            Cmp cmp(Temp_newTemp(), Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), cmp.GetDstLabel());
        }

        ///////////////////////////////

        void testCallGetDstLabel_ByDefault_ReturnEmpty()
        {
            Call call(Temp_newLabel(), nullptr);

            CPPUNIT_ASSERT_EQUAL(static_cast<myLabel>(nullptr), call.GetDstLabel());
        }

        ////////////////////////////////

        void testLabelGetDstRegsAndGetSrcRegs_ByDefault_BothReturnEmpty()
        {
            Label label(Temp_newLabel());

            CPPUNIT_ASSERT_EQUAL((size_t)0, label.GetDstRegs().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, label.GetSrcRegs().size());
        }

        ////////////////////////////////

        void testCmpGetDstRegs_ByDefault_ReturnEmpty()
        {
            myTemp dstReg = Temp_newTemp();
            Cmp cmp(dstReg, 1);

            Registers result = cmp.GetDstRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)0, result.size());
        }

        /////////////////////////////////

        void testCmpGetSrcRegs_ConstSrc_ReturnLeftReg()
        {
            myTemp dstReg = Temp_newTemp();
            Cmp cmp(dstReg, 1);

            Registers result = cmp.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
            CPPUNIT_ASSERT_EQUAL(dstReg, result.at(0));
        }

        //////////////////////////////////

        void testCmpGetSrcRegs_RegSrc_ReturnLeftAndRightRegs()
        {
            myTemp srcReg = Temp_newTemp();
            myTemp dstReg = Temp_newTemp();  
            Cmp cmp(dstReg, srcReg);

            Registers result = cmp.GetSrcRegs();

            CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
            CPPUNIT_ASSERT_EQUAL(dstReg, result.at(0));
            CPPUNIT_ASSERT_EQUAL(srcReg, result.at(1));
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

        void testCallGetDstReg_ByDefault_ReturnCallerSaveRegs()
        {
            Call call(Temp_newLabel(), Temp_makeTempList(Temp_newTemp(), nullptr));

            CPPUNIT_ASSERT_EQUAL((size_t)3, call.GetDstRegs().size());
            CPPUNIT_ASSERT_EQUAL(Frame_EAX(), call.GetDstRegs().at(0));
            CPPUNIT_ASSERT_EQUAL(Frame_ECX(), call.GetDstRegs().at(1));
            CPPUNIT_ASSERT_EQUAL(Frame_EDX(), call.GetDstRegs().at(2));
        }

        void testCallGetSrcRegs_ByDefault_ReturnEmpty()
        {
            Call call(Temp_newLabel(), Temp_makeTempList(Temp_newTemp(), nullptr));

            CPPUNIT_ASSERT_EQUAL((size_t)0, call.GetSrcRegs().size());
        }

        ////////////////////////////////
        ////////////////////////////////

        template <typename T>
        void testOneGetDstLabel_ByDefault_ReturnDstLabel()
        {
            myLabel label = Temp_newLabel();
            T one(label);

            CPPUNIT_ASSERT_EQUAL(label, one.GetDstLabel());
        }

        ////////////////

        void testJmpGetDstLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetDstLabel_ByDefault_ReturnDstLabel<Jmp>();
        }

        ////////////////

        void testJeGetDstLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetDstLabel_ByDefault_ReturnDstLabel<Je>();
        }

        ////////////////

        void testJneGetDstLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetDstLabel_ByDefault_ReturnDstLabel<Jne>();
        }

        ////////////////

        void testJgGetDstLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetDstLabel_ByDefault_ReturnDstLabel<Jg>();
        }

        ////////////////

        void testJgeGetDstLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetDstLabel_ByDefault_ReturnDstLabel<Jge>();
        }

        ////////////////

        void testJlGetDstLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetDstLabel_ByDefault_ReturnDstLabel<Jl>();
        }

        ////////////////

        void testJleGetDstLabel_ByDefault_ReturnDstLabel()
        {
            testOneGetDstLabel_ByDefault_ReturnDstLabel<Jle>();
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(myControlableTest::suite());
    runner.run();
    return 0;
}