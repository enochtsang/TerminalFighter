#pragma once

#include <string>

class KeyboardListener{
public:
    virtual void handle_key_press(std::string keypress) = 0;
};
