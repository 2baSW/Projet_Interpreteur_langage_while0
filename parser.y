%{
//Baba SOW - 12304140 &  Alioune SECK - 12209348 
#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

extern int yylex(void);
void yyerror(const char *s);

NoeudAST *racineAST;
VariableList *variableList;
%}

%union {
    int val;
    char *id;
    NoeudAST *noeud;
}

%token <noeud> T_NUMBER
%token <noeud> T_VARIABLE
%token T_IF T_THEN T_ELSE T_WHILE T_DO T_SKIP T_ASSIGN T_AND T_OR T_NOT T_EQ T_LE T_GE T_LT T_GT
%left '+' '-'
%left '*' '/'
%right UMINUS
%left T_AND T_OR
%right T_NOT
%type <noeud> expr bexpr command program command_list

%%

program:
    command_list '.' { 
        racineAST = $1; 
        afficherCommande(racineAST);
        printf("\n");
        evaluerCommande(racineAST, variableList);
        printf("Liste des variables et leurs valeurs finales : \n");
        afficherListeVariables(variableList);
    }
    ;

command_list:
    command { $$ = $1; }
    | command_list ';' command { $$ = creerNoeudSeq($1, $3); }
    ;

command:
    T_SKIP { $$ = creerNoeudSkip(); }
    | T_VARIABLE T_ASSIGN expr { $$ = creerNoeudAssign(obtenirNomVariable($1), $3); }
    | T_IF bexpr T_THEN command T_ELSE command { $$ = creerNoeudIf($2, $4, $6); }
    | T_WHILE bexpr T_DO command { $$ = creerNoeudWhile($2, $4); }
    | '{' command_list '}' { $$ = creerNoeudBloc($2); }
    ;

bexpr:
    expr T_EQ expr { $$ = creerNoeudOperation(NOEUD_BOOL_EQ, $1, $3); }
    | expr T_LT expr { $$ = creerNoeudOperation(NOEUD_BOOL_LT, $1, $3); }
    | expr T_GT expr { $$ = creerNoeudOperation(NOEUD_BOOL_GT, $1, $3); }
    | expr T_LE expr { $$ = creerNoeudOperation(NOEUD_BOOL_LE, $1, $3); }
    | expr T_GE expr { $$ = creerNoeudOperation(NOEUD_BOOL_GE, $1, $3); }
    | bexpr T_AND bexpr { $$ = creerNoeudOperation(NOEUD_BOOL_ET, $1, $3); }
    | bexpr T_OR bexpr { $$ = creerNoeudOperation(NOEUD_BOOL_OU, $1, $3); }
    | T_NOT bexpr { $$ = creerNoeudOperation(NOEUD_BOOL_NON, $2, NULL); }
    | expr { $$ = $1; }
    ;

expr:
    T_VARIABLE { $$ = $1; }
    | expr '+' expr { $$ = creerNoeudOperation(NOEUD_OP_ADD, $1, $3); }
    | expr '-' expr { $$ = creerNoeudOperation(NOEUD_OP_SUB, $1, $3); }
    | expr '*' expr { $$ = creerNoeudOperation(NOEUD_OP_MUL, $1, $3); }
    | expr '/' expr { $$ = creerNoeudOperation(NOEUD_OP_DIV, $1, $3); }
    | '-' expr %prec UMINUS { $$ = creerNoeudOperation(NOEUD_OP_SUB, creerNoeudNombre(0), $2); }
    | T_NUMBER { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erreur: %s\n", s);
}

int main(void) {
    printf("Démarrage du programme\n");
    variableList = creerListeVariables();
    if (yyparse() == 0) {
        printf("Analyse syntaxique réussie\n");
    } else {
        printf("Échec de l'analyse syntaxique\n");
    }
    libererListeVariables(variableList);
    printf("Programme terminé!\n");
    return 0;
}
