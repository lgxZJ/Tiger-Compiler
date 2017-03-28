#ifndef MY_REGISTER_ALLOCATION_H
#define MY_REGISTER_ALLOCATION_H

#include "myLiveness.h"
#include "CXX_myFrame.h"
#include "CXX_myTranslate.h"

#include <map>
#include <stack>

namespace lgxZJ
{
    namespace RA
    {
        typedef std::map<IS::Register, IS::Register> RegisterMaps;

        //  TODO:   change Register to Node to remove convertion
        class RegisterAllocation
        {
            public:
                explicit RegisterAllocation(IS::Instructions instructions, Trans_myLevel level);

                RegisterMaps GetRegisterMaps();

            private:
                void RealWork(IS::Instructions instructions);
                void InitInformation(IS::Instructions instructions);

                void Build();
                bool Simplify();
                bool Coalesce();
                bool Freeze();
                void Spill();
                void AssignColors();
                void Rewrite();

                bool HaveHighDegreeNodes();

                void SaveDegreeInformation(LA::NodeSet& nodes);
                void SaveAdjacentNodes(LA::NodeSet& nodes);
                void ClassifyIntoDifferentSets(LA::NodeSet& nodes);
                bool InMovePairs(LA::Node node);

                void AdjustNodeCategoryInSimplify(LA::Node reg);

                bool IsCoalescable(
                    LA::Edge edge, LA::NodeSet* commomNodes, unsigned* newNeighborCount, bool* isNeighborEachOther);
                void DoCoalesce(
                    LA::Edge edge, LA::NodeSet& commomNodes, unsigned newNeighborCount, bool isNeighborEachOther);
                void GetNodeNeighbors(LA::Node node, LA::NodeSet* nodes);
                bool IsSimplified(LA::Node node);
                void AdjustAdjacentDegrees(
                    LA::Edge edge, LA::NodeSet& commomNodes, unsigned newNeighborCount, bool isNeighborEachOther);
                void AdjustNodeCategoryInCoalesce(LA::Node from, LA::Edge edge);
                void AdjustNodeCategoryNotCoalesced(LA::Node node);
                void RemoveNodeFromSet(LA::NodeSet& nodes, LA::Node node);

                void MoveFreezedNodeIntoLowDegreeNonMoveSet(LA::Node node);
                void UpdateMoveRelatedNodes(LA::Node freezedNode);
                void UpdateOnePairSet(LA::MovePairs& pairs, LA::Node nodeToFreeze, LA::NodeSet* possibleChangedNonMoveNodes);
                bool InNodeSet(LA::Node node, LA::NodeSet& nodeSet);
                void MoveHighDegreeNodeFromMoveToNonMove(LA::Node node);
                void MoveLowDegreeNodeFromMoveToNonMove(LA::Node node);

                LA::Node SelectSpillNode(LA::NodeSet nodes);

                void AssignColorToPrecolored(LA::Node node);
                void AssignColorToNonPrecolored(LA::Node node);
                IS::Registers GetAvailableColors();
                bool IsPrecoloredNode(LA::Node node);
                void RemoveColorFromColors(IS::Registers& colors, IS::Register color);
                void AssignColorsOnCoalescedNodes();
                
                void AllocateMemoryForSpilledRegs();
                void RewriteDefs(
                    IS::Registers& defRegs, std::shared_ptr<IS::AAI> oneIns, IS::Instructions* newIns);
                bool RegSpilled(IS::Register reg);
                IS::Register CalculateNodeMemoryAddr(IS::Instructions* newIns, IS::Register reg);
                void RewriteUses(
                    IS::Registers& useRegs, std::shared_ptr<IS::AAI> oneIns, IS::Instructions* newIns);

            private:
                LA::NodeSet   lowDegreeNonMoveNodes;
                LA::NodeSet   lowDegreeMoveNodes;
                LA::NodeSet   highDegreeMoveNodes;
                LA::NodeSet   highDegreeNonMoveNodes;

                std::map<LA::Node, unsigned>        nodeDegrees;
                std::stack<LA::Node>                selectStack;
                LA::NodeSet                         coloredNodes;
                std::map<LA::Node, IS::Register>    nodeColors;
                LA::NodeSet                         spilledNodes;
                std::map<LA::Node, myAccess>        spilledMemorys;
                std::map<LA::Node, LA::NodeSet>     adjacentNodes;

                IS::Instructions                    ins;
                LA::CFGraph                         cfGraph;
                LA::MovePairs                       unconstrainedMovePairs;
                LA::MovePairs                       constrainedMovePairs;
                LA::MovePairs                       deletedMovePairs;
                LA::InterferenceGraph               interferenceGraph;
                std::map<LA::Node, LA::NodeSet>     nodeAlias;

                Trans_myLevel                       level;
        };
    }
}

#endif