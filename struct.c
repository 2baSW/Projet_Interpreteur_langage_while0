//Baba SOW - 12304140 &  Alioune SECK - 12209348 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

NoeudAST *creerNoeudNombre(int valeur) {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = NOEUD_NOMBRE;
    noeud->contenu.valeur = valeur;
    return noeud;
}

NoeudAST *creerNoeudVariable(char *nom) {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = NOEUD_VARIABLE;
    noeud->contenu.nom = strdup(nom);
    return noeud;
}

NoeudAST *creerNoeudOperation(TypeNoeud type, NoeudAST *gauche, NoeudAST *droite) {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = type;
    noeud->contenu.op.gauche = gauche;
    noeud->contenu.op.droite = droite;
    return noeud;
}

NoeudAST *creerNoeudIf(NoeudAST *cond, NoeudAST *alors, NoeudAST *sinon) {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = NOEUD_IF;
    noeud->contenu.if_noeud.cond = cond;
    noeud->contenu.if_noeud.alors = alors;
    noeud->contenu.if_noeud.sinon = sinon;
    return noeud;
}

NoeudAST *creerNoeudWhile(NoeudAST *cond, NoeudAST *corps) {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = NOEUD_WHILE;
    noeud->contenu.while_noeud.cond = cond;
    noeud->contenu.while_noeud.corps = corps;
    return noeud;
}

NoeudAST *creerNoeudAssign(char *nom, NoeudAST *valeur) {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = NOEUD_ASSIGN;
    noeud->contenu.op.gauche = creerNoeudVariable(nom);
    noeud->contenu.op.droite = valeur;
    return noeud;
}

NoeudAST *creerNoeudSkip() {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = NOEUD_SKIP;
    return noeud;
}

NoeudAST *creerNoeudSeq(NoeudAST *premiere, NoeudAST *deuxieme) {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = NOEUD_SEQ;
    noeud->contenu.seq.premiere = premiere;
    noeud->contenu.seq.deuxieme = deuxieme;
    return noeud;
}

NoeudAST *creerNoeudBloc(NoeudAST *commandes) {
    NoeudAST *noeud = (NoeudAST *)malloc(sizeof(NoeudAST));
    noeud->type = NOEUD_BLOC;
    noeud->contenu.op.gauche = commandes;
    return noeud;
}

TypeNoeud obtenirTypeNoeud(NoeudAST *noeud) {
    return noeud->type;
}

NoeudAST *obtenirFilsGauche(NoeudAST *noeud) {
    return noeud->contenu.op.gauche;
}

NoeudAST *obtenirFilsDroit(NoeudAST *noeud) {
    return noeud->contenu.op.droite;
}

char *obtenirNomVariable(NoeudAST *noeud) {
    return noeud->contenu.nom;
}

int obtenirValeurNombre(NoeudAST *noeud) {
    return noeud->contenu.valeur;
}

VariableList *creerListeVariables() {
    VariableList *liste = (VariableList *)malloc(sizeof(VariableList));
    liste->tete = NULL;
    return liste;
}

bool ajouterVariable(VariableList *liste, char *nom) {
    NoeudVariable *courant = liste->tete;
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {
            return false; // car elle existe déjà 
        }
        courant = courant->suivant;
    }

    NoeudVariable *nouvelleVariable = (NoeudVariable *)malloc(sizeof(NoeudVariable));
    nouvelleVariable->nom = strdup(nom);
    nouvelleVariable->valeur = 0; // on initialise à 0
    nouvelleVariable->suivant = liste->tete;
    liste->tete = nouvelleVariable;
    return true;
}

void afficherListeVariables(VariableList *liste) {
    NoeudVariable *courant = liste->tete;
    while (courant != NULL) {
        printf("%s = %d\n", courant->nom, courant->valeur);
        courant = courant->suivant;
    }
}

void libererListeVariables(VariableList *liste) {
    NoeudVariable *courant = liste->tete;
    while (courant != NULL) {
        NoeudVariable *suivant = courant->suivant;
        free(courant->nom);
        free(courant);
        courant = suivant;
    }
    free(liste);
}

int obtenirValeurVariable(VariableList *liste, char *nom) {
    NoeudVariable *courant = liste->tete;
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {
            return courant->valeur;
        }
        courant = courant->suivant;
    }
    fprintf(stderr, "Erreur : Variable %s non trouvée\n", nom);
    return 0;
}

void mettreAJourValeurVariable(VariableList *liste, char *nom, int valeur) {
    NoeudVariable *courant = liste->tete;
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {
            courant->valeur = valeur;
            return;
        }
        courant = courant->suivant;
    }
    fprintf(stderr, "Erreur : Variable %s non trouvée\n", nom);
}

void afficherExpression(NoeudAST *noeud) {
    if (noeud == NULL) return;
    
    switch (noeud->type) {
        case NOEUD_NOMBRE:
            printf("%d", noeud->contenu.valeur);
            break;
        case NOEUD_VARIABLE:
            printf("%s", noeud->contenu.nom);
            break;
        case NOEUD_OP_ADD:
            printf("(");
            afficherExpression(noeud->contenu.op.gauche);
            printf(" + ");
            afficherExpression(noeud->contenu.op.droite);
            printf(")");
            break;
        case NOEUD_OP_SUB:
            printf("(");
            afficherExpression(noeud->contenu.op.gauche);
            printf(" - ");
            afficherExpression(noeud->contenu.op.droite);
            printf(")");
            break;
        case NOEUD_OP_MUL:
            printf("(");
            afficherExpression(noeud->contenu.op.gauche);
            printf(" * ");
            afficherExpression(noeud->contenu.op.droite);
            printf(")");
            break;
        case NOEUD_OP_DIV:
            printf("(");
            afficherExpression(noeud->contenu.op.gauche);
            printf(" / ");
            afficherExpression(noeud->contenu.op.droite);
            printf(")");
            break;
        default:
            printf("Erreur: Type de nœud non pris en charge\n");
    }
}

void afficherBExpression(NoeudAST *noeud) {
    if (noeud == NULL) return;
    
    switch (noeud->type) {
        case NOEUD_BOOL_EQ:
            printf("(");
            afficherExpression(obtenirFilsGauche(noeud));
            printf(" == ");
            afficherExpression(obtenirFilsDroit(noeud));
            printf(")");
            break;
        case NOEUD_BOOL_LT:
            printf("(");
            afficherExpression(obtenirFilsGauche(noeud));
            printf(" < ");
            afficherExpression(obtenirFilsDroit(noeud));
            printf(")");
            break;
        case NOEUD_BOOL_GT:
            printf("(");
            afficherExpression(obtenirFilsGauche(noeud));
            printf(" > ");
            afficherExpression(obtenirFilsDroit(noeud));
            printf(")");
            break;
        case NOEUD_BOOL_LE:
            printf("(");
            afficherExpression(obtenirFilsGauche(noeud));
            printf(" <= ");
            afficherExpression(obtenirFilsDroit(noeud));
            printf(")");
            break;
        case NOEUD_BOOL_GE:
            printf("(");
            afficherExpression(obtenirFilsGauche(noeud));
            printf(" >= ");
            afficherExpression(obtenirFilsDroit(noeud));
            printf(")");
            break;
        case NOEUD_BOOL_ET:
            printf("(");
            afficherBExpression(obtenirFilsGauche(noeud));
            printf(" and ");
            afficherBExpression(obtenirFilsDroit(noeud));
            printf(")");
            break;
        case NOEUD_BOOL_OU:
            printf("(");
            afficherBExpression(obtenirFilsGauche(noeud));
            printf(" or ");
            afficherBExpression(obtenirFilsDroit(noeud));
            printf(")");
            break;
        case NOEUD_BOOL_NON:
            printf("(not ");
            afficherBExpression(obtenirFilsGauche(noeud));
            printf(")");
            break;
        default:
            printf("Erreur: Type de nœud booléen non pris en charge\n");
    }
}

void afficherCommande(NoeudAST *noeud) {
    if (noeud == NULL) return;

    switch (noeud->type) {
        case NOEUD_SKIP:
            printf("skip");
            break;
        case NOEUD_ASSIGN:
            printf("%s := ", obtenirNomVariable(noeud->contenu.op.gauche));
            afficherExpression(noeud->contenu.op.droite);
            break;
        case NOEUD_IF:
            printf("if ");
            afficherBExpression(noeud->contenu.if_noeud.cond);
            printf(" then\n");
            afficherCommande(noeud->contenu.if_noeud.alors);
            printf(" else\n");
            afficherCommande(noeud->contenu.if_noeud.sinon);
            printf(" end");
            break;
        case NOEUD_WHILE:
            printf("while ");
            afficherBExpression(noeud->contenu.while_noeud.cond);
            printf(" do\n");
            afficherCommande(noeud->contenu.while_noeud.corps);
            printf(" end");
            break;
        case NOEUD_SEQ:
            afficherCommande(noeud->contenu.seq.premiere);
            printf(" ;\n");
            afficherCommande(noeud->contenu.seq.deuxieme);
            break;
        case NOEUD_BLOC:
            printf("{\n");
            afficherCommande(noeud->contenu.op.gauche);
            printf("}\n");
            break;
        default:
            printf("Erreur: Type de commande non pris en charge\n");
    }
}

int evaluerExpression(NoeudAST *noeud, VariableList *liste) {
    if (noeud == NULL) return 0;

    switch (noeud->type) {
        case NOEUD_NOMBRE:
            return noeud->contenu.valeur;
        case NOEUD_VARIABLE:
            return obtenirValeurVariable(liste, noeud->contenu.nom);
        case NOEUD_OP_ADD:
            return evaluerExpression(noeud->contenu.op.gauche, liste) + evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_OP_SUB:
            return evaluerExpression(noeud->contenu.op.gauche, liste) - evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_OP_MUL:
            return evaluerExpression(noeud->contenu.op.gauche, liste) * evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_OP_DIV:
            return evaluerExpression(noeud->contenu.op.gauche, liste) / evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_BOOL_EQ:
            return evaluerExpression(noeud->contenu.op.gauche, liste) == evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_BOOL_LT:
            return evaluerExpression(noeud->contenu.op.gauche, liste) < evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_BOOL_GT:
            return evaluerExpression(noeud->contenu.op.gauche, liste) > evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_BOOL_LE:
            return evaluerExpression(noeud->contenu.op.gauche, liste) <= evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_BOOL_GE:
            return evaluerExpression(noeud->contenu.op.gauche, liste) >= evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_BOOL_ET:
            return evaluerExpression(noeud->contenu.op.gauche, liste) && evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_BOOL_OU:
            return evaluerExpression(noeud->contenu.op.gauche, liste) || evaluerExpression(noeud->contenu.op.droite, liste);
        case NOEUD_BOOL_NON:
            return !evaluerExpression(noeud->contenu.op.gauche, liste);
        default:
            printf("Erreur: Type de nœud non pris en charge pour l'évaluation\n");
            return 0;
    }
}

void evaluerCommande(NoeudAST *noeud, VariableList *liste) {
    if (noeud == NULL) return;

    switch (noeud->type) {
        case NOEUD_SKIP:
            break;
        case NOEUD_ASSIGN: {
            char *nomVar = obtenirNomVariable(noeud->contenu.op.gauche);
            int valeur = evaluerExpression(noeud->contenu.op.droite, liste);
            mettreAJourValeurVariable(liste, nomVar, valeur);
            break;
        }
        case NOEUD_IF:
            if (evaluerExpression(noeud->contenu.if_noeud.cond, liste)) {
                evaluerCommande(noeud->contenu.if_noeud.alors, liste);
            } else {
                evaluerCommande(noeud->contenu.if_noeud.sinon, liste);
            }
            break;
        case NOEUD_WHILE:
            while (evaluerExpression(noeud->contenu.while_noeud.cond, liste)) {
                evaluerCommande(noeud->contenu.while_noeud.corps, liste);
            }
            break;
        case NOEUD_SEQ:
            evaluerCommande(noeud->contenu.seq.premiere, liste);
            evaluerCommande(noeud->contenu.seq.deuxieme, liste);
            break;
        case NOEUD_BLOC:
            evaluerCommande(noeud->contenu.op.gauche, liste);
            break;
        default:
            printf("Erreur: Type de commande non pris en charge pour l'évaluation\n");
    }
}
