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

        InterferenceGraph::InterferenceGraph(CFGraph cfGraph)
            :   graph(cfGraph.GetRegisterNodeMapRef().size()),
                registers(cfGraph.GetRegisterNodeMapRef().size()),
                cfGraph(cfGraph)
        {
            for (auto oneMap : cfGraph.GetRegisterNodeMapRef())
                registers.at(oneMap.second) = oneMap.first;
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
            return cfGraph.GetRegisterNodeMapRef().at(reg);
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

            Node lhsNode = GetRegNode(lhs);
            Node rhsNode = GetRegNode(rhs);
            assert (graph.IsNodeInside(lhsNode) && graph.IsNodeInside(rhsNode));

            return EdgesContains(lhsNode, rhsNode);
        }

        /////////////////////////////////////////////////////////////////////////
        //                              Liveness
        /////////////////////////////////////////////////////////////////////////

        Liveness::Liveness(CFGraph oneCFGraph)
            :   cfGraph(oneCFGraph),
                movePairs(),
                interferenceGraph(cfGraph)
        {
            DoLiveness();
            GenerateInterferenceGraph();
            GenerateMovePairs();
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
                        Node oneLiveOutNode = GetRegisterNode(oneLiveOutReg);
                        interferenceGraph.SetNodeReg(oneLiveOutNode, oneLiveOutReg);

                        //  there is only one source register in move instruction
                        if (isMoveIns && SrcRegisterIs(srcRegs, oneLiveOutReg))
                            continue;

                        interferenceGraph.AddEdge(defNode, oneLiveOutNode);
                    }
                }
            }
        }

        Node Liveness::GetRegisterNode(myTemp temp) const
        {
            assert(temp != nullptr);
            return cfGraph.GetRegisterNodeMapRef().at(temp);
        }

        bool Liveness::SrcRegisterIs(const Registers& regs, myTemp reg) const
        {
            assert (reg != nullptr);
            assert (regs.size() >= 0 && regs.size() <= 1);

            if (regs.size() == 1)   return regs.at(0) == reg;
            else                    return false;
        }

        ////////////////////////////////////////////////////////////////////////////

        void Liveness::GenerateMovePairs()
        {
            for (auto oneNode : cfGraph.GetDirectedGraph().GetNodes())
            {
                Registers leftOperand = cfGraph.GetNodeIns(oneNode)->GetDstRegs();
                Registers rightOperand = cfGraph.GetNodeIns(oneNode)->GetSrcRegs();

                if (IsTwoContentRegOperandMove(oneNode, leftOperand, rightOperand) &&
                    !IsSelfMove(leftOperand, rightOperand))
                {
                    assert (leftOperand.at(0) != nullptr && rightOperand.at(0) != nullptr);

                    Node leftNode = cfGraph.GetRegisterNodeMapRef().at(leftOperand.at(0));
                    Node rightNode = cfGraph.GetRegisterNodeMapRef().at(rightOperand.at(0));

                    if(!MovePairContains(leftNode, rightNode))
                        movePairs.push_back(Edge(leftNode, rightNode));
                }
            }
        }

        //  NOTE:
        //      This function also recognize conditions when either of the destination
        //      register or the source register is used as a memory location:
        //          mov [reg], reg | mov [reg], [reg] | mov reg, [reg]
        //      On these conditions, the move instructions are not treated as move pairs.
        bool Liveness::IsTwoContentRegOperandMove(
            Node node, Registers& leftOperand, Registers& rightOperand) const
        {
            if (!cfGraph.IsMoveIns(node) ||
                leftOperand.size() != 1 || rightOperand.size() != 1)
                return false;

            //  check memory operand type from "()" characters
            string insStr = cfGraph.GetNodeIns(node)->ToString();
            return insStr.find('(') == string::npos;
        }

        bool Liveness::IsSelfMove(Registers& leftOperand, Registers& rightOperand) const
        {
            assert (leftOperand.size() == 1 && rightOperand.size() == 1);
            return leftOperand.at(0) == rightOperand.at(0);
        }

        bool Liveness::MovePairContains(Node lhsNode, Node rhsNode) const
        {
            for (int i = 0; i < movePairs.size(); ++i)
                if ((movePairs.at(i).GetFrom() == lhsNode && movePairs.at(i).GetTo() == rhsNode) ||
                    (movePairs.at(i).GetFrom() == rhsNode && movePairs.at(i).GetTo() == lhsNode))
                    return true;

            return false;
        }

        ////////////////////////////////////////////////////////////////////////////

        InterferenceGraph Liveness::GetInterferenceGraph() const
        {
            return interferenceGraph;
        }

        ////////////////////////////////////////////////////////////////////////////

        MovePairs Liveness::GetMovePairs() const
        {
            return movePairs;
        }

    }
}
