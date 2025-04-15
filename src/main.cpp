#include "function.h"
#include "gameobject.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "utils.h"
#include "constants.h"

const char* PLAYER_IMAGE = "D:/SDLgame/SDLgame/assets/player/player.png";
const char* ENEMY_IMAGE = "D:/SDLgame/SDLgame/assets/enemy/ship4.png";
const char* BULLET_IMAGE = "D:/SDLgame/SDLgame/assets/player/bullet.png";
const char* BACKGROUND_IMAGE = "D:/SDLgame/SDLgame/assets/background.jpg";
const char* SHOOT_SOUND = "D:/SDLgame/SDLgame/assets/shoot.mp3";
const char* HIT_SOUND = "D:/SDLgame/SDLgame/assets/hit.mp3";
const char* BACKGROUND_MUSIC = "D:/SDLgame/SDLgame/assets/music.mp3";
const char* FONT_PATH = "D:/SDLgame/SDLgame/assets/font/arial.ttf";

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void renderMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = { 255, 255, 255 };

    // Tạo màn hình nền
    SDL_RenderClear(renderer);

    // Hiển thị tiêu đề
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "My SDL Game", white);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleSurface->w / 2, SCREEN_HEIGHT / 4, titleSurface->w, titleSurface->h };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    // Hiển thị nút Start
    SDL_Surface* startSurface = TTF_RenderText_Solid(font, "Start Game", white);
    SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_Rect startRect = { SCREEN_WIDTH / 2 - startSurface->w / 2, SCREEN_HEIGHT / 2, startSurface->w, startSurface->h };
    SDL_RenderCopy(renderer, startTexture, NULL, &startRect);
    SDL_FreeSurface(startSurface);
    SDL_DestroyTexture(startTexture);

    // Hiển thị nút Exit
    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Exit", white);
    SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_Rect exitRect = { SCREEN_WIDTH / 2 - exitSurface->w / 2, SCREEN_HEIGHT / 2 + 50, exitSurface->w, exitSurface->h };
    SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
    SDL_FreeSurface(exitSurface);
    SDL_DestroyTexture(exitTexture);

    SDL_RenderPresent(renderer);
}

void renderTime(SDL_Renderer* renderer, TTF_Font* font, Uint32 elapsedTime) {
    SDL_Color white = { 255, 255, 255 };

    // Chuyển thời gian thành chuỗi
    int seconds = (elapsedTime / 1000) % 60;
    int minutes = (elapsedTime / 60000) % 60;
    int hours = (elapsedTime / 3600000);

    string timeStr = "Time: " + to_string(hours) + ":" + (minutes < 10 ? "0" : "") + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds);

    // Tạo và hiển thị thời gian
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, timeStr.c_str(), white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { SCREEN_WIDTH - textSurface->w - 10, 10, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
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

    bool showMenu = true;
    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();  // Thời gian bắt đầu
    Uint32 elapsedTime = 0;  // Thời gian đã trôi qua

    while (showMenu) {
        renderMenu(renderer, font);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                showMenu = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Kiểm tra nếu click vào nút Start
                if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 && mouseY >= SCREEN_HEIGHT / 2 && mouseY <= SCREEN_HEIGHT / 2 + 50) {
                    showMenu = false;  // Bắt đầu trò chơi
                }
                // Kiểm tra nếu click vào nút Exit
                else if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 && mouseY >= SCREEN_HEIGHT / 2 + 50 && mouseY <= SCREEN_HEIGHT / 2 + 100) {
                    showMenu = false;  // Thoát trò chơi
                    SDL_Quit();
                    return 0;
                }
            }
        }

        SDL_Delay(16);
    }


    Player player(playerTex);
    vector<Enemy> enemies;
    vector<Bullet> bullets;

    Uint32 lastSpawn = SDL_GetTicks();
    int score = 0;
    Mix_PlayMusic(music, -1);

    bool running = true;

    int ammo = 10;
    bool canShoot = true;
    bool reloading = false;               
    Uint32 reloadStartTime = 0;          
    const int RELOAD_TIME = 2000;         



    while (running) {
        Uint32 currentTime = SDL_GetTicks();  // Lấy thời gian hiện tại
        elapsedTime = currentTime - startTime;  // Tính thời gian đã trôi qua

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && ammo > 0 && canShoot) {
                float centerX = player.x + PLAYER_WIDTH / 2;
                float centerY = player.y + PLAYER_HEIGHT / 2;
                float angleRad = atan2(mouseY - centerY, mouseX - centerX);
                float spawnX = centerX + cos(angleRad) * (PLAYER_HEIGHT / 2);
                float spawnY = centerY + sin(angleRad) * (PLAYER_HEIGHT / 2);

                bullets.emplace_back(bulletTex, spawnX, spawnY, mouseX, mouseY);
                Mix_PlayChannel(-1, shootSound, 0);

                ammo--;                            
                if (ammo == 0) canShoot = false;   
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r && !reloading && ammo < 10) {
                reloading = true;                            
                reloadStartTime = SDL_GetTicks();            
                canShoot = false;                            
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

        if (SDL_GetTicks() - lastSpawn > 1000) {
            // Khởi tạo kẻ địch với các tham số x, y và texture
            enemies.emplace_back(enemyTex, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
            lastSpawn = SDL_GetTicks();
        }


        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        renderTime(renderer, font, elapsedTime);  // Hiển thị thời gian

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