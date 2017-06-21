#include "AAI/myMovable.h"
#include "AAI/myControlable.h"
#include "AAI/myComputable.h"
#include "myMunch.h"
#include "CXX_myFrame.h"

#include <cassert>
#include <vector>
#include <algorithm>

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
        //                          static functions
        ////////////////////////////////////////////////////////////////////

        void Munch::State(IR_myStatement statement)
        {
            if (statement == nullptr)   return;

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
            else if (rightOper->kind == IR_myExp_::IR_Temp)
            {                
                //  mov [reg], reg
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.mem->u.temp,
                            rightOper->u.temp,
                            Move::OperandType::Memory)));
            }
            else
            {
                assert (rightOper->kind == IR_myExp_::IR_Name);
                
                assert (false && "should not got here");
                //  todo:string represent
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.mem->u.temp,
                            Move::OperandType::Content,
                            1111111)));
            }
        }

        void Munch::LeftRegisterMoveState(IR_myExp leftOper, IR_myExp rightOper)
        {
            assert (leftOper->kind == IR_myExp_::IR_Temp);

            if (rightOper->kind == IR_myExp_::IR_Mem &&
                rightOper->u.mem->kind == IR_myExp_::IR_Temp)
            {
                //  mov reg, [reg]
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.temp,
                            rightOper->u.mem->u.temp,
                            Move::OperandType::Content,
                            Move::OperandType::Memory)));
            }
            else if (rightOper->kind == IR_myExp_::IR_Const)
            {
                //  mov reg, contValue
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.temp,
                            Move::OperandType::Content,
                            rightOper->u.constValue)));
            }
            else if (rightOper->kind == IR_myExp_::IR_Temp)
            {
                //  mov reg, reg
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.temp,
                            rightOper->u.temp)));
            }
            else if (rightOper->kind == IR_myExp_::IR_Name)
            {
                //  mov reg, label
                return PutIns(shared_ptr<Move>(
                    new Move(leftOper->u.temp,
                            rightOper->u.name)));
            }
            else
            {
                assert (false && "never got here");
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

        ////////////////////////////////////////////////////////////////////

        Instructions Munch::GetIns()
        {
            return ins;
        }

        ////////////////////////////////////////////////////////////////////

        void Munch::Reset()
        {
            ins.clear();
        }

        /////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////

        myTemp Munch::Exp(IR_myExp exp)
        {
            if (exp == nullptr)         return nullptr;

            switch (exp->kind)
            {
                case IR_myExp_::IR_BinOperation:    return BinExp(exp);
                case IR_myExp_::IR_Mem:             return MemExp(exp);
                case IR_myExp_::IR_Temp:            return TempExp(exp);
                case IR_myExp_::IR_ESeq:            return ESeqExp(exp);
                case IR_myExp_::IR_Name:            return NameExp(exp);
                case IR_myExp_::IR_Const:           return ConstExp(exp);
                case IR_myExp_::IR_Call:            return CallExp(exp);

                default:                            assert (false);
            }
        }

        myTemp Munch::BinExp(IR_myExp exp)
        {
            //  only these arithmetic operations are needed in tiger.
            switch(exp->u.binOperation.op)
            {
                case IR_BinOperator::IR_Plus:        return BinAddExp(exp);
                case IR_BinOperator::IR_Minus:      return BinSubExp(exp);
                case IR_BinOperator::IR_Multiply:   return BinMulExp(exp);
                case IR_BinOperator::IR_Divide:     return BinDivExp(exp);
                case IR_BinOperator::IR_Xor:        return BinXorExp(exp);

                default:    assert (false);
            }
        }

        //////////////////////////////////////////////

        #define DEFINE_ADDORSUBORXOR(CLSNAME)                                               \
            IR_myExp leftOper = exp->u.binOperation.left;                                   \
            IR_myExp rightOper = exp->u.binOperation.right;                                 \
            assert (leftOper->kind == IR_myExp_::IR_Temp);                                  \
                                                                                            \
            if (rightOper->kind == IR_myExp_::IR_Temp)                                      \
            {                                                                               \
                /*  add reg, reg        */                                                  \
                PutIns(shared_ptr<CLSNAME>(new CLSNAME(leftOper->u.temp, rightOper->u.temp)));      \
                return leftOper->u.temp;                                                    \
            }                                                                               \
            else if (rightOper->kind == IR_myExp_::IR_Const)                                \
            {                                                                               \
                /*  add reg, constValue       */                                            \
                PutIns(shared_ptr<CLSNAME>(new CLSNAME(leftOper->u.temp, rightOper->u.constValue)));\
                return leftOper->u.temp;                                                    \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                /*  no other forms used in tiger.   */                                      \
                assert (false);                                                             \
            }

            #define DEFINE_MULORDIV(CLSNAME)                                        \
                IR_myExp leftOper = exp->u.binOperation.left;                       \
                IR_myExp rightOper = exp->u.binOperation.right;                     \
                                                                                    \
                assert (leftOper->kind == IR_myExp_::IR_Temp);                      \
                PutIns(shared_ptr<Move>(new Move(Frame_EAX(), leftOper->u.temp)));  \
                                                                                    \
                if (rightOper->kind == IR_myExp_::IR_Temp)                          \
                {                                                                   \
                    PutIns(shared_ptr<CLSNAME>(new CLSNAME(rightOper->u.temp)));    \
                    return Frame_EAX();                                             \
                }                                                                   \
                else                                                                \
                {                                                                   \
                    assert (false);                                                 \
                }

        myTemp Munch::BinAddExp(IR_myExp exp)
        {
            DEFINE_ADDORSUBORXOR(Add);
        }

        myTemp Munch::BinSubExp(IR_myExp exp)
        {
            DEFINE_ADDORSUBORXOR(Sub);
        }

        myTemp Munch::BinMulExp(IR_myExp exp)
        {
            DEFINE_MULORDIV(IMul);
        }

        myTemp Munch::BinDivExp(IR_myExp exp)
        {
            DEFINE_MULORDIV(IDiv);
        }

        myTemp Munch::BinXorExp(IR_myExp exp)
        {
            DEFINE_ADDORSUBORXOR(Xor);
        }

        #undef DEFINE_ADDORSUB
        #undef DEFINE_MULORDIV

        //////////////////////////////////////////////

        myTemp Munch::MemExp(IR_myExp exp)
        {
            assert (exp->u.mem->kind == IR_myExp_::IR_Temp);

            //  mov newReg, [reg]
            myTemp newReg = Temp_newTemp();
            PutIns(shared_ptr<Move>(
                new Move(newReg, exp->u.mem->u.temp,
                        Move::OperandType::Content, Move::OperandType::Memory)));
            return newReg;
        }

        myTemp Munch::TempExp(IR_myExp exp)
        {
            return exp->u.temp;
        }

        myTemp Munch::ESeqExp(IR_myExp exp)
        {
            Munch::State(exp->u.eseq.statement);

            assert(exp->u.eseq.exp->kind == IR_myExp_::IR_Temp);
            return exp->u.eseq.exp->u.temp;
        }

        myTemp Munch::NameExp(IR_myExp exp)
        {
            //  never got here
            assert (false);
        }

        myTemp Munch::ConstExp(IR_myExp exp)
        {
            myTemp newReg = Temp_newTemp();
            PutIns(shared_ptr<Move>(
                new Move(newReg, Move::OperandType::Content, exp->u.constValue)));
            return newReg;
        }

        myTemp Munch::CallExp(IR_myExp exp)
        {
            assert(exp->u.call.funcName != NULL);

            myString funcName = exp->u.call.funcName;
            myTempList argRegs = GetArgRegs(exp->u.call.args);

            PutIns(shared_ptr<Call>(new Call(funcName, argRegs)));
            return Frame_EAX();
        }

        myTempList Munch::GetArgRegs(IR_myExpList argExps)
        {
            myTempList argRegs = nullptr;

            myTempList* listPtr = &argRegs;
            while (argExps)
            {
                //  todo: remove right assert when the compiler is done
                assert (argExps->head->kind == IR_myExp_::IR_Temp);

                *listPtr = Temp_makeTempList(argExps->head->u.temp, nullptr),
                listPtr = &(*listPtr)->tail;

                argExps = argExps->tails;
            }

            return argRegs;
        }

        /////////////////////////////////////////////////////////////////////////////
        //                          Munch::All()
        /////////////////////////////////////////////////////////////////////////////

        void Munch::All(Canonical::Statements statements)
        {
            for (IR_myStatement state : statements)
                Munch::State(state);
        }
    }
}