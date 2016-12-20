#ifndef MY_LIVENESS_H
#define MY_LIVENESS_H

#include "myCFGraph.h"

namespace lgxZJ
{
    namespace LA
    {
        class InterferenceGraph
        {
            protected:
                DirectedGraph   graph;
                IS::Registers   registers;
            
            public:
                explicit InterferenceGraph();
                explicit InterferenceGraph(unsigned n);

                DirectedGraph& GetDGRef();

                myTemp GetNodeReg(Node node) const;
                void SetNodeReg(Node node, myTemp reg);
        };

        typedef std::pair<Node, Node>   MoveRep;
        typedef std::vector<MoveRep>    Moves;

        class Liveness
        {
            protected:
                RegistersSet        in;
                RegistersSet        out;
                const CFGraph&      cfGraph;
                InterferenceGraph   interferenceGraph;

            public:
                explicit Liveness(const CFGraph& cfGraph);

                InterferenceGraph   GetInterferenceGraph() const;
                Moves               GetMoves() const;

            private:
                void CalculateInOut();
                void CalculateOneOut(Node node);
                void CalculateOneIn(Node node);
                void SortOneRegisters(IS::Registers& regs);
                IS::Registers GetSetUnion(IS::Registers& lhs, IS::Registers& rhs);
                IS::Registers GetSetDiff(IS::Registers& lhs, IS::Registers& rhs);
                bool InAndOutChanged(
                    RegistersSet& oldIn, RegistersSet& newIn,
                    RegistersSet& oldOut, RegistersSet& newOut) const;
                
                void GenerateInterferenceGraph();
        };
    }
}

#endif