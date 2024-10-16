#include "Table.h"
using namespace std;

SYM sym;

scope::scope(int L, int pa, int si, int ch, int ro):
level(L), parent(pa), sibling(si), child(ch), root(ro){
    ret_mark = 0;
    ret_type = 0;
}

int scope::table_insert(string key, valueStruct *value){
    if(table.find(key) != table.end()){return -1;} // refuse Simple/Array/Func with same name in one scope
    table[key] = value;
    return 0;
}

scope::~scope(){
    for(auto iter = table.begin(); iter != table.end(); iter++){
        if(iter->second){delete iter->second;}
    }
    table.clear();
    for(int i = 0; i < ident_list.size(); i++){
        if(ident_list[i]){delete ident_list[i];}
    }
    ident_list.clear();
    for(int i = 0; i < fparam_list.size(); i++){
        if(fparam_list[i]){delete fparam_list[i];}
    }
    fparam_list.clear();
}

void table_memory_error(){
    cout << "Error: run out of memory" << endl;
    exit(-1);
}

void sym_init(SYM& sym){
    scope *p_scope = new scope(0, -1, -1, -1, 0);
    if(p_scope == NULL){table_memory_error();}
    sym.Stack.push_back(p_scope);
    sym.scope_trace.push_back(0);
    sym.cur = 0;
}

valueStruct *sym_find(SYM& sym, string key, int& index){
    int level = sym.cur;
    while(level >= 0){
        if(sym.Stack[level]->table.find(key) != sym.Stack[level]->table.end()){
            index = level;
            return sym.Stack[level]->table[key];
        }
        level = sym.Stack[level]->parent;
    }
    return NULL;
}

valueStruct *sym_find_def(SYM& sym, string key, int& index){
    int level = sym.cur;
    while(level >= 0){
        auto target = sym.Stack[level]->table.find(key);
        if(target != sym.Stack[level]->table.end()){
            if(target->second->def_mark){
                index = level;
                return sym.Stack[level]->table[key];
            }
        }
        level = sym.Stack[level]->parent;
    }
    return NULL;
}

int sym_insert(SYM& sym, string key, valueStruct *value){
    return sym.Stack[sym.cur]->table_insert(key, value);
}

void sym_delete(SYM& sym){
    for(int i = 0; i < sym.Stack.size(); i++){
        if(sym.Stack[i]){delete sym.Stack[i];}
    }
    sym.Stack.clear();
    // for(int i = 0; i < sym.Func_list.size(); i++){
    //     if(sym.Func_list[i]){delete sym.Func_list[i];}
    // }
    // sym.Func_list.clear();
    for(int i = 0; i < sym.Array_list.size(); i++){
        if(sym.Array_list[i]){delete sym.Array_list[i];}
    }
    sym.Array_list.clear();
    for(auto iter = sym.IConstant_table.begin(); iter != sym.IConstant_table.end(); iter++){
        if(iter->second){delete iter->second;}
    }
    sym.IConstant_table.clear();
    for(auto iter = sym.FConstant_table.begin(); iter != sym.FConstant_table.end(); iter++){
        if(iter->second){delete iter->second;}
    }
    sym.FConstant_table.clear();
}

void scope_start(SYM& sym){
    int top = sym.Stack.size();
    if(sym.Stack[sym.cur]->child == -1){sym.Stack[sym.cur]->child = top;}
    else{
        int next = sym.Stack[sym.cur]->child;
        while(sym.Stack[next]->sibling != -1){next = sym.Stack[next]->sibling;}
        sym.Stack[next]->sibling = top;
    }
    scope *p_scope = new scope(sym.Stack[sym.cur]->level + 1, sym.cur, -1, -1, sym.Stack[sym.cur]->root);
    if(p_scope == NULL){table_memory_error();}
    if(p_scope->level == 1){p_scope->root = top;}
    sym.Stack.push_back(p_scope);
    sym.cur = top;
    sym.scope_trace.push_back(sym.cur);
}

void scope_end(SYM& sym){
    sym.cur = sym.Stack[sym.cur]->parent;
    sym.scope_trace.push_back(sym.cur);
}

void scope_forward(SYM& sym, int& pos){
    if(pos < sym.scope_trace.size()){sym.cur = sym.scope_trace[++pos];}
}

bool globalvar_insert(SYM& sym, Value *V, Value *Init){
    if(sym.GlobalVar_table.count(V)){return false;}
    sym.GlobalVar_table[V] = Init;
    return true;
}