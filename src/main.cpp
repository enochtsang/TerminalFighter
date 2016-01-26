#include <map>
    
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
extern "C" {
#include <SDL.h>
#include <SDL_ttf.h>
}
#include <stdio.h>
#include <unistd.h>
#include <SDL_ttf.h>
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

#include "I_gamestate.h"
#include "test_state.h"
#include "menu_state.h"
#include "game_constants.h"
#include "gamestate_handler.h"

SDL_Renderer* main_renderer = NULL;
SDL_Window* window = NULL;
bool quit;

bool init_SDL();
void processEvents();
void close();

bool init_SDL()
{
    int numdrivers = SDL_GetNumRenderDrivers ();
    cout << "Render driver count: " << numdrivers << endl;
    for (int i = 0; i < numdrivers; i++) {
        SDL_RendererInfo drinfo;
        SDL_GetRenderDriverInfo (0, &drinfo);
        cout << "Driver name (" << i << "): " << drinfo.name << endl;
        if (drinfo.flags & SDL_RENDERER_SOFTWARE)
            cout << " the main_renderer is a software fallback" << endl;
        if (drinfo.flags & SDL_RENDERER_ACCELERATED)
            cout << " the main_renderer uses hardware acceleration" << endl;
        if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC)
            cout << " present is synchronized with the refresh rate" << endl;
        if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE)
            cout << " the main_renderer supports rendering to texture" << endl;
    }

    printf("Driver: %s\n", SDL_GetCurrentVideoDriver());

    //Initializes SDL
    if (SDL_Init(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Creates the SDL Window
    window = SDL_CreateWindow("Video Application", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    printf("Driver: %s\n", SDL_GetCurrentVideoDriver());

    //Creates the main_renderer.
    main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (main_renderer == NULL)
    {
        printf("Renderer could not be created. SDL_Error: %s \n", SDL_GetError());
        printf("Creating a software main_renderer instead\n");
        main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (main_renderer == NULL)
        {
            printf("Renderer could not be created. SDL_Error: %s \n", SDL_GetError());
            return false;
            //SDL_SetRenderDrawColor(main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }
    return true;
}

/* Cleans up and should free everything used in the program*/
void close()
{
    SDL_DestroyRenderer(main_renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    main_renderer = NULL;
    SDL_Quit();
    exit(0);
}

int main(int argc, char* argv[])
{
    if (!init_SDL()) {
        fprintf(stderr, "Could not initialize SDL!\n");
        return -1;
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF Init failed! %s\n", TTF_GetError());
        return -1;
    }

    std::vector<I_GameState*> gamestates;
    gamestates.push_back(new TestState(main_renderer));
    gamestates.push_back(new MenuState(main_renderer));

    GameStateHandler gs_handler = GameStateHandler(gamestates);
    gs_handler.start(gamestates::MAIN_MENU);
    close();

    for(auto state : gamestates){
        delete state;
    }

    return 0;
}
