#include "myTranslate.h"
#include "myEnvironment.h"
#include "mySemantic.h"

#include <assert.h> //  for assert()
#include <stdlib.h> //  for NULL macro

/////////////////////////////////////////////////////////////////
//                          macros
#define NONE_LEVEL                  NULL

#define MAKE_ONE_TRANS_EXP()    Trans_myExp one =                                   \
                                    makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);\
                                assert (one);

/////////////////////////////////////////////////////////////////
//                          structures

struct Trans_myAccess_
{
    Trans_myLevel   level;
    myAccess        access;
};

struct Trans_myLevel_
{
    Trans_myLevel       previousLevel;
    myFrame             frame;
    Trans_myAccessList  formals;
};

///////////////////////////////////////////

//  a list of labels where needed to be filled.
typedef struct Trans_myPatchList_
{
    myLabel*                    head;
    struct Trans_myPatchList_*  tails;
}* Trans_myPatchList;


typedef struct Trans_myCondiExp_
{
    //  places needed to filled when the condition is true.
    Trans_myPatchList   trues;
    //  places needed to filled when the condition is false.
    Trans_myPatchList   falses;
    IR_myStatement      statement;
}* Trans_myCondiExp;

struct Trans_myExp_
{
    enum { Trans_Exp, Trans_CondiExp, Trans_NoValueExp } kind;
    union 
    {
        IR_myExp                    exp;
        struct Trans_myCondiExp_    condiExp; 
        IR_myStatement              noValueState;
    }u;
};

typedef struct Trans_myExpList_
{
    Trans_myExp                 head;
    struct Trans_myExpList_*    tail;
}*  Trans_myExpList;

///////////////////////////////////////////////////////////////////
//                           private functions

static Trans_myAccess Trans_makeAccess(Trans_myLevel level, myAccess access)
{
    Trans_myAccess accessInLevel =
        makeMemoryBlock(sizeof(*accessInLevel), MEMORY_TYPE_NONE);
    assert (accessInLevel);

    accessInLevel->access = access;
    accessInLevel->level = level;
    return accessInLevel;
}

static Trans_myAccessList Trans_convertAccessFromFrameToLevel(
    myAccessList accessesInFrame, Trans_myLevel level)
{
    Trans_myAccessList result = Trans_makeAccessList(NULL, NULL);

    Trans_myAccessList accessesInLevel = result;
    while (accessesInFrame)
    {
        Trans_myAccess accessInLevel =
            Trans_makeAccess(level, accessesInFrame->head);

        accessesInLevel->head=  accessInLevel;
        accessesInFrame->tail == NULL ?
            (0) :
            (accessesInLevel->tail = Trans_makeAccessList(NULL, NULL));

        accessesInFrame = accessesInFrame->tail;
        accessesInLevel = accessesInLevel->tail;
    }
    return result;
}

///////////////////////////////////////

static Trans_myPatchList Tran_makePatchList(myLabel* head, Trans_myPatchList tail)
{
    Trans_myPatchList one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
    assert (one);

    one->head = head;
    one->tails = tail;
    return one;
}

static Trans_myExp Trans_makeExp(IR_myExp exp)
{
    MAKE_ONE_TRANS_EXP();

    one->kind = Trans_Exp;
    one->u.exp = exp;
    return one;
}

static Trans_myExp Trans_makeNoValueExp(IR_myStatement statement)
{
    MAKE_ONE_TRANS_EXP();

    one->kind = Trans_NoValueExp;
    one->u.noValueState = statement;
    return one;
}

static Trans_myExp Trans_makeCondiExp(
    Trans_myPatchList trues, Trans_myPatchList falses,
    IR_myStatement statement)
{
    MAKE_ONE_TRANS_EXP();

    one->kind = Trans_CondiExp;
    one->u.condiExp.falses = falses;
    one->u.condiExp.trues = trues;
    one->u.condiExp.statement = statement;
    return one;
}

/////////////////////////////////////////

static void fillPatches(Trans_myPatchList patches, myLabel label)
{
    while (patches)
    {
        *patches->head = label;
        patches = patches->tails;
    }
}

//  todo:
static Trans_myPatchList joinPatches(
    Trans_myPatchList first, Trans_myPatchList second)
{
    if (first == NULL) return second;
    while (first->tails)
        first = first->tails;
    first->tails = second;
    return first;
}

static IR_myExp Trans_stripExp(Trans_myExp exp)
{
    switch (exp->kind)
    {
        case Trans_NoValueExp:
            return IR_makeESeq(exp->u.noValueState, IR_makeConst(0));
        case Trans_CondiExp:
        {
            myTemp temp = Temp_newTemp();
            myLabel trueLabel = Temp_newLabel();
            myLabel falseLabel = Temp_newLabel();

            fillPatches(exp->u.condiExp.trues, trueLabel);
            fillPatches(exp->u.condiExp.falses, falseLabel);

            return IR_makeESeq(IR_makeMove(IR_makeTemp(temp), IR_makeConst(1)),
                    IR_makeESeq(exp->u.condiExp.statement, 
                        IR_makeESeq(IR_makeLabel(falseLabel),
                            IR_makeESeq(IR_makeMove(IR_makeTemp(temp), IR_makeConst(0)),
                                IR_makeESeq(IR_makeLabel(trueLabel), IR_makeTemp(temp))))));
        }
        case Trans_Exp:
            return exp->u.exp;
    }
    assert (false); //  never get here
}

static IR_myStatement Trans_stripNoValueExp(Trans_myExp exp)
{
    //  todo:
}

static Trans_myCondiExp Trans_stripCondiExp(Trans_myExp exp)
{
    //  todo:
}

IR_myExp Trans_getExpField(Trans_myExp exp)
{
    return exp->u.exp;
}

/////////////////////////////////////////////////////////////////////
//                      public functions

Trans_myAccessList Trans_makeAccessList(
    Trans_myAccess head, Trans_myAccessList tail)
{
    Trans_myAccessList accessList =
        makeMemoryBlock(sizeof(*accessList), MEMORY_TYPE_NONE);
    assert (accessList);

    accessList->head = head;
    accessList->tail = tail;
    return accessList;
}

///////////////////////////////

int Trans_getAccessListCount(Trans_myAccessList list)
{
    int count = 0;
    while (list)
    {
        ++count;
        list = list->tail;
    }
    return count;
}

//////////////////////////////

Trans_myLevel Trans_getAccessLevel(Trans_myAccess access)
{
    return access->level;
}

////////////////////////////////

bool Trans_isLevelEqual(Trans_myLevel lhs, Trans_myLevel rhs)
{
    return lhs == rhs;
}

////////////////////////////////

static Trans_myLevel g_outermostLevel = NULL;

Trans_myLevel Trans_outermostLevel(void)
{
    if (g_outermostLevel == NULL)
    {
        g_outermostLevel =
            makeMemoryBlock(sizeof(g_outermostLevel), MEMORY_TYPE_NONE);
        assert (g_outermostLevel);

        myLabel outermostLabel = Temp_newNamedLabel("outmost");
        myBoolList noFormalFlags = NULL;
        g_outermostLevel->frame = Frame_newFrame(outermostLabel, noFormalFlags);
        g_outermostLevel->formals = /*Trans_convertAccessFromFrameToLevel(
            Frame_getFormals(g_outermostLevel->frame), NONE_LEVEL);*/
            NULL;
    } 
    return g_outermostLevel;
}

bool Trans_isOutermostLevel(Trans_myLevel level)
{
    assert (g_outermostLevel != NULL);
    assert (level != NULL);
    return g_outermostLevel == level; 
}

////////////////////////////////

//  private forwards in myFrame module
void fillFormalsFromFlags(myFrame frame, myBoolList formalFlags);

//  To obey the C calling conventions, the formal parameters(include
//  static link and return address) are allocated in the parent frame.
//  But logically, these formals are part of the new level, so we add
//  a `formals` field in order to make the formals accessable from the
//  new level.
Trans_myLevel Trans_newLevel(
    Trans_myLevel parent, myLabel name, myBoolList formals)
{
    myBoolList formalsAddStaticLinkAndReturnAddr = Frame_makeBoolList(
        Frame_makeBoolList(formals, true), true);
    fillFormalsFromFlags(parent->frame, formalsAddStaticLinkAndReturnAddr);

    myFrame frame = Frame_newFrame(name, NULL);

    Trans_myLevel level = makeMemoryBlock(sizeof(*level), MEMORY_TYPE_NONE);
    assert (level);
    level->frame = frame;
    level->previousLevel = parent;
    level->formals = Trans_convertAccessFromFrameToLevel(
        Frame_getFormals(parent->frame), level);
    return level;
}

//////////////////////////////

Trans_myAccess Trans_allocateLocal(Trans_myLevel level, bool escapeFlag)
{
    myAccess frameAccess = Frame_allocateLocal(level->frame, escapeFlag);

    Trans_myAccess transAccess = makeMemoryBlock(sizeof(*transAccess), MEMORY_TYPE_NONE);
    assert (transAccess);

    transAccess->access = frameAccess;
    transAccess->level = level;
    return transAccess;
}

//////////////////////////////

Trans_myAccessList Trans_getFormals(Trans_myLevel level)
{
    if (Trans_isOutermostLevel(level))
        return NULL;
    else
        //  skip the static link formal and return value
        return level->formals->tail->tail;
}

myFrame Trans_getFrame(Trans_myLevel level)
{
    return level->frame;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//                              translaters

Trans_myAccess getVarAccessFromName(mySymbol varName)
{
    myVarAndFuncEntry varEntry = MyEnvironment_getVarOrFuncFromName(
        MySemantic_getVarAndFuncEnvironment(),
        varName);
    assert (varEntry);
    return MyEnvironment_getVarAccess(varEntry);
}

///////////////////////////


////////////////////////////////////////////////////////////////////////////////

//  forwards
IR_myExp calculateVarAddr(Trans_myAccess access);

//////////

bool isExpRecordCreation()
{
    //  todo:
    return false;
}

bool isExpArrayCreation()
{
    //  todo:
    return false;
}

IR_myExp doArrayTranslation()
{
    //  todo:
}

IR_myExp doRecordTranslation()
{
    //  todo:
}

IR_myExp doSimpleTranslation(Trans_myAccess varAccess, myExp varBody)
{
    IR_myExp varRepresent;
    if (Frame_isAccessInReg(varAccess->access))
        varRepresent = IR_makeTemp(Frame_getAccessReg(varAccess->access)); 
    else
        varRepresent = calculateVarAddr(varAccess);
    
    IR_myStatement statement = IR_makeMove(varRepresent, Trans_Exp_(varBody));
    //  variable declarations has no result value.
    return IR_makeESeq(statement, IR_makeConst(0));
}

IR_myExp Trans_VarDec(myVarDec varDec)
{
    mySymbol varName;
    myExp varBody;

    switch (varDec->kind)
    {
        case LongFormVar:
            varName = varDec->u.longFormVar->name;
            varBody = varDec->u.longFormVar->exp;
            break;
        case ShortFormVar:
            varName = varDec->u.shortFormVar->name;
            varBody = varDec->u.shortFormVar->exp;
            break;
        default:    assert (false);
    }

    Trans_myAccess varAccess = getVarAccessFromName(varName);
    if (isExpRecordCreation())      return doRecordTranslation();
    else if(isExpArrayCreation())   return doArrayTranslation();
    else                            return doSimpleTranslation(varAccess, varBody);
}

IR_myExp Trans_Dec(myDec dec)
{
    switch (dec->kind)
    {
        case VarDec:    return Trans_VarDec(dec->u.varDec);
        case TypeDec:   //  todo:
        case FuncDec:
        default:    assert (false); //  never get here
    }
}

#define PROCESS_LIST(processFunc, field)    IR_myStatement result;              \
                                            if (list == NULL)   result = NULL;  \
                                            else                result = IR_makeSeq(NULL, NULL);\
                                                                                \
                                            IR_myStatement temp = result;       \
                                            IR_myExp oneResult = NULL;          \
                                            while (list)                        \
                                            {                                   \
                                                oneResult = processFunc(list->field);           \
                                                assert (oneResult->kind == IR_ESeq);            \
                                                                                                \
                                                //  variable declarations have no value         \
                                                temp->u.seq.left = oneResult->u.eseq.statement; \
                                                list->next ?                                    \
                                                    (temp->u.seq.right = IR_makeSeq(NULL, NULL)):\
                                                    (temp->u.seq.right = NULL); \
                                                                                \
                                                list = list->next;              \
                                                temp = temp->u.seq.right;       \
                                            }
                                            

IR_myExp Trans_Decs(myDecList list)
{
    IR_myStatement result;
    if (list == NULL)   result = NULL;
    else                result = IR_makeSeq(NULL, NULL);

    IR_myStatement temp = result; 
    while (list)
    {
        IR_myExp oneResult = Trans_Dec(list->dec);
        assert (oneResult->kind == IR_ESeq);

        //  variable declarations have no value
        temp->u.seq.left = oneResult->u.eseq.statement;
        list->next ?
            (temp->u.seq.right = IR_makeSeq(NULL, NULL)):
            (temp->u.seq.right = NULL);

        list = list->next;
        temp = temp->u.seq.right;
    }
    //PROCESS_LIST(Trans_Dec, dec);

    return IR_makeESeq(result, IR_makeConst(0));
}

IR_myExp Trans_Exps(myExpList list)
{
    IR_myStatement result;
    if (list == NULL)   result = NULL;
    else                result = IR_makeSeq(NULL, NULL);

    IR_myStatement temp = result;
    IR_myExp oneResult = NULL;
    while (list)
    {
        oneResult = Trans_Exp_(list->exp);
        assert (oneResult->kind == IR_ESeq);

        //  variable declarations have no value
        temp->u.seq.left = oneResult->u.eseq.statement;
        list->next ?
            (temp->u.seq.right = IR_makeSeq(NULL, NULL)):
            (temp->u.seq.right = NULL);

        list = list->next;
        temp = temp->u.seq.right;
    }

    if (oneResult == NULL)  //  empty body
        return IR_makeESeq(result, IR_makeConst(0));
    else
        return IR_makeESeq(result, oneResult->u.eseq.exp);
}

IR_myExp Trans_LetExp(myLetExp letExp)
{
     IR_myExp decsResult = Trans_Decs(letExp->decList);
     IR_myExp expsResult = Trans_Exps(letExp->expList);

     assert (decsResult->kind == IR_ESeq);
     assert (expsResult->kind == IR_ESeq);
     return IR_makeESeq(
         IR_makeSeq(decsResult->u.eseq.statement, expsResult->u.eseq.statement),
         expsResult->u.eseq.exp);
}

//////////////////////////////////////////////////////////////

Trans_myExp Trans_LValueExp(myLValueExp lVAlueExp)
{
    //  todo:
}

//////////////////////

IR_myExp calculateVarAddr(Trans_myAccess access)
{
    Trans_myLevel levelUsed = MySemantic_getCurrentLevel();
    Trans_myLevel levelDeclared = access->level;

    IR_myExp sum = IR_makeTemp(Frame_FP());
    while (levelUsed != levelDeclared)
    {
        levelUsed = levelUsed->previousLevel;
        //  stack grows towards low-part
        sum = IR_makeBinOperation(IR_Plus, sum, 
            IR_makeConst(Frame_getLocalCount(levelUsed->frame) * Frame_wordSize));
    }

    return IR_makeBinOperation(IR_Minus,
        sum, IR_makeConst(Frame_getAccessOffset(access->access)));
}

IR_myExp Trans_LValueExp_SimpleVar(myLValueExp lValueExp)
{
    Trans_myAccess varAccess = getVarAccessFromName(lValueExp->id);

    if (Frame_isAccessInFrame(varAccess->access))
    {
        return calculateVarAddr(varAccess);
        /*return Trans_makeExp(varAddr);*/
    }
    else
    {
        return IR_makeTemp(Frame_getAccessReg(varAccess->access)); 
        /*return Trans_makeExp(varReg);*/
    }
}

/////////////////////

IR_myExp Trans_LValueExp_RecordField(myLValueExp lValueExp)
{
    //  todo:
}


////////////////////////////////////////////////////////////

IR_myExp Trans_IntegerLiteralExp(myIntegerLiteralExp integerLiteralExp)
{
    return IR_makeESeq(NULL, IR_makeConst(integerLiteralExp->value));
}

///////////////////////////////////////////////////////////////////////////////

IR_myExp Trans_Exp_(myExp exp)
{
    switch (exp->kind)
    {
        case LetExp:
            return Trans_LetExp(exp->u.letExp);
       // case LValueExp:
            //return Trans_LValueExp(exp->u.lValueExp);
        //  todo:
        case IntegerLiteralExp:
            return Trans_IntegerLiteralExp(exp->u.integerLiteralExp);
        default:    assert (false);
    }
}