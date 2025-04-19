#include "enemy.h"
#include "function.h"
#include "globals.h"

Enemy::Enemy(SDL_Texture* tex, float px, float py)
    : GameObject(tex, px, py, ENEMY_WIDTH, ENEMY_HEIGHT), hp(100) {}

void Enemy::moveToward(Player& player) {
    float deltaX = player.x - this->x;
    float deltaY = player.y - this->y;

    float angle = atan2(deltaY, deltaX);
    float speed = 2.0f;

    this->x += speed * cos(angle);
    this->y += speed * sin(angle);

    this->rotation = angle * 180.0f / M_PI;  // Chuyển đổi từ radian sang độ
}

SDL_Rect Enemy::getRect() const {
    // Trả về một hình chữ nhật chứa vị trí và kích thước của kẻ địch
    return GameObject::getRect(); 
}
