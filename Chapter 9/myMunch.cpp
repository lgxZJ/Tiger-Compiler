#include "AAI/myMovable.h"
#include "AAI/myControlable.h"
#include "AAI/myComputable.h"
#include "myMunch.h"

#include <cassert>
#include <vector>

using namespace lgxZJ::Canonical;
using namespace std;

namespace lgxZJ
{
    namespace IS
    {
        ////////////////////////////////////////////////////////////////////
        //                          static members
        ////////////////////////////////////////////////////////////////////

        Instructions Munch::ins;

        ////////////////////////////////////////////////////////////////////
        //                          functions
        ////////////////////////////////////////////////////////////////////

        void Munch::State(IR_myStatement statement)
        {
            switch (statement->kind)
            {
                case IR_myStatement_::IR_Label:
                    return LabelState(statement);
                case IR_myStatement_::IR_Jump:
                    return JumpState(statement);
                case IR_myStatement_::IR_CJump:
                    return CJumpState(statement);
                case IR_myStatement_::IR_Move:
                    return MoveState(statement);
                case IR_myStatement_::IR_Exp:
                    return ExpState(statement);
                default:    assert (false);
            }
        }

        void Munch::LabelState(IR_myStatement labelState)
        {
            return PutIns(shared_ptr<Label>(new Label(labelState->u.label)));
        }

        void Munch::JumpState(IR_myStatement jumpState)
        {
            return PutIns(shared_ptr<Jmp>(new Jmp(jumpState->u.jump.jumpLabels->head)));
        }

        void Munch::CJumpState(IR_myStatement cjumpState)
        {
            GenerateCmp(cjumpState);
            GenerateJumpable(cjumpState);
        }

        void Munch::GenerateCmp(IR_myStatement cjumpState)
        {
            //  gather the compare instruction
            IR_myExp leftOper = cjumpState->u.cjump.left;
            IR_myExp rightOper = cjumpState->u.cjump.right;

            if (leftOper->kind == IR_myExp_::IR_Temp)
            {
                if (rightOper->kind == IR_myExp_::IR_Const)
                {
                    //  cmp reg, constValue
                    return PutIns(shared_ptr<Cmp>(new Cmp(leftOper->u.temp, rightOper->u.constValue)));
                }
                else
                {
                    //  cmp reg, reg
                    assert (rightOper->kind == IR_myExp_::IR_Temp);
                    return PutIns(shared_ptr<Cmp>(new Cmp(leftOper->u.temp, rightOper->u.temp)));
                }
            }
            else
                //  in tiger translation phase, it is always a register
                assert (false);
        }

        void Munch::GenerateJumpable(IR_myStatement cjumpState)
        {
            myLabel trueLabel = cjumpState->u.cjump.trueLabel;
            switch (cjumpState->u.cjump.op)
            {
                case IR_Equal:
                    return PutIns(shared_ptr<Je>(new Je(trueLabel)));
                case IR_NotEqual:
                    return PutIns(shared_ptr<Jne>(new Jne(trueLabel)));
                case IR_LessThan:
                    return PutIns(shared_ptr<Jl>(new Jl(trueLabel)));
                case IR_LessEqual:
                    return PutIns(shared_ptr<Jle>(new Jle(trueLabel)));
                case IR_GreaterThan:
                    return PutIns(shared_ptr<Jg>(new Jg(trueLabel)));
                case IR_GreaterEqual:
                    return PutIns(shared_ptr<Jge>(new Jge(trueLabel)));

                //  fall through
                case IR_ULessThan:
                case IR_ULessEqual:
                case IR_UGreaterThan:
                case IR_UGreaterEqual:
                default:    assert (false);
            }
        }

        void Munch::MoveState(IR_myStatement moveState)
        {
            IR_myExp leftOper = moveState->u.move.dst;
            IR_myExp rightOper = moveState->u.move.src;

            if (leftOper->kind == IR_myExp_::IR_Mem &&
                leftOper->u.mem->kind == IR_myExp_::IR_Temp)
                return LeftMemoryMoveState(leftOper, rightOper);
            else
                return LeftRegisterMoveState(leftOper, rightOper);
        }

        void Munch::LeftMemoryMoveState(IR_myExp leftOper, IR_myExp rightOper)
        {
            if (rightOper->kind == IR_myExp_::IR_Const)
            {
                //  mov [reg], constValue
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.mem->u.temp,
                            Move::OperandType::Memory,
                            rightOper->u.constValue)));
            }
            else
            {
                assert (rightOper->kind == IR_myExp_::IR_Temp);
                //  mov [reg], reg
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.mem->u.temp,
                            rightOper->u.temp,
                            Move::OperandType::Memory)));
            }
        }

        void Munch::LeftRegisterMoveState(IR_myExp leftOper, IR_myExp rightOper)
        {
            assert (leftOper->kind == IR_myExp_::IR_Temp);
            if (rightOper->kind == IR_myExp_::IR_Const)
            {
                //  mov reg, contValue
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.temp,
                            Move::OperandType::Content,
                            rightOper->u.constValue)));
            }
            else
            {
                assert (rightOper->kind == IR_myExp_::IR_Temp);
                //  mov reg, reg
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.temp,
                            rightOper->u.temp)));
            }
        }

        void Munch::ExpState(IR_myStatement expState)
        {
            Munch::Exp(expState->u.exp);
        }

        ////////////////////////////////////////////////////////////////////

        void Munch::PutIns(shared_ptr<AAI> aaiPtr)
        {
            ins.push_back(aaiPtr);
        }

        Instructions Munch::GetIns()
        {
            return ins;
        }

        void Munch::Reset()
        {
            ins.clear();
        }

        /////////////////////////////////////////////////////////////////////

        myTemp Munch::Exp(IR_myExp exp)
        {
            //  todo:
        }
    }
}