#pragma once
#include "../IRModule.h"
#include <unordered_set>
#include <unordered_map>

using std::unordered_map;
using std::unordered_set;

class CFGPass{
public:
    unordered_map<string, FuncModule *>* pFT;
    unordered_map<BasicBlock *, unordered_set<BasicBlock *>*>* pPredG;
    unordered_map<BasicBlock *, unordered_set<BasicBlock *>*>* pSuccG;
    // unordered_map<BasicBlock *, unordered_set<BasicBlock *>*>* pCurG = NULL;
    unordered_set<BasicBlock *>* pBBs;

    CFGPass(unordered_map<string, FuncModule*>* pFT): pFT(pFT){}
    void Run();
    void RemoveDeadBB();
    void CFGAnalysis();
    void IDomAnalysis(bool reverse);
    void DomFrontierAnalysis();
    void DomTreeAnalysis();

    void LoadG(FuncModule *FM, bool update);
    void FuncDFS(BasicBlock *bb, unordered_set<BasicBlock *>& know);
    void FuncDomDFS(BasicBlock *bb, BasicBlock *target_bb, unordered_set<BasicBlock *>& know, bool reverse);
    void FuncCFGBuild(FuncModule *FM); // build & update CFG
    void FuncRemoveDeadBB(FuncModule *FM);
    // void FuncCFGAnalysis(FuncModule *FM);
    void FuncIDomAnalysis(FuncModule *FM, bool reverse);
    void FuncDomFrontierAnalysis(FuncModule *FM);
    void bbDomFrontierAnalysis(BasicBlock *bb, unordered_map<BasicBlock *, int>& visit);
    void FuncDomTreeAnalysis(FuncModule *FM);
    void DFS4DomTree(BasicBlock *bb, int depth, unordered_set<BasicBlock *>& visited);

    /* func for testing */
    void TestCFG();
    void FuncTestCFG(FuncModule *FM);
};