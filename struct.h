//Baba SOW - 12304140 &  Alioune SECK - 12209348 

#ifndef STRUCT_H
#define STRUCT_H

#include <stdbool.h>

// Types de nœuds pour l'arbre syntaxique
typedef enum {
    NOEUD_NOMBRE,
    NOEUD_VARIABLE,
    NOEUD_OP_ADD,
    NOEUD_OP_SUB,
    NOEUD_OP_MUL,
    NOEUD_OP_DIV,
    NOEUD_BOOL_EQ,
    NOEUD_BOOL_LT,
    NOEUD_BOOL_GT,
    NOEUD_BOOL_LE,
    NOEUD_BOOL_GE,
    NOEUD_BOOL_ET,
    NOEUD_BOOL_OU,
    NOEUD_BOOL_NON,
    NOEUD_SKIP,
    NOEUD_ASSIGN,
    NOEUD_IF,
    NOEUD_WHILE,
    NOEUD_SEQ,
    NOEUD_BLOC
} TypeNoeud;

// Structure pour un nœud de l'arbre syntaxique
typedef struct NoeudAST {
    TypeNoeud type;
    union {
        int valeur;
        char *nom;
        struct {
            struct NoeudAST *gauche;
            struct NoeudAST *droite;
        } op;
        struct {
            struct NoeudAST *cond;
            struct NoeudAST *alors;
            struct NoeudAST *sinon;
        } if_noeud;
        struct {
            struct NoeudAST *cond;
            struct NoeudAST *corps;
        } while_noeud;
        struct {
            struct NoeudAST *premiere;
            struct NoeudAST *deuxieme;
        } seq;
    } contenu;
} NoeudAST;

// Structure pour la liste des variables
typedef struct VariableNode {
    char *nom;
    int valeur;  
    struct VariableNode *suivant;
} NoeudVariable;

typedef struct {
    NoeudVariable *tete;
} VariableList;

// Fonctions pour créer des nœuds
NoeudAST *creerNoeudNombre(int valeur);
NoeudAST *creerNoeudVariable(char *nom);
NoeudAST *creerNoeudOperation(TypeNoeud type, NoeudAST *gauche, NoeudAST *droite);
NoeudAST *creerNoeudIf(NoeudAST *cond, NoeudAST *alors, NoeudAST *sinon);
NoeudAST *creerNoeudWhile(NoeudAST *cond, NoeudAST *corps);
NoeudAST *creerNoeudAssign(char *nom, NoeudAST *valeur);
NoeudAST *creerNoeudSkip();
NoeudAST *creerNoeudSeq(NoeudAST *premiere, NoeudAST *deuxieme);
NoeudAST *creerNoeudBloc(NoeudAST *commandes);

// Fonctions pour obtenir les informations d'un nœud
TypeNoeud obtenirTypeNoeud(NoeudAST *noeud);
NoeudAST *obtenirFilsGauche(NoeudAST *noeud);
NoeudAST *obtenirFilsDroit(NoeudAST *noeud);
char *obtenirNomVariable(NoeudAST *noeud);
int obtenirValeurNombre(NoeudAST *noeud);

// Fonctions pour gérer la liste des variables
VariableList *creerListeVariables();
bool ajouterVariable(VariableList *liste, char *nom);
void afficherListeVariables(VariableList *liste);
void libererListeVariables(VariableList *liste);
int obtenirValeurVariable(VariableList *liste, char *nom); 
void mettreAJourValeurVariable(VariableList *liste, char *nom, int valeur); 

// Fonctions pour afficher les nœuds de l'arbre syntaxique
void afficherExpression(NoeudAST *noeud);
void afficherBExpression(NoeudAST *noeud);
void afficherCommande(NoeudAST *noeud);

// Fonctions pour évaluer les nœuds de l'arbre syntaxique
int evaluerExpression(NoeudAST *noeud, VariableList *liste); 
void evaluerCommande(NoeudAST *noeud, VariableList *liste);

#endif
