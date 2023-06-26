#include <SDL2/SDL.h>

void verifier_initialisation_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

SDL_Window* creer_fenetre(int x, int y, int LARGEUR_FENETRE, int HAUTEUR_FENETRE)
{
    SDL_Window* fenetre = SDL_CreateWindow("Fenêtre", x, y, LARGEUR_FENETRE, HAUTEUR_FENETRE, 0);
    if (!fenetre)
    {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return fenetre;
}

SDL_Renderer* creer_moteur_de_rendu(SDL_Window* fenetre, int index, Uint32 flags)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(fenetre, index, flags);
    if (!renderer)
    {
        fprintf(stderr, "Erreur lors de la création du moteur de rendu : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return renderer;
}

void erreur(const char* message)
{
    fprintf(stderr, "Erreur: %s\n", message);
    exit(EXIT_FAILURE);
}

void avertissement(const char* message)
{
    fprintf(stderr, "Avertissement: %s\n", message);
}