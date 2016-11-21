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
    /*IR_myStatement calcuStatement = varRepresent->u.eseq.statement;
    IR_myExp effectValue = varRepresent->u.eseq.exp;*/
        
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
    return IR_makeESeq(resultStatement, IR_makeMem(calcValue));
}

IR_myExp doAssignment(Trans_myAccess varAccess, IR_myExp varBodyResult)
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


////////////////////////////////////////////////////////////////////////////////

static void getVarNameAndBody(
    myVarDec varDec, mySymbol* varNamePtr, myExp* varBodyPtr)
{
    switch (varDec->kind)
    {
        case LongFormVar:
            *varNamePtr = varDec->u.longFormVar->name;
            *varBodyPtr = varDec->u.longFormVar->exp;
            break;
        case ShortFormVar:
            *varNamePtr = varDec->u.shortFormVar->name;
            *varBodyPtr = varDec->u.shortFormVar->exp;
            break;
        default:    assert (false);
    }
}

IR_myStatement Trans_VarDec(myVarDec varDec)
{
    mySymbol varName;
    myExp varBody;
    getVarNameAndBody(varDec, &varName, &varBody);
 
    return doAssignment(getVarAccessFromName(varName), Trans_Exp_(varBody))
        ->u.eseq.statement;
}

////////////////////////////////////////////////////////////////
/*
void getFuncParts(
    myFuncDec funcDec, mySymbol* funcNamePtr,
    myExp* funcBodyPtr, myTyFieldList* funcFormalsPtr)
{
    switch (funcDec->kind)
    {
        case ProcedureDec:
            *funcNamePtr = funcDec->u.procedureDec->name;
            *funcBodyPtr = funcDec->u.procedureDec->exp;
            *funcFormalsPtr = funcDec->u.procedureDec->tyFieldList;
            break;
        case FunctionDec:
            *funcNamePtr = funcDec->u.functionDec->name;
            *funcBodyPtr = funcDec->u.functionDec->exp;
            *funcFormalsPtr = funcDec->u.functionDec->tyFieldList;
            break;
        default:        assert (false);
    }
}

void removeFormalsFromScope()
{
    //  remove formals from environments
    MySymbol_EndScope(MySemantic_getTypeEnvironment());
    MySymbol_EndScope(MySemantic_getVarAndFuncEnvironment());
}

void processFuncDec(mySymbol funcName, myTyFieldList funcFormals, myExp funcBody)
{
    addFormalsToScope(funcName, funcFormals);

    //  no value expressions must have already been wrapped!!
    IR_myStatement funcState = IR_makeExp(Trans_Exp_(funcBody));
    Frame_myFrag procFrag = Frame_makeProcFrag(
        funcState,
        MySemantic_getCurrentLevel()->frame,
        funcName);
    Trans_addOneProcFrag(procFrag);

    removeFormalsFromScope();
}

IR_myStatement Trans_FuncDec(myFuncDec funcDec)
{
    mySymbol funcName;
    myExp funcBody;
    myTyFieldList funcFormals;

    getFuncParts(funcDec, &funcName, &funcBody, &funcFormals);
    processFuncDec(funcName, funcFormals, funcBody);
    return NULL;    //  function does not need return code
}
*/
////////////////////////////////////////////////////////////////
/*
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
}*/

IR_myExp Trans_LetExp(myLetExp letExp)
{/*
     IR_myStatement decsResult = Trans_Decs(letExp->decList);
     IR_myExp expsResult = Trans_Exps(letExp->expList);

     return IR_makeESeq(decsResult, expsResult);*/
     return NULL;
}

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