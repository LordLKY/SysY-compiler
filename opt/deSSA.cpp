#include "deSSA.h"
using namespace std;

void deSSAPass::Run(){
    destroySSA();
}

void deSSAPass::destroySSA(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncModule *FM = iter->second;
        // for(auto iter0 = FM->BBs.begin(); iter0 != FM->BBs.end();){
        //     BBdestroySSA(*(iter0++));
        // }
        BasicBlock *bb_cur = FM->entry();
        while(!FM->bbList.isend(bb_cur)){
            BBdestroySSA(bb_cur);

            bb_cur = (BasicBlock *)(bb_cur->next);
        }
    }
}

void deSSAPass::BBdestroySSA(BasicBlock *bb){
    Instruction *instr_cur = bb->entry();
    while(!bb->instrList.isend(instr_cur)){
        Instruction *next_instr = (Instruction *)instr_cur->next;
        if(instr_cur->isPhi()){
            PhiRemove((Phi *)instr_cur);
        }

        instr_cur = next_instr;
    }
}

void deSSAPass::PhiRemove(Phi *instr){
    for(int i = 0; i < instr->usee_size; i++){
        BasicBlock *target = instr->bbFromList[i];
        Value *V_user = (Value *)instr;
        Value *V_usee = instr->useeList[i]->V;
        PhiDistributeUsee(instr->master, target, V_user, V_usee);
    }
    instr->remove();
}

void deSSAPass::PhiDistributeUsee(BasicBlock *source, BasicBlock *target, Value *V_user, Value *V_usee){
    if(target->succBB.size() == 1){
        Move *instr = new Move(target, V_usee, V_user);
        // if(!target->exit()->isBr()){
        //     target->insertInstr(instr);
        // }
        // else{target->exit()->insertBefore(instr);}
        MoveInsert(target, instr);
    } // can insert directly when edge is not critical
    else{
        BasicBlock *new_bb = new BasicBlock(target->master, false);
        target->master->BBs.insert(new_bb);
        target->insertAfter(new_bb);
        target->succBB.erase(source);
        target->succBB.insert(new_bb);
        new_bb->precBB.insert(target);
        new_bb->succBB.insert(source);
        if(target->exit()->isBr()){
            Instruction *br_instr = target->exit();
            for(int i = 0; i < br_instr->useeList.size(); i++){
                if(br_instr->useeList[i]->V->name == source->name){
                    br_instr->useeList[i]->V = new_bb;
                }
            }
        }

        Move *instr1 = new Move(new_bb, V_usee, V_user);
        Jump *instr2 = new Jump(new_bb, source);
        // new_bb->insertInstr(instr1);
        MoveInsert(new_bb, instr1);
        new_bb->insertInstr(instr2);
    } // need to insert new BB when edge is critical
}

void deSSAPass::MoveInsert(BasicBlock *bb, Instruction *instr){
    Instruction *insert_place = bb->exit();
    if(insert_place->isBr()){
        insert_place = (Instruction *)(insert_place->prev);
    }
    while(insert_place->op_type == _mov_){
        if(((Move *)insert_place)->useeList[0]->V == ((Move *)instr)->to){
            break;
        }
        insert_place = (Instruction *)(insert_place->prev);
    }
    insert_place->insertAfter(instr);
} // solve the reliance of Phi/Move