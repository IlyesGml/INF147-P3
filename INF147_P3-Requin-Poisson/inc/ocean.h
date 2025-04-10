//
// Created by afifa on 2025-04-04.
//

#ifndef INF147LAB3_OCEAN_H
#define INF147LAB3_OCEAN_H

#define HAUTEUR   60
#define LARGEUR  120

/* Type �num�r� pour les 8 directions de d�placement */
typedef enum {IMPOSSIBLE=-1, HAUT, HAUT_DR, DROITE, BAS_DR, BAS, BAS_G, GAUCHE, HAUT_G} t_direction;

/* Type �num�r� pour le contenu d'une case */
typedef enum {VIDE, POISSON, REQUIN} t_contenu;

/* Type structure pour le contenu d'une case de la grille */
typedef struct {
    t_contenu contenu; //Soit: VIDE, POISSON ou REQUIN
    void *animal; //Pointeur g�n�rique � un animal � cette case, sinon NULL
} t_case;

/* Type-tableau de la grille de l'oc�an (taille : 60 X 120) */
typedef t_case t_ocean[HAUTEUR][LARGEUR];

/*-----------------------------------------------------------------------
1--     Fonction vider_ocean
-----------------------------------------------------------------------

Description :
            Cette fonction initialise toutes les cases de l'oc�an � l'�tat VIDE
            avec un pointeur animal NULL. Elle parcourt l'int�gralit� de la grille
            en deux dimensions (HAUTEUR x LARGEUR) et r�initialise chaque case.

            Le processus suit ces �tapes :
              1. Parcours de chaque ligne (de 0 � HAUTEUR-1)
              2. Pour chaque ligne, parcours de chaque colonne (de 0 � LARGEUR-1)
              3. Pour chaque case :
                  - Met le contenu � VIDE
                  - Met le pointeur animal � NULL

Param�tres d'entr�e :
            - ocean : Pointeur vers la grille de l'oc�an � vider

Param�tres de sortie :
            Aucun retour. La grille est modifi�e directement.

Remarques importantes :
            - Cette fonction doit �tre appel�e avant toute initialisation de simulation
            - Elle garantit que toutes les cases sont dans un �tat connu avant utilisation

Exemple d'utilisation :
            t_ocean mon_ocean;
            vider_ocean(&mon_ocean);

            // Toutes les cases de mon_ocean sont maintenant VIDE avec animal=NULL
*/

void vider_ocean(t_ocean *ocean);

/*-----------------------------------------------------------------------
2--     Fonction obtenir_reference_case_grille
-----------------------------------------------------------------------

Description :
            Cette fonction retourne le pointeur animal stock� dans une case sp�cifique
            de la grille, apr�s avoir v�rifi� que les coordonn�es sont valides.

            Le processus suit ces �tapes :
              1. V�rification que les coordonn�es (posx, posy) sont dans les limites
              2. Retour du pointeur animal si les coordonn�es sont valides
              3. Retour de NULL si les coordonn�es sont invalides

Param�tres d'entr�e :
            - posx : Position en x (colonne) dans la grille
            - posy : Position en y (ligne) dans la grille
            - ocean : Pointeur vers la grille de l'oc�an

Param�tres de sortie :
            - Retourne le pointeur animal si la case existe
            - Retourne NULL si les coordonn�es sont invalides

Exemple de comportement :
            - Pour posx=10, posy=20 : retourne l'animal pr�sent � ces coordonn�es
            - Pour posx=-1, posy=0 : retourne NULL (hors limites)

Remarques importantes :
            - Ne v�rifie pas si le pointeur animal est valide, seulement son existence

Exemple d'utilisation :
                t_animal* ref = (t_animal*)obtenir_reference_case_grille(10, 10, &ocean);

*/

void* obtenir_reference_case_grille(int posx, int posy, t_ocean *ocean);

/*-----------------------------------------------------------------------
3--     Fonction inserer_contenu_pointeur_case_grille
-----------------------------------------------------------------------

Description :
            Cette fonction ins�re un nouveau contenu et un pointeur animal dans une
            case sp�cifique de la grille, apr�s avoir effectu� plusieurs validations.

            Le processus suit ces �tapes :
              1. V�rification que les coordonn�es sont dans les limites
              2. Validation que le contenu est soit VIDE, POISSON ou REQUIN
              3. V�rification de la coh�rence entre le contenu et le pointeur animal
              4. Modification de la case si toutes les validations passent

Param�tres d'entr�e :
            - posx : Position en x (colonne) dans la grille
            - posy : Position en y (ligne) dans la grille
            - ocean : Pointeur vers la grille de l'oc�an
            - contenu : Nouveau contenu � ins�rer (VIDE, POISSON ou REQUIN)
            - animal : Pointeur vers l'animal � associer � cette case

Param�tres de sortie :
            - Retourne 1 si l'insertion a r�ussi
            - Retourne 0 si une validation a �chou�

Exemple de comportement :
            - Pour contenu=POISSON et animal=ptr_valide : retourne 1 (succ�s)
            - Pour contenu=VIDE et animal=ptr_valide : retourne 0 (incoh�rence)

Remarques importantes :
            - La fonction garantit l'int�grit� des donn�es avant modification
            - Ne copie pas l'animal, seulement stocke le pointeur

Exemple d'utilisation :
           // D�claration d'une variable oc�an
            t_ocean ocean;

            // Initialisation manuelle d'un requin
            t_case case_requin = {
                .contenu = REQUIN,    // Utilisation directe de l'enum
                .animal = NULL        // Pas d'allocation, on utilise NULL
            };

            // Insertion dans la grille
            if (inserer_contenu_pointeur_case_grille(15, 20, &ocean, case_requin.contenu, case_requin.animal)) {
                printf("Requin ins�r� � (15,20)\n");
            } else {
                printf("�chec d'insertion\n");
            }
*/

int inserer_contenu_pointeur_case_grille(int posx, int posy, t_ocean *ocean,
                                         t_contenu contenu, void *animal);

/*-----------------------------------------------------------------------
4--     Fonction effacer_contenu_case_grille
-----------------------------------------------------------------------

Description :
            Cette fonction r�initialise une case sp�cifique de la grille � l'�tat VIDE
            avec un pointeur animal NULL, apr�s avoir v�rifi� que les coordonn�es sont valides.

            Le processus suit ces �tapes :
              1. V�rification que les coordonn�es (posx, posy) sont dans les limites
              2. R�initialisation du contenu � VIDE
              3. Mise � NULL du pointeur animal

Param�tres d'entr�e :
            - posx : Position en x (colonne) dans la grille
            - posy : Position en y (ligne) dans la grille
            - ocean : Pointeur vers la grille de l'oc�an

Param�tres de sortie :
            - Retourne 1 si l'effacement a r�ussi
            - Retourne 0 si les coordonn�es sont invalides

Remarques importantes :
            - Garantit que la case est dans un �tat connu apr�s appel
            - Ne lib�re pas la m�moire de l'animal, seulement met le pointeur � NULL

Exemple d'utilisation :
            if (effacer_contenu_case_grille(10, 10, &ocean))
            {
                printf("Case (10,10) effac�e\n");
            } else
            {
                printf("Coordonn�es invalides\n");
            }
            // La case � (10,10) est maintenant VIDE avec animal=NULL
*/

int effacer_contenu_case_grille(int posx, int posy, t_ocean *ocean);

/*-----------------------------------------------------------------------
5--     Fonction nombre_case_voisine_libre
-----------------------------------------------------------------------

Description :
            Cette fonction compte le nombre de cases vides autour d'une position donn�e
            en v�rifiant les 8 cases adjacentes (haut, bas, gauche, droite et 4 diagonales).

            Le processus suit ces �tapes :
              1. V�rification que les coordonn�es centrales sont valides
              2. Pour chaque direction possible :
                  - Calcul des nouvelles coordonn�es
                  - V�rification qu'elles sont dans les limites
                  - Incr�mentation du compteur si la case est VIDE
              3. Retour du nombre total de cases vides trouv�es

Param�tres d'entr�e :
            - posx : Position centrale en x (colonne)
            - posy : Position centrale en y (ligne)
            - ocean : Pointeur vers la grille de l'oc�an

Param�tres de sortie :
            - Retourne le nombre de cases vides adjacentes (0 � 8)
            - Retourne -1 si les coordonn�es centrales sont invalides

Exemple de comportement :
            - Pour une case entour�e de 3 cases vides : retourne 3
            - Pour une case en bordure de grille : retourne moins de 8

Remarques importantes :
            - N'inclut pas la case centrale dans le d�compte
            - Les cases hors limites ne sont pas consid�r�es comme vides

Exemple d'utilisation :
            int cases_libres = nombre_case_voisine_libre(30, 30, &ocean);
            if (cases_libres >= 0)
            {
                printf("Nombre de cases voisines libres: %d\n", cases_libres);
            } else
            {
                printf("Position invalide\n");
            }
*/

int nombre_case_voisine_libre(int posx, int posy, t_ocean *ocean);

/*-----------------------------------------------------------------------
6--     Fonction choix_aleatoire_case_voisine_libre
-----------------------------------------------------------------------

Description :
            Cette fonction s�lectionne al�atoirement une case vide parmi les cases
            adjacentes � une position donn�e, et retourne la direction choisie ainsi
            que les nouvelles coordonn�es.

            Le processus suit ces �tapes :
              1. D�finition des d�placements possibles pour chaque direction
              2. Collecte des directions valides (case dans les limites et VIDE)
              3. Si aucune direction valide :
                  - Retourne les coordonn�es d'origine et direction 0
              4. Si directions valides :
                  - Choix al�atoire d'une direction
                  - Calcul des nouvelles coordonn�es
                  - Retour de la direction et des nouvelles coordonn�es

Param�tres d'entr�e :
            - posx : Position de d�part en x (colonne)
            - posy : Position de d�part en y (ligne)
            - ocean : Pointeur vers la grille de l'oc�an
            - newx : Pointeur pour stocker la nouvelle position x
            - newy : Pointeur pour stocker la nouvelle position y

Param�tres de sortie :
            - Retourne la direction choisie (enum t_direction)
            - Modifie newx et newy avec les nouvelles coordonn�es

Exemple de comportement :
            - Pour une case avec 3 voisins vides : choisit al�atoirement parmi eux
            - Pour une case sans voisins vides : retourne les m�mes coordonn�es

Remarques importantes :
            - Utilise la fonction alea() pour le choix al�atoire
            - Les directions sont d�finies dans l'�num�ration t_direction

            Exemple d'utilisation :
            int newx, newy;
            t_direction dir = choix_aleatoire_case_voisine_libre(25, 25, &ocean, &newx, &newy);

            if (dir != 0 || (newx != 25 && newy != 25))
            {
                printf("D�placement vers %d,%d direction %d\n", newx, newy, dir);
            } else
            {
                printf("Aucune case voisine libre\n");
            }

*/

t_direction choix_aleatoire_case_voisine_libre(int posx, int posy, t_ocean *ocean, int *newx, int *newy);

/*-----------------------------------------------------------------------
7--     Fonction dessiner_ocean
-----------------------------------------------------------------------

Description :
            Cette fonction affiche graphiquement l'�tat actuel de l'oc�an en utilisant
            les fonctions d'affichage du module utilitaire_affichage.

            Le processus suit ces �tapes :
              1. Initialisation des compteurs de poissons et requins
              2. Effacement de la zone d'affichage
              3. Pour chaque case de la grille :
                  - D�termination de la couleur en fonction du contenu
                  - Incr�mentation des compteurs si n�cessaire
                  - Affichage de la case avec la couleur appropri�e
              4. Affichage des informations (temps, nombre de proies/pr�dateurs)
              5. Pause courte pour la visualisation

Param�tres d'entr�e :
            - ocean : Pointeur vers la grille de l'oc�an � afficher
            - temps : Num�ro du cycle actuel � afficher

Param�tres de sortie :
            - Retourne toujours 1 (succ�s)

Exemple de comportement :
            - Les cases POISSON sont affich�es en bleu
            - Les cases REQUIN sont affich�es en rouge
            - Les cases VIDE sont affich�es en noir

Remarques importantes :
            - Utilise les fonctions du module utilitaire_affichage
            - La pause de 10ms permet une visualisation fluide
            - Les couleurs sont d�finies par les constantes BLUE, RED, BLACK

Exemple d'utilisation :
            for (int cycle = 0; cycle < 100; cycle++)
            {
                // Simulation du cycle...
                dessiner_ocean(&ocean, cycle);
                // Pause pour visualisation
            }
            // Affiche l'oc�an avec les poissons (bleu) et requins (rouge)

*/

int dessiner_ocean(t_ocean *ocean, int temps);

#endif //INF147LAB3_OCEAN_H