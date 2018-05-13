#include "otherFct.h"
#include "delay.h"
#include "rectangle.h"



//_________________________________ TODO_____________________________

//modif de Y quand on tape à un bord ou l'autre de la raquette


int main(int argc, char *argv[])
{
    srand(time(NULL));
    game myGame;
    gameState state;

    unsigned int frameLimit = SDL_GetTicks() + FPS;

    rectangle rec;
    rectangle recIA;
    rectangle balle;
    rectangle scoreA;
    rectangle scoreB;
    rectangle barre;
    rectangle splash;

    initRec(&rec,DIX,SCREEN_HEIGHT/DEUX,DIX,CENT,COLORMAX,ZERO,ZERO,COLORMAX);
    initRec(&recIA,SCREEN_WIDTH-VINGT,SCREEN_HEIGHT/DEUX,DIX,CENT,ZERO,ZERO,COLORMAX,COLORMAX);
    initRec(&balle,SCREEN_WIDTH/DEUX,SCREEN_HEIGHT/DEUX,DIX,DIX,ZERO,COLORMAX,ZERO,COLORMAX);
    initRec(&scoreA,(SCREEN_WIDTH/DEUX)-CENT,DIX,QVINGT,RAQUETTEH,ZERO,ZERO,ZERO,COLORMAX);
    initRec(&scoreB,(SCREEN_WIDTH/DEUX)+CINQUANTE,DIX,QVINGT,RAQUETTEH,ZERO,ZERO,ZERO,COLORMAX);
    initRec(&barre,(SCREEN_WIDTH/DEUX),ZERO,CINQ,SCREEN_HEIGHT,COLORMAX,COLORMAX,COLORMAX,COLORMAX);
    initRec(&splash,ZERO,ZERO,SCREEN_WIDTH,SCREEN_HEIGHT,COLORMAX,COLORMAX,COLORMAX,COLORMAX);

    int allerX=ZERO;
    int allerY=ZERO;
    int retourX=UN;
    int res=ZERO;
    int depart=UN;
    int scoreG=ZERO;
    int scoreD=ZERO;
    int manche=UN;
    int up=ZERO;
    int down=ZERO;
    int vitesseX=TROIS;
    int vitesseY=TROIS;
    char texte[30];
    SDL_Color couleur ={COLORMAX,COLORMAX,COLORMAX};
    SDL_Surface *affscore =NULL;

    //_______________________________________
    //OUVERTURE DE SDL_TTF
    //_______________________________________

    TTF_Font *police = NULL;
    TTF_Init();
    police = TTF_OpenFont("PressStart2P-Regular.ttf", QVINGT);

    //_______________________________________
    //OUVERTURE DE SDL_Mixer
    //_______________________________________

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, SCREEN_WIDTH);

    Mix_AllocateChannels(CINQ);

    Mix_Music *musique; //Création du pointeur de type Mix_Music
    Mix_Chunk *ping;    //Création des pointeurs de type Mix_Chunk
    Mix_Chunk *pong;
    Mix_Chunk *ready;
    Mix_Chunk *goal;

    ping = Mix_LoadWAV("ping.wav");
    pong = Mix_LoadWAV("pong.wav");
    ready = Mix_LoadWAV("ready.wav");
    goal = Mix_LoadWAV("point.wav");

    musique = Mix_LoadMUS("music.mp3"); //Chargement de la musique
    Mix_PlayMusic(musique, -UN); //Jouer infiniment la musique

    state.g_bRunning=UN;

    //_______________________________________
    //CREER UN ECRAN
    //_______________________________________

    init("Pong", 250,RAQUETTEH,SCREEN_WIDTH, SCREEN_HEIGHT,ZERO,&myGame);

    //_______________________________________
    //SPLASH SCREEN UN
    //_______________________________________

    myGame.g_psurface= SDL_LoadBMP("splash1.bmp");
    myGame.g_ptexture = SDL_CreateTextureFromSurface(myGame.g_pRenderer, myGame.g_psurface);
    SDL_RenderCopy(myGame.g_pRenderer, myGame.g_ptexture, NULL, &splash.taille);
    SDL_RenderPresent(myGame.g_pRenderer);

    SDL_Delay(SPLASH);

    SDL_DestroyTexture(myGame.g_ptexture);
    SDL_RenderClear(myGame.g_pRenderer);

    //_______________________________________
    //SPLASH SCREEN DEUX
    //_______________________________________

    myGame.g_psurface= SDL_LoadBMP("splash2.bmp");
    myGame.g_ptexture = SDL_CreateTextureFromSurface(myGame.g_pRenderer, myGame.g_psurface);
    SDL_RenderCopy(myGame.g_pRenderer, myGame.g_ptexture, NULL, &splash.taille);
    SDL_RenderPresent(myGame.g_pRenderer);

    SDL_Delay(SPLASH);

    SDL_DestroyTexture(myGame.g_ptexture);
    SDL_RenderClear(myGame.g_pRenderer);




    while(state.g_bRunning)
    {
        while((manche==UN)&&(state.g_bRunning))
        {
            //_______________________________________
            //GESTION DES TOUCHES
            //_______________________________________

            handleEvents(&state,&rec,&up,&down);

            //_______________________________________
            //GESTION DU DEPLACEMENT DE LA BALLE +COLLISIONS
            //_______________________________________

            moveBalle(&recIA,&rec,&balle,&allerX,&allerY,&retourX,&scoreD,&scoreG,&manche,&up,&down,&vitesseX,&vitesseY,ping,pong);
            moveIA(&recIA,&balle,&vitesseY);

            //_______________________________________
            //AFFICHAGE DU SCORE
            //_______________________________________

            sprintf(texte,"%d",scoreG);
            affscore = TTF_RenderText_Solid(police,texte,couleur);
            myGame.g_ptexture = SDL_CreateTextureFromSurface(myGame.g_pRenderer, affscore);
            SDL_RenderCopy(myGame.g_pRenderer, myGame.g_ptexture, NULL, &scoreA.taille);

            sprintf(texte,"%d",scoreD);
            affscore = TTF_RenderText_Solid(police,texte,couleur);
            myGame.g_ptexture = SDL_CreateTextureFromSurface(myGame.g_pRenderer, affscore);
            SDL_RenderCopy(myGame.g_pRenderer, myGame.g_ptexture, NULL, &scoreB.taille);

            //_______________________________________
            //RENDU FINAL
            //_______________________________________

            renderTexture(&myGame,&rec,&recIA,&balle,&barre);

            // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
            delay(frameLimit);
            frameLimit = SDL_GetTicks() + FPS;

            if (depart==UN)
            {

                //action du premier tour
                //random du joueur commençant
                res=rand()%DEUX+UN;

                if(res==UN)
                {
                    allerX=ZERO;
                    allerY=ZERO;
                    retourX=UN;
                }
                else
                {
                    allerX=UN;
                    allerY=UN;
                    retourX=ZERO;
                }

                depart=ZERO;

                SDL_Delay(3000);
            }
        }

        if ((scoreD==DIX)||(scoreG==DIX))
        {
            //_______________________________________
            //FONCTION D'AFFICHAGE DES SCORES DE FIN
            //_______________________________________

            if (scoreD==DIX)
            {
                strcpy(texte,"Le joueur droit à gagné !");
                scoreB.taille.x=SCOREX;
                scoreB.taille.y=SCREEN_HEIGHT/DEUX;
                scoreB.taille.w=SCOREW;
                scoreB.taille.h=QUARANTE;
                affscore = TTF_RenderText_Solid(police,texte,couleur);
                myGame.g_ptexture = SDL_CreateTextureFromSurface(myGame.g_pRenderer, affscore);
                SDL_RenderCopy(myGame.g_pRenderer, myGame.g_ptexture, NULL, &scoreB.taille);

                SDL_RenderPresent(myGame.g_pRenderer);
            }
            else
            {
                strcpy(texte,"Le joueur gauche à gagné !");
                scoreA.taille.x=SCOREX;
                scoreA.taille.y=SCREEN_HEIGHT/DEUX;
                scoreA.taille.w=SCOREW;
                scoreA.taille.h=QUARANTE;
                affscore = TTF_RenderText_Solid(police,texte,couleur);
                myGame.g_ptexture = SDL_CreateTextureFromSurface(myGame.g_pRenderer, affscore);
                SDL_RenderCopy(myGame.g_pRenderer, myGame.g_ptexture, NULL, &scoreA.taille);

                SDL_RenderPresent(myGame.g_pRenderer);
            }

            SDL_Delay(6000);

            state.g_bRunning=ZERO;
        }
        else
        {
            //_______________________________________
            //RESET DU NIVEAU
            //_______________________________________

            Mix_PlayChannel(DEUX, ready, ZERO);
            Mix_PlayChannel(TROIS, goal, ZERO);

            manche=UN;
            vitesseX=TROIS;
            vitesseY=TROIS;

            initRec(&rec,DIX,SCREEN_HEIGHT/DEUX,DIX,CENT,COLORMAX,ZERO,ZERO,COLORMAX);
            initRec(&recIA,SCREEN_WIDTH-VINGT,SCREEN_HEIGHT/DEUX,DIX,CENT,ZERO,ZERO,COLORMAX,COLORMAX);
            initRec(&balle,SCREEN_WIDTH/DEUX,SCREEN_HEIGHT/DEUX,DIX,DIX,ZERO,COLORMAX,ZERO,COLORMAX);

            //_______________________________________
            //AFFICHAGE DU SCORE
            //_______________________________________

            sprintf(texte,"%d",scoreG);
            affscore = TTF_RenderText_Solid(police,texte,couleur);
            myGame.g_ptexture = SDL_CreateTextureFromSurface(myGame.g_pRenderer, affscore);
            SDL_RenderCopy(myGame.g_pRenderer, myGame.g_ptexture, NULL, &scoreA.taille);

            sprintf(texte,"%d",scoreD);
            affscore = TTF_RenderText_Solid(police,texte,couleur);
            myGame.g_ptexture = SDL_CreateTextureFromSurface(myGame.g_pRenderer, affscore);
            SDL_RenderCopy(myGame.g_pRenderer, myGame.g_ptexture, NULL, &scoreB.taille);

            //_______________________________________
            //RENDU FINAL
            //_______________________________________

            renderTexture(&myGame,&rec,&recIA,&balle,&barre);
            SDL_Delay(3000);
        }

    }

    system ("cls");
    //_______________________________________
    //FERMETURE DES SDL
    //_______________________________________

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_FreeSurface(affscore);
    Mix_FreeChunk(ping);    //Libération du son ping
    Mix_FreeChunk(pong);    //Libération du son pong
    Mix_FreeChunk(ready);    //Libération du son ready
    Mix_FreeChunk(goal);    //Libération du son goal
    Mix_FreeMusic(musique); //Libération de la musique
    Mix_CloseAudio();
    destroy(&myGame);

    SDL_Quit();

    return ZERO;
}
