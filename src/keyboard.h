#pragma once

#include "game_object.h"
#include "keyboard_listener.h"
#include "observable.h"
#include <SDL2/SDL.h>
#include <string>
#include "graphics_handler.h"
#include "events_listener.h"


/* Keyboard
 * Will return a single character std::string when a keypress is made
 * Will return non-single character std::string when the following keypresses are made
 *  Button              Return Value
 *  Left arrow          LEFT         
 *  Right arrow         RIGHT
 *  Up arrow            UP
 *  Down arrow          DOWN
 *  Space Key           SPACE
 *  Backspace           BKSPACE
 *  Esc Key             ESC
 *  Enter Key           ENTER
 *  Forward Slash       SLASH
 *  
 */

class Keyboard : public EventsListener, public Observable<KeyboardListener> {

public: 
    Keyboard() { };

    /* Keyboard will have an empty draw */
private:

    void notify_events(SDL_Event e);
    void notify_key(std::string key);

};

