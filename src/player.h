#pragma once
#include "gameobject.h"

class Player : public GameObject {
public:
    int hp;
    Player(SDL_Texture* tex);
    void move(const Uint8* keys, int windowWidth, int windowHeight);
    void render(SDL_Renderer* renderer, int mouseX, int mouseY);
};
