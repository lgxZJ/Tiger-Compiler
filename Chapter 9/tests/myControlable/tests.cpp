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
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(myControlableTest::suite());
    runner.run();
    return 0;
}