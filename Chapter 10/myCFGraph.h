#ifndef MY_FLOW_DG_H
#define MY_FLOW_DG_H

#include "CXX_myTemp.h"
#include "myMunch.h"
#include "myDirectedGraph.h"

namespace lgxZJ
{
    namespace LA
    {
        typedef std::vector<IS::Registers>  RegistersSet;

        class CFGraph
        {
            protected:
                //  Every instruction corresponds to one node in graph in order.
                IS::Instructions    ins;
                DirectedGraph       graph;
                RegistersSet        uses;
                RegistersSet        defs;

            public:
                explicit CFGraph(IS::Instructions oneIns);

                DirectedGraph GetDirectedGraph() const;

                bool IsMoveIns(Node node) const;
                IS::Registers GetUses(Node node) const;
                IS::Registers GetDefs(Node node) const;
                std::shared_ptr<IS::AAI> GetNodeIns(Node node) const;

            private:
                int GetLabelIndex(myLabel label);
                void SetNodeUse(Node node, IS::Registers regs);
                void SetNodeDef(Node node, IS::Registers regs);
                void SetOneEdge(Node node, myLabel label);

                bool IsAAIJmp(std::shared_ptr<IS::AAI> ptr) const;
        };
    }
}

#endif