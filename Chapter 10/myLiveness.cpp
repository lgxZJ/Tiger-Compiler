#include "myLiveness.h"
#include "CXX_myTemp.h"

#include <cassert>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace lgxZJ::IS;

namespace lgxZJ
{
    namespace LA
    {
        //////////////////////////////////////////////////////////////////////////
        //                          InterferenceGraph
        //////////////////////////////////////////////////////////////////////////

        InterferenceGraph::InterferenceGraph() : graph(), registers()
        {}

        InterferenceGraph::InterferenceGraph(unsigned nodeSize)
            : graph(nodeSize), registers(nodeSize)
        {
            for (auto& oneReg : registers)
                oneReg = nullptr;
        }

        //////////////////////////////////////////////////////////////////////////

        DirectedGraph& InterferenceGraph::GetDGRef()
        {
            return graph;
        }

        //////////////////////////////////////////////////////////////////////////

        void InterferenceGraph::AddEdge(Node lhs, Node rhs)
        {
            if (graph.IsEdgeInside(lhs, rhs) || graph.IsEdgeInside(rhs, lhs))
                return;
            if (lhs == rhs)
                return;
            
            graph.AddEdge(lhs, rhs);
        }

        ///////////////////////////////////////////////////////////////////////////

        void InterferenceGraph::AddEdge(myTemp lhs, myTemp rhs)
        {
            assert (lhs != nullptr && rhs != nullptr);

            Node lhsNode = GetRegNode(lhs);
            Node rhsNode = GetRegNode(rhs);

            SetNodeReg(lhsNode, lhs);
            SetNodeReg(rhsNode, rhs);

            AddEdge(lhsNode, rhsNode);
        }

        //////////////////////////////////////////////////////////////////////////

        int InterferenceGraph::GetRegNode(myTemp reg) const
        {
            assert (reg != nullptr);
            Node node = Temp_getTempNum(reg) - 1;

            assert (node >= 0 && node < registers.size());
            return node;
        }

        //////////////////////////////////////////////////////////////////////////

        int InterferenceGraph::GetNodeSize() const
        {
            assert (graph.GetNodes().size() == registers.size());
            return registers.size();
        }

        //////////////////////////////////////////////////////////////////////////

        myTemp InterferenceGraph::GetNodeReg(Node node) const
        {
            assert (node >= 0 && node < registers.size());
            return registers.at(node);
        }

        /////////////////////////////////////////////////////////////////////////

        void InterferenceGraph::SetNodeReg(Node node, myTemp reg)
        {
            assert (node >= 0 && node < registers.size());
            assert (reg != nullptr);
            registers.at(node) = reg;
        }

        /////////////////////////////////////////////////////////////////////////

        bool InterferenceGraph::EdgesContains(Node lhs, Node rhs) const
        {
            assert (graph.IsNodeInside(lhs) && graph.IsNodeInside(rhs));

            if (graph.IsEdgeInside(lhs, rhs) || graph.IsEdgeInside(rhs, lhs))
                return true;
            else
                return false;
        }

        /////////////////////////////////////////////////////////////////////////

        bool InterferenceGraph::EdgesContains(myTemp lhs, myTemp rhs) const
        {
            assert (lhs != nullptr && rhs != nullptr);

            Node lhsNode = Temp_getTempNum(lhs) - 1;
            Node rhsNode = Temp_getTempNum(rhs) - 1;
            assert (graph.IsNodeInside(lhsNode) && graph.IsNodeInside(rhsNode));

            return EdgesContains(lhsNode, rhsNode);
        }

        /////////////////////////////////////////////////////////////////////////
        //                              Liveness
        /////////////////////////////////////////////////////////////////////////

        Liveness::Liveness(const CFGraph& oneCFGraph)
            :   cfGraph(oneCFGraph),
                interferenceGraph(Temp_getTempNum(Temp_newTemp()) - 1)
        {
            DoLiveness();
            GenerateInterferenceGraph();
        }

        //////////////////////////////////////////////////////////////////////////

        void Liveness::DoLiveness()
        {
            InitInAndOut();
            RegistersSet inCopy, outCopy;

            do
            {
                inCopy = in, outCopy = out;
                for (int i = in.size() - 1; i >= 0; --i)
                {
                    CalculateOneOut(i);
                    CalculateOneIn(i);
                }

            } while (InAndOutChanged(inCopy, in, outCopy, out));
        }

        void Liveness::InitInAndOut()
        {
            int size = cfGraph.GetDirectedGraph().GetNodes().size();
            in.resize(size);
            out.resize(size);
        }

        void Liveness::CalculateOneOut(Node node)
        {
            for (auto successorNode : cfGraph.GetDirectedGraph().GetSuccessors(node))
            {
                Registers& oneOut = out.at(node);
                Registers& inOfOneSuccessor = in.at(successorNode);

                SortOneRegisters(oneOut);
                SortOneRegisters(inOfOneSuccessor);
                out.at(node) = GetSetUnion(oneOut, inOfOneSuccessor);
            }
        }

        void Liveness::CalculateOneIn(Node node)
        {
            Registers oneOut = out.at(node);
            Registers oneDef = cfGraph.GetDefs(node);

            SortOneRegisters(oneOut);
            SortOneRegisters(oneDef);
            Registers outMinusDef = GetSetDiff(oneOut, oneDef);
            
            Registers oneUse = cfGraph.GetUses(node);
            SortOneRegisters(oneUse);
            SortOneRegisters(outMinusDef);
            in.at(node) = GetSetUnion(oneUse, outMinusDef);
        }

        void Liveness::SortOneRegisters(Registers& regs)
        {
            struct myTempComp
            {
                bool operator () (myTemp lhs, myTemp rhs)
                { return Temp_getTempNum(lhs) < Temp_getTempNum(rhs); }
            } tempComparator;

            sort(regs.begin(), regs.end(), tempComparator);
        }

        Registers Liveness::GetSetUnion(Registers& lhs, Registers& rhs)
        {
            struct myTempComp
            {
                bool operator () (myTemp lhs, myTemp rhs)
                { return Temp_getTempNum(lhs) < Temp_getTempNum(rhs); }
            } tempComparator;

            Registers result;
            set_union(
                lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                back_inserter(result),
                tempComparator);
            return result;
        }

        Registers Liveness::GetSetDiff(Registers& lhs, Registers& rhs)
        {
            struct myTempComp
            {
                bool operator () (myTemp lhs, myTemp rhs)
                { return Temp_getTempNum(lhs) < Temp_getTempNum(rhs); }
            } tempComparator;

            Registers result;
            set_difference(
                lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                back_inserter(result),
                tempComparator);
            return result;
        }

        bool Liveness::InAndOutChanged(
            RegistersSet& oldIn, RegistersSet& newIn,
            RegistersSet& oldOut, RegistersSet& newOut) const
        {
            assert (oldIn.size() == newIn.size() &&
                    oldOut.size() == newOut.size());

            for (int i = 0; i < oldIn.size(); ++i)
                if (oldIn.at(i).size() != newIn.at(i).size())
                    return true;
            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////

        void Liveness::GenerateInterferenceGraph()
        {
            for (auto node : cfGraph.GetDirectedGraph().GetNodes())
            {
                //  there may be one or zero `def` registers, use for loop to make it
                //  consistent.
                for (auto defReg : cfGraph.GetDefs(node))
                {
                    bool isMoveIns = cfGraph.IsMoveIns(node);

                    Node defNode = GetRegisterNode(defReg);
                    interferenceGraph.SetNodeReg(defNode, defReg);

                    Registers srcRegs = cfGraph.GetNodeIns(node)->GetSrcRegs();
                    for (auto oneLiveOutReg : out.at(node))
                    {
                        //  there is only one source register in move instruction
                        if (isMoveIns && SrcRegisterIs(srcRegs, oneLiveOutReg))
                            continue;

                        Node oneLiveOutNode = GetRegisterNode(oneLiveOutReg);
                        interferenceGraph.SetNodeReg(oneLiveOutNode, oneLiveOutReg);
                        interferenceGraph.AddEdge(defNode, oneLiveOutNode);
                    }
                }
            }
        }

        Node Liveness::GetRegisterNode(myTemp temp) const
        {
            assert(temp != nullptr);
            return Temp_getTempNum(temp) - 1;
        }

        bool Liveness::SrcRegisterIs(const Registers& regs, myTemp reg) const
        {
            assert (reg != nullptr);
            assert (regs.size() >= 0 && regs.size() <= 1);

            if (regs.size() == 1)   return regs.at(0) == reg;
            else                    return false;
        }

        ////////////////////////////////////////////////////////////////////////////

        InterferenceGraph Liveness::GetInterferenceGraph() const
        {
            return interferenceGraph;
        }
    }
}