#include "Mem2Reg.h"
#include <deque>
using namespace std;

void mem2reg::Run(){
    SSA();
}

void mem2reg::SSA(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncSSA(iter->second);
    }
}

void mem2reg::FuncSSA(FuncModule *FM){
    BasicBlock *bb_entry = FM->entry();
    Instruction *instr_cur = bb_entry->entry();
    while(!(bb_entry->instrList.isend(instr_cur))){
        Instruction *instr_next = (Instruction *)(instr_cur->next);
        if(instr_cur->op_type == _alloca_){
            if(!((Alloca *)instr_cur)->isToArray){
                VarSSA(FM, ((Alloca *)instr_cur)->to);
            }
        }
        else{break;}
        instr_cur = instr_next;
    }
}

void mem2reg::VarSSA(FuncModule *FM, Value *V){
    unordered_map<BasicBlock *, unordered_set<Instruction *>> DefMap;
    unordered_map<BasicBlock *, unordered_set<Instruction *>> UseMap;

    CollectDef(FM, V->name, DefMap); // collect alloca/store (alloca can be treated as a kinda Def with UndefInit/ZeroInit)
    CollectUse(FM, V->name, UseMap); // collect load

    if(!UseMap.empty()){
        deque<BasicBlock *> worklist;
        unordered_set<BasicBlock *> placed;
        unordered_set<BasicBlock *> visited;

        for(auto iter = DefMap.begin(); iter != DefMap.end(); iter++){
            worklist.push_back(iter->first);
            visited.insert(iter->first);
        }

        while(!worklist.empty()){
            BasicBlock *bb_cur = worklist.front();
            worklist.pop_front();

            for(auto iter = bb_cur->DomFrontiers.begin(); iter != bb_cur->DomFrontiers.end(); iter++){
                if(!placed.count(*iter)){
                    placed.insert(*iter);

                    Instruction *instr = new Phi(*iter);
                    instr->setUser("%" + to_string(++TEMP), V->V_Data.val_type, Simple, 0);
                    (*iter)->instrList.push_front(instr);

                    if(DefMap.count(*iter)){DefMap[*iter].insert(instr);}
                    else{
                        unordered_set<Instruction *> DefInstrs;
                        DefInstrs.insert(instr);
                        DefMap[*iter] = DefInstrs;
                    }

                    if(UseMap.count(*iter)){UseMap[*iter].insert(instr);}
                    else{
                        unordered_set<Instruction *> UseInstrs;
                        UseInstrs.insert(instr);
                        UseMap[*iter] = UseInstrs;
                    }
                }

                if(!visited.count(*iter)){
                    visited.insert(*iter);
                    worklist.push_back(*iter);
                }
            }
        }

        stack<Value *> S;
        stack<BasicBlock *> S_from;
        VarRename(S, S_from, FM->entry(), UseMap, DefMap);
    } // insert Phi & rename

    for(auto iter = UseMap.begin(); iter != UseMap.end(); iter++){
        for(auto iter1 = (iter->second).begin(); iter1 != (iter->second).end(); iter1++){
            if((*iter1)->op_type != _phi_){(*iter1)->remove();}
        }
    } // remove load

    for(auto iter = DefMap.begin(); iter != DefMap.end(); iter++){
        for(auto iter1 = (iter->second).begin(); iter1 != (iter->second).end(); iter1++){
            if((*iter1)->op_type != _phi_){(*iter1)->remove();}
        }
    } // remove alloca/store
}

void mem2reg::VarRename(stack<Value *>& S, stack<BasicBlock *>& S_from, BasicBlock *bb,
                        unordered_map<BasicBlock *, unordered_set<Instruction *>>& UseMap,
                        unordered_map<BasicBlock *, unordered_set<Instruction *>>& DefMap){
    int v_count = 0;
    Instruction *instr_cur = bb->entry();

    while(!bb->instrList.isend(instr_cur)){
        if(instr_cur->op_type == _load_ && UseMap[bb].count(instr_cur)){
            if(S.empty()){exit(-100);}
            ((Load *)instr_cur)->to->replaceAllUsesWith(S.top());
        }
        else if(instr_cur->op_type == _store_ && DefMap[bb].count(instr_cur)){
            S.push(((Store *)instr_cur)->getFrom());
            S_from.push(bb);
            v_count++;
        }
        else if(instr_cur->op_type == _alloca_ && DefMap[bb].count(instr_cur)){
            S.push(((Alloca *)instr_cur)->from);
            S_from.push(bb);
            v_count++;
        }
        else if(instr_cur->op_type == _phi_ && DefMap[bb].count(instr_cur)){
            S.push(instr_cur);
            S_from.push(bb);
            v_count++;
        }

        instr_cur = (Instruction *)(instr_cur->next);
    } // deal with alloca/load/store/phi-def

    for(auto iter = bb->succBB.begin(); iter != bb->succBB.end(); iter++){
        Instruction *instr_cur1 = (*iter)->entry();
        while(instr_cur1->op_type == _phi_){
            if(UseMap[*iter].count(instr_cur1)){
                ((Phi *)instr_cur1)->PhisetUse(S.top());
                // ((Phi *)instr_cur1)->PhisetFrom(S_from.top());
                ((Phi *)instr_cur1)->PhisetFrom(bb);
            } // shouldn't use S_from

            instr_cur1 = (Instruction *)(instr_cur1->next);
        }
    } // deal with phi-use

    for(auto iter = bb->IDoms.begin(); iter != bb->IDoms.end(); iter++){
        VarRename(S, S_from, *iter, UseMap, DefMap);
    } // DFS on DomTree

    for(int i = 0; i < v_count; i++){S.pop(); S_from.pop();}
}

void mem2reg::CollectUse(FuncModule *FM, string var_name,
                         unordered_map<BasicBlock *, unordered_set<Instruction *>>& UseMap){
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        BasicBlock *bb = *iter;
        unordered_set<Instruction *> UseInstrs;
        Instruction *instr_cur = bb->entry();

        while(!(bb->instrList.isend(instr_cur))){
            if(instr_cur->op_type == _load_ && var_name == ((Load *)instr_cur)->from->name){
                UseInstrs.insert(instr_cur);
            }

            instr_cur = (Instruction *)(instr_cur->next);
        }

        if(!UseInstrs.empty()){UseMap[bb] = UseInstrs;}
    }
}

void mem2reg::CollectDef(FuncModule *FM, string var_name,
                         unordered_map<BasicBlock *, unordered_set<Instruction *>>& DefMap){
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        BasicBlock *bb = *iter;
        unordered_set<Instruction *> DefInstrs;
        Instruction *instr_cur = bb->entry();

        while(!(bb->instrList.isend(instr_cur))){
            if(instr_cur->op_type == _store_ && var_name == ((Store *)instr_cur)->to->name){
                DefInstrs.insert(instr_cur);
            }
            else if(instr_cur->op_type == _alloca_ && var_name == ((Alloca *)instr_cur)->to->name){
                DefInstrs.insert(instr_cur);
            }

            instr_cur = (Instruction *)(instr_cur->next);
        }

        if(!DefInstrs.empty()){DefMap[bb] = DefInstrs;}
    }
}

/*
using S_from in line 134 is actually a mistake
we don't care where the usee of phi is defined but where it comes from
it's crucial to realize this,
otherwise it will bring trouble to deSSA when dealing with critical path
*/