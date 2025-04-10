// requin.cpp
//
// // implementation des fonctions pour la gestion des requins dans l'oc�an
//
#include "main.h"

int initialise_requin(t_liste_requin** tete_requin, t_ocean* ocean, int quantite) {
    // V�rification des entr�es
    if (quantite < 0) return 0;
    if (quantite == 0) return 1;  // Rien � faire

    printf("D�but initialisation de %d requins...\n", quantite);

    t_animal requin;
    int requins_places = 0;

    for (int i = 0; i < quantite; i++) {
        // Initialisation des propri�t�s du requin
        requin.age = alea(0, MAX_AGE_REQUIN);
        requin.energie_sante = alea(1, ENERGIE_DIGESTION*2);
        requin.jrs_gest = (requin.age >= NB_JRS_PUB_REQUIN) ? (alea(1,NB_JRS_GEST_REQUIN)) : 0;

        // Recherche d'une position vide
        int essais = 0;
        int position_trouvee = 0;

        while (essais < MAX_ESSAIS && !position_trouvee) {
            requin.posx = alea(0, LARGEUR - 1);
            requin.posy = alea(0, HAUTEUR - 1);

            if ((*ocean)[requin.posy][requin.posx].contenu == VIDE) {
                position_trouvee = 1;
            }
            essais++;
        }

        // Si pas de position vide trouv�e
        if (!position_trouvee) {
            printf("Attention : seulement %d/%d requins plac�s\n", requins_places, quantite);
            return (requins_places > 0) ? 1 : 0;
        }

        // Placement du requin
        (*ocean)[requin.posy][requin.posx].contenu = REQUIN;
        t_liste_requin* nouveau = insererEnTete(tete_requin, requin);
        if (nouveau == NULL) {
            printf("Erreur m�moire !\n");
            return (requins_places > 0) ? 1 : 0;
        }

        (*ocean)[requin.posy][requin.posx].animal = nouveau;
        requins_places++;
    }

    printf("%d requins initialis�s avec succ�s\n", requins_places);
    return 1;
}

int deplacer_requin_1_case(t_noeud* requin, t_ocean *ocean) {
    int posx_temp = requin->animal.posx;
    int posy_temp = requin->animal.posy;
    int nouvelle_posx, nouvelle_posy;
    t_direction dir;
    int tentatives = 0;
    const int MAX_TENTATIVES = 8;

    do {
        // Choix al�atoire d'une case voisine
        dir = choix_aleatoire_case_voisine_libre(posx_temp, posy_temp, ocean, &nouvelle_posx, &nouvelle_posy);
        tentatives++;

        // Gestion du "wrap-around" horizontal
        if (nouvelle_posx < 0) nouvelle_posx = LARGEUR - 1;
        else if (nouvelle_posx >= LARGEUR) nouvelle_posx = 0;

        // V�rifier si la case est valide et vide
        if (nouvelle_posy >= 0 && nouvelle_posy < HAUTEUR &&
            (*ocean)[nouvelle_posy][nouvelle_posx].contenu == VIDE) {
            break;
        }

        // Condition de sortie si trop de tentatives
        if (tentatives >= MAX_TENTATIVES) {
            return 0; // �chec du d�placement
        }

    } while (1);

    // Mise � jour de la grille
    if (!inserer_contenu_pointeur_case_grille(nouvelle_posx, nouvelle_posy, ocean, REQUIN, &(requin->animal))) {
        return 0;
    }

    // Mise � jour des coordonn�es
    requin->animal.posx = nouvelle_posx;
    requin->animal.posy = nouvelle_posy;

    // Nettoyage ancienne position
    if (!effacer_contenu_case_grille(posx_temp, posy_temp, ocean)) {
        // En cas d'�chec, annuler le d�placement
        effacer_contenu_case_grille(nouvelle_posx, nouvelle_posy, ocean);
        requin->animal.posx = posx_temp;
        requin->animal.posy = posy_temp;
        return 0;
    }

    return 1; // Succ�s
}

int compter_requins(t_liste_requin* liste) {
    int nb_requin = 0;
    t_liste_requin* courant = liste;

    // Parcours de toute la liste
    while (courant != NULL) {
        nb_requin++;
        courant = courant->suivant;
    }

    return nb_requin;
}

int nouveau_requin(t_liste_requin** liste, t_animal* parent, t_ocean* ocean) {
    // Recherche d'une case voisine libre
    int new_x, new_y;
    if (choix_aleatoire_case_voisine_libre(parent->posx, parent->posy, ocean, &new_x, &new_y) == -1) {
        return 0; // Aucune case libre
    }

    // Cr�ation du b�b�-requin
    t_animal bebe = {
            .posx = new_x,
            .posy = new_y,
            .age = 0,
            .energie_sante = ENERGIE_DIGESTION, // �nergie sp�cifique au requin
            .jrs_gest = 0
    };

    // Ajout du b�b� � la liste des requins
    t_liste_requin* nouveau = insererEnTete(liste, bebe);
    if (!nouveau ||
        !inserer_contenu_pointeur_case_grille(new_x, new_y, ocean, REQUIN, &(nouveau->animal))) {
        if (nouveau) supprimerAnimal((t_noeud**)liste, (t_noeud*)nouveau);
        return 0;
    }

    // Mise � jour du parent apr�s reproduction
    reset_gestation(parent, -NB_JRS_GEST_REQUIN); // R�initialisation de la gestation requin
    dec_energie(parent); // Le parent perd de l'�nergie apr�s reproduction

    return 1;
}


int mort_requin(t_liste_requin** liste, t_ocean* ocean) {
    // V�rification des param�tres
    if (liste == NULL || *liste == NULL || ocean == NULL) {
        fprintf(stderr, "Param�tres invalides\n");
        return 0;
    }

    t_liste* courant = *liste;

    // R�cup�ration des coordonn�es du requin
    int posx = courant->animal.posx;
    int posy = courant->animal.posy;

    // V�rification des coordonn�es
    if (posx < 0 || posx >= LARGEUR || posy < 0 || posy >= HAUTEUR) {
        fprintf(stderr, "Coordonn�es invalides (%d,%d)\n", posx, posy);
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

