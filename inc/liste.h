#ifndef LISTE_H
#define LISTE_H

#include "main.h"


#include <stdio.h>
#include <stdlib.h>

#define MAX 100  // Taille maximale de la liste

// D�finition de la structure de la liste statique
/**
 * @brief structure repr�sentant un noeud de la liste.
 * @param animal t_animal repr�sentant un objet poisson ou un requin.
 * @param precedent Pointeur vers le noeud pr�c�dent.
 * @param suivant Pointeur vers le noeud suivant.
 */
typedef struct t_noeud {
    t_animal animal; // Tableau contenant les �l�ments
    struct t_noeud* precedent;    // Pointeur vers le noeud pr�c�dent
    struct t_noeud* suivant;        // Nombre actuel d'�l�ments dans la liste
} t_liste;

// Fonctions pour g�rer la liste statique
t_noeud* creer_noeud(t_animal animal);
t_noeud* insererEnTete(t_liste** liste, t_animal animal);
void supprimerEnTete(t_liste** liste);
void supprimerAnimal(t_noeud** tete ,t_noeud* animal_to_kill);
void libererListe(t_liste* liste);
void afficherListe(t_liste* liste);
t_noeud* insererEnFin(t_liste** liste, t_animal animal);




#endif // LISTE_H