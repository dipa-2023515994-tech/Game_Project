/*#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

enum GameState { INTRO, STORY, MENU, LEVEL_SELECT, SCOREBOARD, COUNTDOWN, PLAYING, PAUSED, GAME_OVER, CONGRATS };

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
                  std::vector<SDL_Rect>& candles, std::vector<std::string>& storySlides, SDL_Rect& goldenEggRect);

#endif
*/
/*
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "game_config1.h"

// Game state enumeration
enum GameState { INTRO, STORY, MENU, LEVEL_SELECT, SCOREBOARD, COUNTDOWN, PLAYING, PAUSED, GAME_OVER, CONGRATS };

// Handle user input and update game state
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
);
*/

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>
#include <vector>
#include "game_config1.h"

enum GameState {
    INTRO,
    STORY,
    MENU,
    LEVEL_SELECT,
    SCOREBOARD,
    COUNTDOWN,
    PLAYING,
    PAUSED,
    GAME_OVER,
    CONGRATS
};

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
);

#endif // GAME_STATE_H
