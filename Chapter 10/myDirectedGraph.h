#ifndef MY_DIRECTED_GRAPH_H
#define MY_DIRECTED_GRAPH_H

#include <vector>
#include <memory>

namespace lgxZJ
{
    namespace LA    //  stand for Liveness Analysis
    {
        ////////////////////////////////////////////////////////////////////////////////
        //                              Node About
        ////////////////////////////////////////////////////////////////////////////////

        typedef int                 Node;
        typedef std::vector<Node>   NodeSet;

        ////////////////////////////////////////////////////////////////////////////////
        //                              Edge About
        ////////////////////////////////////////////////////////////////////////////////

        class Edge
        {
            private:
                Node    from;
                Node    to;

            public:
                explicit Edge(Node oneFrom, Node oneTo)
                    : from(oneFrom), to(oneTo)
                {}

                Node GetFrom() const    { return from; }
                Node GetTo() const      { return to; }
        };

        typedef std::vector<Edge>   EdgeSet;

        ////////////////////////////////////////////////////////////////////////////////
        //                              Directed Graph
        ////////////////////////////////////////////////////////////////////////////////

        class DirectedGraph
        {
            private:
                struct RealNode
                {
                    std::vector<Node>    successors;
                    std::vector<Node>    predecessors;

                    void AddSuccessor(Node successor);
                    void AddPredecessor(Node predecessor);

                    void RemoveSuccessor(Node successor);
                    void RemovePredecessor(Node predecessor);

                    void RemoveOneNodeInSet(NodeSet& nodes, Node one);
                    int GetNodeIndex(NodeSet& nodes, Node node) const;
                };

                std::vector<RealNode>   nodes;
                EdgeSet                 edges;

            public:
                explicit DirectedGraph();
                explicit DirectedGraph(unsigned n);

                //  DO:
                //      Make a new node and add it to the calling graph.
                //  RETURN:
                //      The new node. 
                Node AddNode();

                //  DO:
                //      Get the node by postion.
                //  PARAMS:
                //      pos     The position of the node.
                //  REMARK:
                //      Node positions start from 0 and are recorded in order. So,
                //      the first one has position 0, the second 1, the thrid 2 and
                //      so on...
                //
                //      It's a checked runtime error if the given position is illegal
                //      or does not exists.
                Node GetNode(int pos) const;

                //  DO:
                //      Get all the nodes in the calling graph.
                //  RETURN:
                //      The whole nodes.
                NodeSet GetNodes() const;

                //  DO:
                //      Check if the given node is inside the graph.
                //  REMARK:
                //      If the given node is illegal, it will throw a runtime error.
                bool IsNodeInside(Node node) const;

                //  DO:
                //      Get all the successors of the given node.
                //  REMARK:
                //      If the given node not exists, it will throw a runtime error.
                NodeSet GetSuccessors(Node node) const;

                //  DO:
                //      Get all the predessors of the given node.
                //  REMARK:
                //      If the given node not exists, it will throw a runtime error.
                NodeSet GetPredecessors(Node node) const;


                //  DO:
                //      Make a new edge and add it to the calling graph.
                //  PARAMS:
                //      from    The node where the new edge starts.
                //      to      The node where the new edge ends.
                //  REMARK:
                //      After this call, 'm' will be found in the successor set of node 'n' and
                //      node 'n' will be found in the predecessor set of node 'm'.
                //
                //      It is a checked runtime error if trying to add a self-edge or the given
                //      nodes not exists.
                void AddEdge(Node from, Node to);

                //  DO:
                //      Check if the given edge exists in the calling graph.
                //  PARAMS:
                //      from    The node where the new edge starts.
                //      to      The node where the new edge ends.
                //  REMARK:
                //      It will throw a runtime error if the given nodes not exists.
                bool IsEdgeInside(Node from, Node to) const;

                //  DO:
                //      Get all the edges in the calling graph.
                EdgeSet GetEdges() const;

                //  DO:
                //      Get the degreee of the given node
                int GetNodeDegree(Node node) const;

                //  DO:
                //      Remove the specified edge.
                //  PARAMS:
                //      from    The node where the new edge starts.
                //      to      The node where the new edge ends.
                //  REMARK:
                //      Remove an edge also removes the relations between node 'n' and 'm'.
                //      After removed, edges used before cannot be used anymore, and users
                //      must re-acquire them.
                void RemoveEdge(Node from, Node to);

                //  DO:
                //      Remove all edges in the calling graph.
                //  REMARK:
                //      While edges are al removed, node numbers are not affected.
                void RemoveAllEdges();

                //  DO:
                //      Remove all, include nodes and edges.
                void RemoveAll();

            private:
                void RemoveOneEdge(Node from, Node to);
                int GetEdgeIndex(Node from, Node to) const;
        };
    }
}

#endif