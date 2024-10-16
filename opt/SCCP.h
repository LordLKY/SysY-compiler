#pragma once
#include "../IRModule.h"
#include <unordered_set>
#include <unordered_map>
#include <deque>

using std::unordered_map;
using std::unordered_set;
using std::deque;

struct Edge{
    BasicBlock *bb_from, *bb_to;

    Edge(BasicBlock *bb1, BasicBlock *bb2): bb_from(bb1), bb_to(bb2){}
};

typedef enum{
    UNDEF = 0,
    AC,
    NAC
}LatticeValue;

class SCCPPass{
public:
    unordered_map<string, FuncModule *>* pFT;

    deque<Edge> EdgeWorklist;
    deque<Instruction *> InstrWorklist;
    unordered_map<BasicBlock *, unordered_map<BasicBlock *, bool>> EdgeRecord;
    unordered_map<Instruction *, int> InstrRecord;
    unordered_map<BasicBlock *, bool> bbReachMap;
    unordered_map<Instruction *, float> ValueMap;

    SCCPPass(unordered_map<string, FuncModule*>* pFT): pFT(pFT){}
    void Run();
    void sccp();

    void Funcsccp(FuncModule *FM);
    void sccpInit(FuncModule *FM);
    void sccpUpdate(FuncModule *FM);
    void visitInstr(Instruction *instr);
    void visitExp(Instruction *instr);
    void visitBr(Instruction *instr);
    void visitPhi(Instruction *instr);
    void updateExp(Instruction *instr);
    void updateBr(Instruction *instr);
    void updatePhi(Instruction *instr);

    /* func for testing */
    void printInfo();
};