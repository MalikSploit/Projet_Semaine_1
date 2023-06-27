#include "Animation.h"
#include "../Gestion_Erreurs.c"


SDL_Window* fenetre = NULL;  // Pointeur vers la fenêtre principale du jeu
SDL_Renderer* moteurRendu = NULL;  // Pointeur vers le moteur de rendu pour dessiner à l'écran
SDL_Texture* textureOiseau = NULL;  // Texture de l'oiseau
SDL_Texture* textureTuyau = NULL;  // Texture du tuyau
SDL_Texture* textureFond = NULL;  // Texture du fond d'écran
SDL_Texture* textureTexteFin = NULL;  // Texture du texte de fin de jeu
SDL_Texture* textureNuage = NULL;  // Texture des nuages
TTF_Font* policeFin = NULL;  // Police utilisée pour le texte de fin de jeu
SDL_Color couleurFin = {0, 0, 0, 255};  // Couleur du texte de fin de jeu

SDL_Rect rectOiseau = { 100, HAUTEUR_FENETRE / 2 - TAILLE_OISEAU / 2, TAILLE_OISEAU, TAILLE_OISEAU };  // Rectangle de l'oiseau
int vitesseOiseau = 0;  // Vitesse actuelle de l'oiseau

Tuyau tuyau1, tuyau2;  // Deux instances de la structure Tuyau pour les tuyaux du jeu
int score = 0;  // Score du joueur
bool fin_jeu = false;  // Indicateur de fin de jeu
int departEspaceTuyau = 0;  // Position de départ de l'espace entre les tuyaux


// Fonction qui génère les tuyaux à des positions aléatoires
void genererTuyaux()
{
    // Générer une valeur aléatoire pour l'espace de départ du tuyau 1
    tuyau1.departEspace = rand() % (HAUTEUR_FENETRE - ESPACE_TUYAU);
    // Générer une valeur aléatoire pour l'espace de départ du tuyau 2
    tuyau2.departEspace = rand() % (HAUTEUR_FENETRE - ESPACE_TUYAU);
}

// Fonction qui initialise le jeu
void initialiserJeu()
{
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    fenetre = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGEUR_FENETRE, HAUTEUR_FENETRE, 0);
    if (fenetre == NULL) {
        erreur("Impossible de créer la fenêtre");
    }

    moteurRendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (moteurRendu == NULL)
    {
        erreur("Impossible de créer le moteur de rendu");
    }

    SDL_Surface* surfaceOiseau = IMG_Load("Images/oiseau.png");
    if (surfaceOiseau == NULL)
    {
        erreur("Impossible de charger l'image oiseau.png");
    }
    textureOiseau = SDL_CreateTextureFromSurface(moteurRendu, surfaceOiseau);
    SDL_FreeSurface(surfaceOiseau);

    SDL_Surface* surfaceTuyau = IMG_Load("Images/tuyau.png");
    if (surfaceTuyau == NULL)
    {
        erreur("Impossible de charger l'image tuyau.png");
    }
    textureTuyau = SDL_CreateTextureFromSurface(moteurRendu, surfaceTuyau);
    SDL_FreeSurface(surfaceTuyau);

    SDL_Surface* surfaceNuage = IMG_Load("Images/nuages.jpg");
    if (surfaceNuage == NULL)
    {
        erreur("Impossible de charger l'image nuages.jpg");
    }
    textureNuage = SDL_CreateTextureFromSurface(moteurRendu, surfaceNuage);
    SDL_FreeSurface(surfaceNuage);

    SDL_Surface* surfaceFond = IMG_Load("Images/fond.jpg");
    if (surfaceFond == NULL)
    {
        erreur("Impossible de charger l'image fond.jpg");
    }
    textureFond = SDL_CreateTextureFromSurface(moteurRendu, surfaceFond);
    SDL_FreeSurface(surfaceFond);

    policeFin = TTF_OpenFont("Font/arial_bold.ttf", 58);
    if (policeFin == NULL)
    {
        erreur("Impossible de charger la police arial_bold.ttf");
    }

    tuyau1.x = LARGEUR_FENETRE;
    tuyau2.x = LARGEUR_FENETRE + LARGEUR_FENETRE / 2;
    genererTuyaux();

    // Réinitialisation de l'état du jeu
    fin_jeu = false;
    score = 0;
    vitesseOiseau = 0;

    rectOiseau.y = HAUTEUR_FENETRE / 2 - TAILLE_OISEAU / 2;  // Réinitialisation de la position de l'oiseau
}

// Fonction qui ferme le jeu
void fermerJeu()
{
    SDL_DestroyTexture(textureOiseau);
    SDL_DestroyTexture(textureTuyau);
    SDL_DestroyTexture(textureFond);
    SDL_DestroyTexture(textureTexteFin);
    SDL_DestroyTexture(textureNuage);
    TTF_CloseFont(policeFin);
    SDL_DestroyRenderer(moteurRendu);
    SDL_DestroyWindow(fenetre);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// Fonction qui met à jour le jeu
void mettreAJourJeu()
{
    // Si le jeu est terminé, on quitte
    if (fin_jeu)
    {
        return;
    }

    vitesseOiseau += GRAVITE; // Ajouter la gravité à la vitesse de l'oiseau
    rectOiseau.y += vitesseOiseau; // Mettre à jour la position verticale de l'oiseau

    tuyau1.x -= 2; // Déplacer le premier tuyau vers la gauche
    // Si le premier tuyau a quitté l'écran à gauche
    if (tuyau1.x + LARGEUR_TUYAU < rectOiseau.x)
    {
        tuyau1.x = tuyau2.x + LARGEUR_FENETRE / 2; // Réinitialiser la position du premier tuyau
        tuyau1.departEspace = rand() % (HAUTEUR_FENETRE - ESPACE_TUYAU); // Générer une nouvelle position pour l'espace du premier tuyau
        score++;  // Augmenter le score du joueur
    }

    tuyau2.x -= 2; // Déplacer le deuxième tuyau vers la gauche
    // Si le deuxième tuyau a quitté l'écran à gauche
    if (tuyau2.x + LARGEUR_TUYAU < rectOiseau.x)
    {
        tuyau2.x = tuyau1.x + LARGEUR_FENETRE / 2;
        tuyau2.departEspace = rand() % (HAUTEUR_FENETRE - ESPACE_TUYAU);
        score++;
    }

    // Détection de collision avec les bords de l'écran
    if (rectOiseau.y <= 0 || rectOiseau.y + rectOiseau.h >= HAUTEUR_FENETRE)
    {
        fin_jeu = true; // Marquer la fin du jeu
        printf("Fin du jeu! Votre score: %d\n", score); // Afficher le score du joueur sur le terminal
    }

    // Détection de collision avec les tuyaux
    if ((rectOiseau.x + rectOiseau.w > tuyau1.x && rectOiseau.x < tuyau1.x + LARGEUR_TUYAU && (rectOiseau.y < tuyau1.departEspace || rectOiseau.y + rectOiseau.h > tuyau1.departEspace + ESPACE_TUYAU)) ||
        (rectOiseau.x + rectOiseau.w > tuyau2.x && rectOiseau.x < tuyau2.x + LARGEUR_TUYAU && (rectOiseau.y < tuyau2.departEspace || rectOiseau.y + rectOiseau.h > tuyau2.departEspace + ESPACE_TUYAU)))
    {
        fin_jeu = true;
        printf("Fin du jeu! Votre score: %d\n", score);
    }

    if (fin_jeu)
    {
        char texte_fin[70];
        sprintf(texte_fin, "Fin du jeu! Appuyez sur Y pour recommencer ou N pour quitter.");
        SDL_Surface* surfaceFin = TTF_RenderText_Blended(policeFin, texte_fin, couleurFin);
        if (surfaceFin == NULL)
        {
            avertissement("Échec du rendu du texte de fin du jeu");
        }
        else
        {
            textureTexteFin = SDL_CreateTextureFromSurface(moteurRendu, surfaceFin);
            SDL_FreeSurface(surfaceFin);
        }
    }
}

// Fonction qui dessine le jeu
void dessinerJeu()
{
    SDL_RenderClear(moteurRendu);  // Effacer le rendu précédent

    SDL_RenderCopy(moteurRendu, textureFond, NULL, NULL);  // Copier la texture du fond sur le rendu

    SDL_Rect rectNuage = { 0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE };  // Créer un rectangle pour la texture des nuages
    SDL_RenderCopy(moteurRendu, textureNuage, NULL, &rectNuage);  // Copier la texture des nuages sur le rendu avec le rectangle défini

    SDL_Rect rectTuyau = { tuyau1.x, 0, LARGEUR_TUYAU, tuyau1.departEspace };  // Créer un rectangle pour le premier tuyau
    SDL_RenderCopy(moteurRendu, textureTuyau, NULL, &rectTuyau);  // Copier la texture du premier tuyau sur le rendu avec le rectangle défini

    rectTuyau.y = tuyau1.departEspace + ESPACE_TUYAU;  // Mettre à jour la position verticale du rectangle pour le deuxième tronçon du premier tuyau
    rectTuyau.h = HAUTEUR_FENETRE - rectTuyau.y;  // Mettre à jour la hauteur du rectangle pour le deuxième tronçon du premier tuyau
    SDL_RenderCopy(moteurRendu, textureTuyau, NULL, &rectTuyau);  // Copier la texture du deuxième tronçon du premier tuyau sur le rendu avec le rectangle défini

    rectTuyau.x = tuyau2.x;  // Mettre à jour la position horizontale du rectangle pour le deuxième tuyau
    rectTuyau.y = 0;  // Réinitialiser la position verticale du rectangle pour le deuxième tuyau
    rectTuyau.h = tuyau2.departEspace;  // Mettre à jour la hauteur du rectangle pour le premier tronçon du deuxième tuyau
    SDL_RenderCopy(moteurRendu, textureTuyau, NULL, &rectTuyau);  // Copier la texture du premier tronçon du deuxième tuyau sur le rendu avec le rectangle défini

    rectTuyau.y = tuyau2.departEspace + ESPACE_TUYAU;  // Mettre à jour la position verticale du rectangle pour le deuxième tronçon du deuxième tuyau
    rectTuyau.h = HAUTEUR_FENETRE - rectTuyau.y;  // Mettre à jour la hauteur du rectangle pour le deuxième tronçon du deuxième tuyau
    SDL_RenderCopy(moteurRendu, textureTuyau, NULL, &rectTuyau);  // Copier la texture du deuxième tronçon du deuxième tuyau sur le rendu avec le rectangle défini

    SDL_RenderCopy(moteurRendu, textureOiseau, NULL, &rectOiseau);  // Copier la texture de l'oiseau sur le rendu avec le rectangle défini

    if (fin_jeu)
    {
        SDL_Rect rectTexte = { 50, HAUTEUR_FENETRE / 2 - 50, 540, 100 };  // Créer un rectangle pour le texte de fin de jeu
        SDL_RenderCopy(moteurRendu, textureTexteFin, NULL, &rectTexte);  // Copier la texture du texte de fin de jeu sur le rendu avec le rectangle défini
    }

    SDL_RenderPresent(moteurRendu);  // Mettre à jour l'affichage avec le rendu actuel
}


// Fonction qui fait fonctionner le jeu
void executerJeu()
{
    initialiserJeu();  // Initialiser le jeu

    SDL_Event evenement;  // Créer un événement pour gérer les entrées de l'utilisateur

    while (1)  // Boucle principale du jeu
    {
        while (SDL_PollEvent(&evenement))  // Vérifier s'il y a des événements en attente
        {
            if (evenement.type == SDL_QUIT)  // Si l'événement est de quitter la fenêtre
            {
                fermerJeu();  // Fermer le jeu
                return;  // Sortir de la fonction
            }
            else if (evenement.type == SDL_KEYDOWN)  // Si l'événement est une touche enfoncée
            {
                if (fin_jeu)  // Si le jeu est terminé
                {
                    if (evenement.key.keysym.sym == SDLK_y)  // Si la touche enfoncée est 'y'
                    {
                        fermerJeu();  // Fermer l'état de jeu actuel
                        initialiserJeu();  // Initialiser un nouvel état de jeu
                    }
                    else if (evenement.key.keysym.sym == SDLK_n)  // Si la touche enfoncée est 'n'
                    {
                        fermerJeu();  // Fermer le jeu
                        return;  // Sortir de la fonction
                    }
                }
                else  // Si le jeu est en cours
                {
                    if (evenement.key.keysym.sym == SDLK_SPACE)  // Si la touche enfoncée est la barre d'espace
                    {
                        vitesseOiseau = -FORCE_SAUT;  // Définir la vitesse de l'oiseau vers le haut c'est-à-dire l'oiseau saute
                    }
                }
            }
        }

        mettreAJourJeu();  // Mettre à jour l'état du jeu
        dessinerJeu();  // Dessiner le jeu
        SDL_Delay(20);  // Délai pour contrôler la vitesse de rafraîchissement du jeu
    }
}


int main()
{
    executerJeu();
    return 0;
}
