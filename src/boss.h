#pragma once
#include "gameobject.h"
#include "player.h"
#include "bullet.h"
#include "function.h"
#include "globals.h"

class Boss : public GameObject {
public:
    double angle = 0.0;
    int hp;
    Uint32 lastShootTime;
    Boss(SDL_Texture* tex, float x, float y);

    void update(float playerX, float playerY);
    void shoot(vector<Bullet>& bullets, SDL_Texture* bulletTex, Player& player);
    void render(SDL_Renderer* renderer) override;
};
