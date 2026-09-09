#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <limits>
#include <algorithm>
#include <cmath>

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 1000;
const int GROUND_Y = 800;
const int PLAYER_WIDTH = 200;
const int PLAYER_HEIGHT = 150;
const int FRAME_COUNT = 8;
const Uint32 FRAME_DURATION = 100;
const int SHAKE_DURATION = 200;
const int SHAKE_OFFSET = 10;
const int WAND_USES_LIMIT = 3;
const int OBSTACLE_SPACING = 1200;
const int COLLECTIBLE_SIZE = 80;
const float FLOAT_AMPLITUDE = 10.0f;
const float FLOAT_SPEED = 0.05f;
const int TOP_OBSTACLE_WIDTH = 300;
const int TOP_OBSTACLE_HEIGHT = 100;
const int TOP_OBSTACLE_Y = 300;
const int TOP_OBSTACLE_SPEED = 5;
const int BOTTOM_OBSTACLES_BEFORE_TOP = 4;
const float JUMP_VELOCITY = -15.0f;
const float DOUBLE_JUMP_VELOCITY = -20.0f;
const int BOOSTER_SIZE = 60;
const int MAX_OBSTACLES = 20;
const int DRAGON_WIDTH = 300;
const int DRAGON_HEIGHT = 220;
const int DRAGON_SPEED = 3;
const int DRAGON_STOP_DISTANCE = 300;
const Uint32 WAND_RESPONSE_TIME = 2000;
const int DIAGONAL_SPEED = 5;
const int FALL_SPEED = 5;
const int GOLDEN_EGG_SIZE = 100;
const int GOLDEN_EGG_DISTANCE = 200;
const int PLAYER_AUTO_SPEED = 5;
const Uint32 CONGRATS_DURATION = 3000;
const Uint32 LIGHTNING_DURATION = 500;
const int LIGHTNING_SPEED = 10;


// responsible for rendering text
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int wrapWidth = 0) {
    if (!font) {
        std::cerr << "Font is null in renderText" << std::endl;
        return nullptr;
    }
    SDL_Surface* surface = wrapWidth ? TTF_RenderText_Blended_Wrapped(font, text.c_str(), {0, 0, 0, 255}, wrapWidth)
                                    : TTF_RenderText_Solid(font, text.c_str(), {0, 0, 0, 255});
    if (!surface) {
        std::cerr << "TTF_RenderText failed: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }
    return texture;
}


// load images from disk and covert them into texture
SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        std::cerr << "Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
    } else {
        std::cout << "Successfully loaded texture: " << path << std::endl;
    }
    return texture;
}


//loads all short sound clips (wav)
Mix_Chunk* loadSound(const std::string& path) {
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (!sound) {
        std::cerr << "Failed to load sound: " << path << " - " << Mix_GetError() << std::endl;
    } else {
        std::cout << "Successfully loaded sound: " << path << std::endl;
    }
    return sound;
}


//loads background music(.ogg)
Mix_Music* loadMusic(const std::string& path) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << path << " - " << Mix_GetError() << std::endl;
    } else {
        std::cout << "Successfully loaded music: " << path << std::endl;
    }
    return music;
}


//checks whether two game objects collide by comparing their SDL_Rect positions
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    SDL_Rect intersection;
    if (!SDL_IntersectRect(&a, &b, &intersection)) return false;
    int areaA = a.w * a.h, areaB = b.w * b.h;
    int intersectionArea = intersection.w * intersection.h;
    return (intersectionArea / static_cast<float>(std::min(areaA, areaB))) * 100.0f >= 50.0f;
}



//  reads the high score from a file
int loadHighScore(const std::string& filename) {
    std::ifstream file(filename);
    int hs = 0;
    if (file >> hs) return hs;
    return 0;
}


//  saves the player's score to a text file
void saveHighScore(const std::string& filename, int score) {
    std::ofstream file(filename);
    if (file) file << score;
    else std::cerr << "Failed to save high score to " << filename << std::endl;
}


//loads a previously saved level completion flag from a file
bool loadLevelCompletion(const std::string& filename) {
    std::ifstream file(filename);
    bool completed = false;
    if (file >> completed) return completed;
    return false;
}



// saves whether a level is completed or not 
void saveLevelCompletion(const std::string& filename, bool completed) {
    std::ofstream file(filename);
    if (file) file << completed;
    else std::cerr << "Failed to save level completion to " << filename << std::endl;
}

int main() {

// the random number generator and initialize subsystems
    srand(time(0));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 || IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG || TTF_Init() < 0 || Mix_Init(MIX_INIT_OGG) == 0) {
        std::cerr << "Initialization failed: SDL(" << SDL_GetError() << "), IMG(" << IMG_GetError() << "), TTF(" << TTF_GetError() << "), Mix(" << Mix_GetError() << ")" << std::endl;
        return 1;
    }
    
    
 // intialise audio system
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    
    
// creates game window
    SDL_Window* window = SDL_CreateWindow("HP Runner Enhanced", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 36);
    if (!window || !renderer || !font) {
        std::cerr << "Failed to create window, renderer, or font" << std::endl;
        if (font) TTF_CloseFont(font);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    
    
// load music and sound effects
    Mix_Chunk* collectibleSound = loadSound("assets/collectible.wav");
    Mix_Chunk* boosterSound = loadSound("assets/booster.wav");
    Mix_Chunk* buttonClickSound = loadSound("assets/button_click.wav");
    Mix_Chunk* dragonRoarSound = loadSound("assets/dragon_roar.wav");
    Mix_Chunk* dragonDeadSound = loadSound("assets/dragon_dead.wav");
    Mix_Music* introMusic = loadMusic("assets/intro_music.ogg");
    Mix_Music* storyMusic = loadMusic("assets/story_music.ogg");
    Mix_Music* gameMusic = loadMusic("assets/game_music.ogg");
    if (!collectibleSound || !boosterSound || !buttonClickSound || !dragonRoarSound || !dragonDeadSound || !introMusic || !storyMusic || !gameMusic) {
        std::cerr << "Failed to load sound effects or music" << std::endl;
        if (collectibleSound) Mix_FreeChunk(collectibleSound);
        if (boosterSound) Mix_FreeChunk(boosterSound);
        if (buttonClickSound) Mix_FreeChunk(buttonClickSound);
        if (dragonRoarSound) Mix_FreeChunk(dragonRoarSound);
        if (dragonDeadSound) Mix_FreeChunk(dragonDeadSound);
        if (introMusic) Mix_FreeMusic(introMusic);
        if (storyMusic) Mix_FreeMusic(storyMusic);
        if (gameMusic) Mix_FreeMusic(gameMusic);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface* tempSurface = IMG_Load("assets/sprite.png");
    if (!tempSurface) {
        std::cerr << "Failed to load sprite sheet: " << IMG_GetError() << std::endl;
        Mix_FreeChunk(collectibleSound);
        Mix_FreeChunk(boosterSound);
        Mix_FreeChunk(buttonClickSound);
        Mix_FreeChunk(dragonRoarSound);
        Mix_FreeChunk(dragonDeadSound);
        Mix_FreeMusic(introMusic);
        Mix_FreeMusic(storyMusic);
        Mix_FreeMusic(gameMusic);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Texture* spriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    int frameWidth = tempSurface->w / FRAME_COUNT, frameHeight = tempSurface->h;
    SDL_FreeSurface(tempSurface);

    std::vector<SDL_Texture*> dragonFrames(FRAME_COUNT);
    for (int i = 0; i < FRAME_COUNT; ++i) {
        dragonFrames[i] = loadTexture(renderer, "assets/drag" + std::to_string(i + 1) + ".png");
        if (!dragonFrames[i]) {
            for (auto tex : dragonFrames) if (tex) SDL_DestroyTexture(tex);
            SDL_DestroyTexture(spriteSheet);
            Mix_FreeChunk(collectibleSound);
            Mix_FreeChunk(boosterSound);
            Mix_FreeChunk(buttonClickSound);
            Mix_FreeChunk(dragonRoarSound);
            Mix_FreeChunk(dragonDeadSound);
            Mix_FreeMusic(introMusic);
            Mix_FreeMusic(storyMusic);
            Mix_FreeMusic(gameMusic);
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            Mix_CloseAudio();
            Mix_Quit();
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
            return 1;
        }
    }
    
    
    
    
// load other images
    SDL_Texture* dragonDead[3] = {
        loadTexture(renderer, "assets/dragon_dead1.png"),
        loadTexture(renderer, "assets/dragon_dead2.png"),
        loadTexture(renderer, "assets/dragon_dead3.png")
    };
    SDL_Texture* textures[] = {
        loadTexture(renderer, "assets/golden_egg.png"),
        loadTexture(renderer, "assets/ground.png"),
        loadTexture(renderer, "assets/obstacle_bottom.png"),
        loadTexture(renderer, "assets/collectible.png"),
        loadTexture(renderer, "assets/wand_popup.png"),
        loadTexture(renderer, "assets/booster1.png"),
        loadTexture(renderer, "assets/booster2.png"),
        loadTexture(renderer, "assets/booster3.png"),
        loadTexture(renderer, "assets/level_bg.png"),
        loadTexture(renderer, "assets/level1_button.png"),
        loadTexture(renderer, "assets/level2_button.png"),
        loadTexture(renderer, "assets/level2_locked.png"),
        loadTexture(renderer, "assets/menu_bg.png"),
        loadTexture(renderer, "assets/start_button.png"),
        loadTexture(renderer, "assets/scoreboard_button.png"),
        loadTexture(renderer, "assets/instructions_button.png"),
        loadTexture(renderer, "assets/quit_button.png"),
        loadTexture(renderer, "assets/back_button.png"),
        loadTexture(renderer, "assets/next_button.png"),
        loadTexture(renderer, "assets/prev_button.png"),
        loadTexture(renderer, "assets/wand.png"),
        loadTexture(renderer, "assets/candle.png"),
        loadTexture(renderer, "assets/title_glow.png"),
        loadTexture(renderer, "assets/dumbledore.png"),
        loadTexture(renderer, "assets/buy_button.png"),
        loadTexture(renderer, "assets/later_button.png"),
        loadTexture(renderer, "assets/score_icon.png"),
        loadTexture(renderer, "assets/life_icon.png"),
        loadTexture(renderer, "assets/owl.png"),
        loadTexture(renderer, "assets/wandIcon.png"),
        loadTexture(renderer, "assets/platform.png"),
        loadTexture(renderer, "assets/scroll_box.png"),
        loadTexture(renderer, "assets/bg1.png"),
        loadTexture(renderer, "assets/bg2.png"),
        loadTexture(renderer, "assets/bg3.png"),
        loadTexture(renderer, "assets/new_story_bg.png"),
        loadTexture(renderer, "assets/exit_button.png"),
        loadTexture(renderer, "assets/game_over_window.png"),
        loadTexture(renderer, "assets/play_again_button.png"),
        loadTexture(renderer, "assets/lightning.png")
    };
    
    
    
    
   //free up temporary surface
    for (int i = 0; i < 40; ++i) {
        if (!textures[i]) {
            std::cerr << "Failed to load texture at index " << i << std::endl;
            for (auto tex : dragonFrames) SDL_DestroyTexture(tex);
            for (int j = 0; j < 40; ++j) if (textures[j]) SDL_DestroyTexture(textures[j]);
            for (int j = 0; j < 3; ++j) if (dragonDead[j]) SDL_DestroyTexture(dragonDead[j]);
            SDL_DestroyTexture(spriteSheet);
            Mix_FreeChunk(collectibleSound);
            Mix_FreeChunk(boosterSound);
            Mix_FreeChunk(buttonClickSound);
            Mix_FreeChunk(dragonRoarSound);
            Mix_FreeChunk(dragonDeadSound);
            Mix_FreeMusic(introMusic);
            Mix_FreeMusic(storyMusic);
            Mix_FreeMusic(gameMusic);
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            Mix_CloseAudio();
            Mix_Quit();
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
            return 1;
        }
    }
    
    
    
// load intro images
    std::vector<SDL_Texture*> introFrames(26);
    for (int i = 0; i < 26; ++i) {
        introFrames[i] = loadTexture(renderer, "assets/intro" + std::to_string(i + 1) + ".png");
    }
    
    
    
  // sets up all the core gameplay variable
    SDL_Rect player = {SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, GROUND_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
    float velocityY = 0;
    bool isJumping = false, onPlatform = false, obstaclesStopped = false, dragonStopped = false;
    bool dragonMovingDiagonally = false, isDragonFalling = false, goldenEggVisible = false;
    bool wandOffered = false, wandPurchased = false, waitingForWand = false, waitingForGround = false;
    bool isShaking = false, playerAnimationStopped = false, showLightning = false, lightningHitDragon = false;
    int score = 0, highScore = loadHighScore("assets/highscore.txt"), lives = 3, collisionCount = 0;
    float cameraX = 0, collectibleFloatOffset = 0.0f;
    int currentFrame = 0, dragonFrame = 0, dragonDeadFrame = 0, boosterFrame = 0;
    int dragonX = SCREEN_WIDTH, dragonY = SCREEN_HEIGHT / 2 - DRAGON_HEIGHT / 2;
    int bottomObstaclesPassed = 0, spacePressCount = 0, wandUsesRemaining = WAND_USES_LIMIT;
    Uint32 lastFrameTime = SDL_GetTicks(), lastDragonFrameTime = SDL_GetTicks();
    Uint32 shakeStartTime = 0, boosterAnimTime = SDL_GetTicks(), dragonStopTime = 0;
    Uint32 lastSpacePressTime = 0, introStartTime = SDL_GetTicks(), lightningStartTime = 0;
    Uint32 congratsStartTime = 0, countDownStartTime = 0;
    int countDownValue = 0;
    bool level1Completed = loadLevelCompletion("assets/level1_completion.txt");
    int currentLevel = 1, introFrameIndex = 0, storyIndex = 0;
    int dragonRoarChannel = -1;
    int dragonDeadChannel = -1;
    std::vector<SDL_Rect> bottomObstacles, collectibles, boosters, topObstacles, candles;
    for (int i = 0; i < 20; ++i) {
        candles.push_back({rand() % SCREEN_WIDTH, rand() % (GROUND_Y - 100), 40, 80});
    }
    
    
    std::vector<std::string> storySlides = {
        "STORY:Welcome, young wizard! The Triwizard Tournament returns, an ancient contest of magic and might, brimming with danger and destiny. The Goblet of Fire has chosen the bravest. Will you answer the call? Your path will not be paved with pumpkin pasties. Oh no! Magical traps, tricky creatures, and enchanted platforms await! Time your jumps to land on floating platforms high above.",
        "Three lives remain. One chance at glory beyond the walls of Hogwarts awaits: awaits the champion who dares to reach the Cup! But remember, young wizard, with each challenge comes great risk. Will you claim the Cup or will the tournament claim you?",
        "Instructions: A wizard must try to collect golden balls and boosters as much as possible buying escaping dobbys(jump to avoid him) on the journey. In the middle of the journey , you will be offered to buy wand in exchange of 10 golden balls which might help you later in the journey. Press space(1x/2x)- for single jump(1x),Double jump(2x); Press(w)-for using wand; Press(p)-pause/resume ."
    };
    SDL_Rect goldenEggRect = {0, 0, GOLDEN_EGG_SIZE, GOLDEN_EGG_SIZE};
    SDL_Rect lightningRect = {0, 0, 100, 100};
    

//  resets all core gameplay variables 
    auto resetGame = [&]() {
        player = {SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, GROUND_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
        velocityY = 0; isJumping = false; onPlatform = false; score = 0; collisionCount = 0;
        lives = 3; cameraX = 0; wandOffered = false; wandPurchased = false;
        wandUsesRemaining = WAND_USES_LIMIT; currentFrame = 0; lastFrameTime = SDL_GetTicks();
        bottomObstaclesPassed = 0; obstaclesStopped = false; waitingForGround = false;
        dragonX = SCREEN_WIDTH; dragonY = SCREEN_HEIGHT / 2 - DRAGON_HEIGHT / 2;
        dragonFrame = 0; lastDragonFrameTime = SDL_GetTicks(); dragonStopped = false;
        dragonMovingDiagonally = false; isDragonFalling = false; dragonDeadFrame = 0;
        goldenEggVisible = false; shakeStartTime = 0; collectibleFloatOffset = 0.0f;
        boosterFrame = 0; boosterAnimTime = SDL_GetTicks(); isShaking = false;
        playerAnimationStopped = false; dragonRoarChannel = -1; dragonDeadChannel = -1;
        showLightning = false; lightningHitDragon = false;
        bottomObstacles.clear(); collectibles.clear(); boosters.clear(); topObstacles.clear();
        for (int i = 0; i < 10; ++i) {
            int baseX = SCREEN_WIDTH + i * OBSTACLE_SPACING;
            bottomObstacles.push_back({baseX, GROUND_Y - 70, 70, 70});
            bottomObstacles.push_back({baseX + 450, GROUND_Y - 70, 70, 70});
            collectibles.push_back({baseX + 2 * 450 + 150, GROUND_Y - COLLECTIBLE_SIZE, COLLECTIBLE_SIZE, COLLECTIBLE_SIZE});
            if (i >= BOTTOM_OBSTACLES_BEFORE_TOP && topObstacles.size() < 3) {
                int topObsX = baseX + 2 * 450 + 150;
                topObstacles.push_back({topObsX, TOP_OBSTACLE_Y, TOP_OBSTACLE_WIDTH, TOP_OBSTACLE_HEIGHT});
                boosters.push_back({topObsX + (TOP_OBSTACLE_WIDTH - BOOSTER_SIZE) / 2, TOP_OBSTACLE_Y - BOOSTER_SIZE, BOOSTER_SIZE, BOOSTER_SIZE});
            }
        }
    };
    
    
    
// wand usage (obstacle clearing and dragon attacking)
    auto useWand = [&]() {
        if (wandPurchased && wandUsesRemaining > 0 && !wandOffered) {
            SDL_Rect* closestObs = nullptr; int minDist = std::numeric_limits<int>::max();
            for (auto& obs : bottomObstacles) if (obs.x > player.x && obs.x - player.x < minDist) minDist = obs.x - player.x, closestObs = &obs;
            for (auto& obs : topObstacles) if (obs.x > player.x && obs.x - player.x < minDist) minDist = obs.x - player.x, closestObs = &obs;
            if (closestObs) closestObs->x = -200, wandUsesRemaining--;
            if (waitingForWand && dragonStopped) {
                waitingForWand = false; wandUsesRemaining--;
                showLightning = true; lightningHitDragon = false;
                lightningStartTime = SDL_GetTicks();
                int playerCenterX = player.x + PLAYER_WIDTH / 2;
                int playerCenterY = player.y + PLAYER_HEIGHT / 2;
                lightningRect = {playerCenterX - lightningRect.w / 2, playerCenterY - lightningRect.h / 2, lightningRect.w, lightningRect.h};
                if (dragonRoarChannel != -1) {
                    Mix_HaltChannel(dragonRoarChannel);
                    dragonRoarChannel = -1;
                }
            }
        }
    };

    bool running = true, keyPressed = false;
    enum GameState { INTRO, STORY, MENU, LEVEL_SELECT, SCOREBOARD, COUNTDOWN, PLAYING, PAUSED, GAME_OVER, CONGRATS };
    GameState state = introFrames[0] ? INTRO : MENU;

    // Start intro music
    Mix_PlayMusic(introMusic, -1);

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && !keyPressed) {
                keyPressed = true;
                // enter key
                if (e.key.keysym.sym == SDLK_RETURN && (state == MENU || state == GAME_OVER || state == SCOREBOARD || state == CONGRATS)) state = LEVEL_SELECT;
                // pause key
                else if (e.key.keysym.sym == SDLK_p && state == PLAYING && !wandOffered) state = PAUSED;
                else if (e.key.keysym.sym == SDLK_p && state == PAUSED) {
                    state = PLAYING;
                    if (!obstaclesStopped) Mix_PlayMusic(gameMusic, -1);
                }
                // wand key
                else if (e.key.keysym.sym == SDLK_w && state == PLAYING && wandPurchased && !wandOffered) useWand();
                // single jump double jump
                else if (e.key.keysym.sym == SDLK_SPACE && state == PLAYING && !wandOffered) {
                    Uint32 currentTime = SDL_GetTicks();
                    if (!isJumping || (spacePressCount == 1 && currentTime - lastSpacePressTime <= 300)) {
                        velocityY = (spacePressCount == 1) ? DOUBLE_JUMP_VELOCITY : JUMP_VELOCITY;
                        isJumping = true; onPlatform = false; spacePressCount = (spacePressCount == 1) ? 0 : 1;
                        lastSpacePressTime = currentTime;
                    }
                }
                 //story slides logic
                else if (state == STORY) {
                    if (e.key.keysym.sym == SDLK_RIGHT) storyIndex = std::min(storyIndex + 1, (int)storySlides.size() - 1);
                    else if (e.key.keysym.sym == SDLK_LEFT) storyIndex = std::max(storyIndex - 1, 0);
                    else if (e.key.keysym.sym == SDLK_s) {
                        Mix_HaltMusic();
                        Mix_PlayMusic(introMusic, -1);
                        state = MENU, storyIndex = 0;
                    }
                }
            } 
            
            
            
            // handles mouse clicks on the menu
            else if (e.type == SDL_KEYUP) keyPressed = false;
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx, my; SDL_GetMouseState(&mx, &my); SDL_Point mouse = {mx, my};
                if (state == MENU) {
                    SDL_Rect startBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.3), 300, 90};
                    SDL_Rect scoreboardBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
                    SDL_Rect instrBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5), 300, 90};
                    SDL_Rect quitBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.6), 300, 90};
                    if (SDL_PointInRect(&mouse, &startBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        state = LEVEL_SELECT;
                    }
                    else if (SDL_PointInRect(&mouse, &scoreboardBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        state = SCOREBOARD;
                    }
                    else if (SDL_PointInRect(&mouse, &instrBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        Mix_HaltMusic();
                        Mix_PlayMusic(storyMusic, -1);
                        state = STORY, storyIndex = 0;
                    }
                    else if (SDL_PointInRect(&mouse, &quitBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        running = false;
                    }
                } 
                
                
                 // mouse click on level selection
                else if (state == LEVEL_SELECT) {
                    SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
                    SDL_Rect level2Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5 + 10), 300, 90};
                    const int exitBtnSize = 50;
                    SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
                    if (SDL_PointInRect(&mouse, &level1Btn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        currentLevel = 1; state = COUNTDOWN; countDownStartTime = SDL_GetTicks(); countDownValue = 3; resetGame();
                        Mix_HaltMusic();
                    } else if (SDL_PointInRect(&mouse, &level2Btn) && level1Completed) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        #ifdef _WIN32
                            system("start maze.exe");
                        #else
                            system("./maze &");
                        #endif
                        // Optionally, keep trials running or exit
                        // running = false; // Uncomment to exit trials after launching maze
                    } else if (SDL_PointInRect(&mouse, &exitBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        state = MENU;
                    }
                } 
                
                
                
                // mouse click on story or scoreboard
                else if (state == SCOREBOARD || state == STORY) {
                    int scrollWidth = static_cast<int>(SCREEN_WIDTH * 0.4), scrollHeight = static_cast<int>(SCREEN_HEIGHT * 0.4);
                    SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
                    SDL_Rect backBtn = {scrollRect.x + scrollRect.w - 200, scrollRect.y + scrollRect.h - 100, 180, 80};
                    const int exitBtnSize = 50;
                    SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
                    if (state == STORY) {
                        SDL_Rect nextBtn = {scrollRect.x + scrollRect.w - 200, scrollRect.y + scrollRect.h + 10, 180, 80};
                        SDL_Rect prevBtn = {scrollRect.x + scrollRect.w - 380, scrollRect.y + scrollRect.h + 10, 180, 80};
                        if (SDL_PointInRect(&mouse, &nextBtn)) {
                            Mix_PlayChannel(-1, buttonClickSound, 0);
                            storyIndex++; if (storyIndex >= storySlides.size()) {
                                Mix_HaltMusic();
                                Mix_PlayMusic(introMusic, -1);
                                state = MENU, storyIndex = 0;
                            }
                        } else if (SDL_PointInRect(&mouse, &prevBtn)) {
                            Mix_PlayChannel(-1, buttonClickSound, 0);
                            storyIndex = std::max(storyIndex - 1, 0);
                        }
                    } else if (SDL_PointInRect(&mouse, &backBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        state = MENU;
                    }
                    if (SDL_PointInRect(&mouse, &exitBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        if (state == STORY) {
                            Mix_HaltMusic();
                            Mix_PlayMusic(introMusic, -1);
                        }
                        state = MENU;
                    }
                }
                
                
                
                 // mouse click for wand usage
                else if (state == PLAYING && wandPurchased) {
                    SDL_Rect wandIconRect = {SCREEN_WIDTH - 70, 120, 50, 50};
                    if (SDL_PointInRect(&mouse, &wandIconRect)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        useWand();
                    }
                }
                
                
                
                
                // wand purchase popup  
                else if (wandOffered) {
                    int popupWidth = static_cast<int>(SCREEN_WIDTH * 0.45), popupHeight = static_cast<int>(SCREEN_HEIGHT * 0.45);
                    SDL_Rect popupRect = {SCREEN_WIDTH / 2 - popupWidth / 2, SCREEN_HEIGHT / 2 - popupHeight / 2, popupWidth, popupHeight};
                    SDL_Rect buyBtn = {popupRect.x + (popupRect.w - 360) / 2, popupRect.y + popupRect.h - 90, 180, 80};
                    SDL_Rect laterBtn = {buyBtn.x + 180, buyBtn.y, 180, 80};
                    if (SDL_PointInRect(&mouse, &buyBtn) && score >= 10) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        score -= 10; wandOffered = false; wandPurchased = true;
                    }
                    else if (SDL_PointInRect(&mouse, &laterBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        wandOffered = false; wandPurchased = false;
                    }
                }
                
                
                
                 // game over logic 
                else if (state == GAME_OVER) {
                    SDL_Rect playAgainBtn = {SCREEN_WIDTH / 4 + (SCREEN_WIDTH / 2 - 300) / 2, SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 2 - 90), 300, 90};
                    if (SDL_PointInRect(&mouse, &playAgainBtn)) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                        resetGame();
                        state = COUNTDOWN;
                        countDownStartTime = SDL_GetTicks();
                        countDownValue = 3;
                        Mix_HaltMusic();
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        int shakeOffsetX = 0, shakeOffsetY = 0;
        // applies dynamic camera shake effect
        if (isShaking && SDL_GetTicks() - shakeStartTime < SHAKE_DURATION) {
            shakeOffsetX = (rand() % (SHAKE_OFFSET * 2 + 1)) - SHAKE_OFFSET;
            shakeOffsetY = (rand() % (SHAKE_OFFSET * 2 + 1)) - SHAKE_OFFSET;
        } else isShaking = false;
        
        
        
      // handles intro images sequences
        if (state == INTRO && introFrameIndex < introFrames.size()) {
            if (SDL_GetTicks() - introStartTime >= FRAME_DURATION) {
                introFrameIndex++; introStartTime = SDL_GetTicks();
                if (introFrameIndex >= introFrames.size()) {
                    state = MENU;
                }
            }
            if (introFrames[introFrameIndex]) SDL_RenderCopy(renderer, introFrames[introFrameIndex], nullptr, nullptr);
        }
        
        
        
          // handles story telling everything  
        else if (state == STORY) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, textures[35], nullptr, &bgRect);
            const int scrollWidth = static_cast<int>(SCREEN_WIDTH * 0.4);
            const int scrollHeight = static_cast<int>(SCREEN_HEIGHT * 0.4);
            SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
            SDL_RenderCopy(renderer, textures[31], nullptr, &scrollRect);
            const int dumbledoreHeight = scrollHeight * 1.2;
            const int dumbledoreWidth = static_cast<int>(dumbledoreHeight * 0.5538);
            SDL_Rect dumbledoreRect = {scrollRect.x - dumbledoreWidth + 80, scrollRect.y + 20, dumbledoreWidth, dumbledoreHeight};
            SDL_RenderCopy(renderer, textures[23], nullptr, &dumbledoreRect);
            const int owlHeight = static_cast<int>(SCREEN_HEIGHT * 0.1);
            const int owlWidth = owlHeight;
            SDL_Rect owlRect = {scrollRect.x + scrollRect.w - owlWidth - 20, scrollRect.y + scrollRect.h - owlHeight - 20, owlWidth, owlHeight};
            SDL_RenderCopy(renderer, textures[28], nullptr, &owlRect);
            SDL_Texture* slideText = renderText(renderer, font, storySlides[storyIndex], scrollRect.w - 120);
            if (slideText) {
                SDL_Rect textRect = {scrollRect.x + 60, scrollRect.y + 25, scrollRect.w - 120, scrollRect.h - 50};
                SDL_RenderCopy(renderer, slideText, nullptr, &textRect);
                SDL_DestroyTexture(slideText);
            }
            SDL_Rect nextBtn = {scrollRect.x + scrollRect.w - 200, scrollRect.y + scrollRect.h + 10, 180, 80};
            SDL_Rect prevBtn = {scrollRect.x + scrollRect.w - 380, scrollRect.y + scrollRect.h + 10, 180, 80};
            SDL_RenderCopy(renderer, textures[18], nullptr, &nextBtn);
            SDL_RenderCopy(renderer, textures[19], nullptr, &prevBtn);
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            SDL_Point mouse = {mx, my};
            if (SDL_PointInRect(&mouse, &nextBtn)) {
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                SDL_RenderDrawRect(renderer, &nextBtn);
            }
            if (SDL_PointInRect(&mouse, &prevBtn)) {
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                SDL_RenderDrawRect(renderer, &prevBtn);
            }
            if (textures[20]) {
                SDL_Rect wandCursor = {(SDL_PointInRect(&mouse, &nextBtn) ? nextBtn.x : prevBtn.x) - 40,
                                      (SDL_PointInRect(&mouse, &nextBtn) ? nextBtn.y : prevBtn.y) + 10, 40, 80};
                SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
            }
            SDL_Texture* skipText = renderText(renderer, font, ">>");
            if (skipText) {
                SDL_Rect skipRect = {static_cast<int>(SCREEN_WIDTH * 0.95 - 50), static_cast<int>(SCREEN_HEIGHT * 0.95 - 40), 50, 40};
                SDL_RenderCopy(renderer, skipText, nullptr, &skipRect);
                SDL_DestroyTexture(skipText);
            }
            const int exitBtnSize = 50;
            SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
            SDL_RenderCopy(renderer, textures[36], nullptr, &exitBtn);
            if (SDL_PointInRect(&mouse, &exitBtn)) {
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                SDL_RenderDrawRect(renderer, &exitBtn);
            }
        }
        
        
        
        
        // this block handles menu ,level ,score board
        else if (state == MENU || state == LEVEL_SELECT || state == SCOREBOARD) {
            SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, state == LEVEL_SELECT ? textures[8] : (state == MENU ? textures[12] : textures[12]), nullptr, &bgRect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
            SDL_RenderFillRect(renderer, &bgRect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            for (auto& candle : candles) {
                candle.y -= 1; if (candle.y + candle.h < 0) candle.y = SCREEN_HEIGHT;
                if (state == MENU) SDL_RenderCopy(renderer, textures[21], nullptr, &candle);
            }
            int mx, my; SDL_GetMouseState(&mx, &my); SDL_Point mouse = {mx, my};
            if (state == MENU) {
                SDL_Rect startBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.3), 300, 90};
                SDL_Rect scoreboardBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
                SDL_Rect instrBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5), 300, 90};
                SDL_Rect quitBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.6), 300, 90};
                SDL_RenderCopy(renderer, textures[13], nullptr, &startBtn);
                SDL_RenderCopy(renderer, textures[14], nullptr, &scoreboardBtn);
                SDL_RenderCopy(renderer, textures[15], nullptr, &instrBtn);
                SDL_RenderCopy(renderer, textures[16], nullptr, &quitBtn);
                if (SDL_PointInRect(&mouse, &startBtn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &startBtn);
                if (SDL_PointInRect(&mouse, &scoreboardBtn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &scoreboardBtn);
                if (SDL_PointInRect(&mouse, &instrBtn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &instrBtn);
                if (SDL_PointInRect(&mouse, &quitBtn)) SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255), SDL_RenderDrawRect(renderer, &quitBtn);
                if (textures[20]) {
                    SDL_Rect wandCursor = {0, 0, 40, 80};
                    if (SDL_PointInRect(&mouse, &startBtn)) wandCursor = {startBtn.x - 40, startBtn.y + 10, 40, 80};
                    else if (SDL_PointInRect(&mouse, &scoreboardBtn)) wandCursor = {scoreboardBtn.x - 40, scoreboardBtn.y + 10, 40, 80};
                    else if (SDL_PointInRect(&mouse, &instrBtn)) wandCursor = {instrBtn.x - 40, instrBtn.y + 10, 40, 80};
                    else if (SDL_PointInRect(&mouse, &quitBtn)) wandCursor = {quitBtn.x - 40, quitBtn.y + 10, 40, 80};
                    SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
                }
                SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 250, static_cast<int>(SCREEN_HEIGHT * 0.2 - 50), 500, 100};
                SDL_RenderCopy(renderer, textures[22], nullptr, &titleRect);
            } else if (state == LEVEL_SELECT) {
                SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
                SDL_Rect level2Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5 + 10), 300, 90};
                SDL_RenderCopy(renderer, textures[9], nullptr, &level1Btn);
                SDL_RenderCopy(renderer, level1Completed ? textures[10] : textures[11], nullptr, &level2Btn); // Render unlocked or locked Level 2 button
                if (SDL_PointInRect(&mouse, &level1Btn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &level1Btn);
                if (SDL_PointInRect(&mouse, &level2Btn) && level1Completed) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &level2Btn);
                if (textures[20]) {
                    SDL_Rect wandCursor = {level1Btn.x - 40, level1Btn.y + 10, 40, 80};
                    if (SDL_PointInRect(&mouse, &level2Btn) && level1Completed) wandCursor = {level2Btn.x - 40, level2Btn.y + 10, 40, 80};
                    SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
                }
                SDL_Texture* levelText = renderText(renderer, font, "Select Level");
                if (levelText) {
                    int w, h; TTF_SizeText(font, "Select Level", &w, &h);
                    SDL_Rect textRect = {SCREEN_WIDTH / 2 - w / 2, static_cast<int>(SCREEN_HEIGHT * 0.3 - h / 2), w, h};
                    SDL_RenderCopy(renderer, levelText, nullptr, &textRect);
                    SDL_DestroyTexture(levelText);
                }
                const int exitBtnSize = 50;
                SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
                SDL_RenderCopy(renderer, textures[36], nullptr, &exitBtn);
                if (SDL_PointInRect(&mouse, &exitBtn)) {
                    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                    SDL_RenderDrawRect(renderer, &exitBtn);
                }
            } else if (state == SCOREBOARD) {
                int scrollWidth = static_cast<int>(SCREEN_WIDTH * 0.4), scrollHeight = static_cast<int>(SCREEN_HEIGHT * 0.4);
                SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
                SDL_RenderCopy(renderer, textures[31], nullptr, &scrollRect);
                SDL_Texture* scoreText = renderText(renderer, font, "High Score: " + std::to_string(highScore));
                if (scoreText) {
                    int w, h; TTF_SizeText(font, ("High Score: " + std::to_string(highScore)).c_str(), &w, &h);
                    SDL_Rect textRect = {scrollRect.x + (scrollRect.w - w) / 2, scrollRect.y + scrollRect.h / 2 - h / 2, w, h};
                    SDL_RenderCopy(renderer, scoreText, nullptr, &textRect);
                    SDL_DestroyTexture(scoreText);
                }
                SDL_Rect backBtn = {scrollRect.x + scrollRect.w - 200, scrollRect.y + scrollRect.h - 100, 180, 80};
                SDL_RenderCopy(renderer, textures[17], nullptr, &backBtn);
                if (SDL_PointInRect(&mouse, &backBtn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &backBtn);
                if (textures[20]) {
                    SDL_Rect wandCursor = {backBtn.x - 40, backBtn.y + 10, 40, 80};
                    SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
                }
                const int exitBtnSize = 50;
                SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
                SDL_RenderCopy(renderer, textures[36], nullptr, &exitBtn);
                if (SDL_PointInRect(&mouse, &exitBtn)) {
                    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                    SDL_RenderDrawRect(renderer, &exitBtn);
                }
            }
        } 
        
        
        
        // countdown and game music starts
        else if (state == COUNTDOWN) {
            if (SDL_GetTicks() - countDownStartTime >= 1000) {
                countDownValue--; countDownStartTime = SDL_GetTicks();
                if (countDownValue < 1) {
                    state = PLAYING;
                    Mix_PlayMusic(gameMusic, -1);
                }
            }
            
            
            
            // scrolling bg
            int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
            SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
            
            
             // ground
            SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
            SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
            
            
            //obstacle 
            for (const auto& obs : bottomObstacles) SDL_RenderCopy(renderer, textures[2], nullptr, &obs);
            for (const auto& obs : topObstacles) SDL_RenderCopy(renderer, textures[30], nullptr, &obs);
            
            
             // floating collectibles
            for (const auto& col : collectibles) {
                SDL_Rect floatRect = {col.x, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)), col.w, col.h};
                SDL_RenderCopy(renderer, textures[3], nullptr, &floatRect);
            }
            
            if (SDL_GetTicks() - boosterAnimTime > 150) boosterFrame = (boosterFrame + 1) % 3, boosterAnimTime = SDL_GetTicks();
            for (const auto& boost : boosters) SDL_RenderCopy(renderer, textures[5 + boosterFrame], nullptr, &boost);
            SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
            SDL_Rect drawPlayer = {player.x, player.y, player.h, player.h};
            SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
            SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &overlayRect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_Texture* countDownText = renderText(renderer, font, std::to_string(countDownValue));
            if (countDownText) {
                int w, h; TTF_SizeText(font, std::to_string(countDownValue).c_str(), &w, &h);
                SDL_Rect textRect = {SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h};
                SDL_RenderCopy(renderer, countDownText, nullptr, &textRect);
                SDL_DestroyTexture(countDownText);
            }
        } else if (state == PAUSED) {
            SDL_RenderCopy(renderer, textures[32], nullptr, nullptr);
            SDL_Texture* pauseText = renderText(renderer, font, "Paused - Press P to Resume");
            if (pauseText) {
                SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 25, 500, 50};
                SDL_RenderCopy(renderer, pauseText, nullptr, &pauseRect);
                SDL_DestroyTexture(pauseText);
            }
        } else if (state == PLAYING) {
            if (!wandOffered) {
                if (!obstaclesStopped && bottomObstacles.empty() && !waitingForGround) {
                    if (!isJumping && !onPlatform && player.y == GROUND_Y - PLAYER_HEIGHT) {
                        obstaclesStopped = true;
                        Mix_HaltMusic();
                        if (dragonRoarChannel == -1) {
                            dragonRoarChannel = Mix_PlayChannel(-1, dragonRoarSound, -1);
                        }
                    } else waitingForGround = true;
                }
                velocityY += 0.4f; player.y += static_cast<int>(velocityY);
                if (player.y > GROUND_Y - PLAYER_HEIGHT) {
                    player.y = GROUND_Y - PLAYER_HEIGHT; velocityY = 0; isJumping = false; onPlatform = false; spacePressCount = 0;
                    if (waitingForGround && bottomObstacles.empty()) {
                        obstaclesStopped = true;
                        Mix_HaltMusic();
                        if (dragonRoarChannel == -1) {
                            dragonRoarChannel = Mix_PlayChannel(-1, dragonRoarSound, -1);
                        }
                        waitingForGround = false;
                    }
                }
                bool landedOnPlatform = false;
                for (const auto& obs : topObstacles) {
                    if (velocityY > 0 && player.y + player.h >= obs.y && player.y + player.h <= obs.y + obs.h &&
                        player.x + player.w > obs.x && player.x < obs.x + obs.w) {
                        player.y = obs.y - player.h; velocityY = 0; isJumping = false; onPlatform = true;
                        landedOnPlatform = true; spacePressCount = 0; break;
                    }
                }
                if (onPlatform && !landedOnPlatform) {
                    bool stillOnPlatform = false;
                    for (const auto& obs : topObstacles)
                        if (player.y + player.h == obs.y && player.x + player.w > obs.x && player.x < obs.x + obs.w) stillOnPlatform = true;
                    if (!stillOnPlatform) onPlatform = false, isJumping = true;
                }
                if (!obstaclesStopped && !waitingForGround && !dragonStopped && !bottomObstacles.empty())
                    cameraX += 5;
                std::vector<SDL_Rect> newBottomObstacles;
                for (auto obs : bottomObstacles) {
                    obs.x -= 5;
                    if (checkCollision(player, obs)) {
                        obs.x = -200; collisionCount++; shakeStartTime = SDL_GetTicks(); isShaking = true;
                        if (collisionCount >= 3) {
                            collisionCount = 0; lives--;
                            if (lives <= 0) {
                                state = GAME_OVER;
                                saveHighScore("assets/highscore.txt", std::max(score, highScore));
                                highScore = std::max(score, highScore);
                                if (dragonRoarChannel != -1) {
                                    Mix_HaltChannel(dragonRoarChannel);
                                    dragonRoarChannel = -1;
                                }
                                if (dragonDeadChannel != -1) {
                                    Mix_HaltChannel(dragonDeadChannel);
                                    dragonDeadChannel = -1;
                                }
                            }
                        }
                    }
                    if (obs.x + obs.w >= 0) newBottomObstacles.push_back(obs);
                    else bottomObstaclesPassed++;
                }
                bottomObstacles = std::move(newBottomObstacles);
                if (!bottomObstacles.empty() && bottomObstaclesPassed < MAX_OBSTACLES) {
                    int lastX = bottomObstacles.back().x;
                    if (SCREEN_WIDTH - lastX >= OBSTACLE_SPACING) {
                        int baseX = lastX + OBSTACLE_SPACING;
                        bottomObstacles.push_back({baseX, GROUND_Y - 70, 70, 70});
                        bottomObstacles.push_back({baseX + 450, GROUND_Y - 70, 70, 70});
                        collectibles.push_back({baseX + 2 * 450 + 150, GROUND_Y - COLLECTIBLE_SIZE, COLLECTIBLE_SIZE, COLLECTIBLE_SIZE});
                        if (bottomObstaclesPassed >= BOTTOM_OBSTACLES_BEFORE_TOP && topObstacles.size() < 3) {
                            int topObsX = baseX + 2 * 450 + 150;
                            topObstacles.push_back({topObsX, TOP_OBSTACLE_Y, TOP_OBSTACLE_WIDTH, TOP_OBSTACLE_HEIGHT});
                            boosters.push_back({topObsX + (TOP_OBSTACLE_WIDTH - BOOSTER_SIZE) / 2, TOP_OBSTACLE_Y - BOOSTER_SIZE, BOOSTER_SIZE, BOOSTER_SIZE});
                        }
                    }
                }
                std::vector<SDL_Rect> newTopObstacles;
                for (auto obs : topObstacles) {
                    obs.x -= TOP_OBSTACLE_SPEED;
                    if (obs.x + obs.w >= 0) newTopObstacles.push_back(obs);
                }
                topObstacles = std::move(newTopObstacles);
                std::vector<SDL_Rect> newCollectibles;
                for (auto col : collectibles) {
                    col.x -= 5;
                    SDL_Rect floatRect = {col.x, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)), col.w, col.h};
                    if (checkCollision(player, floatRect)) {
                        col.x = -200;
                        score += 1;
                        Mix_PlayChannel(-1, collectibleSound, 0);
                        wandOffered = (score >= 15 && !wandPurchased);
                    }
                    if (col.x + col.w >= 0) newCollectibles.push_back(col);
                }
                collectibles = std::move(newCollectibles);
                std::vector<SDL_Rect> newBoosters;
                for (auto boost : boosters) {
                    boost.x -= 5;
                    if (checkCollision(player, boost)) {
                        boost.x = -200;
                        score += 3;
                        Mix_PlayChannel(-1, boosterSound, 0);
                    }
                    if (boost.x + boost.w >= 0) newBoosters.push_back(boost);
                }
                boosters = std::move(newBoosters);
                if (obstaclesStopped && goldenEggVisible) {
                    goldenEggRect.x -= 5;
                    SDL_Rect floatEggRect = {goldenEggRect.x + shakeOffsetX, goldenEggRect.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)) + shakeOffsetY, goldenEggRect.w, goldenEggRect.h};
                    if (checkCollision(player, floatEggRect)) {
                        goldenEggRect.x = -200;
                        score += 10;
                        saveLevelCompletion("assets/level1_completion.txt", true);
                        level1Completed = true;
                        if (score > highScore) saveHighScore("assets/highscore.txt", score), highScore = score;
                        state = CONGRATS; congratsStartTime = SDL_GetTicks();
                        if (dragonRoarChannel != -1) {
                            Mix_HaltChannel(dragonRoarChannel);
                            dragonRoarChannel = -1;
                        }
                        if (dragonDeadChannel != -1) {
                            Mix_HaltChannel(dragonDeadChannel);
                            dragonDeadChannel = -1;
                        }
                    }
                }
                collectibleFloatOffset += FLOAT_SPEED;
                if (SDL_GetTicks() - lastFrameTime >= FRAME_DURATION && !playerAnimationStopped) currentFrame = (currentFrame + 1) % FRAME_COUNT, lastFrameTime = SDL_GetTicks();
            }
            
            
            
            // dragon encounter phase
            if (obstaclesStopped) {
                if (SDL_GetTicks() - lastDragonFrameTime >= FRAME_DURATION) {
                    dragonFrame = (dragonFrame + 1) % FRAME_COUNT;
                    lastDragonFrameTime = SDL_GetTicks();
                }
                if (isDragonFalling) {
                    dragonY += FALL_SPEED;
                    if (dragonY >= GROUND_Y - DRAGON_HEIGHT && dragonDeadFrame == 0) {
                        dragonDeadFrame = 1;
                        // stop dead dragon music
                        if (dragonDeadChannel != -1) {
                            Mix_HaltChannel(dragonDeadChannel);
                            dragonDeadChannel = -1;
                        }
                    }
                    if (dragonY >= GROUND_Y - DRAGON_HEIGHT) {
                        dragonY = GROUND_Y - DRAGON_HEIGHT;
                        if (dragonDeadFrame == 1) {
                            dragonDeadFrame = 2;
                        }
                        if (dragonDeadFrame == 2) {
                            dragonDeadFrame = -1;
                            goldenEggRect = {dragonX + DRAGON_WIDTH + GOLDEN_EGG_DISTANCE, GROUND_Y - GOLDEN_EGG_SIZE, GOLDEN_EGG_SIZE, GOLDEN_EGG_SIZE};
                            goldenEggVisible = true;
                        }
                    }
                } 
                
                
                
                // dragon approaching player
                else if (!dragonStopped) {
                    dragonX -= DRAGON_SPEED;
                    if (dragonX <= player.x + DRAGON_STOP_DISTANCE) {
                        dragonStopped = true;
                        dragonStopTime = SDL_GetTicks();
                        waitingForWand = true;
                        playerAnimationStopped = true;
                    }
                }
                
                
                
                // wand reaction timeout
                else if (waitingForWand && !showLightning) {
                    if (SDL_GetTicks() - dragonStopTime >= WAND_RESPONSE_TIME) {
                        waitingForWand = false;
                        dragonStopped = false;
                        dragonMovingDiagonally = true;
                    }
                }
                
                
                
                 //  dragon attack if not used 
                if (dragonMovingDiagonally) {
                    int targetX = player.x + PLAYER_WIDTH / 2 - DRAGON_WIDTH / 2;
                    int targetY = player.y + PLAYER_HEIGHT / 2 - DRAGON_HEIGHT / 2;
                    float dx = targetX - dragonX;
                    float dy = targetY - dragonY;
                    float distance = std::sqrt(dx * dx + dy * dy);
                    if (distance > 0) {
                        dragonX += (dx / distance) * DIAGONAL_SPEED;
                        dragonY += (dy / distance) * DIAGONAL_SPEED;
                    }
                    if (checkCollision({dragonX, dragonY, DRAGON_WIDTH, DRAGON_HEIGHT}, player)) {
                        state = GAME_OVER;
                        saveHighScore("assets/highscore.txt", std::max(score, highScore));
                        highScore = std::max(score, highScore);
                        if (dragonRoarChannel != -1) {
                            Mix_HaltChannel(dragonRoarChannel);
                            dragonRoarChannel = -1;
                        }
                        if (dragonDeadChannel != -1) {
                            Mix_HaltChannel(dragonDeadChannel);
                            dragonDeadChannel = -1;
                        }
                    }
                }
                
                
                
                // lightning towards the dragon
                if (showLightning && SDL_GetTicks() - lightningStartTime < LIGHTNING_DURATION) {
                    int dragonCenterX = dragonX + DRAGON_WIDTH / 2;
                    int dragonCenterY = dragonY + DRAGON_HEIGHT / 2;
                    int playerCenterX = player.x + PLAYER_WIDTH / 2;
                    int playerCenterY = player.y + PLAYER_HEIGHT / 2;
                    float dx = dragonCenterX - lightningRect.x - lightningRect.w / 2;
                    float dy = dragonCenterY - lightningRect.y - lightningRect.h / 2;
                    float distance = std::sqrt(dx * dx + dy * dy);
                    if (distance > 0) {
                        lightningRect.x += static_cast<int>((dx / distance) * LIGHTNING_SPEED);
                        lightningRect.y += static_cast<int>((dy / distance) * LIGHTNING_SPEED);
                    }
                    if (!lightningHitDragon && checkCollision(lightningRect, {dragonX, dragonY, DRAGON_WIDTH, DRAGON_HEIGHT})) {
                        lightningHitDragon = true;
                        isDragonFalling = true;
                        dragonStopped = false;
                        dragonMovingDiagonally = false;
                        dragonDeadFrame = 0;
                        if (dragonDeadChannel == -1) {
                            dragonDeadChannel = Mix_PlayChannel(-1, dragonDeadSound, 0);
                        }
                    }
                } else if (showLightning) {
                    showLightning = false;
                }
            }
            int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
            SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
            SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
            SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
            for (const auto& obs : bottomObstacles) SDL_RenderCopy(renderer, textures[2], nullptr, &obs);
            for (const auto& obs : topObstacles) SDL_RenderCopy(renderer, textures[30], nullptr, &obs);
            for (const auto& col : collectibles) {
                SDL_Rect floatRect = {col.x + shakeOffsetX, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)) + shakeOffsetY, col.w, col.h};
                SDL_RenderCopy(renderer, textures[3], nullptr, &floatRect);
            }
            if (SDL_GetTicks() - boosterAnimTime > 150) boosterFrame = (boosterFrame + 1) % 3, boosterAnimTime = SDL_GetTicks();
            for (const auto& boost : boosters) SDL_RenderCopy(renderer, textures[5 + boosterFrame], nullptr, &boost);
             // floating golden egg
            if (goldenEggVisible) {
                SDL_Rect floatEggRect = {goldenEggRect.x + shakeOffsetX, goldenEggRect.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)) + shakeOffsetY, goldenEggRect.w, goldenEggRect.h};
                SDL_RenderCopy(renderer, textures[0], nullptr, &floatEggRect);
            }
            // drawing player sprite and cutting frames
            SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
            SDL_Rect drawPlayer = {player.x + shakeOffsetX, player.y + shakeOffsetY, player.h, player.h};
            SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
            // if obstacle stop ,dragon appear
            if (obstaclesStopped) {
                SDL_Rect dragonDst = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
                if (isDragonFalling && dragonDeadFrame >= 0) SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
                else if (!isDragonFalling) SDL_RenderCopy(renderer, dragonFrames[dragonFrame], nullptr, &dragonDst);
            }
            // final lightning rendering section
            if (showLightning && SDL_GetTicks() - lightningStartTime < LIGHTNING_DURATION) {
                int dragonCenterX = dragonX + DRAGON_WIDTH / 2;
                int dragonCenterY = dragonY + DRAGON_HEIGHT / 2;
                int playerCenterX = player.x + PLAYER_WIDTH / 2;
                int playerCenterY = player.y + PLAYER_HEIGHT / 2;
                float angle = std::atan2(dragonCenterY - playerCenterY, dragonCenterX - playerCenterX) * 180 / M_PI;
                SDL_Rect lightningDrawRect = {lightningRect.x + shakeOffsetX, lightningRect.y + shakeOffsetY, lightningRect.w, lightningRect.h};
                SDL_RenderCopyEx(renderer, textures[39], nullptr, &lightningDrawRect, angle, nullptr, SDL_FLIP_NONE);
            }
            
            
            
            // wand offered ui renders
            if (wandOffered) {
                int popupWidth = static_cast<int>(SCREEN_WIDTH * 0.45), popupHeight = static_cast<int>(SCREEN_HEIGHT * 0.45);
                SDL_Rect popupRect = {SCREEN_WIDTH / 2 - popupWidth / 2, SCREEN_HEIGHT / 2 - popupHeight / 2, popupWidth, popupHeight};
                SDL_RenderCopy(renderer, textures[4], nullptr, &popupRect);
                SDL_Rect buyBtn = {popupRect.x + (popupRect.w - 360) / 2, popupRect.y + popupRect.h - 90, 180, 80};
                SDL_Rect laterBtn = {buyBtn.x + 180, buyBtn.y, 180, 80};
                SDL_RenderCopy(renderer, textures[24], nullptr, &buyBtn);
                SDL_RenderCopy(renderer, textures[25], nullptr, &laterBtn);
                SDL_Texture* wandText = renderText(renderer, font, "Purchase Wand for 10 points?");
                if (wandText) {
                    int w, h; TTF_SizeText(font, "Purchase Wand for 10 points?", &w, &h);
                    SDL_Rect textRect = {popupRect.x + (popupRect.w - w) / 2, popupRect.y + 20, w, h};
                    SDL_RenderCopy(renderer, wandText, nullptr, &textRect);
                    SDL_DestroyTexture(wandText);
                }
                int mx, my; SDL_GetMouseState(&mx, &my); SDL_Point mouse = {mx, my};
                if (SDL_PointInRect(&mouse, &buyBtn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                if (SDL_PointInRect(&mouse, &laterBtn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                if (textures[20]) {
                    SDL_Rect wandCursor = {(SDL_PointInRect(&mouse, &buyBtn) ? buyBtn.x : laterBtn.x) - 40, (SDL_PointInRect(&mouse, &buyBtn) ? buyBtn.y : laterBtn.y) + 10, 40, 80};
                    SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
                }
            }
            
            
            
            // score box size ,position ,blend ,semi transparent,number,icon
            const int iconSize = 40, gap = 10, boxPadding = 5, boxWidth = 100, boxHeight = 50;
            SDL_Rect scoreBoxRect = {10 + shakeOffsetX, 10 + shakeOffsetY, boxWidth, boxHeight};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &scoreBoxRect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_RenderDrawRect(renderer, &scoreBoxRect);
            SDL_Rect scoreIconRect = {scoreBoxRect.x + boxPadding, scoreBoxRect.y + (boxHeight - iconSize) / 2, iconSize, iconSize};
            SDL_RenderCopy(renderer, textures[26], nullptr, &scoreIconRect);
            SDL_Texture* scoreText = renderText(renderer, font, std::to_string(score));
            if (scoreText) {
                int w, h; TTF_SizeText(font, std::to_string(score).c_str(), &w, &h);
                SDL_Rect scoreTextRect = {scoreIconRect.x + iconSize + gap, scoreBoxRect.y + (boxHeight - h) / 2, w, h};
                SDL_RenderCopy(renderer, scoreText, nullptr, &scoreTextRect);
                SDL_DestroyTexture(scoreText);
            }
            
            
            
            // life box size ,position, blend,semi transparent ,number icon
            SDL_Rect livesBoxRect = {scoreBoxRect.x + boxWidth + gap, scoreBoxRect.y, boxWidth, boxHeight};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &livesBoxRect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &livesBoxRect);
            SDL_Rect lifeIconRect = {livesBoxRect.x + boxPadding, livesBoxRect.y + (boxHeight - iconSize) / 2, iconSize, iconSize};
            SDL_RenderCopy(renderer, textures[27], nullptr, &lifeIconRect);
            SDL_Texture* livesText = renderText(renderer, font, std::to_string(lives));
            if (livesText) {
                int w, h; TTF_SizeText(font, std::to_string(lives).c_str(), &w, &h);
                SDL_Rect livesTextRect = {lifeIconRect.x + iconSize + gap, livesBoxRect.y + (boxHeight - h) / 2, w, h};
                SDL_RenderCopy(renderer, livesText, nullptr, &livesTextRect);
                SDL_DestroyTexture(livesText);
            }
            
            
            
            // wand icon size, number,image 
            if (wandPurchased) {
                SDL_Rect wandIconRect = {SCREEN_WIDTH - 70, 120, 50, 50};
                SDL_RenderCopy(renderer, textures[29], nullptr, &wandIconRect);
                SDL_Texture* wandUsesText = renderText(renderer, font, std::to_string(wandUsesRemaining));
                if (wandUsesText) {
                    int w, h; TTF_SizeText(font, std::to_string(wandUsesRemaining).c_str(), &w, &h);
                    SDL_Rect wandTextRect = {SCREEN_WIDTH - 70, 170, w, h};
                    SDL_RenderCopy(renderer, wandUsesText, nullptr, &wandTextRect);
                    SDL_DestroyTexture(wandUsesText);
                }
            }
        } 
        
        
        
        
        // handles rendering of game over screen
        else if (state == GAME_OVER) {
            int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
            SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
            SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
            SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
            SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
            SDL_Rect drawPlayer = {player.x + shakeOffsetX, player.y + shakeOffsetY, player.w, player.h};
            SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
            if (dragonDeadFrame >= 0) {
                SDL_Rect dragonDst = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
                SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
            }
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 102);
            SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &overlayRect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_Rect gameOverRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
            SDL_RenderCopy(renderer, textures[37], nullptr, &gameOverRect);
            SDL_Rect playAgainBtn = {SCREEN_WIDTH / 4 + (SCREEN_WIDTH / 2 - 300) / 2, SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 2 - 90), 300, 90};
            SDL_RenderCopy(renderer, textures[38], nullptr, &playAgainBtn);
            int mx, my; SDL_GetMouseState(&mx, &my); SDL_Point mouse = {mx, my};
            if (SDL_PointInRect(&mouse, &playAgainBtn)) {
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                SDL_RenderDrawRect(renderer, &playAgainBtn);
            }
            if (textures[20]) {
                SDL_Rect wandCursor = {playAgainBtn.x - 40, playAgainBtn.y + 10, 40, 80};
                SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
            }
        }
        
        else if (state == CONGRATS) {
            if (congratsStartTime == 0) congratsStartTime = SDL_GetTicks();
            if (SDL_GetTicks() - congratsStartTime >= CONGRATS_DURATION) {
                state = LEVEL_SELECT;
                congratsStartTime = 0;
            }
            int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
            SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
            SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
            SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
            SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
            SDL_Rect drawPlayer = {player.x + shakeOffsetX, player.y + shakeOffsetY, player.w, player.h};
            SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
            if (dragonDeadFrame >= 0) {
                SDL_Rect dragonDst = {dragonX + shakeOffsetX,dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
                SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
            }
            // transparent black lay of congratulation
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 102);
            SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &overlayRect);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_Texture* congratsText = renderText(renderer, font, "Congratulations! Level 1 Completed!");
            if (congratsText) {
                int w, h;
                TTF_SizeText(font, "Congratulations! Level 1 Completed!", &w, &h);
                SDL_Rect textRect = {SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h};
                SDL_RenderCopy(renderer, congratsText, nullptr, &textRect);
                SDL_DestroyTexture(congratsText);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    //  freeing dynamically loaded resources
    for (auto tex : dragonFrames) SDL_DestroyTexture(tex);
    for (int i = 0; i < 40; ++i) if (textures[i]) SDL_DestroyTexture(textures[i]);
    for (int i = 0; i < 3; ++i) if (dragonDead[i]) SDL_DestroyTexture(dragonDead[i]);
    for (auto frame : introFrames) if (frame) SDL_DestroyTexture(frame);
    SDL_DestroyTexture(spriteSheet);
    Mix_FreeChunk(collectibleSound);
    Mix_FreeChunk(boosterSound);
    Mix_FreeChunk(buttonClickSound);
    Mix_FreeChunk(dragonRoarSound);
    Mix_FreeChunk(dragonDeadSound);
    Mix_FreeMusic(introMusic);
    Mix_FreeMusic(storyMusic);
    Mix_FreeMusic(gameMusic);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
