#pragma once
#include "function.h"
#include "gameobject.h"
#include "player.h"
#include "bullet.h"

class FireEnemy : public GameObject {
public:
    int hp;
    Uint32 lastFireTime;
    Uint32 fireInterval = 1000; // Bắn mỗi 1 giây

    FireEnemy(SDL_Texture* tex, float x, float y);

    void moveToward(const Player& player);
    void updateAndShoot(vector<Bullet>& enemyBullets, SDL_Texture* bulletTex, const Player& player);
};
