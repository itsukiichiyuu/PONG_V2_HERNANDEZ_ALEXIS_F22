#ifndef OTHERFCT_H_INCLUDED
#define OTHERFCT_H_INCLUDED

#include "rectangle.h"
#include "game.h"

typedef struct gameState{

    int g_bRunning;


}gameState;

extern void handleEvents(gameState *state,rectangle *rec,int *up, int *down);
extern void renderTexture(game *myGame,rectangle *rec,rectangle *recIA,rectangle *balle, rectangle *barre );
extern int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);
extern void moveBalle(rectangle *recIA,rectangle *rec,rectangle *balle,int *allerX,int *allerY,int *retourX,int *scoreD, int *scoreG, int* manche,
                      int *up, int *down,int *vitesseX,int *vitesseY, Mix_Chunk *ping, Mix_Chunk *pong);

extern void moveIA(rectangle *recIA,rectangle *balle,int *vitesseY);
extern void afficheScore(TTF_Font *police,char *texte,SDL_Color *couleur,SDL_Surface *affscore);

#endif
