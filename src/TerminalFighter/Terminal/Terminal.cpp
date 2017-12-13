#include <glog/logging.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "Terminal.h"
#include "GraphicsHandler/I_GraphicsHandler.h"

SDL_Texture* Terminal::terminal_texture_ = NULL;

Terminal::Terminal(
    double x_pos, double y_pos,
    double width, double height,
    std::string graphic_path,
    std::string font_path)
    : GameObject(x_pos, y_pos, 0.0, 0.0, 0)
    , hitbox_(Hitbox(x_pos, y_pos, width, height))
    , terminal_texture_path_(graphic_path)
    , player_text_("")
    , default_font_ (TTF_OpenFont(font_path.c_str(), 84)) {
    if (default_font_ == NULL) {
        LOG(ERROR) << "Failed to load Terminal font: " << font_path;
    };
}

Terminal::~Terminal() {
    if (default_font_ != NULL) {
        TTF_CloseFont(default_font_);
    }
}

void Terminal::draw(I_GraphicsHandler& graphics) {
    if (terminal_texture_ == NULL) {
        LOG(WARNING) << "Terminal graphics were null! Setting terminal graphic...";
        set_texture(graphics.load_image(terminal_texture_path_));
    }

    SDL_Color white = {255, 255, 255};
    SDL_Surface* UIText = TTF_RenderText_Blended(default_font_, player_text_.c_str(), white);
    SDL_Rect ui_src_rect;
    ui_src_rect.x = 0;
    ui_src_rect.y = 0;
    if (UIText == NULL) {
        ui_src_rect.w = 0;
        ui_src_rect.h = 0;
    } else {
        ui_src_rect.w = UIText->w;
        ui_src_rect.h = UIText->h;
    }

    SDL_Rect terminal_src_rect;
    SDL_QueryTexture(terminal_texture_, NULL, NULL, &terminal_src_rect.w, &terminal_src_rect.h);
    terminal_src_rect.x = 0;
    terminal_src_rect.y = 0;

    graphics.draw(terminal_texture_,
                  terminal_src_rect,
                  (int)x_pos(),
                  (int)y_pos()
                  , GraphicPriority::UI,
                  true,
                  0,
                  NULL);
    graphics.draw(UIText,
                  ui_src_rect,
                  (int)x_pos() + 30,
                  (int)y_pos() + 30,
                  GraphicPriority::UI,
                  true,
                  0,
                  NULL);
}

const I_Hitbox& Terminal::hitbox() const {
    return hitbox_;
}

void Terminal::update() {
    //do nothing - TODO: maybe add cool effects one day
}
void Terminal::notify_collision(GameObject& collided_object) {
    //do nothing
}
void Terminal::set_texture(SDL_Texture* texture) {
    terminal_texture_ = texture;
}

void Terminal::handle_key_press(const std::string& keypress) {
    if (keypress.length() == 1) {
        player_text_ = player_text_ + keypress;
    } else if (keypress.compare("BKSPACE") == 0) {
        if (player_text_.length() != 0) {
            player_text_.resize(player_text_.length() - 1);
        }
    } else if (keypress.compare("ENTER") == 0) {
        notify_input(player_text_);
        player_text_.resize(0);
    }
}

void Terminal::notify_input(const std::string& input) {
    for (TerminalListener* listener : Observable<TerminalListener>::listeners_) {
        listener->handle_input(input);
    }
}
