#pragma once
#include "../IRModule.h"
#include <unordered_set>
#include <unordered_map>
#include <deque>

using std::unordered_map;
using std::unordered_set;
using std::deque;

class DCEPass{
public:
    unordered_map<string, FuncModule *>* pFT;
    unordered_map<Value *, Value *>* pGT;
    unordered_set<string> LFuncs; // live funcs
    // unordered_set<Value *> DVars; // dead vars

    unordered_map<string, Instruction *> LoadToMap;
    unordered_map<string, unordered_set<Instruction *>> StoreToMap;

    DCEPass(unordered_map<string, FuncModule*>* pFT,
              unordered_map<Value *, Value *>* pGT):
        pFT(pFT), pGT(pGT){}
    void Run();
    void CollectToInfo(); // need this extra step since load-to/store-to info isn't expressed in def-use chain
    void removeUselessVar(); // try to eliminate some useless load/store/gep
    void BDCE(); // basic (non-aggressive) DCE which can't deal with remaining load/store/gep
    void removeUselessFunc();

    void FuncRemoveUselessVar(FuncModule *FM);
    void FuncBDCE(FuncModule *FM);
    bool hasSideEffect(Instruction *instr);
    void handleSideEffect(Value *V,
                          deque<Value *>& worklist,
                          unordered_set<Instruction *>& LiveInstrs,
                          unordered_set<string>& LiveFuncs,
                          unordered_set<Value *>& records);
    void removeUselessUse(Instruction *instr);
    bool isNotUseless(Instruction *instr);
};

/*
this is a non-aggressive DCE
deling with control flow is difficult (beyond my ability)
since MEM SSA is not equipped (guess what? still beyond my ability ...)
dealing with array(pointer) is extremely complex
I've tried my best to distinguish some useless instructions related to array and eliminate them
it's an ugly method like useless struggling
and it took me 2 days
I don't know why i did it and i think i'm a psycho
all i want to say now is f**k off
and screw it !!!
how pathetic i am ...
*/