#ifndef MY_MUNCH_H
#define MY_MUNCH_H

#include "CXX_myIRTree.h"
#include "myCanonical.h"
#include "AAI/myAaiBase.h"

#include <memory>

namespace lgxZJ
{
    namespace IS
    {
        typedef std::vector<std::shared_ptr<IS::AAI>> Instructions;

        class Munch
        {
            private:
                static Instructions ins;

            public:
                explicit Munch() = delete;

                //  DO:
                //      Do instruction selection in the given statement set.
                static void All(Canonical::Statements statements);

                //  DO:
                //      Do instruction selection in the given statement.
                //  REMARK:
                //      If the given statement is null, it do nothing.
                static void State(IR_myStatement statement);

                //  DO:
                //      Do instruction selection in the given expression.
                //  REMARK:
                //      If the given expression is null, it do nothing.
                static myTemp Exp(IR_myExp exp);

                //  DO:
                //      Get the selected abstract instructions.
                //  REMARK:
                //      Should be called after All(), State() or Exp().
                static Instructions GetIns();

                //  DO:
                //      Reset the selection result.
                static void Reset();

            private:
                static void PutIns(std::shared_ptr<AAI> aai);

            private:
                static void LabelState(IR_myStatement labelState);
                static void JumpState(IR_myStatement jumpState);
                static void CJumpState(IR_myStatement cjumpState);
                static void GenerateCmp(IR_myStatement cjumpState);
                static void GenerateJumpable(IR_myStatement cjumpState);
                static void MoveState(IR_myStatement moveState);
                static void LeftMemoryMoveState(IR_myExp leftOper, IR_myExp rightOper);
                static void LeftRegisterMoveState(IR_myExp leftOper, IR_myExp rightOper);
                static void ExpState(IR_myStatement expState);

                ///////////////////////////

                static myTemp BinExp(IR_myExp exp);
                static myTemp BinAddExp(IR_myExp exp);
                static myTemp BinSubExp(IR_myExp exp);
                static myTemp BinMulExp(IR_myExp exp);
                static myTemp BinDivExp(IR_myExp exp);
                static myTemp BinXorExp(IR_myExp exp);
                
                static myTemp MemExp(IR_myExp exp);
                static myTemp TempExp(IR_myExp exp);
                static myTemp ESeqExp(IR_myExp exp);
                static myTemp NameExp(IR_myExp exp);
                static myTemp ConstExp(IR_myExp exp);
                static myTemp CallExp(IR_myExp exp);
                static myTempList GetArgRegs(IR_myExpList argExps);
        };
    }
}

#endif