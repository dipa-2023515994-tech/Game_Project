/*#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <SDL2/SDL.h>
#include <vector>
#include <functional>

// Game object management functions
void resetGame(int& currentLevel, SDL_Rect& player, float& velocityY, bool& isJumping, bool& onPlatform, int& score,
               int& lives, int& collisionCount, float& cameraX, bool& wandOffered, bool& wandPurchased,
               int& wandUsesRemaining, int& currentFrame, Uint32& lastFrameTime, int& bottomObstaclesPassed,
               bool& obstaclesStopped, bool& waitingForGround, int& dragonX, int& dragonY, int& dragonFrame,
               Uint32& lastDragonFrameTime, bool& dragonStopped, bool& dragonMovingDiagonally, bool& isDragonFalling,
               int& dragonDeadFrame, float& collectibleFloatOffset, int& boosterFrame, Uint32& boosterAnimTime,
               bool& isShaking, bool& playerAnimationStopped, int& dragonRoarChannel, int& dragonDeadChannel,
               bool& goldenEggVisible, bool& wandEffectPlaying, int& wandEffectFrame, Uint32& wandEffectStartTime,
               bool& hidePlayer, bool& waitingForDragonPause, Uint32& dragonPauseStartTime,
               std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& collectibles,
               std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles);
void useWand(bool& wandPurchased, int& wandUsesRemaining, bool& wandOffered, bool& waitingForWand,
             bool& dragonStopped, bool& dragonMovingDiagonally, bool& wandEffectPlaying, int& wandEffectFrame,
             Uint32& wandEffectStartTime, bool& hidePlayer, int& dragonRoarChannel,
             std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles);

#endif
*/
/*
#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <SDL2/SDL.h>
#include <vector>

void resetGame(int& currentLevel, SDL_Rect& player, float& velocityY, bool& isJumping, bool& onPlatform, int& score,
               int& lives, int& collisionCount, float& cameraX, bool& wandOffered, bool& wandPurchased,
               int& wandUsesRemaining, int& currentFrame, Uint32& lastFrameTime, int& bottomObstaclesPassed,
               bool& obstaclesStopped, bool& waitingForGround, int& dragonX, int& dragonY, int& dragonFrame,
               Uint32& lastDragonFrameTime, bool& dragonStopped, bool& dragonMovingDiagonally, bool& isDragonFalling,
               int& dragonDeadFrame, float& collectibleFloatOffset, int& boosterFrame, Uint32& boosterAnimTime,
               bool& isShaking, bool& playerAnimationStopped, int& dragonRoarChannel, int& dragonDeadChannel,
               bool& goldenEggVisible, bool& wandEffectPlaying, int& wandEffectFrame, Uint32& wandEffectStartTime,
               bool& hidePlayer, bool& waitingForDragonPause, Uint32& dragonPauseStartTime,
               std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& collectibles,
               std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles);

void useWand(bool& wandPurchased, int& wandUsesRemaining, bool& wandOffered, bool& waitingForWand,
             bool& dragonStopped, bool& dragonMovingDiagonally, bool& wandEffectPlaying, int& wandEffectFrame,
             Uint32& wandEffectStartTime, bool& hidePlayer, int& dragonRoarChannel,
             std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles);

#endif
*/

#pragma once
#include <SDL2/SDL.h>
#include <vector>

// Initialize or reset game objects to their starting state
void resetGame(
    SDL_Rect& player, float& velocityY, bool& isJumping, bool& onPlatform, int& score,
    int& collisionCount, int& lives, float& cameraX, bool& wandOffered, bool& wandPurchased,
    int& wandUsesRemaining, int& currentFrame, Uint32& lastFrameTime, int& bottomObstaclesPassed,
    bool& obstaclesStopped, bool& waitingForGround, int& dragonX, int& dragonY, int& dragonFrame,
    Uint32& lastDragonFrameTime, bool& dragonStopped, bool& dragonMovingDiagonally,
    bool& isDragonFalling, int& dragonDeadFrame, bool& goldenEggVisible, Uint32& shakeStartTime,
    bool& isShaking, float& collectibleFloatOffset, int& boosterFrame, Uint32& boosterAnimTime,
    bool& playerAnimationStopped, int& dragonRoarChannel, int& dragonDeadChannel, bool& showLightning,
    bool& lightningHitDragon, std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& collectibles,
    std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles, SDL_Rect& goldenEggRect
);

// Handle wand usage to remove obstacles or affect the dragon
void useWand(
    SDL_Rect& player, std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles,
    int& wandUsesRemaining, bool& wandOffered, bool& waitingForWand, bool& dragonStopped,
    bool& showLightning, bool& lightningHitDragon, Uint32& lightningStartTime, SDL_Rect& lightningRect,
    int& dragonRoarChannel
);
