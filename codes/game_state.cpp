/*#include "game_state.h"
#include "game_objects.h"
#include "game_config.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <algorithm>

void handleEvents(GameState& state, bool& running, bool& keyPressed, int& currentLevel, bool& level1Completed,
                  SDL_Rect& player, float& velocityY, bool& isJumping, bool& onPlatform, bool& obstaclesStopped,
                  bool& dragonStopped, bool& dragonMovingDiagonally, bool& isDragonFalling, bool& goldenEggVisible,
                  bool& wandOffered, bool& wandPurchased, bool& waitingForWand, bool& waitingForGround,
                  bool& isShaking, bool& playerAnimationStopped, bool& hidePlayer, bool& wandEffectPlaying,
                  bool& waitingForDragonPause, int& score, int& highScore, int& lives, int& collisionCount,
                  float& cameraX, float& collectibleFloatOffset, int& currentFrame, int& dragonFrame,
                  int& dragonDeadFrame, int& boosterFrame, int& dragonX, int& dragonY, int& bottomObstaclesPassed,
                  int& spacePressCount, int& wandUsesRemaining, Uint32& lastFrameTime, Uint32& lastDragonFrameTime,
                  Uint32& shakeStartTime, Uint32& boosterAnimTime, Uint32& dragonStopTime, Uint32& lastSpacePressTime,
                  Uint32& introStartTime, Uint32& countDownStartTime, Uint32& congratsStartTime, int& introFrameIndex,
                  int& storyIndex, int& wandEffectFrame, Uint32& wandEffectStartTime, Uint32& dragonPauseStartTime,
                  int& dragonRoarChannel, int& dragonDeadChannel, std::vector<SDL_Rect>& bottomObstacles,
                  std::vector<SDL_Rect>& collectibles, std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles,
                  std::vector<SDL_Rect>& candles, std::vector<std::string>& storySlides, SDL_Rect& goldenEggRect) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN && !keyPressed) {
            keyPressed = true;
            if (e.key.keysym.sym == SDLK_RETURN && (state == MENU || state == GAME_OVER || state == SCOREBOARD || state == CONGRATS)) {
                state = LEVEL_SELECT;
            } else if (e.key.keysym.sym == SDLK_p && state == PLAYING && !wandOffered) {
                state = PAUSED;
            } else if (e.key.keysym.sym == SDLK_p && state == PAUSED) {
                state = PLAYING;
            } else if (e.key.keysym.sym == SDLK_w && state == PLAYING && wandPurchased && !wandOffered) {
                useWand(wandPurchased, wandUsesRemaining, wandOffered, waitingForWand, dragonStopped,
                        dragonMovingDiagonally, wandEffectPlaying, wandEffectFrame, wandEffectStartTime,
                        hidePlayer, dragonRoarChannel, bottomObstacles, topObstacles);
            } else if (e.key.keysym.sym == SDLK_SPACE && state == PLAYING && !wandOffered) {
                Uint32 currentTime = SDL_GetTicks();
                if (!isJumping || (spacePressCount == 1 && currentTime - lastSpacePressTime <= 300)) {
                    velocityY = (spacePressCount == 1) ? DOUBLE_JUMP_VELOCITY : JUMP_VELOCITY;
                    isJumping = true;
                    onPlatform = false;
                    spacePressCount = (spacePressCount == 1) ? 0 : 1;
                    lastSpacePressTime = currentTime;
                }
            } else if (state == STORY) {
                if (e.key.keysym.sym == SDLK_RIGHT) {
                    storyIndex = std::min(storyIndex + 1, (int)storySlides.size() - 1);
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    storyIndex = std::max(storyIndex - 1, 0);
                } else if (e.key.keysym.sym == SDLK_s) {
                    state = MENU;
                    storyIndex = 0;
                }
            }
        }
        if (e.type == SDL_KEYUP) keyPressed = false;
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
                } else if (SDL_PointInRect(&mouse, &scoreboardBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = SCOREBOARD;
                } else if (SDL_PointInRect(&mouse, &instrBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = STORY;
                    storyIndex = 0;
                } else if (SDL_PointInRect(&mouse, &quitBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    running = false;
                }
            } else if (state == LEVEL_SELECT) {
                SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
                SDL_Rect level2Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5 + 10), 300, 90};
                const int exitBtnSize = 50;
                SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
                if (SDL_PointInRect(&mouse, &level1Btn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    currentLevel = 1;
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                } else if (SDL_PointInRect(&mouse, &level2Btn) && level1Completed) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    currentLevel = 2;
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                } else if (SDL_PointInRect(&mouse, &exitBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = MENU;
                }
            } else if (state == SCOREBOARD || state == STORY) {
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
                        storyIndex++;
                        if (storyIndex >= storySlides.size()) state = MENU, storyIndex = 0;
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
                    state = MENU;
                }
            } else if (state == PLAYING && wandPurchased) {
                SDL_Rect wandIconRect = {SCREEN_WIDTH - 70, 120, 50, 50};
                if (SDL_PointInRect(&mouse, &wandIconRect)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    useWand(wandPurchased, wandUsesRemaining, wandOffered, waitingForWand, dragonStopped,
                            dragonMovingDiagonally, wandEffectPlaying, wandEffectFrame, wandEffectStartTime,
                            hidePlayer, dragonRoarChannel, bottomObstacles, topObstacles);
                }
            } else if (wandOffered) {
                int popupWidth = static_cast<int>(SCREEN_WIDTH * 0.45), popupHeight = static_cast<int>(SCREEN_HEIGHT * 0.45);
                SDL_Rect popupRect = {SCREEN_WIDTH / 2 - popupWidth / 2, SCREEN_HEIGHT / 2 - popupHeight / 2, popupWidth, popupHeight};
                SDL_Rect buyBtn = {popupRect.x + (popupRect.w - 360) / 2, popupRect.y + popupRect.h - 90, 180, 80};
                SDL_Rect laterBtn = {buyBtn.x + 180, buyBtn.y, 180, 80};
                if (SDL_PointInRect(&mouse, &buyBtn) && score >= 10) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    score -= 10;
                    wandOffered = false;
                    wandPurchased = true;
                } else if (SDL_PointInRect(&mouse, &laterBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    wandOffered = false;
                    wandPurchased = false;
                }
            } else if (state == GAME_OVER) {
                SDL_Rect playAgainBtn = {SCREEN_WIDTH / 4 + (SCREEN_WIDTH / 2 - 300) / 2, SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 2 - 90), 300, 90};
                if (SDL_PointInRect(&mouse, &playAgainBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                }
            }
        }
    }
}
*/
/*
#include "game_state.h"
#include "game_objects.h"
#include "game_config.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <algorithm>

void handleEvents(GameState& state, bool& running, bool& keyPressed, int& currentLevel, bool& level1Completed,
                  SDL_Rect& player, float& velocityY, bool& isJumping, bool& onPlatform, bool& obstaclesStopped,
                  bool& dragonStopped, bool& dragonMovingDiagonally, bool& isDragonFalling, bool& goldenEggVisible,
                  bool& wandOffered, bool& wandPurchased, bool& waitingForWand, bool& waitingForGround,
                  bool& isShaking, bool& playerAnimationStopped, bool& hidePlayer, bool& wandEffectPlaying,
                  bool& waitingForDragonPause, int& score, int& highScore, int& lives, int& collisionCount,
                  float& cameraX, float& collectibleFloatOffset, int& currentFrame, int& dragonFrame,
                  int& dragonDeadFrame, int& boosterFrame, int& dragonX, int& dragonY, int& bottomObstaclesPassed,
                  int& spacePressCount, int& wandUsesRemaining, Uint32& lastFrameTime, Uint32& lastDragonFrameTime,
                  Uint32& shakeStartTime, Uint32& boosterAnimTime, Uint32& dragonStopTime, Uint32& lastSpacePressTime,
                  Uint32& introStartTime, Uint32& countDownStartTime, Uint32& congratsStartTime, int& introFrameIndex,
                  int& storyIndex, int& wandEffectFrame, Uint32& wandEffectStartTime, Uint32& dragonPauseStartTime,
                  int& dragonRoarChannel, int& dragonDeadChannel, std::vector<SDL_Rect>& bottomObstacles,
                  std::vector<SDL_Rect>& collectibles, std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles,
                  std::vector<SDL_Rect>& candles, std::vector<std::string>& storySlides, SDL_Rect& goldenEggRect) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN && !keyPressed) {
            keyPressed = true;
            if (e.key.keysym.sym == SDLK_RETURN && (state == MENU || state == GAME_OVER || state == SCOREBOARD || state == CONGRATS)) {
                state = LEVEL_SELECT;
            } else if (e.key.keysym.sym == SDLK_p && state == PLAYING && !wandOffered) {
                state = PAUSED;
            } else if (e.key.keysym.sym == SDLK_p && state == PAUSED) {
                state = PLAYING;
            } else if (e.key.keysym.sym == SDLK_w && state == PLAYING && wandPurchased && !wandOffered) {
                useWand(wandPurchased, wandUsesRemaining, wandOffered, waitingForWand, dragonStopped,
                        dragonMovingDiagonally, wandEffectPlaying, wandEffectFrame, wandEffectStartTime,
                        hidePlayer, dragonRoarChannel, bottomObstacles, topObstacles);
            } else if (e.key.keysym.sym == SDLK_SPACE && state == PLAYING && !wandOffered) {
                Uint32 currentTime = SDL_GetTicks();
                if (!isJumping || (spacePressCount == 1 && currentTime - lastSpacePressTime <= 300)) {
                    velocityY = (spacePressCount == 1) ? DOUBLE_JUMP_VELOCITY : JUMP_VELOCITY;
                    isJumping = true;
                    onPlatform = false;
                    spacePressCount = (spacePressCount == 1) ? 0 : 1;
                    lastSpacePressTime = currentTime;
                }
            } else if (state == STORY) {
                if (e.key.keysym.sym == SDLK_RIGHT) {
                    storyIndex = std::min(storyIndex + 1, (int)storySlides.size() - 1);
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    storyIndex = std::max(storyIndex - 1, 0);
                } else if (e.key.keysym.sym == SDLK_s) {
                    state = MENU;
                    storyIndex = 0;
                }
            }
        }
        if (e.type == SDL_KEYUP) keyPressed = false;
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
                } else if (SDL_PointInRect(&mouse, &scoreboardBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = SCOREBOARD;
                } else if (SDL_PointInRect(&mouse, &instrBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = STORY;
                    storyIndex = 0;
                } else if (SDL_PointInRect(&mouse, &quitBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    running = false;
                }
            } else if (state == LEVEL_SELECT) {
                SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
                SDL_Rect level2Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5 + 10), 300, 90};
                const int exitBtnSize = 50;
                SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
                if (SDL_PointInRect(&mouse, &level1Btn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    currentLevel = 1;
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                } else if (SDL_PointInRect(&mouse, &level2Btn) && level1Completed) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    currentLevel = 2;
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                } else if (SDL_PointInRect(&mouse, &exitBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = MENU;
                }
            } else if (state == SCOREBOARD || state == STORY) {
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
                        storyIndex++;
                        if (storyIndex >= storySlides.size()) state = MENU, storyIndex = 0;
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
                    state = MENU;
                }
            } else if (state == PLAYING && wandPurchased) {
                SDL_Rect wandIconRect = {SCREEN_WIDTH - 70, 120, 50, 50};
                if (SDL_PointInRect(&mouse, &wandIconRect)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    useWand(wandPurchased, wandUsesRemaining, wandOffered, waitingForWand, dragonStopped,
                            dragonMovingDiagonally, wandEffectPlaying, wandEffectFrame, wandEffectStartTime,
                            hidePlayer, dragonRoarChannel, bottomObstacles, topObstacles);
                }
            } else if (wandOffered) {
                int popupWidth = static_cast<int>(SCREEN_WIDTH * 0.45), popupHeight = static_cast<int>(SCREEN_HEIGHT * 0.45);
                SDL_Rect popupRect = {SCREEN_WIDTH / 2 - popupWidth / 2, SCREEN_HEIGHT / 2 - popupHeight / 2, popupWidth, popupHeight};
                SDL_Rect buyBtn = {popupRect.x + (popupRect.w - 360) / 2, popupRect.y + popupRect.h - 90, 180, 80};
                SDL_Rect laterBtn = {buyBtn.x + 180, buyBtn.y, 180, 80};
                if (SDL_PointInRect(&mouse, &buyBtn) && score >= 10) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    score -= 10;
                    wandOffered = false;
                    wandPurchased = true;
                } else if (SDL_PointInRect(&mouse, &laterBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    wandOffered = false;
                    wandPurchased = false;
                }
            } else if (state == GAME_OVER) {
                SDL_Rect playAgainBtn = {SCREEN_WIDTH / 4 + (SCREEN_WIDTH / 2 - 300) / 2, SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 2 - 90), 300, 90};
                if (SDL_PointInRect(&mouse, &playAgainBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                }
            }
        }
    }
}
*/
/*
#include "game_state.h"
#include "game_objects1.h"
#include "game_config1.h"
#include "utils1.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <algorithm>

void handleEvents(GameState& state, bool& running, bool& keyPressed, int& currentLevel, bool& level1Completed,
                  SDL_Rect& player, float& velocityY, bool& isJumping, bool& onPlatform, bool& obstaclesStopped,
                  bool& dragonStopped, bool& dragonMovingDiagonally, bool& isDragonFalling, bool& goldenEggVisible,
                  bool& wandOffered, bool& wandPurchased, bool& waitingForWand, bool& waitingForGround,
                  bool& isShaking, bool& playerAnimationStopped, bool& hidePlayer, bool& wandEffectPlaying,
                  bool& waitingForDragonPause, int& score, int& highScore, int& lives, int& collisionCount,
                  float& cameraX, float& collectibleFloatOffset, int& currentFrame, int& dragonFrame,
                  int& dragonDeadFrame, int& boosterFrame, int& dragonX, int& dragonY, int& bottomObstaclesPassed,
                  int& spacePressCount, int& wandUsesRemaining, Uint32& lastFrameTime, Uint32& lastDragonFrameTime,
                  Uint32& shakeStartTime, Uint32& boosterAnimTime, Uint32& dragonStopTime, Uint32& lastSpacePressTime,
                  Uint32& introStartTime, Uint32& countDownStartTime, Uint32& congratsStartTime, int& introFrameIndex,
                  int& storyIndex, int& wandEffectFrame, Uint32& wandEffectStartTime, Uint32& dragonPauseStartTime,
                  int& dragonRoarChannel, int& dragonDeadChannel, std::vector<SDL_Rect>& bottomObstacles,
                  std::vector<SDL_Rect>& collectibles, std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles,
                  std::vector<SDL_Rect>& candles, std::vector<std::string>& storySlides, SDL_Rect& goldenEggRect) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN && !keyPressed) {
            keyPressed = true;
            if (e.key.keysym.sym == SDLK_RETURN && (state == MENU || state == GAME_OVER || state == SCOREBOARD || state == CONGRATS)) {
                state = LEVEL_SELECT;
            } else if (e.key.keysym.sym == SDLK_p && state == PLAYING && !wandOffered) {
                state = PAUSED;
            } else if (e.key.keysym.sym == SDLK_p && state == PAUSED) {
                state = PLAYING;
            } else if (e.key.keysym.sym == SDLK_w && state == PLAYING && wandPurchased && !wandOffered) {
                useWand(wandPurchased, wandUsesRemaining, wandOffered, waitingForWand, dragonStopped,
                        dragonMovingDiagonally, wandEffectPlaying, wandEffectFrame, wandEffectStartTime,
                        hidePlayer, dragonRoarChannel, player, bottomObstacles, topObstacles);
            } else if (e.key.keysym.sym == SDLK_SPACE && state == PLAYING && !wandOffered) {
                Uint32 currentTime = SDL_GetTicks();
                if (!isJumping || (spacePressCount == 1 && currentTime - lastSpacePressTime <= 300)) {
                    velocityY = (spacePressCount == 1) ? DOUBLE_JUMP_VELOCITY : JUMP_VELOCITY;
                    isJumping = true;
                    onPlatform = false;
                    spacePressCount = (spacePressCount == 1) ? 0 : 1;
                    lastSpacePressTime = currentTime;
                }
            } else if (state == STORY) {
                if (e.key.keysym.sym == SDLK_RIGHT) {
                    storyIndex = std::min(storyIndex + 1, (int)storySlides.size() - 1);
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    storyIndex = std::max(storyIndex - 1, 0);
                } else if (e.key.keysym.sym == SDLK_s) {
                    state = MENU;
                    storyIndex = 0;
                }
            }
        }
        if (e.type == SDL_KEYUP) keyPressed = false;
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
                } else if (SDL_PointInRect(&mouse, &scoreboardBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = SCOREBOARD;
                } else if (SDL_PointInRect(&mouse, &instrBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = STORY;
                    storyIndex = 0;
                } else if (SDL_PointInRect(&mouse, &quitBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    running = false;
                }
            } else if (state == LEVEL_SELECT) {
                SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
                SDL_Rect level2Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5 + 10), 300, 90};
                const int exitBtnSize = 50;
                SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
                if (SDL_PointInRect(&mouse, &level1Btn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    currentLevel = 1;
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                } else if (SDL_PointInRect(&mouse, &level2Btn) && level1Completed) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    currentLevel = 2;
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                } else if (SDL_PointInRect(&mouse, &exitBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = MENU;
                }
            } else if (state == SCOREBOARD || state == STORY) {
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
                        storyIndex++;
                        if (storyIndex >= storySlides.size()) state = MENU, storyIndex = 0;
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
                    state = MENU;
                }
            } else if (state == PLAYING && wandPurchased) {
                SDL_Rect wandIconRect = {SCREEN_WIDTH - 70, 120, 50, 50};
                if (SDL_PointInRect(&mouse, &wandIconRect)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    useWand(wandPurchased, wandUsesRemaining, wandOffered, waitingForWand, dragonStopped,
                            dragonMovingDiagonally, wandEffectPlaying, wandEffectFrame, wandEffectStartTime,
                            hidePlayer, dragonRoarChannel, player, bottomObstacles, topObstacles);
                }
            } else if (wandOffered) {
                int popupWidth = static_cast<int>(SCREEN_WIDTH * 0.45), popupHeight = static_cast<int>(SCREEN_HEIGHT * 0.45);
                SDL_Rect popupRect = {SCREEN_WIDTH / 2 - popupWidth / 2, SCREEN_HEIGHT / 2 - popupHeight / 2, popupWidth, popupHeight};
                SDL_Rect buyBtn = {popupRect.x + (popupRect.w - 360) / 2, popupRect.y + popupRect.h - 90, 180, 80};
                SDL_Rect laterBtn = {buyBtn.x + 180, buyBtn.y, 180, 80};
                if (SDL_PointInRect(&mouse, &buyBtn) && score >= 10) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    score -= 10;
                    wandOffered = false;
                    wandPurchased = true;
                } else if (SDL_PointInRect(&mouse, &laterBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    wandOffered = false;
                    wandPurchased = false;
                }
            } else if (state == GAME_OVER) {
                SDL_Rect playAgainBtn = {SCREEN_WIDTH / 4 + (SCREEN_WIDTH / 2 - 300) / 2, SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 2 - 90), 300, 90};
                if (SDL_PointInRect(&mouse, &playAgainBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    resetGame(currentLevel, player, velocityY, isJumping, onPlatform, score, lives, collisionCount,
                              cameraX, wandOffered, wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime,
                              bottomObstaclesPassed, obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame,
                              lastDragonFrameTime, dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame,
                              collectibleFloatOffset, boosterFrame, boosterAnimTime, isShaking, playerAnimationStopped,
                              dragonRoarChannel, dragonDeadChannel, goldenEggVisible, wandEffectPlaying, wandEffectFrame,
                              wandEffectStartTime, hidePlayer, waitingForDragonPause, dragonPauseStartTime,
                              bottomObstacles, collectibles, boosters, topObstacles);
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                }
            }
        }
    }
}
*/
/*
#include "game_state.h"
#include "game_objects1.h"
#include "game_config1.h"
#include "utils1.h"
#include <algorithm>
#include <SDL2/SDL_mixer.h>

void handleEvents(
    SDL_Event& e, bool& running, bool& keyPressed, GameState& state, int& currentLevel,
    Uint32& countDownStartTime, int& countDownValue, SDL_Rect& player, float& velocityY,
    bool& isJumping, bool& onPlatform, int& score, int& highScore, int& lives,
    int& collisionCount, float& cameraX, int& currentFrame, Uint32& lastFrameTime,
    int& dragonX, int& dragonY, int& dragonFrame, Uint32& lastDragonFrameTime,
    int& bottomObstaclesPassed, int& spacePressCount, Uint32& lastSpacePressTime,
    int& wandUsesRemaining, bool& wandOffered, bool& wandPurchased, bool& waitingForGround,
    bool& obstaclesStopped, bool& dragonStopped, bool& dragonMovingDiagonally,
    bool& isDragonFalling, int& dragonDeadFrame, bool& goldenEggVisible,
    Uint32& shakeStartTime, bool& isShaking, float& collectibleFloatOffset,
    int& boosterFrame, Uint32& boosterAnimTime, bool& playerAnimationStopped,
    Uint32& introStartTime, int& introFrameIndex, int& storyIndex,
    int& dragonRoarChannel, int& dragonDeadChannel, bool& showLightning,
    bool& lightningHitDragon, Uint32& lightningStartTime, SDL_Rect& lightningRect,
    Uint32& congratsStartTime, bool& waitingForWand, std::vector<SDL_Rect>& bottomObstacles,
    std::vector<SDL_Rect>& collectibles, std::vector<SDL_Rect>& boosters,
    std::vector<SDL_Rect>& topObstacles, std::vector<SDL_Rect>& candles,
    SDL_Rect& goldenEggRect, bool level1Completed
) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN && !keyPressed) {
            keyPressed = true;
            if (e.key.keysym.sym == SDLK_RETURN && (state == MENU || state == GAME_OVER || state == SCOREBOARD || state == CONGRATS)) state = LEVEL_SELECT;
            else if (e.key.keysym.sym == SDLK_p && state == PLAYING && !wandOffered) state = PAUSED;
            else if (e.key.keysym.sym == SDLK_p && state == PAUSED) state = PLAYING;
            else if (e.key.keysym.sym == SDLK_w && state == PLAYING && wandPurchased && !wandOffered) {
                useWand(player, bottomObstacles, topObstacles, wandUsesRemaining, wandOffered, waitingForWand, dragonStopped, showLightning, lightningHitDragon, lightningStartTime, lightningRect, dragonRoarChannel);
            }
            else if (e.key.keysym.sym == SDLK_SPACE && state == PLAYING && !wandOffered) {
                Uint32 currentTime = SDL_GetTicks();
                if (!isJumping || (spacePressCount == 1 && currentTime - lastSpacePressTime <= 300)) {
                    velocityY = (spacePressCount == 1) ? DOUBLE_JUMP_VELOCITY : JUMP_VELOCITY;
                    isJumping = true; onPlatform = false; spacePressCount = (spacePressCount == 1) ? 0 : 1;
                    lastSpacePressTime = currentTime;
                }
            } else if (state == STORY) {
                if (e.key.keysym.sym == SDLK_RIGHT) storyIndex = std::min(storyIndex + 1, (int)storySlides.size() - 1);
                else if (e.key.keysym.sym == SDLK_LEFT) storyIndex = std::max(storyIndex - 1, 0);
                else if (e.key.keysym.sym == SDLK_s) state = MENU, storyIndex = 0;
            }
        } else if (e.type == SDL_KEYUP) keyPressed = false;
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
                    state = STORY, storyIndex = 0;
                }
                else if (SDL_PointInRect(&mouse, &quitBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    running = false;
                }
            } else if (state == LEVEL_SELECT) {
                SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
                const int exitBtnSize = 50;
                SDL_Rect exitBtn = {SCREEN_WIDTH - exitBtnSize - 10, 10, exitBtnSize, exitBtnSize};
                if (SDL_PointInRect(&mouse, &level1Btn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    currentLevel = 1; state = COUNTDOWN; countDownStartTime = SDL_GetTicks(); countDownValue = 3;
                    resetGame(player, velocityY, isJumping, onPlatform, score, collisionCount, lives, cameraX, wandOffered,
                              wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime, bottomObstaclesPassed,
                              obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame, lastDragonFrameTime,
                              dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame, goldenEggVisible,
                              shakeStartTime, isShaking, collectibleFloatOffset, boosterFrame, boosterAnimTime,
                              playerAnimationStopped, dragonRoarChannel, dragonDeadChannel, showLightning,
                              lightningHitDragon, bottomObstacles, collectibles, boosters, topObstacles, goldenEggRect);
                } else if (SDL_PointInRect(&mouse, &exitBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    state = MENU;
                }
            } else if (state == SCOREBOARD || state == STORY) {
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
                        storyIndex++; if (storyIndex >= storySlides.size()) state = MENU, storyIndex = 0;
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
                    state = MENU;
                }
            } else if (state == PLAYING && wandPurchased) {
                SDL_Rect wandIconRect = {SCREEN_WIDTH - 70, 120, 50, 50};
                if (SDL_PointInRect(&mouse, &wandIconRect)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    useWand(player, bottomObstacles, topObstacles, wandUsesRemaining, wandOffered, waitingForWand,
                            dragonStopped, showLightning, lightningHitDragon, lightningStartTime, lightningRect,
                            dragonRoarChannel);
                }
            } else if (wandOffered) {
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
            } else if (state == GAME_OVER) {
                SDL_Rect playAgainBtn = {SCREEN_WIDTH / 4 + (SCREEN_WIDTH / 2 - 300) / 2, SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 2 - 90), 300, 90};
                if (SDL_PointInRect(&mouse, &playAgainBtn)) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    resetGame(player, velocityY, isJumping, onPlatform, score, collisionCount, lives, cameraX, wandOffered,
                              wandPurchased, wandUsesRemaining, currentFrame, lastFrameTime, bottomObstaclesPassed,
                              obstaclesStopped, waitingForGround, dragonX, dragonY, dragonFrame, lastDragonFrameTime,
                              dragonStopped, dragonMovingDiagonally, isDragonFalling, dragonDeadFrame, goldenEggVisible,
                              shakeStartTime, isShaking, collectibleFloatOffset, boosterFrame, boosterAnimTime,
                              playerAnimationStopped, dragonRoarChannel, dragonDeadChannel, showLightning,
                              lightningHitDragon, bottomObstacles, collectibles, boosters, topObstacles, goldenEggRect);
                    state = COUNTDOWN;
                    countDownStartTime = SDL_GetTicks();
                    countDownValue = 3;
                }
            }
        }
    }
}
*/

#include "game_state.h"
#include "game_config1.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

void handleEvents(
    SDL_Event& e,
    bool& running,
    bool& keyPressed,
    GameState& state,
    int& currentLevel,
    Uint32& countDownStartTime,
    int& countDownValue,
    SDL_Rect& player,
    float& velocityY,
    bool& isJumping,
    bool& onPlatform,
    bool& obstaclesStopped,
    bool& dragonStopped,
    bool& dragonMovingDiagonally,
    bool& isDragonFalling,
    bool& goldenEggVisible,
    bool& wandOffered,
    bool& wandPurchased,
    bool& waitingForWand,
    bool& waitingForGround,
    bool& isShaking,
    bool& playerAnimationStopped,
    bool& hidePlayer,
    bool& wandEffectPlaying,
    bool& waitingForDragonPause,
    int& score,
    int& lives,
    int& collisionCount,
    float& collectibleFloatOffset,
    int& currentFrame,
    int& dragonFrame,
    int& dragonDeadFrame,
    int& boosterFrame,
    int& dragonX,
    int& dragonY,
    int& bottomObstaclesPassed,
    int& wandUsesRemaining,
    Uint32& wandEffectStartTime,
    int& wandEffectFrame,
    const std::vector<SDL_Rect>& bottomObstacles,
    const std::vector<SDL_Rect>& collectibles,
    const std::vector<SDL_Rect>& boosters,
    const std::vector<SDL_Rect>& topObstacles,
    SDL_Rect& goldenEggRect,
    bool level1Completed
) {
    static int spacePressCount = 0; // Track number of space presses for double jump
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (e.key.repeat == 0) {
                    keyPressed = true;
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_SPACE:
                            if (state == PLAYING) {
                                if (!isJumping || (spacePressCount < 2 && !onPlatform)) {
                                    isJumping = true;
                                    spacePressCount++;
                                    velocityY = (spacePressCount == 1) ? JUMP_VELOCITY : DOUBLE_JUMP_VELOCITY;
                                    Mix_PlayChannel(-1, buttonClickSound, 0); // Play sound on jump
                                }
                            } else if (state == INTRO || state == STORY) {
                                state = MENU;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                        case SDLK_p:
                            if (state == PLAYING) {
                                state = PAUSED;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            } else if (state == PAUSED) {
                                state = PLAYING;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                        case SDLK_1:
                            if (state == LEVEL_SELECT) {
                                currentLevel = 1;
                                countDownStartTime = SDL_GetTicks();
                                countDownValue = 3;
                                state = COUNTDOWN;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                        case SDLK_2:
                            if (state == LEVEL_SELECT && level1Completed) {
                                currentLevel = 2;
                                countDownStartTime = SDL_GetTicks();
                                countDownValue = 3;
                                state = COUNTDOWN;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                        case SDLK_m:
                            if (state == SCOREBOARD || state == LEVEL_SELECT || state == GAME_OVER || state == CONGRATS) {
                                state = MENU;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                        case SDLK_s:
                            if (state == MENU) {
                                state = SCOREBOARD;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                        case SDLK_l:
                            if (state == MENU) {
                                state = LEVEL_SELECT;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                        case SDLK_y:
                            if (state == PLAYING && wandOffered && !wandPurchased && score >= 10) {
                                score -= 10;
                                wandPurchased = true;
                                wandUsesRemaining = 3;
                                wandOffered = false;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                        case SDLK_n:
                            if (state == PLAYING && wandOffered) {
                                wandOffered = false;
                                Mix_PlayChannel(-1, buttonClickSound, 0);
                            }
                            break;
                    }
                }
                break;
            case SDL_KEYUP:
                if (e.key.keysym.sym == SDLK_SPACE && state == PLAYING) {
                    if (player.y >= GROUND_Y - PLAYER_HEIGHT) {
                        isJumping = false;
                        spacePressCount = 0;
                        velocityY = 0.0f;
                        player.y = GROUND_Y - PLAYER_HEIGHT;
                    }
                }
                keyPressed = false;
                break;
        }
    }
}
