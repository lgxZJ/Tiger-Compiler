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
};

class CFGraphTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(CFGraphTest);

        CPPUNIT_TEST(testCtor_ByDefault_InitMembersToSameSizeAsIns);
        CPPUNIT_TEST(testCtor_ByDefault_ConstructGraph);
        CPPUNIT_TEST(testCtor_ByDefault_ConstructUses);
        CPPUNIT_TEST(testCtor_ByDefault_ConstructDefs);

        CPPUNIT_TEST_SUITE_END();

        ////////////////////////////////////////////////////////////////////////
        //                              fixture

        //      je label2  
        //      a = 0
        //  label1:
        //      c = c + b
        //  label2:
        //      a = a / c
        //      jmp label1
        //      a = a * c
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
            CPPUNIT_ASSERT_EQUAL((size_t)1, cfGraph.GetDefs(5).size());
            CPPUNIT_ASSERT_EQUAL((size_t)0, cfGraph.GetDefs(6).size());
            CPPUNIT_ASSERT_EQUAL((size_t)1, cfGraph.GetDefs(7).size());
        }
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CFGraphTest::suite());
    runner.run();
    return 0;
}