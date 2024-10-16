#include "CFG.h"
using namespace std;

void CFGPass::Run(){
    RemoveDeadBB();
    CFGAnalysis();
    IDomAnalysis(false);
    IDomAnalysis(true);
    DomFrontierAnalysis();
    DomTreeAnalysis();

    // TestCFG(); // just for testing
}

void CFGPass::RemoveDeadBB(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncCFGBuild(iter->second);
        FuncRemoveDeadBB(iter->second);
    }
}

void CFGPass::CFGAnalysis(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncCFGBuild(iter->second);
        // FuncCFGAnalysis(iter->second);
    }
}

void CFGPass::IDomAnalysis(bool reverse){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncIDomAnalysis(iter->second, reverse);
    }
}

void CFGPass::DomFrontierAnalysis(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncDomFrontierAnalysis(iter->second);
    }
}

void CFGPass::DomTreeAnalysis(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncDomTreeAnalysis(iter->second);
    }
}

void CFGPass::LoadG(FuncModule *FM, bool update){
    pPredG = &(FM->PredG);
    pSuccG = &(FM->SuccG);
    pBBs = &(FM->BBs);

    if(update){
        pPredG->clear();
        pSuccG->clear();
        pBBs->clear();
    }
}

void CFGPass::FuncDFS(BasicBlock *bb, unordered_set<BasicBlock *>& know){
    if(know.count(bb)){return;}
    else{
        know.insert(bb);
        for(auto iter = bb->succBB.begin(); iter != bb->succBB.end(); iter++){
            FuncDFS(*iter, know);
        }
    }
}

void CFGPass::FuncDomDFS(BasicBlock *bb, BasicBlock *target_bb, unordered_set<BasicBlock *>& know, bool reverse){
    if(know.count(bb)){return;}
    if(bb == target_bb){
        know.insert(bb);
        return;
    }
    else{
        know.insert(bb);
        // if(reverse){pCurG = pPredG;}
        // else{pCurG = pSuccG;}
        if(reverse){
            for(auto iter = bb->precBB.begin(); iter != bb->precBB.end(); iter++){
                FuncDomDFS(*iter, target_bb, know, reverse);
            }
        }
        else{
            for(auto iter = bb->succBB.begin(); iter != bb->succBB.end(); iter++){
                FuncDomDFS(*iter, target_bb, know, reverse);
            }
        }
    }
}

void CFGPass::FuncCFGBuild(FuncModule *FM){
    LoadG(FM, true);

    BasicBlock *bb_cur = FM->entry();
    while(!FM->bbList.isend(bb_cur)){
        (*pPredG)[bb_cur] = &(bb_cur->precBB);
        (*pSuccG)[bb_cur] = &(bb_cur->succBB);
        pBBs->insert(bb_cur);

        bb_cur = (BasicBlock *)(bb_cur->next);
    }

    bb_cur = FM->entry();
    while(!FM->bbList.isend(bb_cur)){
        Instruction *instr_end = bb_cur->exit();
        if(instr_end->op_type == _br_){
            BasicBlock *br1 = ((Br *)instr_end)->getBr1();
            BasicBlock *br2 = ((Br *)instr_end)->getBr2();

            bb_cur->succBB.insert(br1);
            bb_cur->succBB.insert(br2);
            (*pPredG)[br1]->insert(bb_cur);
            (*pPredG)[br2]->insert(bb_cur);
        }
        else if(instr_end->op_type == _jump_){
            BasicBlock *target = ((Jump *)instr_end)->getTarget();

            bb_cur->succBB.insert(target);
            (*pPredG)[target]->insert(bb_cur);
        }

        bb_cur = (BasicBlock *)(bb_cur->next);
    }

    for(auto iter = pBBs->begin(); iter != pBBs->end(); iter++){
        if((*iter)->succBB.empty()){FM->real_exit->precBB.insert(*iter);}
    } // add info about real exit
}

void CFGPass::FuncRemoveDeadBB(FuncModule *FM){
    unordered_set<BasicBlock *> know;
    BasicBlock *root = FM->entry();
    FuncDFS(root, know);

    BasicBlock *bb_cur = root;
    while(!FM->bbList.isend(bb_cur)){
        BasicBlock *next_bb_cur = (BasicBlock *)(bb_cur->next);
        if(!know.count(bb_cur)){
            bb_cur->remove();
        }
        bb_cur = next_bb_cur;
    }
}

void CFGPass::FuncIDomAnalysis(FuncModule *FM, bool reverse){
    LoadG(FM, false);

    BasicBlock *bb_cur = FM->entry();
    while(!FM->bbList.isend(bb_cur)){
        unordered_set<BasicBlock *> know;
        BasicBlock *root = FM->entry();
        if(reverse){root = FM->real_exit;}
        FuncDomDFS(root, bb_cur, know, reverse);

        for(auto iter = pBBs->begin(); iter != pBBs->end(); iter++){
            if(!know.count(*iter)){
                if(reverse){bb_cur->RDoms.insert(*iter);}
                else{bb_cur->Doms.insert(*iter);}
            }
        }

        bb_cur = (BasicBlock *)(bb_cur->next);
    } // get Doms/RDoms

    unordered_map<BasicBlock *, int> DomVisitMap;
    for(auto iter = pBBs->begin(); iter != pBBs->end(); iter++){DomVisitMap[*iter] = 0;}

    if(!reverse){
        bb_cur = FM->exit();
        while(!FM->bbList.isbegin(bb_cur)){
            for(auto iter = bb_cur->Doms.begin(); iter != bb_cur->Doms.end(); iter++){
                if(!DomVisitMap[*iter]){
                    bool flag = true;
                    for(auto iter1 = bb_cur->Doms.begin(); iter1 != bb_cur->Doms.end(); iter1++){
                        if((*iter1)->Doms.count(*iter)){flag = false; break;}
                    }
                    if(flag){
                        bb_cur->IDoms.insert(*iter);
                        (*iter)->IDomParent = bb_cur;
                        DomVisitMap[*iter] = 1;
                    }
                }
            }

            bb_cur = (BasicBlock *)(bb_cur->prev);
        }
    } // get IDoms

    else{
        bb_cur = FM->entry();
        while(!FM->bbList.isend(bb_cur)){
            for(auto iter = bb_cur->RDoms.begin(); iter != bb_cur->RDoms.end(); iter++){
                if(!DomVisitMap[*iter]){
                    bool flag = true;
                    for(auto iter1 = bb_cur->RDoms.begin(); iter1 != bb_cur->RDoms.end(); iter1++){
                        if((*iter1)->RDoms.count(*iter)){flag = false; break;}
                    }
                    if(flag){
                        bb_cur->IRDoms.insert(*iter);
                        DomVisitMap[*iter] = 1;
                    }
                }
            }

            bb_cur = (BasicBlock *)(bb_cur->next);
        }
    } // get IRDoms
} // a naive IDom analysis (LT/SEMI-NCA is too complex ...)

void CFGPass::FuncDomFrontierAnalysis(FuncModule *FM){
    pBBs = &(FM->BBs);

    BasicBlock *bb_cur = FM->entry();
    unordered_map<BasicBlock *, int> DFVisitMap;
    for(auto iter = pBBs->begin(); iter != pBBs->end(); iter++){DFVisitMap[*iter] = 0;}

    bbDomFrontierAnalysis(FM->entry(), DFVisitMap);
}

void CFGPass::bbDomFrontierAnalysis(BasicBlock *bb, unordered_map<BasicBlock *, int>& visit){
    for(auto iter = bb->succBB.begin(); iter != bb->succBB.end(); iter++){
        if(!bb->Doms.count(*iter)){bb->DomFrontiers.insert(*iter);}
    }

    for(auto iter = bb->IDoms.begin(); iter != bb->IDoms.end(); iter++){
        if(!visit[*iter]){bbDomFrontierAnalysis(*iter, visit);}
        for(auto iter1 = (*iter)->DomFrontiers.begin(); iter1 != (*iter)->DomFrontiers.end(); iter1++){
            if(!bb->Doms.count(*iter1)){bb->DomFrontiers.insert(*iter1);}
        }
    }

    visit[bb] = 1;
}

void CFGPass::FuncDomTreeAnalysis(FuncModule *FM){
    unordered_set<BasicBlock *> visited;
    DFS4DomTree(FM->entry(), 0, visited);
}

void CFGPass::DFS4DomTree(BasicBlock *bb, int depth, unordered_set<BasicBlock *>& visited){
    if(visited.count(bb)){return;}
    visited.insert(bb);
    bb->dom_depth = depth;

    for(auto iter = bb->IDoms.begin(); iter != bb->IDoms.end(); iter++){
        if(!visited.count(*iter)){
            DFS4DomTree(*iter, depth + 1, visited);
        }
    }
}

/* func for testing */
void CFGPass::TestCFG(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        FuncTestCFG(iter->second);
    }
}

void CFGPass::FuncTestCFG(FuncModule *FM){
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        cout << (*iter)->name << ":" << endl;

        cout << "  DOMS: ";
        for(auto iter1 = (*iter)->Doms.begin(); iter1 != (*iter)->Doms.end(); iter1++){
            cout << (*iter1)->name << " ";
        }
        cout << endl;

        cout << "  IDOMS: ";
        for(auto iter1 = (*iter)->IDoms.begin(); iter1 != (*iter)->IDoms.end(); iter1++){
            cout << (*iter1)->name << " ";
        }
        cout << endl;

        cout << "  RDOMS: ";
        for(auto iter1 = (*iter)->RDoms.begin(); iter1 != (*iter)->RDoms.end(); iter1++){
            cout << (*iter1)->name << " ";
        }
        cout << endl;

        cout << "  DF: ";
        for(auto iter1 = (*iter)->DomFrontiers.begin(); iter1 != (*iter)->DomFrontiers.end(); iter1++){
            cout << (*iter1)->name << " ";
        }
        cout << endl;

        cout << endl;
    }
}