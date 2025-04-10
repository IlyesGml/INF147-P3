#include "main.h"
#include "liste.h"

// Initialisation de la liste vide
/**
 * @brief cr�er une liste avec le premier animal.
 *
 * @param animal Poisson ou requin � ajouter � la premi�re position de la liste.
 *
 * @return t_noeud*
 */
t_noeud* creer_noeud(t_animal animal)
{
    t_noeud* nouveau_noeud = (t_noeud*)malloc(sizeof(t_noeud));
    if (nouveau_noeud == NULL)
    {
        printf("Erreur d'allocation de m�moire pour le noeud.\n");
        return NULL;
    }
    nouveau_noeud->animal = animal;
    nouveau_noeud->precedent = NULL;
    nouveau_noeud->suivant = NULL;
    return nouveau_noeud;
}
/**
 * @brief Ajouter un poisson ou requin � la t�te de la liste.
 *
 * @param liste t_liste_requins ou t_liste_poissons sont la destination du nouveau poisson ou requin.
 * @param animal le poisson ou requin � ajouter � la liste dans t_liste.
 */
t_noeud* insererEnTete(t_liste** tete, t_animal animal)
{
    t_noeud* nouveau_noeud = creer_noeud(animal);
    if (nouveau_noeud == NULL)
    {
#ifdef DEBUG
        printf("Erreur d'allocation de m�moire pour le noeud.\n");
#endif
        return NULL;
    }
    if (*tete == NULL)
    {
        *tete = nouveau_noeud;
#ifdef DEBUG
        printf("Liste vide, Tete: %p\n", tete);
#endif
        return nouveau_noeud;
    }
    else
    {
        nouveau_noeud->suivant = *tete;
        (*tete)->precedent = nouveau_noeud;
        *tete = nouveau_noeud;
    }
    return nouveau_noeud;
}

void supprimerEnTete(t_liste** liste)
{
    if (*liste == NULL)
    {
#ifdef DEBUG
        printf("Erreur : la liste est vide.\n");
#endif
        return;
    }
    t_noeud* noeud_a_supprimer = *liste;
    if ((*liste)->suivant == NULL)
    {
        free(noeud_a_supprimer);
        *liste = NULL;
    }
    else
    {
        *liste = (*liste)->suivant;
        (*liste)->precedent = NULL;
        free(noeud_a_supprimer);
    }
}

void supprimerAnimal(t_noeud** tete, t_noeud* animal_to_kill)
{
    if (animal_to_kill == NULL || *tete == NULL)
    {
#ifdef DEBUG
        printf("Erreur : le noeud � supprimer est NULL.\n");
#endif
        return;
    }
    if ((*tete) == animal_to_kill)
    {
#ifdef DEBUG
        printf("Noeud � supprimer est le premier de la liste.\n");
#endif
        (*tete) = animal_to_kill->suivant;
        animal_to_kill->suivant->precedent = NULL;
        free(animal_to_kill);
        return;
    }
    if (animal_to_kill->suivant == NULL)
    {
#ifdef DEBUG
        printf("Noeud � supprimer est le dernier dans la liste.\n");
#endif
        t_noeud* pre_noeud = animal_to_kill->precedent;
        pre_noeud->suivant = NULL;
    }
    else
    {
        t_noeud* pre_noeud = animal_to_kill->precedent;
        t_noeud* next_noeud = animal_to_kill->suivant;
        pre_noeud->suivant = animal_to_kill->suivant;
        next_noeud->precedent = animal_to_kill->precedent;
    }


    free(animal_to_kill);
}

void libererListe(t_liste* liste)
{
    t_noeud* courant;
    while (liste != NULL)
    {
        courant = liste;
        liste = liste->suivant;
        free(courant);
    }
}

void afficherListe(t_liste* liste)
{
    t_noeud* courant = liste;
    for (int i = 0; courant != NULL; i++)
    {
        printf("Animal %d : Age = %d, Energie = %d, Position = (%d, %d)\n", i,
            courant->animal.age, courant->animal.energie_sante,
            courant->animal.posx, courant->animal.posy);
        courant = courant->suivant;
    }
    printf("\n");
}

t_noeud* insererEnFin(t_liste** liste, t_animal animal)
{
    t_liste* nouveau_noeud = creer_noeud(animal);
    if (nouveau_noeud == NULL)
    {
#ifdef DEBUG
        printf("Erreur d'allocation de memoire pour le noeud.\n");
#endif
        return NULL;
    }

    // Cas où la liste est vide
    if (*liste == NULL)
    {
        *liste = nouveau_noeud;
        return nouveau_noeud;
    }

    // Parcours jusqu'au dernier élément
    t_liste* courant = *liste;
    while (courant->suivant != NULL)
    {
        courant = courant->suivant;
    }
    // Ajout du nouveau nœud à la fin
    courant->suivant = nouveau_noeud;
    nouveau_noeud->precedent = courant;
    return nouveau_noeud;
}

t_noeud* obtenir_prochain(t_liste* liste)
{
    if (liste->suivant == NULL)
    {
        return NULL;
    }
    return liste->suivant;
}

