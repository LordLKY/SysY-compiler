#include "SCCP.h"
using namespace std;

void SCCPPass::Run(){
    sccp();
}

void SCCPPass::sccp(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        Funcsccp(iter->second);
    }
}

void SCCPPass::Funcsccp(FuncModule *FM){
    sccpInit(FM); // init

    while(!(EdgeWorklist.empty() && InstrWorklist.empty())){
        while(!EdgeWorklist.empty()){
            Edge edge_cur(EdgeWorklist.front());
            EdgeWorklist.pop_front();
            BasicBlock *from = edge_cur.bb_from, *to = edge_cur.bb_to;

            if(from == NULL || (!EdgeRecord[from][to])){
                if(from){EdgeRecord[from][to] = true;}
                bbReachMap[to] = true;

                Instruction *instr_cur = to->entry();
                while(!to->instrList.isend(instr_cur)){
                    visitInstr(instr_cur);

                    instr_cur = (Instruction *)(instr_cur->next);
                }
            }
        } // deal with EdgeWorklist

        while(!InstrWorklist.empty()){
            Instruction *instr_cur = InstrWorklist.front();
            InstrWorklist.pop_front();

            if(bbReachMap[instr_cur->master]){
                visitInstr(instr_cur);
            }
        } // deal with InstrWorklist
    }

    sccpUpdate(FM); // update
}

void SCCPPass::sccpInit(FuncModule *FM){
    EdgeWorklist.clear();
    EdgeRecord.clear();
    InstrWorklist.clear();
    InstrRecord.clear();

    BasicBlock *bb_cur = FM->entry();
    Edge edge0(NULL, bb_cur);
    EdgeWorklist.push_back(edge0);

    while(!FM->bbList.isend(bb_cur)){
        Instruction *instr_cur = bb_cur->entry();
        while(!bb_cur->instrList.isend(instr_cur)){
            if(instr_cur->isExp() || instr_cur->isPhi() || instr_cur->isBr()){
                InstrRecord[instr_cur] = UNDEF;
            }
            else{
                InstrRecord[instr_cur] = NAC;
            }

            ValueMap[instr_cur] = 0;

            instr_cur = (Instruction *)(instr_cur->next);
        } // initialize InstrRecord & ValueMap

        EdgeRecord[bb_cur] = unordered_map<BasicBlock *, bool>();
        for(auto iter = bb_cur->succBB.begin(); iter != bb_cur->succBB.end(); iter++){
            EdgeRecord[bb_cur][*iter] = false;
        } // initialize EdgeRecord

        if(bb_cur == FM->entry()){bbReachMap[bb_cur] = true;}
        else{bbReachMap[bb_cur] = false;} // initialize bbReachMap

        bb_cur = (BasicBlock *)(bb_cur->next);
    }
}

void SCCPPass::sccpUpdate(FuncModule *FM){
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        Instruction *instr_cur = (*iter)->entry();
        while(!(*iter)->instrList.isend(instr_cur)){
            Instruction *next_instr = (Instruction *)(instr_cur->next);
            if(instr_cur->isExp()){updateExp(instr_cur);}
            else if(instr_cur->isBr()){updateBr(instr_cur);}
            else if(instr_cur->isPhi()){updatePhi(instr_cur);}

            instr_cur = next_instr;
        }
    }
}

void SCCPPass::visitInstr(Instruction *instr){
    int old_lattice = InstrRecord[instr];
    if(old_lattice == NAC){return;}
    
    if(instr->isExp()){visitExp(instr);}
    else if(instr->isBr()){visitBr(instr);}
    else if(instr->isPhi()){visitPhi(instr);}

    if(InstrRecord[instr] != old_lattice){
        if(instr->isUsed()){
            Use *use_cur = (Use *)(instr->useList.front());
            while(use_cur != instr->useList.end()){
                if(use_cur->user->isInstr()){
                    InstrWorklist.push_back((Instruction *)(use_cur->user));
                }

                use_cur = (Use *)(use_cur->next);
            }
        }
    } // deal with potential change
}

void SCCPPass::visitExp(Instruction *instr){
    if(instr->op_type <= _itof_){
        Value *V = instr->useeList[0]->V;

        if(V->isConst()){
            InstrRecord[instr] = AC;
            if(V->V_type == IntConst){
                ValueMap[instr] = (float)stoi(V->name);
            }
            else{ValueMap[instr] = stof(V->name);}
        }
        else if(V->isInstr()){
            InstrRecord[instr] = InstrRecord[(Instruction *)V];
            if(InstrRecord[(Instruction *)V] == AC){
                ValueMap[instr] = ValueMap[(Instruction *)V];
            }
        }
        else{InstrRecord[instr] = NAC;}
    } // unary op

    else{
        float v1, v2;
        bool isAC = true;
        Value *V1 = instr->useeList[0]->V, *V2 = instr->useeList[1]->V;

        if(V1->isConst()){
            if(V1->V_type == IntConst){
                v1 = (float)stoi(V1->name);
            }
            else{v1 = stof(V1->name);} 
        }
        else if(V1->isInstr() && InstrRecord[(Instruction *)V1] != NAC){
            v1 = ValueMap[(Instruction *)V1];
        }
        else{isAC = false;}

        if(V2->isConst()){
            if(V2->V_type == IntConst){
                v2 = (float)stoi(V2->name);
            }
            else{v2 = stof(V2->name);} 
        }
        else if(V2->isInstr() && InstrRecord[(Instruction *)V2] != NAC){
            v2 = ValueMap[(Instruction *)V2];
        }
        else{isAC = false;}

        if(!isAC){InstrRecord[instr] = NAC;}
        else{
            if(V1->isConst() || V2->isConst() ||
               InstrRecord[(Instruction *)V1] == AC || InstrRecord[(Instruction *)V2] == AC){
                InstrRecord[instr] = AC;
                switch(instr->op_type){
                case _add_: ValueMap[instr] = v1 + v2; break;
                case _sub_: ValueMap[instr] = v1 - v2; break;
                case _mul_: ValueMap[instr] = v1 * v2; break;
                case _div_: ValueMap[instr] = v1 / v2; break;
                case _mod_: ValueMap[instr] = (int)v1 % (int)v2; break;
                case _ge_: ValueMap[instr] = v1 >= v2; break;
                case _le_: ValueMap[instr] = v1 <= v2; break;
                case _gt_: ValueMap[instr] = v1 > v2; break;
                case _lt_: ValueMap[instr] = v1 < v2; break;
                case _eq_: ValueMap[instr] = v1 == v2; break;
                case _ne_: ValueMap[instr] = v1 != v2; break;
                }
            }
            else{InstrRecord[instr] = UNDEF;} 
        }
    } // binary op
}

void SCCPPass::visitBr(Instruction *instr){
    if(instr->op_type == _jump_){
        BasicBlock *from = instr->master, *to = ((Jump *)instr)->getTarget();
        Edge edge(from, to);
        EdgeWorklist.push_back(edge);
    } // jump

    else if(instr->op_type == _br_){
        BasicBlock *from = instr->master; 
        BasicBlock *to1 = ((Br *)instr)->getBr1(), *to2 = ((Br *)instr)->getBr2();
        Value *cond = instr->useeList[0]->V;

        float v_cond = 0;
        bool isAC = true;
        if(cond->isConst()){
            if(cond->V_type == IntConst){
                v_cond = (float)stoi(cond->name);
            }
            else{v_cond = stof(cond->name);}
        }
        else if(cond->isInstr() && InstrRecord[(Instruction *)cond] != NAC){
            v_cond = ValueMap[(Instruction *)cond];
        }
        else{isAC = false;}

        if(isAC){
            if(v_cond){
                Edge edge(from, to1);
                EdgeWorklist.push_back(edge);
            }
            else{
                Edge edge(from, to2);
                EdgeWorklist.push_back(edge);
            }
        }
        else{
            Edge edge1(from, to1), edge2(from, to2);
            EdgeWorklist.push_back(edge1);
            EdgeWorklist.push_back(edge2);
        }
    } // br
}

void SCCPPass::visitPhi(Instruction *instr){
    float v_phi = 0;
    bool isAC = true, isDef = false;

    for(int i = 0; i < ((Phi *)instr)->usee_size; i++){
        float v;
        Value *V = ((Phi *)instr)->useeList[i]->V;
        if(V->isConst()){
            if(V->V_type == IntConst){
                v = (float)stoi(V->name);
            }
            else{v = stof(V->name);}
            if(isDef && v != v_phi){isAC = false; break;}
            v_phi = v, isDef = true;
        }
        else if(V->isInstr()){
            if(bbReachMap[((Phi *)instr)->bbFromList[i]]){
                if(InstrRecord[(Instruction *)V] == NAC){isAC = false; break;}
                else if(InstrRecord[(Instruction *)V] == AC){
                    v = ValueMap[(Instruction *)V];
                    if(isDef && v != v_phi){isAC = false; break;}
                    v_phi = v, isDef = true;
                }
            }
        }
        else{isAC = false; break;}
    } // test the souce and value of Phi

    if(isAC){
        ValueMap[instr] = v_phi;
        InstrRecord[instr] = AC; // how to deal with UNDEF ???
    }
    else{InstrRecord[instr] = NAC;}
}

void SCCPPass::updateExp(Instruction *instr){
    if(InstrRecord[instr] == AC){
        Value *new_V = NULL;
        valueUnion V_Const;
        if(instr->V_Data.val_type == I32 || instr->V_Data.val_type == CONST_I32 ||
           instr->V_Data.val_type == I1){
            V_Const.i = (int)ValueMap[instr];
            new_V = new Constant(IntConst, V_Const);
        }
        else{
            V_Const.f = ValueMap[instr];
            new_V = new Constant(FloatConst, V_Const);
        }
        instr->replaceAllUsesWith(new_V);
        instr->remove();
    }
}

void SCCPPass::updateBr(Instruction *instr){
    if(instr->op_type == _jump_){return;}
    BasicBlock *from = instr->master; 
    BasicBlock *to1 = ((Br *)instr)->getBr1(), *to2 = ((Br *)instr)->getBr2();

    if(EdgeRecord[from][to1] && !EdgeRecord[from][to2]){
        instr->remove();
        Instruction *new_instr = new Jump(from, to1);
        from->insertInstr(new_instr);
    }

    else if(!EdgeRecord[from][to1] && EdgeRecord[from][to2]){
        instr->remove();
        Instruction *new_instr = new Jump(from, to2);
        from->insertInstr(new_instr);
    }
}

void SCCPPass::updatePhi(Instruction *instr){
    updateExp(instr);
}

/* func for testing */
void SCCPPass::printInfo(){
    cout << "InstrRecord:" << endl;
    for(auto iter = InstrRecord.begin(); iter != InstrRecord.end(); iter++){
        cout << "  " << iter->first->name << " | " << iter->second << endl;
    }
    cout << endl;

    cout << "ValueMap:" << endl;
    for(auto iter = ValueMap.begin(); iter != ValueMap.end(); iter++){
        cout << "  " << iter->first->name << " | " << iter->second << endl;
    }
    cout << endl;

    cout << "bbReachMap:" << endl;
    for(auto iter = bbReachMap.begin(); iter != bbReachMap.end(); iter++){
        cout << "  " << iter->first->name << " | " << iter->second << endl;
    }
    cout << endl;
}