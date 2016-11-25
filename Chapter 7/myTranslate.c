#include "myTranslate.h"
#include "myEnvironment.h"
#include "mySemantic.h"
#include "recursiveDecs.h"  //  for addFormalsToScope().

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
    Trans_myAccessList  formals;    //  include return value and static link
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

bool Trans_isLevelSame(Trans_myLevel lhs, Trans_myLevel rhs)
{
    return Trans_isLevelEqual(lhs, rhs) &&
            Frame_isFrameSame(lhs->frame, rhs->frame) &&
            lhs->formals == rhs->formals &&
            lhs->previousLevel == rhs->previousLevel;
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
    }

    myLabel outermostLabel = Temp_newNamedLabel("outmost");
    myBoolList noFormalFlags = NULL;
    g_outermostLevel->frame = Frame_newFrame(outermostLabel, noFormalFlags);
    g_outermostLevel->formals = NULL;
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

static void addOneFrag(Frame_myFragList* fragHolder, Frame_myFrag frag)
{
    Frame_myFragList* iter = fragHolder;
    if (*iter == NULL)
        *iter = Frame_makeFragList(NULL, NULL);

    if ((*iter)->head == NULL)
        (*iter)->head = frag, iter = &(*iter)->tail;
    else
        (*iter)->tail = Frame_makeFragList(NULL, NULL),
        (*iter)->tail->head = frag,
        iter = &((*iter)->tail->tail);
}

static Frame_myFragList g_procFrags = NULL;
void Trans_addOneProcFrag(Frame_myFrag frag)
{
    assert (frag->kind == Frame_ProcFrag);
    addOneFrag(&g_procFrags, frag);
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
    addOneFrag(&g_stringFrags, frag);
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

#define INIT_INTERATOR(x)                               \
        IR_myStatement* iterator = &(x->u.seq.right);   \
        IR_myStatement* nextIter;

#define ALLOCATE_AND_SET_ITERATOR()     do {    \
        *iterator = IR_makeSeq(NULL, NULL);     \
        nextIter = &((*iterator)->u.seq.right); \
        iterator = &((*iterator)->u.seq.left);  \
    }while (false)

#define MOVE_TO_NEXT_ITERATOR()         do {    \
        iterator = nextIter;                    \
    }while (false)

#define SET_VALUE(value)        *iterator = (value)


IR_myExp calculateVarAddr(Trans_myAccess access)
{
    Trans_myLevel levelUsed = MySemantic_getCurrentLevel();
    Trans_myLevel levelDeclared = access->level;

    IR_myExp tempExp = IR_makeTemp(Temp_newTemp());
    IR_myStatement sum = IR_makeSeq(
        IR_makeMove(tempExp, IR_makeTemp(Frame_FP())),
        NULL);

    INIT_INTERATOR(sum);
    while (levelUsed != levelDeclared)
    {
        levelUsed = levelUsed->previousLevel;

        ALLOCATE_AND_SET_ITERATOR();
        //  stack grows towards low-part
        IR_myStatement state = IR_makeExp(IR_makeBinOperation(IR_Plus,
            tempExp, 
            IR_makeConst(Frame_getLocalCount(levelUsed->frame) * Frame_wordSize)));
        SET_VALUE(state);

        MOVE_TO_NEXT_ITERATOR();
    }

    ALLOCATE_AND_SET_ITERATOR();
    IR_myStatement state = IR_makeExp(IR_makeBinOperation(IR_Plus,
                tempExp,
                IR_makeConst(Frame_getAccessOffset(access->access))));
    SET_VALUE(state);

    return IR_makeESeq(sum, tempExp);
}

#undef ALLOCATE_AND_SET_ITERATOR
#undef MOVE_TO_NEXT_ITERATOR
#undef INIT_INTERATOR
#undef SET_VALUE

//////////////

Trans_myAccess getVarAccessFromName(mySymbol varName)
{
    myVarAndFuncEntry varEntry = MyEnvironment_getVarOrFuncFromName(
        MySemantic_getVarAndFuncEnvironment(),
        varName);
    assert (varEntry);
    return MyEnvironment_getVarAccess(varEntry);
}


/////////////////////////////////////////////////////////////////////

IR_myExp doInRegAssigment(myAccess access, IR_myExp varBodyResult)
{
    IR_myExp varRepresent = IR_makeTemp(Frame_getAccessReg(access));

    IR_myStatement bodyState;
    IR_myExp bodyValue;
    IR_divideExp(varBodyResult, &bodyState, &bodyValue);

    IR_myStatement resultStatement = IR_makeSeq(
        bodyState, 
        IR_makeMove(varRepresent, bodyValue));
    return IR_makeESeq(resultStatement, varRepresent);
}

IR_myExp doInFrameAssigment(Trans_myAccess varAccess, IR_myExp varBodyResult)
{
    IR_myExp varRepresent = calculateVarAddr(varAccess);
        
    IR_myStatement calcState;
    IR_myExp calcValue;
    IR_divideExp(varRepresent, &calcState, &calcValue);

    IR_myStatement bodyState;
    IR_myExp bodyValue;
    IR_divideExp(varBodyResult, &bodyState, &bodyValue);

    //  form a new intermediate code, save value to memory location
    IR_myStatement resultStatement = IR_makeSeq(
        IR_makeSeq(calcState, bodyState),
        IR_makeMove(IR_makeMem(calcValue), bodyValue));

    //  result is the memory content
    IR_myExp newReg = IR_makeTemp(Temp_newTemp());
    resultStatement = IR_makeSeq(
        resultStatement, IR_makeMove(newReg, IR_makeMem(calcValue)));
    return IR_makeESeq(resultStatement, newReg);
}

IR_myExp doAssignmentTranslation(Trans_myAccess varAccess, IR_myExp varBodyResult)
{
    if (Frame_isAccessInReg(varAccess->access))
    {
        return doInRegAssigment(varAccess->access, varBodyResult);
    } 
    else
    {
        return doInFrameAssigment(varAccess, varBodyResult);
    }
}
///////////////////////////


//////////////////////////////////////////////////////////////

IR_myExp Trans_LValueExp_SimpleVar(myLValueExp lValueExp)
{
    Trans_myAccess varAccess = getVarAccessFromName(lValueExp->id);

    if (Frame_isAccessInFrame(varAccess->access))
    {
        IR_myExp ret = calculateVarAddr(varAccess);
        assert (ret && ret->kind == IR_ESeq); 
        return  IR_makeESeq(
            ret->u.eseq.statement, IR_makeMem(ret->u.eseq.exp));
    }
    else
        return IR_makeTemp(Frame_getAccessReg(varAccess->access)); 
}

/////////////////////

IR_myExp Trans_LValueExp_GetRecordPtr(myLValueExp lValueExp)
{
    IR_myExp ret = Trans_LValueExp_SimpleVar(lValueExp);

    //  record variables are always in frame
    //  BinOperation must start with a temporary as its left operand
    IR_myExp tempExp = IR_makeTemp(Temp_newTemp());
    IR_myStatement stateResult = IR_makeSeq(
        ret->u.eseq.statement,
        IR_makeMove(tempExp, ret->u.eseq.exp)); 
    return IR_makeESeq(stateResult, tempExp); 
}


/////////////////////

IR_myExp Trans_LValueExp_GetArrayPtr(myLValueExp lValueExp)
{
    //  treat array as record when getting its address
    return Trans_LValueExp_GetRecordPtr(lValueExp);
}

/////////////////////

//
//	assignment：
//	    l-value:
//	                    int         Record      Array
//	        in reg      no-mem      no-mem      no-mem
//	        in frame    mem         mem         mem
//
//	    r-value：
//
//	        the same as described below
//
//	r-value:
//	                    int         Record      Array
//	        in reg      no-mem      no-mem      no-mem
//	        in frame    mem         mem         mem
//
IR_myExp Trans_LValueExp(myLValueExp lValueExp)
{
    //  todo:
    switch (lValueExp->kind)
    {
        case SimpleVar:
            return Trans_LValueExp_SimpleVar(lValueExp);
        case RecordField:
        case ArraySubscript:
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

//  todo: remove this wrap to places Binoperation used
IR_myExp Trans_IntegerLiteralExp(myIntegerLiteralExp integerLiteralExp)
{
    //  in order to make BinOperation left operand an register,
    //  we complicate the process here.
    IR_myExp resultReg = IR_makeTemp(Temp_newTemp());
    return IR_makeESeq(
        IR_makeMove(resultReg, IR_makeConst(integerLiteralExp->value)),
        resultReg);
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
    if (exp == NULL)        return NULL;

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
            /*return Trans_LetExp(exp->u.letExp);*/
            assert (false);
        case WhileExp:
            return Trans_WhileExp(exp->u.whileExp);
        case NegationExp:
            return Trans_NegationExp(exp->u.negationExp);
        case BreakExp:
            return Trans_BreakExp(exp->u.breakExp);
        
        default:    assert (false);
    }
}

////////////////////////////////////////////////////////////////////////////

static void processFuncBody(
    IR_myExp bodyResult, mySymbol routineName, bool hasReturnValue)
{
    IR_myStatement bodyState;
	IR_myExp bodyValue;
	IR_divideExp(bodyResult, &bodyState, &bodyValue);

	IR_myStatement resultState = IR_makeSeq(bodyState, NULL);
    if (hasReturnValue)
        resultState->u.seq.right = IR_makeMove(
            IR_makeTemp(Frame_RV()), bodyValue);

	myFrame frame =  Trans_getFrame(MyEnvironment_getFuncLevel(
	    MyEnvironment_getVarOrFuncFromName(
	        MySemantic_getVarAndFuncEnvironment(), routineName)));
	Frame_myFrag frag = Frame_makeProcFrag(resultState, frame, routineName);
	Trans_addOneProcFrag(frag);
}

void Trans_proccedureBody(IR_myExp bodyResult, mySymbol procName)
{
    bool hasReturnValue = false;
    processFuncBody(bodyResult, procName, hasReturnValue);
}

void Trans_functionBody(IR_myExp bodyResult, mySymbol funcName)
{
    bool hasReturnValue = true;
    processFuncBody(bodyResult, funcName, hasReturnValue);
}

//////////////////////////////////////////////////////////////////////////////////

//  keep a string fragment
void Trans_string(IR_myExp labelExp, myString str)
{
    assert (labelExp->kind == IR_Name);
    Frame_myFrag strFrag = Frame_makeStringFrag(labelExp->u.name, str);
    Trans_addOneStringFrag(strFrag);
}

//////////////////////////////////////////////////////////////////////////////////

void getAddressContent(IR_myExp* addressExpPtr)
{
    assert (addressExpPtr && (*addressExpPtr)->kind == IR_ESeq);
    assert ((*addressExpPtr)->u.eseq.exp->kind == IR_Temp);

    IR_myExp tempReg = IR_makeTemp(Temp_newTemp());
    (*addressExpPtr)->u.eseq.statement = IR_makeSeq(
        (*addressExpPtr)->u.eseq.statement,
        IR_makeMove(tempReg, IR_makeMem((*addressExpPtr)->u.eseq.exp))); 
    (*addressExpPtr)->u.eseq.exp = tempReg;
}

///////////////////////////////////////////////////////////////////////////

static IR_myStatement translateSpaceAllocation(
    IR_myExp subscriptResult, IR_myExp* subscriptValuePtr, IR_myExp* memoryAddrResultPtr)
{
    //  maybe not needed
    IR_myStatement subscriptState;
    IR_myExp subscriptValue;
    IR_divideExp(subscriptResult, &subscriptState, &subscriptValue);

    //  todo: subscript bound check
    //  allocate space for array at runtime
    IR_myExp tempReg = IR_makeTemp(Temp_newTemp());
    IR_myStatement tempRegMoveState = IR_makeMove(
        tempReg, subscriptValue);
    IR_myExp allocateResult = Frame_externalCall("malloc",
        IR_makeExpList(
            IR_makeBinOperation(IR_Multiply, tempReg, IR_makeConst(Frame_wordSize)),
            NULL));
    assert (allocateResult->kind == IR_ESeq);

    //  save results
    *subscriptValuePtr = subscriptValue;
    *memoryAddrResultPtr = allocateResult->u.eseq.exp;
    return IR_makeSeq(
        IR_makeSeq(subscriptState, tempRegMoveState), 
        allocateResult->u.eseq.statement);
}

static IR_myExp saveResultForReturn(IR_myExp memoryAddrReg, IR_myStatement* combinedStatePtr)
{
    assert (memoryAddrReg->kind == IR_Temp);
    //  save result for return
    IR_myExp resultAddr = IR_makeTemp(Temp_newTemp());
    (*combinedStatePtr) = IR_makeSeq(
        (*combinedStatePtr),
        IR_makeMove(resultAddr, memoryAddrReg));

    return resultAddr;
}

static IR_myExp combineInitState(IR_myExp initValueResult, IR_myStatement* combinedStatePtr)
{
    IR_myStatement initState;
    IR_myExp initValue;
    IR_divideExp(initValueResult, &initState, &initValue);

    (*combinedStatePtr) = IR_makeSeq((*combinedStatePtr), initState);
    return initValue;
}

static void prepareArrayAssignmentLoop(IR_myStatement* combinedStatePtr,
    IR_myExp* loopRegPtr, IR_myStatement* loopLabelExpPtr,
    IR_myStatement* skipLabelExpPtr)
{
    (*loopRegPtr) = IR_makeTemp(Temp_newTemp());
    (*combinedStatePtr) = IR_makeSeq(
        (*combinedStatePtr), IR_makeMove((*loopRegPtr), IR_makeConst(0)));

    (*loopLabelExpPtr) = IR_makeLabel(Temp_newLabel());
    (*skipLabelExpPtr) = IR_makeLabel(Temp_newLabel());

    (*combinedStatePtr) = IR_makeSeq((*combinedStatePtr), *loopLabelExpPtr);
} 

static void combineLoopChecker(
    IR_myStatement* combinedStatePtr, IR_myExp loopReg,
    IR_myExp subscriptValueReg, IR_myStatement skipLabelExp)
{
    (*combinedStatePtr) = IR_makeSeq((*combinedStatePtr),
        IR_makeCJump(IR_GreaterEqual,
            loopReg, subscriptValueReg,
            skipLabelExp->u.label, NULL));
}

static IR_myStatement doOneElementAssignment(
    IR_myExp memoryAddrReg, IR_myExp initValue)
{
    return IR_makeMove(IR_makeMem(memoryAddrReg), initValue);
}

static void calcNextAddress(IR_myStatement* loopStatePtr, IR_myExp memoryAddrReg)
{
    *loopStatePtr = IR_makeSeq(
        (*loopStatePtr),
        IR_makeExp(IR_makeBinOperation(IR_Plus, 
            memoryAddrReg, IR_makeConst(-Frame_wordSize))));
}

static void incrementLoopReg(IR_myStatement* loopStatePtr, IR_myExp loopReg)
{
    (*loopStatePtr) = IR_makeSeq(
        (*loopStatePtr),
        IR_makeExp(IR_makeBinOperation(IR_Plus, 
            loopReg, IR_makeConst(1))));
}

static void jumpToLoop(
    IR_myStatement* loopStatePtr, IR_myStatement loopLabelExp)
{
    (*loopStatePtr) = IR_makeSeq(
        (*loopStatePtr),
        IR_makeJump(IR_makeName(loopLabelExp->u.label),
            Temp_makeLabelList(loopLabelExp->u.label, NULL)));
}

static void defineSkipLabel(
    IR_myStatement* loopStatePtr, IR_myStatement skipLabelExp)
{
    (*loopStatePtr) = IR_makeSeq((*loopStatePtr), skipLabelExp);
}

static void combineLoopState(
    IR_myStatement* combinedStatePtr, IR_myStatement loopState)
{
    (*combinedStatePtr) = IR_makeSeq((*combinedStatePtr), loopState);
}

static void doArrayAssignmentLoop(
    IR_myStatement* combinedStatePtr, IR_myExp loopReg,
    IR_myExp subscriptValueReg, IR_myStatement skipLabelExp,
    IR_myStatement loopLabelExp, IR_myExp memoryAddrReg,
    IR_myExp initValue)
{
    combineLoopChecker(
        combinedStatePtr, loopReg, subscriptValueReg, skipLabelExp);

    IR_myStatement loopState =
        doOneElementAssignment(memoryAddrReg, initValue);
    calcNextAddress(&loopState, memoryAddrReg);
    incrementLoopReg(&loopState, loopReg);
    jumpToLoop(&loopState, loopLabelExp);
    defineSkipLabel(&loopState, skipLabelExp);

    combineLoopState(combinedStatePtr, loopState);
}

static void doArrayAssignment(IR_myStatement* combinedStatePtr, IR_myExp subscriptValueReg,
                        IR_myExp memoryAddrReg, IR_myExp initValue)
{
    IR_myExp loopReg = NULL;
    IR_myStatement loopLabelExp = NULL, skipLabelExp = NULL;

    prepareArrayAssignmentLoop(combinedStatePtr,
        &loopReg, &loopLabelExp, &skipLabelExp);

    doArrayAssignmentLoop(
        combinedStatePtr, loopReg, subscriptValueReg,
        skipLabelExp, loopLabelExp, memoryAddrReg, initValue);
}

IR_myExp Trans_arrayCreation(IR_myExp subscriptResult, IR_myExp initValueResult)
{
    IR_myExp memoryAddrReg = NULL;
    IR_myExp subscriptValueReg = NULL;
    IR_myStatement combinedState = translateSpaceAllocation(
        subscriptResult, &subscriptValueReg, &memoryAddrReg);

    IR_myExp resultAddr = saveResultForReturn(memoryAddrReg, &combinedState);
    IR_myExp initValue = combineInitState(initValueResult, &combinedState);
    doArrayAssignment(&combinedState, subscriptValueReg, memoryAddrReg, initValue);
    return IR_makeESeq(combinedState, resultAddr);
}

/////////////////////////////////////////////////////////////////////////////////

IR_myExp Trans_noFieldRecordCreation()
{
    //  tiger supports empty record type, so although empty,
    //  we must allocate some space for it.
    IR_myExp constOne = IR_makeConst(1);
    return Frame_externalCall(
        "malloc", IR_makeExpList(constOne, NULL));
}

/////////////////////////////////////////////////////////////////////////////////

static IR_myStatement calcAndletTempRegHoldResult(
    IR_myStatement leftState, IR_myStatement rightState,
    IR_myExp leftValueReg, IR_myExp rightValueReg,
    IR_BinOperator op, IR_myExp* tempRegPtr)
{
    //      when value is holded in a register, we cannot adjust it, which
    //  will cause the original variable changed. So, we make a new register
    //  hold the result, namely a temporary variable representation.
    IR_myExp tempReg = IR_makeTemp(Temp_newTemp());
    *tempRegPtr = tempReg;

    return IR_makeSeq(
        IR_makeSeq(
            IR_makeSeq(leftState, rightState),
            IR_makeMove(tempReg, leftValueReg)),
        IR_makeExp(IR_makeBinOperation(op, tempReg, rightValueReg)));
}

IR_myExp Trans_arithmetic(IR_myExp leftTran, IR_myExp rightTran, IR_BinOperator op)
{
    IR_myStatement leftState;
    IR_myExp leftValueReg;
    IR_divideExp(leftTran, &leftState, &leftValueReg);

    IR_myStatement rightState;
    IR_myExp rightValueReg;
    IR_divideExp(rightTran, &rightState, &rightValueReg);

    IR_myExp tempReg;
    IR_myStatement resultState = calcAndletTempRegHoldResult(
        leftState, rightState, leftValueReg, rightValueReg,
        op, &tempReg);
    return IR_makeESeq(resultState, tempReg);
}

///////////////////////////////////////////////////////////////////////////////////

static void translateLoopVarInitial(
    Trans_myAccess loopVarAccess, IR_myExp lowRangeResult,
    IR_myStatement* stateResultPtr, IR_myExp* loopVarRepPtr)
{
    IR_myExp loopVarInitialResult =
        doAssignmentTranslation(loopVarAccess, lowRangeResult);

    (*stateResultPtr) = loopVarInitialResult->u.eseq.statement;
    (*loopVarRepPtr) = loopVarInitialResult->u.eseq.exp;
}

static void translateHighRange(
    IR_myExp highRangeResult, IR_myStatement* stateResultPtr,
    IR_myExp* highRangeRepPtr)
{
    IR_myStatement highState;
    IR_divideExp(highRangeResult, &highState, highRangeRepPtr);

    (*stateResultPtr) = IR_makeSeq((*stateResultPtr), highState);
}

static IR_myStatement translateLoopLabelDefinition(
    IR_myStatement loopLabelExp)
{
    return loopLabelExp;
}

static void translateLoopChecker(IR_myStatement skipCjumpStatement,
    IR_myStatement skipLabelExp, IR_myStatement* stateResultPtr)
{
    skipCjumpStatement->u.cjump.trueLabel = skipLabelExp->u.label;
    skipCjumpStatement->u.cjump.falseLabel = NULL;
    (*stateResultPtr) = IR_makeSeq((*stateResultPtr), skipCjumpStatement);
}

static void translateLoopBody(
    IR_myStatement loopState, IR_myStatement* stateResultPtr)
{
    (*stateResultPtr) = IR_makeSeq((*stateResultPtr), loopState);
}

static void translateLoopRegIncrement(
    IR_myExp loopReg, IR_myStatement* stateResultPtr)
{
    (*stateResultPtr) = IR_makeSeq(
        (*stateResultPtr), 
        IR_makeExp(IR_makeBinOperation(IR_Plus, loopReg, IR_makeConst(1))));
}

static void translateLoopJumper(
    IR_myStatement loopLabelExp, IR_myStatement* stateResultPtr)
{
    (*stateResultPtr) = IR_makeSeq((*stateResultPtr),
        IR_makeJump(IR_makeName(loopLabelExp->u.label),
            Temp_makeLabelList(loopLabelExp->u.label, NULL)));
}

static void translateSkipLabelDefinition(
    IR_myStatement skipLabelExp, IR_myStatement* stateResultPtr)
{
    (*stateResultPtr) = IR_makeSeq((*stateResultPtr), skipLabelExp);
}

static IR_myStatement translateCommonLoop(
    IR_myStatement skipCjumpStatementWithoutLabel, IR_myStatement loopState)
{
    assert (skipCjumpStatementWithoutLabel->kind == IR_CJump);
    assert (skipCjumpStatementWithoutLabel->u.cjump.left->kind == IR_Temp);

    IR_myExp loopReg = skipCjumpStatementWithoutLabel->u.cjump.left;
    IR_myStatement skipLabelExp = IR_makeLabel(Temp_newLabel());
    IR_myStatement loopLabelExp = IR_makeLabel(Temp_newLabel());

    IR_myStatement stateResult =
        translateLoopLabelDefinition(loopLabelExp);
    translateLoopChecker(skipCjumpStatementWithoutLabel, skipLabelExp, &stateResult);
    translateLoopBody(loopState, &stateResult);
    translateLoopJumper(loopLabelExp, &stateResult);
    translateSkipLabelDefinition(skipLabelExp, &stateResult);

    return stateResult;
}

static IR_myStatement translateInitDefinition(
    Trans_myAccess loopVarAccess, IR_myExp lowRangeResult,
    IR_myExp highRangeResult, IR_myExp* loopVarRepPtr,
    IR_myExp* highRangeRepPtr)
{
    IR_myStatement initResult;
    translateLoopVarInitial(
        loopVarAccess, lowRangeResult, &initResult, loopVarRepPtr);
    translateHighRange(highRangeResult, &initResult, highRangeRepPtr);

    return initResult;
}

static IR_myStatement translateLoopDefinition(
    IR_myExp loopVarRep, IR_myExp highRangeRep, IR_myExp loopBodyResult)
{
    IR_myStatement skipCjumpstateWithoutLabel = IR_makeCJump(
        IR_GreaterThan, loopVarRep, highRangeRep, NULL, NULL);
    
    IR_myStatement loopState;
    IR_myExp loopValue;
    IR_divideExp(loopBodyResult, &loopState, &loopValue);
    loopState = IR_makeSeq(loopState, IR_makeExp(IR_makeBinOperation(IR_Plus,
        loopVarRep, IR_makeConst(1))));

     return translateCommonLoop(skipCjumpstateWithoutLabel, loopState);
}

IR_myExp Trans_for(
    IR_myExp lowRangeResult, IR_myExp highRangeResult,
    Trans_myAccess loopVarAccess, IR_myExp loopBodyResult)
{
    IR_myExp loopVarRep;
    IR_myExp highRangeRep;
    IR_myStatement initResult =
        translateInitDefinition(loopVarAccess, lowRangeResult, highRangeResult,
            &loopVarRep, &highRangeRep);

    IR_myStatement loopResult =
        translateLoopDefinition(loopVarRep, highRangeRep, loopBodyResult);
    //  for expressions have no value
    return IR_makeESeq(
         IR_makeSeq(initResult, loopResult), NULL);
}

////////////////////////////////////////////////////////////////////////////////

static void translateCondition(
    IR_myExp conditionTrans, IR_myStatement* stateReturnPtr, myLabel elseLabel)
{
    IR_myExp condiValue = NULL;
    IR_divideExp(conditionTrans, stateReturnPtr, &condiValue);

    //  if non-zero, fall through
    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr),
        IR_makeCJump(IR_Equal, condiValue, IR_makeConst(0),
            elseLabel, NULL));
}

static void defineLabel(IR_myStatement* stateReturnPtr, myLabel label)
{
    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr), IR_makeLabel(label));
} 

static IR_myExp defineClause(
    IR_myStatement* stateReturnPtr, IR_myExp clauseTrans)
{
    IR_myStatement clauseState;
    IR_myExp clauseValue;
    IR_divideExp(clauseTrans, &clauseState, &clauseValue);
    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr), clauseState);

    //  may have value(IR_TEMP) or not(NULL)
    assert (clauseValue == NULL || clauseValue->kind == IR_Temp);
    return clauseValue;
}

static void defineValueSaving(
    IR_myStatement* stateReturnPtr, IR_myExp* valueReturnPtr, IR_myExp valueRep)
{
    assert ((*valueReturnPtr)->kind == IR_Temp);
    assert (valueRep->kind == IR_Temp);

    (*stateReturnPtr) = IR_makeSeq(
            (*stateReturnPtr),
            IR_makeMove((*valueReturnPtr), valueRep));
}

static void translateThenClause(
    IR_myExp thenClauseTrans, IR_myStatement* stateReturnPtr,
    IR_myExp* valueReturnPtr, myLabel endLabel, bool hasReturn)
{
    IR_myExp thenValue = defineClause(stateReturnPtr, thenClauseTrans);
    if (hasReturn)
        defineValueSaving(stateReturnPtr, valueReturnPtr, thenValue);
    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr),
        IR_makeJump(IR_makeName(endLabel), Temp_makeLabelList(endLabel, NULL)));
}

static void translateElseClause(
    IR_myExp elseTrans, myLabel elseLabel,
    IR_myStatement* stateReturnPtr, IR_myExp* valueReturnPtr,
    bool hasReturn)
{
    defineLabel(stateReturnPtr, elseLabel);
    IR_myExp elseValue = defineClause(stateReturnPtr, elseTrans);
    if (hasReturn)
        defineValueSaving(stateReturnPtr, valueReturnPtr, elseValue);
}

IR_myExp Trans_ifThenElse(
    IR_myExp conditionTrans, IR_myExp thenTrans, IR_myExp elseTrans, bool hasReturn)
{
    IR_myStatement stateReturn;
    IR_myExp valueReturn = IR_makeTemp(Temp_newTemp());
    myLabel elseLabel = Temp_newLabel();
    myLabel endLabel = Temp_newLabel();

    translateCondition(conditionTrans, &stateReturn, elseLabel);
    translateThenClause(thenTrans, &stateReturn, &valueReturn, endLabel, hasReturn);
    translateElseClause(elseTrans, elseLabel, &stateReturn, &valueReturn, hasReturn);
    defineLabel(&stateReturn, endLabel);

    if (!hasReturn)
        valueReturn = NULL;
    return IR_makeESeq(stateReturn, valueReturn);
}

////////////////////////////////////////////////////////////////////////

IR_myExp Trans_ifThen(IR_myExp conditionTrans, IR_myExp thenTrans)
{
    IR_myStatement stateReturn;
    myLabel skipLabel = Temp_newLabel();

    translateCondition(conditionTrans, &stateReturn, skipLabel);
    defineClause(&stateReturn, thenTrans);
    defineLabel(&stateReturn, skipLabel);

    //  no return value
    return IR_makeESeq(stateReturn, NULL);
}

///////////////////////////////////////////////////////////////////////

//  NOTE: stateResultPtr must be already initialized!!!!
static IR_myExp combineOneTrans(
    IR_myExp oneOperandTrans, IR_myStatement* stateReturnPtr)
{
    IR_myStatement stateOne;
    IR_myExp valueOne;
    IR_divideExp(oneOperandTrans, &stateOne, &valueOne);

    (*stateReturnPtr) = IR_makeSeq((*stateReturnPtr), stateOne);
    return valueOne;
}

static void jumpToResultSettingStateByEqualValueAndOperator(
    IR_myStatement* stateReturnPtr, IR_myExp compareResultReg,
    myLabel assignTrueLabel, int equalValue, IR_RelOperator equalOp)
{
    assert (compareResultReg->kind == IR_Temp);

    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr), 
        IR_makeCJump(
            equalOp, compareResultReg, IR_makeConst(equalValue),
            assignTrueLabel, NULL));
}

static void assignFalseToResultRegAndJumpToEnd(
    IR_myStatement* stateReturnPtr, IR_myExp resultReg, myLabel endLabel)
{
    assert (resultReg->kind == IR_Temp); 

    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr), IR_makeMove(resultReg, IR_makeConst(0)));
    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr),
        IR_makeJump(IR_makeName(endLabel), Temp_makeLabelList(endLabel, NULL)));
}

static void assignTrueToResultRegAndDefineEndLabel(
    IR_myStatement* stateReturnPtr, myLabel assignTrueLabel,
    IR_myExp resultReg, myLabel endLabel)
{
    assert (resultReg->kind == IR_Temp);

    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr), IR_makeLabel(assignTrueLabel));
    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr), IR_makeMove(resultReg, IR_makeConst(1)));
    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr), IR_makeLabel(endLabel));
}

//////////////////////////////////////////

static IR_myExp regToCompareResult_Common(
    IR_myExp compareResultReg, int equalValue, IR_RelOperator equalOP)
{
    IR_myStatement stateReturn = NULL;

    IR_myExp newReg = IR_makeTemp(Temp_newTemp());
    myLabel assignOneLabel = Temp_newLabel();
    myLabel endLabel = Temp_newLabel();

    jumpToResultSettingStateByEqualValueAndOperator(
        &stateReturn, compareResultReg, assignOneLabel,
        equalValue, equalOP);
    assignFalseToResultRegAndJumpToEnd(
        &stateReturn, newReg, endLabel);
    assignTrueToResultRegAndDefineEndLabel(
        &stateReturn, assignOneLabel, newReg, endLabel);

    return IR_makeESeq(stateReturn, newReg);
}

//  the return value of strCompare() if string equals
static const int g_callReturnValueIfEqual = 0;


static IR_myExp regToCompareResult_Equal(
    IR_myExp compareResultReg)
{
    return regToCompareResult_Common(
        compareResultReg, g_callReturnValueIfEqual, IR_Equal);
}

static IR_myExp regToCompareResult_NotEqual(IR_myExp compareResultReg)
{
    IR_myExp ret = regToCompareResult_Equal(compareResultReg);
    IR_myExp resultReg = ret->u.eseq.exp;

    IR_myStatement notResult = IR_makeExp(
        IR_makeBinOperation(IR_Xor, resultReg, IR_makeConst(1)));
    return IR_makeESeq(
        IR_makeSeq(ret->u.eseq.statement, notResult),
        resultReg);
}

static IR_myExp regToCompareResult_GreaterThan(
    IR_myExp compareResultReg)
{
    return regToCompareResult_Common(
        compareResultReg, g_callReturnValueIfEqual, IR_GreaterThan);
}

static IR_myExp regToCompareResult_LessThan(
    IR_myExp compareResultReg)
{
    return regToCompareResult_Common(
        compareResultReg, g_callReturnValueIfEqual, IR_LessThan);
}



static IR_myExp regToCompareResult_GreaterEqual(
    IR_myExp compareResultReg)
{
    return regToCompareResult_Common(
        compareResultReg, g_callReturnValueIfEqual, IR_GreaterEqual);
}

static IR_myExp regToCompareResult_LessEqual(
    IR_myExp compareResultReg)
{
    return regToCompareResult_Common(
        compareResultReg, g_callReturnValueIfEqual, IR_LessEqual);
}

///////////////////////

static IR_myExp regToCompareResult(
    IR_myExp compareResultReg, IR_RelOperator op)
{
    assert (compareResultReg->kind == IR_Temp);
    switch (op)
    {
        case IR_Equal:
            return regToCompareResult_Equal(compareResultReg);
        case IR_NotEqual:
            return regToCompareResult_NotEqual(compareResultReg);
        case IR_GreaterThan:
            return regToCompareResult_GreaterThan(compareResultReg);
        case IR_LessThan:
            return regToCompareResult_LessThan(compareResultReg);
        case IR_GreaterEqual:
            return regToCompareResult_GreaterEqual(compareResultReg);
        case IR_LessEqual:
            return regToCompareResult_LessEqual(compareResultReg);
        default:    assert (false);
    }
}

static IR_myExp prepareStringRegForConvert(
    IR_myExp leftValue, IR_myExp rightValue, IR_myStatement* stateReturnPtr)
{
    //  todo:   a runtime call function---strCompare(,)
    //        return -1, 0, 1 if less, equal or greater
    IR_myExp callExp = IR_makeCall(
            IR_makeName(Temp_newNamedLabel("strCompare")),
            IR_makeExpList(leftValue, IR_makeExpList(rightValue, NULL)));
    return combineOneTrans(callExp, stateReturnPtr);
}

static IR_myExp prepareOthersRegForConvert(
    IR_myExp leftValue, IR_myExp rightValue, IR_myStatement* stateReturnPtr)
{
    //  array and record variables are actually pointers, treat them equally
    (*stateReturnPtr) = IR_makeSeq(
            (*stateReturnPtr),
            IR_makeExp(IR_makeBinOperation(IR_Minus, leftValue, rightValue)));
    return leftValue;
}

static IR_myExp convertToReturn(
    IR_myExp regToConvert, IR_RelOperator op, IR_myStatement* stateReturnPtr)
{
    IR_myExp convertRet = regToCompareResult(regToConvert, op);
    assert (convertRet->kind == IR_ESeq);
    assert (convertRet->u.eseq.exp->kind == IR_Temp);

    (*stateReturnPtr) = IR_makeSeq(
        (*stateReturnPtr), convertRet->u.eseq.statement);
    return IR_makeESeq((*stateReturnPtr), convertRet->u.eseq.exp);
}

IR_myExp Trans_comparison(
    IR_myExp leftTrans, IR_myExp rightTrans, IR_RelOperator op, bool isString)
{
    IR_myStatement stateResult = NULL;
    IR_myExp regToConvert = NULL;

    IR_myExp leftValue = combineOneTrans(leftTrans, &stateResult);
    IR_myExp rightValue = combineOneTrans(rightTrans, &stateResult);

    if (isString)
    {
        regToConvert =
            prepareStringRegForConvert(leftValue, rightValue, &stateResult);
    }
    else
    {
        regToConvert =
            prepareOthersRegForConvert(leftValue, rightValue, &stateResult);
    }

    return convertToReturn(regToConvert, op, &stateResult);
}

////////////////////////////////////////////////////////////////////////////

static void combineOperandAndSaveResultToNewReg(
    IR_myExp leftTrans, IR_myExp newReg, IR_myStatement* stateReturnPtr)
{
    IR_myExp leftValue = combineOneTrans(leftTrans, stateReturnPtr);
    (*stateReturnPtr) = IR_makeSeq((*stateReturnPtr), IR_makeMove(newReg, leftValue));
}

static void skipToEndIfLeftResultIsValueGiven(
    IR_myExp resultReg, myLabel endLabel, int value, IR_myStatement* stateReturnPtr)
{
    jumpToResultSettingStateByEqualValueAndOperator(
        stateReturnPtr, resultReg, endLabel, value, IR_Equal);
}

static void skipToEndIfLeftResultIsTrue(
    IR_myExp resultReg, myLabel endLabel, IR_myStatement* stateReturnPtr)
{
    skipToEndIfLeftResultIsValueGiven(resultReg, endLabel, true, stateReturnPtr);
}

static void skipToEndIfLeftResultIsFalse(
    IR_myExp resultReg, myLabel endLabel, IR_myStatement* stateReturnPtr)
{
    skipToEndIfLeftResultIsValueGiven(resultReg, endLabel, false, stateReturnPtr);
}

IR_myExp Trans_booleanOperate(
    IR_myExp leftTrans, IR_myExp rightTrans, IR_BinOperator op)
{
    IR_myStatement stateReturn = NULL;
    IR_myExp resultReg = IR_makeTemp(Temp_newTemp());
    myLabel endLabel = Temp_newLabel();

    combineOperandAndSaveResultToNewReg(leftTrans, resultReg, &stateReturn);

    if (op == IR_Or)
        skipToEndIfLeftResultIsTrue(resultReg, endLabel, &stateReturn);
    else
        skipToEndIfLeftResultIsFalse(resultReg, endLabel, &stateReturn);
        
    combineOperandAndSaveResultToNewReg(rightTrans, resultReg, &stateReturn);
    defineLabel(&stateReturn, endLabel);

    return IR_makeESeq(stateReturn, resultReg);
}