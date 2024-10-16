#pragma once
#include "IRModule.h"
#include "./opt/PrintPass.h"
#include "./opt/CFG.h"
#include "./opt/Mem2Reg.h"
#include "./opt/DCE.h"
#include "./opt/SCCP.h"
#include "./opt/GEPSplit.h"
#include "./opt/LoopConstruct.h"
#include "./opt/GVNGCM.h"
#include "./opt/deSSA.h"
#include "./opt/RA.h"
#include <unordered_map>

using std::unordered_map;
using std::string;


class PassManager{
public:
    bool OPT = true;
    bool ShowIR = false;
    unordered_map<string, FuncModule*>* pFT;
    unordered_map<Value *, Value *>* pGT;

    PassManager(unordered_map<string, FuncModule*>* pFT,
                unordered_map<Value *, Value *>* pGT): pFT(pFT), pGT(pGT){}
    void Run();
    void SetOPT(bool opt){OPT = opt;}
    void SetIR(bool showIR){ShowIR = showIR;}
};