#pragma once
#include "../IRModule.h"
#include <unordered_set>
#include <unordered_map>
#include <stack>

using std::unordered_map;
using std::unordered_set;
using std::stack;

class mem2reg{
public:
    unordered_map<string, FuncModule *>* pFT;

    mem2reg(unordered_map<string, FuncModule*>* pFT): pFT(pFT){}
    void Run();
    void SSA();

    void FuncSSA(FuncModule *FM);
    void VarSSA(FuncModule *FM, Value *V);
    void VarRename(stack<Value *>& S, stack<BasicBlock *>& S_from, BasicBlock *bb,
                   unordered_map<BasicBlock *, unordered_set<Instruction *>>& UseMap,
                   unordered_map<BasicBlock *, unordered_set<Instruction *>>& DefMap);
    void CollectUse(FuncModule *FM, string var_name,
                    unordered_map<BasicBlock *, unordered_set<Instruction *>>& UseMap);
    void CollectDef(FuncModule *FM, string var_name,
                    unordered_map<BasicBlock *, unordered_set<Instruction *>>& DefMap);
};