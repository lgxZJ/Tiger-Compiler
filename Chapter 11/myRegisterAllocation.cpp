#include "myRegisterAllocation.h"
#include "CXX_myFrame.h"

#include "AAI/myMovable.h"
#include "AAI/myComputable.h"

#include <algorithm>
#include <cassert>

using namespace std;
using namespace lgxZJ::IS;
using namespace lgxZJ::LA;

namespace lgxZJ
{
    namespace RA
    {
        /////////////////////////////////////////////////////////////////////////
        //                         Register Allocation

        RegisterAllocation::RegisterAllocation(Instructions instructions, Trans_myLevel level)
         : cfGraph(instructions), interferenceGraph(&cfGraph)
        {
            this->level = level;
            RealWork(instructions);
        }

        void RegisterAllocation::RealWork(Instructions instructions)
        {
            InitInformation(instructions);
            Build();
            
            while ( !lowDegreeNonMoveNodes.empty() ||
                    !lowDegreeMoveNodes.empty() ||
                    !unconstrainedMovePairs.empty() ||
                    HaveHighDegreeNodes() )
            {
                if ( !lowDegreeNonMoveNodes.empty() )
                    Simplify();
                else if ( !unconstrainedMovePairs.empty() )
                    Coalesce();
                else if ( !lowDegreeMoveNodes.empty() )
                    Freeze();
                else if ( HaveHighDegreeNodes() )
                    Spill();
            }

            AssignColors();
            if ( !spilledNodes.empty() )
                Rewrite(), RealWork(ins);
        }

        void RegisterAllocation::InitInformation(Instructions instructions)
        {
            lowDegreeMoveNodes.clear();
            lowDegreeNonMoveNodes.clear();
            highDegreeMoveNodes.clear();
            highDegreeNonMoveNodes.clear();

            stack<Node> temp;
            selectStack.swap( temp );

            nodeDegrees.clear();
            coloredNodes.clear();
            nodeColors.clear();
            spilledNodes.clear();
            spilledMemorys.clear();
            adjacentNodes.clear();

            unconstrainedMovePairs.clear();
            constrainedMovePairs.clear();
            deletedMovePairs.clear();
            nodeAlias.clear();

            cfGraph = CFGraph(instructions);
            Liveness liveness(cfGraph);

            ins = instructions;
            interferenceGraph = liveness.GetInterferenceGraph();
            unconstrainedMovePairs = liveness.GetMovePairs();
        }

        bool RegisterAllocation::HaveHighDegreeNodes()
        {
            return !highDegreeMoveNodes.empty() || !highDegreeNonMoveNodes.empty();
        }

        /////////////////////////////////////////////////////////////////////////

        void RegisterAllocation::Build()
        {
            auto allNodes = interferenceGraph.GetDGRef().GetNodes();

            SaveDegreeInformation(allNodes);
            SaveAdjacentNodes(allNodes);
            ClassifyIntoDifferentSets(allNodes);
        }

        void RegisterAllocation::SaveDegreeInformation(NodeSet& nodes)
        {
            DirectedGraph& directedGraph = interferenceGraph.GetDGRef();
            for (auto oneNode : nodes)
            {
                nodeDegrees[oneNode] = 
                    directedGraph.GetNodeDegree(oneNode);
            }
        }

        void RegisterAllocation::SaveAdjacentNodes(NodeSet& nodes)
        {
            for (auto oneNode : nodes)
            {
                //  TODO: test self-node
                auto preNodes = interferenceGraph.GetDGRef().GetPredecessors(oneNode);
                auto sucNodes = interferenceGraph.GetDGRef().GetSuccessors(oneNode);

                sort(preNodes.begin(), preNodes.end());
                sort(sucNodes.begin(), sucNodes.end());

                NodeSet mergedNodes;
                set_union(
                    preNodes.begin(), preNodes.end(), sucNodes.begin(), sucNodes.end(),
                    back_inserter(mergedNodes));
                
                NodeSet regNodes;
                for (auto node : mergedNodes)
                    regNodes.push_back(node);

                adjacentNodes[oneNode] = regNodes;
            }
        }

        void RegisterAllocation::ClassifyIntoDifferentSets(NodeSet& nodes)
        {
            DirectedGraph& directedGraph = interferenceGraph.GetDGRef();
            for (auto oneNode : nodes)
            {
                if (directedGraph.GetNodeDegree(oneNode) >= Frame_registerNumber)
                {
                    if (InMovePairs(oneNode))
                        highDegreeMoveNodes.push_back(oneNode);
                    else
                        highDegreeNonMoveNodes.push_back(oneNode);
                }
                else
                {
                    if (InMovePairs(oneNode))
                        lowDegreeMoveNodes.push_back(oneNode);
                    else
                        lowDegreeNonMoveNodes.push_back(oneNode);
                }
            }
        }

        bool RegisterAllocation::InMovePairs(Node node)
        {
            for (auto onePair : unconstrainedMovePairs)
            {
                if (onePair.GetFrom() == node || onePair.GetTo() == node)
                    return true;
            }
            for (auto onePair : constrainedMovePairs)
            {
                if (onePair.GetFrom() == node || onePair.GetTo() == node)
                    return true;
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////

        bool RegisterAllocation::Simplify()
        {
            //      we do not remove all nodes because interleave Simplify
            //  with coalescing eliminates most move instructions
            if (!lowDegreeNonMoveNodes.empty())
            {
                auto nodeReg = lowDegreeNonMoveNodes.back();

                selectStack.push(nodeReg);
                lowDegreeNonMoveNodes.pop_back();
                assert ( 1 == nodeDegrees.erase(nodeReg));

                for (auto adjacentNode : adjacentNodes[nodeReg])
                {
                    AdjustNodeCategoryInSimplify(adjacentNode);
                    if (nodeDegrees[adjacentNode] > 0)  --nodeDegrees[adjacentNode];
                }
                return true;
            }

            return false;
        }

        void RegisterAllocation::AdjustNodeCategoryInSimplify(Node node)
        {
            //  high-degree become low-degree
            if (nodeDegrees[node] == Frame_registerNumber)
            {
                if (InMovePairs(node))
                    RemoveNodeFromSet( highDegreeMoveNodes, node );
                else
                    RemoveNodeFromSet( highDegreeNonMoveNodes, node );

                if (InMovePairs(node))
                    lowDegreeMoveNodes.push_back(node);
                else
                    lowDegreeNonMoveNodes.push_back(node);
            }
        }

        ////////////////////////////////////////////////////////////////////////////

        bool RegisterAllocation::Coalesce()
        {
            //  choose one to coalesce
            while ( !unconstrainedMovePairs.empty() )
            {
                NodeSet commomNeighbors;
                unsigned newNeighborCount = 0;
                bool isNeighborEachOther = false;

                Edge edge = unconstrainedMovePairs.back();
                unconstrainedMovePairs.pop_back();
                
                if (IsCoalescable(
                    edge, &commomNeighbors, &newNeighborCount, &isNeighborEachOther))
                {
                    DoCoalesce(
                        edge, commomNeighbors, newNeighborCount, isNeighborEachOther);
                    return true;
                }
                else
                    constrainedMovePairs.push_back(edge);
                /*else
                    //  if no longer "moved", move it to non-move
                    AdjustNodeCategoryNotCoalesced(edge.GetFrom()),
                    AdjustNodeCategoryNotCoalesced(edge.GetTo());*/
            }
            /*
            for (auto moveIter = movePairs.begin(); moveIter != movePairs.end(); ++moveIter)
            {
                NodeSet commomNeighbors;
                unsigned newNeighborCount = 0;
                bool isNeighborEachOther = false;

                if (IsCoalescable(
                    *moveIter, &commomNeighbors, &newNeighborCount, &isNeighborEachOther))
                {
                    movePairs.erase(moveIter);
                    DoCoalesce(
                        *moveIter, commomNeighbors, newNeighborCount, isNeighborEachOther);
                    return true;
                }
            }*/

            return false;
        }

        bool RegisterAllocation::IsCoalescable(
            Edge edge, NodeSet* commomNodes, unsigned* newNeighborCount, bool* isNeighborEachOther)
        {
            if (interferenceGraph.EdgesContains(edge.GetFrom(), edge.GetTo()))
                return false;

            //  use Briggs coalesce algorithm to judge if two nodes
            //  are coalescable
            NodeSet fromNeighbors;
            GetNodeNeighbors(edge.GetFrom(), &fromNeighbors);

            NodeSet toNeighbors;
            GetNodeNeighbors(edge.GetTo(), &toNeighbors);

            commomNodes->clear();
            *newNeighborCount = 0;
            for (auto oneNode : fromNeighbors)
            {
                if ( !IsSimplified(oneNode) )
                {
                    *newNeighborCount = *newNeighborCount + 1;

                    if (oneNode == edge.GetTo())
                        *isNeighborEachOther = true;

                    for (auto anotherNode : toNeighbors)
                    {
                        if ( !IsSimplified(anotherNode) )
                            *newNeighborCount = *newNeighborCount + 1;
                        if (oneNode == anotherNode)
                            commomNodes->push_back(oneNode);
                    }
                }
            }

            *newNeighborCount = *newNeighborCount - commomNodes->size();
            return *newNeighborCount < Frame_registerNumber;
        }

        void RegisterAllocation::DoCoalesce(
            Edge edge, NodeSet& commomNodes, unsigned newNeighborCount, bool isNeighborEachOther)
        {
            auto nodeFrom = edge.GetFrom(), nodeTo = edge.GetTo();

            deletedMovePairs.push_back(edge);

            nodeAlias[nodeFrom].push_back(nodeTo);
            nodeAlias[nodeTo].push_back(nodeFrom);

            AdjustNodeCategoryInCoalesce(edge.GetFrom(), edge);
            AdjustNodeCategoryInCoalesce(edge.GetTo(), edge);
            AdjustAdjacentDegrees(edge, commomNodes, newNeighborCount, isNeighborEachOther);
        }

        void RegisterAllocation::GetNodeNeighbors(Node node, NodeSet* nodes)
        {
            auto dgRef = interferenceGraph.GetDGRef();

            auto fromPreNodes = dgRef.GetPredecessors(node);
            auto fromSucNodes = dgRef.GetSuccessors(node);
            merge(  fromPreNodes.begin(), fromPreNodes.end(), 
                    fromSucNodes.begin(), fromSucNodes.end(),
                    back_inserter(*nodes)  );
        }

        bool RegisterAllocation::IsSimplified(Node node)
        {
            auto tempStack(selectStack);
            while ( !tempStack.empty() )
            {
                if ( tempStack.top() == node )
                    return true;
                tempStack.pop();
            }
            
            return false;
        }

        void RegisterAllocation::AdjustAdjacentDegrees(
            Edge edge, NodeSet& commomNodes, unsigned newNeighborCount, bool isNeighborEachOther)
        {
            nodeDegrees[edge.GetFrom()] = newNeighborCount;
            nodeDegrees[edge.GetTo()] = newNeighborCount;

            for (auto oneNode : commomNodes)
                --nodeDegrees[oneNode];
        }

        void RegisterAllocation::AdjustNodeCategoryInCoalesce(Node node, Edge edge)
        {
            unsigned oldDegree = nodeDegrees[node];

            if (oldDegree >= Frame_registerNumber)
            {
                RemoveNodeFromSet( highDegreeMoveNodes, node );

                //  high-move -> low-move
                if (InMovePairs(edge.GetFrom()) || InMovePairs(edge.GetTo()))
                    lowDegreeMoveNodes.push_back(node);
                else    //   high-move -> low-non-move
                    lowDegreeNonMoveNodes.push_back(node);
            }
            else
            {
                //  low-move -> low-non-move
                if ( !(InMovePairs(edge.GetFrom()) || InMovePairs(edge.GetTo())) )
                    RemoveNodeFromSet( lowDegreeMoveNodes, node ),
                    lowDegreeNonMoveNodes.push_back(node);
            }
        }

        void RegisterAllocation::RemoveNodeFromSet(NodeSet& nodes, Node node)
        {
            auto nodePos = find( nodes.begin(), nodes.end(), node );

            if ( nodePos != nodes.end() );
                nodes.erase ( nodePos );
        }

        void RegisterAllocation::AdjustNodeCategoryNotCoalesced(Node node)
        {
            //  after this move-pair removed
            unsigned oldDegree = nodeDegrees[node];

            if (oldDegree >= Frame_registerNumber)
            {
                //   high-move -> high-non-move
                if (!InMovePairs(node))
                    RemoveNodeFromSet(highDegreeMoveNodes, node),
                    highDegreeNonMoveNodes.push_back(node);
            }
            else
            {
                //  low-move -> low-non-move
                if ( !InMovePairs(node) )
                    RemoveNodeFromSet( lowDegreeMoveNodes, node ),
                    lowDegreeNonMoveNodes.push_back(node);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////

        bool RegisterAllocation::Freeze()
        {
            if (lowDegreeMoveNodes.empty())
                return false;
            else
            {
                //  freeze all moves in which this node is related
                Node nodeToFreeze = lowDegreeMoveNodes.back();
                MoveFreezedNodeIntoLowDegreeNonMoveSet(nodeToFreeze);
                UpdateMoveRelatedNodes(nodeToFreeze);
                return true;
            }
        }

        void RegisterAllocation::MoveFreezedNodeIntoLowDegreeNonMoveSet(LA::Node node)
        {
            lowDegreeMoveNodes.pop_back();
            lowDegreeNonMoveNodes.push_back(node);
        }

       void RegisterAllocation::UpdateMoveRelatedNodes(Node freezedNode)
       {
            NodeSet possibleChangedNonMoveNodes;
            UpdateOnePairSet(
                unconstrainedMovePairs, freezedNode, &possibleChangedNonMoveNodes);
            UpdateOnePairSet(
                constrainedMovePairs, freezedNode, &possibleChangedNonMoveNodes);

            for (auto oneNode : possibleChangedNonMoveNodes)
                if (!InMovePairs(oneNode))
                {
                    if (InNodeSet(oneNode, highDegreeMoveNodes))
                        MoveHighDegreeNodeFromMoveToNonMove(oneNode);
                    if (InNodeSet(oneNode, lowDegreeMoveNodes))
                        MoveLowDegreeNodeFromMoveToNonMove(oneNode);
                }
       }

        void RegisterAllocation::UpdateOnePairSet(MovePairs& pairs, Node nodeToFreeze, NodeSet* possibleChangedNonMoveNodes)
        {
                MovePairs newMovePairs;
                for (auto onePair : pairs)
                {
                    if (onePair.GetFrom() == nodeToFreeze)
                        possibleChangedNonMoveNodes->push_back(onePair.GetTo());
                    else if (onePair.GetTo() == nodeToFreeze)
                        possibleChangedNonMoveNodes->push_back(onePair.GetFrom());
                    else
                        newMovePairs.push_back(onePair);
                }

                pairs.swap(newMovePairs);
        }

        bool RegisterAllocation::InNodeSet(Node node, NodeSet& nodeSet)
        {
            for (auto oneNode : nodeSet)
                if (oneNode == node)    return true;

            return false;
        }

        void RegisterAllocation::MoveHighDegreeNodeFromMoveToNonMove(Node node)
        {
            RemoveNodeFromSet( highDegreeMoveNodes, node );
            highDegreeNonMoveNodes.push_back(node);
        }

        void RegisterAllocation::MoveLowDegreeNodeFromMoveToNonMove(Node node)
        {
            RemoveNodeFromSet( lowDegreeMoveNodes, node );
            lowDegreeNonMoveNodes.push_back(node);
        }

        ///////////////////////////////////////////////////////////////////////////////////

        void RegisterAllocation::Spill()
        {
            NodeSet allHighNodes;
            merge(  highDegreeMoveNodes.begin(), highDegreeMoveNodes.end(),
                    highDegreeNonMoveNodes.begin(), highDegreeNonMoveNodes.end(),
                    back_inserter(allHighNodes) );

            if (!allHighNodes.empty())
            {
                Node node = SelectSpillNode(allHighNodes);

                if (InNodeSet(node, highDegreeMoveNodes))
                {
                    RemoveNodeFromSet( highDegreeMoveNodes, node );
                    UpdateMoveRelatedNodes(node);
                }
                else
                {
                    RemoveNodeFromSet( highDegreeNonMoveNodes, node );
                }

                lowDegreeNonMoveNodes.push_back(node);
            }
        }

        Node RegisterAllocation::SelectSpillNode(NodeSet nodes)
        {
            //  the lowest priority node is the one that interferences with
            //  manys but used rarely.
            Node result = nodes[0];
            float lowestSpillPriority = 
                    (   cfGraph.GetUses(result).size() +
                        cfGraph.GetDefs(result).size() ) /
                        interferenceGraph.GetDGRef().GetNodeDegree(result);

            for (auto oneNode : nodes)
            {
                float priority = (  cfGraph.GetUses(oneNode).size() +
                                    cfGraph.GetDefs(oneNode).size() ) /
                                    interferenceGraph.GetDGRef().GetNodeDegree(oneNode);
                if (priority < lowestSpillPriority)
                    lowestSpillPriority = priority,
                    result = oneNode;
            }
            return result;
        }

        /////////////////////////////////////////////////////////////////////////////////

        void RegisterAllocation::AssignColors()
        {
            while (!selectStack.empty())
            {
                Node node = selectStack.top();
                selectStack.pop();

                if (IsPrecoloredNode(node))
                    AssignColorToPrecolored(node);
                else
                    AssignColorToNonPrecolored(node); 
            }

            AssignColorsOnCoalescedNodes();
        }

        void RegisterAllocation::AssignColorToPrecolored(Node node)
        {
            coloredNodes.push_back(node);
            nodeColors[node] = interferenceGraph.GetNodeReg(node);
        }

        void RegisterAllocation::AssignColorToNonPrecolored(Node node)
        {
            Registers colors = GetAvailableColors();
            for (auto oneNode : adjacentNodes[node])
            {
                if (InNodeSet(oneNode, coloredNodes) || IsPrecoloredNode(oneNode))
                    RemoveColorFromColors(colors, nodeColors[oneNode]);
            }

            if (colors.empty())
                spilledNodes.push_back(node);
            else
            {
                coloredNodes.push_back(node);
                nodeColors[node] = colors.front();
            }
        }

        Registers RegisterAllocation::GetAvailableColors()
        {
            //  no include stack pointer
            return Registers {
                Frame_EAX(), Frame_EBX(), Frame_ECX(), Frame_EDX(),
                Frame_ESI(), Frame_EDI(), Frame_EBP(),
            };
        }

        bool RegisterAllocation::IsPrecoloredNode(Node node)
        {
            Register reg = interferenceGraph.GetNodeReg(node);

            assert (reg != Frame_ESP());
            
            if (reg == Frame_EAX() ||
                reg == Frame_EBX() ||
                reg == Frame_ECX() ||
                reg == Frame_EDX() ||
                reg == Frame_ESI() ||
                reg == Frame_EDI() ||
                reg == Frame_EBP())

                return true;
            else
                return false;
        }

        void RegisterAllocation::RemoveColorFromColors(IS::Registers& colors, Register color)
        {
            auto colorPos = find ( colors.begin(), colors.end(), color );

            if ( colorPos != colors.end() )
                colors.erase( colorPos );
        }

        void RegisterAllocation::AssignColorsOnCoalescedNodes()
        {
            for (auto onePair : deletedMovePairs)
            {
                if (InNodeSet(onePair.GetFrom(), coloredNodes))
                    nodeColors[onePair.GetTo()] = nodeColors[onePair.GetFrom()];
                else
                    nodeColors[onePair.GetFrom()] = nodeColors[onePair.GetTo()];
            }
        }

        ///////////////////////////////////////////////////////////////////////////

        void RegisterAllocation::Rewrite()
        {
            AllocateMemoryForSpilledRegs();
            
            Instructions newIns;
            for (auto it = ins.begin(); it != ins.end(); ++it)
            {
                Registers useRegs = (*it)->GetDstRegs();
                Registers defRegs = (*it)->GetSrcRegs();

                if (useRegs.empty() && defRegs.empty())
                {
                    newIns.push_back(*it);
                    continue;
                }
                if ( !defRegs.empty() )
                    RewriteDefs(defRegs, *it, &newIns);
                if ( !useRegs.empty() )
                    RewriteUses(useRegs, *it, &newIns);
            }
        }

        void RegisterAllocation::AllocateMemoryForSpilledRegs()
        {
            for (auto oneNode : spilledNodes)
                spilledMemorys[oneNode] = Frame_allocateLocal(
                    Trans_getFrame(level), true);
        }

        void RegisterAllocation::RewriteDefs(Registers& defRegs, shared_ptr<AAI> oneIns, Instructions* newIns)
        {
            for (auto oneReg : defRegs)
            {
                if ( RegSpilled(oneReg) )
                {
                    //  replace spilled register
                    Register newReg = Temp_newTemp();

                    oneIns->ReplaceReg(oneReg, newReg);
                    newIns->push_back(oneIns);

                    Register memoryAddr = CalculateNodeMemoryAddr(newIns, oneReg);
                    //  store new register value to memory
                    newIns->push_back(make_shared<Move>(
                        memoryAddr, newReg, Move::OperandType::Memory, Move::OperandType::Content));
                }
            }
        }

        IS::Register RegisterAllocation::CalculateNodeMemoryAddr(Instructions* newIns, Register reg)
        {
            //  calculate memory address
            Register memoryAddr = Temp_newTemp();
            newIns->push_back(make_shared<Move>(memoryAddr, Frame_FP()));
            newIns->push_back(make_shared<Sub>(
                memoryAddr,
                Frame_getAccessOffset(spilledMemorys[interferenceGraph.GetRegNode(reg)])
                    * Frame_wordSize));
            
            return memoryAddr;
        }

        bool RegisterAllocation::RegSpilled(IS::Register reg)
        {
            assert (reg != nullptr);
            return InNodeSet(interferenceGraph.GetRegNode(reg), spilledNodes);
        }

        void RegisterAllocation::RewriteUses(
            Registers& useRegs, shared_ptr<AAI> oneIns, Instructions* newIns)
        {
            for (auto oneReg : useRegs)
            {
                if ( RegSpilled(oneReg) )
                {
                    Register newReg = Temp_newTemp();
                    Register memoryAddr = CalculateNodeMemoryAddr(newIns, oneReg);

                    //  fetch memory value to new register
                    newIns->push_back(make_shared<Move>(
                        newReg, memoryAddr,
                        Move::OperandType::Content, Move::OperandType::Memory));

                    oneIns->ReplaceReg(oneReg, newReg);
                    newIns->push_back(oneIns);
                }
            }
        }

        /////////////////////////////////////////////////////////////////////////////

        RegisterMaps RegisterAllocation::GetRegisterMaps()
        {
            RegisterMaps maps;
            for (auto it = nodeColors.begin(); it != nodeColors.end(); ++it)
            {
                maps[interferenceGraph.GetNodeReg(it->first)] = it->second;
            }

            return maps;
        }
    }
}