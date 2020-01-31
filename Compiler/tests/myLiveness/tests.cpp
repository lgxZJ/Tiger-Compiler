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
	    CPPUNIT_TEST(testCtor_PassSizeParam_InitRegsToRegs);
            CPPUNIT_TEST(testetNodeSize_ByDefault_GetSize);
            CPPUNIT_TEST(testGetNodeReg_ByDefault_GetWhatSetted);
            CPPUNIT_TEST(testAddEdge_ByDefault_AddOneEdge);
            CPPUNIT_TEST(testAddEdge_ReverseEdge_NotAdd);
            CPPUNIT_TEST(testAddEdge_SameEdge_NotAdd);
            CPPUNIT_TEST(testAddEdgeRegisters_ByDefault_AlsoSetNode);
            CPPUNIT_TEST(testEdgesContains_PassNotContainedEdge_ReturnFalse);
            CPPUNIT_TEST(testEdgesContains_PassContainedEdge_ReturnTrue);

            CPPUNIT_TEST_SUITE_END();

        ////////////////////////////////////////////////////////////////////
        //                          tests

        void testCtor_ByDefault_InitMembersToZeroSize()
        {
            CFGraph cfGraph(Instructions{});
            InterferenceGraph graph(cfGraph);

            InterferenceGraphMock mock(graph);
            CPPUNIT_ASSERT_EQUAL((size_t)0, graph.GetDGRef().GetNodes().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, graph.GetDGRef().GetEdges().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetRegisters().size());
        }

        void testCtor_PassSizeParam_InitNodesAndRegsToThisSize()
        {
            unsigned initSize = 8;
            CFGraph cfGraph(Instructions{
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
            });
            InterferenceGraph graph(cfGraph);

            InterferenceGraphMock mock(graph);
            CPPUNIT_ASSERT_EQUAL((size_t)initSize, graph.GetDGRef().GetNodes().size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, graph.GetDGRef().GetEdges().size());
            CPPUNIT_ASSERT_EQUAL((size_t)initSize, mock.GetRegisters().size());
        }

        void testCtor_PassSizeParam_InitRegsToRegs()
        {
            unsigned initSize = 8;
            CFGraph cfGraph(Instructions{
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
            });
            InterferenceGraph graph(cfGraph);

            InterferenceGraphMock mock(graph);
            for (int i = 0; i < initSize; ++i)
                CPPUNIT_ASSERT_EQUAL(graph.GetNodeReg(i), mock.GetRegisters().at(i));
        }

        void testetNodeSize_ByDefault_GetSize()
        {
            unsigned initSize = 6;
            CFGraph cfGraph(Instructions{
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
            });
            InterferenceGraph graph(cfGraph);

            CPPUNIT_ASSERT_EQUAL(initSize, (unsigned)graph.GetNodeSize());
        }

        void testGetNodeReg_ByDefault_GetWhatSetted()
        {
            CFGraph cfGraph(Instructions{
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()), });
            InterferenceGraph graph(cfGraph);
            myTemp reg = Temp_newTemp();
            graph.SetNodeReg(0, reg);

            myTemp regGot = graph.GetNodeReg(0);

            CPPUNIT_ASSERT_EQUAL(reg, regGot);
        }

        void testAddEdge_ByDefault_AddOneEdge()
        {
            CFGraph cfGraph(Instructions{
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Move::OperandType::Content, 0),
            });
            InterferenceGraph graph(cfGraph);

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
            CFGraph cfGraph(Instructions{
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Move::OperandType::Content, 0),
            });
            InterferenceGraph graph(cfGraph);

            graph.AddEdge(0,1);
            graph.AddEdge(1,0);

            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetDGRef().GetEdges().size());
        }

        void testAddEdge_SameEdge_NotAdd()
        {
            CFGraph cfGraph(Instructions{
                make_shared<Move>(Temp_newTemp(), Temp_newTemp()),
                make_shared<Move>(Temp_newTemp(), Move::OperandType::Content, 0),
            });
            InterferenceGraph graph(cfGraph);

            graph.AddEdge(0,1);
            graph.AddEdge(0,1);

            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetDGRef().GetEdges().size());
        }

        void testAddEdgeRegisters_ByDefault_AlsoSetNode()
        {
            myTemp regOne = Temp_newTemp(), regTwo = Temp_newTemp();
            CFGraph cfGraph(Instructions{make_shared<Move>(regOne, regTwo)});
            InterferenceGraph graph(cfGraph);

            graph.AddEdge(regOne, regTwo);

            CPPUNIT_ASSERT_EQUAL(regOne, graph.GetNodeReg(graph.GetRegNode(regOne)));
            CPPUNIT_ASSERT_EQUAL(regTwo, graph.GetNodeReg(graph.GetRegNode(regTwo)));
        }

        void testEdgesContains_PassNotContainedEdge_ReturnFalse()
        {
            myTemp regOne = Temp_newTemp(), regTwo = Temp_newTemp(), regThree = Temp_newTemp();
            CFGraph cfGraph(Instructions{
                make_shared<Move>(regOne, regTwo),
                make_shared<Move>(regThree, Move::OperandType::Content, 0),
            });
            InterferenceGraph graph(cfGraph);

            graph.AddEdge(0,1);

            CPPUNIT_ASSERT_EQUAL(false, graph.EdgesContains(regOne, regThree));
        }

        void testEdgesContains_PassContainedEdge_ReturnTrue()
        {
            myTemp regOne = Temp_newTemp(), regTwo = Temp_newTemp();
            CFGraph cfGraph(Instructions{make_shared<Move>(regOne, regTwo)});
            InterferenceGraph graph(cfGraph);

            graph.AddEdge(regOne,regTwo);

            CPPUNIT_ASSERT_EQUAL(true, graph.EdgesContains(regOne, regTwo));
            CPPUNIT_ASSERT_EQUAL(true, graph.EdgesContains(regTwo, regOne));
        }
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

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
            CPPUNIT_TEST(testCtor_FirstCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_FirstCFGraph_GenerateMovePairs);

  	    //CPPUNIT_TEST(testCtor_SecondCFGraph_CalculateIn);
            // CPPUNIT_TEST(testCtor_SecondCFGraph_CalculateOut);
            // CPPUNIT_TEST(testCtor_SecondCFGraph_GenerateInterferenceGraph);
            // CPPUNIT_TEST(testCtor_SecondCFGraph_GenerateMovePairs);

            CPPUNIT_TEST(testCtor_ThirdCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_ThirdCFGraph_GenerateMovePairs);

            CPPUNIT_TEST(testCtor_FourthCFGraph_CalculateIn);
            CPPUNIT_TEST(testCtor_FourthCFGraph_CalculateOut);
            CPPUNIT_TEST(testCtor_FourthCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_FourthCFGraph_GenerateMovePairs);

            CPPUNIT_TEST(testCtor_FifthCFGraph_CalculateOut);
            CPPUNIT_TEST(testCtor_FifthCFGraph_CalculateIn);
            CPPUNIT_TEST(testCtor_FifthCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_FifthCFGraph_GenerateMovePairs);

            CPPUNIT_TEST(testCtor_SixthCGraph_CalculateOut);
            CPPUNIT_TEST(testCtor_SixthCFGraph_CalculateIn);
            CPPUNIT_TEST(testCtor_SixthCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_SixthCFGraph_GenerateMovePairs);

            CPPUNIT_TEST(testCtor_SeventhCGraph_CalculateOut);
            CPPUNIT_TEST(testCtor_SeventhCGraph_CalculateIn);
            CPPUNIT_TEST(testCtor_SeventhCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_SeventhCFGraph_GenerateMovePairs);

            CPPUNIT_TEST(testCtor_EighthCGraph_CalculateOut);
            CPPUNIT_TEST(testCtor_EighthCGraph_CalculateIn);
            CPPUNIT_TEST(testCtor_EighthCFGraph_GenerateInterferenceGraph);
            CPPUNIT_TEST(testCtor_EightCFGraph_GenerateMovePairs);

            CPPUNIT_TEST(testCtor_NinthCGraph_CalculateOut);
            CPPUNIT_TEST(testCtor_NinthCGraph_CalculateIn);
            CPPUNIT_TEST(testCtor_NinthCGraph_CalculateInterferenceGraph);
            CPPUNIT_TEST(testCtor_NinthCFGraph_GenerateMovePairs);

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
        //  The move pairs should be:
        //      ebx ---- eax
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
        //      eax --- ecx
        //      eax --- edx
        //      ebx --- ecx
        //      ebx --- edx
        //  The move pairs should be:
        //      null
        /*CFGraph makeSecondCFGraph(myTemp regA, myTemp regB)
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
	}*/

        //
        //  Make a control flow graph with following instructions:
        //      1.eax = ebx
        //      2.ecx = ecx + eax
        //      3.edx = edx + ebx
        //
        //  Interference graph should be:
        //      eax --- ecx
        //      edx --- ebx
        //  The move pairs should be:
        //      eax --- ebx
        CFGraph makeThirdCFGraph(myTemp regA, myTemp regB, myTemp regC, myTemp regD)
        {
            Instructions ins;

            ins.push_back( make_shared<Move>(regA, regB) );
            ins.push_back( make_shared<Add>(regC, regA) );
            ins.push_back( make_shared<Add>(regD, regB) );
            return CFGraph(ins);
        }

        //  Make a control flow graph with following instructions:
        //      1.  eax = 0
        //      2.label:
        //      3.  edx = 2
        //      4.  eax = eax * edx
        //      5.  call malloc
        //      6.  je label
        //      7.  ecx = eax
        //
        //  Liveness results should be the following:
        //  |   line num    |    use/defs    |   out / in    |
        //  |       1       |       /eax     |       /       | 
        //  |       2       |       /        |    eax/       |
        //  |       3       |       /edx     |eax,edx/eax,edx|
        //  |       4       |eax,edx/eax,edx |       /ebx    |
        //  |       5       |       /a,c,d   |    eax/       |
        //  |       6       |       /        |    eax/eax    |   
        //  |       7       |    eax/ ecx    |       /eax    |
        //
        //  Interference graph should be:
        //      eax --- ecx
        //      eax --- edx
        //  The move pairs should be:
        //      ecx --- eax
        CFGraph makeFourthCFGraph(myTemp eax, myTemp ecx, myTemp edx)
        {
            Instructions ins;
            myLabel label = Temp_newLabel();

            ins.push_back( make_shared<Move>(eax, Move::OperandType::Content, 0) );
            ins.push_back( make_shared<Label>(label) );
            ins.push_back( make_shared<Move>(edx, Move::OperandType::Content, 2) );
            ins.push_back( make_shared<IMul>(edx) );
            ins.push_back( make_shared<Call>(const_cast<myString>("func"), nullptr) );
            ins.push_back( make_shared<Je>(label) );
            ins.push_back( make_shared<Move>(ecx, eax) );
            return CFGraph(ins);
        }

        //
        //  Make a control flow graph with following instructions:
        //      1.[eax] = ebx
        //      2.ecx = ecx + eax
        //      3.[ecx] = [ebx]
        //      4.ecx = [ebx]
        //
        //  Liveness results should be the following:
        //  |   line num    |    use/defs    |   out / in    |
        //  |       1       |    ebx/        |       /ebx    | 
        //  |       2       |ebx,ecx/        |    ebx/ebx,ecx|
        //  |       3       |eax,ecx/ecx     |    b,c/a,b,c  |
        //  |       4       |eax,ebx/        |  a,b,c/a,b,c  |
        //
        //  Interference graph should be:
        //      ebx --- ecx
        //  The move pairs should be:
        //      null
        CFGraph makeFifthCFGraph(myTemp regA, myTemp regB, myTemp regC)
        {
            Instructions ins;

            ins.push_back( make_shared<Move>(regA, regB, Move::OperandType::Memory) );
            ins.push_back( make_shared<Add>(regC, regA) );
            ins.push_back( make_shared<Move>(
                regC, regB, Move::OperandType::Memory, Move::OperandType::Memory) );
            ins.push_back( make_shared<Move>(
                regC, regB, Move::OperandType::Content, Move::OperandType::Memory) );
            return CFGraph(ins);
        }

        CFGraph makeSixthCFGraph(myTemp reg32, myTemp reg20, myTemp reg33, myTemp reg36, myTemp reg11,
                                myTemp reg37, myTemp reg34, myTemp reg35, myTemp reg38)
        {
            return CFGraph(Instructions {
                make_shared<Move>(reg32, reg20, Move::OperandType::Memory, Move::OperandType::Content),
                make_shared<Move>(reg33, reg32, Move::OperandType::Content, Move::OperandType::Memory),
                make_shared<Move>(reg36, reg11, Move::OperandType::Content, Move::OperandType::Content),
                make_shared<Add>(reg36, -4),
                make_shared<Move>(reg37, reg36, Move::OperandType::Content, Move::OperandType::Memory),
                make_shared<Move>(reg34, Move::OperandType::Content, 1),
                make_shared<Move>(reg35, Move::OperandType::Content, 4),
                make_shared<Move>(Frame_EAX(), reg34, Move::OperandType::Content, Move::OperandType::Content),
                make_shared<IMul>(reg35),
                make_shared<Add>(reg37, Frame_EAX()),
                make_shared<Move>(reg38, reg37, Move::OperandType::Content, Move::OperandType::Memory)
            });
        }

        //
        //  Make a control flow graph with following instructions:
        //      1.regA = 1
        //      2.regB = 2
        //      3.call label(regA, regB)
        //
        //  Liveness results should be the following:
        //  |   line num    |    use/defs    |   out / in    |
        //  |       1       |       /regA    |   regA/       | 
        //  |       2       |       /regB    |regA,regB/regA |
        //  |       3       |regA,regB/eax,ecx,edx|  /regA,regB|
        //
        //  Interference graph should be:
        //      regA --- regB
        //  The move pairs should be:
        //      null
        CFGraph makeSeventhCFGraph(myTemp regA, myTemp regB)
        {
            return CFGraph(Instructions {
                make_shared<Move>(regA, Move::OperandType::Content, 1),
                make_shared<Move>(regB, Move::OperandType::Content, 2),
                make_shared<Call>(const_cast<myString>("func"),
                    Temp_makeTempList(regA, Temp_makeTempList(regB, NULL)))
            });
        }


        CFGraph makeEighthIns(
            myTemp reg142, myTemp reg144, myTemp reg143, myTemp reg145, myTemp reg146)
        {
            return CFGraph(Instructions {
                make_shared<Move>(reg142, Frame_EBP(), Move::OperandType::Content, Move::OperandType::Content),
                make_shared<Add>(reg142, -4),
                make_shared<Move>(reg144, reg142, Move::OperandType::Content, Move::OperandType::Memory),
                make_shared<Move>(reg143, Frame_EBP(), Move::OperandType::Content,Move::OperandType::Content),
                make_shared<Add>(reg143, -8),
                make_shared<Move>(reg145, reg143, Move::OperandType::Content,Move::OperandType::Memory),
                make_shared<Move>(reg146, reg144, Move::OperandType::Content,Move::OperandType::Content),
                make_shared<Add>(reg146, reg145),
            });
        }

        CFGraph makeNinthIns(
            myTemp reg131, myTemp reg133, myTemp reg134, myTemp reg137, myTemp reg135,
            myTemp reg138, myTemp reg139, myTemp reg140, myTemp reg141,
            myTemp reg142, myTemp reg144, myTemp reg143, myTemp reg145, myTemp reg146)
        {
            return CFGraph(Instructions {
                make_shared<Move>(reg133, Frame_EBP(), Move::OperandType::Content, Move::OperandType::Content),
                make_shared<Add>(reg133, -4),
                make_shared<Move>(reg131, Move::OperandType::Content, 1),
                make_shared<Move>(reg133, reg131, Move::OperandType::Memory, Move::OperandType::Content),
                make_shared<Move>(reg134, reg133, Move::OperandType::Content, Move::OperandType::Memory),
                make_shared<Move>(reg137, Frame_EBP(), Move::OperandType::Content, Move::OperandType::Content),
                make_shared<Add>(reg137, -8),
                make_shared<Move>(reg135, Move::OperandType::Content, 2),
                make_shared<Move>(reg137, reg135, Move::OperandType::Memory, Move::OperandType::Content),
                make_shared<Move>(reg138, reg137, Move::OperandType::Content, Move::OperandType::Memory),
                make_shared<Move>(reg139, Move::OperandType::Content, 1),
                make_shared<Move>(reg140, Move::OperandType::Content, 3),
                make_shared<Move>(reg141, reg139, Move::OperandType::Content, Move::OperandType::Content),
                make_shared<Add>(reg141, reg140),

                make_shared<Move>(reg142, Frame_EBP(), Move::OperandType::Content, Move::OperandType::Content),
                make_shared<Add>(reg142, -4),
                make_shared<Move>(reg144, reg142, Move::OperandType::Content, Move::OperandType::Memory),
                make_shared<Move>(reg143, Frame_EBP(), Move::OperandType::Content,Move::OperandType::Content),
                make_shared<Add>(reg143, -8),
                make_shared<Move>(reg145, reg143, Move::OperandType::Content,Move::OperandType::Memory),
                make_shared<Move>(reg146, reg144, Move::OperandType::Content,Move::OperandType::Content),
                make_shared<Add>(reg146, reg145),
            });
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

        void AssertOneRegisters_Three(const Registers& regs, myTemp regOne, myTemp regTwo, myTemp regThree)
        {
            CPPUNIT_ASSERT_EQUAL((size_t)3, regs.size());
            CPPUNIT_ASSERT(registersContainsThis(regs, regOne));
            CPPUNIT_ASSERT(registersContainsThis(regs, regTwo));
            CPPUNIT_ASSERT(registersContainsThis(regs, regThree));
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
            InterferenceGraph ifGraph = 
                Liveness(makeFirstCFGraph(regA, regB, regC)).GetInterferenceGraph();

            const DirectedGraph graph = ifGraph.GetDGRef();
            CPPUNIT_ASSERT_EQUAL((size_t)2, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regA, regC));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regB, regC));
        }

        void testCtor_FirstCFGraph_GenerateMovePairs()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = Temp_newTemp();
            Liveness liveness = Liveness(makeFirstCFGraph(regA, regB, regC));

            MovePairs movePairs = liveness.GetMovePairs();
            InterferenceGraph ifGraph = liveness.GetInterferenceGraph();
            CPPUNIT_ASSERT_EQUAL((size_t)1, movePairs.size());
            CPPUNIT_ASSERT_EQUAL(ifGraph.GetRegNode(regB), movePairs.at(0).GetFrom());
            CPPUNIT_ASSERT_EQUAL(ifGraph.GetRegNode(regA), movePairs.at(0).GetTo());
        }

        ///////////////////////////////////////////

        /*void testCtor_SecondCFGraph_CalculateIn()
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
	}*/

        // void testCtor_SecondCFGraph_CalculateOut()
        // {
        //     myTemp regA = Frame_EAX(), regB = Frame_EBX();
        //     Liveness liveness(makeSecondCFGraph(regA, regB));

        //     LivenessMock mock(liveness);
        //     CPPUNIT_ASSERT_EQUAL((size_t)6, mock.GetOut().size());

        //     AssertOneRegisters_One(mock.GetOut().at(0), regA);
        //     CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(1).size());
        //     CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(2).size());
        //     AssertOneRegisters_Two(mock.GetOut().at(3), regA, regB);
        //     CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(4).size());
        //     CPPUNIT_ASSERT_EQUAL((size_t)0,  mock.GetOut().at(5).size());
        // }

        // void testCtor_SecondCFGraph_GenerateInterferenceGraph()
        // {
        //     myTemp regA = Frame_EAX(), regB = Frame_EBX();
        //     InterferenceGraph ifGraph =
        //         Liveness(makeSecondCFGraph(regA, regB)).GetInterferenceGraph();

        //     const DirectedGraph graph = ifGraph.GetDGRef();
        //     CPPUNIT_ASSERT_EQUAL((size_t)5, graph.GetEdges().size());
        //     CPPUNIT_ASSERT_EQUAL( true, ifGraph.EdgesContains(regA, regB) );
        //     CPPUNIT_ASSERT_EQUAL( true, ifGraph.EdgesContains(regA, Frame_ECX()) );
        //     CPPUNIT_ASSERT_EQUAL( true, ifGraph.EdgesContains(regB, Frame_ECX()) );
        //     CPPUNIT_ASSERT_EQUAL( true, ifGraph.EdgesContains(regA, Frame_EDX()) );
        //     CPPUNIT_ASSERT_EQUAL( true, ifGraph.EdgesContains(regB, Frame_EDX()) );
        // }

        // void testCtor_SecondCFGraph_GenerateMovePairs()
        // {
        //     myTemp regA = Frame_EAX(), regB = Frame_EBX();
        //     Liveness liveness = Liveness(makeSecondCFGraph(regA, regB));

        //     MovePairs movePairs = liveness.GetMovePairs();
        //     CPPUNIT_ASSERT_EQUAL((size_t)0, movePairs.size());
        // }

        /////////////////////////////////////////////////////

        void testCtor_ThirdCFGraph_GenerateInterferenceGraph()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX(), regC = Frame_ECX(), regD = Frame_EDX();
            InterferenceGraph ifGraph =
                Liveness(makeThirdCFGraph(regA, regB, regC, regD)).GetInterferenceGraph();

            const DirectedGraph graph = ifGraph.GetDGRef();
            CPPUNIT_ASSERT_EQUAL((size_t)4, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regA, regC));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regA, regD));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regC, regB));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regC, regD));
        }

        void testCtor_ThirdCFGraph_GenerateMovePairs()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX(), regC = Frame_ECX(), regD = Frame_EDX();
            Liveness liveness = Liveness(makeThirdCFGraph(regA, regB, regC, regD));

            MovePairs movePairs = liveness.GetMovePairs();
            InterferenceGraph ifGraph = liveness.GetInterferenceGraph();
            CPPUNIT_ASSERT_EQUAL((size_t)1, movePairs.size());
            CPPUNIT_ASSERT_EQUAL(ifGraph.GetRegNode(regA), movePairs.at(0).GetFrom());
            CPPUNIT_ASSERT_EQUAL(ifGraph.GetRegNode(regB), movePairs.at(0).GetTo());
        }

        /////////////////////////////////////////////////////

        void testCtor_FourthCFGraph_CalculateIn()
        {
            myTemp regA = Frame_EAX(), regC = Frame_ECX(), regD = Frame_EDX();
            Liveness liveness(makeFourthCFGraph(regA, regC, regD));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)7, mock.GetIn().size());

            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetIn().at(0).size());
            CPPUNIT_ASSERT_EQUAL((size_t)1, mock.GetIn().at(1).size());
            AssertOneRegisters_One(mock.GetIn().at(2), regA);
            AssertOneRegisters_Two(mock.GetIn().at(3), regA, regD);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetIn().at(4).size());
            AssertOneRegisters_One(mock.GetIn().at(5), regA);
            AssertOneRegisters_One(mock.GetIn().at(6), regA);
        }

        void testCtor_FourthCFGraph_CalculateOut()
        {
            myTemp regA = Frame_EAX(), regC = Frame_ECX(), regD = Frame_EDX();
            Liveness liveness(makeFourthCFGraph(regA, regC, regD));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)7, mock.GetOut().size());

            AssertOneRegisters_One(mock.GetOut().at(0), regA);
            AssertOneRegisters_One(mock.GetOut().at(1), regA);
            AssertOneRegisters_Two(mock.GetOut().at(2), regA, regD);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(3).size());
            AssertOneRegisters_One(mock.GetOut().at(4), regA);
            AssertOneRegisters_One(mock.GetOut().at(5), regA);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(6).size());
        }

        void testCtor_FourthCFGraph_GenerateInterferenceGraph()
        {
            myTemp regA = Frame_EAX(), regC = Frame_ECX(), regD = Frame_EDX();
            InterferenceGraph ifGraph =
                Liveness(makeFourthCFGraph(regA, regC, regD)).GetInterferenceGraph();

            const DirectedGraph graph = ifGraph.GetDGRef();
            CPPUNIT_ASSERT_EQUAL((size_t)2, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regA, regC));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regA, regD));
        }

        void testCtor_FourthCFGraph_GenerateMovePairs()
        {
            myTemp regA = Frame_EAX(), regC = Frame_ECX(), regD = Frame_EDX();
            Liveness liveness = Liveness(makeFourthCFGraph(regA, regC, regD));

            MovePairs movePairs = liveness.GetMovePairs();
            InterferenceGraph ifGraph = liveness.GetInterferenceGraph();
            CPPUNIT_ASSERT_EQUAL((size_t)1, movePairs.size());
            CPPUNIT_ASSERT_EQUAL(ifGraph.GetRegNode(regC), movePairs.at(0).GetFrom());
            CPPUNIT_ASSERT_EQUAL(ifGraph.GetRegNode(regA), movePairs.at(0).GetTo());
        }

        ////////////////////////////////////////////////////////////////

        void testCtor_FifthCFGraph_CalculateOut()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX(), regC = Frame_ECX();
            Liveness liveness(makeFifthCFGraph(regA, regB, regC));

            LivenessMock mock(liveness);

            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(3).size());
            AssertOneRegisters_One(mock.GetOut().at(2), regB);
            AssertOneRegisters_Two(mock.GetOut().at(1), regB, regC);
            AssertOneRegisters_Three(mock.GetOut().at(0), regA, regB, regC);
        }

        void testCtor_FifthCFGraph_CalculateIn()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX(), regC = Frame_ECX();
            Liveness liveness(makeFifthCFGraph(regA, regB, regC));

            LivenessMock mock(liveness);

            AssertOneRegisters_One(mock.GetIn().at(3), regB);
            AssertOneRegisters_Two(mock.GetIn().at(2), regB, regC);
            AssertOneRegisters_Three(mock.GetIn().at(1), regA, regB, regC);
            AssertOneRegisters_Three(mock.GetIn().at(0), regA, regB, regC);
        }

        void testCtor_FifthCFGraph_GenerateInterferenceGraph()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX(), regC = Frame_ECX();
            InterferenceGraph ifGraph =
                Liveness(makeFifthCFGraph(regA, regB, regC)).GetInterferenceGraph();

            const DirectedGraph graph = ifGraph.GetDGRef();

            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regB, regC));
        }

        void testCtor_FifthCFGraph_GenerateMovePairs()
        {
            myTemp regA = Frame_EAX(), regB = Frame_EBX(), regC = Frame_ECX();
            Liveness liveness(makeFifthCFGraph(regA, regB, regC));

            CPPUNIT_ASSERT_EQUAL(true, liveness.GetMovePairs().empty());
        }

        /////////////////////////////////////////////////////////////////

        void testCtor_SixthCGraph_CalculateOut()
        {
            myTemp reg32 = Temp_newTemp(), reg20 = Temp_newTemp(), reg33 = Temp_newTemp();
            myTemp reg36 = Temp_newTemp(), reg11 = Temp_newTemp(), reg37 = Temp_newTemp();
            myTemp reg34 = Temp_newTemp(), reg35 = Temp_newTemp(), reg38 = Temp_newTemp();

            Liveness liveness(makeSixthCFGraph(reg32, reg20, reg33, reg36, reg11, reg37, reg34, reg35, reg38));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(10).size());
            AssertOneRegisters_One(mock.GetOut().at(9), reg37);
            AssertOneRegisters_Two(mock.GetOut().at(8), reg37, Frame_EAX());
            AssertOneRegisters_Three(mock.GetOut().at(7), Frame_EAX(), reg35, reg37);
            AssertOneRegisters_Three(mock.GetOut().at(6), reg34, reg35, reg37);
            AssertOneRegisters_Two(mock.GetOut().at(5), reg34, reg37);
            AssertOneRegisters_One(mock.GetOut().at(4), reg37);
            AssertOneRegisters_One(mock.GetOut().at(3), reg36);
            AssertOneRegisters_One(mock.GetOut().at(2), reg36);
            AssertOneRegisters_One(mock.GetOut().at(1), reg11);
            AssertOneRegisters_Two(mock.GetOut().at(0), reg32, reg11);
        }

        void testCtor_SixthCFGraph_CalculateIn()
        {
            myTemp reg32 = Temp_newTemp(), reg20 = Temp_newTemp(), reg33 = Temp_newTemp();
            myTemp reg36 = Temp_newTemp(), reg11 = Temp_newTemp(), reg37 = Temp_newTemp();
            myTemp reg34 = Temp_newTemp(), reg35 = Temp_newTemp(), reg38 = Temp_newTemp();

            Liveness liveness(makeSixthCFGraph(reg32, reg20, reg33, reg36, reg11, reg37, reg34, reg35, reg38));

            LivenessMock mock(liveness);
            AssertOneRegisters_One(mock.GetIn().at(10), reg37);
            AssertOneRegisters_Two(mock.GetIn().at(9), reg37, Frame_EAX());
            AssertOneRegisters_Three(mock.GetIn().at(8), reg37, reg35, Frame_EAX());
            AssertOneRegisters_Three(mock.GetIn().at(7), reg34, reg35, reg37);
            AssertOneRegisters_Two(mock.GetIn().at(6), reg34, reg37);
            AssertOneRegisters_One(mock.GetIn().at(5), reg37);
            AssertOneRegisters_One(mock.GetIn().at(4), reg36);
            AssertOneRegisters_One(mock.GetIn().at(3), reg36);
            AssertOneRegisters_One(mock.GetIn().at(2), reg11);
            AssertOneRegisters_Two(mock.GetIn().at(1), reg32, reg11);
            AssertOneRegisters_Three(mock.GetIn().at(0), reg32, reg20, reg11);
        }

        void testCtor_SixthCFGraph_GenerateInterferenceGraph()
        {
            myTemp reg32 = Temp_newTemp(), reg20 = Temp_newTemp(), reg33 = Temp_newTemp();
            myTemp reg36 = Temp_newTemp(), reg11 = Temp_newTemp(), reg37 = Temp_newTemp();
            myTemp reg34 = Temp_newTemp(), reg35 = Temp_newTemp(), reg38 = Temp_newTemp();

            InterferenceGraph ifGraph =
                Liveness(makeSixthCFGraph(reg32, reg20, reg33, reg36, reg11, reg37, reg34, reg35, reg38))
                    .GetInterferenceGraph();

            const DirectedGraph graph = ifGraph.GetDGRef();
            CPPUNIT_ASSERT_EQUAL(11, ifGraph.GetNodeSize());
            CPPUNIT_ASSERT_EQUAL((size_t)8, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(Frame_EDX(), Frame_EAX()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(Frame_EDX(), reg37));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg33, reg11));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg37, reg34));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg37, Frame_EAX()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg37, reg35));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(Frame_EAX(), reg35));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg34, reg35));
        }

        void testCtor_SixthCFGraph_GenerateMovePairs()
        {
            myTemp reg32 = Temp_newTemp(), reg20 = Temp_newTemp(), reg33 = Temp_newTemp();
            myTemp reg36 = Temp_newTemp(), reg11 = Temp_newTemp(), reg37 = Temp_newTemp();
            myTemp reg34 = Temp_newTemp(), reg35 = Temp_newTemp(), reg38 = Temp_newTemp();

            Liveness liveness(makeSixthCFGraph(reg32, reg20, reg33, reg36, reg11, reg37, reg34, reg35, reg38));

            CPPUNIT_ASSERT_EQUAL((size_t)2, liveness.GetMovePairs().size());
        }

        //////////////////////////////////////////////////////////////////

        void testCtor_SeventhCGraph_CalculateOut()
        {
            myTemp regA = Temp_newTemp();
            myTemp regB = Temp_newTemp();

            Liveness liveness(makeSeventhCFGraph(regA, regB));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(2).size());
            AssertOneRegisters_Two(mock.GetOut().at(1), regA, regB);
            AssertOneRegisters_One(mock.GetOut().at(0), regA);
        }

        void testCtor_SeventhCGraph_CalculateIn()
        {
            myTemp regA = Temp_newTemp();
            myTemp regB = Temp_newTemp();

            Liveness liveness(makeSeventhCFGraph(regA, regB));

            LivenessMock mock(liveness);
            AssertOneRegisters_Two(mock.GetIn().at(2), regA, regB);
            AssertOneRegisters_One(mock.GetIn().at(1), regA);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetIn().at(0).size());
        }

        void testCtor_SeventhCFGraph_GenerateInterferenceGraph()
        {
            myTemp regA = Temp_newTemp();
            myTemp regB = Temp_newTemp();

            InterferenceGraph ifGraph =
                Liveness(makeSeventhCFGraph(regA, regB)).GetInterferenceGraph();

            const DirectedGraph& graph = ifGraph.GetDGRef();
            CPPUNIT_ASSERT_EQUAL(5, ifGraph.GetNodeSize());
            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(regA, regB));
        }

        void testCtor_SeventhCFGraph_GenerateMovePairs()
        {
            myTemp regA = Temp_newTemp();
            myTemp regB = Temp_newTemp();

            Liveness liveness(makeSeventhCFGraph(regA, regB));

            CPPUNIT_ASSERT_EQUAL((size_t)0, liveness.GetMovePairs().size());
        }

        //////////////////////////////////////////////////////////////////

        void testCtor_EighthCGraph_CalculateOut()
        {
            myTemp reg142 = Temp_newTemp();
            myTemp reg144 = Temp_newTemp();
            myTemp reg143 = Temp_newTemp();
            myTemp reg145 = Temp_newTemp();
            myTemp reg146 = Temp_newTemp();

            Liveness liveness(makeEighthIns(reg142, reg144, reg143, reg145, reg146));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(7).size());
            AssertOneRegisters_Two(mock.GetOut().at(6), reg145, reg146);
            AssertOneRegisters_Two(mock.GetOut().at(5), reg144, reg145);
            AssertOneRegisters_Two(mock.GetOut().at(4), reg143, reg144);
            AssertOneRegisters_Two(mock.GetOut().at(3), reg143, reg144);
            AssertOneRegisters_Two(mock.GetOut().at(2), Frame_EBP(), reg144);
            AssertOneRegisters_Two(mock.GetOut().at(1), Frame_EBP(), reg142);
            AssertOneRegisters_Two(mock.GetOut().at(0), Frame_EBP(), reg142);
        }

        void testCtor_EighthCGraph_CalculateIn()
        {
            myTemp reg142 = Temp_newTemp();
            myTemp reg144 = Temp_newTemp();
            myTemp reg143 = Temp_newTemp();
            myTemp reg145 = Temp_newTemp();
            myTemp reg146 = Temp_newTemp();

            Liveness liveness(makeEighthIns(reg142, reg144, reg143, reg145, reg146));

            LivenessMock mock(liveness);
            AssertOneRegisters_Two(mock.GetIn().at(7), reg145, reg146);
            AssertOneRegisters_Two(mock.GetIn().at(6), reg145, reg144);
            AssertOneRegisters_Two(mock.GetIn().at(5), reg144, reg143);
            AssertOneRegisters_Two(mock.GetIn().at(4), reg143, reg144);
            AssertOneRegisters_Two(mock.GetIn().at(3), Frame_EBP(), reg144);
            AssertOneRegisters_Two(mock.GetIn().at(2), Frame_EBP(), reg142);
            AssertOneRegisters_Two(mock.GetIn().at(1), Frame_EBP(), reg142);
            AssertOneRegisters_One(mock.GetIn().at(0), Frame_EBP());
        }

        void testCtor_EighthCFGraph_GenerateInterferenceGraph()
        {
            myTemp reg142 = Temp_newTemp();
            myTemp reg144 = Temp_newTemp();
            myTemp reg143 = Temp_newTemp();
            myTemp reg145 = Temp_newTemp();
            myTemp reg146 = Temp_newTemp();

            InterferenceGraph ifGraph =
                Liveness(makeEighthIns(reg142, reg144, reg143, reg145, reg146))
                    .GetInterferenceGraph();

            const DirectedGraph& graph = ifGraph.GetDGRef();
            CPPUNIT_ASSERT_EQUAL(6, ifGraph.GetNodeSize());
            CPPUNIT_ASSERT_EQUAL((size_t)5, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg142, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(Frame_EBP(), reg144));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg143, reg144));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg144, reg145));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg145, reg146));
        }

        void testCtor_EightCFGraph_GenerateMovePairs()
        {
            myTemp reg142 = Temp_newTemp();
            myTemp reg144 = Temp_newTemp();
            myTemp reg143 = Temp_newTemp();
            myTemp reg145 = Temp_newTemp();
            myTemp reg146 = Temp_newTemp();

            Liveness liveness(makeEighthIns(reg142, reg144, reg143, reg145, reg146));

            CPPUNIT_ASSERT_EQUAL((size_t)3, liveness.GetMovePairs().size());
        }

        /////////////////////////////////////////////////

        void testCtor_NinthCGraph_CalculateOut()
        {
            myTemp reg131 = Temp_newTemp();
            myTemp reg133 = Temp_newTemp();
            myTemp reg134 = Temp_newTemp();
            myTemp reg135 = Temp_newTemp(); 
            myTemp reg137 = Temp_newTemp();
            myTemp reg138 = Temp_newTemp();
            myTemp reg139 = Temp_newTemp();
            myTemp reg140 = Temp_newTemp();
            myTemp reg141 = Temp_newTemp();

            myTemp reg142 = Temp_newTemp();
            myTemp reg144 = Temp_newTemp();
            myTemp reg143 = Temp_newTemp();
            myTemp reg145 = Temp_newTemp();
            myTemp reg146 = Temp_newTemp();

            Liveness liveness(makeNinthIns( reg131, reg133, reg134, reg137,
                                            reg135, reg138, reg139, reg140,
                                            reg141, reg142, reg144, reg143, 
                                            reg145, reg146));

            LivenessMock mock(liveness);
            CPPUNIT_ASSERT_EQUAL((size_t)0, mock.GetOut().at(21).size());
            AssertOneRegisters_Two(mock.GetOut().at(20), reg145, reg146);
            AssertOneRegisters_Two(mock.GetOut().at(19), reg144, reg145);
            AssertOneRegisters_Two(mock.GetOut().at(18), reg143, reg144);
            AssertOneRegisters_Two(mock.GetOut().at(17), reg143, reg144);
            AssertOneRegisters_Two(mock.GetOut().at(16), Frame_EBP(), reg144);
            AssertOneRegisters_Two(mock.GetOut().at(15), Frame_EBP(), reg142);
            AssertOneRegisters_Two(mock.GetOut().at(14), Frame_EBP(), reg142);
            AssertOneRegisters_One(mock.GetOut().at(13), Frame_EBP());
            AssertOneRegisters_Three(mock.GetOut().at(12), Frame_EBP(), reg140, reg141);
            AssertOneRegisters_Three(mock.GetOut().at(11), Frame_EBP(), reg139, reg140);
            AssertOneRegisters_Two(mock.GetOut().at(10), Frame_EBP(), reg139);
            AssertOneRegisters_One(mock.GetOut().at(9), Frame_EBP());
            AssertOneRegisters_Two(mock.GetOut().at(8), Frame_EBP(), reg137);
            AssertOneRegisters_Three(mock.GetOut().at(7), Frame_EBP(), reg135, reg137);
            AssertOneRegisters_Two(mock.GetOut().at(6), Frame_EBP(), reg137);
            AssertOneRegisters_Two(mock.GetOut().at(5), Frame_EBP(), reg137);
            AssertOneRegisters_One(mock.GetOut().at(4), Frame_EBP());
            AssertOneRegisters_Two(mock.GetOut().at(3), Frame_EBP(), reg133);
            AssertOneRegisters_Three(mock.GetOut().at(2), Frame_EBP(), reg131, reg133);
            AssertOneRegisters_Two(mock.GetOut().at(1), Frame_EBP(), reg133);
            AssertOneRegisters_Two(mock.GetOut().at(0), Frame_EBP(), reg133);
        }

        void testCtor_NinthCGraph_CalculateIn()
        {
            myTemp reg131 = Temp_newTemp();
            myTemp reg133 = Temp_newTemp();
            myTemp reg134 = Temp_newTemp();
            myTemp reg135 = Temp_newTemp(); 
            myTemp reg137 = Temp_newTemp();
            myTemp reg138 = Temp_newTemp();
            myTemp reg139 = Temp_newTemp();
            myTemp reg140 = Temp_newTemp();
            myTemp reg141 = Temp_newTemp();

            myTemp reg142 = Temp_newTemp();
            myTemp reg144 = Temp_newTemp();
            myTemp reg143 = Temp_newTemp();
            myTemp reg145 = Temp_newTemp();
            myTemp reg146 = Temp_newTemp();

            Liveness liveness(makeNinthIns( reg131, reg133, reg134, reg137,
                                            reg135, reg138, reg139, reg140,
                                            reg141, reg142, reg144, reg143, 
                                            reg145, reg146));

            LivenessMock mock(liveness);
            AssertOneRegisters_Two(mock.GetIn().at(21), reg145, reg146);
            AssertOneRegisters_Two(mock.GetIn().at(20), reg144, reg145);
            AssertOneRegisters_Two(mock.GetIn().at(19), reg143, reg144);
            AssertOneRegisters_Two(mock.GetIn().at(18), reg143, reg144);
            AssertOneRegisters_Two(mock.GetIn().at(17), Frame_EBP(), reg144);
            AssertOneRegisters_Two(mock.GetIn().at(16), Frame_EBP(), reg142);
            AssertOneRegisters_Two(mock.GetIn().at(15), Frame_EBP(), reg142);
            AssertOneRegisters_One(mock.GetIn().at(14), Frame_EBP());
            AssertOneRegisters_Three(mock.GetIn().at(13), Frame_EBP(), reg140, reg141);
            AssertOneRegisters_Three(mock.GetIn().at(12), Frame_EBP(), reg140, reg139);
            AssertOneRegisters_Two(mock.GetIn().at(11), Frame_EBP(), reg139);
            AssertOneRegisters_One(mock.GetIn().at(10), Frame_EBP());
            AssertOneRegisters_Two(mock.GetIn().at(9), Frame_EBP(), reg137);
            AssertOneRegisters_Three(mock.GetIn().at(8), Frame_EBP(), reg137, reg135);
            AssertOneRegisters_Two(mock.GetIn().at(7), Frame_EBP(), reg137);
            AssertOneRegisters_Two(mock.GetIn().at(6), Frame_EBP(), reg137);
            AssertOneRegisters_One(mock.GetIn().at(5), Frame_EBP());
            AssertOneRegisters_Two(mock.GetIn().at(4), Frame_EBP(), reg133);
            AssertOneRegisters_Three(mock.GetIn().at(3), Frame_EBP(), reg133, reg131);
            AssertOneRegisters_Two(mock.GetIn().at(2), Frame_EBP(), reg133);
            AssertOneRegisters_Two(mock.GetIn().at(1), Frame_EBP(), reg133);
            AssertOneRegisters_One(mock.GetIn().at(0), Frame_EBP());
        }

        void testCtor_NinthCGraph_CalculateInterferenceGraph()
        {
            myTemp reg131 = Temp_newTemp();
            myTemp reg133 = Temp_newTemp();
            myTemp reg134 = Temp_newTemp();
            myTemp reg135 = Temp_newTemp(); 
            myTemp reg137 = Temp_newTemp();
            myTemp reg138 = Temp_newTemp();
            myTemp reg139 = Temp_newTemp();
            myTemp reg140 = Temp_newTemp();
            myTemp reg141 = Temp_newTemp();

            myTemp reg142 = Temp_newTemp();
            myTemp reg144 = Temp_newTemp();
            myTemp reg143 = Temp_newTemp();
            myTemp reg145 = Temp_newTemp();
            myTemp reg146 = Temp_newTemp();

            InterferenceGraph ifGraph =
                Liveness(makeNinthIns(  reg131, reg133, reg134, reg137,
                                        reg135, reg138, reg139, reg140,
                                        reg141, reg142, reg144, reg143, 
                                        reg145, reg146))
                    .GetInterferenceGraph();

            const DirectedGraph& graph = ifGraph.GetDGRef();
            CPPUNIT_ASSERT_EQUAL(15, ifGraph.GetNodeSize());
            CPPUNIT_ASSERT_EQUAL((size_t)18, graph.GetEdges().size());
	    CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg133, reg131));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg137, reg135));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg144, reg145));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg145, reg146));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg143, reg144));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg140, reg141));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg139, reg140));
            
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg133, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg137, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg131, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg135, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg144, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg142, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg141, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg139, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg134, Frame_EBP()));
            CPPUNIT_ASSERT_EQUAL(true, ifGraph.EdgesContains(reg138, Frame_EBP()));
        }

        void testCtor_NinthCFGraph_GenerateMovePairs()
        {
            myTemp reg131 = Temp_newTemp();
            myTemp reg133 = Temp_newTemp();
            myTemp reg134 = Temp_newTemp();
            myTemp reg135 = Temp_newTemp(); 
            myTemp reg137 = Temp_newTemp();
            myTemp reg138 = Temp_newTemp();
            myTemp reg139 = Temp_newTemp();
            myTemp reg140 = Temp_newTemp();
            myTemp reg141 = Temp_newTemp();

            myTemp reg142 = Temp_newTemp();
            myTemp reg144 = Temp_newTemp();
            myTemp reg143 = Temp_newTemp();
            myTemp reg145 = Temp_newTemp();
            myTemp reg146 = Temp_newTemp();

            Liveness liveness(makeNinthIns( reg131, reg133, reg134, reg137,
                                            reg135, reg138, reg139, reg140,
                                            reg141, reg142, reg144, reg143, 
                                            reg145, reg146));

            CPPUNIT_ASSERT_EQUAL((size_t)6, liveness.GetMovePairs().size());
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
