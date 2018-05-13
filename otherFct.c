#include "otherFct.h"

void handleEvents(gameState *state,rectangle *rec,int *up, int *down){


    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=ZERO;break;
        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                            {
                                case SDLK_UP: *up=UN; break;
                                case SDLK_DOWN: *down=UN; break;
                                case SDLK_ESCAPE: state->g_bRunning=ZERO; break;
                            }
                            break;

        case SDL_KEYUP:
                        switch (event.key.keysym.sym)
                            {
                                case SDLK_UP: *up=ZERO; break;
                                case SDLK_DOWN: *down=ZERO; break;
                            }
                            break;

        default:break;

        }
    }

}

void renderTexture(game *myGame,rectangle *rec,rectangle *recIA, rectangle *balle, rectangle *barre ) {


    SDL_SetRenderDrawColor(myGame->g_pRenderer,rec->R,rec->G,rec->B,rec->A);
    SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

    SDL_RenderFillRect(myGame->g_pRenderer, &rec->taille);

    //____________________________________________________________________________________________
    //____________________________________________________________________________________________

    SDL_SetRenderDrawColor(myGame->g_pRenderer,recIA->R,recIA->G,recIA->B,recIA->A);
    SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

    SDL_RenderFillRect(myGame->g_pRenderer, &recIA->taille);

    //____________________________________________________________________________________________
    //____________________________________________________________________________________________

    SDL_SetRenderDrawColor(myGame->g_pRenderer,balle->R,balle->G,balle->B,balle->A);
    SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

    SDL_RenderFillRect(myGame->g_pRenderer, &balle->taille);

    //____________________________________________________________________________________________
    //____________________________________________________________________________________________

    SDL_SetRenderDrawColor(myGame->g_pRenderer,barre->R,barre->G,barre->B,barre->A);
    SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

    SDL_RenderFillRect(myGame->g_pRenderer, &barre->taille);

    //____________________________________________________________________________________________
    //____________________________________________________________________________________________

    SDL_RenderPresent(myGame->g_pRenderer);

    SDL_SetRenderDrawColor(myGame->g_pRenderer,ZERO,ZERO,25,COLORMAX);
    SDL_DestroyTexture(myGame->g_ptexture);
    SDL_RenderClear(myGame->g_pRenderer);

}

int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame){


    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;


    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=ZERO)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-UN,SDL_RENDERER_PRESENTVSYNC);

            }
    }else{

        return ZERO;
    }

    return UN;
}


void moveBalle(rectangle *recIA,rectangle *rec,rectangle *balle,int *allerX, int *allerY, int *retourX,
               int *scoreD, int *scoreG, int *manche,int* up, int *down, int *vitesseX ,int *vitesseY, Mix_Chunk *ping, Mix_Chunk *pong)
{
    //__________________________________________________
    //CHANGEMENT DE DIRECTION Y
    //__________________________________________________


    if (balle->taille.y>=(SCREEN_HEIGHT-balle->taille.h))
    {
        Mix_PlayChannel(UN, pong, ZERO);
        *allerY=ZERO;
    }

    if (balle->taille.y<=ZERO)
    {
        Mix_PlayChannel(UN, pong, ZERO);
        *allerY=UN;
    }

    //__________________________________________________
    //COLLISION JOUEUR UN
    //__________________________________________________

    if (balle->taille.x<=(rec->taille.w+rec->taille.x))
    {
        if((balle->taille.y<=(rec->taille.h+rec->taille.y))&&((balle->taille.y+balle->taille.h)>=rec->taille.y))
        {
            Mix_PlayChannel(ZERO, ping, ZERO);

            if (*retourX==ZERO)
            {
                *retourX=UN;
                *allerX=ZERO;
            }
            else
            {
                *retourX=ZERO;
                *allerX=UN;
            }
            *vitesseX=*vitesseX+UN;
            if (*vitesseX>VITESSEMAX) *vitesseX=VITESSEMAX;


            //si on tape dans le coin du haut de la barre on modifie la vitesseY de la balle

            if ((((balle->taille.y)+(balle->taille.h/DEUX))<=((rec->taille.y)+(rec->taille.h/QUATRE)))&&(((balle->taille.y)+(balle->taille.h/DEUX))>=((rec->taille.y))))
            {

                if (*vitesseY==ZERO)
                {
                   *allerY=ZERO;
                }

                if (*allerY==ZERO)
                {
                    *vitesseY=*vitesseY+TROIS;
                }
                else
                {
                    *vitesseY=*vitesseY-TROIS;
                }

                if (*vitesseY<ZERO)
                {
                    *vitesseY=(*vitesseY*-UN);

                    if (*allerY==ZERO)
                    {
                        *allerY=UN;
                    }
                    else
                    {
                        *allerY=ZERO;
                    }

                }

            }

            //si on tape dans le coin du milieu/haut de la barre on modifie la vitesseY de la balle

            if ((((balle->taille.y)+(balle->taille.h/DEUX))<=((rec->taille.y)+((rec->taille.h/DEUX))-DIX))&&(((balle->taille.y)+(balle->taille.h/DEUX))>=((rec->taille.y)+(rec->taille.h/QUATRE))))
            {
                if (*vitesseY==ZERO)
                {
                   *allerY=ZERO;
                }

                if (*allerY==ZERO)
                {
                    *vitesseY=*vitesseY+UN;
                }
                else
                {
                    *vitesseY=*vitesseY-UN;
                }

                if (*vitesseY<ZERO)
                {
                    *vitesseY=(*vitesseY*-UN);

                    if (*allerY==ZERO)
                    {
                        *allerY=UN;
                    }
                    else
                    {
                        *allerY=ZERO;
                    }

                }

            }

            //si on tape dans le coins du bas de la barre on modifie la vitesseY de la balle

            if ((((balle->taille.y)+(balle->taille.h/DEUX))>=((rec->taille.y)+(rec->taille.h-(rec->taille.h/QUATRE))))&&(((balle->taille.y)+(balle->taille.h/DEUX))<=((rec->taille.h)+rec->taille.y)))
            {

                if (*vitesseY==ZERO)
                {
                   *allerY=ZERO;
                }

                if (*allerY==ZERO)
                {
                    *vitesseY=*vitesseY-TROIS;
                }
                else
                {
                    *vitesseY=*vitesseY+TROIS;
                }

                if (*vitesseY<ZERO)
                {
                    *vitesseY=(*vitesseY*-UN);

                    if (*allerY==UN)
                    {
                        *allerY=ZERO;
                    }
                    else
                    {
                        *allerY=UN;
                    }

                }

            }
            //si on tape dans le coins du milieu/bas de la barre on modifie la vitesseY de la balle

            if ((((balle->taille.y) + (balle->taille.h/DEUX))>=((rec->taille.y) + (rec->taille.h - (rec->taille.h/DEUX))+DIX))&&(((balle->taille.y) + (balle->taille.h/DEUX))<=((rec->taille.h + rec->taille.y - (rec->taille.h/QUATRE)))))
            {

                if (*vitesseY==ZERO)
                {
                   *allerY=ZERO;
                }

                if (*allerY==ZERO)
                {
                    *vitesseY=*vitesseY-UN;
                }
                else
                {
                    *vitesseY=*vitesseY+UN;
                }

                if (*vitesseY<ZERO)
                {
                    *vitesseY=(*vitesseY*-UN);

                    if (*allerY==UN)
                    {
                        *allerY=ZERO;
                    }
                    else
                    {
                        *allerY=UN;
                    }

                }

            }
            //ajouter les deux autres spots

        }

    }

    //__________________________________________________
    //COLLISION JOUEUR IA
    //__________________________________________________

    if ((balle->taille.x+balle->taille.w)>=(recIA->taille.x))
    {
        if((balle->taille.y<=(recIA->taille.h+recIA->taille.y))&&((balle->taille.y+balle->taille.h)>=recIA->taille.y))
        {
            Mix_PlayChannel(ZERO, ping, ZERO);
            if (*retourX==ZERO)
            {
                *retourX=UN;
                *allerX=ZERO;
            }
            else
            {
                *retourX=ZERO;
                *allerX=UN;
            }
        }

    }
    //__________________________________________________
    //DEPLACEMENT BALLE
    //__________________________________________________


    if (*allerX==ZERO)
    {
        balle->taille.x=balle->taille.x-*vitesseX;
    }
    else
    {
        balle->taille.x=balle->taille.x+*vitesseX;
    }

    if (*allerY==ZERO)
    {
        balle->taille.y=balle->taille.y-*vitesseY;
    }
    else
    {
        balle->taille.y=balle->taille.y+*vitesseY;
    }


    //__________________________________________________
    //DEPLACEMENT JOUEUR
    //__________________________________________________

    if (*up)
    {
        if(rec->taille.y>=DIX)
        {
            rec->taille.y=rec->taille.y-DIX;
        }

    }
    if (*down)
    {
        if((rec->taille.y+rec->taille.h)<=(SCREEN_HEIGHT-DIX))
        {
            rec->taille.y=rec->taille.y+DIX;
        }

    }


    //__________________________________________________
    //SCORE
    //__________________________________________________

    if ((balle->taille.x>SCREEN_WIDTH)||(balle->taille.x<ZERO))
    {
        if (balle->taille.x>SCREEN_WIDTH)
        {
            *scoreG=*scoreG+UN;
        }
        if (balle->taille.x<ZERO)
        {
            *scoreD=*scoreD+UN;
        }
        *manche=ZERO;
    }

}

void moveIA(rectangle *recIA,rectangle *balle,int *vitesseY)
{
    int down=ZERO;
    int up=ZERO;

    if ((balle->taille.y-CINQUANTE)>recIA->taille.y)
    {
        down=UN;
        up=ZERO;
    }
    if ((balle->taille.y-CINQUANTE)<recIA->taille.y)
    {
        down=ZERO;
        up=UN;
    }
    if (up)
    {
        if(recIA->taille.y>=DIX)
        {
            if(*vitesseY<=DIX)
            {
                recIA->taille.y=recIA->taille.y-*vitesseY;
            }
            else
            {
                recIA->taille.y=recIA->taille.y-DIX;
            }
        }

    }
    if (down)
    {
        if((recIA->taille.y+recIA->taille.h)<=(SCREEN_HEIGHT-DIX))
        {
            if(*vitesseY<=DIX)
            {
                recIA->taille.y=recIA->taille.y+*vitesseY;
            }
            else
            {
                recIA->taille.y=recIA->taille.y+DIX;
            }

        }

    }

}
