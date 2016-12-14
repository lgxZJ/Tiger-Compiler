#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../myDirectedGraph.h"

using namespace lgxZJ::LA;

class DirectedGraphTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(DirectedGraphTest);

        CPPUNIT_TEST(testCtor_DefaultCtor_EmptyGraph);
        CPPUNIT_TEST(testCtor_OneParamCtor_ParamNodesAndEmptyEdge);
        CPPUNIT_TEST(testNewNode_ByDefault_AddOneNodeAndReturnPosition);
        CPPUNIT_TEST(testGetNode_ByDefault_ReturnWhatPassed);
        CPPUNIT_TEST(testIsNodeInside_PassNodeNotAdded_ReturnFalse);
        CPPUNIT_TEST(testIsNodeInside_PassNodeAdded_ReturnTrue);
        CPPUNIT_TEST(testGetSuccessors_ByDefault_GetItsSuccessors);
        CPPUNIT_TEST(testGetPredecessors_ByDefault_GetItsPredecessors);
        CPPUNIT_TEST(testAddEdge_ByDefault_AddOneEdge);
        CPPUNIT_TEST(testAddEdge_ByDefault_AddSuccessorAndPredecessor);
        CPPUNIT_TEST(testIsEdgeInside_PassEdgeNotInside_ReturnFalse);
        CPPUNIT_TEST(testIsEdgeInside_PassEdgeInside_ReturnTrue);
        CPPUNIT_TEST(testGetNodeDegree_ByDefault_ReturnSuccessorCountPlusPredecessorCount);

        CPPUNIT_TEST_SUITE_END();

    public:
        ///////////////////////////////////////////////////////////////////
        //                          tests

        void testCtor_DefaultCtor_EmptyGraph()
        {
            DirectedGraph graph;

            size_t nodeNum = graph.GetNodes().size();
            size_t edgeNum = graph.GetEdges().size();
            CPPUNIT_ASSERT_EQUAL((size_t)0, nodeNum);
            CPPUNIT_ASSERT_EQUAL((size_t)0, edgeNum);
        }

        void testCtor_OneParamCtor_ParamNodesAndEmptyEdge()
        {
            DirectedGraph graph(3);

            size_t nodeNum = graph.GetNodes().size();
            size_t edgeNum = graph.GetEdges().size();
            CPPUNIT_ASSERT_EQUAL((size_t)3, nodeNum);
            CPPUNIT_ASSERT_EQUAL((size_t)0, edgeNum);
        }

        void testNewNode_ByDefault_AddOneNodeAndReturnPosition()
        {
            DirectedGraph graph;

            Node node = graph.AddNode();

            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetNodes().size());
            CPPUNIT_ASSERT_EQUAL(node, Node(0));;
        }

        void testGetNode_ByDefault_ReturnWhatPassed()
        {
            DirectedGraph graph;
            graph.AddNode();

            Node node = graph.GetNode(0);

            CPPUNIT_ASSERT_EQUAL(0, node);
        }

        void testIsNodeInside_PassNodeNotAdded_ReturnFalse()
        {
            DirectedGraph graph;

            bool result = graph.IsNodeInside(0);

            CPPUNIT_ASSERT_EQUAL(false, result);
        }

        void testIsNodeInside_PassNodeAdded_ReturnTrue()
        {
            DirectedGraph graph;
            graph.AddNode();

            bool result = graph.IsNodeInside(0);

            CPPUNIT_ASSERT_EQUAL(true, result);
        }

        void testGetSuccessors_ByDefault_GetItsSuccessors()
        {
            DirectedGraph graph(3);
            Node firstNode = 0, secondNode = 1, thirdNode = 2; 
            graph.AddEdge(firstNode, secondNode);
            graph.AddEdge(secondNode, firstNode);
            graph.AddEdge(thirdNode, secondNode);

            NodeSet firstResult = graph.GetSuccessors(firstNode);
            NodeSet secondResult = graph.GetSuccessors(secondNode);
            NodeSet thirdResult = graph.GetSuccessors(thirdNode);
            CPPUNIT_ASSERT_EQUAL((size_t)1, firstResult.size());
            CPPUNIT_ASSERT_EQUAL(secondNode, firstResult.at(0));
            CPPUNIT_ASSERT_EQUAL((size_t)1, secondResult.size());
            CPPUNIT_ASSERT_EQUAL(firstNode, secondResult.at(0));
            CPPUNIT_ASSERT_EQUAL((size_t)1 ,thirdResult.size());
            CPPUNIT_ASSERT_EQUAL(secondNode, thirdResult.at(0));
        }

        void testGetPredecessors_ByDefault_GetItsPredecessors()
        {
            DirectedGraph graph(3);
            Node firstNode = 0, secondNode = 1, thirdNode = 2; 
            graph.AddEdge(firstNode, secondNode);
            graph.AddEdge(secondNode, firstNode);
            graph.AddEdge(thirdNode, secondNode);

            NodeSet firstResult = graph.GetPredecessors(firstNode);
            NodeSet secondResult = graph.GetPredecessors(secondNode);
            NodeSet thirdResult = graph.GetPredecessors(thirdNode);
            CPPUNIT_ASSERT_EQUAL((size_t)1, firstResult.size());
            CPPUNIT_ASSERT_EQUAL(secondNode, firstResult.at(0));
            CPPUNIT_ASSERT_EQUAL((size_t)2, secondResult.size());
            CPPUNIT_ASSERT_EQUAL(firstNode, secondResult.at(0));
            CPPUNIT_ASSERT_EQUAL(thirdNode, secondResult.at(1));
            CPPUNIT_ASSERT_EQUAL((size_t)0 ,thirdResult.size());
        }

        void testAddEdge_ByDefault_AddOneEdge()
        {
            DirectedGraph graph(3);

            graph.AddEdge(1, 2);

            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetEdges().size());
            CPPUNIT_ASSERT_EQUAL(1, graph.GetEdges().at(0).GetFrom());
            CPPUNIT_ASSERT_EQUAL(2, graph.GetEdges().at(0).GetTo());
        }

        void testAddEdge_ByDefault_AddSuccessorAndPredecessor()
        {
            DirectedGraph graph(2);
            Node firstNode = 0, secondNode = 1;

            graph.AddEdge(secondNode, firstNode);

            CPPUNIT_ASSERT_EQUAL((size_t)0, graph.GetSuccessors(firstNode).size());
            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetSuccessors(secondNode).size());
            CPPUNIT_ASSERT_EQUAL(firstNode, graph.GetSuccessors(secondNode).at(0));
            CPPUNIT_ASSERT_EQUAL((size_t)1, graph.GetPredecessors(firstNode).size());
            CPPUNIT_ASSERT_EQUAL(secondNode, graph.GetPredecessors(firstNode).at(0));
            CPPUNIT_ASSERT_EQUAL((size_t)0, graph.GetPredecessors(secondNode).size());
        }

        void testIsEdgeInside_PassEdgeNotInside_ReturnFalse()
        {
            DirectedGraph graph(3);
            Node firstNode = 0, secondNode = 1, thirdNode = 2;
            graph.AddEdge(firstNode, thirdNode);

            bool firstRet = graph.IsEdgeInside(firstNode, secondNode);
            bool secondRet = graph.IsEdgeInside(secondNode, thirdNode);
            bool thirdRet = graph.IsEdgeInside(thirdNode, secondNode);
            bool fourthRet = graph.IsEdgeInside(thirdNode, firstNode);
            bool fifthRet = graph.IsEdgeInside(secondNode, firstNode);
            CPPUNIT_ASSERT_EQUAL(false, firstRet);
            CPPUNIT_ASSERT_EQUAL(false, secondRet);
            CPPUNIT_ASSERT_EQUAL(false, thirdRet);
            CPPUNIT_ASSERT_EQUAL(false, fourthRet);
            CPPUNIT_ASSERT_EQUAL(false, fifthRet);
        }

        void testIsEdgeInside_PassEdgeInside_ReturnTrue()
        {
            DirectedGraph graph(3);
            Node firstNode = 0, secondNode = 1, thirdNode = 2;
            graph.AddEdge(firstNode, thirdNode);

            bool result = graph.IsEdgeInside(firstNode, thirdNode);
            CPPUNIT_ASSERT_EQUAL(true, result);
        }

        void testGetNodeDegree_ByDefault_ReturnSuccessorCountPlusPredecessorCount()
        {
            DirectedGraph graph(3);
            Node firstNode = 0, secondNode = 1, thirdNode = 2;
            graph.AddEdge(firstNode, thirdNode);
            graph.AddEdge(firstNode, secondNode);

            int firstResult = graph.GetNodeDegree(firstNode);
            int secondResult = graph.GetNodeDegree(secondNode);
            CPPUNIT_ASSERT_EQUAL(2, firstResult);
            CPPUNIT_ASSERT_EQUAL(1, secondResult);
        }
};

int main ()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(DirectedGraphTest::suite());
    runner.run();
    return 0;
}