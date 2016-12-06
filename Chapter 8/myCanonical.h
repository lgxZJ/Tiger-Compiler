#ifndef MY_CANONICAL_H
#define MY_CANONICAL_H

#include "CXX_myIRTree.h"

#include <vector>

namespace lgxZJ
{
    namespace Canonical
    {
        typedef std::vector<IR_myStatement>     Statements;
        typedef Statements                      Block;
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
                static Blocks ToBlocks(Statements statements);

            private:
                static void Linearize_(Statements& outResult, IR_myStatement statements);
                static bool BlockStartsWithoutLabel(Block& block, IR_myStatement state);
                static bool BlockEndsWithoutJumpable(Block& block, IR_myStatement state);
                static bool IsJumpableStatement(IR_myStatement statement);
                static void DefineALabelForBlock(Block& block);
                static void AddJumpableAndAddBlockToResult(Blocks& blocks, Block& block, myLabel nextLabel);
                static void AddBlockToResult(Blocks& blocks, Block& block);
                static void AddEpilogueToRemainedAndAddBlockToResult(Blocks& blocks, Block& block);
        };
    }
}


#endif