#include "SpriteAnimator.h"

SpriteAnimator::SpriteAnimator()
    : initialized_(false)
{}

// Keeping this as an init right now because the objects may not
int SpriteAnimator::init(SDL_Texture* texture, int rows, int cols, int on_every) {
    on_every_ = on_every;
    rows_ = rows;
    cols_ = cols;
    total_frames_ = rows * cols;
    current_frame_ = 0;
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    // We will run off the assumption that the given
    // spritesheet has it's starting sprites on 0,0.
    // If there is padding... manually get rid of it for now
    total_sprite_sheet_size_.x = 0;
    total_sprite_sheet_size_.y = 0;
    total_sprite_sheet_size_.w = w;
    total_sprite_sheet_size_.h = h;
    initialized_ = true;
    printf("Total RECT: with src_rect: (x,y): %d, %d, (w,h) %d, %d\n",
            total_sprite_sheet_size_.x,
            total_sprite_sheet_size_.y,
            total_sprite_sheet_size_.w,
            total_sprite_sheet_size_.h);
}

SDL_Rect SpriteAnimator::get_next_frame(int& returned_frame) {
    // We want to be able to call this function, and start with frame 0
    // So we advance the frame at the end of this function
    returned_frame = current_frame_;
    SDL_Rect return_rect;

    int on_row = current_frame_ / cols_; //integer division for the floor
    int on_col = current_frame_ % (rows_ - 1);

    printf("on_row:%d on_col:%d", on_row, on_col);
    return_rect.w = total_sprite_sheet_size_.w / cols_;
    return_rect.h = total_sprite_sheet_size_.h / rows_;
    return_rect.x = on_col * (total_sprite_sheet_size_.w / cols_);
    return_rect.y = on_row * (total_sprite_sheet_size_.h / rows_);

    incrementor_++;
    incrementor_ = incrementor_ % on_every_;
    if (incrementor_ == 0) {
        current_frame_++;
    }
    current_frame_ = current_frame_ % total_frames_;

    return return_rect;
}

SDL_Rect SpriteAnimator::get_next_frame() {
    int throw_away;
    return get_next_frame(throw_away);
}

bool SpriteAnimator::is_initialized() {
    return initialized_;
}
