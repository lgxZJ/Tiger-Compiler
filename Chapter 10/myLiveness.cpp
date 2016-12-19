#include "myLiveness.h"
#include "CXX_myTemp.h"

#include <cassert>
#include <algorithm>
#include <iterator>

using namespace std;

namespace lgxZJ
{
    namespace LA
    {
        //////////////////////////////////////////////////////////////////////////
        //                          InterferenceGraph
        //////////////////////////////////////////////////////////////////////////

        InterferenceGraph::InterferenceGraph() : graph(), registers()
        {}

        InterferenceGraph::InterferenceGraph(unsigned n) : graph(n), registers(n)
        {}

        //////////////////////////////////////////////////////////////////////////

        DirectedGraph& InterferenceGraph::GetDGRef()
        {
            return graph;
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
        //                              Liveness
        /////////////////////////////////////////////////////////////////////////

        Liveness::Liveness(CFGraph oneCFGraph) : cfGraph(oneCFGraph)
        {
            int size = cfGraph.GetDirectedGraph().GetNodes().size();
            in.resize(size);
            out.resize(size);

            CalculateInOut();
            GenerateInterferenceGraph();
        }

        //////////////////////////////////////////////////////////////////////////

        void Liveness::CalculateInOut()
        {
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

        void Liveness::CalculateOneOut(Node node)
        {
            for (auto successorNode : cfGraph.GetDirectedGraph().GetSuccessors(node))
            {
                IS::Registers& oneOut = out.at(node);
                IS::Registers& inOfOneSuccessor = in.at(successorNode);

                SortOneRegisters(oneOut);
                SortOneRegisters(inOfOneSuccessor);
                out.at(node) = GetSetUnion(oneOut, inOfOneSuccessor);
            }

            int bb = out.at(node).size();
                int cc = bb + 1;
        }

        void Liveness::CalculateOneIn(Node node)
        {
            int aa = out.at(node).size();

            IS::Registers& oneOut = out.at(node);
            IS::Registers oneDef = cfGraph.GetDefs(node);

            SortOneRegisters(oneOut);
            SortOneRegisters(oneDef);
            IS::Registers outMinusDef = GetSetDiff(oneOut, oneDef);int minusSize = outMinusDef.size();
            
            IS::Registers oneUse = cfGraph.GetUses(node);int useSize = oneUse.size();
            SortOneRegisters(oneUse);
            SortOneRegisters(outMinusDef);
            in.at(node) = GetSetUnion(oneUse, outMinusDef);

            int bb = in.at(node).size();
                int cc = bb + 1;
        }

        void Liveness::SortOneRegisters(IS::Registers& regs)
        {
            struct myTempComp
            {
                bool operator () (myTemp lhs, myTemp rhs)
                { return Temp_getTempNum(lhs) < Temp_getTempNum(rhs); }
            } tempComparator;

            sort(regs.begin(), regs.end(), tempComparator);
        }

        IS::Registers Liveness::GetSetUnion(IS::Registers& lhs, IS::Registers& rhs)
        {
            IS::Registers result;
            set_union(
                lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                back_inserter(result));
            return result;
        }

        IS::Registers Liveness::GetSetDiff(IS::Registers& lhs, IS::Registers& rhs)
        {
            IS::Registers result;
            set_difference(
                lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                back_inserter(result));
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
            
        }
    }
}