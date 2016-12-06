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

        Blocks Canonical::ToBlocks(Statements statements)
        {
            Blocks blocks;

            Block block;
            for_each(statements.begin(), statements.end(), [&blocks, &block](IR_myStatement state)
            {
                if (BlockStartsWithoutLabel(block, state))
                    DefineALabelForBlock(block);

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
                    !block.empty() &&
                    !IsJumpableStatement(state);
        }

        void Canonical::DefineALabelForBlock(Block& block)
        {
            block.push_back(IR_makeLabel(Temp_newLabel()));
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
    }
}