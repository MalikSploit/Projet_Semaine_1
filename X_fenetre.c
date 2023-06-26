#include <SDL2/SDL.h>
#include "Gestion_Erreurs.c"

#define LARGEUR_FENETRE 200  // Définition de la largeur de la fenêtre
#define HAUTEUR_FENETRE 200  // Définition de la hauteur de la fenêtre
#define NOMBRE_DE_FENETRES 30  // Nombre de fenêtres à créer


int main()
{
    // Initialisation de la SDL pour le rendu vidéo
    verifier_initialisation_SDL();

    // Récupération des dimensions de l'écran
    SDL_DisplayMode DM;
    if (SDL_GetCurrentDisplayMode(0, &DM) != 0)
    {
        fprintf(stderr, "Erreur lors de la récupération du mode d'affichage actuel : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    int largeur_ecran = DM.w;  // Largeur de l'écran
    int hauteur_ecran = DM.h;  // Hauteur de l'écran

    // Création de tableaux pour stocker les pointeurs vers les fenêtres et les moteurs de rendu
    SDL_Window* fenetres[NOMBRE_DE_FENETRES * 2];
    SDL_Renderer* moteurs_de_rendu[NOMBRE_DE_FENETRES * 2];

    // Calcul des positions des fenêtres pour former un 'X'
    for (int i = 0; i < NOMBRE_DE_FENETRES; i++)
    {
        // Positions pour la ligne diagonale du 'X'
        int position_x_1 = (largeur_ecran - LARGEUR_FENETRE) * i / (NOMBRE_DE_FENETRES - 1);
        int position_y_1 = (hauteur_ecran - HAUTEUR_FENETRE) * i / (NOMBRE_DE_FENETRES - 1);

        // Création de la fenêtre
        fenetres[i] = creer_fenetre(position_x_1, position_y_1, LARGEUR_FENETRE, HAUTEUR_FENETRE);
        // Création du moteur de rendu pour cette fenêtre
        moteurs_de_rendu[i] = creer_moteur_de_rendu(fenetres[i], -1, 0);
        // Définition de la couleur de rendu (noir dans ce cas)
        SDL_SetRenderDrawColor(moteurs_de_rendu[i], 0, 0, 0, 255);
        // Nettoyage de l'écran actuel avec la couleur de rendu
        SDL_RenderClear(moteurs_de_rendu[i]);
        // Présentation de ce qui a été rendu à l'écran
        SDL_RenderPresent(moteurs_de_rendu[i]);

        // Positions pour la ligne '\' du 'X'
        int position_x_2 = (largeur_ecran - LARGEUR_FENETRE) * (NOMBRE_DE_FENETRES - 1 - i) / (NOMBRE_DE_FENETRES - 1);
        int position_y_2 = (hauteur_ecran - HAUTEUR_FENETRE) * i / (NOMBRE_DE_FENETRES - 1);
        // Création de la fenêtre
        fenetres[i + NOMBRE_DE_FENETRES] = SDL_CreateWindow("Fenêtre", position_x_2, position_y_2, LARGEUR_FENETRE, HAUTEUR_FENETRE, 0);
        // Création du moteur de rendu pour cette fenêtre
        moteurs_de_rendu[i + NOMBRE_DE_FENETRES] = SDL_CreateRenderer(fenetres[i + NOMBRE_DE_FENETRES], -1, 0);
        // Définition de la couleur de rendu (noir dans ce cas)
        SDL_SetRenderDrawColor(moteurs_de_rendu[i + NOMBRE_DE_FENETRES], 0, 0, 0, 255);
        // Nettoyage de l'écran actuel avec la couleur de rendu
        SDL_RenderClear(moteurs_de_rendu[i + NOMBRE_DE_FENETRES]);
        // Présentation de ce qui a été rendu à l'écran
        SDL_RenderPresent(moteurs_de_rendu[i + NOMBRE_DE_FENETRES]);
    }

    // Attendre 5 secondes avant de fermer les fenêtres
    SDL_Delay(5000);

    // Fermer et détruire les fenêtres et les moteurs de rendu
    for (int i = 0; i < NOMBRE_DE_FENETRES * 2; i++)
    {
        SDL_DestroyRenderer(moteurs_de_rendu[i]);
        SDL_DestroyWindow(fenetres[i]);
    }

    // Arrêt de la SDL
    SDL_Quit();

    return 0;
}
