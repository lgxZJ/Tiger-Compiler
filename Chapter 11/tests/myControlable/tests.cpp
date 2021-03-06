#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../AAI/myControlable.h"

using namespace std;
using namespace lgxZJ::IS;

template <typename T>
class OneTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( OneTest<T> );
    CPPUNIT_TEST( testReplaceReg_ByDefault_DoNothing );
    CPPUNIT_TEST_SUITE_END();

    public:
        void testReplaceReg_ByDefault_DoNothing()
        {
            T oneObject(Temp_newLabel());

            oneObject.ReplaceReg(Temp_newTemp(), Temp_newTemp());

            CPPUNIT_ASSERT_EQUAL( 0, (int)oneObject.GetDstRegs().size() );
            CPPUNIT_ASSERT_EQUAL( 0, (int)oneObject.GetSrcRegs().size() );
        }
};

class CallMock : public Call
{
    public:
        CallMock(Call call) : Call(call)    {}
        myTempList GetTemps()   { return regList;  }
};

class CallTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( CallTest );
    CPPUNIT_TEST( testReplaceReg_ByDefault_ReplaceOldReg );
    CPPUNIT_TEST_SUITE_END();

    public:
        void testReplaceReg_ByDefault_ReplaceOldReg()
        {
            myTemp firstReg = Temp_newTemp(),
                    secondReg = Temp_newTemp(),
                    otherReg = Temp_newTemp();
            Call call(
                    Temp_newLabel(),
                    Temp_makeTempList(firstReg, Temp_makeTempList(secondReg, nullptr)));
            
            call.ReplaceReg(firstReg, otherReg);
            call.ReplaceReg(secondReg, otherReg);

            myTempList temps = CallMock(call).GetTemps();
            while (temps != nullptr)
            {
                CPPUNIT_ASSERT_EQUAL( temps->head, otherReg );
                temps = temps->tail;
            }
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(OneTest<Label>::suite());

    runner.addTest(OneTest<Jmp>::suite());
    runner.addTest(OneTest<Je>::suite());
    runner.addTest(OneTest<Jne>::suite());
    runner.addTest(OneTest<Jg>::suite());
    runner.addTest(OneTest<Jge>::suite());
    runner.addTest(OneTest<Jl>::suite());
    runner.addTest(OneTest<Jle>::suite());

    runner.addTest(CallTest::suite());
    runner.run();
    return 0;
}