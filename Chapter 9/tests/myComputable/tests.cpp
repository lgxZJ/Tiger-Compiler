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

template <typename T>
class OneComputeMocker : public T
{
    public:
        explicit OneComputeMocker(T t) : T(t)    {}

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

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulCtor_RegSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIMulCtor_ValueSource_SetWhatPassed));

            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivCtor_RegSource_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testIDivCtor_ValueSource_SetWhatPassed));
                
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorCtor_BothOperandsReg_SetWhatPassed));
            suite->addTest(new CppUnit::TestCaller<TwoComputeTest>(
                "testCtor", &TwoComputeTest::testXorCtor_LeftRegRightConstValue_SetWhatPassed));
                
            return suite;
        }

        ///////////////////////////////////////////////////////////////////////
        //                          tests template

        template <typename T>
        void testTwoOperandCtor_BothOperandsReg_SetWhatPassed()
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
        void testTwoOperandCtor_LeftRegRightConstValue_SetWhatPassed()
        {
            myTemp dst = Temp_newTemp();
            int srcValue = 1;
            T one(dst, srcValue);

            TwoComputeMocker<T> oneMocker(one);
            CPPUNIT_ASSERT_EQUAL(dst, oneMocker.GetDstReg());
            CPPUNIT_ASSERT_EQUAL(BinaryUnion::Kind::Value, oneMocker.GetSrcType());
            CPPUNIT_ASSERT_EQUAL(srcValue, oneMocker.GetSrcValue());
        }

        template <typename T>
        void testOneOperandCtor_RegSource_SetWhatPassed()
        {
            myTemp src = Temp_newTemp();
            T one(src);

            OneComputeMocker<T> oneMocker(one);
            CPPUNIT_ASSERT_EQUAL(BinaryUnion::Kind::Reg, oneMocker.GetSrcType());
            CPPUNIT_ASSERT_EQUAL(src, oneMocker.GetSrcReg());
        }

        template <typename T>
        void testOneOperandCtor_ValueSource_SetWhatPassed()
        {
            int srcValue;
            T one(srcValue);

            OneComputeMocker<T> oneMocker(one);
            CPPUNIT_ASSERT_EQUAL(BinaryUnion::Kind::Value, oneMocker.GetSrcType());
            CPPUNIT_ASSERT_EQUAL(srcValue, oneMocker.GetSrcValue());
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

        /////////////////////////////

        void testSubCtor_BothOperandsReg_SetWhatPassed()
        {
            testTwoOperandCtor_BothOperandsReg_SetWhatPassed<Sub>();
        }

        void testSubCtor_LeftRegRightConstValue_SetWhatPassed()
        {
            testTwoOperandCtor_LeftRegRightConstValue_SetWhatPassed<Sub>();
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

        ///////////////////////////////

        void testIDivCtor_RegSource_SetWhatPassed()
        {
            testOneOperandCtor_RegSource_SetWhatPassed<IMul>();
        }

        void testIDivCtor_ValueSource_SetWhatPassed()
        {
            testOneOperandCtor_ValueSource_SetWhatPassed<IMul>();
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
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(TwoComputeTest::suite());
    runner.run();
    return 0;
}