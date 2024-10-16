#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>

// using namespace std;
using std::vector;
using std::unordered_map;
using std::string;

union valueUnion{
    int i;
    float f;
};

class Value;
class FuncModule;

typedef enum{
    VOID_TYPE = 0,
    I1,
    CONST_I32,
    I32,
    CONST_F32,
    F32,
    I32_PTR,
    F32_PTR
}val_type_enum; // a label to distinguish types
/*
to avoid complex operation of pointer, there are no explicit poiter-types in my IR
an ident can be inferred as a pointer if:
1. its V-type is MEM
2. its sym_type is Array
PTR here refer to array used as FuncParam (ex. a in func(int a[][3]){a[2][2] = 1;})
this is not an ideal design
*/

typedef enum{
    Simple = 1,
    Array,
    Func
}sym_type_enum;

struct valueStruct{
    int val_type;
    int sym_type;
    bool def_mark;
    int other_index;
    valueUnion val;
    Value *V;

    valueStruct(){def_mark = false; V = NULL;}
    valueStruct(int val_type, int sym_type, bool def_mark, int other_index):
        val_type(val_type), sym_type(sym_type), def_mark(def_mark), other_index(other_index){
            V = NULL;
        }
    ~valueStruct(){delete V;}
};

typedef struct{
    string name;
    int val_type;
    int sym_type;
    int scope_index;
    int other_index; // only for Array
    Value *V;
}identInfo; // terrible design! can be refined

class scope{
public:
    unordered_map<string, valueStruct *> table;
    vector<identInfo *> ident_list; // used in func or global scope
    vector<identInfo *> fparam_list; // used in func scope
    int level;
    int parent;
    int sibling; // example: {...} {...}
    int child; // example: { {...} }
    int root; // root is func or global scope (level1 or level0)
    int ret_mark; // termiante when meet "return"
    int ret_type;

    scope(int L, int pa, int si, int ch, int ro);
    int table_insert(string key, valueStruct *value);
    ~scope();
};

typedef struct{
    int val_type;
    vector<int> dim_list;
}arrayStruct;

typedef struct{
    vector<scope *> Stack;
    // vector<funcStruct *> Func_list;
    vector<arrayStruct *> Array_list;
    unordered_map<string, Value *> IConstant_table;
    unordered_map<string, Value *> FConstant_table;
    unordered_map<string, FuncModule*> Function_table;
    unordered_map<Value *, Value *> GlobalVar_table;
    vector<int> scope_trace; // trace the change of scope in parser
    int cur;
}SYM;

extern SYM sym;

void table_memory_error();

void sym_init(SYM& sym);
valueStruct *sym_find(SYM& sym, string key, int& index); // used in parser
valueStruct *sym_find_def(SYM& sym, string key, int& index); // used in AST, deal with: {int x;{int x = x + 1;}}
int sym_insert(SYM& sym, string key, valueStruct *value);
void sym_delete(SYM& sym);
void scope_start(SYM& sym); // used in parser
void scope_end(SYM& sym); // used in parser
void scope_forward(SYM& sym, int& pos); // used in AST, refer to scope_trace
bool globalvar_insert(SYM& sym, Value *V, Value *Init);