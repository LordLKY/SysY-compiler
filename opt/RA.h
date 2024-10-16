#pragma once
#include "../IRModule.h"
#include <unordered_set>
#include <unordered_map>
#include <deque>

using std::unordered_map;
using std::unordered_set;
using std::deque;

class raPass{
public:
    unordered_map<string, FuncModule *>* pFT;

    raPass(unordered_map<string, FuncModule*>* pFT): pFT(pFT){}
    void Run();
    void LA(); // LiveAnalysis
    void buildRIG(); // build conflict graph
    void RA(); // register allocation

    void FuncLA(FuncModule *FM);
    bool bbLA(BasicBlock *bb); // return whether there is any change
    Value *isRegDef(Instruction *instr); // whether a instr define a new reg
    void FuncRIG(FuncModule *FM); // build RIG for a func
    void FuncRA(FuncModule *FM);
    void RAdfs(deque<unordered_set<string>>& allocation,
               unordered_map<string, unordered_set<string>>& rig,
               deque<unordered_set<string>>& final_allocation,
               vector<string>& Regs,
               int& cnt, int cur);

    /* func for testing */
    void printLA();
    void printRIG();
    void printRA();
};

/*
this is a naive(suck) register allocation algorithm(RA)
it executes live-analysis(LA) with the granularity of basicblock which is not a good option
(but it's too costly with the granularity of instruction)
and construct RIG based on the result of LA
instead of using graph coloring(GC),
(to be honest, i think GC is not a good algorithm for RA
LA -> RIG -> coloring -> spilling -> reconstruct -> LA again -> RIG again -> coloring again ...
as you can see, it's cost can be very high
that's why LLVM uses an internal scanning method rather than GC)
i design a more simple but inefficient naturally algorithm,
do aggrassive GC but ignore the limit of REG_NUM,
leave most-used ones and treat spilling ones as mem,
ignore the reconstruction of spilling since i'm planning to use a stack-style VM as target machine
(which means we can access mem without reg
it's ... shameless)
as for whether it works, what can i say ...
*/