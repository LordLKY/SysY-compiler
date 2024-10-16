#pragma once
#include "../IRModule.h"
#include <unordered_set>
#include <unordered_map>

using std::unordered_map;
using std::unordered_set;

class GVNGCMPass{
public:
    unordered_map<string, FuncModule *>* pFT;
    unordered_map<string, Instruction *> VNMap;

    unordered_map<Value *, BasicBlock *> EarlyMap;
    unordered_map<Value *, BasicBlock *> LateMap;
    unordered_set<Instruction *> visited1; // for schedule_early
    unordered_set<Instruction *> visited2; // for instr_schedule_late
    unordered_set<Value *> visited3; // for value_schedule_late

    unordered_map<string, unordered_set<Instruction *>> LoadFromMap;
    unordered_map<string, unordered_set<Instruction *>> StoreToMap;

    GVNGCMPass(unordered_map<string, FuncModule*>* pFT): pFT(pFT){}
    void Run();
    void CollectMemInfo(); // need this extra step since load-from/store-to info isn't expressed in def-use chain
    void GVN();
    void GCM();

    /* funcs for GVN */
    void FuncGVN(FuncModule *FM);
    void bbGVN(BasicBlock *bb);
    void InstrVN(Instruction *instr);
    string InstrHash(Instruction *instr);
    bool canVN(Instruction *instr);

    /* funcs for GCM */
    void FuncGCM(FuncModule *FM);
    bool isPinned(Instruction *instr);
    void schedule_early(FuncModule *FM);
    void instr_schedule_early(Instruction *instr);
    void schedule_late(FuncModule *FM);
    void instr_schedule_late(Instruction *instr);
    void value_schedule_late(Value *V);
    BasicBlock *FindLCA(BasicBlock *bb1, BasicBlock *bb2);

    /* func for testing */
    void GCMprintInfo();
};

/*
once again, dancing on a volcano
never know what potential bugs it may cause 
*/