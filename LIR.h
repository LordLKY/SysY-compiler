#pragma once
#include "IRModule.h"
#include <unordered_map>
using std::unordered_map;

extern vector<string> LIRset;
extern vector<GlobalArrayInit *> GAIset;

class Convertor{
public:
    bool ShowLIR = false;
    unordered_map<string, FuncModule *>* pFT;
    unordered_map<Value *, Value *>* pGT;

    unordered_map<string, int>* NameMap; // use it to convert virtual reg to real reg
    unordered_set<string> PreAllocaSet;

    Convertor(unordered_map<string, FuncModule*>* pFT,
              unordered_map<Value *, Value *>* pGT):
        pFT(pFT), pGT(pGT){}
    void Run();
    void ConvertPass();
    void LIRPrint();
    bool SetLIR(bool showLIR){ShowLIR = showLIR;}

    string ConvertReg(string name); // convert virtual reg to real reg
    string ConvertPtrType(Value *V);
    void ConvertInstr(Instruction *instr);
    void ConvertAlloca(Alloca *instr);
    void ConvertLoad(Load *instr);
    void ConvertStore(Store *instr);
    void ConvertZext(Zext *instr);
    void ConvertFtoi(Ftoi *instr);
    void ConvertItof(Itof *instr);
    void ConvertBiOp(Instruction *instr);
    void ConvertGep(GEP *instr);
    void ConvertCall(Call *instr);
    void ConvertRet(Ret *instr);
    void ConvertBr(Br *instr);
    void ConvertJump(Jump *instr);
    void ConvertPhi(Phi *instr);
    void ConvertPrintCall(PrintCall *instr);
    void ConvertMove(Move *instr);
};