#include "PrintPass.h"
using namespace std;

void PrintPass::Run(){
    PrintGT();
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncModule *pF = iter->second;
        pF->Print();

        BasicBlock *bb_cur = pF->entry();
        while(bb_cur != pF->bbList.end()){
            bb_cur->Print();

            // Instruction *instr_cur = (Instruction *)bb_cur->entry_();
            Instruction *instr_cur = bb_cur->entry();
            while(!bb_cur->instrList.isend(instr_cur)){
                instr_cur->Print();
                instr_cur = (Instruction *)(instr_cur->next);
            }

            bb_cur = (BasicBlock *)(bb_cur->next);
            cout << endl;
        }

        cout << endl;
    }
}

void PrintPass::PrintGT(){
    for(auto iter = pGT->begin(); iter != pGT->end(); iter++){
        Value *from = iter->second, *to = iter->first;
        cout << to->name << " = alloca " << to->showType() << " , ";
        from->Print();
        cout << " (Global)" << endl;
    }
    cout << endl;
}