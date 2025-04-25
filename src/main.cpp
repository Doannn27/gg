#include "function.h"
#include "gameobject.h"
#include "player.h"
#include "enemy.h"
#include "fireenemy.h"
#include "boss.h"
#include "bullet.h"
#include "utils.h"
#include "globals.h"

const char* PLAYER_IMAGE = "D:/SDLgame/SDLgame/assets/player/player.png";
const char* ENEMY_IMAGE = "D:/SDLgame/SDLgame/assets/enemy/ship4.png";
const char* BULLET_IMAGE = "D:/SDLgame/SDLgame/assets/player/bullet.png";
const char* BACKGROUND_IMAGE = "D:/SDLgame/SDLgame/assets/background.jpg";
const char* SHOOT_SOUND = "D:/SDLgame/SDLgame/assets/shoot.mp3";
const char* HIT_SOUND = "D:/SDLgame/SDLgame/assets/hit.mp3";
const char* BACKGROUND_MUSIC = "D:/SDLgame/SDLgame/assets/space.mp3";
const char* FONT_PATH = "D:/SDLgame/SDLgame/assets/font/arial.ttf";
const char* FIRE_ENEMY_IMAGE = "D:/SDLgame/SDLgame/assets/enemy/ship_9.png";
const char* BOSS_IMAGE = "D:/SDLgame/SDLgame/assets/enemy/boss.png";
const char* MENU_IMAGE = "D:/SDLgame/SDLgame/assets/menu.jpg";
const char* GAME_MUSIC = "D:/SDLgame/SDLgame/assets/music.mp3";

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void renderMenu(SDL_Renderer* renderer, TTF_Font* font, int windowWidth, int windowHeight) {
    SDL_Color white = { 255, 255, 255 };
    // Tạo màn hình nền
    SDL_RenderClear(renderer);
    // Hiển thị tiêu đề
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Try to Win", white);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleSurface->w / 2, SCREEN_HEIGHT / 4, titleSurface->w, titleSurface->h };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    // Hiển thị nút Start
    SDL_Surface* startSurface = TTF_RenderText_Solid(font, "Start Game", white);
    SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_Rect startRect = { windowWidth / 2 - startSurface->w / 2, windowHeight / 2, startSurface->w, startSurface->h };
    SDL_RenderCopy(renderer, startTexture, NULL, &startRect);
    SDL_FreeSurface(startSurface);
    SDL_DestroyTexture(startTexture);
    // Hiển thị nút Exit
    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Exit", white);
    SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_Rect exitRect = { SCREEN_WIDTH / 2 - exitSurface->w / 2, SCREEN_HEIGHT / 2 + 50, exitSurface->w, exitSurface->h };
    // Lưu highscore
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, "High Scores", white);
    SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    SDL_Rect highScoreRect = { SCREEN_WIDTH / 2 - highScoreSurface->w / 2, SCREEN_HEIGHT / 2 + 100, highScoreSurface->w, highScoreSurface->h };
    
    SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreRect);
    SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
    SDL_FreeSurface(exitSurface);
    SDL_DestroyTexture(exitTexture);

    SDL_RenderPresent(renderer);
}

void renderTime(SDL_Renderer* renderer, TTF_Font* font, Uint32 elapsedTime, int windowWidth) {
    SDL_Color white = { 255, 255, 255 };
    // Chuyển thời gian thành chuỗi
    int seconds = (elapsedTime / 1000) % 60;
    int minutes = (elapsedTime / 60000) % 60;
    int hours = (elapsedTime / 3600000);
    string timeStr = "Time: " + to_string(hours) + ":" + (minutes < 10 ? "0" : "") + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds);
    // Tạo và hiển thị thời gian
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, timeStr.c_str(), white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { windowWidth - textSurface->w - 10, 10, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void renderPauseMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = { 255, 255, 255 };
    // Tạo màn hình nền cho menu pause
    SDL_RenderClear(renderer);
    // Hiển thị tiêu đề
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Pause Menu", white);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleSurface->w / 2, SCREEN_HEIGHT / 4, titleSurface->w, titleSurface->h };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    // Hiển thị nút "Tiếp tục"
    SDL_Surface* resumeSurface = TTF_RenderText_Solid(font, "Continue", white);
    SDL_Texture* resumeTexture = SDL_CreateTextureFromSurface(renderer, resumeSurface);
    SDL_Rect resumeRect = { SCREEN_WIDTH / 2 - resumeSurface->w / 2, SCREEN_HEIGHT / 2, resumeSurface->w, resumeSurface->h };
    SDL_RenderCopy(renderer, resumeTexture, NULL, &resumeRect);
    SDL_FreeSurface(resumeSurface);
    SDL_DestroyTexture(resumeTexture);
    // Hiển thị nút "Trở về màn hình ban đầu"
    SDL_Surface* exitMenuSurface = TTF_RenderText_Solid(font, "Back to Main Menu", white);
    SDL_Texture* exitMenuTexture = SDL_CreateTextureFromSurface(renderer, exitMenuSurface);
    SDL_Rect exitMenuRect = { SCREEN_WIDTH / 2 - exitMenuSurface->w / 2, SCREEN_HEIGHT / 2 + 50, exitMenuSurface->w, exitMenuSurface->h };
    SDL_RenderCopy(renderer, exitMenuTexture, NULL, &exitMenuRect);
    SDL_FreeSurface(exitMenuSurface);
    SDL_DestroyTexture(exitMenuTexture);
    // Hiển thị nút "Display"
    SDL_Surface* displaySurface = TTF_RenderText_Solid(font, "Display", white);
    SDL_Texture* displayTexture = SDL_CreateTextureFromSurface(renderer, displaySurface);
    SDL_Rect displayRect = { SCREEN_WIDTH / 2 - displaySurface->w / 2, SCREEN_HEIGHT / 2 + 100, displaySurface->w, displaySurface->h };
    SDL_RenderCopy(renderer, displayTexture, NULL, &displayRect);
    SDL_FreeSurface(displaySurface);
    SDL_DestroyTexture(displayTexture);
    SDL_RenderPresent(renderer);
}

void renderAudioSettings(SDL_Renderer* renderer, TTF_Font* font, Mix_Music* music) {
    SDL_Color white = { 255, 255, 255 };
    SDL_RenderClear(renderer);
    // Hiển thị tiêu đề
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Audio Settings", white);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleSurface->w / 2, SCREEN_HEIGHT / 4, titleSurface->w, titleSurface->h };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    // Hiển thị nút tắt hoặc bật nhạc
    string audioStatus = Mix_PlayingMusic() ? "Music: ON" : "Music: OFF";
    SDL_Surface* audioSurface = TTF_RenderText_Solid(font, audioStatus.c_str(), white);
    SDL_Texture* audioTexture = SDL_CreateTextureFromSurface(renderer, audioSurface);
    SDL_Rect audioRect = { SCREEN_WIDTH / 2 - audioSurface->w / 2, SCREEN_HEIGHT / 2, audioSurface->w, audioSurface->h };
    SDL_RenderCopy(renderer, audioTexture, NULL, &audioRect);
    SDL_FreeSurface(audioSurface);
    SDL_DestroyTexture(audioTexture);
    // Hiển thị nút điều chỉnh âm lượng
    int volume = Mix_VolumeMusic(-1);
    string volumeText = "Volume: " + to_string(volume);
    SDL_Surface* volumeSurface = TTF_RenderText_Solid(font, volumeText.c_str(), white);
    SDL_Texture* volumeTexture = SDL_CreateTextureFromSurface(renderer, volumeSurface);
    SDL_Rect volumeRect = { SCREEN_WIDTH / 2 - volumeSurface->w / 2, SCREEN_HEIGHT / 2 + 50, volumeSurface->w, volumeSurface->h };
    SDL_RenderCopy(renderer, volumeTexture, NULL, &volumeRect);
    SDL_FreeSurface(volumeSurface);
    SDL_DestroyTexture(volumeTexture);
    // Hiển thị nút trở lại
    SDL_Surface* backSurface = TTF_RenderText_Solid(font, "Back", white);
    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
    SDL_Rect backRect = { SCREEN_WIDTH / 2 - backSurface->w / 2, SCREEN_HEIGHT / 2 + 100, backSurface->w, backSurface->h };
    SDL_RenderCopy(renderer, backTexture, NULL, &backRect);
    SDL_FreeSurface(backSurface);
    SDL_DestroyTexture(backTexture);

    SDL_RenderPresent(renderer);
}

void renderGameOverMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = { 255, 255, 255 };
    SDL_RenderClear(renderer);
    // Tiêu đề Game Over
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "GAME OVER", white);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleSurface->w / 2, SCREEN_HEIGHT / 4, titleSurface->w, titleSurface->h };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    // Nút Replay
    SDL_Surface* replaySurface = TTF_RenderText_Solid(font, "Play Again", white);
    SDL_Texture* replayTexture = SDL_CreateTextureFromSurface(renderer, replaySurface);
    SDL_Rect replayRect = { SCREEN_WIDTH / 2 - replaySurface->w / 2, SCREEN_HEIGHT / 2, replaySurface->w, replaySurface->h };
    SDL_RenderCopy(renderer, replayTexture, NULL, &replayRect);
    SDL_FreeSurface(replaySurface);
    SDL_DestroyTexture(replayTexture);
    // Nút Exit
    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Exit", white);
    SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_Rect exitRect = { SCREEN_WIDTH / 2 - exitSurface->w / 2, SCREEN_HEIGHT / 2 + 60, exitSurface->w, exitSurface->h };
    SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
    SDL_FreeSurface(exitSurface);
    SDL_DestroyTexture(exitTexture);

    SDL_RenderPresent(renderer);
}

void renderLoadingScreen(SDL_Renderer* renderer) {
    SDL_Texture* loadingTex = loadTexture(MENU_IMAGE, renderer);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, loadingTex, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(loadingTex);
    SDL_Delay(2000); // Hiển thị trong 2 giây
}

int loadHighScore(const string& filename) {
    ifstream file(filename);
    int score = 0;
    if (file.is_open()) {
        file >> score;
        file.close();
    }
    return score;
}

void saveHighScore(const string& filename, int score) {
    ofstream file(filename);
    if (file.is_open()) {
        file << score;
        file.close();
    }
}

void renderHighScoreScreen(SDL_Renderer* renderer, TTF_Font* font, int highScore) {
    SDL_Color white = { 255, 255, 255 };
    SDL_RenderClear(renderer);

    // Tiêu đề
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "HIGH SCORES", white);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleSurface->w / 2, SCREEN_HEIGHT / 4, titleSurface->w, titleSurface->h };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    // Hiển thị điểm cao nhất
    string hsText = "Highest Score: " + to_string(highScore);
    SDL_Surface* hsSurface = TTF_RenderText_Solid(font, hsText.c_str(), white);
    SDL_Texture* hsTexture = SDL_CreateTextureFromSurface(renderer, hsSurface);
    SDL_Rect hsRect = { SCREEN_WIDTH / 2 - hsSurface->w / 2, SCREEN_HEIGHT / 2, hsSurface->w, hsSurface->h };
    SDL_RenderCopy(renderer, hsTexture, NULL, &hsRect);
    SDL_FreeSurface(hsSurface);
    SDL_DestroyTexture(hsTexture);

    // Hiển thị nút "Back"
    SDL_Surface* backSurface = TTF_RenderText_Solid(font, "Back", white);
    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
    SDL_Rect backRect = { SCREEN_WIDTH / 2 - backSurface->w / 2, SCREEN_HEIGHT / 2 + 80, backSurface->w, backSurface->h };
    SDL_RenderCopy(renderer, backTexture, NULL, &backRect);
    SDL_FreeSurface(backSurface);
    SDL_DestroyTexture(backTexture);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    bool keepPlaying = true;

    while (keepPlaying) {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        IMG_Init(IMG_INIT_PNG);
        TTF_Init();
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        SDL_Texture* background = loadTexture(BACKGROUND_IMAGE, renderer);
        SDL_Texture* playerTex = loadTexture(PLAYER_IMAGE, renderer);
        SDL_Texture* enemyTex = loadTexture(ENEMY_IMAGE, renderer);
        SDL_Texture* fireEnemyTex = loadTexture(FIRE_ENEMY_IMAGE, renderer);
        SDL_Texture* bossTex = loadTexture(BOSS_IMAGE, renderer);
        SDL_Texture* bulletTex = loadTexture(BULLET_IMAGE, renderer);

        Mix_Chunk* shootSound = Mix_LoadWAV(SHOOT_SOUND);
        Mix_Chunk* hitSound = Mix_LoadWAV(HIT_SOUND);
        Mix_Music* menuMusic = Mix_LoadMUS(BACKGROUND_MUSIC);
        Mix_Music* gameMusic = Mix_LoadMUS(GAME_MUSIC);
        if (!menuMusic) {
            cerr << "[ERROR] Failed to load menu music: " << Mix_GetError() << endl;
        }
        if (!gameMusic) {
            cerr << "[ERROR] Failed to load game music: " << Mix_GetError() << endl;
        }

        TTF_Font* font = TTF_OpenFont(FONT_PATH, 24);
        if (!font) {
            cerr << "[ERROR] TTF_OpenFont failed: " << TTF_GetError() << endl;
            return -1;
        }

        bool showMenu = true;
        SDL_Event e;
        int windowWidth = SCREEN_WIDTH;
        int windowHeight = SCREEN_HEIGHT;

        const string HIGH_SCORE_FILE = "highscore.txt";
        int highScore = loadHighScore(HIGH_SCORE_FILE);

        Uint32 startTime = SDL_GetTicks();  // Thời gian bắt đầu
        Uint32 elapsedTime = 0;  // Thời gian đã trôi qua
        renderLoadingScreen(renderer);  // Hiển thị ảnh chờ trước khi vào menu
        Mix_PlayMusic(menuMusic, -1);

        while (showMenu) {
            renderMenu(renderer, font, windowWidth, windowHeight);

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
                        Mix_HaltMusic();              // Dừng nhạc menu
                        Mix_PlayMusic(gameMusic, -1); // Bắt đầu nhạc game
                    }
                    // Kiểm tra nếu click vào nút Exit
                    else if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 && mouseY >= SCREEN_HEIGHT / 2 + 50 && mouseY <= SCREEN_HEIGHT / 2 + 100) {
                        showMenu = false;  // Thoát trò chơi
                        SDL_Quit();
                        return 0;
                    }

                    else if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 &&
                        mouseY >= SCREEN_HEIGHT / 2 + 100 && mouseY <= SCREEN_HEIGHT / 2 + 150) {
                        bool inHighScore = true;
                        while (inHighScore) {
                            renderHighScoreScreen(renderer, font, highScore);

                            while (SDL_PollEvent(&e)) {
                                if (e.type == SDL_QUIT) {
                                    inHighScore = false;
                                    showMenu = false;
                                }
                                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                                    int mx, my;
                                    SDL_GetMouseState(&mx, &my);
                                    // Nếu nhấn "Back"
                                    if (mx >= SCREEN_WIDTH / 2 - 100 && mx <= SCREEN_WIDTH / 2 + 100 &&
                                        my >= SCREEN_HEIGHT / 2 + 80 && my <= SCREEN_HEIGHT / 2 + 120) {
                                        inHighScore = false;
                                    }
                                }
                            }
                            SDL_Delay(16);
                        }
                    }
                }
            }

            SDL_Delay(16);
        }


        Player player(playerTex);
        Boss* boss = nullptr;
        vector<Enemy> enemies;
        vector<Bullet> bullets;
        vector<FireEnemy> fireEnemies;
        vector<Bullet> enemyBullets;


        Uint32 lastSpawn = SDL_GetTicks();
        int score = 0;

        int killCount = 0;

        bool running = true;

        int ammo = 20;
        bool canShoot = true;
        bool reloading = false;
        Uint32 reloadStartTime = 0;
        Uint32 lastHitTime = 0;     // Thời điểm lần cuối bị trừ máu
        const int HIT_COOLDOWN = 1500; // Khoảng thời gian miễn nhiễm (ms)
        const int RELOAD_TIME = 2000;

        bool isPaused = false;  // Biến lưu trạng thái tạm dừng

        while (running) {
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);

            Uint32 now = SDL_GetTicks();
            if (isPaused) {
                renderPauseMenu(renderer, font);

                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) running = false;

                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                        isPaused = false;  // Quay lại chơi
                    }

                    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);

                        // Kiểm tra nút "Continue"
                        if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 && mouseY >= SCREEN_HEIGHT / 2 && mouseY <= SCREEN_HEIGHT / 2 + 50) {
                            isPaused = false;  // Quay lại chơi
                        }

                        // Kiểm tra nút "Back to Main Menu"
                        else if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 && mouseY >= SCREEN_HEIGHT / 2 + 50 && mouseY <= SCREEN_HEIGHT / 2 + 100) {
                            running = false;  // Quay lại menu chính
                        }

                        // Kiểm tra nút "Display" để vào bảng điều khiển âm thanh
                        else if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 && mouseY >= SCREEN_HEIGHT / 2 + 100 && mouseY <= SCREEN_HEIGHT / 2 + 150) {
                            renderAudioSettings(renderer, font, gameMusic);  // Mở bảng điều chỉnh âm thanh
                        }
                    }
                }

                SDL_Delay(16);
            }
            else {

                Uint32 currentTime = SDL_GetTicks();  // Lấy thời gian hiện tại
                elapsedTime = currentTime - startTime;  // Tính thời gian đã trôi qua

                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) running = false;

                    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && ammo > 0 && canShoot) {
                        float centerX = player.x + PLAYER_WIDTH / 2;
                        float centerY = player.y + PLAYER_HEIGHT / 2;
                        float dx = mouseX - centerX;
                        float dy = mouseY - centerY;
                        float distance = sqrt(dx * dx + dy * dy);

                        if (distance > 5.0f) {  // Tránh bắn khi chuột quá gần (góc bắn không xác định)
                            float angleRad = atan2(dy, dx);
                            float spawnX = centerX + cos(angleRad) * (PLAYER_HEIGHT / 2);
                            float spawnY = centerY + sin(angleRad) * (PLAYER_HEIGHT / 2);

                            bullets.emplace_back(bulletTex, spawnX, spawnY, mouseX, mouseY);
                            Mix_PlayChannel(-1, shootSound, 0);

                            ammo--;
                            if (ammo == 0) canShoot = false;
                        }
                    }

                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r && !reloading && ammo < 20) {
                        reloading = true;
                        reloadStartTime = SDL_GetTicks();
                        canShoot = false;
                    }

                }

                const Uint8* keys = SDL_GetKeyboardState(NULL);
                player.move(keys, windowWidth, windowHeight);

                if (reloading && SDL_GetTicks() - reloadStartTime >= RELOAD_TIME) {
                    ammo = 20;
                    canShoot = true;
                    reloading = false;
                }

                for (auto& bullet : bullets) bullet.update();
                bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.offScreen(); }), bullets.end());

                for (auto& bullet : enemyBullets) bullet.update();
                enemyBullets.erase(remove_if(enemyBullets.begin(), enemyBullets.end(), [](Bullet& b) {return b.offScreen(); }), enemyBullets.end());

                for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
                    if (checkCollision(&*it, &player) && now - lastHitTime >= HIT_COOLDOWN) {
                        player.hp -= 10;
                        lastHitTime = now;
                        it = enemyBullets.erase(it);

                        if (player.hp <= 0) {
                            running = false;
                            break;
                        }
                    }
                    else {
                        ++it;
                    }
                }

                for (auto& enemy : enemies) enemy.moveToward(player);

                for (auto& fireEnemy : fireEnemies) {
                    fireEnemy.moveToward(player);
                    fireEnemy.updateAndShoot(enemyBullets, bulletTex, player);
                }

                if (boss) {
                    boss->update(player.x + PLAYER_WIDTH / 2, player.y + PLAYER_HEIGHT / 2);
                    boss->shoot(enemyBullets, bulletTex, player);
                }   

                for (auto& enemy : enemies) {
                    if (checkCollision(&enemy, &player) && now - lastHitTime >= HIT_COOLDOWN) {
                        if (boss && checkCollision(boss, &player) && now - lastHitTime >= HIT_COOLDOWN) {
                            player.hp -= 20;
                            lastHitTime = now;
                        }
                        player.hp -= 10;
                        lastHitTime = now;

                        if (player.hp <= 0) {
                            running = false;  // Thoát game khi hết máu
                            break;
                        }
                    }
                }

                for (auto it = bullets.begin(); it != bullets.end(); ) {
                    bool erased = false;

                    // Check vs normal enemies
                    for (auto et = enemies.begin(); et != enemies.end(); ) {
                        if (checkCollision(&*it, &*et)) {
                            Mix_PlayChannel(-1, hitSound, 0);
                            score += 10;
                            killCount++;
                            if (killCount % 20 == 0 && player.hp < 100) {
                                player.hp += 10;
                                if (player.hp > 100) player.hp = 100;
                            }

                            et = enemies.erase(et);
                            it = bullets.erase(it);
                            erased = true;
                            break;
                        }
                        else ++et;
                    }
                    if (erased) continue;

                    // Check vs fire enemies
                    for (auto fet = fireEnemies.begin(); fet != fireEnemies.end(); ) {
                        if (checkCollision(&*it, &*fet)) {
                            Mix_PlayChannel(-1, hitSound, 0);
                            score += 15;
                            killCount++;
                            if (killCount % 20 == 0 && player.hp < 100) {
                                player.hp += 10;
                                if (player.hp > 100) player.hp = 100;
                            }

                            fet = fireEnemies.erase(fet);
                            it = bullets.erase(it);
                            erased = true;
                            break;
                        }
                        else ++fet;
                    }
                    if (erased) continue;

                    // Check vs boss
                    if (boss && checkCollision(&*it, boss)) {
                        boss->hp -= 10;
                        Mix_PlayChannel(-1, hitSound, 0);
                        it = bullets.erase(it);
                        if (boss->hp <= 0) {
                            score += 100;
                            killCount += 1;
                            delete boss;
                            boss = nullptr;
                        }
                        continue;
                    }

                    ++it;
                }

                if (SDL_GetTicks() - lastSpawn > 800) {
                    if (rand() % 3 == 0) {
                        fireEnemies.emplace_back(fireEnemyTex, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
                    }
                    else {
                        enemies.emplace_back(enemyTex, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
                    }
                    lastSpawn = SDL_GetTicks();
                }
                if (killCount >= 60 && boss == nullptr) {
                    if (player.hp < 90) {
                        player.hp += 20;
                    }
                    else {
                        if (player.hp == 90) player.hp += 10;
                    }
                    boss = new Boss(bossTex, SCREEN_WIDTH / 2 - 60, 0);
                }

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);

                renderTime(renderer, font, elapsedTime, windowWidth);  // Hiển thị thời gian

                player.render(renderer, mouseX, mouseY);
                for (auto& enemy : enemies) enemy.render(renderer);
                for (auto& bullet : bullets) bullet.render(renderer);
                for (auto& bullet : enemyBullets) bullet.render(renderer);
                for (auto& fe : fireEnemies) fe.render(renderer);

                if (boss) boss->render(renderer);

                SDL_Color white = { 255, 255, 255 };
                string status = "HP: " + to_string(player.hp) +
                    "  Score: " + to_string(score) +
                    "  Kill: " + to_string(killCount);

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
                    windowWidth - ammoSurface->w - 20,
                    windowHeight - ammoSurface->h - 20,
                    ammoSurface->w,
                    ammoSurface->h
                };

                SDL_RenderCopy(renderer, ammoTexture, NULL, &ammoRect);
                SDL_FreeSurface(ammoSurface);
                SDL_DestroyTexture(ammoTexture);


                SDL_RenderPresent(renderer);
                SDL_Delay(16);
            }
        }

        if (score > highScore) {
            saveHighScore(HIGH_SCORE_FILE, score);
        }

        // Hiện menu gameover
        bool gameOverMenu = true;
        while (gameOverMenu) {
            renderGameOverMenu(renderer, font);

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    gameOverMenu = false;
                }

                // Nhấn ESC để thoát khỏi full màn hình
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                    Uint32 flags = SDL_GetWindowFlags(window);
                    if (flags & SDL_WINDOW_FULLSCREEN || flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                        SDL_SetWindowFullscreen(window, 0); // Thoát full màn hình
                    }
                    else {
                        gameOverMenu = false; // Nếu không phải full thì thoát menu
                    }
                }

                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // Nút "Play Again"
                    if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 &&
                        mouseY >= SCREEN_HEIGHT / 2 && mouseY <= SCREEN_HEIGHT / 2 + 50) {
                        main(argc, argv); // Chơi lại bằng cách gọi lại main
                        return 0;
                    }

                    // Nút "Exit"
                    if (mouseX >= SCREEN_WIDTH / 2 - 100 && mouseX <= SCREEN_WIDTH / 2 + 100 &&
                        mouseY >= SCREEN_HEIGHT / 2 + 60 && mouseY <= SCREEN_HEIGHT / 2 + 110) {
                        gameOverMenu = false;
                    }
                }
            }

            SDL_Delay(16);
        }

        TTF_CloseFont(font);
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(playerTex);
        SDL_DestroyTexture(enemyTex);
        SDL_DestroyTexture(bulletTex);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        if (shootSound) { Mix_FreeChunk(shootSound); shootSound = nullptr; }
        if (hitSound) { Mix_FreeChunk(hitSound); hitSound = nullptr; }
        if (gameMusic) {
            Mix_FreeMusic(gameMusic);
            gameMusic = nullptr;
        }
        if (menuMusic) {
            Mix_FreeMusic(menuMusic);
            menuMusic = nullptr;
        }
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}