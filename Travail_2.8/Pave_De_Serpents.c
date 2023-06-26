#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> // Inclusion de la bibliothèque SDL2_gfx pour dessiner des formes primitives
#include "../Gestion_Erreurs.c"
#include <math.h>


// Définition des constantes pour la taille de la fenêtre
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600

// Définition des constantes pour la rosace
#define NOMBRE_PETALES 8
#define RAYON_ROSACE 200
#define RAYON_ORBITE 100

int main()
{
    verifier_initialisation_SDL(); // Initialisation de la SDL pour l'affichage vidéo
    SDL_Window* fenetre = creer_fenetre(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR_FENETRE, HAUTEUR_FENETRE); // Création de la fenêtre
    SDL_Renderer* renderer = creer_moteur_de_rendu(fenetre, -1, SDL_RENDERER_ACCELERATED); // Création du moteur de rendu

    int enExecution = 1; // Variable pour contrôler la boucle principale du programme
    SDL_Event evenement; // Structure pour gérer les événements SDL
    double rotation = 0; // Variable pour contrôler la rotation de la rosace

    // Boucle principale du programme
    while (enExecution)
    {
        // Traitement des événements SDL
        while (SDL_PollEvent(&evenement))
        {
            // Si l'utilisateur ferme la fenêtre, on sort de la boucle principale
            if (evenement.type == SDL_QUIT)
            {
                enExecution = 0;
            }
        }

        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // On met la couleur noire
        SDL_RenderClear(renderer); // Effacer l'écran avec la couleur choisie

        // Calcul des coordonnées du centre de la rosace
        double centreX = LARGEUR_FENETRE / 2 + RAYON_ORBITE * cos(rotation);
        double centreY = HAUTEUR_FENETRE / 2 + RAYON_ORBITE * sin(rotation);

        // Boucle pour dessiner la rosace
        for (int i = 0; i < 360; ++i)
        {
            // Calcul des coordonnées d'un point sur la rosace
            double angle1 = i * M_PI / 180;
            double x1 = centreX + RAYON_ROSACE * cos(NOMBRE_PETALES * angle1) * cos(angle1);
            double y1 = centreY + RAYON_ROSACE * cos(NOMBRE_PETALES * angle1) * sin(angle1);

            // Calcul des coordonnées du point suivant sur la rosace
            double angle2 = (i + 1) * M_PI / 180;
            double x2 = centreX + RAYON_ROSACE * cos(NOMBRE_PETALES * angle2) * cos(angle2);
            double y2 = centreY + RAYON_ROSACE * cos(NOMBRE_PETALES * angle2) * sin(angle2);

            // Calcul de la couleur du pétale en fonction de l'angle
            int rouge = (int)(255 * (cos(angle1) + 1) / 2);
            int vert = (int)(255 * (sin(angle1) + 1) / 2);
            int bleu = 255 - rouge;

            // Création d'un tableau contenant les coordonnées des sommets du pétale
            Sint16 vx[3] = { (Sint16)centreX, (Sint16)x1, (Sint16)x2 };
            Sint16 vy[3] = { (Sint16)centreY, (Sint16)y1, (Sint16)y2 };
            // Dessin de la pétale
            filledPolygonRGBA(renderer, vx, vy, 3, rouge, vert, bleu, 255);
        }

        // Dessin du cercle qui entoure la rosace
        circleRGBA(renderer, centreX, centreY, RAYON_ROSACE, 255, 255, 255, 255);

        // Affichage du rendu à l'écran
        SDL_RenderPresent(renderer);

        // Attente pour maintenir une cadence d'environ 60 images par seconde (60 FPS)
        SDL_Delay(1000 / 60);

        // Augmentation de l'angle de rotation pour la prochaine itération
        rotation += 0.04; // Plus c'est grand plus c'est rapide :)

        // Si la rotation dépasse un tour complet, on la réinitialise
        if (rotation > 2 * M_PI)
        {
            rotation -= 2 * M_PI;
        }
    }

    // Nettoyage des ressources SDL
    SDL_DestroyRenderer(renderer); // Destruction du renderer
    SDL_DestroyWindow(fenetre); // Destruction de la fenêtre
    SDL_Quit(); // Arrêt de la SDL

    return 0;
}
