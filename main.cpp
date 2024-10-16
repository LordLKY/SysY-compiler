#include <cassert>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <string>
#include "AST.h"
#include "PassManager.h"
#include "LIR.h"
#include "VM.h"
// #include "Table.h"
using namespace std;

extern FILE *yyin;
extern int yyparse(astNode *&ast);
// extern SYM sym;

bool OPT = true;
bool ShowIR = false;
bool ShowLIR = false;
bool ShowRuntime = false;

int main(int argc, const char *argv[]){
/* input */
    assert(argc >= 2);
    auto input = argv[1];

    for(int i = 2; i < argc; i++){
        auto arg_set = argv[i];

        if(strcmp(arg_set, "-O0") == 0){OPT = false; continue;}
        else if(strcmp(arg_set, "-O1") == 0){OPT = true; continue;}

        if(strcmp(arg_set, "-ShowIR") == 0){ShowIR = true; continue;}

        if(strcmp(arg_set, "-ShowLIR") == 0){ShowLIR = true; continue;}

        if(strcmp(arg_set, "-ShowRuntime") == 0){ShowRuntime = true; continue;}

        if(strcmp(arg_set, "-Prof") == 0){Prof = true; continue;}
    }  

/* scan & parse & build sym/AST */
    yyin = fopen(input, "r");
    assert(yyin);
    sym_init(sym);
    astNode *ast = nullptr;
    auto ret = yyparse(ast);
    sym.cur = 0;

/* sym/AST debug */
    // PrintSYM(sym);
    // PrintAST(ast, 0);

/* AST -> IR */
    ast->AST2IR();

/* run pass */
    PassManager PM(&sym.Function_table, &sym.GlobalVar_table);
    PM.SetOPT(OPT);
    PM.SetIR(ShowIR);
    PM.Run();

/* IR -> LIR */
    Convertor convertor(&sym.Function_table, &sym.GlobalVar_table);
    convertor.SetLIR(ShowLIR);
    convertor.Run();

/* runtime */
    RUNTIME runtime;
    runtime.SetRuntime(ShowRuntime);
    runtime.Run();

/* complete */
    ast->AST_delete();
    sym_delete(sym);

    return 0;
}
