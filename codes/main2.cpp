/*
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <cstdio>
#include "game_config1.h"
#include "game_state.h"
#include "rendering1.h"
#include "utils1.h"

// Global variables
SDL_Texture* textures[40] = {nullptr};
TTF_Font* font = nullptr;
std::vector<SDL_Texture*> introFrames;
std::vector<std::string> storySlides;
SDL_Texture* spriteSheet = nullptr;
int frameWidth = 50;
int frameHeight = 50;
std::vector<SDL_Texture*> dragonFrames;
SDL_Texture* dragonDead[3] = {nullptr};
SDL_Texture* wandEffectSheet = nullptr;
int wandEffectFrameWidth = 50;
int wandEffectFrameHeight = 50;
bool wandPurchased = false;
Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* wandSound = nullptr;
Mix_Chunk* buttonClickSound = nullptr;
Uint32 introStartTime = 0; // Initialize here

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "Unable to initialize SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }
    
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Unable to initialize SDL_mixer: %s\n", Mix_GetError());
        return 1;
    }
    Mix_AllocateChannels(4);
    
    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Unable to initialize SDL_image: %s\n", IMG_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        return 1;
    }
    
    // Load font
    font = TTF_OpenFont("assets/arial.ttf", 24);
    if (!font) {
        fprintf(stderr, "Warning: Unable to load font: %s\n", TTF_GetError());
    }
    
    // Load audio files
    backgroundMusic = Mix_LoadMUS("assets/intro.ogg");
    if (!backgroundMusic) {
        fprintf(stderr, "Warning: Unable to load background music: %s\n", Mix_GetError());
    } else {
        Mix_PlayMusic(backgroundMusic, -1);
    }
    
    wandSound = Mix_LoadWAV("assets/wand_sound.wav");
    if (!wandSound) {
        fprintf(stderr, "Warning: Unable to load wand sound: %s\n", Mix_GetError());
    }
    
    buttonClickSound = Mix_LoadWAV("assets/button_click.wav");
    if (!buttonClickSound) {
        fprintf(stderr, "Warning: Unable to load button click sound: %s\n", Mix_GetError());
    }
    
    // Load 28 intro PNGs
    for (int i = 1; i <= 28; ++i) {
        char filename[64];
        snprintf(filename, sizeof(filename), "assets/intro%d.png", i);
        SDL_Surface* surface = IMG_Load(filename);
        if (surface) {
            introFrames.push_back(SDL_CreateTextureFromSurface(renderer, surface));
            SDL_FreeSurface(surface);
            printf("Loaded %s successfully\n", filename);
        } else {
            fprintf(stderr, "Warning: Unable to load %s: %s\n", filename, IMG_GetError());
        }
    }
    
    // Initialize game variables
    GameState state = INTRO;
    bool running = true;
    bool keyPressed = false;
    int currentLevel = 1;
    bool level1Completed = false;
    Uint32 countDownStartTime = 0;
    int countDownValue = 3;
    SDL_Rect player = {100, GROUND_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
    float velocityY = 0.0f;
    bool isJumping = false;
    bool onPlatform = false;
    std::vector<SDL_Rect> candles;
    std::vector<SDL_Rect> bottomObstacles;
    std::vector<SDL_Rect> collectibles;
    std::vector<SDL_Rect> boosters;
    std::vector<SDL_Rect> topObstacles;
    SDL_Rect goldenEggRect = {0, 0, 50, 50};
    int score = 0;
    int lives = 3;
    int collisionCount = 0;
    float collectibleFloatOffset = 0.0f;
    int currentFrame = 0;
    int dragonFrame = 0;
    int dragonDeadFrame = -1;
    int boosterFrame = 0;
    int dragonX = 600;
    int dragonY = 100;
    int bottomObstaclesPassed = 0;
    int wandUsesRemaining = 0;
    Uint32 wandEffectStartTime = 0;
    int wandEffectFrame = 0;
    bool wandOffered = false;
    bool waitingForWand = false;
    bool waitingForGround = false;
    bool isShaking = false;
    bool playerAnimationStopped = false;
    bool hidePlayer = false;
    bool wandEffectPlaying = false;
    bool waitingForDragonPause = false;
    bool dragonStopped = false;
    bool obstaclesStopped = false;
    bool dragonMovingDiagonally = false;
    bool isDragonFalling = false;
    bool goldenEggVisible = false;
    int highScore = 0;
    int storyIndex = 0;
    int shakeOffsetX = 0;
    int shakeOffsetY = 0;
    introStartTime = SDL_GetTicks();
    
    SDL_Surface* wandEffectSurface = nullptr;
    if (wandEffectSurface) {
        wandEffectFrameWidth = wandEffectSurface->w / WAND_EFFECT_FRAMES;
        wandEffectFrameHeight = wandEffectSurface->h;
        wandEffectSheet = SDL_CreateTextureFromSurface(renderer, wandEffectSurface);
        SDL_FreeSurface(wandEffectSurface);
    }
    
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            handleEvents(e, running, keyPressed, state, currentLevel, countDownStartTime, countDownValue, player, velocityY, isJumping,
                         onPlatform, obstaclesStopped, dragonStopped, dragonMovingDiagonally, isDragonFalling, goldenEggVisible,
                         wandOffered, wandPurchased, waitingForWand, waitingForGround, isShaking,
                         playerAnimationStopped, hidePlayer, wandEffectPlaying, waitingForDragonPause,
                         score, lives, collisionCount, collectibleFloatOffset, currentFrame, dragonFrame,
                         dragonDeadFrame, boosterFrame, dragonX, dragonY, bottomObstaclesPassed,
                         wandUsesRemaining, wandEffectStartTime, wandEffectFrame, bottomObstacles,
                         collectibles, boosters, topObstacles, goldenEggRect, level1Completed);
        }
        
        // Debug: Check state and texture loading
        printf("Current state: %d (INTRO=0, STORY=1, MENU=2, ...)\n", state);
        printf("introFrames size: %zu\n", introFrames.size());
        if (!introFrames.empty() && introFrames[0]) {
            printf("introFrames[0] is loaded\n");
        } else {
            printf("Warning: introFrames is empty or introFrames[0] is null\n");
        }
        if (!textures[0]) printf("Warning: textures[0] is null\n");
        if (!spriteSheet) printf("Warning: spriteSheet is null\n");
        if (!dragonDead[0]) printf("Warning: dragonDead[0] is null\n");
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black clear
        SDL_RenderClear(renderer);
        
        switch (state) {
            case INTRO:
                renderIntro(state, currentFrame, renderer, textures, introFrames);
                break;
            case STORY:
                renderStory(state, storyIndex, storySlides, candles, renderer, font, textures);
                break;
            case MENU:
                renderMenu(state, candles, renderer, textures);
                break;
            case LEVEL_SELECT:
                renderLevelSelect(state, level1Completed, candles, renderer, font, textures);
                break;
            case SCOREBOARD:
                renderScoreboard(state, highScore, candles, renderer, font, textures);
                break;
            case COUNTDOWN:
                renderCountdown(state, countDownValue, currentLevel, 0.0f, player, currentFrame, boosterFrame, collectibleFloatOffset,
                                bottomObstacles, collectibles, boosters, topObstacles, renderer, font, textures,
                                spriteSheet, frameWidth, frameHeight);
                break;
            case PAUSED:
                renderPaused(state, renderer, font, textures);
                break;
            case PLAYING:
                renderPlaying(state, currentLevel, 0.0f, player, velocityY, isJumping, onPlatform, obstaclesStopped,
                              dragonStopped, dragonMovingDiagonally, isDragonFalling, goldenEggVisible,
                              wandOffered, wandPurchased, waitingForWand, waitingForGround, isShaking,
                              playerAnimationStopped, hidePlayer, wandEffectPlaying, waitingForDragonPause,
                              score, lives, collisionCount, collectibleFloatOffset, currentFrame, dragonFrame,
                              dragonDeadFrame, boosterFrame, dragonX, dragonY, bottomObstaclesPassed,
                              wandUsesRemaining, wandEffectStartTime, wandEffectFrame, bottomObstacles,
                              collectibles, boosters, topObstacles, goldenEggRect, renderer, font, textures,
                              spriteSheet, wandEffectSheet, dragonFrames, dragonDead, frameWidth, frameHeight,
                              wandEffectFrameWidth, wandEffectFrameHeight, shakeOffsetX, shakeOffsetY);
                break;
            case GAME_OVER:
                renderGameOver(state, 0.0f, player, currentFrame, dragonDeadFrame, dragonX, dragonY,
                               shakeOffsetX, shakeOffsetY, renderer, textures, spriteSheet, dragonDead,
                               frameWidth, frameHeight);
                break;
            case CONGRATS:
                renderCongrats(state, 0.0f, player, currentFrame, dragonDeadFrame, dragonX, dragonY,
                               shakeOffsetX, shakeOffsetY, renderer, font, textures, spriteSheet, dragonDead,
                               frameWidth, frameHeight);
                break;
        }
        
        SDL_RenderPresent(renderer);
    }
    
    // Cleanup
    for (int i = 0; i < 40; ++i) {
        if (textures[i]) SDL_DestroyTexture(textures[i]);
    }
    for (auto& tex : introFrames) {
        if (tex) SDL_DestroyTexture(tex);
    }
    for (auto& tex : dragonFrames) {
        if (tex) SDL_DestroyTexture(tex);
    }
    for (int i = 0; i < 3; ++i) {
        if (dragonDead[i]) SDL_DestroyTexture(dragonDead[i]);
    }
    if (spriteSheet) SDL_DestroyTexture(spriteSheet);
    if (wandEffectSheet) SDL_DestroyTexture(wandEffectSheet);
    if (font) TTF_CloseFont(font);
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    if (wandSound) Mix_FreeChunk(wandSound);
    if (buttonClickSound) Mix_FreeChunk(buttonClickSound);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
*/
/*
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <cstdio>
#include "game_config1.h"
#include "game_state.h"
#include "rendering1.h"
#include "utils1.h"

// Global variables
SDL_Texture* textures[40] = {nullptr};
TTF_Font* font = nullptr;
std::vector<SDL_Texture*> introFrames;
std::vector<std::string> storySlides;
SDL_Texture* spriteSheet = nullptr;
int frameWidth = 50;
int frameHeight = 50;
std::vector<SDL_Texture*> dragonFrames;
SDL_Texture* dragonDead[3] = {nullptr};
SDL_Texture* wandEffectSheet = nullptr;
int wandEffectFrameWidth = 50;
int wandEffectFrameHeight = 50;
bool wandPurchased = false;
Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* wandSound = nullptr;
Mix_Chunk* buttonClickSound = nullptr;
Uint32 introStartTime = 0;
bool hasLoggedError = false; // To limit warning messages

// Function to load a texture with error handling
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "Failed to load %s: %s\n", path, IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "Unable to initialize SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }
    
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Unable to initialize SDL_mixer: %s\n", Mix_GetError());
        return 1;
    }
    Mix_AllocateChannels(4);
    
    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Unable to initialize SDL_image: %s\n", IMG_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        return 1;
    }
    
    // Load font
    font = TTF_OpenFont("assets/arial.ttf", 24);
    if (!font) {
        fprintf(stderr, "Warning: Unable to load font: %s\n", TTF_GetError());
    }
    
    // Load audio files
    backgroundMusic = Mix_LoadMUS("assets/intro.ogg");
    if (!backgroundMusic) {
        fprintf(stderr, "Warning: Unable to load background music: %s\n", Mix_GetError());
    } else {
        Mix_PlayMusic(backgroundMusic, -1);
    }
    
    wandSound = Mix_LoadWAV("assets/wand_sound.wav");
    if (!wandSound) {
        fprintf(stderr, "Warning: Unable to load wand sound: %s\n", Mix_GetError());
    }
    
    buttonClickSound = Mix_LoadWAV("assets/button_click.wav");
    if (!buttonClickSound) {
        fprintf(stderr, "Warning: Unable to load button click sound: %s\n", Mix_GetError());
    }
    
    // Load 28 intro PNGs
    for (int i = 1; i <= 28; ++i) {
        char filename[64];
        snprintf(filename, sizeof(filename), "assets/intro%d.png", i);
        SDL_Texture* texture = loadTexture(filename, renderer);
        if (texture) {
            introFrames.push_back(texture);
            printf("Loaded %s successfully\n", filename);
        } else {
            fprintf(stderr, "Warning: Unable to load %s\n", filename);
        }
    }
    
    // Load textures, spriteSheet, and dragonDead
    textures[0] = loadTexture("assets/texture.png", renderer);
    spriteSheet = loadTexture("assets/spriteSheet.png", renderer);
    dragonDead[0] = loadTexture("assets/dragonDead.png", renderer);
    wandEffectSheet = loadTexture("assets/wandEffect.png", renderer);
    if (wandEffectSheet) {
        SDL_QueryTexture(wandEffectSheet, nullptr, nullptr, &wandEffectFrameWidth, &wandEffectFrameHeight);
        wandEffectFrameWidth /= WAND_EFFECT_FRAMES; // Adjust for frame count
    }
    
    // Log asset loading errors once
    if (!hasLoggedError) {
        if (!textures[0]) fprintf(stderr, "Warning: textures[0] is null\n");
        if (!spriteSheet) fprintf(stderr, "Warning: spriteSheet is null\n");
        if (!dragonDead[0]) fprintf(stderr, "Warning: dragonDead[0] is null\n");
        if (!wandEffectSheet) fprintf(stderr, "Warning: wandEffectSheet is null\n");
        hasLoggedError = true;
    }
    
    // Initialize game variables
    GameState state = INTRO;
    bool running = true;
    bool keyPressed = false;
    int currentLevel = 1;
    bool level1Completed = false;
    Uint32 countDownStartTime = 0;
    int countDownValue = 3;
    SDL_Rect player = {100, GROUND_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
    float velocityY = 0.0f;
    bool isJumping = false;
    bool onPlatform = false;
    std::vector<SDL_Rect> candles;
    std::vector<SDL_Rect> bottomObstacles;
    std::vector<SDL_Rect> collectibles;
    std::vector<SDL_Rect> boosters;
    std::vector<SDL_Rect> topObstacles;
    SDL_Rect goldenEggRect = {0, 0, 50, 50};
    int score = 0;
    int lives = 3;
    int collisionCount = 0;
    float collectibleFloatOffset = 0.0f;
    int currentFrame = 0;
    int dragonFrame = 0;
    int dragonDeadFrame = -1;
    int boosterFrame = 0;
    int dragonX = 600;
    int dragonY = 100;
    int bottomObstaclesPassed = 0;
    int wandUsesRemaining = 0;
    Uint32 wandEffectStartTime = 0;
    int wandEffectFrame = 0;
    bool wandOffered = false;
    bool waitingForWand = false;
    bool waitingForGround = false;
    bool isShaking = false;
    bool playerAnimationStopped = false;
    bool hidePlayer = false;
    bool wandEffectPlaying = false;
    bool waitingForDragonPause = false;
    bool dragonStopped = false;
    bool obstaclesStopped = false;
    bool dragonMovingDiagonally = false;
    bool isDragonFalling = false;
    bool goldenEggVisible = false;
    int highScore = 0;
    int storyIndex = 0;
    int shakeOffsetX = 0;
    int shakeOffsetY = 0;
    introStartTime = SDL_GetTicks();
    
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            handleEvents(e, running, keyPressed, state, currentLevel, countDownStartTime, countDownValue, player, velocityY, isJumping,
                         onPlatform, obstaclesStopped, dragonStopped, dragonMovingDiagonally, isDragonFalling, goldenEggVisible,
                         wandOffered, wandPurchased, waitingForWand, waitingForGround, isShaking,
                         playerAnimationStopped, hidePlayer, wandEffectPlaying, waitingForDragonPause,
                         score, lives, collisionCount, collectibleFloatOffset, currentFrame, dragonFrame,
                         dragonDeadFrame, boosterFrame, dragonX, dragonY, bottomObstaclesPassed,
                         wandUsesRemaining, wandEffectStartTime, wandEffectFrame, bottomObstacles,
                         collectibles, boosters, topObstacles, goldenEggRect, level1Completed);
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black clear
        SDL_RenderClear(renderer);
        
        switch (state) {
            case INTRO:
                renderIntro(state, currentFrame, renderer, textures, introFrames);
                // Auto-transition to STORY after 28 frames (assuming 100ms per frame)
                if (!introFrames.empty() && currentFrame >= 28 * 100) {
                    state = STORY;
                    currentFrame = 0;
                }
                break;
            case STORY:
                renderStory(state, storyIndex, storySlides, candles, renderer, font, textures);
                break;
            case MENU:
                renderMenu(state, candles, renderer, textures);
                break;
            case LEVEL_SELECT:
                renderLevelSelect(state, level1Completed, candles, renderer, font, textures);
                break;
            case SCOREBOARD:
                renderScoreboard(state, highScore, candles, renderer, font, textures);
                break;
            case COUNTDOWN:
                renderCountdown(state, countDownValue, currentLevel, 0.0f, player, currentFrame, boosterFrame, collectibleFloatOffset,
                                bottomObstacles, collectibles, boosters, topObstacles, renderer, font, textures,
                                spriteSheet, frameWidth, frameHeight);
                break;
            case PAUSED:
                renderPaused(state, renderer, font, textures);
                break;
            case PLAYING:
                renderPlaying(state, currentLevel, 0.0f, player, velocityY, isJumping, onPlatform, obstaclesStopped,
                              dragonStopped, dragonMovingDiagonally, isDragonFalling, goldenEggVisible,
                              wandOffered, wandPurchased, waitingForWand, waitingForGround, isShaking,
                              playerAnimationStopped, hidePlayer, wandEffectPlaying, waitingForDragonPause,
                              score, lives, collisionCount, collectibleFloatOffset, currentFrame, dragonFrame,
                              dragonDeadFrame, boosterFrame, dragonX, dragonY, bottomObstaclesPassed,
                              wandUsesRemaining, wandEffectStartTime, wandEffectFrame, bottomObstacles,
                              collectibles, boosters, topObstacles, goldenEggRect, renderer, font, textures,
                              spriteSheet, wandEffectSheet, dragonFrames, dragonDead, frameWidth, frameHeight,
                              wandEffectFrameWidth, wandEffectFrameHeight, shakeOffsetX, shakeOffsetY);
                break;
            case GAME_OVER:
                renderGameOver(state, 0.0f, player, currentFrame, dragonDeadFrame, dragonX, dragonY,
                               shakeOffsetX, shakeOffsetY, renderer, textures, spriteSheet, dragonDead,
                               frameWidth, frameHeight);
                break;
            case CONGRATS:
                renderCongrats(state, 0.0f, player, currentFrame, dragonDeadFrame, dragonX, dragonY,
                               shakeOffsetX, shakeOffsetY, renderer, font, textures, spriteSheet, dragonDead,
                               frameWidth, frameHeight);
                break;
        }
        
        SDL_RenderPresent(renderer);
        // Control frame rate (assuming 60 FPS)
        SDL_Delay(16); // ~60 FPS
        currentFrame += 16; // Increment frame counter for INTRO animation
    }
    
    // Cleanup
    for (int i = 0; i < 40; ++i) {
        if (textures[i]) SDL_DestroyTexture(textures[i]);
    }
    for (auto& tex : introFrames) {
        if (tex) SDL_DestroyTexture(tex);
    }
    for (auto& tex : dragonFrames) {
        if (tex) SDL_DestroyTexture(tex);
    }
    for (int i = 0; i < 3; ++i) {
        if (dragonDead[i]) SDL_DestroyTexture(dragonDead[i]);
    }
    if (spriteSheet) SDL_DestroyTexture(spriteSheet);
    if (wandEffectSheet) SDL_DestroyTexture(wandEffectSheet);
    if (font) TTF_CloseFont(font);
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    if (wandSound) Mix_FreeChunk(wandSound);
    if (buttonClickSound) Mix_FreeChunk(buttonClickSound);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <cstdio>
#include "game_config1.h"
#include "game_state.h"
#include "rendering1.h"
#include "utils1.h"

// Global variables
SDL_Texture* textures[40] = {nullptr};
TTF_Font* font = nullptr;
std::vector<SDL_Texture*> introFrames;
std::vector<std::string> storySlides;
SDL_Texture* spriteSheet = nullptr;
int frameWidth = 50;
int frameHeight = 50;
std::vector<SDL_Texture*> dragonFrames;
SDL_Texture* dragonDead[3] = {nullptr};
SDL_Texture* wandEffectSheet = nullptr;
int wandEffectFrameWidth = 50;
int wandEffectFrameHeight = 50;
bool wandPurchased = false;
Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* wandSound = nullptr;
Mix_Chunk* buttonClickSound = nullptr;
Uint32 introStartTime = 0;

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "Unable to initialize SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }
    
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Unable to initialize SDL_mixer: %s\n", Mix_GetError());
        return 1;
    }
    Mix_AllocateChannels(4);
    
    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Unable to initialize SDL_image: %s\n", IMG_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        return 1;
    }
    
    // Load font
    font = TTF_OpenFont("assets/arial.ttf", 24);
    if (!font) {
        fprintf(stderr, "Warning: Unable to load font: %s\n", TTF_GetError());
    }
    
    // Load audio files
    backgroundMusic = Mix_LoadMUS("assets/intro.ogg");
    if (!backgroundMusic) {
        fprintf(stderr, "Warning: Unable to load background music: %s\n", Mix_GetError());
    } else {
        Mix_PlayMusic(backgroundMusic, -1);
    }
    
    wandSound = Mix_LoadWAV("assets/wand_sound.wav");
    if (!wandSound) {
        fprintf(stderr, "Warning: Unable to load wand sound: %s\n", Mix_GetError());
    }
    
    buttonClickSound = Mix_LoadWAV("assets/button_click.wav");
    if (!buttonClickSound) {
        fprintf(stderr, "Warning: Unable to load button click sound: %s\n", Mix_GetError());
    }
    
    // Load 28 intro PNGs
    for (int i = 1; i <= 28; ++i) {
        char filename[64];
        snprintf(filename, sizeof(filename), "assets/intro%d.png", i);
        SDL_Surface* surface = IMG_Load(filename);
        if (surface) {
            introFrames.push_back(SDL_CreateTextureFromSurface(renderer, surface));
            SDL_FreeSurface(surface);
            printf("Loaded %s successfully\n", filename);
        } else {
            fprintf(stderr, "Warning: Unable to load %s: %s\n", filename, IMG_GetError());
        }
    }
    
    // Initialize game variables
    GameState state = INTRO;
    bool running = true;
    bool keyPressed = false;
    int currentLevel = 1;
    bool level1Completed = false;
    Uint32 countDownStartTime = 0;
    int countDownValue = 3;
    SDL_Rect player = {100, GROUND_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
    float velocityY = 0.0f;
    bool isJumping = false;
    bool onPlatform = false;
    std::vector<SDL_Rect> candles;
    std::vector<SDL_Rect> bottomObstacles;
    std::vector<SDL_Rect> collectibles;
    std::vector<SDL_Rect> boosters;
    std::vector<SDL_Rect> topObstacles;
    SDL_Rect goldenEggRect = {0, 0, 50, 50};
    int score = 0;
    int lives = 3;
    int collisionCount = 0;
    float collectibleFloatOffset = 0.0f;
    int currentFrame = 0;
    int dragonFrame = 0;
    int dragonDeadFrame = -1;
    int boosterFrame = 0;
    int dragonX = 600;
    int dragonY = 100;
    int bottomObstaclesPassed = 0;
    int wandUsesRemaining = 0;
    Uint32 wandEffectStartTime = 0;
    int wandEffectFrame = 0;
    bool wandOffered = false;
    bool waitingForWand = false;
    bool waitingForGround = false;
    bool isShaking = false;
    bool playerAnimationStopped = false;
    bool hidePlayer = false;
    bool wandEffectPlaying = false;
    bool waitingForDragonPause = false;
    bool dragonStopped = false;
    bool obstaclesStopped = false;
    bool dragonMovingDiagonally = false;
    bool isDragonFalling = false;
    bool goldenEggVisible = false;
    int highScore = 0;
    int storyIndex = 0;
    int shakeOffsetX = 0;
    int shakeOffsetY = 0;
    introStartTime = SDL_GetTicks();
    
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            handleEvents(e, running, keyPressed, state, currentLevel, countDownStartTime, countDownValue, player, velocityY, isJumping,
                         onPlatform, obstaclesStopped, dragonStopped, dragonMovingDiagonally, isDragonFalling, goldenEggVisible,
                         wandOffered, wandPurchased, waitingForWand, waitingForGround, isShaking,
                         playerAnimationStopped, hidePlayer, wandEffectPlaying, waitingForDragonPause,
                         score, lives, collisionCount, collectibleFloatOffset, currentFrame, dragonFrame,
                         dragonDeadFrame, boosterFrame, dragonX, dragonY, bottomObstaclesPassed,
                         wandUsesRemaining, wandEffectStartTime, wandEffectFrame, bottomObstacles,
                         collectibles, boosters, topObstacles, goldenEggRect, level1Completed);
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black clear
        SDL_RenderClear(renderer);
        
        switch (state) {
            case INTRO:
                renderIntro(state, currentFrame, renderer, textures, introFrames);
                // Auto-transition to STORY after 28 frames (100ms each)
                if (!introFrames.empty() && currentFrame >= 28 * 100) {
                    state = STORY;
                    currentFrame = 0;
                }
                break;
            case STORY:
                renderStory(state, storyIndex, storySlides, candles, renderer, font, textures);
                break;
            case MENU:
                renderMenu(state, candles, renderer, textures);
                break;
            case LEVEL_SELECT:
                renderLevelSelect(state, level1Completed, candles, renderer, font, textures);
                break;
            case SCOREBOARD:
                renderScoreboard(state, highScore, candles, renderer, font, textures);
                break;
            case COUNTDOWN:
                renderCountdown(state, countDownValue, currentLevel, 0.0f, player, currentFrame, boosterFrame, collectibleFloatOffset,
                                bottomObstacles, collectibles, boosters, topObstacles, renderer, font, textures,
                                spriteSheet, frameWidth, frameHeight);
                break;
            case PAUSED:
                renderPaused(state, renderer, font, textures);
                break;
            case PLAYING:
                renderPlaying(state, currentLevel, 0.0f, player, velocityY, isJumping, onPlatform, obstaclesStopped,
                              dragonStopped, dragonMovingDiagonally, isDragonFalling, goldenEggVisible,
                              wandOffered, wandPurchased, waitingForWand, waitingForGround, isShaking,
                              playerAnimationStopped, hidePlayer, wandEffectPlaying, waitingForDragonPause,
                              score, lives, collisionCount, collectibleFloatOffset, currentFrame, dragonFrame,
                              dragonDeadFrame, boosterFrame, dragonX, dragonY, bottomObstaclesPassed,
                              wandUsesRemaining, wandEffectStartTime, wandEffectFrame, bottomObstacles,
                              collectibles, boosters, topObstacles, goldenEggRect, renderer, font, textures,
                              spriteSheet, wandEffectSheet, dragonFrames, dragonDead, frameWidth, frameHeight,
                              wandEffectFrameWidth, wandEffectFrameHeight, shakeOffsetX, shakeOffsetY);
                break;
            case GAME_OVER:
                renderGameOver(state, 0.0f, player, currentFrame, dragonDeadFrame, dragonX, dragonY,
                               shakeOffsetX, shakeOffsetY, renderer, textures, spriteSheet, dragonDead,
                               frameWidth, frameHeight);
                break;
            case CONGRATS:
                renderCongrats(state, 0.0f, player, currentFrame, dragonDeadFrame, dragonX, dragonY,
                               shakeOffsetX, shakeOffsetY, renderer, font, textures, spriteSheet, dragonDead,
                               frameWidth, frameHeight);
                break;
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
        currentFrame += 16; // Increment frame counter for INTRO animation
    }
    
    // Cleanup
    for (int i = 0; i < 40; ++i) {
        if (textures[i]) SDL_DestroyTexture(textures[i]);
    }
    for (auto& tex : introFrames) {
        if (tex) SDL_DestroyTexture(tex);
    }
    for (auto& tex : dragonFrames) {
        if (tex) SDL_DestroyTexture(tex);
    }
    for (int i = 0; i < 3; ++i) {
        if (dragonDead[i]) SDL_DestroyTexture(dragonDead[i]);
    }
    if (spriteSheet) SDL_DestroyTexture(spriteSheet);
    if (wandEffectSheet) SDL_DestroyTexture(wandEffectSheet);
    if (font) TTF_CloseFont(font);
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    if (wandSound) Mix_FreeChunk(wandSound);
    if (buttonClickSound) Mix_FreeChunk(buttonClickSound);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
