%{
#include <iostream>
#include <string>
#include <math.h>
#include "AST.h"
#include "parser.tab.h"
using namespace std;

void lex_error(string s);
float Hex2Dec_f(string hex); // hexadecimal-floating to decimal-floating
%}

%%

"float" {return FLOAT;}
"int" {return INT;}
"void" {return VOID;}
"const" {return CONST;}
"while" {return WHILE;}
"if" {return IF;}
"else" {return ELSE;}
"break" {return BREAK;}
"continue" {return CONTINUE;}
"return" {return RETURN;}

"print" {return PRINT;}

[_a-zA-Z][_a-zA-Z0-9]* { 
    yylval.Ident = new string(yytext, yyleng);
    return IDENTIFIER;
}

([0-9]+[\.][0-9]*|[0-9]*[\.][0-9]+) {
    // normal float
    yylval.fVal = atof(yytext);
    return DECIMAL;
}

0[Xx](([0-9a-fA-F]*\.[0-9a-fA-F]+)|([0-9a-fA-F]+\.)) {
    // hex float
    yylval.fVal = Hex2Dec_f(string(yytext).substr(2));
    return DECIMAL;
}

0 {
    // zero
    yylval.iVal = atoi(yytext);
    return INTEGER;
}

[1-9][0-9]* {
    // normal int
    yylval.iVal = atoi(yytext);
    return INTEGER;
}

0[Xx][0-9a-fA-F]+ {
    // hex int
    yylval.iVal = stoi(string(yytext).substr(2), nullptr, 16);
    return INTEGER;
}

0[0-7]* {
    // oct int
    yylval.iVal = stoi(string(yytext).substr(1), nullptr, 8);
    return INTEGER;
}

[-()<>=+*/%;!,] {return yytext[0];}
"{" {return yytext[0];}
"}" {return yytext[0];}
"[" {return yytext[0];}
"]" {return yytext[0];}

">=" {return GE;}
"<=" {return LE;}
"==" {return EQ;}
"!=" {return NE;}
"&&" {return AND;}
"||" {return OR;}

[ \t\r]+ {
    // whitespace
}

[\n] {
    yylineno++;
}

("//"[^\n]*) {
    // line comment
}

"/*"([^\*]|(\*)+[^/])*"*/" {
    // block comment
}

. lex_error("unknown character");

%%

void lex_error(string s){
    cout << "Error: " << s << " at symbol '";
    cout << string(yytext) << "' on line " << yylineno << endl;
    exit(-1);
}

float Hex2Dec_f(string hex){
    float decimal = 0.0, multiplier = 1.0;
    int len = hex.length();
    int decimalPointIndex = hex.find('.');
    if(decimalPointIndex != string::npos){
        for(int i = decimalPointIndex + 1; i < len; i++){
            char c = hex[i];
            if(c >= '0' && c <= '9'){decimal += (c - '0') * pow(16, len - i - 1);}
            else if(c >= 'a' && c <= 'f'){decimal += (c - 'a' + 10) * pow(16, len - i - 1);}
            else if(c >= 'A' && c <= 'F'){decimal += (c - 'A' + 10) * pow(16, len - i - 1);}
            else{}
            multiplier *= 16;
        }
        for (int i = 0; i < decimalPointIndex; i++) {
            char c = hex[i];
            if(c >= '0' && c <= '9'){decimal += (c - '0') * pow(16, len - i - 2);}
            else if(c >= 'a' && c <= 'f'){decimal += (c - 'a' + 10) * pow(16, len - i - 2);}
            else if(c >= 'A' && c <= 'F'){decimal += (c - 'A' + 10) * pow(16, len - i - 2);}
            else{}
        }
    }
    return decimal / multiplier;
}

int yywrap() {
    return 1;
}