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
#include <cassert>

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
            CPPUNIT_TEST(testCtor_PassSizeParam_InitRegsToNull);
            CPPUNIT_TEST(testetNodeSize_ByDefault_GetSize);
            CPPUNIT_TEST(testGetNodeReg_ByDefault_GetWhatSetted);
            CPPUNIT_TEST(testAddEdge_ByDefault_AddOneEdge);
            CPPUNIT_TEST(testAddEdge_ReverseEdge_NotAdd);
            CPPUNIT_TEST(testAddEdge_SameEdge_NotAdd);

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

        void testCtor_PassSizeParam_InitRegsToNull()
        {
            unsigned initSize = 9;
            InterferenceGraph graph(initSize);

            InterferenceGraphMock mock(graph);
            for (int i = 0; i < initSize; ++i)
                CPPUNIT_ASSERT_EQUAL((myTemp)nullptr, mock.GetRegisters().at(i));
        }

        void testetNodeSize_ByDefault_GetSize()
        {
            int graphOneSize = 0;
            int graphTwoSize = 89;
            InterferenceGraph graphOne;
            InterferenceGraph graphTwo(graphTwoSize);

            CPPUNIT_ASSERT_EQUAL(graphOneSize, graphOne.GetNodeSize());
            CPPUNIT_ASSERT_EQUAL(graphTwoSize, graphTwo.GetNodeSize());
        }

        void testGetNodeReg_ByDefault_GetWhatSetted()
        {
            InterferenceGraph graph(1);
            myTemp reg = Temp_newTemp();
            graph.SetNodeReg(0, reg);

            myTemp regGot = graph.GetNodeReg(0);

            CPPUNIT_ASSERT_EQUAL(reg, regGot);
        }

        void testAddEdge_ByDefault_AddOneEdge()
        {
            InterferenceGraph graph(3);

            graph.AddEdge(0,1);
            graph.AddEdge(1,2);

            CPPUNIT_ASSERT_EQUAL((size_t)2, graph.GetDGRef().GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(0, graph.GetDGRef().GetEdges().at(0).GetFrom());
            CPPUNIT_ASSERT_EQUAL(1, graph.GetDGRef().GetEdges().at(0).GetTo());
            CPPUNIT_ASSERT_EQUAL(1, graph.GetDGRef().GetEdges().at(1).GetFrom());
            CPPUNIT_ASSERT_EQUAL(2, graph.GetDGRef().GetEdges().at(1).GetTo());
        }

        void testAddEdge_ReverseEdge_NotAdd()
        {
            InterferenceGraph graph(3);

            graph.AddEdge(0,1);
            graph.AddEdge(1,0);

            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetDGRef().GetEdges().size());
        }

        void testAddEdge_SameEdge_NotAdd()
        {
            InterferenceGraph graph(3);

            graph.AddEdge(0,1);
            graph.AddEdge(0,1);

            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetDGRef().GetEdges().size());
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
            CPPUNIT_TEST(testCtor_SecondCFGraph_CalculateIn);
            CPPUNIT_TEST(testCtor_SecondCFGraph_CalculateOut);
            CPPUNIT_TEST(testCtor_FirstCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_SecondCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_ThirdCFGraph_GenerateInterferenceGraph);

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
        //  Interference graph should be:
        //      eax --- ecx
        //      ebx --- ecx
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

        //
        //  Make a control flow graph with following instructions:
        //      1.eax = 0
        //      2.eax = eax * 2
        //      3.jmp label
        //      4.call malloc
        //      5.eax = eax / ebx
        //      6.label:
        //
        //  Liveness results should be the following:
        //  |   line num    |    use/defs    |   out / in    |
        //  |       1       |       /eax     |    eax/       | 
        //  |       2       |    eax/eax     |       /eax    |
        //  |       3       |       /        |       /       |
        //  |       4       |       /eax     |eax,ebx/ebx    |
        //  |       5       |eax,ebx/eax     |       /eax,ebx|
        //  |       6       |       /        |       /       |
        //
        //  Interference graph should be:
        //      eax --- ebx
        CFGraph makeSecondCFGraph(myTemp regA, myTemp regB)
        {
            Instructions ins;
            myLabel label = Temp_newLabel();

            ins.push_back(make_shared<Move>(regA, Move::OperandType::Content, 0));
            ins.push_back(make_shared<IMul>(2));
            ins.push_back(make_shared<Jmp>(label));
            ins.push_back(make_shared<Call>(Temp_newLabel(), nullptr));
            ins.push_back(make_shared<IDiv>(regB));
            ins.push_back(make_shared<Label>(label));
            return CFGraph(ins);
        }

        //
        //  Make a control flow graph with following instructions:
        //      1.eax = ebx
        //      2.ecx = ecx + eax
        //      3.edx = edx + ebx
        //
        //  Interference graph should be:
        //      eax --- ecx
        //      edx --- ebx
        CFGraph makeThirdCFGraph(myTemp regA, myTemp regB, myTemp regC, myTemp regD)
        {
            Instructions ins;

            ins.push_back( make_shared<Move>(regA, regB) );
            ins.push_back( make_shared<Add>(regC, regA) );
            ins.push_back( make_shared<Add>(regD, regB) );
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

        bool EdgesContains(const EdgeSet& edgeSet, myTemp lhs, myTemp rhs)
        {
            assert (lhs != nullptr && rhs != nullptr);

            Node lhsNode = Temp_getTempNum(lhs) - 1;
            Node rhsNode = Temp_getTempNum(rhs) - 1;

            for (auto edge : edgeSet)
            {
                int from = edge.GetFrom();
                int to = edge.GetTo();
                if ((edge.GetFrom() == lhsNode && edge.GetTo() == rhsNode)  ||
                    (edge.GetFrom() == rhsNode && edge.GetTo() == lhsNode))
                    return true;
            }
            return false;
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

        void testCtor_FirstCFGraph_GenerateInterferenceGraph()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = Temp_newTemp();
            Liveness liveness(makeFirstCFGraph(regA, regB, regC));

            const DirectedGraph graph = liveness.GetInterferenceGraph().GetDGRef();
            CPPUNIT_ASSERT_EQUAL((size_t)2, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, EdgesContains(graph.GetEdges(), regA, regC));
            CPPUNIT_ASSERT_EQUAL(true, EdgesContains(graph.GetEdges(), regB, regC));
        }

        void testCtor_SecondCFGraph_CalculateIn()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX();
            Liveness liveness(makeSecondCFGraph(regA, regB));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)6, mock.GetIn().size());

            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetIn().at(0).size());
            AssertOneRegisters_One(mock.GetIn().at(1), regA);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetIn().at(2).size());
            AssertOneRegisters_One(mock.GetIn().at(3), regB);
            AssertOneRegisters_Two(mock.GetIn().at(4), regA, regB);
            CPPUNIT_ASSERT_EQUAL((size_t)0,  mock.GetIn().at(5).size());
        }

        void testCtor_SecondCFGraph_CalculateOut()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX();
            Liveness liveness(makeSecondCFGraph(regA, regB));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)6, mock.GetOut().size());

            AssertOneRegisters_One(mock.GetOut().at(0), regA);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(1).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(2).size());
            AssertOneRegisters_Two(mock.GetOut().at(3), regA, regB);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(4).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0,  mock.GetOut().at(5).size());
        }

        void testCtor_SecondCFGraph_GenerateInterferenceGraph()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX();
            Liveness liveness(makeSecondCFGraph(regA, regB));

            const DirectedGraph graph = liveness.GetInterferenceGraph().GetDGRef();
            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(
                true,
                EdgesContains(graph.GetEdges(), regA, regB));
        }

        void testCtor_ThirdCFGraph_GenerateInterferenceGraph()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX(), regC = Frame_ECX(), regD = Frame_EDX();
            Liveness liveness(makeThirdCFGraph(regA, regB, regC, regD));

            const DirectedGraph graph = liveness.GetInterferenceGraph().GetDGRef();
            CPPUNIT_ASSERT_EQUAL((size_t)4, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, EdgesContains(graph.GetEdges(), regA, regC));
            CPPUNIT_ASSERT_EQUAL(true, EdgesContains(graph.GetEdges(), regA, regD));
            CPPUNIT_ASSERT_EQUAL(true, EdgesContains(graph.GetEdges(), regC, regB));
            CPPUNIT_ASSERT_EQUAL(true, EdgesContains(graph.GetEdges(), regC, regD));
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