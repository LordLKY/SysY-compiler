#include "AST.h"
#include <cstdarg>
using namespace std;


// SYM sym;
// int TEMP = 0;
int SCOPE_INDEX = 0;
// string CUR_INFO = "";
// int CUR_INSTR = 0;
// BasicBlock *CUR_BB = NULL;
// FuncModule *CUR_FUNC = NULL;
BasicBlock *WHILE_ENTRY = NULL;
BasicBlock *WHILE_END = NULL;


Value *expNode::Exp2IR(){
    if(op == '='){AssignOp2IR(this); return NULL;} // AssignOp
    else if(op < _INTEGER){
        if(op == '>' || op == '<'){return CmpOp2IR(op, this);}
        else{return BinaryOp2IR(op, this);}
    }
    else if(op > _AND && op < _PRINT){return CmpOp2IR(op, this);}
    else if(op == _AND || op == _OR){return CondOp2IR(op, this);} // CondOp
    else if(op == _ConstDefs || op == _VarDefs){Defs2IR(this); return NULL;} // DefsOp
    else if(op == _CompUnits){CompUnits2IR(this); return NULL;} // CompUnits
    else if(op == _BlockItems){BlockItems2IR(this); return NULL;} // Block
    else if(op == _RETURN){ReturnOp2IR(this); return NULL;} // ReturnOp
    else if(op == _IF){IfOp2IR(this); return NULL;} // IfOp
    else if(op == _WHILE){WhileOp2IR(this); return NULL;} // WhileOp
    else if(op == _BREAK){BreakOp2IR(this); return NULL;} // BreakOp
    else if(op == _CONTINUE){ContinueOp2IR(this); return NULL;} // ContinueOp
    else if(op == _FuncDef){FuncDef2IR(this); return NULL;} // FuncDef
    else if(op == _ArrayExp){return ArrayExp2IR(this, true);} // ArrayExp
    else if(op == _FuncCall){return FuncCall2IR(this);}
    else if(op == _PRINT){Print2IR(this); return NULL;} // PrintOp
    else{return NULL;}
}

Value *astNode::AST2IR(){
    if(type == Exp){return node.exp_node.Exp2IR();}
    else if(type == I){return IntConstant2IR(this);}
    else if(type == F){return FloatConstant2IR(this);}
    else if(type == Id){return SimpleId2IR(&node.ident_node, true);}
    else{return NULL;}
}

void astNode::AST_delete(){
    if(type == Exp){
        for(int i = 0; i < node.exp_node.num; i++){
            node.exp_node.pNodes[i]->AST_delete();
            delete node.exp_node.pNodes[i];
        }
    }
}


/* funcs related to preprocessing */
void sym_preprocess_datadef(int val_type, astNode *p){
    expNode *pNode = &(p->node.exp_node);
    for(int i = 0; i < pNode->num; i++){
        astNode *p1 = pNode->pNodes[i];
        astNode *obj = p1->node.exp_node.pNodes[0];
        astNode *val = NULL;
        if(p1->node.exp_node.num > 1){val = p1->node.exp_node.pNodes[1];}
        if(obj->type == Id){sym_preprocess_simple(val_type, obj, val, false);}
        else{
            if(obj->node.exp_node.op == _ArrayDef){sym_preprocess_array(val_type, obj, false);}
            else{exit(-5);}
        }
    }
}

void sym_preprocess_simple(int val_type, astNode *p1, astNode *p2, bool b){
    identNode *pNode = &(p1->node.ident_node);

    valueStruct *vS = new valueStruct(val_type, Simple, false, 0);
    if(vS == NULL){exit(-1);}
    pNode->val_type = val_type;
    if(sym_insert(sym, pNode->name, vS) < 0){exit(-6);}

    if(val_type == CONST_F32 || val_type == CONST_I32){
        if(p2){
            if(p2->type == I){
                if(val_type == CONST_I32){vS->val.i = p2->node.int_node.value;}
                else{vS->val.f = (float)p2->node.int_node.value;}
            }
            else if(p2->type == F){
                if(val_type == CONST_I32){vS->val.i = (int)p2->node.float_node.value;}
                else{vS->val.f = p2->node.float_node.value;}
            }
            else if(p2->type == Id && p2->node.ident_node.sym_type == Simple){
                float result = 0;
                if(!Cal4SimpleId(&(p2->node.ident_node), result)){exit(-7);}
                if(val_type == CONST_I32){vS->val.i = (int)result;}
                else{vS->val.f = result;}
            }
            else{exit(-7);}
        }
        return;
    }

    identInfo *iI = new identInfo();
    if(iI == NULL){exit(-1);}
    iI->name = pNode->name, iI->sym_type = Simple, iI->val_type = val_type;
    iI->scope_index = sym.cur;
    if(b){
        sym.Stack[sym.Stack[sym.cur]->root]->fparam_list.push_back(iI);
        vS->def_mark = true;
    }
    else{sym.Stack[sym.Stack[sym.cur]->root]->ident_list.push_back(iI);}

    /*
    vS for simple ident:
    val_type
    sym_type: Simple
    def_mark
    val: value of the ident
    */

   Value *V = new Value(Mem, "@" + pNode->name + "_" + to_string(sym.cur), *vS);
   if(!sym.cur){V->isGlobal = true;}
   vS->V = V, iI->V = V;
}

void sym_preprocess_array(int val_type, astNode *p, bool b){
    expNode *pNode = &(p->node.exp_node);
    identNode *pNode1 = &(pNode->pNodes[0]->node.ident_node);

    valueStruct *vS = new valueStruct(val_type, Array, false, pNode->num - 1);
    if(vS == NULL){exit(-1);}
    pNode1->val_type = val_type;
    vS->val.i = sym.Array_list.size();
    if(sym_insert(sym, pNode1->name, vS) < 0){exit(-8);}

    arrayStruct *aS = new arrayStruct();
    if(aS == NULL){exit(-1);}
    aS->val_type = val_type;
    aS->dim_list.resize(pNode->num - 1);
    for(int i = 1; i < pNode->num; i++){
        int dim_type = pNode->pNodes[i]->type;
        int dim;
        if(dim_type == I){
            dim = pNode->pNodes[i]->node.int_node.value;
            if(dim < 0){exit(-9);}
        }
        else if(dim_type == Id && pNode->pNodes[i]->node.ident_node.sym_type == Simple){
            float result = 0;
            if(!Cal4SimpleId(&(pNode->pNodes[i]->node.ident_node), result)){exit(-9);}
            if(pNode->pNodes[i]->node.ident_node.val_type == CONST_F32){exit(-9);}
            dim = (int)result;
            if(dim < 0){exit(-9);}
        }
        else{exit(-9);}
        aS->dim_list[i - 1] = dim;
    }
    vS->other_index = pNode->num - 1;

    identInfo *iI = new identInfo();
    if(iI == NULL){exit(-1);}
    iI->name = pNode1->name, iI->sym_type = Array, iI->val_type = val_type;
    iI->scope_index = sym.cur;
    iI->other_index = sym.Array_list.size();
    if(b){
        sym.Stack[sym.Stack[sym.cur]->root]->fparam_list.push_back(iI);
        vS->def_mark = true;
        vS->val_type = (val_type + 9) / 2; // in func: I32 -> I32_PTR / F32 -> F32_PTR
        iI->val_type = vS->val_type;
    }
    else{sym.Stack[sym.Stack[sym.cur]->root]->ident_list.push_back(iI);}

    sym.Array_list.push_back(aS);

    /*
    vS for array ident:
    val_type
    sym_type: Array
    def_mark
    val: index in Array_list
    other_index: dimension of current ident
    */

   Value *V = new Value(Mem, "@" + pNode1->name + "_" + to_string(sym.cur), *vS);
   if(!sym.cur){V->isGlobal = true;}
   V->isArray = true;
   vS->V = V, iI->V = V;
}

void sym_preprocess_func(int ret_type, astNode *p1, astNode *p2){
    identNode *pNode1 = &(p1->node.ident_node);
    expNode *pNode2 = &(p2->node.exp_node);

    valueStruct *vS = new valueStruct(ret_type, Func, false, 0);
    if(vS == NULL){exit(-1);}
    pNode1->val_type = ret_type;
    vS->val.i = sym.Stack[sym.Stack.size() - 1]->root;
    if(sym_insert(sym, pNode1->name, vS) < 0){exit(-10);}
    vS->def_mark = true;

    sym.cur = sym.Stack[sym.Stack.size() - 1]->root; // jump into the scope of func

    // funcStruct *fS = new funcStruct();
    // if(fS == NULL){exit(-1);}
    // fS->ret_type = ret_type;
    // fS->param_list.resize(pNode2->num);
    sym.Stack[sym.cur]->ret_type = ret_type;
    for(int i = 0; i < pNode2->num; i++){
        expNode *param = &(pNode2->pNodes[i]->node.exp_node);
        // paramStruct pS;
        int val_type = param->pNodes[0]->node.int_node.value;
        if(val_type == _INT){val_type = I32;}
        else{val_type = F32;}
        if(param->pNodes[1]->type == Id){sym_preprocess_simple(val_type, param->pNodes[1], NULL, true);}
        else{sym_preprocess_array(val_type, param->pNodes[1], true);}
        // fS->param_list[i] = pS;
    }

    sym.cur = 0; // jump out of the scope of func (global scope)

    // sym.Func_list.push_back(fS);

    /*
    vS for func ident:
    val_type: ret_type
    sym_type: func
    def_mark
    val: index of the corresponding scope
    */
}


/* funcs related to type */
int Type_AST(astNode *pNode){
    if(pNode->type == I){return CONST_I32;}
    else if(pNode->type == F){return CONST_F32;}
    else if(pNode->type == Exp){return pNode->node.exp_node.val_type;}
    else if(pNode->type == Id && pNode->node.ident_node.sym_type == Simple){return pNode->node.ident_node.val_type;}
    else{return 0;}
}

int Type_Merge(int type1, int type2){
    if(type1 < 1 || type2 < 1){exit(-3);}
    if(type1 == I1){return Type_Merge(I32, type2);}
    if(type2 == I1){return Type_Merge(type1, I32);}
    if(type1 == CONST_F32 && type2 == I32){return F32;}
    else if(type1 == I32 && type2 == CONST_F32){return F32;}
    return max(type1, type2);
}

void Type_Assign(Value *v1, int type1, Value *v2, int type2){
    if(type1 == I32){
        if(type2 == I32 || type2 == CONST_I32){}
        else if(type2 == CONST_F32){ftoi((Constant *)v2);}
        else if(type2 == F32){
            Instruction *new_instr = new Ftoi(CUR_BB, v2);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), I32, Simple, 0);
            v2 = new_instr;
        }
        else if(type2 == I1){
            Instruction *new_instr = new Zext(CUR_BB, v2);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), I32, Simple, 0);
            v2 = new_instr;
        }
        else{exit(-4);}
    }
    else if(type1 == F32){
        if(type2 == CONST_I32){itof((Constant *)v2);}
        else if(type2 == CONST_F32 || type2 == F32){}
        else if(type2 == I32){
            Instruction *new_instr = new Itof(CUR_BB, v2);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), F32, Simple, 0);
            v2 = new_instr;
        }
        else if(type2 == I1){
            Instruction *new_instr0 = new Zext(CUR_BB, v2);
            CUR_BB->insertInstr(new_instr0);
            new_instr0->setUser("%" + to_string(++TEMP), I32, Simple, 0);
            Instruction *new_instr = new Itof(CUR_BB, new_instr0);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), F32, Simple, 0);
            v2 = new_instr;
        }
        else{exit(-4);}
    }
    else{exit(-35);}
    Store *instr = new Store(CUR_BB, v2, v1);
    CUR_BB->insertInstr(instr);
}

Value *Type_Match(int type1, Value *v2, int type2){
    if(type1 == I32){
        if(type2 == CONST_I32 || type2 == I32){}
        else if(type2 == CONST_F32){ftoi((Constant *)v2);}
        else if(type2 == F32){
            Instruction *new_instr = new Ftoi(CUR_BB, v2);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), I32, Simple, 0);
            return new_instr;
        }
        else if(type2 == I1){
            Instruction *new_instr = new Zext(CUR_BB, v2);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), I32, Simple, 0);
            return new_instr;
        }
        else{return NULL;}
    }
    else if(type1 == F32){
        if(type2 == CONST_I32){itof((Constant *)v2);}
        else if(type2 == CONST_F32 || type2 == F32){}
        else if(type2 == I32){
            Instruction *new_instr = new Ftoi(CUR_BB, v2);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), I32, Simple, 0);
            return new_instr;
        }
        else if(type2 == I1){
            Instruction *new_instr0 = new Zext(CUR_BB, v2);
            CUR_BB->insertInstr(new_instr0);
            new_instr0->setUser("%" + to_string(++TEMP), I32, Simple, 0);
            Instruction *new_instr = new Itof(CUR_BB, new_instr0);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), F32, Simple, 0);
            return new_instr;
        }
        else{return NULL;}
    }
    else if(type1 == I1){
        if(type2 == I32 || type2 == CONST_I32){
            valueUnion V_Const;
            V_Const.i = 0;
            Constant *V = new Constant(IntConst, V_Const);
            Instruction *new_instr = new CmpOp(_ne_, CUR_BB, v2, V);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), I1, Simple, 0);
            return new_instr;
        }
        else if(type2 == F32 || type2 == CONST_F32){
            valueUnion V_Const;
            V_Const.f = 0;
            Constant *V = new Constant(FloatConst, V_Const);
            Instruction *new_instr = new CmpOp(_ne_, CUR_BB, v2, V);
            CUR_BB->insertInstr(new_instr);
            new_instr->setUser("%" + to_string(++TEMP), I1, Simple, 0);
            return new_instr;
        }
        else if(type2 == I1){}
        else{return NULL;}
    }
    else{
        if(type1 != type2){return NULL;}
    }
    return v2;
}


/* funcs related to computation while compiling */
void Cal4BinaryOp(int op, expNode *pNode){
    if(op >= _INTEGER && (op <= _AND || op >= _PRINT)){return;}

    astNode *l = pNode->pNodes[0], *r = pNode->pNodes[1];
    float l_val, r_val;
    int l_type, r_type;

    if(l->type == I){
        l_val = l->node.int_node.value;
        l_type = CONST_I32;
    }
    else if(l->type == F){
        l_val = l->node.float_node.value;
        l_type = CONST_F32;
    }
    else if(l->type == Exp){
        Cal4BinaryOp(l->node.exp_node.op, &(l->node.exp_node));
        if(l->node.exp_node.cal){
            l_val = l->node.exp_node.cal_val;
            l_type = l->node.exp_node.val_type;
        }
        else{return;}
    }
    // else if(l->type == Id && l->node.ident_node.sym_type == Simple){
    //     if(!Cal4SimpleId(&(l->node.ident_node), l_val)){return;}
    //     else{l_type = l->node.ident_node.val_type;}
    // }
    else{return;}

    if(r->type == I){
        r_val = r->node.int_node.value;
        r_type = CONST_I32;
    }
    else if(r->type == F){
        r_val = r->node.float_node.value;
        r_type = CONST_F32;
    }
    else if(r->type == Exp){
        Cal4BinaryOp(r->node.exp_node.op, &(r->node.exp_node));
        if(r->node.exp_node.cal){
            r_val = r->node.exp_node.cal_val;
            r_type = r->node.exp_node.val_type;
        }
        else{return;}
    }
    // else if(r->type == Id && r->node.ident_node.sym_type == Simple){
    //     if(!Cal4SimpleId(&(r->node.ident_node), r_val)){return;}
    //     else{r_type = r->node.ident_node.val_type;}
    // }
    else{return;}

    pNode->val_type = Type_Merge(l_type, r_type);

    switch(op){
    case '+':
        pNode->cal_val = l_val + r_val;
        break;
    case '-':
        pNode->cal_val = l_val - r_val;
        break;
    case '*':
        pNode->cal_val = l_val * r_val;
        break;
    case '/':
        pNode->cal_val = l_val / r_val;
        break;
    case '%':
        pNode->cal_val = (int)l_val % (int)r_val;
        break;
    case _LE:
        pNode->cal_val = l_val <= r_val;
        break;
    case _GE:
        pNode->cal_val = l_val >= r_val;
        break;
    case '>':
        pNode->cal_val = l_val > r_val;
        break;
    case '<':
        pNode->cal_val = l_val < r_val;
        break;
    case _EQ:
        pNode->cal_val = l_val == r_val;
        break;
    case _NE:
        pNode->cal_val = l_val != r_val;
        break;
    }

    pNode->cal = 1;
}

bool Cal4SimpleId(identNode *pNode, float& result){
    int lval_index = 0;
    valueStruct *lval_info = sym_find(sym, pNode->name, lval_index);
    if(lval_info == NULL || lval_info->sym_type == Array){return false;}
    pNode->val_type = lval_info->val_type;
    if(pNode->val_type == CONST_I32){
        result = lval_info->val.i;
        return true;
    }
    else if(pNode->val_type == CONST_F32){
        result = lval_info->val.f;
        return true;
    }
    return false; 
}


/* funcs to constrct AST */
astNode *newI(int value){
    // value is corresponding to INTEGER
    astNode *p = new astNode();
    if(p == NULL){exit(-1);}
    p->type = I;
    p->node.int_node.value = value;
    return p;
}

astNode *newF(float value){
    // value is corresponding to DECIMAL
    astNode *p = new astNode();
    if(p == NULL){exit(-1);}
    p->type = F;
    p->node.float_node.value = value;
    return p;
}

astNode *newId(int sym_type, string *name){
    // name is corresponding to IDENTIFIER
    astNode *p = new astNode();
    if(p == NULL){exit(-1);}
    p->type = Id;
    p->node.ident_node.name = *name;
    p->node.ident_node.sym_type = sym_type;

    float result = 0;
    if(Cal4SimpleId(&(p->node.ident_node), result)){
        if(p->node.ident_node.val_type == CONST_I32){
            delete p;
            p = newI((int)result);
        }
        else if(p->node.ident_node.val_type == CONST_F32){
            delete p;
            p = newF(result);
        }
        else{}
    }

    return p;
}

astNode *newExp(int op, int num, ...){
    // ... contain a list of pNode-type yylval
    va_list ap;
    astNode *p = new astNode();
    if(p == NULL){exit(-1);}
    p->type = Exp;
    p->node.exp_node.op = op;
    p->node.exp_node.num = num;
    p->node.exp_node.cal = 0;
    p->node.exp_node.pNodes.resize(num);
    va_start(ap, num);
    for(int i = 0; i < num; i++){
        p->node.exp_node.pNodes[i] = va_arg(ap, astNode *);
    }
    va_end(ap);

    Cal4BinaryOp(op, &(p->node.exp_node));
    if(p->node.exp_node.cal){
        if(p->node.exp_node.val_type == CONST_I32){
            delete p;
            p = newI((int)p->node.exp_node.cal_val);
        }
        else if(p->node.exp_node.val_type == CONST_F32){
            delete p;
            p = newF(p->node.exp_node.cal_val);
        }
        else{}
    }

    return p;
}

astNode *mergeNode(astNode *pNode1, astNode *pNode2){
    pNode1->node.exp_node.num++;
    pNode1->node.exp_node.pNodes.push_back(pNode2);
    return pNode1;
}


/* funcs related to CompUnit */
void CompUnits2IR(expNode *pNode){
    for(int i = 0; i < pNode->num; i++){
        pNode->pNodes[i]->AST2IR();
    }
}


/* funcs related to Block */
void BlockItems2IR(expNode *pNode){
    scope_forward(sym, SCOPE_INDEX);

    scope *cur_scope = sym.Stack[sym.cur];

    if(cur_scope->level == 1){
        FParamList2IR(cur_scope->fparam_list, true);
        IdentList2IR(cur_scope->ident_list);
        FParamList2IR(cur_scope->fparam_list, false);
    }
    
    for(int i = 0; i < pNode->num; i++){
        pNode->pNodes[i]->AST2IR();
    }

    if(cur_scope->level == 1){
        if(!cur_scope->ret_mark){
            if(cur_scope->ret_type == VOID_TYPE){
                // insert "return void"
                Instruction *instr = new Ret(CUR_BB);
                CUR_BB->insertInstr(instr);
            }
            else{exit(-26);}
        }
    }

    scope_forward(sym, SCOPE_INDEX);
}


/* funcs related to ident_list/fparam_list in each scope (except global) */
void IdentList2IR(vector<identInfo *>& ident_list){
    for(int i = 0; i < ident_list.size(); i++){
        identInfo *iI = ident_list[i];
        if(iI->sym_type == Simple){
            if(iI->val_type == CONST_I32 || iI->val_type == CONST_F32){return;}
            Value *from = new Value(UndefInit, "UndefInit", iI->V->V_Data);
            Instruction *instr = new Alloca(CUR_BB, from, iI->V);
            CUR_BB->insertInstr(instr);
        }
        else if(iI->sym_type == Array){
            if(iI->val_type == CONST_I32 || iI->val_type == CONST_F32){
                Value *from = new Value(ZeroInit, "ZeroInit", iI->V->V_Data);
                Instruction *instr = new Alloca(CUR_BB, from, iI->V);
                CUR_BB->insertInstr(instr);
            }
            else{
                Value *from = new Value(UndefInit, "UndefInit", iI->V->V_Data);
                Instruction *instr = new Alloca(CUR_BB, from, iI->V);
                CUR_BB->insertInstr(instr); 
            }
        }
        else{}
    }
}

void FParamList2IR(vector<identInfo *>& fparam_list, bool b){
    if(b){
        for(int i = 0; i < fparam_list.size(); i++){
            identInfo *iI = fparam_list[i];
            Value *from = new Value(UndefInit, "UndefInit", iI->V->V_Data);
            Instruction *instr = new Alloca(CUR_BB, from, iI->V);
            CUR_BB->insertInstr(instr);
        }
    }
    else{
        vector<Param *> param_list = CUR_BB->master->ParamList;
        if(param_list.size() != fparam_list.size()){exit(-52);}
        for(int i = 0; i < param_list.size(); i++){
            // identInfo *iI = fparam_list[i];
            Value *from = param_list[i];
            Value *to = fparam_list[i]->V;
            Instruction *instr = new Store(CUR_BB, from, to);
            CUR_BB->insertInstr(instr);
        }
    }
}


/* funcs related to Constant */
Value *IntConstant2IR(astNode *pNode){
    string key = to_string(pNode->node.int_node.value);
    if(sym.IConstant_table.count(key)){return sym.IConstant_table[key];}
    valueUnion V_Const;
    V_Const.i = pNode->node.int_node.value;
    Value *V = new Constant(IntConst, V_Const);
    sym.IConstant_table[key] = V;
    return V;
}

Value *FloatConstant2IR(astNode *pNode){
    string key = to_string(pNode->node.float_node.value);
    if(sym.FConstant_table.count(key)){return sym.FConstant_table[key];}
    valueUnion V_Const;
    V_Const.f = pNode->node.float_node.value;
    Value *V = new Constant(FloatConst, V_Const);
    sym.FConstant_table[key] = V;
    return V;
}

void itof(Constant *V){
    float value = (float)V->V_Const.i;
    string key = to_string(value);
    if(sym.FConstant_table.count(key)){
        V = (Constant *)sym.FConstant_table[key];
    }
    valueUnion V_Const;
    V_Const.f = value;
    V = new Constant(FloatConst, V_Const);
    sym.FConstant_table[key] = V;
}

void ftoi(Constant *V){
    int value = (int)V->V_Const.f;
    string key = to_string(value);
    if(sym.IConstant_table.count(key)){
        V = (Constant *)sym.IConstant_table[key];
    }
    valueUnion V_Const;
    V_Const.f = value;
    V = new Constant(FloatConst, V_Const);
    sym.IConstant_table[key] = V;
}


/* funcs related to Def */
void Defs2IR(expNode *pNode){
    for(int i = 0; i < pNode->num; i++){
        expNode *p = &(pNode->pNodes[i]->node.exp_node);
        if(p->pNodes[0]->type == Id){SimpleDef2IR(p);}
        else{ArrayDef2IR(p);}
    }
}

void SimpleDef2IR(expNode *pNode){
    string l_name = pNode->pNodes[0]->node.ident_node.name;
    int lval_index = 0;
    valueStruct *lval_info = sym_find(sym, l_name, lval_index); // use sym_find here
    if(lval_info == NULL){exit(-11);}
    int l_type = lval_info->val_type;
    Value *l = lval_info->V;

    if(pNode->op == _ConstDef){return;}

    if(l->isGlobal && pNode->num == 1){
        Value *from = new Value(ZeroInit, "ZeroInit", l->V_Data);
        // Instruction *instr = new Alloca(CUR_BB, from, l);
        // CUR_BB->insertInstr(instr);
        if(!globalvar_insert(sym, l, from)){exit(-39);}
    }

    else if(pNode->num > 1){
        astNode *r_node = pNode->pNodes[1];
        Value *r = r_node->AST2IR();

        if(!(l->isGlobal)){
            if(r_node->type == I){Type_Assign(l, l_type, r, CONST_I32);}
            else if(r_node->type == F){Type_Assign(l, l_type, r, CONST_F32);}
            else if(r_node->type == Id){Type_Assign(l, l_type, r, r_node->node.ident_node.val_type);}
            else if(r_node->type == Exp){Type_Assign(l, l_type, r, r_node->node.exp_node.val_type);}
        }

        else{
            if(r_node->type == I){
                if(l_type == F32){itof((Constant *)r);}
            }
            else if(r_node->type == F){
                if(l_type == I32){ftoi((Constant *)r);} 
            }
            else{exit(-25);}
            // Instruction *instr = new Alloca(CUR_BB, r, l);
            // CUR_BB->insertInstr(instr);
            if(!globalvar_insert(sym, l, r)){exit(-39);}
        }
    }
    
    lval_info->def_mark = true;
}

void ArrayDef2IR(expNode *pNode){
    expNode *pNode1 = &(pNode->pNodes[0]->node.exp_node);
    identNode *pNode2 = &(pNode1->pNodes[0]->node.ident_node);

    string l_name = pNode2->name;
    int lval_index = 0;
    valueStruct *lval_info = sym_find(sym, l_name, lval_index); // use sym_find here
    if(lval_info == NULL){exit(-11);}
    int lval_val = lval_info->val.i;
    int l_type = lval_info->val_type;
    Value *l = lval_info->V;

    if(l->isGlobal && pNode->num == 1){
        Value *from = new Value(ZeroInit, "ZeroInit", l->V_Data);
        // Instruction *instr = new Alloca(CUR_BB, from, l);
        // CUR_BB->insertInstr(instr);
        if(!globalvar_insert(sym, l, from)){exit(-39);}
    }

    if(pNode->num > 1){
        if(pNode->pNodes[1]->type != Exp){exit(-12);}
        expNode *pNode3 = &(pNode->pNodes[1]->node.exp_node);
        vector<ArrayValStruct> flatten_array;
        int pos = 0;
        vector<int>& dim_list = sym.Array_list[lval_val]->dim_list;
        int len = dim_list.size();
        vector<int> dim_num_list(len);
        dim_num_list[len - 1] = 1;
        for(int i = 1; i < len; i++){
            dim_num_list[len - 1 - i] = dim_num_list[len - i] * dim_list[len - i];
        }
        ArrayInitVal_flatten(pNode3, pos, dim_num_list, flatten_array, dim_list[0], true);

        Value *from = ArrayInitVal2IR(l_type, flatten_array, dim_num_list, l);

        if(l->isGlobal){
            // Instruction *instr = new Alloca(CUR_BB, from, l);
            // CUR_BB->insertInstr(instr);
            if(!globalvar_insert(sym, l, from)){exit(-39);}
        }
    }

    lval_info->def_mark = true;
}


/* funcs related to ArrayInitVal */
int ArrayInitVal_flatten(expNode *pNode, int& pos, vector<int>& dim_num_list, vector<ArrayValStruct>& flatten_array, int first_dim, bool b){
    int level_record = 1, pos_record = pos, len = dim_num_list.size();
    for(int i = 0; i < pNode->num; i++){
        if(pNode->pNodes[i]->type == Exp &&
           (pNode->pNodes[i]->node.exp_node.op == _ConstArrayInitVal ||
           pNode->pNodes[i]->node.exp_node.op == _VarArrayInitVal)){
            int level = ArrayInitVal_flatten(&(pNode->pNodes[i]->node.exp_node), pos, dim_num_list, flatten_array, 0, false);
            level_record = max(level, level_record);
        }
        else{
            // cout << pos << endl;
            ArrayValStruct avS(pos, pNode->pNodes[i]);
            flatten_array.push_back(avS);
            pos++;
        }
    }
    int dim_num;
    if(b){dim_num = first_dim * dim_num_list[0];}
    else{
        if(level_record > len - 1){exit(-13);}
        dim_num = dim_num_list[len - 1 - level_record];
    }
    if(pos_record % dim_num){exit(-14);}
    if(pos - pos_record > dim_num){exit(-13);}
    pos = pos_record + dim_num;
    return level_record + 1;
}

Value *ArrayInitVal2IR(int type, vector<ArrayValStruct>& flatten_array, vector<int>& dim_num_list, Value *V){
    int n = dim_num_list.size(), len = flatten_array.size();

    if(V->isGlobal){
        GlobalArrayInit *init = new GlobalArrayInit(type);
        for(int i = 0; i < len; i++){
            int level_pos = flatten_array[i].pos;
            astNode *pNode = flatten_array[i].p;
            if(pNode->type != I && pNode->type != F){exit(-24);}
            if(type == CONST_I32 || type == I32){
                if(pNode->type == I){init->addValue(pNode->node.int_node.value, level_pos);}
                else{init->addValue((int)pNode->node.float_node.value, level_pos);}
            }
            else if(type == CONST_F32 || type == F32){
                if(pNode->type == I){init->addValue((float)pNode->node.int_node.value, level_pos);}
                else{init->addValue(pNode->node.float_node.value, level_pos);}
            }
        }
        return init;
    }

    for(int i = 0; i < len; i++){
        int level_pos = flatten_array[i].pos;
        astNode *pNode = flatten_array[i].p;
        Value *to = new Value(Reg, "%" + to_string(++TEMP), V->V_Data);
        to->V_Data.other_index = 0;
        to->isArray = true;
        Instruction *instr1 = new GetPtrElement(CUR_BB, V, to, false);
        for(int j = 0; j < n; j++){
            valueUnion V_Const;
            V_Const.i = level_pos / dim_num_list[j];
            Value *dim = new Constant(IntConst, V_Const);
            // instr1->setUse(dim, j);
            instr1->setUse(dim, j + 1);
            level_pos = level_pos % dim_num_list[j];
        } // locating
        CUR_BB->insertInstr(instr1);

        // Value *l = to;
        Value *l = instr1; // enable gep to follow SSA
        Value *r = pNode->AST2IR();
        if(type == CONST_I32){
            if(pNode->type == I){Type_Assign(l, I32, r, CONST_I32);}
            else if(pNode->type == F){Type_Assign(l, I32, r, CONST_F32);}
            // else if(pNode->type == Id){
            //     if(pNode->node.ident_node.val_type == CONST_I32){Type_Assign(l, I32, r, CONST_I32);}
            //     else if(pNode->node.ident_node.val_type == CONST_F32){Type_Assign(l, I32, r, CONST_F32);}
            //     else{exit(-15);}
            // }
            else{exit(-15);}
        }
        else if(type == CONST_F32){
            if(pNode->type == I){Type_Assign(l, F32, r, CONST_I32);}
            else if(pNode->type == F){Type_Assign(l, F32, r, CONST_F32);}
            // else if(pNode->type == Id){
            //     if(pNode->node.ident_node.val_type == CONST_I32){Type_Assign(l, F32, r, CONST_I32);}
            //     else if(pNode->node.ident_node.val_type == CONST_F32){Type_Assign(l, F32, r, CONST_F32);}
            //     else{exit(-15);}
            // }
            else{exit(-15);}
        }
        else{
            if(pNode->type == I){Type_Assign(l, type, r, CONST_I32);}
            else if(pNode->type == F){Type_Assign(l, type, r, CONST_F32);}
            else if(pNode->type == Id){Type_Assign(l, type, r, pNode->node.ident_node.val_type);}
            else if(pNode->type == Exp){Type_Assign(l, type, r, pNode->node.exp_node.val_type);}
        }
    }
    return NULL;
}


/* funcs related to simple identifier */
Value *SimpleId2IR(identNode *pNode, bool b){
    int lval_index = 0;
    valueStruct *lval_info = sym_find_def(sym, pNode->name, lval_index);
    if(lval_info == NULL || lval_info->sym_type != Simple){exit(-2);}
    else{
        pNode->val_type = lval_info->val_type;
        pNode->sym_type = Simple;
        if(!b){
            // CUR_INFO = lval_info->V->name;
            return lval_info->V;
        }
        if(pNode->val_type == CONST_I32){
            // CUR_INFO = to_string(((Constant *)lval_info->V)->V_Const.i);
            return lval_info->V;
        }
        else if(pNode->val_type == CONST_F32){
            // CUR_INFO = to_string(((Constant *)lval_info->V)->V_Const.f);
            return lval_info->V;
        }
        Value *to = new Value(Reg, "%" + to_string(++TEMP), lval_info->V->V_Data);
        Instruction *new_instr = new Load(CUR_BB, lval_info->V, to);
        CUR_BB->insertInstr(new_instr);
        return to;
    }
}


/* funcs related to FuncDef */
void FuncDef2IR(expNode *pNode){
    string name = pNode->pNodes[0]->node.ident_node.name;
    int ret_type = pNode->pNodes[0]->node.ident_node.val_type;
    valueStruct func_info(ret_type, Func, true, 0);
    FuncModule *funcmodule = new FuncModule(name, func_info);
    BasicBlock *entry = new BasicBlock(funcmodule);
    CUR_FUNC = funcmodule;
    CUR_BB = entry;

    sym.Function_table[name] = funcmodule; // it has to be put here in case of func iteration

    if(pNode->num == 2){
        pNode->pNodes[1]->AST2IR();
    }
    else{
        FuncFParams2IR(sym.Stack[sym.scope_trace[SCOPE_INDEX + 1]]->fparam_list);
        pNode->pNodes[2]->AST2IR();
    }
    // sym.Function_table[name] = funcmodule;
}

void FuncFParams2IR(vector<identInfo *>& fparam_list){
    for(int i = 0; i < fparam_list.size(); i++){
        identInfo *iI = fparam_list[i];
        Param *param = new Param("%" + iI->name, CUR_FUNC);
        param->V_Data = iI->V->V_Data;
        param->isArray = iI->V->isArray;
        CUR_FUNC->addParam(param);
    }
}


/* funcs related to AssignOp ('=') */
void AssignOp2IR(expNode *pNode){
    astNode *pNode1 = pNode->pNodes[0];
    Value *l = NULL;
    if(pNode1->type == Id){
        l = SimpleId2IR(&(pNode1->node.ident_node), false);
    }
    else if(pNode1->type == Exp && pNode1->node.exp_node.op == _ArrayExp){
        l = ArrayExp2IR(&(pNode1->node.exp_node), false);
    }
    else{exit(-16);}
    int l_val_type = Type_AST(pNode->pNodes[0]);
    if(l_val_type == CONST_I32 || l_val_type == CONST_F32){exit(-16);}
    Value *r = pNode->pNodes[1]->AST2IR();
    int r_val_type = Type_AST(pNode->pNodes[1]);
    Type_Assign(l, l_val_type, r, r_val_type);
}


/* funcs related to IfOp */
void IfOp2IR(expNode *pNode){
    BasicBlock *br1 = new BasicBlock(CUR_FUNC); // if then
    BasicBlock *br2 = NULL; // if else
    if(pNode->num == 3){br2 = new BasicBlock(CUR_FUNC);}
    BasicBlock *br3 = new BasicBlock(CUR_FUNC); // if end

    Value *cond = pNode->pNodes[0]->AST2IR();
    int cond_val_type = Type_AST(pNode->pNodes[0]);
    Value *new_cond = Type_Match(I1, cond, cond_val_type);
    if(new_cond == NULL){exit(-40);}

    if(pNode->num == 2){
        Instruction *instr1 = new Br(CUR_BB, new_cond, br1, br3);
        CUR_BB->insertInstr(instr1);

        CUR_BB = br1;
        Value *stmt = pNode->pNodes[1]->AST2IR();
        Instruction *instr2 = new Jump(CUR_BB, br3);
        CUR_BB->insertInstr(instr2);
        CUR_BB = br3;
    }
    else if(pNode->num == 3){
        Instruction *instr1 = new Br(CUR_BB, new_cond, br1, br2);
        CUR_BB->insertInstr(instr1);

        CUR_BB = br1;
        pNode->pNodes[1]->AST2IR();
        Instruction *instr2 = new Jump(CUR_BB, br3);
        CUR_BB->insertInstr(instr2);

        CUR_BB = br2;
        pNode->pNodes[2]->AST2IR();
        Instruction *instr3 = new Jump(CUR_BB, br3);
        CUR_BB->insertInstr(instr3);
        CUR_BB = br3;
    }
}


/* funcs related to Loop */
void WhileOp2IR(expNode *pNode){
    BasicBlock *br1 = new BasicBlock(CUR_FUNC); // while entry
    BasicBlock *br2 = new BasicBlock(CUR_FUNC); // while body
    BasicBlock *br3 = new BasicBlock(CUR_FUNC); // while end

    Instruction *instr0 = new Jump(CUR_BB, br1);
    CUR_BB->insertInstr(instr0);

    CUR_BB = br1;
    Value *cond = pNode->pNodes[0]->AST2IR();
    int cond_val_type = Type_AST(pNode->pNodes[0]);
    Value *new_cond = Type_Match(I1, cond, cond_val_type);
    if(new_cond == NULL){exit(-41);}
    Instruction *instr1 = new Br(CUR_BB, new_cond, br2, br3);
    CUR_BB->insertInstr(instr1);
    WHILE_ENTRY = br1;
    WHILE_END = br3;

    CUR_BB = br2;
    pNode->pNodes[1]->AST2IR();
    Instruction *instr3 = new Jump(CUR_BB, br1);
    CUR_BB->insertInstr(instr3);

    CUR_BB = br3;
    WHILE_ENTRY = NULL;
    WHILE_END = NULL;
}

void BreakOp2IR(expNode *pNode){
    if(WHILE_END == NULL){exit(-36);}
    Instruction *instr = new Jump(CUR_BB, WHILE_END);
    CUR_BB->insertInstr(instr);
    CUR_BB->ret_mark = 1;
}

void ContinueOp2IR(expNode *pNode){
    if(WHILE_ENTRY == NULL){exit(-37);}
    Instruction *instr = new Jump(CUR_BB, WHILE_ENTRY);
    CUR_BB->insertInstr(instr);
    CUR_BB->ret_mark = 1;
}


/* funcs related to ReturnOp */
void ReturnOp2IR(expNode *pNode){
    int r_type;
    int ret_type = sym.Stack[sym.Stack[sym.cur]->root]->ret_type;
    Value *r = new Value(UndefInit, "UndefInit");
    if(pNode->num == 0){
        if(Type_Match(ret_type, r, VOID_TYPE) == NULL){exit(-34);}
        Instruction *instr = new Ret(CUR_BB);
        CUR_BB->insertInstr(instr);
    }
    else{
        r = pNode->pNodes[0]->AST2IR();
        r_type = Type_AST(pNode->pNodes[0]);
        Value *new_r = Type_Match(ret_type, r, r_type);
        if(new_r == NULL){exit(-34);}
        Instruction *instr = new Ret(CUR_BB, new_r);
        CUR_BB->insertInstr(instr);
    }
    sym.Stack[sym.cur]->ret_mark = 1, CUR_BB->ret_mark = 1; // termiante when meet "return"
}


/* funcs related to Op */
Value *BinaryOp2IR(int op, expNode *pNode){
    Value *l = pNode->pNodes[0]->AST2IR();
    int l_val_type = Type_AST(pNode->pNodes[0]);
    Value *r = pNode->pNodes[1]->AST2IR();
    int r_val_type = Type_AST(pNode->pNodes[1]);
    int exp_val_type = Type_Merge(l_val_type, r_val_type);
    pNode->val_type = exp_val_type;
    Value *new_l = Type_Match(exp_val_type, l, l_val_type);
    Value *new_r = Type_Match(exp_val_type, r, r_val_type);
    if(new_l == NULL || new_r == NULL){exit(-22);}
    int exp_op;
    switch(op){
    case '+':
        exp_op = _add_; break;
    case '-':
        exp_op = _sub_; break;
    case '*':
        exp_op = _mul_; break;
    case '/':
        exp_op = _div_; break;
    case '%':
        exp_op = _mod_; break;
    }
    Instruction *instr = new BiOp(exp_op, CUR_BB, new_l, new_r);
    instr->setUser("%" + to_string(++TEMP), exp_val_type, Simple, 0);
    CUR_BB->insertInstr(instr);
    return instr;
}

Value *CmpOp2IR(int op, expNode *pNode){
    Value *l = pNode->pNodes[0]->AST2IR();
    int l_val_type = Type_AST(pNode->pNodes[0]);
    Value *r = pNode->pNodes[1]->AST2IR();
    int r_val_type = Type_AST(pNode->pNodes[1]);
    int exp_val_type = Type_Merge(l_val_type, r_val_type);
    pNode->val_type = I1;
    Value *new_l = Type_Match(exp_val_type, l, l_val_type);
    Value *new_r = Type_Match(exp_val_type, r, r_val_type);
    if(new_l == NULL || new_r == NULL){exit(-22);}
    int exp_op;
    switch(op){
    case _LE:
        exp_op = _le_; break;
    case _GE:
        exp_op = _ge_; break;
    case '>':
        exp_op = _gt_; break;
    case '<':
        exp_op = _lt_; break;
    case _EQ:
        exp_op = _eq_; break;
    case _NE:
        exp_op = _ne_; break;
    }
    Instruction *instr = new CmpOp(exp_op, CUR_BB, new_l, new_r);
    instr->setUser("%" + to_string(++TEMP), I1, Simple, 0);
    CUR_BB->insertInstr(instr);
    return instr;
}


/* funcs related to LogicOp */
Value *CondOp2IR(int op, expNode *pNode){
    BasicBlock *br1 = new BasicBlock(CUR_FUNC); // if then
    BasicBlock *br2 = new BasicBlock(CUR_FUNC); // if end

    pNode->val_type = I1;
    Value *l = pNode->pNodes[0]->AST2IR();
    int l_val_type = Type_AST(pNode->pNodes[0]);
    Value *new_l = Type_Match(I1, l, l_val_type);
    if(new_l == NULL){exit(-38);}
    Value *Bool = new Value(Mem, "@bool" + to_string(++TEMP));
    Bool->V_Data = valueStruct(I1, Simple, true, 0);
    Value *from = new Value(UndefInit, "UndefInit", l->V_Data);
    Instruction *instr0 = new Alloca(CUR_BB, from, Bool);
    // CUR_BB->insertInstr(instr0);
    CUR_BB->master->entry()->instrList.push_front(instr0);
    Instruction *instr1 = new Store(CUR_BB, new_l, Bool);
    CUR_BB->insertInstr(instr1);
    if(op == _AND){
        Instruction *instr2 = new Br(CUR_BB, new_l, br1, br2);
        CUR_BB->insertInstr(instr2);
    }
    else{
        Instruction *instr1_1 = new Zext(CUR_BB, new_l);
        CUR_BB->insertInstr(instr1_1);
        instr1_1->setUser("%" + to_string(++TEMP), I32, Simple, 0);
        valueUnion V_Const;
        V_Const.i = 0;
        Value *V = new Constant(IntConst, V_Const);
        Instruction *instr1_2 = new CmpOp(_eq_, CUR_BB, instr1_1, V);
        instr1_2->setUser("%" + to_string(++TEMP), I1, true, 0);
        CUR_BB->insertInstr(instr1_2);
        Instruction *instr2 = new Br(CUR_BB, instr1_2, br1, br2);
        CUR_BB->insertInstr(instr2);
    }

    CUR_BB = br1;
    Value *r = pNode->pNodes[1]->AST2IR();
    int r_val_type = Type_AST(pNode->pNodes[1]);
    Value *new_r = Type_Match(I1, r, r_val_type);
    if(new_r == NULL){exit(-38);}
    Instruction *instr3 = new Store(CUR_BB, new_r, Bool);
    CUR_BB->insertInstr(instr3);
    Instruction *instr4 = new Jump(CUR_BB, br2);
    CUR_BB->insertInstr(instr4);

    CUR_BB = br2;
    Value *to = new Value(Reg, "%" + to_string(++TEMP), Bool->V_Data);
    Instruction *instr5 = new Load(CUR_BB, Bool, to);
    CUR_BB->insertInstr(instr5);
    return to;

    /*
    cond1 && cond2 ->
    I1 Bool = cond1
    if(cond1) Bool = cond2
    Bool

    cond1 || cond2 ->
    I1 Bool = cond1
    if(!cond1) Bool = cond2
    Bool
    */
}


/* funcs related to ArrayExp */
Value *ArrayExp2IR(expNode *pNode, bool b){
    identNode *pNode1 = &(pNode->pNodes[0]->node.ident_node);
    int lval_index = 0;
    valueStruct *lval_info = sym_find_def(sym, pNode1->name, lval_index);
    if(lval_info == NULL || lval_info->sym_type != Array){exit(-17);}
    pNode1->val_type = lval_info->val_type, pNode->val_type = lval_info->val_type;
    if(pNode->val_type > F32){pNode->val_type = pNode->val_type * 2 - 9;}
    // Ptr -> normal type, because all ArrayExp(not in FuncCall or FuncFParam) must be normal-type in SysY
    pNode1->sym_type = Array;
    int index = lval_info->val.i;
    vector<int>& dim_list = sym.Array_list[index]->dim_list;
    Value *from = lval_info->V;
    Value *to = new Value(Reg, "%" + to_string(++TEMP), from->V_Data);
    to->V_Data.other_index = 0;
    if(from->V_Data.val_type > F32){to->V_Data.val_type = from->V_Data.val_type * 2 - 9;}
    to->isArray = true;

    Instruction *instr = NULL; // enable gep to follow SSA

    if(pNode1->val_type <= F32){
        // Instruction *instr = new GetPtrElement(CUR_BB, from, to, false);
        instr = new GetPtrElement(CUR_BB, from, to, false); // enable gep to follow SSA
        int i = 1;
        for(; i < pNode->num; i++){
            astNode *pNode2 = pNode->pNodes[i];
            Value *dim = pNode2->AST2IR();
            if(i > dim_list.size()){exit(-18);}
            if(pNode2->type == I){
                int dim_val = pNode2->node.int_node.value;
                if(dim_val >= dim_list[i - 1] || dim_val < 0){exit(-19);}
                // instr->setUse(dim, i - 1);
                instr->setUse(dim, i);
            }
            else if(pNode2->type == F){exit(-20);}
            else if(pNode2->type == Id || pNode2->type == Exp){
                int dim_type;
                if(pNode2->type == Id){dim_type = pNode2->node.ident_node.val_type;}
                else{dim_type = pNode2->node.exp_node.val_type;}
                if(dim_type == CONST_I32 || dim_type == I32){
                    // if(dim_type == CONST_I32){
                    //     int dim_val = ((Constant *)dim)->V_Const.i;
                    //     if(dim_val >= dim_list[i - 1] || dim_val < 0){exit(-19);}
                    //     local_list[i] = dim_val;
                    // }
                    // instr->setUse(dim, i - 1);
                    instr->setUse(dim, i);
                }
                else{exit(-20);}
            }
        }
        if(i <= dim_list.size()){exit(-21);}
        CUR_BB->insertInstr(instr);
    } // normal type, must be located to element (dim is useless)

    else{
        // Instruction *instr = new GetPtrElement(CUR_BB, from, to, true);
        instr = new GetPtrElement(CUR_BB, from, to, true); // enable gep to follow SSA
        int i = 1;
        for(; i < pNode->num; i++){
            astNode *pNode2 = pNode->pNodes[i];
            Value *dim = pNode2->AST2IR();
            if(i > dim_list.size() + 1){exit(-18);}
            if(pNode2->type == I){
                int dim_val = pNode2->node.int_node.value;
                if(dim_val < 0){exit(-19);}
                if(i == 1){
                    // instr->setUse(dim, i - 1);
                    instr->setUse(dim, i);
                }
                else{
                    if(dim_val >= dim_list[i - 2]){exit(-19);}
                    // instr->setUse(dim, i - 1);
                    instr->setUse(dim, i);
                }
            }
            else if(pNode2->type == F){exit(-20);}
            else if(pNode2->type == Id || pNode2->type == Exp){
                int dim_type;
                if(pNode2->type == Id){dim_type = pNode2->node.ident_node.val_type;}
                else{dim_type = pNode2->node.exp_node.val_type;}
                if(dim_type == CONST_I32 || dim_type == I32){
                    // if(dim_type == CONST_I32){
                    //     int dim_val = ((Constant *)s)->V_Const.i;
                    //     if(dim_val < 0){exit(-19);}
                    //     if(i > 1 && dim_val >= dim_list[i - 2]){exit(-19);}
                    // }
                    // instr->setUse(dim, i - 1);
                    instr->setUse(dim, i);
                }
                else{exit(-20);}
            }
        }
        if(i <= dim_list.size() + 1){exit(-21);}
        CUR_BB->insertInstr(instr);
    } // FuncRParam-type, first action is getptr instead of getelementptr

    to = instr; // enable gep to follow SSA

    if(b){
        Value *new_to = new Value(Reg, "%" + to_string(++TEMP));
        new_to->V_Data = valueStruct(pNode->val_type, Simple, true, 0);
        Instruction *instr0 = new Load(CUR_BB, to, new_to);
        CUR_BB->insertInstr(instr0);
        return new_to;
    }
    return to;
}


/* funcs related to FuncCall */
Value *FuncCall2IR(expNode *pNode){
    identNode *pNode1 = &(pNode->pNodes[0]->node.ident_node);
    int lval_index = 0;
    valueStruct *lval_info = sym_find_def(sym, pNode1->name, lval_index);
    if(lval_info == NULL || lval_info->sym_type != Func){exit(-32);}
    pNode1->val_type = lval_info->val_type, pNode->val_type = lval_info->val_type;
    pNode1->sym_type = Func;
    int index = lval_info->val.i;
    vector<identInfo *> param_list = sym.Stack[index]->fparam_list;
    Instruction *instr = new Call(CUR_BB, sym.Function_table[pNode1->name]);

    if(pNode->num > 1){
        int i = 1;
        expNode *p = &(pNode->pNodes[1]->node.exp_node);
        for(; i <= p->num; i++){
            astNode *pNode2 = p->pNodes[i - 1];
            if(i > param_list.size()){exit(-33);}
            identInfo *iI = param_list[i - 1];
            int fparam_type = iI->val_type;
            if(pNode2->type == I){
                Value *rparam =Type_Match(fparam_type, pNode2->AST2IR(), CONST_I32);
                if(rparam != NULL){instr->setUse(rparam, i);}
                else{exit(-42);}
            }
            else if(pNode2->type == F){
                Value *rparam = Type_Match(fparam_type, pNode2->AST2IR(), CONST_F32);
                if(rparam != NULL){instr->setUse(rparam, i);}
                else{exit(-43);}
            }
            else if(pNode2->type == Id){
                identNode *pNode3 = &(pNode2->node.ident_node);
                int dim = -1, dim_list_index = 0;
                Value *rparam = FuncCallIdent2IR(pNode3, dim, dim_list_index);
                if(pNode3->sym_type == Simple){
                    Value *new_rparam = Type_Match(fparam_type, rparam, pNode3->val_type);
                    if(new_rparam != NULL){instr->setUse(new_rparam, i);}
                    else{exit(-44);}
                }
                else{
                    vector<int>& r_param_dim_list = sym.Array_list[dim_list_index]->dim_list;
                    vector<int>& f_param_dim_list = sym.Array_list[iI->other_index]->dim_list;
                    if(dim != f_param_dim_list.size()){
                        exit(-45);
                    }
                    int start = r_param_dim_list.size() - dim;
                    for(int j = start; j < r_param_dim_list.size(); j++){
                        if(r_param_dim_list[j] != f_param_dim_list[j - start]){exit(-46);}
                    }
                    Value *new_rparam = Type_Match(fparam_type, rparam, pNode3->val_type);
                    if(new_rparam != NULL){instr->setUse(new_rparam, i);}
                    else{exit(-47);} 
                }
            }
            else if(pNode2->type == Exp){
                expNode *pNode3 = &(pNode2->node.exp_node);
                if(pNode3->op == _ArrayExp){
                    int dim = 0, dim_list_index = 0;
                    Value *rparam = FuncCallArrayExp2IR(pNode3, dim, dim_list_index);
                    vector<int>& r_param_dim_list = sym.Array_list[dim_list_index]->dim_list;
                    vector<int>& f_param_dim_list = sym.Array_list[iI->other_index]->dim_list;
                    if(dim >= 0){
                       if(dim != f_param_dim_list.size()){exit(-48);}
                        int start = r_param_dim_list.size() - dim;
                        for(int j = start; j < r_param_dim_list.size(); j++){
                            if(r_param_dim_list[j] != f_param_dim_list[j - start]){exit(-49);}
                        } 
                    }
                    Value *new_rparam = Type_Match(fparam_type, rparam, pNode3->val_type);
                    if(new_rparam != NULL){instr->setUse(new_rparam, i);}
                    else{exit(-50);}
                }
                else{
                    Value *v = pNode3->Exp2IR();
                    Value *rparam = Type_Match(fparam_type, v, pNode3->val_type);
                    if(rparam != NULL){instr->setUse(rparam, i);}
                    else{exit(-51);}
                }
            }
        }
        if(i <= param_list.size()){exit(-52);}
    }
    CUR_BB->insertInstr(instr);

    if(pNode->val_type == VOID_TYPE){return NULL;}
    instr->setUser("%" + to_string(++TEMP), pNode->val_type, Simple, 0);
    return instr; 
}

Value *FuncCallArrayExp2IR(expNode *pNode, int& dim, int& dim_list_index){
    identNode *pNode1 = &(pNode->pNodes[0]->node.ident_node);
    int lval_index = 0;
    valueStruct *lval_info = sym_find_def(sym, pNode1->name, lval_index);
    if(lval_info == NULL || lval_info->sym_type != Array){exit(-28);}
    pNode1->val_type = lval_info->val_type, pNode->val_type = lval_info->val_type;
    pNode1->sym_type = Array;
    int index = lval_info->val.i;
    vector<int>& dim_list = sym.Array_list[index]->dim_list;
    dim_list_index = index;
    Value *from = lval_info->V;
    Value *to = new Value(Reg, "%" + to_string(++TEMP), from->V_Data);
    to->isArray = true;
    Instruction *instr = new GetPtrElement(CUR_BB, from, to, false);
    CUR_BB->insertInstr(instr);

    int i = 1;
    for(; i < pNode->num; i++){
        if(i > dim_list.size()){exit(-29);}
        astNode *pNode2 = pNode->pNodes[i];
        Value * Dim = pNode2->AST2IR();
        if(pNode2->type == I){
            int dim_val = pNode2->node.int_node.value;
            if(dim_val >= dim_list[i - 1] || dim_val < 0){exit(-30);}
            // instr->setUse(Dim, i - 1);
            instr->setUse(Dim, i);
        }
        else if(pNode2->type == F){exit(-31);}
        else if(pNode2->type == Id || pNode2->type == Exp){
            int dim_type;
            if(pNode2->type == Id){dim_type = pNode2->node.ident_node.val_type;}
            else{dim_type = pNode2->node.exp_node.val_type;}
            if(dim_type == CONST_I32 || dim_type == I32){
                // if(dim_type == CONST_I32){
                //     int dim_val = stoi(s);
                //     if(dim_val >= dim_list[i - 1] || dim_val < 0){exit(-30);} 
                // }
                // instr->setUse(Dim, i - 1);
                instr->setUse(Dim, i);
            }
            else{exit(-31);}
        }
    }

    to = instr; // enable gep to follow SSA
    Instruction *MayUse = new Load(CUR_BB, to, NULL); // only serves as a may-use tag

    if(i == dim_list.size() + 1){
        dim = -1;
        if(to->V_Data.val_type > F32){to->V_Data.val_type = to->V_Data.val_type * 2 - 9;}
        to->V_Data.other_index = 0;
        Value *new_to = new Value(Reg, "%" + to_string(++TEMP));
        new_to->V_Data = valueStruct(pNode->val_type, Simple, true, 0);
        Instruction *instr0 = new Load(CUR_BB, to, new_to);
        CUR_BB->insertInstr(instr0);
        return new_to;
    } // it's actually normal type
    else{
        valueUnion V_Const;
        V_Const.i = 0;
        Value *zero = new Constant(IntConst, V_Const);
        // instr->setUse(zero, 0);
        instr->setUse(zero, 1); // is pos correct ???
        dim = dim_list.size() - i; // record the dimension of Ptr
        to->V_Data.other_index = dim;
        if(to->V_Data.val_type <= F32){to->V_Data.val_type = (to->V_Data.val_type + 9) / 2;}
        // if(!dim){pNode->val_type = (pNode->val_type + 9) / 2;} // change its type to Ptr
        pNode->val_type = (pNode->val_type + 9) / 2;
        return to;
    } // FuncCall-type, example: int a[3][3][3][3]; func(a[1]); in FuncCall the type of "a" is actually [3, 3]* (Ptr)
}

Value *FuncCallIdent2IR(identNode *pNode, int& dim, int& dim_list_index){
    int lval_index = 0;
    valueStruct *lval_info = sym_find_def(sym, pNode->name, lval_index);
    if(lval_info == NULL){exit(-27);}
    Value *from = lval_info->V;

    if(lval_info->sym_type == Simple){
        pNode->val_type = lval_info->val_type;
        pNode->sym_type = Simple;
        if(pNode->val_type == CONST_I32){return from;}
        else if(pNode->val_type == CONST_F32){return from;}
        Value *to = new Value(Reg, "%" + to_string(++TEMP), from->V_Data);
        Instruction *instr = new Load(CUR_BB, from, to);
        CUR_BB->insertInstr(instr);
        return to;
    }

    else{
        pNode->sym_type = Array;
        dim_list_index = lval_info->val.i;
        if(lval_info->val_type >= I32_PTR){
            dim = sym.Array_list[dim_list_index]->dim_list.size();
            pNode->val_type = lval_info->val_type;
        }
        else{
            dim = sym.Array_list[dim_list_index]->dim_list.size() - 1; // record the dimension of Ptr
            pNode->val_type = (lval_info->val_type + 9) / 2; // change its type to Ptr
        }

        Value *to = new Value(Reg, "%" + to_string(++TEMP), from->V_Data);
        to->V_Data.other_index = dim;
        to->isArray = true;
        to->V_Data.val_type = pNode->val_type;
        Instruction *instr = new GetPtrElement(CUR_BB, from, to, false);
        valueUnion V_Const;
        V_Const.i = 0;
        Value *zero = new Constant(IntConst, V_Const);
        // instr->setUse(zero, 0);
        instr->setUse(zero, 1);
        CUR_BB->insertInstr(instr);

        to = instr; // enable gep to follow SSA
        Instruction *MayUse = new Load(CUR_BB, to, NULL); // only serves as a may-use tag

        return to;
    }
}

/* funcs related to PRINT */
void Print2IR(expNode *pNode){
    Value *v = pNode->pNodes[0]->AST2IR();
    Instruction *instr = new PrintCall(CUR_BB, v);
    CUR_BB->insertInstr(instr);
}

/* funcs for debugging */
void PrintAST(astNode *pNode, int level){
    string w_s(level, '-');
    if(pNode->type == I){cout << w_s << "-" << I << ":" << pNode->node.int_node.value << endl;}
    else if(pNode->type == F){cout << w_s << "-" << F << ":" << pNode->node.float_node.value << endl;}
    else if(pNode->type == Id){cout << w_s << "-" << Id << ":" << pNode->node.ident_node.name << endl;}
    else if(pNode->type == Exp){
        cout << w_s << "-" << Exp << ":" << pNode->node.exp_node.op << endl;
        for(int i = 0; i < pNode->node.exp_node.num; i++){
            PrintAST(pNode->node.exp_node.pNodes[i], level + 1);
        }
    }
}

void PrintSYM(SYM& sym){
    cout << "NUM OF SCOPE: " << sym.Stack.size() << endl;
    cout << "DETAIL OF SCOPE:" << endl;
    for(int i = 0; i < sym.Stack.size(); i++){
        cout << "  SCOPE" << i << ":" << endl;
        cout << "    LEVEL: " << sym.Stack[i]->level << endl;
        cout << "    CHILD: " << sym.Stack[i]->child << endl;
        cout << "    PARENT: " << sym.Stack[i]->parent << endl;
        cout << "    SIBLING: " << sym.Stack[i]->sibling << endl;
        cout << "    ROOT: " << sym.Stack[i]->root << endl;
        cout << "    NUM OF FPARAM: " << sym.Stack[i]->fparam_list.size() << endl;
        cout << "    NUM OF IDENT: " << sym.Stack[i]->ident_list.size() << endl;
    }
    cout << "TRACE OF SCOPE:" << endl;
    cout << "  ";
    for(int i = 0; i < sym.scope_trace.size(); i++){
        cout << sym.scope_trace[i] << " ";
    }
    cout << endl;
}