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
                explicit RegisterAllocation(
                    IS::Instructions instructions, Trans_myLevel level, unsigned regNum);

                RegisterMaps GetRegisterMaps();
                void SetRegNum(unsigned num)    {   regNum = num;    }

            private:
                void RealWork(IS::Instructions instructions);
                void InitInformation(IS::Instructions instructions);

                void Build();
                bool Simplify();//  todo: do not simplify precolored nodes
                bool Coalesce();
                bool Freeze();
                void Spill();   //  todo: do not spill precolored nodes
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
                void AnalyzeAlias(LA::Node* newNode, LA::Node* removeNode, LA::Edge edge);
                LA::Node GetNodeAlias(LA::Node node);
                void AddAlias(LA::Node newNode, LA::Node removeNode);
                void GetNodeNeighbors(LA::Node node, LA::NodeSet* nodes);
                bool IsSimplified(LA::Node node);
                void AdjustAdjacentDegrees(
                    LA::Edge edge, LA::NodeSet& commomNodes, unsigned newNeighborCount, bool isNeighborEachOther);
                void AdjustNodeCategoryInCoalesce(LA::Node from, LA::Edge edge);
                void AdjustNodeCategoryNotCoalesced(LA::Node node);
                void RemoveNodeFromSet(LA::NodeSet& nodes, LA::Node node);
                void RemoveNonAliasNodeFromSet(LA::NodeSet& ndoes, LA::Node node);

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
                void AssignColorsOnCoalescedNodes(LA::Node node);
                
                void AllocateMemoryForSpilledRegs();
                void RewriteOneIns(std::shared_ptr<IS::AAI> oneIns, IS::Instructions* newIns);

                void RewriteOneAdd(std::shared_ptr<IS::AAI> oneAdd, IS::Instructions* newIns);
                void RewriteOneSub(std::shared_ptr<IS::AAI> oneSub, IS::Instructions* newIns);
                void RewriteOneXor(std::shared_ptr<IS::AAI> oneXor, IS::Instructions* newIns);
                void RewriteOneMul(std::shared_ptr<IS::AAI> oneMul, IS::Instructions* newIns);
                void RewriteOneDiv(std::shared_ptr<IS::AAI> oneDiv, IS::Instructions* newIns);
                void RewriteOneCmp(std::shared_ptr<IS::AAI> oneCmp, IS::Instructions* newIns);
                void RewriteOneCall(std::shared_ptr<IS::AAI> oneCall, IS::Instructions* newIns);
                void RewriteOneMov(std::shared_ptr<IS::AAI> oneMov, IS::Instructions* newIns);

                void RewriteBothRegs(IS::Instructions* newIns, std::shared_ptr<IS::AAI> one, IS::Register leftReg, IS::Register rightReg);
                void RewriteLeftReg(IS::Instructions* newIns, std::shared_ptr<IS::AAI> one, IS::Register leftReg);
                void RewriteRightReg(IS::Instructions* newIns, std::shared_ptr<IS::AAI> one, IS::Register leftReg);

                void RewriteOneRegToMemory(IS::Instructions* newIns, IS::Register* reg);
                void RewriteRegsToMemoryAndReplace(IS::Instructions* newIns, std::shared_ptr<IS::AAI> one, IS::Registers& reg);

                bool RegSpilled(IS::Register reg);
                IS::Register CalculateNodeMemoryAddr(IS::Instructions* newIns, IS::Register reg);

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
                unsigned                            regNum;
        };
    }
}

#endif