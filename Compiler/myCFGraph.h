#ifndef MY_FLOW_DG_H
#define MY_FLOW_DG_H

#include "CXX_myTemp.h"
#include "myMunch.h"
#include "myDirectedGraph.h"

#include <map>

namespace lgxZJ
{
    namespace LA
    {
        typedef std::vector<IS::Registers>          RegistersSet;
        typedef std::map<IS::Register, LA::Node>    RegisterNodeMap;

        class CFGraph
        {
            protected:
                //  to check only one epilogue each subroutine
                int epilogueCount;

                //  Every instruction corresponds to one node in graph in order.
                IS::Instructions    ins;
                DirectedGraph       graph;
                RegistersSet        uses;
                RegistersSet        defs;

                RegisterNodeMap     regNodeMaps;

            public:
                explicit CFGraph(IS::Instructions oneIns);

                //  DO:
                //      Get inner directed graph.
                DirectedGraph GetDirectedGraph() const;

                //  DO:
                //      Get Register-Node maps.

                const RegisterNodeMap& GetRegisterNodeMapRef() const;

                //  DO:
                //      Check if the given node represents a move instruction.
                //  REMARK:
                //      If the given node is invalid, it throws a runtime error.
                bool IsMoveIns(Node node) const;

                //  DO:
                //      Get the `use` set of the given node which will be used in generating
                //      interference graph.
                //  REMARK:
                //      If the given node is invalid, it throws a runtime error.
                IS::Registers GetUses(Node node) const;

                //  DO:
                //      Get the `def` set of the given node which will be used in generating
                //      interference graph.
                //  REMARK:
                //      If the given node is invalid, it throws a runtime error.
                IS::Registers GetDefs(Node node) const;

                //  DO:
                //      Get the instruction represented by the given node.
                //  REMARK:
                //      If the given node is invalid, it throws a runtime error.
                std::shared_ptr<IS::AAI> GetNodeIns(Node node) const;

		void PrintRegisterMaps() const;

            private:
                int GetLabelIndex(myLabel label);
                void SetNodeUse(Node node, IS::Registers regs);
                void SetNodeDef(Node node, IS::Registers regs);
                void SetOneEdge(Node node, myLabel label);

                void SetRegNodeMaps();

                bool IsAAIJmp(std::shared_ptr<IS::AAI> ptr) const;
        };
    }
}

#endif
