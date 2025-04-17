#pragma once
#include "function.h"
#include "constants.h"

class GameObject {
public:
    float x, y;              // Vị trí của đối tượng
    SDL_Texture* texture;    // Texture của đối tượng
    SDL_Rect rect;           // Hình chữ nhật chứa vị trí và kích thước của đối tượng
    float rotation;          // Góc xoay của đối tượng

    // Constructor của GameObject
    GameObject(SDL_Texture* tex, float px, float py, int w, int h)
        : texture(tex), x(px), y(py), rotation(0) {
        rect = { static_cast<int>(x), static_cast<int>(y), w, h };
    }

    // Hàm render đối tượng với góc xoay
    virtual void render(SDL_Renderer* renderer) {
        rect.x = static_cast<int>(x);
        rect.y = static_cast<int>(y);

        // Sử dụng SDL_RenderCopyEx để vẽ với góc xoay
        SDL_RenderCopyEx(renderer, texture, nullptr, &rect, rotation, nullptr, SDL_FLIP_NONE);
    }

    // Hàm trả về hình chữ nhật chứa đối tượng
    virtual SDL_Rect getRect() const {
        return SDL_Rect{
            static_cast<int>(x),
            static_cast<int>(y),
            rect.w,
            rect.h
        };
    }
};
