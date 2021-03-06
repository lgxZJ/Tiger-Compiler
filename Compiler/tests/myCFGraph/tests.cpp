#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../myCFGraph.h"
#include "../../CXX_myFrame.h"
#include "../../AAI/myComputable.h"
#include "../../AAI/myControlable.h"
#include "../../AAI/myMovable.h"

#include <memory>

using namespace std;
using namespace lgxZJ::LA;
using namespace lgxZJ::IS;

class CFGraphMocker : public CFGraph
{
    public:
        explicit CFGraphMocker(CFGraph cfGraph) : CFGraph(cfGraph) {}

        RegistersSet GetAllUses() const { return uses; }
        RegistersSet GetAllDefs() const { return defs; }
        int GetEpilogueCount() const { return epilogueCount; }
};

class CFGraphTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(CFGraphTest);

        CPPUNIT_TEST(testCtor_ByDefault_InitMembersToSameSizeAsIns);
        CPPUNIT_TEST(testCtor_ByDefault_ConstructGraph);
        CPPUNIT_TEST(testCtor_ByDefault_ConstructUses);
        CPPUNIT_TEST(testCtor_ByDefault_ConstructDefs);
        CPPUNIT_TEST(testCtor_ByDefault_ConstructMaps);
        CPPUNIT_TEST(testGetNodeIns_ByDefault_GetIns);
        CPPUNIT_TEST(testIsMoveIns_PassMove_ReturnTrue);
        CPPUNIT_TEST(testIsMoveIns_PassNonMove_ReturnTrue);
        CPPUNIT_TEST(testGetRegisterNodeMapRef_ByDefault_DifferentRegDifferentNode);
        CPPUNIT_TEST(testGetRegisterNodeMapRef_InternalRegUse_MapContainsTheseReg);
        CPPUNIT_TEST(testGetRegisterNodeMapRef_EmptyInstruction_EmptyMap);

        CPPUNIT_TEST_SUITE_END();

        ////////////////////////////////////////////////////////////////////////
        //                              fixture

        //  Instructions:
        //          je label2  
        //          a = 0
        //      label1:
        //          c = c + b
        //      label2:
        //          a = a / c
        //          jmp label1
        //          a = a * c
        //
        //  Defs:           Uses:
        //          null            null
        //          a               null
        //          null            null
        //          c               b, c
        //          null            null
        //          a, d            a, c
        //          null            null
        //          a, d            a, c
        Instructions makeExampleIns()
        {
            Instructions ins;

            myLabel label1 = Temp_newLabel();
            myLabel label2 = Temp_newLabel();
            myTemp regA = Frame_EAX();
            myTemp regB = Temp_newTemp();
            myTemp regC = Temp_newTemp();

            ins.push_back(make_shared<Je>(label2));
            ins.push_back(shared_ptr<Move>(new Move(regA, Move::OperandType::Content, 0)));
            ins.push_back(make_shared<Label>(label1));
            ins.push_back(make_shared<Add>(regC, regB));
            ins.push_back(make_shared<Label>(label2));
            ins.push_back(shared_ptr<IDiv>(new IDiv(regC)));
            ins.push_back(make_shared<Jmp>(label1));
            ins.push_back(make_shared<IMul>(regC));
            return ins;
        }

        ////////////////////////////////////////////////////////////////////////
        //                              tests

        void testOneEdge_FromTo(Edge edge, Node from, Node to)
        {
            CPPUNIT_ASSERT_EQUAL(from, edge.GetFrom());
            CPPUNIT_ASSERT_EQUAL(to, edge.GetTo());
        }

        void testCtor_ByDefault_InitMembersToSameSizeAsIns()
        {
            Instructions ins;
            ins.push_back(shared_ptr<Label>(new Label(Temp_newLabel())));
            ins.push_back(shared_ptr<Cmp>(new Cmp(Temp_newTemp(), Temp_newTemp())));
            CFGraphMocker mock((CFGraph(ins)));

            CPPUNIT_ASSERT_EQUAL(0, mock.GetEpilogueCount());
            CPPUNIT_ASSERT_EQUAL((size_t)2, mock.GetDirectedGraph().GetNodes().size());
            CPPUNIT_ASSERT_EQUAL((size_t)2, mock.GetAllUses().size());
            CPPUNIT_ASSERT_EQUAL((size_t)2, mock.GetAllDefs().size());
        }

        void testCtor_ByDefault_ConstructGraph()
        {
            CFGraph cfGraph(makeExampleIns());

            DirectedGraph graph = cfGraph.GetDirectedGraph();

            CPPUNIT_ASSERT_EQUAL((size_t)8, graph.GetEdges().size());
            testOneEdge_FromTo(graph.GetEdges().at(0), 0, 4);
            testOneEdge_FromTo(graph.GetEdges().at(1), 0, 1);
            testOneEdge_FromTo(graph.GetEdges().at(2), 1, 2);
            testOneEdge_FromTo(graph.GetEdges().at(3), 2, 3);
            testOneEdge_FromTo(graph.GetEdges().at(4), 3, 4);
            testOneEdge_FromTo(graph.GetEdges().at(5), 4, 5);
            testOneEdge_FromTo(graph.GetEdges().at(6), 5, 6);
            testOneEdge_FromTo(graph.GetEdges().at(7), 6, 2);
        }

        void testCtor_ByDefault_ConstructUses()
        {
            CFGraph cfGraph(makeExampleIns());

            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetUses(0).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetUses(1).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetUses(2).size());
            CPPUNIT_ASSERT_EQUAL((size_t)2, cfGraph.GetUses(3).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetUses(4).size());
            CPPUNIT_ASSERT_EQUAL((size_t)2, cfGraph.GetUses(5).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetUses(6).size());
            CPPUNIT_ASSERT_EQUAL((size_t)2, cfGraph.GetUses(7).size());
        }

        void testCtor_ByDefault_ConstructDefs()
        {
            CFGraph cfGraph(makeExampleIns());

            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetDefs(0).size());
            CPPUNIT_ASSERT_EQUAL((size_t)1, cfGraph.GetDefs(1).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetDefs(2).size());
            CPPUNIT_ASSERT_EQUAL((size_t)1, cfGraph.GetDefs(3).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetDefs(4).size());
            CPPUNIT_ASSERT_EQUAL((size_t)2, cfGraph.GetDefs(5).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetDefs(6).size());
            CPPUNIT_ASSERT_EQUAL((size_t)2, cfGraph.GetDefs(7).size());
        }

        void testCtor_ByDefault_ConstructMaps()
        {
            myTemp  regA = Temp_newTemp(), regB = Temp_newTemp(),
                    regC = regA, regD = regB,
                    regE = Temp_newTemp(), regEAX = Frame_EAX(),
                    regF = regEAX;
            Instructions ins{
                make_shared<Move>(regA, regB),
                make_shared<Move>(regA, regC),
                make_shared<Move>(regA, regD),
                make_shared<Move>(regA, regE),
                make_shared<Move>(regA, regEAX),
                make_shared<Move>(regA, regF),
            };
            CFGraph cfGraph(ins);

            const RegisterNodeMap& ref = cfGraph.GetRegisterNodeMapRef();
            CPPUNIT_ASSERT_EQUAL( 4, static_cast<int>(cfGraph.GetRegisterNodeMapRef().size()) );
            CPPUNIT_ASSERT( ref.at(regA) != ref.at(regB) );
            CPPUNIT_ASSERT( ref.at(regA) != ref.at(regE) );
            CPPUNIT_ASSERT( ref.at(regA) != ref.at(regEAX) );
            CPPUNIT_ASSERT( ref.at(regB) != ref.at(regE) );
            CPPUNIT_ASSERT( ref.at(regB) != ref.at(regEAX) );
            CPPUNIT_ASSERT( ref.at(regE) != ref.at(regEAX) );
        }

        void testGetNodeIns_ByDefault_GetIns()
        {
            shared_ptr<AAI> aaiOne = make_shared<Label>(Temp_newLabel());
            shared_ptr<AAI> aaiTwo = make_shared<Cmp>(Temp_newTemp(), Temp_newTemp());
            Instructions ins;
            ins.push_back(aaiOne);
            ins.push_back(aaiTwo);
            CFGraph cfGraph(ins);

            CPPUNIT_ASSERT_EQUAL(aaiOne, cfGraph.GetNodeIns(0));
            CPPUNIT_ASSERT_EQUAL(aaiTwo, cfGraph.GetNodeIns(1));
        }

        void testIsMoveIns_PassMove_ReturnTrue()
        {
            Instructions ins;
            ins.push_back(make_shared<Move>(Temp_newTemp(), Temp_newTemp()));

            CFGraph graph(ins);

            CPPUNIT_ASSERT_EQUAL(true, graph.IsMoveIns(0));
        }

        void testIsMoveIns_PassNonMove_ReturnTrue()
        {
            myLabel label = Temp_newLabel();
            Instructions ins;
            ins.push_back(make_shared<Label>(label));
            ins.push_back(make_shared<Add>(Temp_newTemp(), Temp_newTemp()));
            ins.push_back(make_shared<IMul>(Temp_newTemp()));
            ins.push_back(make_shared<Cmp>(Temp_newTemp(), Temp_newTemp()));
            ins.push_back(make_shared<Jmp>(label));

            CFGraph graph(ins);

            for (int i = 0; i < 5; ++i)
                CPPUNIT_ASSERT_EQUAL(false, graph.IsMoveIns(i));
        }

        void testGetRegisterNodeMapRef_ByDefault_DifferentRegDifferentNode()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp(), regC = regB;
            CFGraph graph (Instructions{
                make_shared<Add>(regA, regB),
                make_shared<Cmp>(regB, regC),
            });

            const RegisterNodeMap& map = graph.GetRegisterNodeMapRef();

            CPPUNIT_ASSERT_EQUAL( 2, (int)map.size() );
            CPPUNIT_ASSERT( map.at(regA) != map.at(regB) );
            CPPUNIT_ASSERT( map.at(regC) == map.at(regB) );
        }

        void testGetRegisterNodeMapRef_InternalRegUse_MapContainsTheseReg()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp();
            CFGraph graph (Instructions{
                make_shared<IMul>(regA),
                make_shared<IDiv>(regB),
            });

            const RegisterNodeMap& map = graph.GetRegisterNodeMapRef();

            CPPUNIT_ASSERT_EQUAL( 4, (int)map.size() );
            CPPUNIT_ASSERT( map.at(regA) != map.at(regB) );
            CPPUNIT_ASSERT( map.at(Frame_EAX()) != map.at(Frame_EDX()) );
        }

        void testGetRegisterNodeMapRef_EmptyInstruction_EmptyMap()
        {
            myTemp regA = Temp_newTemp(), regB = Temp_newTemp();
            CFGraph graph (Instructions{});

            const RegisterNodeMap& map = graph.GetRegisterNodeMapRef();

            CPPUNIT_ASSERT_EQUAL( 0, (int)map.size() );
        }
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CFGraphTest::suite());
    runner.run();
    return 0;
}