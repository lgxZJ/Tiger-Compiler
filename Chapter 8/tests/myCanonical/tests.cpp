#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
//#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../myCanonical.h"

////////////////////////////////////////////

using namespace lgxZJ::Canonical;

class CanonicalTest: public CppUnit::TestFixture
{
    public:
        static CppUnit::Test* suite()
        {
            CppUnit::TestSuite* suiteOfTests = new CppUnit::TestSuite("CanonicalTest");

            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testLinearize_NonSeqStatement_CollectedInResult));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testLinearize_SeqStatement_NotCollectedInResult));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testLinearize_MixedSomeStatement_CollectNoSeqInResult));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testLinearize_NullStatement_NotCollectInResult));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testLinearize_ExpStatement_CollectedInResult));

            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testToBlocks_EmptyStatements_CollectOnlyEpilogueBlock));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testToBlocks_OneBlockStatementsWithNoBeginLabel_AddLabelAutomatically));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testToBlocks_TwoBlockStatementWithNoJumpAtEnd_AddEpilogueJumpAutomatically));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testLinearize", &CanonicalTest::testToBlocks_OneBlockstatementsEndWithJumpable_AddEpilogueBlock));
            
            return suiteOfTests;
        }

        /////////////////////////////////////////////////////////////

        void testLinearize_NonSeqStatement_CollectedInResult()
        {
            IR_myStatement nonSeqState = IR_makeLabel((myLabel)12);

            Statements result = Canonical::Linearize(nonSeqState);

            CPPUNIT_ASSERT_EQUAL(result.size(), (size_t)1);
            CPPUNIT_ASSERT_EQUAL(result.at(0), nonSeqState);
        }

        void testLinearize_SeqStatement_NotCollectedInResult()
        {
            IR_myStatement seqState = IR_makeSeq(IR_makeLabel((myLabel)12), nullptr);

            Statements result = Canonical::Linearize(seqState);

            CPPUNIT_ASSERT_EQUAL(result.size(), (size_t)1);
            CPPUNIT_ASSERT_EQUAL(result.at(0)->kind, IR_myStatement_::IR_Label);
        }

        void testLinearize_NullStatement_NotCollectInResult()
        {
            IR_myStatement nullState = IR_makeSeq(nullptr, nullptr);

            Statements result = Canonical::Linearize(nullState);

            CPPUNIT_ASSERT_EQUAL(result.size(), (size_t)0);
        }

        void testLinearize_MixedSomeStatement_CollectNoSeqInResult()
        {
            IR_myStatement mixedState = IR_makeSeq(
                IR_makeSeq(IR_makeLabel((myLabel)12), nullptr),
                IR_makeSeq(
                    IR_makeMove((IR_myExp)23, (IR_myExp)67),
                    IR_makeJump((IR_myExp)667, (myLabelList)56)));

            Statements result = Canonical::Linearize(mixedState);

            CPPUNIT_ASSERT_EQUAL(result.size(), (size_t)3);
            CPPUNIT_ASSERT_EQUAL(result.at(0)->kind, IR_myStatement_::IR_Label);
            CPPUNIT_ASSERT_EQUAL(result.at(1)->kind, IR_myStatement_::IR_Move);
            CPPUNIT_ASSERT_EQUAL(result.at(2)->kind, IR_myStatement_::IR_Jump);
        }

        void testLinearize_ExpStatement_CollectedInResult()
        {
            IR_myStatement expState = IR_makeExp(IR_makeMem((IR_myExp)12));

            Statements result = Canonical::Linearize(expState);

            CPPUNIT_ASSERT_EQUAL(result.size(), (size_t)1);
            CPPUNIT_ASSERT_EQUAL(result.at(0)->kind, IR_myStatement_::IR_Exp);
        }

        //////////////////////////////////////////////////////////////////////

        void testToBlocks_EmptyStatements_CollectOnlyEpilogueBlock()
        {
            Statements statements;

            Blocks blocks = Canonical::ToBlocks(statements);

            CPPUNIT_ASSERT_EQUAL(blocks.size(), (size_t)1);
            CPPUNIT_ASSERT_EQUAL(blocks.at(0).size(), (size_t)2);
            CPPUNIT_ASSERT_EQUAL(blocks.at(0).back()->kind, IR_myStatement_::IR_Jump);
        }

        void testToBlocks_OneBlockStatementsWithNoBeginLabel_AddLabelAutomatically()
        {
            Statements statements;
            statements.push_back(IR_makeExp((IR_myExp)12));

            Blocks blocks = Canonical::ToBlocks(statements);

            CPPUNIT_ASSERT_EQUAL(blocks.at(0).at(0)->kind, IR_myStatement_::IR_Label);
        }

        void testToBlocks_TwoBlockStatementWithNoJumpAtEnd_AddEpilogueJumpAutomatically()
        {
            Statements statements;
            statements.push_back(IR_makeExp((IR_myExp)12));
            statements.push_back(IR_makeJump((IR_myExp)121, Temp_makeLabelList(nullptr, nullptr)));
            statements.push_back(IR_makeExp((IR_myExp)45));

            Blocks blocks = Canonical::ToBlocks(statements);

            CPPUNIT_ASSERT_EQUAL(blocks.size(), (size_t)2);
            CPPUNIT_ASSERT_EQUAL(blocks.at(0).at(0)->kind, IR_myStatement_::IR_Label);
            CPPUNIT_ASSERT_EQUAL(blocks.at(1).at(0)->kind, IR_myStatement_::IR_Label);
        }

        void testToBlocks_TwoBlocksStatementsWithNoJumpAtFirstEnd_AddJumpAutomatically()
        {
            Statements statements;
            statements.push_back(IR_makeExp((IR_myExp)12));
            statements.push_back(IR_makeLabel(Temp_newLabel()));
            statements.push_back(IR_makeExp((IR_myExp)45));

            Blocks blocks = Canonical::ToBlocks(statements);

            CPPUNIT_ASSERT_EQUAL(blocks.size(), (size_t)2);
            CPPUNIT_ASSERT_EQUAL(blocks.at(0).back()->kind, IR_myStatement_::IR_Jump);
        }

        void testToBlocks_OneBlockstatementsEndWithJumpable_AddEpilogueBlock()
        {
            Statements statements;
            statements.push_back(
                IR_makeJump(IR_makeName(Temp_newLabel()), Temp_makeLabelList(nullptr, nullptr)));;

            Blocks blocks = Canonical::ToBlocks(statements);

            CPPUNIT_ASSERT_EQUAL(blocks.size(), (size_t)2);
            CPPUNIT_ASSERT_EQUAL(blocks.back().size(), (size_t)2);
            CPPUNIT_ASSERT_EQUAL(blocks.back().back()->kind, IR_myStatement_::IR_Jump);
        }

        void testToBlocks_TwoBlockStatementsOnlyEndsWithJumpable_CollectThreeBlocks()
        {
            Statements statements;
            statements.push_back(
                IR_makeJump(IR_makeName(Temp_newLabel()), Temp_makeLabelList(nullptr, nullptr)));;
            statements.push_back(
                IR_makeJump(IR_makeName(Temp_newLabel()), Temp_makeLabelList(nullptr, nullptr)));;
            
            Blocks blocks = Canonical::ToBlocks(statements);

            CPPUNIT_ASSERT_EQUAL(blocks.size(), (size_t)3);
            CPPUNIT_ASSERT_EQUAL(blocks.front().at(0)->kind, IR_myStatement_::IR_Label);
            CPPUNIT_ASSERT_EQUAL(blocks.front().at(1)->kind, IR_myStatement_::IR_Label);
            CPPUNIT_ASSERT_EQUAL(blocks.front().at(2)->kind, IR_myStatement_::IR_Label);
        }
};

//////////////////////////////////////////

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CanonicalTest::suite());
    runner.run();
    return 0;
}