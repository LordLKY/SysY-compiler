#pragma once
#include "../IRModule.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>

using std::unordered_map;
using std::unordered_set;
using std::vector;

class LoopConstructPass{
public:
    unordered_map<string, FuncModule *>* pFT;

    LoopConstructPass(unordered_map<string, FuncModule*>* pFT): pFT(pFT){}
    void Run();
    void loop_construct();

    Loop *collectLoop(FuncModule *FM);
    void DFS4collectLoop(BasicBlock *bb, Loop *loop);
    void buildLoopTree(Loop *loop, unordered_set<BasicBlock *>& visited);

    /* func for testing */
    void printInfo();
};

/*
this pass can only deal with reducible graph,
which means one loop is either sparated apart from, or completely included in another loop
graph that isn't reducible is rare and hard to analyse (that's why the abusion of 'goto' is not recommended)
I only tested my code on some graphs with simple loop structure,
so it may cause some bugs in some very complex loop graphs 
*/