#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../AAI/myComputable.h"

using namespace lgxZJ::IS;

template <typename T>
class TwoComputeMocker : public T
{
    public:
        explicit TwoComputeMocker(T t) : T(t)    {}

        myTemp GetDstReg()  { return T::dstReg; }
        myTemp GetSrcReg()  { return T::srcRep.u.reg; }
        int    GetSrcValue(){ return T::srcRep.u.value; }
        BinaryUnion::Kind GetSrcType() { return T::srcRep.kind; }
};

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
                "testCtor", &TwoComputeTest::testSubCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testSubCtor_LeftRegRightConstValue_SetWhatPassed));
            
            return suite;
        }

        ///////////////////////////////////////////////////////////////////////
        //                          tests template

        template <typename T>
        void testCtorTemplate_BothOperandsReg_SetWhatPassed()
        {
            myTemp dst = Temp_newTemp();
            myTemp src = Temp_newTemp();
            T add(dst, src);

            TwoComputeMocker<T> addMocker(add);
            CPPUNIT_ASSERT_EQUAL(dst, addMocker.GetDstReg());
            CPPUNIT_ASSERT_EQUAL(BinaryUnion::Kind::Reg, addMocker.GetSrcType());
            CPPUNIT_ASSERT_EQUAL(src, addMocker.GetSrcReg());
        }

        template <typename T>
        void testCtorTemplate_LeftRegRightConstValue_SetWhatPassed()
        {
            myTemp dst = Temp_newTemp();
            int srcValue = 1;
            T add(dst, srcValue);

            TwoComputeMocker<T> addMocker(add);
            CPPUNIT_ASSERT_EQUAL(dst, addMocker.GetDstReg());
            CPPUNIT_ASSERT_EQUAL(BinaryUnion::Kind::Value, addMocker.GetSrcType());
            CPPUNIT_ASSERT_EQUAL(srcValue, addMocker.GetSrcValue());
        }

        ///////////////////////////////////////////////////////////////////////
        //                              tests

        void testAddCtor_BothOperandsReg_SetWhatPassed()
        {
            testCtorTemplate_BothOperandsReg_SetWhatPassed<Add>();
        }

        void testAddCtor_LeftRegRightConstValue_SetWhatPassed()
        {
            testCtorTemplate_LeftRegRightConstValue_SetWhatPassed<Add>();
        }

        void testSubCtor_BothOperandsReg_SetWhatPassed()
        {
            testCtorTemplate_BothOperandsReg_SetWhatPassed<Sub>();
        }

        void testSubCtor_LeftRegRightConstValue_SetWhatPassed()
        {
            testCtorTemplate_LeftRegRightConstValue_SetWhatPassed<Sub>();
        }
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(TwoComputeTest::suite());
    runner.run();
    return 0;
}