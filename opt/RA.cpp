#include "RA.h"
#include <deque>
#include <algorithm>
#define REG_NUM 16
using namespace std;

bool DFS_EARLY_STOP = false;

void raPass::Run(){
    LA();
    buildRIG();
    RA();

    // just for testing
    // printLA();
    // printRIG();
    // printRA();
}

void raPass::LA(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncLA(iter->second);
    }
}

void raPass::buildRIG(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncRIG(iter->second);
    }
}

void raPass::RA(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncRA(iter->second);
    }
}

void raPass::FuncLA(FuncModule *FM){
    deque<BasicBlock *> worklist;
    worklist.push_back(FM->real_exit);

    while(!worklist.empty()){
        BasicBlock *bb = worklist.front();
        bool b = bbLA(bb);
        if(b || bb->firstLA){
            for(auto iter = bb->precBB.begin(); iter != bb->precBB.end(); iter++){
                worklist.push_back(*iter);
            }
        }
        bb->firstLA = false;
        worklist.pop_front();
    }
}

bool raPass::bbLA(BasicBlock *bb){
    unordered_set<Value *> newValues;

    for(auto iter = bb->succBB.begin(); iter != bb->succBB.end(); iter++){
        for(auto iter0 = (*iter)->LiveIn.begin(); iter0 != (*iter)->LiveIn.end(); iter0++){
            if(!bb->LiveOut.count(*iter0)){
                bb->LiveOut.insert(*iter0);
                newValues.insert(*iter0);
            }
        }
    }

    Instruction *instr_cur = bb->exit();
    while(!bb->instrList.isbegin(instr_cur)){
        Value *v = isRegDef(instr_cur);
        if(v != NULL){
            if(bb->firstLA && !bb->LiveOut.count(v)){
                bb->LiveOut.insert(v);
            }
            if(newValues.count(v)){
                newValues.erase(v);
            }
            if(!bb->master->VirtualRegs.count(v->name)){
                bb->master->VirtualRegs[v->name] = v;
            }
        }
        if(bb->firstLA){
            for(int i = 0; i < instr_cur->useeList.size(); i++){
                Value *v0 = instr_cur->useeList[i]->V;
                if(v0->isReg()){
                    newValues.insert(v0);
                    bb->LiveOut.insert(v0);
                }
                if(!bb->master->VirtualRegs.count(v0->name)){
                    bb->master->VirtualRegs[v0->name] = v0;
                }
            }
        }

        instr_cur = (Instruction *)(instr_cur->prev);
    }

    for(auto iter = newValues.begin(); iter != newValues.end(); iter++){
        if(!bb->LiveIn.count(*iter)){
            bb->LiveIn.insert(*iter);
        }
    }

    return !newValues.empty();
}

Value *raPass::isRegDef(Instruction *instr){
    if(instr->op_type != _store_ && instr->op_type != _load_){
        if(instr->isReg()){return instr;}
        else{return NULL;}
    }
    else if(instr->op_type == _load_){
        if(((Load *)instr)->to->isReg()){
            return ((Load *)instr)->to;
        }
        else{return NULL;}
    }
    else if(instr->op_type == _store_){
        if(((Store *)instr)->to->isReg()){
            return ((Store *)instr)->to;
        }
        else{return NULL;}
    }
    return NULL;
}

void raPass::FuncRIG(FuncModule *FM){
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        BasicBlock *bb = *iter;
        for(auto iter1 = bb->LiveOut.begin(); iter1 != bb->LiveOut.end(); iter1++){
            for(auto iter2 = next(iter1, 1); iter2 != bb->LiveOut.end(); iter2++){
                string r1 = (*iter1)->name, r2 = (*iter2)->name;
                if(r1 == r2){continue;}
                if(!FM->RIG.count(r1)){FM->RIG[(*iter1)->name] = unordered_set<string>();}
                if(!FM->RIG.count(r2)){FM->RIG[(*iter2)->name] = unordered_set<string>();}
                if(!FM->RIG[r1].count(r2)){FM->RIG[r1].insert(r2);}
                if(!FM->RIG[r2].count(r1)){FM->RIG[r2].insert(r1);}
            }
        }
    }
}

void raPass::FuncRA(FuncModule *FM){
    unordered_map<string, unordered_set<string>>& rig = FM->RIG;
    vector<string> Regs;
    for(auto iter = rig.begin(); iter != rig.end(); iter++){
        FM->RAmap[iter->first] = -1;
        Regs.push_back(iter->first);
    }

    deque<unordered_set<string>> allocation;
    deque<unordered_set<string>> final_allocation;
    int cnt = INT_MAX;
    DFS_EARLY_STOP = false;
    RAdfs(allocation, rig, final_allocation, Regs, cnt, 0); // calculate an allocation with least regs

    vector<pair<int, int>> costs;
    for(int i = 0; i < final_allocation.size(); i++){
        int cost = 0;
        for(auto iter = final_allocation[i].begin(); iter != final_allocation[i].end(); iter++){
            cost += (FM->VirtualRegs)[*iter]->useList.getSize();
        }
        costs.push_back(make_pair(i, cost));
    }
    sort(costs.begin(), costs.end(), [](const pair<int, int>& p1, const pair<int, int>& p2){
        return p1.second > p2.second;
    }); // sacrifice reg with lowest use cost

    for(int i = 0; i < costs.size(); i++){
        // if(i >= REG_NUM){break;}
        int j = costs[i].first;
        for(auto iter = final_allocation[j].begin(); iter != final_allocation[j].end(); iter++){
            FM->RAmap[*iter] = i;
        }
    }

    for(int i = REG_NUM; i < costs.size(); i++){
        Value *to = new Value(Mem, "@reg" + to_string(i));
        Instruction *instr = new Alloca(FM->entry(), to);
        FM->entry()->instrList.push_front(instr);
    } // spilling
}

void raPass::RAdfs(deque<unordered_set<string>>& allocation,
                   unordered_map<string, unordered_set<string>>& rig,
                   deque<unordered_set<string>>& final_allocation,
                   vector<string>& Regs,
                   int& cnt, int cur){
    if(DFS_EARLY_STOP){return;}
    if(allocation.size() >= cnt){return;}
    if(cur == Regs.size()){
        cnt = allocation.size();
        final_allocation.assign(allocation.begin(), allocation.end());
        DFS_EARLY_STOP = true;
        return;
    }

    string rs = Regs[cur];
    for(int i = 0; i < allocation.size(); i++){
        auto iter = allocation[i].begin();
        for(; iter != allocation[i].end(); iter++){
            if(rig[rs].count(*iter)){break;}
        }
        if(iter == allocation[i].end()){
            allocation[i].insert(rs);
            RAdfs(allocation, rig, final_allocation, Regs, cnt, cur + 1);
            allocation[i].erase(rs);
        }
    }

    unordered_set<string> init_set({rs});
    allocation.push_back(init_set);
    RAdfs(allocation, rig, final_allocation, Regs, cnt, cur + 1);
    allocation.pop_back();
}

/* func for testing */
void raPass::printLA(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        cout << "LA for FUNC " << iter->first << ":" << endl;
        for(auto iter1 = iter->second->BBs.begin(); iter1 != iter->second->BBs.end(); iter1++){
            cout << "  " << "LiveOut of " << (*iter1)->name << ": ";
            for(auto iter2 = (*iter1)->LiveOut.begin(); iter2 != (*iter1)->LiveOut.end(); iter2++){
                cout << (*iter2)->name << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void raPass::printRIG(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        cout << "RIG of FUNC " << iter->first << ":" << endl;
        unordered_map<string, unordered_set<string>>& rig = iter->second->RIG;
        for(auto iter1 = rig.begin(); iter1 != rig.end(); iter1++){
            cout << "  " << iter1->first << ": ";
            for(auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); iter2++){
                cout << (*iter2) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void raPass::printRA(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        cout << "RA of FUNC " << iter->first << ":" << endl;
        unordered_map<string, int>& ra_map = iter->second->RAmap;
        for(auto iter1 = ra_map.begin(); iter1 != ra_map.end(); iter1++){
            cout << "  " << iter1->first << " -> " << iter1->second;
        }
        cout << endl;
    }
}