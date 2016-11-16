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

/*
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
*/
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

/*
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
*/
/////////////////////////////////////////

static void fillPatches(Trans_myPatchList patches, myLabel label)
{
    while (patches)
    {
        *patches->head = label;
        patches = patches->tails;
    }
}
/*
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
*/
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
//

#define ADD_ONE_FRAG(globalVar)  if (g_procFrags == NULL)   \
        g_procFrags = Frame_makeFragList(NULL, NULL);       \
                                                            \
        if (g_procFrags->head == NULL)                      \
            g_procFrags->head = frag;                       \
        else                                                \
            g_procFrags->tail = Frame_makeFragList(frag, NULL);

static Frame_myFragList g_procFrags = NULL;
void Trans_addOneProcFrag(Frame_myFrag frag)
{
    assert (frag->kind == Frame_ProcFrag);
    ADD_ONE_FRAG(g_procFrags);
}

Frame_myFragList Trans_getProcFrags(void)
{
    return g_procFrags;
}

void Trans_resetProcFrags(void)
{
    g_procFrags = NULL;
}

///////////

static Frame_myFragList g_stringFrags = NULL;
void Trans_addOneStringFrag(Frame_myFrag frag)
{
    assert (frag->kind == Frame_StringFrag);
    ADD_ONE_FRAG(g_stringFrags);
}

Frame_myFragList Trans_getStringFrags(void)
{
    return g_stringFrags;
}

void Trans_resetStringFrags(void)
{
    g_stringFrags = NULL;
}

////////////////////////////////////////////////////////////////////////
//                              translaters

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

//////////////

Trans_myAccess getVarAccessFromName(mySymbol varName)
{
    myVarAndFuncEntry varEntry = MyEnvironment_getVarOrFuncFromName(
        MySemantic_getVarAndFuncEnvironment(),
        varName);
    assert (varEntry);
    return MyEnvironment_getVarAccess(varEntry);
}


IR_myExp doAssignment(Trans_myAccess varAccess, IR_myExp varBodyResult)
{
    IR_myExp varRepresent;
    if (Frame_isAccessInReg(varAccess->access))
        varRepresent = IR_makeTemp(Frame_getAccessReg(varAccess->access)); 
    else
        varRepresent = calculateVarAddr(varAccess);
    
    IR_myStatement statement = IR_makeMove(varRepresent, varBodyResult);
    //  assignment result is the value of right-operand
    if (Frame_isAccessInReg(varAccess->access))
        return IR_makeESeq(statement, varRepresent);
    else
        return IR_makeESeq(statement, IR_makeMem(varRepresent));
}
///////////////////////////


////////////////////////////////////////////////////////////////////////////////


IR_myStatement Trans_VarDec(myVarDec varDec)
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
 
    return doAssignment(getVarAccessFromName(varName), Trans_Exp_(varBody))
        ->u.eseq.statement;
}

////////////////////////////////////////////////////////////////

IR_myStatement Trans_FuncDec(myFuncDec funcDec)
{
    mySymbol funcName;
    myExp funcBody;

    switch (funcDec->kind)
    {
        case ProcedureDec:
            funcName = funcDec->u.procedureDec->name;
            funcBody = funcDec->u.procedureDec->exp;
            break;
        case FunctionDec:
            funcName = funcDec->u.functionDec->name;
            funcBody = funcDec->u.functionDec->exp;
            break;
        default:        assert (false);
    }

    //  no value expressions must have already been wrapped!!
    IR_myStatement funcState = IR_makeExp(Trans_Exp_(funcBody));

    Frame_myFrag procFrag = Frame_makeProcFrag(funcState,
        MySemantic_getCurrentLevel()->frame,
        funcName);
    Trans_addOneProcFrag(procFrag);
    return NULL;    //  function does not need return code
}

////////////////////////////////////////////////////////////////

IR_myStatement Trans_Dec(myDec dec)
{
    switch (dec->kind)
    {
        case VarDec:    return Trans_VarDec(dec->u.varDec);
        case TypeDec:   break;  //  do nothing
        case FuncDec:   return Trans_FuncDec(dec->u.funcDec);
        default:        assert (false); //  never get here
    }
}   

//  declarations have no value, so they are just statements
IR_myStatement Trans_Decs(myDecList list)
{
    IR_myStatement result;
    if (list == NULL)   result = NULL;                  
    else                result = IR_makeSeq(NULL, NULL);
                                                        
    IR_myStatement temp = result;                       
    IR_myStatement oneResult = NULL;                          
    while (list)                                        
    {                                                   
        oneResult = Trans_Dec(list->dec);           
                                                        
        temp->u.seq.left = oneResult;       
        list->next ?                                    
        (temp->u.seq.right = IR_makeSeq(NULL, NULL)):   
        (temp->u.seq.right = NULL);                     
                                                        
        list = list->next;                             
        temp = temp->u.seq.right;                       
    }    
    return result;
}

IR_myExp Trans_Exps(myExpList list)
{
    //  variable declarations have no value
    IR_myExp result;
    if (list == NULL)   return  NULL;              
    else                result = IR_makeESeq(NULL, NULL);

    IR_myExp toFill = result;
    IR_myExp temp = result;                                        
    while (list->next)                  
    {                                                   
        IR_myExp oneResult = Trans_Exp_(list->exp);           
                                                        
        temp->u.eseq.statement = IR_makeExp(oneResult);       
        list->next->next ?                                    
        (temp->u.eseq.exp = IR_makeESeq(NULL, NULL)):   
        (toFill = temp);   
                                                        
        list = list->next;                             
        temp = temp->u.eseq.exp;                       
    } 

    //  last one is the value of let-exp
    toFill->u.eseq.exp = Trans_Exp_(list->exp);
    return result;
}

IR_myExp Trans_LetExp(myLetExp letExp)
{
     IR_myStatement decsResult = Trans_Decs(letExp->decList);
     IR_myExp expsResult = Trans_Exps(letExp->expList);

     return IR_makeESeq(decsResult, expsResult);
}

//////////////////////////////////////////////////////////////

IR_myExp Trans_LValueExp_SimpleVar(myLValueExp lValueExp)
{
    Trans_myAccess varAccess = getVarAccessFromName(lValueExp->id);

    if (Frame_isAccessInFrame(varAccess->access))
        return calculateVarAddr(varAccess);
    else
        return IR_makeTemp(Frame_getAccessReg(varAccess->access)); 
}

/////////////////////

IR_myExp Trans_LValueExp_RecordField(myLValueExp lValueExp)
{
    //  todo:
    return NULL;
}

/////////////////////

IR_myExp Trans_LValueExp_ArraySubscript(myLValueExp lValueExp)
{
    //  todo:
    return NULL;
}

/////////////////////

IR_myExp Trans_LValueExp(myLValueExp lValueExp)
{
    //  todo:
    switch (lValueExp->kind)
    {
        case SimpleVar:
            return Trans_LValueExp_SimpleVar(lValueExp);
        case RecordField:
            return Trans_LValueExp_RecordField(lValueExp);
        case ArraySubscript:
            return Trans_LValueExp_ArraySubscript(lValueExp);
        default:    assert (false);
    }
}

////////////////////////////////////////////////////////////

IR_myExp Trans_FunctionCallExp(myFunctionCallExp functionCallExp)
{
    //  todo:
    return NULL;
}

////////////////////////////////////////////////////////////

IR_myExp Trans_NilExp(myNilExp nilExp)
{
    //  todo:
    return NULL;
}

////////////////////////////////////////////////////////////

IR_myExp Trans_IntegerLiteralExp(myIntegerLiteralExp integerLiteralExp)
{
    return IR_makeConst(integerLiteralExp->value);
}

///////////////////////////////////////////////////////////

IR_myExp Trans_StringLiteralExp(myStringLiteralExp stringLiteralExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_ArrayCreationExp(myArrayCreationExp arrayCreationExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_RecordCreationExp(myRecordCreationExp recordCreationExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_ArithmeticExp(myArithmeticExp arithmeticExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_ParenthesesExp(myParenthesesExp parenthesesExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_NoValueExp(myNoValueExp noValueExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_SequencingExp(mySequencingExp sequencingExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_ForExp(myForExp forExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_IfThenElseExp(myIfThenElseExp ifthenElseExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_IfThenExp(myIfThenExp ifThenExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_ComparisonExp(myComparisonExp comparisonExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_BooleanOperateExp(myBooleanOperateExp booleanOperateExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_AssignmentExp(myAssignmentExp assignmentExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_WhileExp(myWhileExp whileExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_NegationExp(myNegationExp negationExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////

IR_myExp Trans_BreakExp(myBreakExp breakExp)
{
    //  todo:
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

IR_myExp Trans_Exp_(myExp exp)
{
    switch (exp->kind)
    {
        case LValueExp:
            return Trans_LValueExp(exp->u.lValueExp);
        case FunctionCallExp:
            return Trans_FunctionCallExp(exp->u.functionCallExp);
        case NilExp:
            return Trans_NilExp(exp->u.nilExp);
        case IntegerLiteralExp:
            return Trans_IntegerLiteralExp(exp->u.integerLiteralExp);
        case StringLiteralExp:
            return Trans_StringLiteralExp(exp->u.stringLiteralExp);
        case ArrayCreationExp:
            return Trans_ArrayCreationExp(exp->u.arrayCreationExp);
        case RecordCreationExp:
            return Trans_RecordCreationExp(exp->u.recordCreationExp);
        case ArithmeticExp:
            return Trans_ArithmeticExp(exp->u.arithmeticExp);
        case ParenthesesExp:
            return Trans_ParenthesesExp(exp->u.parenthesesExp);
        case NoValueExp:
            return Trans_NoValueExp(exp->u.noValueExp);
        case SequencingExp:
            return Trans_SequencingExp(exp->u.sequencingExp);
        case ForExp:
            return Trans_ForExp(exp->u.forExp);
        case IfThenElseExp:
            return Trans_IfThenElseExp(exp->u.ifThenElseExp);
        case IfThenExp:
            return Trans_IfThenExp(exp->u.ifThenExp);
        case ComparisonExp:
            return Trans_ComparisonExp(exp->u.comparisonExp);
        case BooleanOperateExp:
            return Trans_BooleanOperateExp(exp->u.booleanOperateExp);
        case AssignmentExp:
            return Trans_AssignmentExp(exp->u.assignmentExp);
        case LetExp:
            return Trans_LetExp(exp->u.letExp);
        case WhileExp:
            return Trans_WhileExp(exp->u.whileExp);
        case NegationExp:
            return Trans_NegationExp(exp->u.negationExp);
        case BreakExp:
            return Trans_BreakExp(exp->u.breakExp);
        
        default:    assert (false);
    }
}