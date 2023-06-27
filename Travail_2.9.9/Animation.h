#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LARGEUR_FENETRE 640  // Définition de la largeur de la fenêtre du jeu
#define HAUTEUR_FENETRE 480  // Définition de la hauteur de la fenêtre du jeu
#define LARGEUR_TUYAU 80  // Définition de la largeur d'un tuyau
#define ESPACE_TUYAU 150  // Définition de l'espace entre les tuyaux
#define TAILLE_OISEAU 32  // Définition de la taille de l'oiseau
#define GRAVITE 1  // Définition de la valeur de la gravité pour l'oiseau
#define FORCE_SAUT 10  // Définition de la valeur de la force de saut pour l'oiseau

// Définition de la structure Tuyau qui contient la position horizontale (x) et la position verticale du départ de l'espace (departEspace)
typedef struct {
    int x;
    int departEspace;
} Tuyau;

// Prototypes des fonctions du jeu
void genererTuyaux();
void initialiserJeu();
void fermerJeu();
void mettreAJourJeu();
void dessinerJeu();
void executerJeu();

#endif /* ANIMATION_H */
