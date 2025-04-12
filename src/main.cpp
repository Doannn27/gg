// Game hoàn chỉnh sử dụng SDL2, SDL_image, SDL_mixer, SDL_ttf
#include "function.h"
#include "gameobject.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "utils.h"
#include "constants.h"


// Tên file mặc định, bạn chỉ cần thay đúng tên file ảnh/âm thanh/font của bạn
const char* PLAYER_IMAGE = "D:/SDLgame/SDLgame/assets/player.png";
const char* ENEMY_IMAGE = "D:/SDLgame/SDLgame/assets/enemy.jpg";
const char* BULLET_IMAGE = "D:/SDLgame/SDLgame/assets/bullet.png";
const char* BACKGROUND_IMAGE = "D:/SDLgame/SDLgame/assets/background.jpg";
const char* SHOOT_SOUND = "D:/SDLgame/SDLgame/assets/shoot.mp3";
const char* HIT_SOUND = "D:/SDLgame/SDLgame/assets/hit.mp3";
const char* BACKGROUND_MUSIC = "D:/SDLgame/SDLgame/assets/music.mp3";
const char* FONT_PATH = "D:/SDLgame/SDLgame/assets/arial.ttf";

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* background = loadTexture(BACKGROUND_IMAGE, renderer);
    SDL_Texture* playerTex = loadTexture(PLAYER_IMAGE, renderer);
    SDL_Texture* enemyTex = loadTexture(ENEMY_IMAGE, renderer);
    SDL_Texture* bulletTex = loadTexture(BULLET_IMAGE, renderer);

    Mix_Chunk* shootSound = Mix_LoadWAV(SHOOT_SOUND);
    Mix_Chunk* hitSound = Mix_LoadWAV(HIT_SOUND);
    Mix_Music* music = Mix_LoadMUS(BACKGROUND_MUSIC);

    TTF_Font* font = TTF_OpenFont(FONT_PATH, 24);
    if (!font) {
        std::cerr << "❌ TTF_OpenFont failed: " << TTF_GetError() << std::endl;
        return -1;
    }


    Player player(playerTex);
    vector<Enemy> enemies;
    vector<Bullet> bullets;

    Uint32 lastSpawn = SDL_GetTicks();
    int score = 0;
    Mix_PlayMusic(music, -1);

    bool running = true;
    SDL_Event e;

    int ammo = 10;
    bool canShoot = true;
    bool reloading = false;               // 🔥 NEW
    Uint32 reloadStartTime = 0;           // 🔥 NEW
    const int RELOAD_TIME = 2000;         // 🔥 NEW (2 giây)



    while (running) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;

            // 🔫 BẮN ĐẠN
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && ammo > 0 && canShoot) {
                float centerX = player.x + PLAYER_WIDTH / 2;
                float centerY = player.y + PLAYER_HEIGHT / 2;
                float angleRad = atan2(mouseY - centerY, mouseX - centerX);
                float spawnX = centerX + cos(angleRad) * (PLAYER_HEIGHT / 2);
                float spawnY = centerY + sin(angleRad) * (PLAYER_HEIGHT / 2);

                bullets.emplace_back(bulletTex, spawnX, spawnY, mouseX, mouseY);
                Mix_PlayChannel(-1, shootSound, 0);

                ammo--;                            // 🔥 NEW: trừ đạn sau mỗi lần bắn
                if (ammo == 0) canShoot = false;   // 🔥 NEW: hết đạn thì cấm bắn
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r && !reloading && ammo < 10) {
                reloading = true;                            // 🔥 Bắt đầu reload
                reloadStartTime = SDL_GetTicks();            // 🔥 Ghi lại thời điểm bắt đầu reload
                canShoot = false;                            // 🔥 Cấm bắn trong lúc reload
            }

        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        player.move(keys);

        if (reloading && SDL_GetTicks() - reloadStartTime >= RELOAD_TIME) {
            ammo = 10;
            canShoot = true;
            reloading = false;
        }

        for (auto& bullet : bullets) bullet.update();
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.offScreen(); }), bullets.end());

        for (auto& enemy : enemies) enemy.moveToward(player);

        for (auto it = bullets.begin(); it != bullets.end();) {
            bool hit = false;
            for (auto et = enemies.begin(); et != enemies.end();) {
                if (checkCollision(&*it, &*et)) {
                    Mix_PlayChannel(-1, hitSound, 0);
                    score += 10;
                    et = enemies.erase(et);
                    it = bullets.erase(it);
                    hit = true;
                    break;
                }
                else ++et;
            }
            if (!hit) ++it;
        }

        if (SDL_GetTicks() - lastSpawn > 2000) {
            enemies.emplace_back(enemyTex, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
            lastSpawn = SDL_GetTicks();
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        player.render(renderer, mouseX, mouseY);
        for (auto& enemy : enemies) enemy.render(renderer);
        for (auto& bullet : bullets) bullet.render(renderer);

        SDL_Color white = { 255, 255, 255 };
        string status = "HP: " + to_string(player.hp) +
            "  Score: " + to_string(score);

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, status.c_str(), white);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        string ammoStatus = reloading ? "Reloading..." : "Ammo: " + to_string(ammo);
        SDL_Surface* ammoSurface = TTF_RenderText_Solid(font, ammoStatus.c_str(), white);
        SDL_Texture* ammoTexture = SDL_CreateTextureFromSurface(renderer, ammoSurface);

        SDL_Rect ammoRect = {
            SCREEN_WIDTH - ammoSurface->w - 20,
            SCREEN_HEIGHT - ammoSurface->h - 20,
            ammoSurface->w,
            ammoSurface->h
        };

        SDL_RenderCopy(renderer, ammoTexture, NULL, &ammoRect);
        SDL_FreeSurface(ammoSurface);
        SDL_DestroyTexture(ammoTexture);


        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    Mix_FreeChunk(shootSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeMusic(music);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(enemyTex);
    SDL_DestroyTexture(bulletTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}