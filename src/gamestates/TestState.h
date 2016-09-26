#pragma once
#include <SDL2/SDL.h>

#include "I_Gamestate.h"
#include "Delay.h"
#include "GameConstants.h"
#include "KeyboardListener.h"
#include "Keyboard.h"
#include "MissileLauncher.h"
#include "Events.h"

class Delay;
class SDL_Renderer;

class TestState : public I_GameState, public KeyboardListener, public EventsListener {
public:
    TestState(SDL_Renderer& renderer);
    gamestates::GameStateName run();
    gamestates::GameStateName name() const;

    /* listeners */
    void handle_key_press(const std::string& keypress);
    void notify_events(const SDL_Event& e);

private:
    void display_debug_frames_(Delay *delayer);
    bool exit_;

    SDL_Renderer& renderer_;
};