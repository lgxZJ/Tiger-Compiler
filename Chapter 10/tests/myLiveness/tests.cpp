#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../../myLiveness.h"
#include "../../AAI/myMovable.h"
#include "../../AAI/myComputable.h"
#include "../../AAI/myControlable.h"

#include <memory>
#include <algorithm>

using namespace std;
using namespace lgxZJ::IS;
using namespace lgxZJ::LA;

class InterferenceGraphMock : public InterferenceGraph
{
    public:
        explicit InterferenceGraphMock(InterferenceGraph graph)
            : InterferenceGraph(graph) 
        {}

        DirectedGraph GetGraph() const { return graph; }
        Registers GetRegisters() const { return registers; }
};

class InterferenceGraphTest : public CppUnit::TestFixture
{
    public:
            CPPUNIT_TEST_SUITE(InterferenceGraphTest);

            CPPUNIT_TEST(testCtor_ByDefault_InitMembersToZeroSize);
            CPPUNIT_TEST(testCtor_PassSizeParam_InitNodesAndRegsToThisSize);
            CPPUNIT_TEST(testGetNodeReg_ByDefault_GetWhatSetted);

            CPPUNIT_TEST_SUITE_END();

        ////////////////////////////////////////////////////////////////////
        //                          tests

        void testCtor_ByDefault_InitMembersToZeroSize()
        {
            InterferenceGraph graph;

            InterferenceGraphMock mock(graph);
            CPPUNIT_ASSERT_EQUAL((size_t)0, graph.GetDGRef().GetNodes().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, graph.GetDGRef().GetEdges().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetRegisters().size());
        }

        void testCtor_PassSizeParam_InitNodesAndRegsToThisSize()
        {
            unsigned initSize = 9;
            InterferenceGraph graph(initSize);

            InterferenceGraphMock mock(graph);
            CPPUNIT_ASSERT_EQUAL((size_t)initSize, graph.GetDGRef().GetNodes().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, graph.GetDGRef().GetEdges().size());
            CPPUNIT_ASSERT_EQUAL((size_t)initSize, mock.GetRegisters().size());
        }

        void testGetNodeReg_ByDefault_GetWhatSetted()
        {
            InterferenceGraph graph(1);
            myTemp reg = Temp_newTemp();
            graph.SetNodeReg(0, reg);

            myTemp regGot = graph.GetNodeReg(0);

            CPPUNIT_ASSERT_EQUAL(reg, regGot);
        }        
};

class LivenessMock : public Liveness
{
    public:
        explicit LivenessMock(Liveness liveness) : Liveness(liveness) {}
        RegistersSet GetIn() const { return in; }
        RegistersSet GetOut() const { return out; }
};

class LivenessTest : public CppUnit::TestFixture
{
    public:
            CPPUNIT_TEST_SUITE(LivenessTest);

            CPPUNIT_TEST(testCtor_FirstCFGraph_CalculateIn);
            CPPUNIT_TEST(testCtor_FirstCFGraph_CalculateOut);
            //CPPUNIT_TEST(testGetNodeReg_ByDefault_GetWhatSetted);

            CPPUNIT_TEST_SUITE_END();

        

        //
        //  Make a control flow graph with following instructions:
        //      1.eax = 0
        //      2.label:
        //      3.eax = eax + 1
        //      4.ebx = eax
        //      5.ecx = ecx + ebx
        //      6.ebx = ebx + 2
        //      7.eax = ebx
        //      8.cmp eax, 12
        //      9.jl lebal
        //
        //  Results should be the following:
        //  |   line num    |    use/defs    |   out / in    |
        //  |       1       |       /eax     |eax,ecx/ ecx   | 
        //  |       2       |       /        |eax,ecx/eax,ecx|
        //  |       3       |    eax/eax     |eax,ecx/eax,ecx|
        //  |       4       |    eax/ebx     |ebx,ecx/eax,ecx|
        //  |       5       |ebx,ecx/ ebx    |ebx,ecx/ebx,ecx|
        //  |       6       |    ebx/ebx     |ebx,ecx/ebx,ecx|
        //  |       7       |    ebx/eax     |eax,ecx/ebx,ecx|
        //  |       8       |    eax/        |eax,ecx/eax,ecx|
        //  |       9       |       /        |eax,ecx/eax,ecx|
        //
        CFGraph makeFirstCFGraph(myTemp regA, myTemp regB, myTemp regC)
        {
            Instructions ins;
            myLabel label = Temp_newLabel();

            ins.push_back(make_shared<Move>(regA, Move::OperandType::Content, 0));
            ins.push_back(make_shared<Label>(label));
            ins.push_back(make_shared<Add>(regA, 1));
            ins.push_back(make_shared<Move>(regB, regA));
            ins.push_back(make_shared<Add>(regC, regB));
            ins.push_back(make_shared<Add>(regB, 2));
            ins.push_back(make_shared<Move>(regA, regB));
            ins.push_back(make_shared<Cmp>(regA, 12));
            ins.push_back(make_shared<Jl>(label));
            return CFGraph(ins);
        }

        bool registersContainsThis(const Registers& regs, myTemp reg)
        {
            return find(regs.begin(), regs.end(), reg) != regs.end();
        }

        void AssertOneRegisters_One(const Registers& regs, myTemp regOne)
        {
            CPPUNIT_ASSERT_EQUAL((size_t)1, regs.size());
            CPPUNIT_ASSERT(registersContainsThis(regs, regOne));
        }

        void AssertOneRegisters_Two(const Registers& regs, myTemp regOne, myTemp regTwo)
        {
            CPPUNIT_ASSERT_EQUAL((size_t)2, regs.size());
            CPPUNIT_ASSERT(registersContainsThis(regs, regOne));
            CPPUNIT_ASSERT(registersContainsThis(regs, regTwo));
        }

        ////////////////////////////////////////////////////////////////////
        //                          tests

        void testCtor_FirstCFGraph_CalculateIn()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = Temp_newTemp();
            Liveness liveness(makeFirstCFGraph(regA, regB, regC));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)9, mock.GetIn().size());

            AssertOneRegisters_One(mock.GetIn().at(0), regC);
            AssertOneRegisters_Two(mock.GetIn().at(1), regA, regC);
            AssertOneRegisters_Two(mock.GetIn().at(2), regA, regC);
            AssertOneRegisters_Two(mock.GetIn().at(3), regA, regC);
            AssertOneRegisters_Two(mock.GetIn().at(4), regB, regC);
            AssertOneRegisters_Two(mock.GetIn().at(5), regB, regC);
            AssertOneRegisters_Two(mock.GetIn().at(6), regB, regC);
            AssertOneRegisters_Two(mock.GetIn().at(7), regA, regC);
            AssertOneRegisters_Two(mock.GetIn().at(8), regA, regC);
        }

        void testCtor_FirstCFGraph_CalculateOut()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = Temp_newTemp();
            Liveness liveness(makeFirstCFGraph(regA, regB, regC));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)9, mock.GetIn().size());

            AssertOneRegisters_Two(mock.GetOut().at(0), regA, regC);
            AssertOneRegisters_Two(mock.GetOut().at(1), regA, regC);
            AssertOneRegisters_Two(mock.GetOut().at(2), regA, regC);
            AssertOneRegisters_Two(mock.GetOut().at(3), regB, regC);
            AssertOneRegisters_Two(mock.GetOut().at(4), regB, regC);
            AssertOneRegisters_Two(mock.GetOut().at(5), regB, regC);
            AssertOneRegisters_Two(mock.GetOut().at(6), regA, regC);
            AssertOneRegisters_Two(mock.GetOut().at(7), regA, regC);
            AssertOneRegisters_Two(mock.GetOut().at(8), regA, regC);
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(InterferenceGraphTest::suite());
    runner.addTest(LivenessTest::suite());
    runner.run();
    return 0;
}