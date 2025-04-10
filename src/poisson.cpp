// poisson.cpp
//
// // Impl�mentation des fonctions pour la gestion des poissons dans l'oc�an
//
#include "main.h"

int initialise_poisson(t_liste_poisson** tete_poisson, t_ocean* ocean, int quantite) {
    // V�rification des entr�es
    if ((quantite < 0)||(quantite > MAX_POISSON)) return 0;
    if (quantite == 0) return 1;  // Rien � faire
#ifdef DEBUG
    printf("D�but initialisation de %d poissons...\n", quantite);
#endif
    t_animal poisson;
    int poissons_places = 0;

    for (int i = 0; i < quantite; i++)
    {
        // Initialisation des propri�t�s du poisson
        poisson.age = alea(0, MAX_AGE_POISSON);
        poisson.energie_sante = alea(1, ENERGIE_INIT_POISSON);
        poisson.jrs_gest = 0;

        // Recherche d'une position vide
        int essais = 0;
        int position_trouvee = 0;

        while (essais < MAX_ESSAIS && !position_trouvee)
        {
            poisson.posx = alea(0, LARGEUR - 1);
            poisson.posy = alea(0, HAUTEUR - 1);

            if ((*ocean)[poisson.posy][poisson.posx].contenu == VIDE)
            {
                position_trouvee = 1;
            }
            essais++;
        }

        // Si pas de position vide trouv�e
        if (!position_trouvee)
        {
            #ifdef DEBUG
            printf("Attention : seulement %d/%d poissons plac�s\n", poissons_places, quantite);
            #endif
            return (poissons_places > 0) ? 1 : 0;
        }

        // Placement du poisson
        (*ocean)[poisson.posy][poisson.posx].contenu = POISSON;
        t_liste_poisson* nouveau = insererEnTete(tete_poisson, poisson);
        if (nouveau == NULL)
        {
            #ifdef DEBUG
            printf("Erreur m�moire !\n");
            #endif
            return (poissons_places > 0) ? 1 : 0;
        }

        (*ocean)[poisson.posy][poisson.posx].animal = nouveau;
        poissons_places++;
    }
    #ifdef DEBUG
    printf("%d poissons initialis�s avec succ�s\n", poissons_places);
    #endif
    return 1;
}

int deplacer_poisson_1_case(t_noeud* poisson, t_ocean* ocean) {
    int posx_temp = poisson->animal.posx;
    int posy_temp = poisson->animal.posy;
    int nouvelle_posx, nouvelle_posy;
    t_direction dir;
    int tentatives = 0;
    const int MAX_TENTATIVES = 8;
    do
    {
        // Choix al�atoire d'une case voisine
        dir = choix_aleatoire_case_voisine_libre(posx_temp, posy_temp, ocean, &nouvelle_posx, &nouvelle_posy);
        tentatives++;

        // Gestion du "wrap-around" horizontal
        if (nouvelle_posx < 0)
            nouvelle_posx = LARGEUR - 1;
        else if (nouvelle_posx >= LARGEUR)
            nouvelle_posx = 0;

        // V�rifier si la case est valide et vide
        if (nouvelle_posy >= 0 && nouvelle_posy < HAUTEUR &&
            (*ocean)[nouvelle_posy][nouvelle_posx].contenu == VIDE)
        {
            break;
        }

        // Condition de sortie si trop de tentatives
        if (tentatives >= MAX_TENTATIVES)
        {
            return 0; // �chec
        }

    } while (1);

    // Mise � jour de la grille
    if (!inserer_contenu_pointeur_case_grille(nouvelle_posx, nouvelle_posy, ocean, POISSON, &(poisson->animal)))
    {
        return 0;
    }

    // Mise � jour des coordonn�es
    poisson->animal.posx = nouvelle_posx;
    poisson->animal.posy = nouvelle_posy;

    // Nettoyage ancienne position
    if (!effacer_contenu_case_grille(posx_temp, posy_temp, ocean))
    {
        // Si d'�chec, annuler d�placement
        effacer_contenu_case_grille(nouvelle_posx, nouvelle_posy, ocean);
        poisson->animal.posx = posx_temp;
        poisson->animal.posy = posy_temp;
        return 0;
    }

    return 1;
}

int compter_poissons(t_liste_poisson* liste) {
    int nb_poisson = 0;
    t_liste_requin* courant = liste;

    // Parcours de toute la liste
    while (courant != NULL)
    {
        nb_poisson++;
        courant = courant->suivant;
    }

    return nb_poisson;
}

int nouveau_poisson(t_liste_poisson** liste, t_animal* parent, t_ocean* ocean) {
    // V�rification capacit� maximale
    if (compter_poissons(*liste) >= MAX_POISSON)
    {
        reset_gestation(parent, -NB_JRS_GEST_POISSON);
        return 0;
    }
    // Recherche case libre
    int new_x, new_y;
    if (choix_aleatoire_case_voisine_libre(parent->posx, parent->posy, ocean, &new_x, &new_y) == -1)
    {
        reset_gestation(parent, -NB_JRS_GEST_POISSON);
        return 0;
    }

    // Probabilit� 33% de fausse couche
    if (alea(1, 3) == 1)
    {
        reset_gestation(parent, -NB_JRS_GEST_POISSON);
        return 0;
    }

    // Cr�ation b�b�
    t_animal bebe = {
            .posx = new_x,
            .posy = new_y,
            .age = 0,
            .energie_sante = ENERGIE_INIT_POISSON,
            .jrs_gest = 0
    };

    // Ajout � la liste
    t_liste_poisson* nouveau = insererEnTete(liste, bebe);
    if (!nouveau ||
        !inserer_contenu_pointeur_case_grille(new_x, new_y, ocean, POISSON, &(nouveau->animal)))
    {
        if (nouveau) supprimerAnimal((t_noeud**)liste, (t_noeud*)nouveau);
        return 0;
    }

    // Mise � jour parent
    reset_gestation(parent, -NB_JRS_GEST_POISSON);
    dec_energie(parent);

    return 1;
}

int mort_poisson(t_liste_poisson** liste, t_ocean* ocean) {
    // V�rification des param�tres
    if (liste == NULL || *liste == NULL || ocean == NULL)
    {
        #ifdef DEBUG
        fprintf(stderr, "Param�tres invalides\n");
        #endif
        return 0;
    }

    t_liste* courant = *liste;

    // R�cup�ration des coordonn�es du poisson
    int posx = courant->animal.posx;
    int posy = courant->animal.posy;

    // V�rification des coordonn�es
    if (posx < 0 || posx >= LARGEUR || posy < 0 || posy >= HAUTEUR)
    {
        #ifdef DEBUG
        fprintf(stderr, "Coordonn�es invalides (%d,%d)\n", posx, posy);
        #endif
        return 0;
    }

    // Suppression de la t�te
    *liste = courant->suivant;

    // Nettoyage de la case dans l'oc�an
    (*ocean)[posy][posx].contenu = VIDE;
    (*ocean)[posy][posx].animal = NULL;

    // Lib�ration m�moire
    free(courant);

    return 1;
}
