#pragma once
#include "LIR.h"
#include <stack>
#include <deque>
#define REG_NUM 16
#define INIT_MEM_SIZE 10000
using std::stack;
using std::deque;

extern bool Prof;

class STACK{
public:
    stack<double> op_stack;

    void push(double data){op_stack.push(data);}
    double pop(){
        double data = op_stack.top();
        op_stack.pop();
        return data;
    }
    double top(){return op_stack.top();}
    void op_add();
    void op_sub();
    void op_mul();
    void op_div_i();
    void op_div_f();
    void op_mod();
    void op_eq();
    void op_ne();
    void op_gt();
    void op_lt();
    void op_ge();
    void op_le();
    bool op_if();
};

class REG_FILE{
public:
    vector<double> reg_space;

    REG_FILE(){reg_space.resize(REG_NUM);}
    double read_reg(int id);
    void write_reg(int id, double data);
};

class DATA_MEM{
public:
    vector<double> mem_space;
    int top;

    DATA_MEM(): top(0) {mem_space.resize(INIT_MEM_SIZE);}
    double read_mem(int id);
    void write_mem(int id, double data);
};

class RUNTIME{
public:
    bool ShowRuntime = false;
    int ip, stride;
    double ret;
    vector<string> InstrSet;
    STACK runtime_stack;
    REG_FILE reg_file;
    DATA_MEM data_mem;
    deque<unordered_map<string, int> *> AddrMaps;
    unordered_map<string, int> LabelMap;

    RUNTIME(): ip(0), stride(1){}
    void Run();
    void SetRuntime(bool showRuntime){ShowRuntime = showRuntime;}
    void LoadInstr(vector<string>& LIRset);
    void InstrDecode(string& instr, string& s1, string& s2);
    void InstrExe(string& op, string& obj);
    int CalStride(string& obj, bool all);
    int FindAddr(string& obj);
};