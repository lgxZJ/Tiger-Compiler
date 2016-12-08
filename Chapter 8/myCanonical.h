#ifndef MY_CANONICAL_H
#define MY_CANONICAL_H

#include "CXX_myIRTree.h"

#include <vector>

namespace lgxZJ
{
    namespace Canonical
    {
        //  statements begins with the first one, ends with the last one in sequence.
        typedef std::vector<IR_myStatement>     Statements;
        typedef Statements                      Block;
        //  statements orders are kept.
        typedef std::vector<Statements>         Blocks;
        

        class Canonical
        {
            public:
                Canonical() = delete;

                //  DO:
                //      Linearize statements into the format 'SEQ(s1, SEQ(s2,,,, SEQ(sn, snn)))';
                //  REMARK:
                //      Much is done in the translation phase, so the work here is very simple.
                static Statements Linearize(IR_myStatement statements);

                //  DO:
                //      Convert linearized statements into Basic Blocks.
                //  REMARK:
                //      Basic Blocks:
                //          1.The first statement is a label definition.
                //          2.The last statement is a jump or cjump.
                //          3.There are no other labels, jumps, or cjumps.
                static Blocks ToBlocks(Statements& statements);

                //  DO:
                //      Trace blocks, adjust block orders.
                //  REMARK:
                //      1.Adjust as many as possible block orders to:
                //          *Make an unconditional jump followed by its target value.
                //          *Make a conditional jump followed by its false label.
                static Blocks Trace(Blocks blocks);

                //  DO:
                //      Flatten traced-blocks into statements and remove redundant Jumps.
                //  REMARK:
                //      1.Remove Jump statements that followed by target labels.
                //      2.Statement order is kept.
                //  NOTE:
                //      Target labels following Jump's cannot be removed only if there are
                //      no other places where reference them. To make things simple, i just
                //      remain them where they are.
                static Statements Flatten(Blocks blocks);

            private:
                typedef std::vector<unsigned> IndexSet;

                static void Linearize_(Statements& outResult, IR_myStatement statements);
                static bool BlockStartsWithoutLabel(Block& block, IR_myStatement state);
                static bool BlockEndsWithoutJumpable(Block& block, IR_myStatement state);
                static bool IsJumpableStatement(IR_myStatement statement);
                static void DefineALabelForBlock(Block& block);
                static bool FormerBlockEndsWithNonFalseLabelCJump(Blocks& blocks);
                static bool FormerBlockEndsWithCJump(Blocks& blocks);
                static myLabel GetCurrentBlockLabel(Block& block, IR_myStatement statement);
                static void FillNonFalseLabelCJump(Block& formerBlock, myLabel falseLabel);
                static bool FormerBlockEndsWithCJumpButNeitherFollows(Blocks& blocks, myLabel followLabel);
                static void RewriteCJump(Blocks& blocks);                
                static void AddJumpableAndAddBlockToResult(Blocks& blocks, Block& block, myLabel nextLabel);
                static void AddBlockToResult(Blocks& blocks, Block& block);
                static void AddEpilogueToRemainedAndAddBlockToResult(Blocks& blocks, Block& block);

                static IndexSet GetNewTraceIndices(Blocks& blocks);
                static void AppendNewTraceToResult(Blocks& result, Blocks& blocks, IndexSet& traceIndices);
                static int GetNextBlockIndex(Blocks& blocks, IndexSet& indices);
                static myLabel GetNextLabelFromJumpable(IR_myStatement jumpableState);
                static bool IsEpilogueLabel(Blocks& blocks, myLabel label);
                static int FindLabelInBlocks(Blocks& blocks, myLabel label);
                static bool IsBlockMarked(IndexSet& currentTraceIndices, unsigned blockIndex);
                static void RemoveMarkedBlocks(Blocks& blocks, IndexSet& indices);

                static void RemoveRedundantJumps(Blocks& blocks);
        };
    }
}


#endif