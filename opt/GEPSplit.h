#pragma once
#include "../IRModule.h"
#include <unordered_map>

using std::unordered_map;

class GEPSplitPass{
public:
    unordered_map<string, FuncModule *>* pFT;

    unordered_map<string, unordered_set<Instruction *>> LoadFromMap;
    unordered_map<string, unordered_set<Instruction *>> StoreToMap;

    GEPSplitPass(unordered_map<string, FuncModule*>* pFT): pFT(pFT){}
    void Run();
    void CollectMemInfo(); // need this extra step since load-from/store-to info isn't expressed in def-use chain
    void gepSplitPass();
    void gepSplit(GetPtrElement *instr);
};

/*
as you can see
using combined gep-type instructions brings some convenience in previous progress
but now it's time to split them
and wait GVNGCM optimizing them

example:
%a = gep i32[3][3][3] @Array, [1, 1, 1] (%a: i32*)
use %a
->
%b = gep i32[3][3][3] @Array, 1
%c = gep i32[3][3] %b, 1
%d = gep i32[3] %c, 1 (%d: i32*)
use %d
*/