#include "LoopConstruct.h"
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

void LoopConstructPass::Run(){
    loop_construct();

    // printInfo(); // just for testing
}

void LoopConstructPass::loop_construct(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        Loop *rootLoop = collectLoop(iter->second);

        unordered_set<BasicBlock *> visited;
        buildLoopTree(rootLoop, visited);
    }
}

Loop *LoopConstructPass::collectLoop(FuncModule *FM){
    FM->Loops.clear();
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        (*iter)->LoopStartWith.clear();
    } // updating may be necessary

    Loop *RootLoop = new Loop("LOOP" + to_string(++LOOP_COUNT), FM->entry(), FM->real_exit, FM);
    RootLoop->isRoot = true;
    RootLoop->layer = 0;
    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        if(*iter != FM->entry()){RootLoop->addBB(*iter);}
    }
    FM->entry()->addLoop(RootLoop);

    for(auto iter = FM->BBs.begin(); iter != FM->BBs.end(); iter++){
        for(auto iter1 = (*iter)->precBB.begin(); iter1 != (*iter)->precBB.end(); iter1++){
            if((*iter)->Doms.count(*iter1)){
                Loop *loop = new Loop("LOOP" + to_string(++LOOP_COUNT), *iter, *iter1, FM);
                (*iter)->addLoop(loop);
                FM->addLoop(loop);
                DFS4collectLoop(*iter1, loop);
                loop->bbSet.erase(*iter1); // bbSet doesn't include tail
            }
        }
    }

    return RootLoop;
}

void LoopConstructPass::DFS4collectLoop(BasicBlock *bb, Loop *loop){
    if(bb == loop->head){return;}
    loop->addBB(bb);
    for(auto iter = bb->precBB.begin(); iter != bb->precBB.end(); iter++){
        if(!loop->bbSet.count(*iter) && !bb->Doms.count(*iter)){
            DFS4collectLoop(*iter, loop);
        }
    }
}

void LoopConstructPass::buildLoopTree(Loop *loop, unordered_set<BasicBlock *>& visited){
    BasicBlock *head = loop->head, *tail = loop->tail;
    head->loop_depth = max(loop->layer, head->loop_depth);
    if(!visited.count(head)){visited.insert(head);}

    reverse(loop->bbRPO.begin(), loop->bbRPO.end());
    vector<BasicBlock *>& worklist = loop->bbRPO; // bbs in loop with top-order

    for(int i = 0; i < worklist.size(); i++){
        BasicBlock *bb = worklist[i];
        if(visited.count(bb)){continue;}
        bb->loop_depth = max(loop->layer, bb->loop_depth);

        for(auto iter = bb->LoopStartWith.begin(); iter != bb->LoopStartWith.end(); iter++){
            loop->addChild(*iter);
            (*iter)->setParent(loop);
            (*iter)->layer = loop->layer + 1;
            buildLoopTree(*iter, visited);
        }

        if(!visited.count(bb)){visited.insert(bb);}
    }
}

/* func for testing */
void LoopConstructPass::printInfo(){
    for(auto iter = pFT->begin(); iter != pFT->end(); iter++){
        for(auto iter1 = (iter->second)->Loops.begin(); iter1 != (iter->second)->Loops.end(); iter1++){
            (*iter1)->Print();
        }
    }
}