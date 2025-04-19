#include "boss.h"
#include "function.h"
#include "globals.h"

Boss::Boss(SDL_Texture* tex, float x, float y)
    : GameObject(tex, x, y, 120, 120), hp(300), lastShootTime(0) {
}

void Boss::update(float playerX, float playerY) {
    // Di chuyển zigzag hoặc lượn sóng
    static float t = 0.0f;
    t += 0.05f;
    y += 0.5f;
    x += sin(t) * 2.0f;
    // Tính góc hướng về player
    float centerX = x + rect.w / 2;
    float centerY = y + rect.h / 2;
    float dx = playerX - centerX;
    float dy = playerY - centerY;
    angle = atan2(dy, dx) * 180.0 / M_PI;
}

void Boss::shoot(std::vector<Bullet>& bullets, SDL_Texture* bulletTex, Player& player) {
    Uint32 now = SDL_GetTicks();
    if (now - lastShootTime >= 1000) {
        lastShootTime = now;

        float cx = x + rect.w / 2;
        float cy = y + rect.h / 2;

        for (int angle = -45; angle <= 45; angle += 15) {
            float rad = atan2(player.y - cy, player.x - cx) + angle * M_PI / 180.0f;
            float dx = cx + cos(rad) * 50;
            float dy = cy + sin(rad) * 50;
            bullets.emplace_back(bulletTex, cx, cy, dx, dy);
        }
    }
}

void Boss::render(SDL_Renderer* renderer) {
    SDL_Rect dstRect = { static_cast<int>(x), static_cast<int>(y), rect.w, rect.h };
    SDL_Point center = { rect.w / 2, rect.h / 2 };
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, &center, SDL_FLIP_NONE);
}

