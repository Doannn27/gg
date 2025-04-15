#include "enemy.h"
#include "function.h" // Để sử dụng các hàm toán học như atan2

Enemy::Enemy(SDL_Texture* tex, float px, float py)
    : GameObject(tex, px, py, ENEMY_WIDTH, ENEMY_HEIGHT), hp(100) {  // Gọi constructor của GameObject với đầy đủ tham số
}

void Enemy::moveToward(Player& player) {
    // Tính toán khoảng cách giữa enemy và player
    float deltaX = player.x - this->x;
    float deltaY = player.y - this->y;

    // Tính góc hướng đến người chơi
    float angle = atan2(deltaY, deltaX);

    // Tốc độ di chuyển của enemy
    float speed = 2.0f;

    // Cập nhật vị trí của enemy
    this->x += speed * cos(angle);
    this->y += speed * sin(angle);
}

SDL_Rect Enemy::getRect() const {
    // Trả về một hình chữ nhật chứa vị trí và kích thước của kẻ địch
    return GameObject::getRect();  // Sử dụng phương thức getRect() của GameObject để trả về kích thước và vị trí
}
