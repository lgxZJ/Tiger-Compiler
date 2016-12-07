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
                "testToBlocks", &CanonicalTest::testToBlocks_EmptyStatements_CollectOnlyEpilogueBlock));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testToBlocks", &CanonicalTest::testToBlocks_OneBlockStatementsWithNoBeginLabel_AddLabelAutomatically));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testToBlocks", &CanonicalTest::testToBlocks_TwoBlockStatementWithNoJumpAtEnd_AddEpilogueJumpAutomatically));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testToBlocks", &CanonicalTest::testToBlocks_OneBlockstatementsEndWithJumpable_AddEpilogueBlock));
                suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testToBlocks", &CanonicalTest::testToBlocks_NonFalseLabelCJumpBlocks_FillThatCJump));

            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testTrace", &CanonicalTest::testTrace_OneControlFlowBlocks_SequenceNotChange));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testTrace", &CanonicalTest::testTrace_OneTrueFollowedCJumpBlocks_FirstFollowedByFalseFlow));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testTrace", &CanonicalTest::testTrace_OneFalseFollowedCJumpBlocks_FirstFollowedByFalseFlow));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testTrace", &CanonicalTest::testTrace_JumpNotFollowedByTargetBlocks_JumpFollowedByTarget));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testTrace", &CanonicalTest::testTrace_JumpFollowedByTargetBlocks_JumpFollowedByTarget));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testTrace", &CanonicalTest::testFlatten_JumpFollowedByTargetBlocks_JumpRemoved));
            suiteOfTests->addTest(new CppUnit::TestCaller<CanonicalTest>(
                "testTrace", &CanonicalTest::testFlatten_ByDefault_StatementsOrderIsKept));
            
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

    void testToBlocks_NonFalseLabelCJumpBlocks_FillThatCJump()
    {
        myLabel trueLabel = Temp_newLabel();
        myLabel doneLabel = Temp_newLabel();
        Statements statements = makeNonFalseLabelCJumpStatements(trueLabel, doneLabel);        

        Blocks result = Canonical::ToBlocks(statements);

        CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());
        CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_Label, result.at(1).front()->kind);
        CPPUNIT_ASSERT_EQUAL(result.at(1).front()->u.label, result.at(0).back()->u.cjump.falseLabel);
    }

        /////////////////////////////////////////////////////////////////////////////////

        static Statements makeNonFalseLabelCJumpStatements(myLabel trueLabel, myLabel doneLabel)
        {
            Statements statements;
            statements.push_back(
            IR_makeCJump(IR_Equal, IR_makeConst(1), IR_makeConst(1), trueLabel, nullptr));

            //  uncomment lines below to see if ToBlocks() works correct if label is followed but
            //  no cjump false-label filled.
            //statements.push_back(IR_makeLabel(Temp_newLabel()));
            statements.push_back(IR_makeExp((IR_myExp)12));
            statements.push_back(IR_makeJump(IR_makeName(doneLabel), Temp_makeLabelList(doneLabel, nullptr)));

            statements.push_back(IR_makeLabel(trueLabel));
            statements.push_back(IR_makeLabel(doneLabel));
            return statements;
        }

        static Blocks makeOneControlFlowBlocks()
        {
            Statements statements;
            myLabel firstLabel = Temp_newLabel();
            statements.push_back(IR_makeJump(
                IR_makeName(firstLabel), Temp_makeLabelList(firstLabel, nullptr)));

            myLabel secondLabel = Temp_newLabel();
            statements.push_back(IR_makeLabel(firstLabel));
            statements.push_back(IR_makeJump(
                IR_makeName(secondLabel), Temp_makeLabelList(secondLabel, nullptr)));
            
            statements.push_back(IR_makeLabel(secondLabel));

            return Canonical::ToBlocks(statements);
        }

        Blocks makeOneTrueFollowedCJumpBlocks(
            myLabel trueLabel, myLabel falseLabel, myLabel doneLabel)
        {
            Statements statements;
            statements.push_back(
                IR_makeCJump(IR_Equal, IR_makeConst(1), IR_makeConst(1),trueLabel, falseLabel));
            
            statements.push_back(IR_makeLabel(trueLabel));
            statements.push_back(
                IR_makeJump(IR_makeName(doneLabel), Temp_makeLabelList(doneLabel, nullptr)));
            
            statements.push_back(IR_makeLabel(falseLabel));
            statements.push_back(
                IR_makeJump(IR_makeName(doneLabel), Temp_makeLabelList(doneLabel, nullptr)));
            
            statements.push_back(IR_makeLabel(doneLabel));
            return Canonical::ToBlocks(statements);
        }

        Blocks makeOneFalseFollowedCJumpBlocks(
            myLabel trueLabel, myLabel falseLabel, myLabel doneLabel)
        {
            return makeOneTrueFollowedCJumpBlocks(falseLabel, trueLabel, doneLabel);
        }

        Blocks makeJumpNotFollowedByTargetBlocks(myLabel jumpTarget, myLabel followedLabel)
        {
            myLabel doneLabel = Temp_newLabel();
            Statements statements;

            statements.push_back(
                IR_makeJump(IR_makeName(jumpTarget), Temp_makeLabelList(jumpTarget, nullptr)));
            
            statements.push_back(IR_makeLabel(followedLabel));
            statements.push_back(IR_makeExp((IR_myExp)12));
            statements.push_back(
                IR_makeJump(IR_makeName(doneLabel), Temp_makeLabelList(doneLabel, nullptr)));

            statements.push_back(IR_makeLabel(jumpTarget));
            statements.push_back(
                IR_makeJump(IR_makeName(doneLabel), Temp_makeLabelList(doneLabel, nullptr)));

            statements.push_back(IR_makeLabel(doneLabel));
            return Canonical::ToBlocks(statements);
        }

        Blocks makeJumpFollowedByTargetBlocks(myLabel jumpTarget)
        {
            Statements statements;

            statements.push_back(
                IR_makeJump(IR_makeName(jumpTarget), Temp_makeLabelList(jumpTarget, nullptr)));
            statements.push_back(IR_makeLabel(jumpTarget));
            return Canonical::ToBlocks(statements);
        }

        //////////////////////////

        void testTrace_OneControlFlowBlocks_SequenceNotChange()
        {
            Blocks blocks = makeOneControlFlowBlocks();

            Blocks result = Canonical::Trace(blocks);

            CPPUNIT_ASSERT_EQUAL(blocks.size(), result.size());
            for (int i = 0; i < blocks.size(); ++i)
                CPPUNIT_ASSERT_EQUAL(blocks.at(i).front(), result.at(i).front());
        }

        void testTrace_OneTrueFollowedCJumpBlocks_FirstFollowedByFalseFlow()
        {
            myLabel doneLabel = Temp_newLabel();
            myLabel trueLabel = Temp_newLabel();
            myLabel falseLabel = Temp_newLabel();
            Blocks blocks = makeOneTrueFollowedCJumpBlocks(trueLabel, falseLabel, doneLabel); 

            Blocks result = Canonical::Trace(blocks);

            CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_CJump, result.at(0).back()->kind);
            CPPUNIT_ASSERT_EQUAL(falseLabel, result.at(1).front()->u.label);
        }

        void testTrace_OneFalseFollowedCJumpBlocks_FirstFollowedByFalseFlow()
        {
            myLabel doneLabel = Temp_newLabel();
            myLabel trueLabel = Temp_newLabel();
            myLabel falseLabel = Temp_newLabel();
            Blocks blocks = makeOneFalseFollowedCJumpBlocks(falseLabel, trueLabel, doneLabel); 

            Blocks result = Canonical::Trace(blocks);

            CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_CJump, result.at(0).back()->kind);
            CPPUNIT_ASSERT_EQUAL(falseLabel, result.at(1).front()->u.label);
        }

        void testTrace_JumpNotFollowedByTargetBlocks_JumpFollowedByTarget()
        {
            myLabel jumpTarget = Temp_newLabel();
            myLabel followedLabel = Temp_newLabel();
            Blocks blocks = makeJumpNotFollowedByTargetBlocks(jumpTarget, followedLabel);

            Blocks result = Canonical::Trace(blocks);

            CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());
            CPPUNIT_ASSERT_EQUAL(result.at(1).front()->u.label, result.at(0).back()->u.jump.exp->u.name);
        }

        void testTrace_JumpFollowedByTargetBlocks_JumpFollowedByTarget()
        {
            myLabel jumpTarget = Temp_newLabel();
            Blocks blocks = makeJumpFollowedByTargetBlocks(jumpTarget);

            Blocks result = Canonical::Trace(blocks);

            CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
            CPPUNIT_ASSERT_EQUAL(result.at(1).front()->u.label, result.at(0).back()->u.jump.exp->u.name);
        }

        /////////////////////////////////////////////////////////////////////

        Statements makeOneStatements()
        {
            Statements statements;
            myLabel firstLabel = Temp_newLabel();
            myLabel secondLabel = Temp_newLabel();

            statements.push_back(IR_makeLabel(Temp_newLabel()));
            statements.push_back(
                IR_makeCJump(IR_Equal, IR_makeConst(1), IR_makeConst(1), secondLabel, nullptr));

            statements.push_back(IR_makeLabel(firstLabel));
            statements.push_back(IR_makeJump(IR_makeName(secondLabel), Temp_makeLabelList(secondLabel, nullptr)));

            statements.push_back(IR_makeLabel(secondLabel));
            return statements;
        }

        ////////////////////////////////

        void testFlatten_JumpFollowedByTargetBlocks_JumpRemoved()
        {
            myLabel jumpTarget = Temp_newLabel();
            Blocks blocks = Canonical::Trace(makeJumpFollowedByTargetBlocks(jumpTarget));

            Statements result = Canonical::Flatten(blocks);

            CPPUNIT_ASSERT_EQUAL((size_t)3, result.size());
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_Label, result.at(0)->kind);
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_Label, result.at(1)->kind);
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_Jump, result.at(2)->kind);
        }

        void testFlatten_ByDefault_StatementsOrderIsKept()
        {
            Statements statements = makeOneStatements();
            
            Statements result =
                Canonical::Flatten(Canonical::Trace(Canonical::ToBlocks(statements)));

            CPPUNIT_ASSERT_EQUAL((size_t)5, result.size());
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_Label, result.at(0)->kind);
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_CJump, result.at(1)->kind);
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_Label, result.at(2)->kind);
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_Label, result.at(3)->kind);
            CPPUNIT_ASSERT_EQUAL(IR_myStatement_::IR_Jump, result.at(4)->kind);
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