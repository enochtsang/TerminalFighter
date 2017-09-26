#include <map>
#include <unistd.h>
#include <memory>

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
extern "C" {
#include <SDL.h>
#include <SDL_ttf.h>
}
#include <SDL_ttf.h>
#include <SDL2/SDL.h>
#include <glog/logging.h>

#include "GameState/I_GameState.h"
#include "GameStateHandler/GameStateHandler.h"

#include "TestState/TestState.h"
#include "MenuState/MenuState.h"
#include "GameConstants/GameConstants.h"

SDL_Renderer* main_renderer = NULL;
SDL_Window* window = NULL;
bool quit;

void print_renderer_info();
bool init_sdl();
bool create_sdl_window(bool high_dpi);
bool create_main_renderer();
void close();

void print_renderer_info() {
    int numdrivers = SDL_GetNumRenderDrivers ();
    LOG(INFO) << "Render driver count: " << numdrivers;

    for (int i = 0; i < numdrivers; i++) {
        SDL_RendererInfo drinfo;
        SDL_GetRenderDriverInfo (0, &drinfo);
        LOG(INFO) << "Driver name (" << i << "): " << drinfo.name;

        if (drinfo.flags & SDL_RENDERER_SOFTWARE) {
            LOG(INFO) << "The main_renderer is a software fallback";
        }

        if (drinfo.flags & SDL_RENDERER_ACCELERATED) {
            LOG(INFO) << "The main_renderer uses hardware acceleration";
        }

        if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC) {
            LOG(INFO) << "Tresent is synchronized with the refresh rate";
        }

        if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE) {
            LOG(INFO) << "The main_renderer supports rendering to texture";
        }
    }
}

bool init_sdl() {
    //Initializes SDL
    if (SDL_Init(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) < 0) {
        LOG(ERROR) << "SDL could not initialize! SDL Error: " << SDL_GetError();
        return false;
    }

    return true;
}

bool create_sdl_window(bool high_dpi) {
    int sdl_flags = SDL_WINDOW_SHOWN;
    auto screen_width = SCREEN_WIDTH;
    auto screen_height = SCREEN_HEIGHT;

    if (high_dpi) {
        sdl_flags |= SDL_WINDOW_ALLOW_HIGHDPI;
        screen_width /= 2;
        screen_height /= 2;
    }

    window = SDL_CreateWindow("TerminalFighter",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_width,
                              screen_height,
                              sdl_flags);

    if (window == NULL) {
        LOG(ERROR) << "Window could not be created! SDL Error: " << SDL_GetError();
        return false;
    }

    LOG(INFO) << "Driver: " <<  SDL_GetCurrentVideoDriver();
    return true;
}

bool create_main_renderer() {
    //Creates the main_renderer.
    main_renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (main_renderer == NULL) {
        LOG(WARNING) << "Renderer could not be created. SDL_Error: " << SDL_GetError();
        LOG(WARNING) << "Creating a software main_renderer instead";
        main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        if (main_renderer == NULL) {
            LOG(ERROR) << "Renderer could not be created. SDL_Error: " << SDL_GetError();
            return false;
            //SDL_SetRenderDrawColor(main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }

    return true;
}

/* Cleans up and should free everything used in the program*/
void close() {
    SDL_DestroyRenderer(main_renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    main_renderer = NULL;
    SDL_Quit();
}

int main(int argc, char* argv[]) {
#if defined(__linux__) || defined(__APPLE__)
    system("mkdir -p /tmp/TerminalFighter/");
    FLAGS_log_dir = "/tmp/TerminalFighter";
#endif
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Logging Intialized INFO";

    bool high_dpi = false;

    if (argc > 1 && strcmp(argv[1], "high_dpi") == 0) {
        high_dpi = true;
    }

    print_renderer_info();

    if (!init_sdl()) {
        LOG(FATAL) << "Could not initialize SDL!";
    }

    if (!create_sdl_window(high_dpi)) {
        LOG(FATAL) << "Could not create window!";
    }

    if (!create_main_renderer()) {
        LOG(FATAL) << "Could not create main renderer";
    }

    if (TTF_Init() != 0) {
        LOG(FATAL) << "TTF Init failed! " << TTF_GetError();
    }

    std::unique_ptr<I_GameState> test_state(new TestState(*main_renderer));
    std::unique_ptr<I_GameState> menu_state(new MenuState(*main_renderer));
    std::vector<I_GameState*> gamestates;
    gamestates.push_back(test_state.get());
    gamestates.push_back(menu_state.get());
    GameStateHandler gs_handler = GameStateHandler(gamestates);
    gs_handler.start(gamestates::MAIN_MENU);
    close();
    return 0;
}
