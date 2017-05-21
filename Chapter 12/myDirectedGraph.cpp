#include "myDirectedGraph.h"

#include <cassert>
#include <algorithm>
#include <iterator>

using namespace std;

namespace lgxZJ
{
    namespace LA
    {
        ///////////////////////////////////////////////////////////////////
        //                          DirectedGraph
        ///////////////////////////////////////////////////////////////////

        void DirectedGraph::RealNode::AddSuccessor(Node successor)
        {
            successors.push_back(successor);
        }

        void DirectedGraph::RealNode::AddPredecessor(Node predecessor)
        {
            predecessors.push_back(predecessor);
        }

        void DirectedGraph::RealNode::RemoveSuccessor(Node node)
        {
            RemoveOneNodeInSet(successors, node);
        }

        void DirectedGraph::RealNode::RemovePredecessor(Node node)
        {
            RemoveOneNodeInSet(predecessors, node);
        }

        void DirectedGraph::RealNode::RemoveOneNodeInSet(NodeSet& nodes, Node one)
        {
            int nodeIndex = GetNodeIndex(nodes, one);
            assert (nodeIndex != -1);

            NodeSet::iterator iter = nodes.begin();
            advance(iter, nodeIndex);

            nodes.erase(iter);
        }

        int DirectedGraph::RealNode::GetNodeIndex(NodeSet& nodes, Node node) const
        {
            for (int i = 0; i < nodes.size(); ++i)
                if (nodes.at(i) == node)    return i;
            return -1;
        }


        ////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////

        DirectedGraph::DirectedGraph() : nodes(), edges()
        {
        }

        DirectedGraph::DirectedGraph(unsigned n) : nodes(n), edges()
        {
        }

        ////////////////////////////////////////////////////////////////////

        Node DirectedGraph::AddNode()
        {
            nodes.push_back(RealNode());
            return nodes.size() - 1;
        }

        ////////////////////////////////////////////////////////////////////

        Node DirectedGraph::GetNode(int pos) const
        {
            assert (!nodes.empty());
            assert (pos >= 0 && pos < nodes.size());

            return pos;
        }

        ////////////////////////////////////////////////////////////////////

        NodeSet DirectedGraph::GetNodes() const
        {
            NodeSet result;
            for (int i = 0; i < nodes.size(); ++i)
                result.push_back(i);

            return result;
        }

        ////////////////////////////////////////////////////////////////////

        bool DirectedGraph::IsNodeInside(Node node) const
        {
            assert (node >= 0);
            return !nodes.empty() && node < nodes.size();
        }

        ////////////////////////////////////////////////////////////////////

        NodeSet DirectedGraph::GetSuccessors(Node node) const
        {
            assert (IsNodeInside(node));

            NodeSet result;
            for (auto edge : edges)
                if (edge.GetFrom() == node) result.push_back(edge.GetTo());

            return result;
        }

        ////////////////////////////////////////////////////////////////////

        NodeSet DirectedGraph::GetPredecessors(Node node) const
        {
            assert (IsNodeInside(node));

            NodeSet result;
            for (auto edge : edges)
                if (edge.GetTo() == node) result.push_back(edge.GetFrom());

            return result;
        }

        ////////////////////////////////////////////////////////////////////

        void DirectedGraph::AddEdge(Node from, Node to)
        {
            assert (IsNodeInside(from) && IsNodeInside(to));
            assert (from != to && "can not add self-edge!");

            if (!IsEdgeInside(from, to))
            {
                edges.push_back(Edge(from, to));

                nodes.at(from).AddSuccessor(to);
                nodes.at(to).AddPredecessor(from);
            }
        }

        ////////////////////////////////////////////////////////////////////

        bool DirectedGraph::IsEdgeInside(Node from, Node to) const
        {
            return GetEdgeIndex(from, to) != -1;
        }

        ////////////////////////////////////////////////////////////////////

        EdgeSet DirectedGraph::GetEdges() const
        {
            return edges;
        }

        ////////////////////////////////////////////////////////////////////

        int DirectedGraph::GetNodeDegree(Node node) const
        {
            assert (IsNodeInside(node));
            return GetSuccessors(node).size() + GetPredecessors(node).size();
        }

        ///////////////////////////////////////////////////////////////////

        void DirectedGraph::RemoveEdge(Node from, Node to)
        {
            RemoveOneEdge(from, to);
            
            nodes.at(from).RemoveSuccessor(to);
            nodes.at(to).RemovePredecessor(from);
        }

        void DirectedGraph::RemoveOneEdge(Node from, Node to)
        {
            int edgeIndex=  GetEdgeIndex(from, to);
            assert (edgeIndex != -1);

            EdgeSet::iterator iter = edges.begin();
            advance(iter, edgeIndex);

            edges.erase(iter);
        }

        /////////////////////////////////////////////////////////////////////

        void DirectedGraph::RemoveAllEdges()
        {
            edges.clear();

            for (auto& node : nodes)
                node.successors.clear(),
                node.predecessors.clear();
        }

        /////////////////////////////////////////////////////////////////////

        void DirectedGraph::RemoveAll()
        {
            RemoveAllEdges();
            nodes.clear();
        }

        ////////////////////////////////////////////////////////////////////

        int DirectedGraph::GetEdgeIndex(Node from, Node to) const
        {
            assert (IsNodeInside(from ) && IsNodeInside(to));

            for (int i = 0; i < edges.size(); ++i)
                if (edges.at(i).GetFrom() == from && edges.at(i).GetTo() == to)
                    return i;

            return -1;
        }

    }
}