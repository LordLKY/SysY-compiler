#include "DCE.h"
#include <deque>
using namespace std;

void DCEPass::Run(){
    CollectToInfo();
    removeUselessVar();
    BDCE();
    removeUselessFunc();
}

void DCEPass::CollectToInfo(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        for(auto bb_iter = (iter->second)->BBs.begin(); bb_iter != (iter->second)->BBs.end(); bb_iter++){
            Instruction *instr_cur = (*bb_iter)->entry();
            while(!((*bb_iter)->instrList.isend(instr_cur))){
                string name;
                if(instr_cur->op_type == _load_){
                    name = ((Load *)instr_cur)->to->name;
                    if(!LoadToMap.count(name)){LoadToMap[name] = instr_cur;}
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

void DCEPass::removeUselessVar(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncRemoveUselessVar(iter->second);
    }

    for(auto iter = pGT->begin(); iter != pGT->end();){
        if(!iter->first->isUsed()){
            // DVars.insert(iter->first);
            pGT->erase(iter++);
        }
        else{iter++;}
    } // remove useless global var from GT
}

void DCEPass::BDCE(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncBDCE(iter->second);
    }
}

void DCEPass::removeUselessFunc(){
    auto iter = pFT->begin();
    while(iter != pFT->end()){
        if(!LFuncs.count((iter)->first)){
            pFT->erase(iter++);
        }
        else{iter++;}
    }
}

void DCEPass::FuncRemoveUselessVar(FuncModule *FM){
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        Instruction *instr_cur = (*iter)->entry();
        while(!((*iter)->instrList.isend(instr_cur))){
            // if(instr_cur->op_type == _br_ || instr_cur->op_type == _jump_ || 
            //    instr_cur->op_type == _ret_ || instr_cur->op_type == _call_){}
            // else if(!instr_cur->isUsed()){
            //     removeUselessUse(instr_cur);
            // }
            if(!isNotUseless(instr_cur)){
                removeUselessUse(instr_cur);
            }
            instr_cur = (Instruction *)(instr_cur->next);
        }
    }
}

void DCEPass::FuncBDCE(FuncModule *FM){
    deque<Value *> worklist;
    unordered_set<Instruction *> LiveInstrs;
    unordered_set<string> LiveFuncs;
    unordered_set<Value *> records;

    LiveFuncs.insert("main");

    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        Instruction *instr_cur = (*iter)->entry();
        while(!((*iter)->instrList.isend(instr_cur))){
            if(hasSideEffect(instr_cur)){
                worklist.push_back(instr_cur);
                records.insert(instr_cur);
            }

            instr_cur = (Instruction *)(instr_cur->next);
        }
    }

    while(!worklist.empty()){
        Value *V = worklist.front();
        worklist.pop_front();
        handleSideEffect(V, worklist, LiveInstrs, LiveFuncs, records);
    }

    for(auto iter = LiveFuncs.begin(); iter != LiveFuncs.end(); iter++){
        if(!LFuncs.count(*iter)){LFuncs.insert(*iter);}
    }

    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        Instruction *instr_cur = (*iter)->entry();
        while(!((*iter)->instrList.isend(instr_cur))){
            Instruction *next_instr = (Instruction *)(instr_cur->next);
            if(!LiveInstrs.count(instr_cur)){
                instr_cur->remove();
            }

            instr_cur = next_instr;
        }
    }
}

bool DCEPass::hasSideEffect(Instruction *instr){
    if(instr->op_type == _br_ || instr->op_type == _jump_ || instr->op_type == _ret_ ||
       instr->op_type == _call_ || instr->op_type == _print_){
        return true;
    }
    else if(instr->op_type == _store_){
        Store *instr0 = (Store *)instr;
        // if(instr0->isToArray){
        //     // return ((Instruction *)instr0->to)->useeList[0]->V->isUsed();
        //     return instr0->real_to->isUsed();
        // }
        return instr0->real_to->isStoreReallyUsed();
    }
    else if(instr->op_type == _load_){
        Load *instr0 = (Load *)instr;
        return instr0->to->isUsed();
    }
    else if(instr->op_type == _gep_){
        GetPtrElement *instr0 = (GetPtrElement *)instr;
        return instr0->isUsed() || StoreToMap.count(instr0->name);
    }
    else if(instr->op_type == _alloca_){
        Alloca *instr0 = (Alloca *)instr;
        return instr0->to->isUsed();
    }
    return false;
}

void DCEPass::handleSideEffect(Value *V,
                               deque<Value *>& worklist,
                               unordered_set<Instruction *>& LiveInstrs,
                               unordered_set<string>& LiveFuncs,
                               unordered_set<Value *>& records){
    if(!V->isInstr()){
        if(V->isFunc()){
            LiveFuncs.insert(V->name);
        }
        return;
    }

    Instruction *instr = (Instruction *)V;
    if(!LiveInstrs.count(instr)){LiveInstrs.insert(instr);}
    for(int i = 0; i < instr->useeList.size(); i++){
        Value *V = instr->useeList[i]->V;
        if(V->isVar() || V->isFunc()){
            if(!records.count(V)){
                worklist.push_back(V);
                records.insert(V);
            }
        }
    }
}

void DCEPass::removeUselessUse(Instruction *instr){
    if(isNotUseless(instr)){return;}
    if(instr->releaseUseTag){return;}
    instr->releaseUse();

    if(instr->op_type == _store_){
        Value *V = ((Store *)instr)->to;
        if(StoreToMap.count(V->name)){
            StoreToMap.erase(V->name);
        }
        if(V->isInstr() && (!V->isUsed())){
            removeUselessUse((Instruction *)V);
        }
    }

    for(int i = 0; i < instr->useeList.size(); i++){
        Value *V = instr->useeList[i]->V;
        if(V->isInstr() && (!V->isUsed())){
            removeUselessUse((Instruction *)V);
        }
        else if(V->isVar() && (!V->isUsed())){
            if(LoadToMap.count(V->name)){
                removeUselessUse(LoadToMap[V->name]);
            }
            if(StoreToMap.count(V->name)){
                for(auto iter = StoreToMap[V->name].begin(); iter != StoreToMap[V->name].end(); iter++){
                    removeUselessUse((*iter));
                }
            }
        }
    }
}

bool DCEPass::isNotUseless(Instruction *instr){
    if(instr->op_type == _br_ || instr->op_type == _jump_ || instr->op_type == _ret_ ||
       instr->op_type == _call_ || instr->op_type == _print_){
        return true;
    }
    else if(instr->op_type == _store_){
        Store *instr0 = (Store *)instr;
        // if(instr0->isToArray){
        //     // return ((Instruction *)instr0->to)->useeList[0]->V->isUsed();
        //     return instr0->real_to->isUsed(); 
        // }
        return instr0->real_to->isStoreReallyUsed();
    }
    else if(instr->op_type == _load_){
        Load *instr0 = (Load *)instr;
        return instr0->to->isUsed();
    }
    else if(instr->op_type == _gep_){
        GetPtrElement *instr0 = (GetPtrElement *)instr;
        return instr0->isUsed() || StoreToMap.count(instr0->name);
    }
    else if(instr->op_type == _alloca_){
        Alloca *instr0 = (Alloca *)instr;
        return instr0->to->isUsed();
    }
    return instr->isUsed();
}