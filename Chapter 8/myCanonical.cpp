#include "myCanonical.h"

#include <algorithm>
#include <cassert>

using namespace std;

namespace lgxZJ
{
    namespace Canonical
    {
        
        Statements Canonical::Linearize(IR_myStatement statements)
        {
            Statements result;
            Linearize_(result, statements);
            return result;
        }
             
        void Canonical::Linearize_(Statements& outResult, IR_myStatement statements)
        {
            if (statements == nullptr)
                return;


            switch (statements->kind)
            {
                case IR_myStatement_::IR_Seq:
                    return  Linearize_(outResult, statements->u.seq.left),
                            Linearize_(outResult, statements->u.seq.right);
                
                //  fall through
                case IR_myStatement_::IR_Exp:
                case IR_myStatement_::IR_CJump:
                case IR_myStatement_::IR_Jump:
                case IR_myStatement_::IR_Move:
                case IR_myStatement_::IR_Label:
                    return outResult.push_back(statements);
                default:        assert (false);
            }
        }

        /////////////////////////////////////////////////////////////////////

        Blocks Canonical::ToBlocks(Statements& statements)
        {
            Blocks blocks;

            Block block;
            for_each(statements.begin(), statements.end(), [&blocks, &block](IR_myStatement state)
            {
                if (BlockStartsWithoutLabel(block, state))
                    DefineALabelForBlock(block);
                if (FormerBlockEndsWithNonFalseLabelCJump(blocks))
                    FillNonFalseLabelCJump(blocks.back(), GetCurrentBlockLabel(block, state));

                if (BlockEndsWithoutJumpable(block, state))
                    AddJumpableAndAddBlockToResult(blocks, block, state->u.label);

                block.push_back(state);

                if (IsJumpableStatement(state))
                    AddBlockToResult(blocks, block);
            });

            AddEpilogueToRemainedAndAddBlockToResult(blocks, block);
            return blocks;
        }

        bool Canonical::IsJumpableStatement(IR_myStatement statement)
        {
            return statement->kind == IR_myStatement_::IR_Jump ||
                    statement->kind == IR_myStatement_::IR_CJump;
        }

        bool Canonical::BlockStartsWithoutLabel(Block& block, IR_myStatement state)
        {
            return block.empty() && state->kind != IR_myStatement_::IR_Label;   
        }

        bool Canonical::BlockEndsWithoutJumpable(Block& block, IR_myStatement state)
        {
            return  state->kind == IR_myStatement_::IR_Label &&
                    !block.empty();
        }

        void Canonical::DefineALabelForBlock(Block& block)
        {
            block.push_back(IR_makeLabel(Temp_newLabel()));
        }


        bool Canonical::FormerBlockEndsWithNonFalseLabelCJump(Blocks& blocks)
        {
            if (blocks.empty())
                return false;

            Block& formerBlock = blocks.back();
            return  formerBlock.back()->kind == IR_myStatement_::IR_CJump &&
                    formerBlock.back()->u.cjump.falseLabel == nullptr;
        }

        myLabel Canonical::GetCurrentBlockLabel(Block& block, IR_myStatement statement)
        {
            return block.empty() ? statement->u.label : block.front()->u.label;
        }

        void Canonical::FillNonFalseLabelCJump(Block& formerBlock, myLabel falseLabel)
        {
            formerBlock.back()->u.cjump.falseLabel = falseLabel;
        }

        void Canonical::AddBlockToResult(Blocks& blocks, Block& block)
        {
            blocks.push_back(block),
            block.clear();
        }

        void Canonical::AddJumpableAndAddBlockToResult(
            Blocks& blocks, Block& block, myLabel nextLabel)
        {
            block.push_back(
                    IR_makeJump(
                        IR_makeName(nextLabel),
                        Temp_makeLabelList(nextLabel, nullptr)));
            AddBlockToResult(blocks, block);
        }

        void Canonical::AddEpilogueToRemainedAndAddBlockToResult(Blocks& blocks, Block& block)
        {
            if (block.empty())
                DefineALabelForBlock(block); 
            myLabel epilogueLabel = Temp_newLabel();
            AddJumpableAndAddBlockToResult(blocks, block, epilogueLabel);
        }

        /////////////////////////////////////////////////////////////////////

        Blocks Canonical::Trace(Blocks blocks)
        {
            Blocks result;

            IndexSet currentTraceIndices;
            while (!blocks.empty())
            {
                currentTraceIndices = GetNewTraceIndices(blocks);
                AppendNewTraceToResult(result, blocks, currentTraceIndices);
                RemoveMarkedBlocks(blocks, currentTraceIndices);
            }

            return result;
        }

        Canonical::IndexSet Canonical::GetNewTraceIndices(Blocks& blocks)
        {
            //  trace start from the first block
            IndexSet traceIndices;
            traceIndices.push_back(0);

            int nextBlockIndex = GetNextBlockIndex(blocks, traceIndices);
            while (nextBlockIndex != -1 && !IsBlockMarked(traceIndices, nextBlockIndex))
            {
                traceIndices.push_back(nextBlockIndex);
            }

            return traceIndices;
        }


        void Canonical::AppendNewTraceToResult(Blocks& result, Blocks& blocks, IndexSet& traceIndices)
        {
            for_each(traceIndices.begin(), traceIndices.end(),
                [&result, &blocks](int index)
                {
                result.push_back(blocks.at(index));
                });
        }

        int Canonical::GetNextBlockIndex(Blocks& blocks, IndexSet& currentTraceIndices)
        {
            assert (!currentTraceIndices.empty());

            //  The traced block index sequence is ordered to make the same with traced blocks.
            //  So, the last index belongs to the last traced block
            IR_myStatement jumpableState = blocks.at(currentTraceIndices.back()).back();
            myLabel nextLabel = GetNextLabelFromJumpable(jumpableState);

            if (IsEpilogueLabel(blocks, nextLabel))
                return -1;
            else
                FindLabelInBlocks(blocks, nextLabel);
        }

        myLabel Canonical::GetNextLabelFromJumpable(IR_myStatement jumpableState)
        {
            switch (jumpableState->kind)
            {
                case IR_myStatement_::IR_Jump:
                {
                    assert (jumpableState->u.jump.exp->kind == IR_myExp_::IR_Name);
                    return jumpableState->u.jump.exp->u.name;
                }
                case IR_myStatement_::IR_CJump:
                {
                    myLabel falseLabel = jumpableState->u.cjump.falseLabel;

                    //      every cjump should only contain valid labels and there must be one
                    //  label that is valid.
                    if (falseLabel == nullptr)
                    {
                        jumpableState->u.cjump.falseLabel = jumpableState->u.cjump.trueLabel;
                        jumpableState->u.cjump.trueLabel = falseLabel;
                        jumpableState->u.cjump.op = revertRelOperator(jumpableState->u.cjump.op);
                    }
                    return jumpableState->u.cjump.falseLabel;
                }
                default:    assert (false);
            }
        }

        bool Canonical::IsEpilogueLabel(Blocks& blocks, myLabel label)
        {
            return label == blocks.back().back()->u.jump.exp->u.name;
        }

        int Canonical::FindLabelInBlocks(Blocks& blocks, myLabel label)
        {
            for (int i = 0; i < blocks.size(); ++i)
                if (blocks.at(i).front()->u.label == label) return i;
            return -1;
        }

        bool Canonical::IsBlockMarked(IndexSet& currentTraceIndices, unsigned blockIndex)
        {
            return find_if(currentTraceIndices.begin(), currentTraceIndices.end(), [blockIndex](unsigned one)
                        {
                            return one == blockIndex;
                        })
                    != currentTraceIndices.end();
        }

        void Canonical::RemoveMarkedBlocks(Blocks& blocks, IndexSet& indices)
        {
            Blocks result;
            for (int i = 0; i < blocks.size(); ++i)
            {
                if (!IsBlockMarked(indices, i))
                    result.push_back(blocks.at(i));
            }
            swap(blocks, result);
        }

        ///////////////////////////////////////////////////////////////////////////

        Statements Canonical::Flatten(Blocks blocks)
        {
            RemoveRedundantJumps(blocks);

            Statements statements;
            for_each (blocks.begin(), blocks.end(), [&statements](Block& block)
            {
                for_each (block.begin(), block.end(), [&statements](IR_myStatement state)
                {
                    statements.push_back(state);
                });
            });

            return statements;
        }

        void Canonical::RemoveRedundantJumps(Blocks& blocks)
        {
            //  skip epilogue jump
            for (size_t i = 0; i + 1 < blocks.size(); ++i)
            {
                IR_myStatement jumpableState = blocks.at(i).back();
                switch (jumpableState->kind)
                {
                    case IR_myStatement_::IR_CJump:
                        break;
                    case IR_myStatement_::IR_Jump:
                        //  i only use on jump target in all the former phases
                        assert (jumpableState->u.jump.exp->kind == IR_myExp_::IR_Name);
                        assert (jumpableState->u.jump.jumpLabels->tail == nullptr);
                        if (jumpableState->u.jump.jumpLabels->head == blocks.at(i + 1).front()->u.label)
                            blocks.at(i).pop_back();
                        break;
                    default:        assert (false);
                }
            }
        }
    }
}