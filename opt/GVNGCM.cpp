#include "GVNGCM.h"
using namespace std;

void GVNGCMPass::Run(){
    CollectMemInfo();
    GVN();
    GCM();
}

void GVNGCMPass::CollectMemInfo(){
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

void GVNGCMPass::GVN(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncGVN(iter->second);
    }
}

void GVNGCMPass::GCM(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncGCM(iter->second);

        // GCMprintInfo(); // just for testing
    }
}

void GVNGCMPass::FuncGVN(FuncModule *FM){
    VNMap.clear();
    BasicBlock *bb_root = FM->entry();
    bbGVN(bb_root);
}

void GVNGCMPass::bbGVN(BasicBlock *bb){
    Instruction *instr_cur = bb->entry();
    while(!bb->instrList.isend(instr_cur)){
        Instruction *next_instr = (Instruction *)(instr_cur->next);
        if(canVN(instr_cur)){InstrVN(instr_cur);}

        instr_cur = next_instr;
    }

    for(auto iter = bb->IDoms.begin(); iter != bb->IDoms.end(); iter++){
        bbGVN(*iter);
    } // traversal on Dom-tree is same as RPO(loop is inevitable)
}

void GVNGCMPass::InstrVN(Instruction *instr){
    string V_hash = InstrHash(instr);
    if(VNMap.count(V_hash)){
        instr->replaceAllUsesWith(VNMap[V_hash]);
        if(LoadFromMap.count(instr->name)){
            for(auto iter = LoadFromMap[instr->name].begin(); iter != LoadFromMap[instr->name].end(); iter++){
                ((Load *)*iter)->from = VNMap[V_hash];
            }
        } // need this extra step since load-from info isn't expressed in def-use chain
        if(StoreToMap.count(instr->name)){
            for(auto iter = StoreToMap[instr->name].begin(); iter != StoreToMap[instr->name].end(); iter++){
                ((Store *)*iter)->to = VNMap[V_hash];
            }
        } // need this extra step since store-to info isn't expressed in def-use chain
        instr->remove();
    }
    else{
        VNMap[V_hash] = instr;
    }
}

string GVNGCMPass::InstrHash(Instruction *instr){
    if(instr->op_type < _add_){
        Value *V1 = instr->useeList[0]->V;
        return V1->name + to_string(instr->op_type);
    } // unary op
    else{
        string v1 = instr->useeList[0]->V->name, v2 = instr->useeList[1]->V->name;
        string op_str = to_string(instr->op_type);
        if(instr->op_type == _add_ || instr->op_type == _mul_){
            return v1 < v2 ? v1 + op_str + v2 : v2 + op_str + v1;
        }
        else if(instr->op_type == _gt_){
            return v2 + to_string(_lt_) + v1;
        } // v1 > v2 -> v2 < v1
        else if(instr->op_type == _ge_){
            return v2 + to_string(_le_) + v1;
        } // v1 >= v2 -> v2 <= v1
        else{return v1 + op_str + v2;}
    } // binary op
}

bool GVNGCMPass::canVN(Instruction *instr){
    return instr->isExp() || instr->op_type == _GEP_;
}

void GVNGCMPass::FuncGCM(FuncModule *FM){
    EarlyMap.clear();
    LateMap.clear();
    visited1.clear();
    visited2.clear();
    visited3.clear();

    schedule_early(FM);
    schedule_late(FM);
}

bool GVNGCMPass::isPinned(Instruction *instr){
    return (instr->op_type < _zext_ || instr->op_type > _GEP_);
}

void GVNGCMPass::schedule_early(FuncModule *FM){
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        Instruction *instr_cur = (*iter)->entry();
        while(!(*iter)->instrList.isend(instr_cur)){
            if(isPinned(instr_cur)){EarlyMap[instr_cur] = *iter;}
            else{EarlyMap[instr_cur] = FM->entry();}

            if(instr_cur->op_type == _load_){EarlyMap[((Load *)instr_cur)->to] = *iter;}

            instr_cur = (Instruction *)(instr_cur->next);
        }
    } // init

    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        Instruction *instr_cur = (*iter)->entry();
        while(!(*iter)->instrList.isend(instr_cur)){
            if(isPinned(instr_cur) && (!visited1.count(instr_cur))){
                instr_schedule_early(instr_cur);

                if(instr_cur->op_type == _store_){
                    Value *to = ((Store *)instr_cur)->to;
                    if(to->isInstr() && !visited1.count((Instruction *)to)){
                        instr_schedule_early((Instruction *)to);
                    }
                }
            }

            instr_cur = (Instruction *)(instr_cur->next);
        }
    } // pass
}

void GVNGCMPass::instr_schedule_early(Instruction *instr){
    visited1.insert(instr);
    BasicBlock *earlistBB = EarlyMap[instr];
    for(int i = 0; i < instr->useeList.size(); i++){
        Value *V = instr->useeList[i]->V;
        BasicBlock *newBB = NULL;
        if(V->isInstr()){
            Instruction *usee_instr = (Instruction *)V;
            if(!visited1.count(usee_instr)){
                instr_schedule_early(usee_instr);
            }
            if(!isPinned(instr)){
                newBB = EarlyMap[V];
            }
        }
        else if(V->V_type == Reg && EarlyMap.count(V)){
            newBB = EarlyMap[V];
        }
        if(newBB && newBB->dom_depth > earlistBB->dom_depth){
            earlistBB = newBB;
        }
    }
    EarlyMap[instr] = earlistBB;
}

void GVNGCMPass::schedule_late(FuncModule *FM){
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        Instruction *instr_cur = (*iter)->entry();
        while(!(*iter)->instrList.isend(instr_cur)){
            if(LateMap.count(instr_cur)){
                LateMap[instr_cur] = FindLCA(LateMap[instr_cur], *iter);
            }
            else{LateMap[instr_cur] = *iter;}

            if(instr_cur->op_type == _store_){
                if(LateMap.count(((Store *)instr_cur)->to)){
                    LateMap[((Store *)instr_cur)->to] = FindLCA(LateMap[((Store *)instr_cur)->to], *iter);
                }
                else{LateMap[((Store *)instr_cur)->to] = *iter;}
            }

            instr_cur = (Instruction *)(instr_cur->next);
        }
    } // init

    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        Instruction *instr_cur = (*iter)->entry();
        while(instr_cur->next != NULL){
            Instruction *next_instr = (Instruction *)(instr_cur->next);

            // if(isPinned(instr_cur) && (!visited2.count(instr_cur)))
            // add the condition of isPinned may ignore some instr that should be moved
            // ignore the condition of isPinned in other hand may lead more bugs
            if(!visited2.count(instr_cur)){
                instr_schedule_late(instr_cur);
            }

            if(instr_cur->op_type == _load_){
                Value *to = ((Load *)instr_cur)->to;
                if(to->isInstr() && (!visited2.count((Instruction *)to))){
                    instr_schedule_late((Instruction *)to);
                }
                else if(!visited3.count(to)){
                    value_schedule_late(to);
                }
            }

            instr_cur = next_instr;
        }
    } // pass
}

void GVNGCMPass::instr_schedule_late(Instruction *instr){
    unordered_set<Instruction *> userSet;
    visited2.insert(instr);
    // BasicBlock *latestBB = LateMap[instr];
    BasicBlock *latestBB = nullptr;
    if(LateMap.count(instr)){latestBB = LateMap[instr];}
    else{return;}

    if(!instr->useList.empty()){
        Use *use_cur = (Use *)(instr->useList.front());
        while(!instr->useList.isend(use_cur)){
            if(use_cur->user->isInstr()){
                Instruction *user_instr = (Instruction *)(use_cur->user);
                if(!visited2.count(user_instr)){
                    instr_schedule_late(user_instr); 
                }
                if(!isPinned(instr)){
                    userSet.insert(user_instr);
                    if(user_instr->isPhi()){
                        int usee_index = use_cur->useePos;
                        latestBB = FindLCA(latestBB, ((Phi *)user_instr)->bbFromList[usee_index]);
                    }
                    else if(LateMap.count(user_instr)){latestBB = FindLCA(latestBB, LateMap[user_instr]);}
                }
            }

            use_cur = (Use *)(use_cur->next);
        }
    }
    
    if(!isPinned(instr)){
        BasicBlock *bestBB = latestBB, *bb_cur = latestBB;
        while(bb_cur != EarlyMap[instr]){
            if(bb_cur->loop_depth < bestBB->loop_depth) {
                bestBB = bb_cur;
            }
            bb_cur = bb_cur->IDomParent;
        }
        if(bb_cur->loop_depth < bestBB->loop_depth){
            bestBB = bb_cur;
        }
        LateMap[instr] = bestBB; // find bestBB with low loop_depth and relatively deep dom_depth

        if(bestBB != instr->master){
            ((LinkNode *)instr)->remove();
            instr->master = bestBB;
            Instruction *insert_pos = NULL;
            Instruction *instr_cur = bestBB->entry();
            while(!bestBB->instrList.isend(instr_cur)){
                if((!instr_cur->isPhi()) || instr_cur->op_type != _alloca_){
                    if(userSet.count(instr_cur)){
                        insert_pos = instr_cur;
                        break;
                    }
                    if(instr_cur->op_type == _store_ && ((Store *)instr_cur)->to == instr){
                        insert_pos = instr_cur;
                        break;
                    }
                }

                instr_cur = (Instruction *)(instr_cur->next);
            }
            if(insert_pos == NULL){bestBB->exit()->insertBefore(instr);}
            else{insert_pos->insertBefore(instr);}
        } // choose right(before any user) and deepest place to insert
    }
}

void GVNGCMPass::value_schedule_late(Value *V){
    visited3.insert(V);
    if(!V->useList.empty()){
        Use *use_cur = (Use *)(V->useList.front());
        while(!V->useList.isend(use_cur)){
            if(use_cur->user->isInstr()){
                Instruction *user_instr = (Instruction *)(use_cur->user);
                if(!visited2.count(user_instr)){
                   instr_schedule_late(user_instr); 
                }
            }

            use_cur = (Use *)(use_cur->next);
        }
    }
}

BasicBlock *GVNGCMPass::FindLCA(BasicBlock *bb1, BasicBlock *bb2){
    if(bb1 == NULL){return bb2;}
    if(bb1 == bb2){return bb1;}
    else if(bb1->dom_depth < bb2->dom_depth){
        return FindLCA(bb1, bb2->IDomParent);
    }
    else if(bb1->dom_depth > bb2->dom_depth){
        return FindLCA(bb1->IDomParent, bb2);
    }
    else{return FindLCA(bb1->IDomParent, bb2->IDomParent);}
} // a naive LCA. can be optimized

/* func for testing */
void GVNGCMPass::GCMprintInfo(){
    cout << "INFO of SCHEDULE_EARLY:" << endl;
    for(auto iter = EarlyMap.begin(); iter != EarlyMap.end(); iter++){
        cout << "  " << iter->first->name << " ";
        if(iter->second == NULL){cout << "NULL" << endl;}
        else{cout << iter->second->name << endl;}
    }
    cout << endl;
    cout << "INFO of SCHEDULE_LATE:" << endl;
    for(auto iter = LateMap.begin(); iter != LateMap.end(); iter++){
        cout << "  " << iter->first->name << " ";
        if(iter->second == NULL){cout << "NULL" << endl;}
        else{cout << iter->second->name << endl;}
    }
    cout << endl;
}