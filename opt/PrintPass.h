#pragma once
#include "../IRModule.h"
#include <unordered_map>

using std::unordered_map;

class PrintPass{
public:
    unordered_map<string, FuncModule *>* pFT;
    unordered_map<Value *, Value *>* pGT;

    PrintPass(unordered_map<string, FuncModule*>* pFT,
              unordered_map<Value *, Value *>* pGT):
        pFT(pFT), pGT(pGT){}
    void Run();
    void PrintGT();
};