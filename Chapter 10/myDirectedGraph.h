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
                };

                std::vector<RealNode>   nodes;
                EdgeSet                 edges;

            public:
                explicit DirectedGraph();

                //  DO:
                //      Make a new node and add it to the calling graph.
                //  RETURN:
                //      The new node. 
                Node NewNode();

                //  DO:
                //      Get the node by postion.
                //  PARAMS:
                //      pos     The position of the node.
                //  REMARK:
                //      Node positions start from 0 and are recorded in order. So,
                //      the first one has position 0, the second 1, the thrid 2 and
                //      so on...
                Node GetNode(int pos) const;

                //  DO:
                //      Get all the nodes in the calling graph.
                //  RETURN:
                //      The whole nodes.
                NodeSet GetNodes() const;

                //  DO:
                //      Check if the given node is inside the graph.
                bool IsNodeInside(Node node) const;

                //  DO:
                //      Get all the successors of the given node.
                NodeSet GetSuccessors(Node node) const;

                //  DO:
                //      Get all the predessors of the given node.
                NodeSet GetPredecessors(Node node) const;


                //  DO:
                //      Make a new edge and add it to the calling graph.
                //  PARAMS:
                //      from    The node where the new edge starts.
                //      to      The node where the new edge ends.
                //  REMARK:
                //      After this call, 'm' will be found in the successor set of node 'n' and
                //      node 'n' will be found in the predecessor set of node 'm'.
                void NewEdge(Node from, Node to);

                //  DO:
                //      Check if the given edge exists in the calling graph.
                //  PARAMS:
                //      from    The node where the new edge starts.
                //      to      The node where the new edge ends.
                void IsEdgeInside(Node from, Node to) const;

                //  DO:
                //      Get all the edges in the calling graph.
                EdgeSet GetEdges() const;

                //  DO:
                //      Count the number of edges start from the given node.
                int EdgeCountFrom(Node node);

                //  DO:
                //      Count the number of edges end to the given node.
                int EdgeCountTo(Node node);

                //  DO:
                //      Remove the specified edge.
                //  PARAMS:
                //      from    The node position where the new edge starts.
                //      to      The node position where the new edge ends.
                //  REMARK:
                //      Remove an edge also removes the relations between node 'n' and 'm'.
                void RemoveEdge(int from, int to);

                //  DO:
                //      Remove all edges in the calling graph.
                //  REMARK:
                //      While edges are al removed, node numbers are not affected.
                void RemoveAllEdges();

                //  DO:
                //      Remove all, include nodes and edges.
                void RemoveAll();
        };
    }
}

#endif