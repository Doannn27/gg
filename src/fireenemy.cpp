#include "fireenemy.h"
#include "function.h"
#include "globals.h"

FireEnemy::FireEnemy(SDL_Texture* tex, float x, float y)
    : GameObject(tex, x, y, ENEMY_WIDTH, ENEMY_HEIGHT), hp(100)
{
    lastFireTime = SDL_GetTicks();
}

void FireEnemy::moveToward(const Player& player) {
    float dx = player.x - this->x;
    float dy = player.y - this->y;
    float angle = atan2(dy, dx);

    float speed = 1.5f;
    this->x += speed * cos(angle);
    this->y += speed * sin(angle);

    this->rotation = angle * 180.0f / M_PI;
}

void FireEnemy::updateAndShoot(std::vector<Bullet>& enemyBullets, SDL_Texture* bulletTex, const Player& player) {
    Uint32 now = SDL_GetTicks();
    if (now - lastFireTime >= fireInterval) {
        SDL_Rect r = getRect();
        float startX = r.x + r.w / 2.0f;
        float startY = r.y + r.h / 2.0f;
        float px = player.x + PLAYER_WIDTH / 2;
        float py = player.y + PLAYER_HEIGHT / 2;

        enemyBullets.emplace_back(bulletTex, startX, startY, px, py); // bắn về phía player
        lastFireTime = now;
    }
}
