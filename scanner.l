%{
//Baba SOW - 12304140 &  Alioune SECK - 12209348 
#include "struct.h"
#include "parser.tab.h"

extern VariableList *variableList;
%}

%%

"if"                    { return T_IF; }
"then"                  { return T_THEN; }
"else"                  { return T_ELSE; }
"while"                 { return T_WHILE; }
"do"                    { return T_DO; }
"skip"                  { return T_SKIP; }
":="                    { return T_ASSIGN; }
"and"                   { return T_AND; }
"or"                    { return T_OR; }
"not"                   { return T_NOT; }
"=="                    { return T_EQ; }
"<="                    { return T_LE; }
">="                    { return T_GE; }
"<"                     { return T_LT; }
">"                     { return T_GT; }
"{"                     { return '{'; }
"}"                     { return '}'; }
"."                     { return '.'; }
[0-9]+                  { yylval.noeud = creerNoeudNombre(atoi(yytext)); return T_NUMBER; }
[a-zA-Z][0-9]{0,2}      { yylval.noeud = creerNoeudVariable(strdup(yytext)); ajouterVariable(variableList, yytext); return T_VARIABLE; }
";"                     { return ';'; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
[ \t\n]+                { /*On les ignore*/ }
.                       { fprintf(stderr, "Erreur, caractère inconnu: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}
