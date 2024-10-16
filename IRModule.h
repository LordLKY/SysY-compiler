#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include "Table.h"

// using namespace std;
using std::unordered_set;
using std::to_string;

typedef enum{
    _alloca_,
    _load_,
    _store_,
    _zext_,
    _ftoi_,
    _itof_,
    _add_,
    _sub_,
    _mul_,
    _div_,
    _mod_,
    _ge_,
    _le_,
    _gt_,
    _lt_,
    _eq_,
    _ne_,
    _gep_, // refer GetPtrElement
    _GEP_, // refer GEP 
    _call_,
    _ret_,
    _br_,
    _jump_,
    _phi_,
    _print_,
    _mov_
}all_instr_type;

typedef enum{
    // constant
    IntConst = 0,
    FloatConst,

    // init
    UndefInit,
    ZeroInit,
    ArrayInit, // specifically GlobalArrayInit

    // Param
    Parameter,

    // module in IR
    Mem,
    Reg,
    Instr,
    FM, // aka func
    BB
}module_type;

class Use;
class User;
class Instruction;
class BasicBlock;
// class FuncModule;
class Loop;

extern int TEMP;
// extern int CUR_INSTR;
extern int INSTR_COUNT;
extern int BB_COUNT;
extern int LOOP_COUNT;
extern BasicBlock *CUR_BB;
extern FuncModule *CUR_FUNC;


class LinkNode{
public:
    LinkNode *prev;
    LinkNode *next;

    LinkNode(): prev(NULL), next(NULL){}
    LinkNode(LinkNode *prev, LinkNode *next): prev(prev), next(next){}
    void insertBefore(LinkNode *lnode);
    void insertAfter(LinkNode *lnode);
    void remove();
};

class LinkList{
private:
    int size;

public:
    LinkNode llbegin;
    LinkNode llend;
    LinkList(): size(0){llbegin.next = &llend, llend.prev = &llbegin;}
    int getSize(){return size;}
    bool empty(){return size == 0;}
    bool isbegin(LinkNode *lnode){return lnode == &llbegin;}
    bool isend(LinkNode *lnode){return lnode == &llend;}
    LinkNode *begin(){return &llbegin;}
    LinkNode *end(){return &llend;}
    LinkNode *front(){return llbegin.next;}
    LinkNode *back(){return llend.prev;}
    void push_back(LinkNode *lnode){llend.insertBefore(lnode); size++;}
    void push_front(LinkNode *lnode){llbegin.insertAfter(lnode); size++;}
    void pop_front(){if(size){front()->remove(); size--;}}
    void pop_back(){if(size){back()->remove(); size--;}}
    void remove(LinkNode *lnode){lnode->remove(); size--;}
    void clear();
};

class Value: public LinkNode{
public:
    int V_type; // Instr/BB ...
    string name;
    valueStruct V_Data;
    LinkList useList; // other option: Use *useList; list<Use *> useList;
    bool isGlobal = false;
    bool isArray = false;

    Value(){};
    Value(int V_type, string name): V_type(V_type), name(name){}
    Value(int V_type, string name, valueStruct& V_Data):
        V_type(V_type), name(name), V_Data(V_Data){}
    // void addUse(Use *use){useList.push_back((LinkNode *)use);}
    void addUse(Use *use);
    // Use *getBeginUse(){return (Use *)useList.front();}
    Use *getBeginUse();
    // Use *getEndUse(){return (Use *)useList.back();}
    Use *getEndUse();
    void replaceAllUsesWith(Value *to);
    string showType();
    bool isInstr(){return V_type == Instr;}
    bool isVar(){return V_type <= Instr && V_type >= Mem;}
    bool isReg(){return name.length() && name[0] == '%';}
    bool isFunc(){return V_type == FM;}
    bool isUsed(){return !useList.empty();}
    bool isStoreReallyUsed(); // if users are all _gep_, this value isn't really used
    bool isConst(){return V_type == IntConst || V_type == FloatConst;}
    virtual void Print();
};

class Use: public LinkNode{
public:
    Value *V;
    User *user;
    int useePos; // position in useeList

    Use(){};
    Use(Value *V, User *user, int useePos):
        V(V), user(user), useePos(useePos){}
};

class User: public Value{
public:
    vector<Use *> useeList;

    void setUser(string name, int val_type, int sym_type, int other_index);
    void setUser(string name, valueStruct v_data);
    void setUse(Value *V, int pos);
    // void replaceUseeWith(Value *from, Value *to, int useePos);
    void replaceUseeWith(Value *to, int useePos);
};

class Constant: public Value{
public:
    valueUnion V_Const;

    Constant(int V_type, valueUnion V_Const):
        Value(V_type, ""), V_Const(V_Const){
            if(V_type == IntConst){V_Data.val_type = CONST_I32, name = to_string(V_Const.i);}
            else{V_Data.val_type = CONST_F32, name = to_string(V_Const.f);}
        }
    void Print() override;
};

class GlobalArrayInit: public Value{
public:
    int val_type;
    vector<int> Pos; // record the position of defined value
    vector<int> IArray;
    vector<float> FArray;

    GlobalArrayInit(int val_type): val_type(val_type){V_type = ArrayInit;}
    void addValue(int v, int pos){Pos.push_back(pos), IArray.push_back(v);}
    void addValue(float v, int pos){Pos.push_back(pos), FArray.push_back(v);}
    void Print() override;
};

class Param: public Value{
public:
    FuncModule *master;
    
    Param(string name, FuncModule *master): Value(Parameter, name), master(master){}
    void Print() override;
};

class BasicBlock: public Value{
public:
    FuncModule *master;
    LinkList instrList;
    bool ret_mark = false;

    /* data related to CFG */
    unordered_set<BasicBlock *> precBB;
    unordered_set<BasicBlock *> succBB;
    unordered_set<BasicBlock *> Doms;
    unordered_set<BasicBlock *> IDoms;
    unordered_set<BasicBlock *> RDoms;
    unordered_set<BasicBlock *> IRDoms;
    unordered_set<BasicBlock *> DomFrontiers;
    int dom_depth;
    BasicBlock *IDomParent = NULL;

    /* data related to Loop */
    int loop_depth = 0;
    unordered_set<Loop *> LoopStartWith;

    /* data related to LiveAnalysis */
    bool firstLA = true;
    unordered_set<Value *> LiveIn;
    unordered_set<Value *> LiveOut;

    BasicBlock(){};
    BasicBlock(FuncModule *master);
    BasicBlock(FuncModule *master, bool Insert);
    // Instruction *entry(){return (Instruction *)instrList.front();}
    Instruction *entry();
    // Instruction *exit(){return (Instruction *)instrList.back();}
    Instruction *exit();
    // void insertInstr(Instruction *instr){instrList.push_back((LinkNode *)instr);}
    void insertInstr(Instruction *instr);
    void remove();
    void addLoop(Loop *loop);
    void Print() override;
};

class Instruction: public User{
public:
    int op_type;
    BasicBlock *master;
    int instr_id;
    bool releaseUseTag = false;

    Instruction(int op_type, BasicBlock *master):
        op_type(op_type), master(master), instr_id(++INSTR_COUNT){V_type = Instr;}
    void releaseUse();
    bool isExp(){return op_type <= _ne_ && op_type >= _zext_;}
    bool isBr(){return op_type == _jump_ || op_type == _br_;}
    bool isPhi(){return op_type == _phi_;}
    void remove();
};

class FuncModule: public Value{
public:
    LinkList bbList;
    vector<Param *> ParamList;
    int retType;
    BasicBlock *real_exit = NULL; // a virtual exit defined in CFG pass 
    unordered_map<BasicBlock *, unordered_set<BasicBlock *>*> PredG;
    unordered_map<BasicBlock *, unordered_set<BasicBlock *>*> SuccG;
    unordered_set<BasicBlock *> BBs;
    unordered_set<Loop *> Loops;

    /* data related to Register Allocation */
    unordered_map<string, Value *> VirtualRegs;
    unordered_map<string, unordered_set<string>> RIG; // register interference graph
    unordered_map<string, int> RAmap; // reflect virtual reg to real reg

    FuncModule(string name, valueStruct& vS): Value(FM, name, vS){
        real_exit = new BasicBlock(this, false);
    }
    BasicBlock *entry(){return (BasicBlock *)bbList.front();}
    BasicBlock *exit(){return (BasicBlock *)bbList.back();}
    // void addParam(Param *param){ParamList.push_back(param);}
    void addParam(Param *param);
    void insertBB(BasicBlock *bb, bool End);
    void addLoop(Loop *loop);
    void Print() override;
};

class Loop{
public:
    string name;
    BasicBlock *head;
    BasicBlock *tail;
    FuncModule *master;
    unordered_set<BasicBlock *> bbSet;
    vector<BasicBlock *> bbRPO;
    Loop *parent = NULL;
    unordered_set<Loop *> children;
    bool isRoot = false;
    int layer;
    
    Loop(string name, BasicBlock *H, BasicBlock *T, FuncModule *master):
        name(name), head(H), tail(T), master(master){}
    void setParent(Loop *loop){parent = loop;}
    void addBB(BasicBlock *bb){bbSet.insert(bb); bbRPO.push_back(bb);}
    void addChild(Loop *loop){children.insert(loop);}
    void Print();
}; // will be built in LoopConstruct pass


/* classes for different instr */
class Alloca: public Instruction{
public:
    Value *from, *to;
    bool isToArray = false;
    bool isToGlobal = false;

    Alloca(BasicBlock *master, Value *from, Value *to):
        Instruction(_alloca_, master), from(from), to(to){
            isToArray = to->isArray;
            isToGlobal = to->isGlobal;
        }
    Alloca(BasicBlock *master, Value *to):
        Instruction(_alloca_, master), to(to){} // incomplete form, dealing with spilling reg
    void Print() override;
};

class Load: public Instruction{
public:
    Value *from, *to;
    bool isFromArray;
    bool isFromGlobal;

    Load(BasicBlock *master, Value *from, Value *to):
        Instruction(_load_, master), from(from), to(to){
            isFromArray = from->isArray;
            isFromGlobal = from->isGlobal;
            this->setUse(from, 0);

            if(isFromArray){
                this->setUse(((Instruction *)from)->useeList[0]->V, 1); // is it safe ???
            }
        }
    void Print() override;
};

class Store: public Instruction{
public:
    Value *from, *to;
    bool isToArray;
    bool isToGlobal;
    Value *real_to = NULL;

    Store(BasicBlock *master, Value *from, Value *to):
        Instruction(_store_, master), from(from), to(to){
            isToArray = to->isArray;
            isToGlobal = to->isGlobal;
            this->setUse(from, 0);

            if(isToArray && to->isInstr()){
                real_to = ((Instruction *)to)->useeList[0]->V;
            }
            else{real_to = to;}
        }
    Value *getFrom(){return useeList[0]->V;}
    void Print() override;
};

class Zext: public Instruction{
public:
    Zext(BasicBlock *master, Value *usee):
        Instruction(_zext_, master){this->setUse(usee, 0);}
    void Print() override;
};

class Ftoi: public Instruction{
public:
    Ftoi(BasicBlock *master, Value *usee):
        Instruction(_ftoi_, master){this->setUse(usee, 0);}
    void Print() override;
};

class Itof: public Instruction{
public:
    Itof(BasicBlock *master, Value *usee):
        Instruction(_itof_, master){this->setUse(usee, 0);}
    void Print() override;
};

class BiOp: public Instruction{
public:
    BiOp(int V_type, BasicBlock *master, Value *usee1, Value *usee2):
        Instruction(V_type, master){
            this->setUse(usee1, 0);
            this->setUse(usee2, 1);
        }
    void Print() override;
};

class CmpOp: public Instruction{
public:
    CmpOp(int V_type, BasicBlock *master, Value *usee1, Value *usee2):
        Instruction(V_type, master){
            this->setUse(usee1, 0);
            this->setUse(usee2, 1);
        }
    void Print() override;
};

class GEP: public Instruction{
public:
    bool gp;

    GEP(BasicBlock *master, Value *usee, bool gp):
        Instruction(_GEP_, master), gp(gp){
            this->setUse(usee, 0);
            isArray = true;
        }
    void Print() override;
};

class GetPtrElement: public Instruction{
public:
    Value *from, *to;
    bool gp;

    GetPtrElement(BasicBlock *master, Value *from, Value *to, bool gp):
        Instruction(_gep_, master), from(from), to(to), gp(gp){
            this->setUse(from, 0);

            isArray = true;
            setUser(to->name, to->V_Data);
        }
    void Print() override;
}; // combine a group of GEPs in one instr since it's convenient now. it will be extended in GEPSplit pass.

class Call: public Instruction{
public:
    Call(BasicBlock *master, FuncModule *func):
        Instruction(_call_, master){
            this->setUse(func, 0);
        }
    void Print() override;
};

class Ret: public Instruction{
public:
    Ret(BasicBlock *master):
        Instruction(_ret_, master){}
    Ret(BasicBlock *master, Value *from):
        Instruction(_ret_, master){
            this->setUse(from, 0);
        }
    void Print() override;
};

class Br: public Instruction{
public:
    Br(BasicBlock *master, Value *cond, BasicBlock *br1, BasicBlock *br2):
        Instruction(_br_, master){
            this->setUse(cond, 0);
            this->setUse(br1, 1);
            this->setUse(br2, 2);
        }
    BasicBlock *getBr1();
    BasicBlock *getBr2();
    void Print() override;
};

class Jump: public Instruction{
public:
    Jump(BasicBlock *master, BasicBlock *target):
        Instruction(_jump_, master){
            this->setUse(target, 0);
        }
    BasicBlock *getTarget();
    void Print() override;
};

class Phi: public Instruction{
public:
    int usee_size;
    vector<BasicBlock *> bbFromList;

    Phi(BasicBlock *master): Instruction(_phi_, master), usee_size(0){}
    void PhisetUse(Value *V);
    void PhisetFrom(BasicBlock *bb);
    void Print() override;
};

class PrintCall: public Instruction{
public:
    PrintCall(BasicBlock *master, Value *v):
        Instruction(_print_, master){
            this->setUse(v, 0);
        }
    void Print() override;
};

class Move: public Instruction{
public:
    Value *to;

    Move(BasicBlock *master, Value *from, Value *to):
        Instruction(_mov_, master), to(to){
        this->setUse(from, 0);
        this->setUser(to->name, to->V_Data);
        }
    void Print() override;
};


/* func for printing */
string Type2IR(int val_type); // val_type -> type in IR
string ArrayType2IR(int val_type, int other_index, vector<int>& dim_list); // [a][b][c] -> string([a [b [c ]]])

/*
after doing some optimization,
i realize that excluding load/store from def-use chain is a very silly option
i wish i could realize it when designing IR
this wrong decision causes a lotta troubles in optimization
now reconstructing the codes is extremely costly
so ... i didn't fix this terrible design (i'm lazy and i have no patience)  
*/