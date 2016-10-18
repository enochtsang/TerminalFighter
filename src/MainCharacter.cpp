#include <stdio.h>

#include <SDL2/SDL.h>

#include "MainCharacter.h"
#include "I_GraphicsHandler.h"

SDL_Texture* MainCharacter::texture_ = NULL;

MainCharacter::MainCharacter(double x_pos, double y_pos)
    : GameObject(x_pos, y_pos, 0, 0)
    , hitbox_(Hitbox(x_pos_, y_pos_, 100, 100)) {
}

void MainCharacter::update() {
}

void MainCharacter::draw(I_GraphicsHandler& graphics) {
    if (texture_ == NULL) {
        printf("MainCharacter graphics were null! Setting MainCharacter graphic...\n");
        set_texture(graphics.load_image("assets/ships/Arman.png"));
    }

    graphics.draw(texture_, (int)x_pos(), (int)y_pos(), GraphicPriority::MIDDLE);
}

void MainCharacter::set_texture(SDL_Texture* texture) {
    texture_ = texture;
}

const I_Hitbox& MainCharacter::hitbox() const {
    return hitbox_;
}

void MainCharacter::notify_collision(GameObject& collided_object) {
}