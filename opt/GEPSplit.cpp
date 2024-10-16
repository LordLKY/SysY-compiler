#include "GEPSplit.h"
using namespace std;

void GEPSplitPass::Run(){
    CollectMemInfo();
    gepSplitPass();
}

void GEPSplitPass::CollectMemInfo(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        for(auto bb_iter = (iter->second)->BBs.begin(); bb_iter != (iter->second)->BBs.end(); bb_iter++){
            Instruction *instr_cur = (*bb_iter)->entry();
            while(!((*bb_iter)->instrList.isend(instr_cur))){
                string name;
                if(instr_cur->op_type == _load_){
                    name = ((Load *)instr_cur)->from->name;
                    if(!LoadFromMap.count(name)){
                        LoadFromMap[name] = unordered_set<Instruction *>();
                    }
                    LoadFromMap[name].insert(instr_cur);
                }
                else if(instr_cur->op_type == _store_){
                    name = ((Store *)instr_cur)->to->name;
                    if(!StoreToMap.count(name)){
                        StoreToMap[name] = unordered_set<Instruction *>();
                    }
                    StoreToMap[name].insert(instr_cur);
                }
                
                instr_cur = (Instruction *)(instr_cur->next);
            }
        }
    }
}

void GEPSplitPass::gepSplitPass(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        for(auto iter1 = (iter->second)->BBs.begin(); iter1 != (iter->second)->BBs.end(); iter1++){
            Instruction *instr_cur = (*iter1)->entry();
            while(!(*iter1)->instrList.isend(instr_cur)){
                Instruction *next_instr = (Instruction *)(instr_cur->next);
                if(instr_cur->op_type == _gep_){gepSplit((GetPtrElement *)instr_cur);}

                instr_cur = next_instr;
            }
        }
    }
}

void GEPSplitPass::gepSplit(GetPtrElement *instr){
    int n_dim = instr->useeList.size() - 1;
    Value *from = instr->from;

    for(int i = 0; i < n_dim; i++){
        bool gp = (i == 0 && instr->gp);
        Instruction *new_from = NULL;
        new_from = new GEP(instr->master, from, gp);
        new_from->setUse(instr->useeList[i + 1]->V, 1);
        new_from->setUser("%" + to_string(++TEMP), from->V_Data);
        if(!gp){new_from->V_Data.other_index -= 1;} // gp won't cause dim-reduction
        else{new_from->V_Data.val_type = new_from->V_Data.val_type * 2 - 9;}
        new_from->isArray = true;
        instr->insertBefore(new_from);
        from = new_from;
    }

    instr->replaceAllUsesWith(from);
    if(LoadFromMap.count(instr->name)){
        for(auto iter = LoadFromMap[instr->name].begin(); iter != LoadFromMap[instr->name].end(); iter++){
            ((Load *)*iter)->from = from;
        }
    } // need this extra step since load-from info isn't expressed in def-use chain
    if(StoreToMap.count(instr->name)){
        for(auto iter = StoreToMap[instr->name].begin(); iter != StoreToMap[instr->name].end(); iter++){
            ((Store *)*iter)->to = from;
        }
    } // need this extra step since store-to info isn't expressed in def-use chain
    instr->remove();
}