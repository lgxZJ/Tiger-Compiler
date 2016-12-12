#ifndef MY_MUNCH_H
#define MY_MUNCH_H

#include "CXX_myIRTree.h"
#include "myCanonical.h"
#include "myCodeGen.h"

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

                static void All(Canonical::Statements statements);
                static void State(IR_myStatement statement);
                static myTemp Exp(IR_myExp exp);

                static Instructions GetIns();
                static void PutIns(std::shared_ptr<AAI> aai);
                static void Reset();

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
        };
    }
}

#endif