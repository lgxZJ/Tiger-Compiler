#include "myRegisterAllocation.h"
#include "CXX_myFrame.h"

#include "AAI/myMovable.h"
#include "AAI/myComputable.h"
#include "AAI/myControlable.h"

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

        RegisterAllocation::RegisterAllocation(
            Instructions instructions, Trans_myLevel level, unsigned regNum)
         : cfGraph(instructions), interferenceGraph(cfGraph), regNum(regNum)
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
                if (directedGraph.GetNodeDegree(oneNode) >= regNum)
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
            if (nodeDegrees[node] == regNum)
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
            }
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
            return *newNeighborCount < regNum;
        }

        void RegisterAllocation::DoCoalesce(
            Edge edge, NodeSet& commomNodes, unsigned newNeighborCount, bool isNeighborEachOther)
        {
            auto nodeFrom = edge.GetFrom(), nodeTo = edge.GetTo();
            deletedMovePairs.push_back(edge);

            Node newNode = nodeFrom, removeNode = nodeTo;
            AnalyzeAlias(&newNode, &removeNode, edge);

            RemoveNodeFromSet(lowDegreeMoveNodes, removeNode);
            RemoveNodeFromSet(highDegreeMoveNodes, removeNode);
            AddAlias(newNode, removeNode);

            AdjustNodeCategoryInCoalesce(nodeFrom, edge);
            /*AdjustNodeCategoryInCoalesce(edge.GetTo(), edge);*/
            AdjustAdjacentDegrees(edge, commomNodes, newNeighborCount, isNeighborEachOther);
        }

        void RegisterAllocation::AnalyzeAlias(Node* newNode, Node* removeNode, Edge edge)
        {
            *newNode = edge.GetFrom();
            *removeNode = edge.GetTo();

            Node fromAlias = GetNodeAlias(edge.GetFrom());
            Node toAlias = GetNodeAlias(edge.GetTo());

            if (fromAlias != -1)
                *newNode = fromAlias;
            else if (toAlias != -1)
                *removeNode = *newNode, *newNode = toAlias;
        }

        Node RegisterAllocation::GetNodeAlias(Node node)
        {
            for (auto oneAlias : nodeAlias)
            {
                if (oneAlias.first == node)
                    return oneAlias.first;

                for (auto oneAliasNode : oneAlias.second)
                    if (oneAliasNode == node)
                        return oneAlias.first;
            }
            return -1;
        }

        void RegisterAllocation::AddAlias(Node newNode, Node removeNode)
        {
            //  try to gather same alias into one set
            nodeAlias[newNode].push_back(removeNode);
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
            //  the to-node is removed, no need to change this degree

            for (auto oneNode : commomNodes)
                --nodeDegrees[oneNode];
        }

        void RegisterAllocation::AdjustNodeCategoryInCoalesce(Node node, Edge edge)
        {
            unsigned oldDegree = nodeDegrees[node];

            if (oldDegree >= regNum)
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
            RemoveNonAliasNodeFromSet(nodes, node);

            for (auto oneAlias : nodeAlias)
            {
                if (oneAlias.first == node)
                    for (auto oneNode : oneAlias.second)
                        RemoveNonAliasNodeFromSet(nodes, oneNode);
                if (find(oneAlias.second.begin(), oneAlias.second.end(), node)
                        != 
                    oneAlias.second.end())

                    RemoveNonAliasNodeFromSet(nodes, oneAlias.first);
            }
        }

        void RegisterAllocation::RemoveNonAliasNodeFromSet(NodeSet& nodes, Node node)
        {
            auto nodePos = find( nodes.begin(), nodes.end(), node );
            if ( nodePos != nodes.end() )
                nodes.erase ( nodePos );
        }

        void RegisterAllocation::AdjustNodeCategoryNotCoalesced(Node node)
        {
            //  after this move-pair removed
            unsigned oldDegree = nodeDegrees[node];

            if (oldDegree >= regNum)
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
                        (float)interferenceGraph.GetDGRef().GetNodeDegree(result);

            for (auto oneNode : nodes)
            {
                float priority = (  cfGraph.GetUses(oneNode).size() +
                                    cfGraph.GetDefs(oneNode).size() ) /
                                    (float)interferenceGraph.GetDGRef().GetNodeDegree(oneNode);
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

                AssignColorsOnCoalescedNodes(node);
            }
        }

        void RegisterAllocation::AssignColorToPrecolored(Node node)
        {
            coloredNodes.push_back(node);
            nodeColors[node] = interferenceGraph.GetNodeReg(node);
        }

        void RegisterAllocation::AssignColorToNonPrecolored(Node node)
        {
            Registers colors = GetAvailableColors();
            RemoveAdjacentNodeColors(&colors, adjacentNodes[node]);
            RemoveAliasAdjacentNodeColors(&colors, node);

            if (colors.empty())
                spilledNodes.push_back(node);
            else
            {
                coloredNodes.push_back(node);
                nodeColors[node] = colors.front();
            }
        }

        void RegisterAllocation::RemoveAdjacentNodeColors(Registers* colors, NodeSet adjacentNodes)
        {
            for (auto oneNode : adjacentNodes) 
                RemoveOneNodeColorFromColors(colors, oneNode);
        }

        void RegisterAllocation::RemoveAliasAdjacentNodeColors(Registers* colors, Node node)
        {
            Node aliasNode = GetNodeAlias(node);

            if (aliasNode != -1)
            {
                if ( aliasNode != node )
                    RemoveAdjacentNodeColors(colors, adjacentNodes[aliasNode]);

                for (auto oneAlias : nodeAlias[aliasNode])
                    if ( oneAlias != node)
                        RemoveAdjacentNodeColors(colors, adjacentNodes[oneAlias]);
            }
        }

        void RegisterAllocation::RemoveOneNodeColorFromColors(Registers* colors, Node node)
        {
            if (InNodeSet(node, coloredNodes) || IsPrecoloredNode(node))
                RemoveColorFromColors(
                    *colors,
                    IsPrecoloredNode(node) ?
                        interferenceGraph.GetNodeReg(node) :
                        nodeColors[node]);
        }

        Registers RegisterAllocation::GetAvailableColors()
        {
            //  no include stack pointer and base pointer
            Registers regs =  {
                Frame_EAX(), Frame_EBX(), Frame_ECX(), Frame_EDX(),
                Frame_ESI(), Frame_EDI()
            };

            assert (regNum >= 0 && regNum <= 6);
            return Registers(regs.begin(), regs.begin() + regNum);
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

        void RegisterAllocation::AssignColorsOnCoalescedNodes(Node node)
        {
            Node aliasNode = GetNodeAlias(node);
            
            if (aliasNode != -1)
            {
                if (IsPrecoloredNode(aliasNode))
                    nodeColors[aliasNode] = interferenceGraph.GetNodeReg(aliasNode);
                else if ( aliasNode != node)
                    nodeColors[aliasNode] = nodeColors[node];

                for (auto oneAlias : nodeAlias[aliasNode])
                    if (IsPrecoloredNode(oneAlias))
                        nodeColors[oneAlias] = interferenceGraph.GetNodeReg(oneAlias);
                    else if (oneAlias != node)
                        nodeColors[oneAlias] = nodeColors[node];
            }
        }

        ///////////////////////////////////////////////////////////////////////////

        void RegisterAllocation::Rewrite()
        {
            AllocateMemoryForSpilledRegs();
            
            Instructions newIns;
            for (auto it = ins.begin(); it != ins.end(); ++it)
                RewriteOneIns(*it, &newIns);

            ins.swap(newIns);
        }

        void RegisterAllocation::AllocateMemoryForSpilledRegs()
        {
            for (auto oneNode : spilledNodes)
                spilledMemorys[oneNode] = Frame_allocateLocal(
                    Trans_getFrame(level), true);
        }

        void RegisterAllocation::RewriteOneIns(shared_ptr<AAI> oneIns, Instructions* newIns)
        {
            if (oneIns->ToString().find("addl") != string::npos)
                RewriteOneAdd(oneIns, newIns);
            else if (oneIns->ToString().find("subl") != string::npos)
                RewriteOneSub(oneIns, newIns);
            else if (oneIns->ToString().find("imull") != string::npos)
                RewriteOneMul(oneIns, newIns);
            else if (oneIns->ToString().find("idivl") != string::npos)
                RewriteOneDiv(oneIns, newIns);
            else if (oneIns->ToString().find("xorl") != string::npos)
                RewriteOneXor(oneIns, newIns);
            else if (oneIns->ToString().find("cmpl") != string::npos)
                RewriteOneCmp(oneIns, newIns);
            else if (oneIns->ToString().find("call") != string::npos)
                RewriteOneCall(oneIns, newIns);
            else if (oneIns->ToString().find("movl") != string::npos)
                RewriteOneMov(oneIns, newIns);
            //  else do nothing
        }

        void RegisterAllocation::RewriteOneAdd(shared_ptr<AAI> oneAdd, Instructions* newIns)
        {
            //  in instruction
            //      `add(sub) regA, regB(or constValue)`
            //
            //  spill regA into
            //      `mov regNew [memA]`
            //      `add(sub) newNew regB(or constValue)`
            //      `mov [memA] regNew`
            //  
            //  spill regB(not constValue) sinto
            //      `mov regNew [memB]`
            //      `add(sub) regA, regNew`
            //
            //  spill both(no constValue) into
            //      `mov regNewB [memB]`
            //      `mov regNewA [memA]`
            //      `add(sub) newNewA regNewB`
            //      `mov [memA] regNewA`
            Registers dstRegs = oneAdd->GetDstRegs();
            assert (dstRegs.size() == 1);
            assert ( !IsPrecoloredNode(interferenceGraph.GetRegNode(dstRegs[0])) );

            Registers srcRegs = oneAdd->GetSrcRegs();
            assert (srcRegs.size() >= 1 && srcRegs[0] == dstRegs[0]);

            bool leftSpilled = RegSpilled(dstRegs[0]),
                rightSpilled = srcRegs.size() == 2 ?
                                     RegSpilled(srcRegs[1]) :
                                     false;

            if (leftSpilled && rightSpilled)
                RewriteBothRegs(newIns, oneAdd, dstRegs[0], srcRegs[1]);
            else if (leftSpilled)
                RewriteLeftReg(newIns, oneAdd, dstRegs[0]);
            else if (rightSpilled)
                RewriteRightReg(newIns, oneAdd, srcRegs[1]);
            else
                newIns->push_back(oneAdd);
        }

        void RegisterAllocation::RewriteBothRegs(
            Instructions* newIns, shared_ptr<AAI> one, Register leftReg, Register rightReg)
        {
            myTemp newRightReg = Temp_newTemp();
            myTemp rightMemoryAddr = CalculateNodeMemoryAddr(newIns, rightReg);
            newIns->push_back(make_shared<Move>(
                newRightReg, rightMemoryAddr,
                Move::OperandType::Content, Move::OperandType::Memory));

            Register newLeftReg = Temp_newTemp();
            Register leftMemoryAddr = CalculateNodeMemoryAddr(newIns, leftReg);
            newIns->push_back(make_shared<Move>(
                newLeftReg, leftMemoryAddr,
                Move::OperandType::Content, Move::OperandType::Memory));

            one->ReplaceReg(leftReg, newLeftReg);
            one->ReplaceReg(rightReg, newRightReg);
            newIns->push_back(one);

            newIns->push_back(make_shared<Move>(
                leftMemoryAddr, newLeftReg, Move::OperandType::Memory, Move::OperandType::Content));
        }

        void RegisterAllocation::RewriteLeftReg(
            Instructions* newIns, shared_ptr<AAI> one, Register leftReg)
        {
            Register newReg = Temp_newTemp();
            Register memoryAddr = CalculateNodeMemoryAddr(newIns, leftReg);

            newIns->push_back(make_shared<Move>(
                newReg, memoryAddr,
                Move::OperandType::Content, Move::OperandType::Memory));

            one->ReplaceReg(leftReg, newReg);
            newIns->push_back(one);
            
            newIns->push_back(make_shared<Move>(
                memoryAddr, newReg, Move::OperandType::Memory, Move::OperandType::Content));
        }

        void RegisterAllocation::RewriteRightReg(
            Instructions* newIns, shared_ptr<AAI> one, Register rightReg)
        {
            myTemp newReg = Temp_newTemp();
            myTemp memoryAddr = CalculateNodeMemoryAddr(newIns, rightReg);

            newIns->push_back(make_shared<Move>(
                newReg, memoryAddr,
                Move::OperandType::Content, Move::OperandType::Memory));
            
            one->ReplaceReg(rightReg, newReg);
            newIns->push_back(one);
        }

        void RegisterAllocation::RewriteOneSub(shared_ptr<AAI> oneSub, Instructions* newIns)
        {
            RewriteOneAdd(oneSub, newIns);
        }

        void RegisterAllocation::RewriteOneXor(shared_ptr<AAI> oneXor, Instructions* newIns)
        {
            RewriteOneAdd(oneXor, newIns);
        }

        void RegisterAllocation::RewriteOneMul(shared_ptr<AAI> oneMul, Instructions* newIns)
        {
            //  in instruction
            //      `imul(idiv) regA`
            //  spill regA into
            //      `mov regNew [memA]`
            //      `imul(idiv) regNew`
            //
            //  in instruction
            //      `imul(idiv) constValue`
            //  spill do nothing
            //
            //  cannot spill EAX,EDX because they are default destinations
            Registers srcRegs = oneMul->GetSrcRegs();
            assert (srcRegs.size() >= 1);
            assert ( !IsPrecoloredNode(interferenceGraph.GetRegNode(srcRegs[1])) );

            if (srcRegs.size() == 2 && RegSpilled(srcRegs[1]))
            {
                Register newReg = Temp_newTemp();
                Register memoryAddr = CalculateNodeMemoryAddr(newIns, srcRegs[1]);

                newIns->push_back(make_shared<Move>(
                    newReg, memoryAddr,
                    Move::OperandType::Content, Move::OperandType::Memory));

                oneMul->ReplaceReg(srcRegs[1], newReg);
                newIns->push_back(oneMul);
            }
            else
                newIns->push_back(oneMul);
        }

        void RegisterAllocation::RewriteOneDiv(shared_ptr<AAI> oneDiv, Instructions* newIns)
        {
            RewriteOneMul(oneDiv, newIns);
        }

        void RegisterAllocation::RewriteOneCmp(shared_ptr<AAI> oneCmp, Instructions* newIns)
        {
            //  in instruction
            //      `cmp regA regB`
            //
            //  spill regA or regB into
            //      `mov regNew [memA]`
            //      `cmp regNew regB`
            Registers srcRegs = oneCmp->GetSrcRegs();
            assert (srcRegs.size() >= 1);
            assert ( !IsPrecoloredNode(interferenceGraph.GetRegNode(srcRegs[0])) );
            if (srcRegs.size() == 2)
                     assert(!IsPrecoloredNode(interferenceGraph.GetRegNode(srcRegs[1])));

            for (auto& reg : srcRegs)
            {
                myTemp oldReg = reg;

                RewriteOneRegToMemory(newIns, &reg);
                oneCmp->ReplaceReg(oldReg, reg);
            }
            newIns->push_back(oneCmp);
        }

        void RegisterAllocation::RewriteOneRegToMemory(IS::Instructions* newIns, IS::Register* reg)
        {
            myTemp newReg = Temp_newTemp();
            myTemp memoryReg = CalculateNodeMemoryAddr(newIns, *reg);

            newIns->push_back(make_shared<Move>(
                newReg, memoryReg,
                Move::OperandType::Content, Move::OperandType::Memory));

            *reg = newReg;
        }

        void RegisterAllocation::RewriteOneCall(shared_ptr<AAI> oneCall, Instructions* newIns)
        {
            Registers regs = oneCall->GetSrcRegs();
            /*for (auto& reg : regs)
            {
                if (RegSpilled(reg))
                {
                    Register oldReg =  reg;
                    assert ( !IsPrecoloredNode(interferenceGraph.GetRegNode(reg)) );
                
                    //  we cannot get the function label from an AAI* ptr,
                    //  but we can replace the registers since they are all
                    //  source-registers(only fetched, not stored).
                    RewriteOneRegToMemory(newIns, &reg),
                    oneCall->ReplaceReg(oldReg, reg);
                }
            }*/
            RewriteRegsToMemoryAndReplace(newIns, oneCall, regs);
            newIns->push_back(oneCall);
        }

        void RegisterAllocation::RewriteOneMov(shared_ptr<AAI> oneMov, Instructions* newIns)
        {
            //  in instruction
            //      `mov [regA] regB`
            //
            //  spill regA(or regB) into
            //      `mov regNewA [memA]`
            //      `mov [regNewA] regB`
            //
            //  in instruction
            //      `mov regA regB`
            //
            //  spill regA into
            //      `mov regNewA [memA]`
            //      `mov regNewA regB`
            Registers srcRegs = oneMov->GetSrcRegs();
            Registers dstRegs = oneMov->GetDstRegs();
            assert (dstRegs.size() <= 1);

            RewriteRegsToMemoryAndReplace(newIns, oneMov, dstRegs);
            RewriteRegsToMemoryAndReplace(newIns, oneMov, srcRegs);
            newIns->push_back(oneMov);
        }

        void RegisterAllocation::RewriteRegsToMemoryAndReplace(
            Instructions* newIns, shared_ptr<IS::AAI> one, Registers& regs)
        {
            for (auto& reg : regs)
            {
                myTemp oldReg = reg;
                if (RegSpilled(reg))
                    RewriteOneRegToMemory(newIns, &reg),
                    one->ReplaceReg(oldReg, reg);
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

        /////////////////////////////////////////////////////////////////////////////

        RegisterMap RegisterAllocation::GetRegisterMap()
        {
            RegisterMap maps;
            for (auto it = nodeColors.begin(); it != nodeColors.end(); ++it)
            {
                maps[interferenceGraph.GetNodeReg(it->first)] = it->second;
            }

            return maps;
        }

        ////////////////////////////////////////////////////////////////////////////

        Instructions RegisterAllocation::GetIns()
        {
            return this->ins;
        }
    }
}
