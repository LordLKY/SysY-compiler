%{
#include<iostream>
#include<string>
#include "AST.h"
using namespace std;

int yylex();
void yyerror(astNode *&ast, const char *s); // changed by %parse-param
%}

%parse-param {astNode *&ast} // additional param for parser

%union{
    int iVal;
    float fVal;
    string *Ident;
    astNode *pNode;
}

%token <iVal> INTEGER
%token <fVal> DECIMAL
%token <Ident> IDENTIFIER
%token WHILE IF ELSE BREAK CONTINUE RETURN
%token INT FLOAT VOID CONST
%token OR AND GE LE EQ NE

%token PRINT

%type <pNode> CompUnitList CompUnit
%type <pNode> Decl ConstDecl VarDecl
%type <pNode> ConstDef ConstDefList ConstInitVal ConstArrayInitVal
%type <pNode> VarDef VarDefList VarInitVal VarArrayInitVal
%type <pNode> Block BlockItem BlockList
%type <pNode> Stmt Exp Cond LVal ConstExp
%type <pNode> PrimaryExp Number UnaryExp
%type <pNode> MulExp AddExp RelExp EqExp LAndExp LOrExp
%type <pNode> FuncDef FuncFParam FuncFParams FuncRParams FuncFArrayParam
%type <pNode> ArrayDef ArrayExp

%%

CompUnitList
    : CompUnit {
        $$ = newExp(_CompUnits, 1, $1); /* wrap as CompUnitList */
        ast = $$;
    }
    | CompUnitList CompUnit {
        $$ = mergeNode($1, $2); /* merge the nodes */
        ast = $$;
    };

CompUnit
    : Decl {$$ = $1;}
    | FuncDef {$$ = $1;};

Decl
    : ConstDecl {$$ = $1;}
    | VarDecl {$$ = $1;};

ArrayDef
    : IDENTIFIER '[' ConstExp ']' {$$ = newExp(_ArrayDef, 2, newId(Array, $1), $3);}
    | ArrayDef '[' ConstExp ']' {$$ = mergeNode($1, $3);};

ConstDecl
    : CONST INT ConstDefList ';' {
        $$ = $3;
        sym_preprocess_datadef(CONST_I32, $3);
    }
    | CONST FLOAT ConstDefList ';' {
        $$ = $3;
        sym_preprocess_datadef(CONST_F32, $3);
    };

ConstDef
    : IDENTIFIER '=' ConstInitVal {$$ = newExp(_ConstDef, 2, newId(Simple, $1), $3);}
    | ArrayDef '=' ConstInitVal {$$ = newExp(_ConstDef, 2, $1, $3);};

ConstArrayInitVal
    : ConstInitVal {$$ = newExp(_ConstArrayInitVal, 1, $1);}
    | ConstArrayInitVal ',' ConstInitVal {$$ = mergeNode($1, $3);};

ConstInitVal
    : ConstExp {$$ = $1;}
    | '{' '}' {$$ = newExp(_ConstArrayInitVal, 0);}
    | '{' ConstArrayInitVal '}' {$$ = $2;};

ConstDefList
    : ConstDef {$$ = newExp(_ConstDefs, 1, $1); /* wrap as ConstDefList */}
    | ConstDefList ',' ConstDef {$$ = mergeNode($1, $3); /* merge the nodes*/};

VarDecl
    : INT VarDefList ';' {
        $$ = $2;
        sym_preprocess_datadef(I32, $2);
    }
    | FLOAT VarDefList ';' {
        $$ = $2;
        sym_preprocess_datadef(F32, $2);
    };

VarDef
    : IDENTIFIER {$$ = newExp(_VarDef, 1, newId(Simple, $1));}
    | IDENTIFIER '=' VarInitVal {$$ = newExp(_VarDef, 2, newId(Simple, $1), $3);};
    | ArrayDef {$$ = newExp(_VarDef, 1, $1);}
    | ArrayDef '=' VarInitVal {$$ = newExp(_VarDef, 2, $1, $3);};

VarArrayInitVal
    : VarInitVal {$$ = newExp(_VarArrayInitVal, 1, $1);}
    | VarArrayInitVal ',' VarInitVal {$$ = mergeNode($1, $3);}; 

VarInitVal
    : Exp {$$ = $1;}
    | '{' '}' {$$ = newExp(_VarArrayInitVal, 0);}
    | '{' VarArrayInitVal '}' {$$ = $2;};

VarDefList
    : VarDef {$$ = newExp(_VarDefs, 1, $1); /* wrap as VarDefList */}
    | VarDefList ',' VarDef {$$ = mergeNode($1, $3); /* merge the nodes */};

FuncDef
    : INT IDENTIFIER '(' ')' Block {
        astNode *p = newId(Func, $2);
        $$ = newExp(_FuncDef, 2, p, $5);
        sym_preprocess_func(I32, p, newExp(_FuncFParams, 0));
    }
    | INT IDENTIFIER '(' FuncFParams ')' Block {
        astNode *p = newId(Func, $2);
        $$ = newExp(_FuncDef, 3, p, $4, $6);
        sym_preprocess_func(I32, p, $4);
    }
    | FLOAT IDENTIFIER '(' ')' Block {
        astNode *p = newId(Func, $2);
        $$ = newExp(_FuncDef, 2, p, $5);
        sym_preprocess_func(F32, p, newExp(_FuncFParams, 0));
    }
    | FLOAT IDENTIFIER '(' FuncFParams ')' Block {
        astNode *p = newId(Func, $2);
        $$ = newExp(_FuncDef, 3, p, $4, $6);
        sym_preprocess_func(F32, p, $4);
    }
    | VOID IDENTIFIER '(' ')' Block {
        astNode *p = newId(Func, $2);
        $$ = newExp(_FuncDef, 2, p, $5);
        sym_preprocess_func(VOID_TYPE, p, newExp(_FuncFParams, 0));
    }
    | VOID IDENTIFIER '(' FuncFParams ')' Block {
        astNode *p = newId(Func, $2);
        $$ = newExp(_FuncDef, 3, p, $4, $6);
        sym_preprocess_func(VOID_TYPE, p, $4);
    };

FuncFParams
    : FuncFParam {$$ = newExp(_FuncFParams, 1, $1);}
    | FuncFParams ',' FuncFParam {$$ = mergeNode($1, $3);};

FuncFParam
    : INT IDENTIFIER {$$ = newExp(_FuncFParam, 2, newI(_INT), newId(Simple, $2));}
    | FLOAT IDENTIFIER {$$ = newExp(_FuncFParam, 2, newI(_FLOAT), newId(Simple, $2));}
    | INT FuncFArrayParam {$$ = newExp(_FuncFParam, 2, newI(_INT), $2);}
    | FLOAT FuncFArrayParam {$$ = newExp(_FuncFParam, 2, newI(_FLOAT), $2);};

FuncFArrayParam
    : IDENTIFIER '[' ']' {$$ = newExp(_ArrayDef, 1, newId(Array, $1));}
    | FuncFArrayParam '[' Exp ']' {$$ = mergeNode($1, $3);};

ScopeStart
    : '{' {scope_start(sym);};

ScopeEnd
    : '}' {scope_end(sym);};

Block
    : ScopeStart ScopeEnd {$$ = newExp(_BlockItems, 0);}
    | ScopeStart BlockList ScopeEnd {$$ = $2;};

BlockItem
    : Decl {$$ = $1;}
    | Stmt {$$ = $1;};

BlockList
    : BlockItem {$$ = newExp(_BlockItems, 1, $1); /* wrap as BlockList */}
    | BlockList BlockItem {$$ = mergeNode($1, $2); /* merge the nodes */};

Stmt
    : LVal '=' Exp ';' {$$ = newExp('=', 2, $1, $3);}
    | ';' {$$ = newExp(';', 0);}
    | Exp ';' {$$ = $1;}
    | Block {$$ = $1;}
    | IF '(' Cond ')' Stmt ELSE Stmt {$$ = newExp(IF, 3, $3, $5, $7);}
    | IF '(' Cond ')' Stmt {$$ = newExp(IF, 2, $3, $5);}
    | WHILE '(' Cond ')' Stmt {$$ = newExp(WHILE, 2, $3, $5);}
    | BREAK ';' {$$ = newExp(BREAK, 0);}
    | CONTINUE ';' {$$ = newExp(CONTINUE, 0);}
    | RETURN ';' {$$ = newExp(RETURN, 0);}
    | RETURN Exp ';' {$$ = newExp(RETURN, 1, $2);}
    | PRINT '(' Exp ')' ';' {$$ = newExp(_PRINT, 1, $3);};

Exp
    : AddExp {$$ = $1;};

ArrayExp
    : IDENTIFIER '[' Exp ']' {$$ = newExp(_ArrayExp, 2, newId(Array, $1), $3);}
    | ArrayExp '[' Exp ']' {$$ = mergeNode($1, $3);};

Cond
    : LOrExp {$$ = $1;};

LVal
    : IDENTIFIER {$$ = newId(Simple, $1);}
    | ArrayExp {$$ = $1;};

PrimaryExp
    : Number {$$ = $1;}
    | LVal {$$ = $1;}
    | '(' Exp ')' {$$ = $2;};

Number
    : INTEGER {$$ = newI($1);}
    | DECIMAL {$$ = newF($1);};

UnaryExp
    : PrimaryExp {$$ = $1;}
    | IDENTIFIER '(' ')' {$$ = newExp(_FuncCall, 1, newId(Func, $1));}
    | IDENTIFIER '(' FuncRParams ')' {$$ = newExp(_FuncCall, 2, newId(Func, $1), $3);}
    | '+' UnaryExp {$$ = newExp('+', 2, newI(0), $2);}
    | '-' UnaryExp {$$ = newExp('-', 2, newI(0), $2);}
    | '!' UnaryExp {$$ = newExp(EQ, 2, newI(0), $2);};

FuncRParams
    : Exp {$$ = newExp(_FuncRParams, 1, $1);}
    | FuncRParams ',' Exp {$$ = mergeNode($1, $3);};

MulExp
    : UnaryExp {$$ = $1;}
    | MulExp '*' UnaryExp {$$ = newExp('*', 2, $1, $3);}
    | MulExp '/' UnaryExp {$$ = newExp('/', 2, $1, $3);}
    | MulExp '%' UnaryExp {$$ = newExp('%', 2, $1, $3);};

AddExp
    : MulExp {$$ = $1;}
    | AddExp '+' MulExp {$$ = newExp('+', 2, $1, $3);}
    | AddExp '-' MulExp {$$ = newExp('-', 2, $1, $3);};

RelExp
    : AddExp {$$ = $1;}
    | RelExp '<' AddExp {$$ = newExp('<', 2, $1, $3);}
    | RelExp '>' AddExp {$$ = newExp('>', 2, $1, $3);}
    | RelExp LE AddExp {$$ = newExp(LE, 2, $1, $3);}
    | RelExp GE AddExp {$$ = newExp(GE, 2, $1, $3);};

EqExp
    : RelExp {$$ = $1;}
    | EqExp EQ RelExp {$$ = newExp(EQ, 2, $1, $3);}
    | EqExp NE RelExp {$$ = newExp(NE, 2, $1, $3);};

LAndExp
    : EqExp {$$ = $1;}
    | LAndExp AND EqExp {$$ = newExp(AND, 2, $1, $3);};

LOrExp
    : LAndExp {$$ = $1;}
    | LOrExp OR LAndExp {$$ = newExp(OR, 2, $1, $3);};

ConstExp
    : AddExp {$$ = $1;};

%%

void yyerror(astNode *&ast, const char *s){
    extern int yylineno; // defined and maintained in lex
    extern char *yytext; // defined and maintained in lex
    cout << "Error: " << string(s) << " at symbol '";
    cout << string(yytext) << "' on line " << yylineno << endl;
    exit(-1);
}