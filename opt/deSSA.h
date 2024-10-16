#pragma once
#include "../IRModule.h"
#include <unordered_set>
#include <unordered_map>

using std::unordered_map;
using std::unordered_set;

class deSSAPass{
public:
    unordered_map<string, FuncModule *>* pFT;

    deSSAPass(unordered_map<string, FuncModule*>* pFT): pFT(pFT){}
    void Run();
    void destroySSA();
    void BBdestroySSA(BasicBlock *bb);
    void PhiRemove(Phi *instr);
    void PhiDistributeUsee(BasicBlock *source, BasicBlock *target, Value *V_user, Value *V_usee);
    void MoveInsert(BasicBlock *bb, Instruction *instr);
};