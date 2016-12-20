#ifndef MY_LIVENESS_H
#define MY_LIVENESS_H

#include "myCFGraph.h"

namespace lgxZJ
{
    namespace LA
    {
        //  REMARK:
        //      Indeed, interference graph is an undirected graph. But here,
        //      i just reuse the directed graph desinged earlier because
        //      directed graph is subset of undirected graph.
        class InterferenceGraph
        {
            protected:
                DirectedGraph   graph;
                IS::Registers   registers;
            
            public:
                explicit InterferenceGraph();
                explicit InterferenceGraph(unsigned nodeSize);

                //  DO:
                //      Get inner directed graph reference.
                DirectedGraph& GetDGRef();

                //  DO:
                //      Add an edge into a pretended undirected graph.
                //  REMARK:
                //      Self-edge and same edge is not added.
                void AddEdge(Node lhs, Node rhs);

                //  DO:
                //      Get the node size of this interference graph which is also
                //  the register count.
                int GetNodeSize() const;

                //  DO:
                //      Get the register represented the given node.
                //  REMARK:
                //      If the given node is not valid, it throws a runtime error.
                myTemp GetNodeReg(Node node) const;

                //  DO:
                //      Set the given node's value to the given register.
                //  REMARK:
                //      If the given node is not a valid one or the register given is null,
                //      it throws a runtime error.
                void SetNodeReg(Node node, myTemp reg);
        };

        class Liveness
        {
            protected:
                RegistersSet        in;
                RegistersSet        out;
                const CFGraph&      cfGraph;
                InterferenceGraph   interferenceGraph;

            public:
                explicit Liveness(const CFGraph& cfGraph);

                //  DO:
                //      Get the constructed interference graph.
                InterferenceGraph   GetInterferenceGraph() const;

            private:
                void DoLiveness();
                void InitInAndOut();
                void CalculateOneOut(Node node);
                void CalculateOneIn(Node node);
                void SortOneRegisters(IS::Registers& regs);
                IS::Registers GetSetUnion(IS::Registers& lhs, IS::Registers& rhs);
                IS::Registers GetSetDiff(IS::Registers& lhs, IS::Registers& rhs);
                bool InAndOutChanged(
                    RegistersSet& oldIn, RegistersSet& newIn,
                    RegistersSet& oldOut, RegistersSet& newOut) const;
                
                void GenerateInterferenceGraph();
                Node GetRegisterNode(myTemp temp) const;
                bool SrcRegisterIs(const IS::Registers& regs, const myTemp reg) const;
        };
    }
}

#endif