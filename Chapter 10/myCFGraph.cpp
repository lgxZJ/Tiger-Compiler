#include "myCFGraph.h"

#include <cassert>

using namespace std;
using namespace lgxZJ::IS;

namespace lgxZJ
{
    namespace LA
    {
        ////////////////////////////////////////////////////////////////////////
        //                          CFGraph
        ////////////////////////////////////////////////////////////////////////

        CFGraph::CFGraph(Instructions oneIns)
            : ins(oneIns), graph(ins.size()), uses(ins.size()), defs(ins.size())
        {
            for (int i = 0; i < ins.size(); ++i)
                {
                    SetNodeUse(i, ins.at(i)->GetSrcRegs());
                    SetNodeDef(i, ins.at(i)->GetDstRegs());
                    SetOneEdge(i, ins.at(i)->GetDstLabel());
                }
        }

        //////////////////////////////////////////////////////

        void CFGraph::SetNodeUse(Node node, Registers regs)
        {
            for (auto oneReg : regs)
                uses.at(node).push_back(oneReg);
        }

        //////////////////////////////////////////////////////

        void CFGraph::SetNodeDef(Node node, Registers regs)
        {
            for (auto oneReg : regs)
                defs.at(node).push_back(oneReg);
        }

        //////////////////////////////////////////////////////

        void CFGraph::SetOneEdge(Node node, myLabel label)
        {
            if (label != nullptr)   //  a jumpable
            {
                int labelIndex = GetLabelIndex(label);
                assert (labelIndex != -1);
                graph.AddEdge(node, labelIndex);
            }

            //  uncondition jump cannot fall through
            if (IsAAIJmp(ins.at(node)))
                return;
            else    //  fall through to the next instruction if exists
                if (node != (ins.size() - 1))
                    graph.AddEdge(node, node + 1);
        }

        //////////////////////////////////////////////////////

        bool CFGraph::IsAAIJmp(shared_ptr<AAI> ptr) const
        {
            return ptr->ToString().find("jmp") != string::npos;

        }

        //////////////////////////////////////////////////////

        int CFGraph::GetLabelIndex(myLabel label)
        {
            assert (label != nullptr);

            for (int i = 0; i < ins.size(); ++i)
                if (ins.at(i)->GetDefLabel() == label)  return i;
            return -1;
        }

        //////////////////////////////////////////////////////

        DirectedGraph CFGraph::GetDirectedGraph() const
        {
            return graph;
        }

        //////////////////////////////////////////////////////

        Registers CFGraph::GetUses(Node node) const
        {
            assert (node >= 0 && node < uses.size());
            return uses.at(node);
        }

        //////////////////////////////////////////////////////

        Registers CFGraph::GetDefs(Node node) const
        {
            assert (node >= 0 && node < uses.size());
            return defs.at(node);
        }

        /////////////////////////////////////////////////////

        bool CFGraph::IsMoveIns(Node node) const
        {
            assert (node >= 0 && node < uses.size());
            return ins.at(node)->ToString().find("mov") != string::npos;
        }

    }
}