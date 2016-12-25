#ifndef MY_LIVENESS_H
#define MY_LIVENESS_H

#include "myCFGraph.h"

namespace lgxZJ
{
    namespace LA
    {
        typedef EdgeSet   MovePairs;

        //  REMARK:
        //      Indeed, interference graph is an undirected graph. But here,
        //      i just reuse the directed graph desinged earlier because
        //      directed graph is subset of undirected graph.
        //
        //      Every node represent one register, which position is decided
        //      by register number(can be got via Temp_getTempNum() function).
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
                //      Add an edge(specified by nodes) into a pretended undirected graph.
                //  REMARK:
                //      Self-edge and same edge is not added.
                void AddEdge(Node lhs, Node rhs);

                //  DO:
                //      Add an edge(specified by registers) into a pretended undirected graph.
                //  REMARK:
                //      This function also set node values to registers.
                //
                //      If passed registers are nulls or not included in the graph, it throws
                //      a runtime error.
                void AddEdge(myTemp lhs, myTemp rhs);

                //  DO:
                //      Get the node in graph corresponding to the given register.
                //  REMARK:
                //      If passed registers are nulls or not included in the graph, it throws
                //      a runtime error.
                int GetRegNode(myTemp reg) const;

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

                //  DO:
                //      Check if the given edge is included in the graph.
                //  REMARK:
                //      If the node passed in is not valid, it throws a runtime error.
                bool EdgesContains(Node lhs, Node rhs) const;

                //  DO:
                //      Check if the given edge is included in the graph.
                //  REMARK:
                //      If the given registers are nulls or not included in the graph, it throws
                //      a runtime error.
                bool EdgesContains(myTemp lhs, myTemp rhs) const;
        };

        class Liveness
        {
            protected:
                RegistersSet        in;
                RegistersSet        out;
                const CFGraph&      cfGraph;

                MovePairs           movePairs;
                InterferenceGraph   interferenceGraph;

            public:
                explicit Liveness(const CFGraph& cfGraph);

                //  DO:
                //      Get the constructed interference graph.
                InterferenceGraph   GetInterferenceGraph() const;

                //  DO:
                //      Get the move pairs aloong with the interference graph that
                //      should be assigned the same register if possible (so that
                //      Move can be deleted).
                //  REAMRK:
                //      The move pairs are instructions that have both register operands
                //      and may or may not be an real interference edge. 
                MovePairs GetMovePairs() const;

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

                void GenerateMovePairs();
                bool MovePairContains(Node lhs, Node rhs) const;
                bool IsTwoRegisterOperandMove(Node node, IS::Registers& leftOperand, IS::Registers& rightOperand) const;
                bool IsSelfMove(IS::Registers& leftOperand, IS::Registers& rightOperand) const;
        };
    }
}

#endif