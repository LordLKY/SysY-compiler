#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Table.h"
#include "IRModule.h"

// using namespace std;

typedef enum{
    _INTEGER = 258,
    _DECIMAL,
    _IDENTIFIER,
    _WHILE,
    _IF,
    _ELSE,
    _BREAK,
    _CONTINUE,
    _RETURN,
    _INT,
    _FLOAT,
    _VOID,
    _CONST,
    _OR,
    _AND,
    _GE,
    _LE,
    _EQ,
    _NE,
    _PRINT,
    _CompUnits,
    _ArrayDef,
    _ConstDef,
    _ConstArrayInitVal,
    _ConstDefs,
    _VarDef,
    _VarArrayInitVal,
    _VarDefs,
    _FuncDef,
    _FuncFParams,
    _FuncFParam,
    _ArrayExp,
    _BlockItems,
    _FuncCall,
    _FuncRParams
}all_op;

typedef enum{I, F, Id, Exp}nodeEnum;

class astNode;

class intNode{
public:
    int val_type;
    int value;

    intNode(){val_type = CONST_I32;}
};

class floatNode{
public:
    int val_type;
    float value;

    floatNode(){val_type = CONST_F32;}
};

class identNode{
public:
    int val_type;
    int sym_type;
    string name;
};

class expNode{
public:
    int op;
    int num;
    int val_type; // type of the expNode
    int cal; // whether the expNode can be caculated during compiling
    float cal_val; // if(cal) return result 
    vector<astNode *> pNodes;

    Value *Exp2IR();
};

union nodeUnion{
    intNode int_node;
    floatNode float_node;
    identNode ident_node;
    expNode exp_node;

    nodeUnion(){}
    ~nodeUnion(){}
};

class astNode{
public:
    nodeEnum type;
    nodeUnion node;

    Value *AST2IR();
    void AST_delete(); // release the space of AST
};

struct ArrayValStruct{
    int pos;
    astNode *p;

    ArrayValStruct(int pos0, astNode *p0):p(p0), pos(pos0){}
}; // used to deal with ArrayInitVal


/* funcs to preprocess sym in parser */
void sym_preprocess_datadef(int val_type, astNode *p);
void sym_preprocess_simple(int val_type, astNode *p1, astNode *p2, bool b); // insert single symbol
void sym_preprocess_array(int val_type, astNode *p, bool b); // insert array symbol
void sym_preprocess_func(int ret_type, astNode *p1, astNode *p2); // insert params of func into the corresponding scope


/* funcs related to type */
int Type_AST(astNode *pNode); // distinguish the type of a node by returning val_type
int Type_Merge(int type1, int type2); // example: I32 + F32 -> F32
void Type_Assign(Value *v1, int type1, Value *v2, int type2); // deal with: type1 v1 = type2 v2
Value *Type_Match(int type1, Value *v2, int type2); // used in FuncCall/ret/Exp


/* funcs try to calculate Exp (only consider BinaryOp&SimpleId so far) */
void Cal4BinaryOp(int op, expNode *pNode);
bool Cal4SimpleId(identNode *pNode, float& result);


/* funcs to constrct AST */
astNode *newI(int value); // create an intNode
astNode *newF(float value); // create a floatNode
astNode *newId(int sym_type, string *name); // create an identNode
astNode *newExp(int op, int num, ...); // create an expNode
astNode *mergeNode(astNode *pNode1, astNode *pNode2); // merge two nodes (the first one is the master)


/* funcs below convert AST to IR */

/* funcs related to CompUnit */
void CompUnits2IR(expNode *pNode);


/* funcs related to Block */
void BlockItems2IR(expNode *pNode);


/* funcs related to ident_list/fparam_list in each scope (except global) */
void IdentList2IR(vector<identInfo *>& ident_list);
void FParamList2IR(vector<identInfo *>& fparam_list, bool b); // b distinguish param-declaration and param-passing


/* funcs related to Constant */
Value *IntConstant2IR(astNode *pNode);
Value *FloatConstant2IR(astNode *pNode);
void itof(Constant *V);
void ftoi(Constant *V);


/* funcs related to Def */
void Defs2IR(expNode *pNode); // deal with a group of defs
void SimpleDef2IR(expNode *pNode); // deal with the def about simple ident
void ArrayDef2IR(expNode *pNode); // deal with the def about array ident


/* funcs related to ArrayInitVal */
int ArrayInitVal_flatten(expNode *pNode, int& pos, vector<int>& dim_num_list, vector<ArrayValStruct>& flatten_array, int first_dim, bool b);
Value *ArrayInitVal2IR(int type, vector<ArrayValStruct>& flatten_array, vector<int>& dim_num_list, Value *V);


/* funcs related to simple identifier */
Value *SimpleId2IR(identNode *pNode, bool b);


/* funcs related to FuncDef */
void FuncDef2IR(expNode *pNode);
void FuncFParams2IR(vector<identInfo *>& fparam_list);


/* funcs related to AssignOp ('=') */
void AssignOp2IR(expNode *pNode);


/* funcs related to IfOp */
void IfOp2IR(expNode *pNode); // deal with if ... and if ... else ...


/* funcs related to Loop */
void WhileOp2IR(expNode *pNode);
void BreakOp2IR(expNode *pNode);
void ContinueOp2IR(expNode *pNode);


/* funcs related to ReturnOp */
void ReturnOp2IR(expNode *pNode);


/* funcs related to Op */
Value *BinaryOp2IR(int op, expNode *pNode); // deal with binary-operator
Value *CmpOp2IR(int op, expNode *pNode);


/* funcs related to LogicOp */
Value *CondOp2IR(int op, expNode *pNode);


/* funcs related to ArrayExp */
Value *ArrayExp2IR(expNode *pNode, bool b); // there are 3 types of ArrayExp: normal, FuncCall-type, FuncRParam-type


/* funcs related to FuncCall */
Value *FuncCall2IR(expNode *pNode);
Value *FuncCallArrayExp2IR(expNode *pNode, int& dim, int& dim_list_index);
Value *FuncCallIdent2IR(identNode *pNode, int& dim, int& dim_list_index); // a simple ident in FuncCall is possibly an array ident

/* funcs related to PRINT */
void Print2IR(expNode *pNode);

/* funcs for debugging */

/* print AST for test */
void PrintAST(astNode *pNode, int level);

/* print sym for test */
void PrintSYM(SYM& sym);