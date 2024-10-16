#include "IRModule.h"
#include <sstream>
using namespace std;

int TEMP = 0;
// int CUR_INSTR = 0;
int INSTR_COUNT = 0;
int BB_COUNT = 0;
int LOOP_COUNT = 0;
BasicBlock *CUR_BB = NULL;
FuncModule *CUR_FUNC = NULL;

/* func for LinkNode & LinkList */
void LinkNode::insertBefore(LinkNode *lnode){
    LinkNode *old_prev = prev;
    prev = lnode;
    lnode->next = this;
    lnode->prev = old_prev;
    old_prev->next = lnode;
}

void LinkNode::insertAfter(LinkNode *lnode){
    LinkNode *old_next = next;
    next = lnode;
    lnode->prev = this;
    lnode->next = old_next;
    old_next->prev = lnode;
}

void LinkNode::remove(){
    prev->next = next;
    next->prev = prev;
    prev = NULL;
    next = NULL;
}

void LinkList::clear(){
    LinkNode *cur = front();
    while(cur != &(llend)){
        LinkNode *next_cur = cur->next;
        cur->remove();
        cur = next_cur;
    }
}


/* func for Value */
void Value::addUse(Use *use){useList.push_back(use);}

Use *Value::getBeginUse(){return (Use *)useList.front();}

Use *Value::getEndUse(){return (Use *)useList.back();}

void Value::replaceAllUsesWith(Value *to){
    while(!useList.empty()){
        Use *use = (Use *)useList.front();
        User *user = use->user;
        user->replaceUseeWith(to, use->useePos);

        int op_type = ((Instruction *)user)->op_type;
        if(use->useePos == 0){
            if(op_type == _load_){((Load *)user)->from = to;}
            else if(op_type == _store_){((Store *)user)->from = to;}
            else if(op_type == _gep_){((GetPtrElement *)user)->from = to;}
        }
        // useList.remove(useList.front());
        useList.pop_front();
    }
}

string Value::showType(){
    if(!isArray){return Type2IR(V_Data.val_type);}
    else{return ArrayType2IR(V_Data.val_type, V_Data.other_index, sym.Array_list[V_Data.val.i]->dim_list);}
}

bool Value::isStoreReallyUsed(){
    if(useList.empty()){return false;}
    if(V_Data.val_type >= I32_PTR){return true;}
    Use *use_cur = (Use *)(useList.front());
    while(use_cur != useList.end()){
        if(((Instruction *)(use_cur->user))->op_type != _gep_){
            return true;
        }
        use_cur = (Use *)(use_cur->next);
    }
    return false;
}

void Value::Print(){
    if(V_type <= Reg){cout << name;}
}


/* func for Use */


/* func for User */
void User::setUser(string V_name, int val_type, int sym_type, int other_index){
    name = V_name;
    V_Data = valueStruct(val_type, sym_type, true, other_index);
}

void User::setUser(string V_name, valueStruct v_data){
    name = V_name;
    V_Data = v_data;
    V_Data.V = NULL;
}

void User::setUse(Value *V, int pos){
    Use *new_use = new Use(V, this, pos);
    V->addUse(new_use);
    useeList.push_back(new_use);
}

// void User::replaceUseeWith(Value *from, Value *to, int useePos){}
void User::replaceUseeWith(Value *to, int useePos){
    Use *new_usee = new Use(to, this, useePos);
    to->addUse(new_usee);
    // delete useeList[useePos];
    useeList[useePos] = new_usee;
}


/* func for constant */
void Constant::Print(){
    if(V_type == IntConst){cout << V_Const.i;}
    else{cout << V_Const.f;}
}


/* func for GlobalArrayInit */
void GlobalArrayInit::Print(){
    cout << "{";
    if(val_type == CONST_I32 || val_type == I32){
        for(int i = 0; i < Pos.size(); i++){
            cout << Pos[i] << ": " << IArray[i];
            if(i < Pos.size() - 1){cout << ", ";}
        }
    }
    else{
        for(int i = 0; i < Pos.size(); i++){
            cout << Pos[i] << ": " << FArray[i];
            if(i < Pos.size() - 1){cout << ", ";}
        }
    }
    cout << "}";
}


/* func for param */
void Param::Print(){
    cout << name;
}


/* func for BasicBlock */
BasicBlock::BasicBlock(FuncModule *master){
    this->master = master;
    V_type = BB;
    name = "BB" + to_string(++BB_COUNT);
    master->insertBB(this, true);
}

BasicBlock::BasicBlock(FuncModule *master, bool Insert){
    this->master = master;
    V_type = BB;
    name = "BB" + to_string(++BB_COUNT);
    if(Insert){master->insertBB(this, true);}
}

Instruction *BasicBlock::entry(){return (Instruction *)instrList.front();}

Instruction *BasicBlock::exit(){return (Instruction *)instrList.back();}

void BasicBlock::insertInstr(Instruction *instr){
    if(!ret_mark){instrList.push_back(instr);}
}

void BasicBlock::remove(){
    Instruction *instr_cur = entry();
    while(!instrList.isend(instr_cur)){
        Instruction *next_instr_cur = (Instruction *)(instr_cur->next);
        instr_cur->remove();
        instr_cur = next_instr_cur;
    }
    LinkNode::remove();
}

void BasicBlock::addLoop(Loop *loop){LoopStartWith.insert(loop);}

void BasicBlock::Print(){
    cout << name << ":" << endl;
}


/* func for Instruction */
void Instruction::releaseUse(){
    if(!releaseUseTag){
        for(int i = 0; i < useeList.size(); i++){
            useeList[i]->V->useList.remove(useeList[i]);
        }
        releaseUseTag = true;
    }
}

void Instruction::remove(){
    // for(int i = 0; i < useeList.size(); i++){
    //     // useeList[i]->remove();
    //     useeList[i]->V->useList.remove(useeList[i]);
    // }
    releaseUse();
    useeList.clear();
    // useList.clear(); // can all the Uses be deleted ???
    LinkNode::remove();
}


/* func for FuncModule */
void FuncModule::addParam(Param *param){ParamList.push_back(param);}

void FuncModule::insertBB(BasicBlock *bb, bool End){
    if(End){bbList.push_back(bb);}
    else{bbList.push_front(bb);}
}

void FuncModule::addLoop(Loop *loop){Loops.insert(loop);}

void FuncModule::Print(){
    cout << "func " << Type2IR(V_Data.val_type) << " " << name << "(";
    for(int i = 0; i < ParamList.size(); i++){
        cout << ParamList[i]->showType();
        cout << " ";
        ParamList[i]->Print();
        if(i < ParamList.size() - 1){cout << ", ";}
    }
    cout << "):" << endl;
}


/* func for Loop */
void Loop::Print(){
    Loop *loop = this;
    if(loop->layer == 0){return;}
    cout << loop->name + ":" << endl;
    cout << "  master: " << loop->master->name << endl;
    cout << "  head: " << loop->head->name << endl;
    cout << "  tail: " << loop->tail->name << endl;
    cout << "  layer: " << loop->layer << endl;
    cout << "  bbSet: ";
    for(auto iter = loop->bbSet.begin(); iter != loop->bbSet.end(); iter++){
        cout << (*iter)->name << " ";
    }
    cout << endl;
    cout << "  bbRPO: ";
    for(int i = 0; i < loop->bbRPO.size(); i++){
        cout << loop->bbRPO[i]->name << " ";
    }
    cout << endl;
    if(loop->parent){cout << "  parent: " << loop->parent->name << endl;}
    else{cout << "  parent: null" << endl;}
    if(!loop->children.empty()){
        cout << "  children: ";
        for(auto iter = loop->children.begin(); iter != loop->children.end(); iter++){
            cout << (*iter)->name << " ";
        }
        cout << endl;
    }
    else{cout << "  children: null" << endl;}
    cout << endl;
}


/* func for different instr */
void Alloca::Print(){
    string type = to->showType();
    cout << "  " << to->name << " = alloca " << type;
    if((isToArray && (to->V_Data.val_type == CONST_I32 || to->V_Data.val_type == CONST_F32)) || isToGlobal){
        cout << ", ";
        from->Print();
    }
    // just for testing
    if(isToGlobal){cout << " (Global)";}
    if(isToArray){cout << " (Array)";}
    cout << endl;
}

void Load::Print(){
    string type = from->showType();
    cout << "  " << to->name << " = load " << type << " , " << type << "* ";
    cout << from->name;
    // just for testing
    if(isFromGlobal){cout << " (Global)";}
    if(isFromArray){cout << " (Array)";}
    cout << endl;
}

void Store::Print(){
    string type = to->showType();
    cout << "  store " << type << " " << from->name << " , " << type << "* " << to->name;
    // just for testing
    if(isToGlobal){cout << " (Global)";}
    if(isToArray){cout << " (Array)";}
    cout << endl;
}

void Zext::Print(){
    cout << "  " << name << " = zext i1 " << useeList[0]->V->name << endl;
}

void Ftoi::Print(){
    cout << "  " << name << " = ftoi float " << useeList[0]->V->name << endl;
}

void Itof::Print(){
    cout << "  " << name << " = itof i32 " << useeList[0]->V->name << endl;
}

void BiOp::Print(){
    string type = useeList[0]->V->showType();
    string opCode;
    switch(op_type){
    case _add_:
        opCode = "add"; break;
    case _sub_:
        opCode = "sub"; break;
    case _mul_:
        opCode = "mul"; break;
    case _div_:
        opCode = "div"; break;
    case _mod_:
        opCode = "mod"; break;
    }
    cout << "  " << this->showType() << " " << name << " = " << opCode << " " << type << " " << useeList[0]->V->name;
    cout << " , " << type << " " << useeList[1]->V->name << endl; 
}

void CmpOp::Print(){
    string type = useeList[0]->V->showType();
    string opCode;
    switch(op_type){
    case _le_:
        opCode = "le"; break;
    case _ge_:
        opCode = "ge"; break;
    case _gt_:
        opCode = "gt"; break;
    case _lt_:
        opCode = "lt"; break;
    case _eq_:
        opCode = "eq"; break;
    case _ne_:
        opCode = "ne"; break;
    }
    cout << "  " << name << " = " << opCode << " " << type << " " << useeList[0]->V->name;
    cout << " , " << type << " " << useeList[1]->V->name << endl;
}

void GEP::Print(){
    cout << "  " << name;
    if(gp){cout << " = gp ";}
    else{cout << " = gep ";}
    cout << useeList[0]->V->showType() << "* " << useeList[0]->V->name << " , ";
    cout << useeList[1]->V->name << endl;
}

void GetPtrElement::Print(){
    cout << "  " << to->name;
    if(gp){cout << " = gp ";}
    else{cout << " = gep ";}
    cout << from->showType() << "* ";
    cout << from->name << " , [";
    for(int i = 1; i < useeList.size(); i++){
        // useeList[i]->V->Print();
        cout << useeList[i]->V->name;
        if(i < useeList.size() - 1){cout << ", ";}
    }
    cout << "]" << endl;
}

void Call::Print(){
    cout << "  ";
    if(useeList[0]->V->V_Data.val_type != VOID_TYPE){cout << name << " = ";}
    cout << "call " << this->useeList[0]->V->showType() << " " << this->useeList[0]->V->name << "(";
    for(int i = 1; i < useeList.size(); i++){
        // cout << useeList[i]->V->showType();
        cout << " ";
        // useeList[i]->V->Print();
        cout << useeList[i]->V->name;
        if(i < useeList.size() - 1){cout << ", ";}
    }
    cout << ")" << endl;
}

void Ret::Print(){
    cout << "  ret ";
    if(useeList.empty()){cout << "void";}
    else{
        cout << useeList[0]->V->showType() << " ";
        // useeList[0]->V->Print();
        cout << useeList[0]->V->name;
    }
    cout << endl;
}

BasicBlock * Br::getBr1(){return (BasicBlock *)(useeList[1]->V);}

BasicBlock *Br::getBr2(){return (BasicBlock *)(useeList[2]->V);}

void Br::Print(){
    cout << "  br i1 " << useeList[0]->V->name << " , ";
    cout << useeList[1]->V->name << " , ";
    cout << useeList[2]->V->name << endl; 
}

BasicBlock *Jump::getTarget(){return (BasicBlock *)(useeList[0]->V);}

void Jump::Print(){
    cout << "  jump " << useeList[0]->V->name << endl; 
}

void Phi::PhisetUse(Value *V){
    this->setUse(V, usee_size);
    usee_size += 1;
}

void Phi::PhisetFrom(BasicBlock *bb){bbFromList.push_back(bb);}

void Phi::Print(){
    cout << "  " << name << " = phi ";
    for(int i = 0; i < usee_size; i++){
        cout << useeList[i]->V->showType() << " " << useeList[i]->V->name;
        if(i < usee_size - 1){cout << ", ";}
    }
    cout << endl;
}

void PrintCall::Print(){
    cout << "  print " << useeList[0]->V->name << endl;
}

void Move::Print(){
    cout << "  mov " << useeList[0]->V->name << " , " << to->name << endl;
}


/* func for printing */
string Type2IR(int val_type){
    switch(val_type){
    case I1: return string("i1");
    case CONST_I32: return string("i32");
    case CONST_F32: return string("float");
    case I32: return string("i32");
    case F32: return string("float");
    case 267: return string("i32"); // _INT
    case 268: return string("float"); // _FLOAT
    case VOID_TYPE: return string("void");
    case I32_PTR: return string("i32");
    case F32_PTR: return string("float");
    default: return "";
    }
}

string ArrayType2IR(int val_type, int other_index, vector<int>& dim_list){
    string s("");
    string s0(other_index, ']');
    if(dim_list.empty()){return Type2IR(val_type);}
    for(int i = dim_list.size() - other_index; i < dim_list.size(); i++){
        s += "[" + to_string(dim_list[i]) + ", ";
    }
    if(val_type > F32){s0 += "*";}
    return s + Type2IR(val_type) + s0;
}