#include "PassManager.h"
using namespace std;

template<typename T>
void Pass(unordered_map<string, FuncModule*>* pFT){
    T* t = new T(pFT);
    t->Run();
    delete t;
}

template<typename T>
void Pass(unordered_map<string, FuncModule*>* pFT,
          unordered_map<Value *, Value *>* pGT){
    T* t = new T(pFT, pGT);
    t->Run();
    delete t;
}

void PassManager::Run(){
    Pass<CFGPass>(pFT);

    if(OPT){
        Pass<mem2reg>(pFT);
        Pass<DCEPass>(pFT, pGT);
        Pass<SCCPPass>(pFT); 
    }
    
    Pass<GEPSplitPass>(pFT);

    if(OPT){
        Pass<LoopConstructPass>(pFT);
        Pass<GVNGCMPass>(pFT);
        Pass<deSSAPass>(pFT);  
    }

    Pass<raPass>(pFT);

    if(ShowIR){
        Pass<PrintPass>(pFT, pGT);
    }
}