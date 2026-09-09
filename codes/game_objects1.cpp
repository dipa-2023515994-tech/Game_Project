/*#include "game_objects.h"
#include "game_config.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <limits>
#include <cmath>

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
               std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles) {
    player = {SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, GROUND_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
    velocityY = 0;
    isJumping = false;
    onPlatform = false;
    score = 0;
    lives = (currentLevel == 1) ? 3 : 2;
    collisionCount = 0;
    cameraX = 0;
    wandOffered = false;
    wandPurchased = false;
    wandUsesRemaining = WAND_USES_LIMIT;
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();
    bottomObstaclesPassed = 0;
    obstaclesStopped = false;
    waitingForGround = false;
    dragonX = SCREEN_WIDTH;
    dragonY = SCREEN_HEIGHT / 2 - DRAGON_HEIGHT / 2;
    dragonFrame = 0;
    lastDragonFrameTime = SDL_GetTicks();
    dragonStopped = false;
    dragonMovingDiagonally = false;
    isDragonFalling = false;
    dragonDeadFrame = 0;
    collectibleFloatOffset = 0.0f;
    boosterFrame = 0;
    boosterAnimTime = SDL_GetTicks();
    isShaking = false;
    playerAnimationStopped = false;
    dragonRoarChannel = -1;
    dragonDeadChannel = -1;
    wandEffectPlaying = false;
    wandEffectFrame = 0;
    wandEffectStartTime = 0;
    hidePlayer = false;
    waitingForDragonPause = false;
    dragonPauseStartTime = 0;
    bottomObstacles.clear();
    collectibles.clear();
    boosters.clear();
    topObstacles.clear();
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
}

void useWand(bool& wandPurchased, int& wandUsesRemaining, bool& wandOffered, bool& waitingForWand,
             bool& dragonStopped, bool& dragonMovingDiagonally, bool& wandEffectPlaying, int& wandEffectFrame,
             Uint32& wandEffectStartTime, bool& hidePlayer, int& dragonRoarChannel,
             std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles) {
    if (wandPurchased && wandUsesRemaining > 0 && !wandOffered) {
        SDL_Rect* closestObs = nullptr;
        int minDist = std::numeric_limits<int>::max();
        for (auto& obs : bottomObstacles) {
            if (obs.x > player.x && obs.x - player.x < minDist) {
                minDist = obs.x - player.x;
                closestObs = &obs;
            }
        }
        for (auto& obs : topObstacles) {
            if (obs.x > player.x && obs.x - player.x < minDist) {
                minDist = obs.x - player.x;
                closestObs = &obs;
            }
        }
        if (closestObs) {
            closestObs->x = -200;
            wandUsesRemaining--;
        }
        if (waitingForWand && dragonStopped) {
            waitingForWand = false;
            dragonStopped = false;
            dragonMovingDiagonally = false;
            wandEffectPlaying = true;
            wandEffectFrame = 0;
            wandEffectStartTime = SDL_GetTicks();
            wandUsesRemaining--;
            hidePlayer = true;
            if (dragonRoarChannel != -1) {
                Mix_HaltChannel(dragonRoarChannel);
                dragonRoarChannel = -1;
            }
        }
    }
}
*/
/*
#include "game_objects.h"
#include "game_config.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <limits>
#include <cmath>

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
               std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles) {
    player.x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    player.y = GROUND_Y - PLAYER_HEIGHT;
    player.w = PLAYER_WIDTH;
    player.h = PLAYER_HEIGHT;
    velocityY = 0;
    isJumping = false;
    onPlatform = false;
    score = 0;
    lives = (currentLevel == 1) ? 3 : 2;
    collisionCount = 0;
    cameraX = 0;
    wandOffered = false;
    wandPurchased = false;
    wandUsesRemaining = WAND_USES_LIMIT;
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();
    bottomObstaclesPassed = 0;
    obstaclesStopped = false;
    waitingForGround = false;
    dragonX = SCREEN_WIDTH;
    dragonY = SCREEN_HEIGHT / 2 - DRAGON_HEIGHT / 2;
    dragonFrame = 0;
    lastDragonFrameTime = SDL_GetTicks();
    dragonStopped = false;
    dragonMovingDiagonally = false;
    isDragonFalling = false;
    dragonDeadFrame = 0;
    collectibleFloatOffset = 0.0f;
    boosterFrame = 0;
    boosterAnimTime = SDL_GetTicks();
    isShaking = false;
    playerAnimationStopped = false;
    dragonRoarChannel = -1;
    dragonDeadChannel = -1;
    wandEffectPlaying = false;
    wandEffectFrame = 0;
    wandEffectStartTime = 0;
    hidePlayer = false;
    waitingForDragonPause = false;
    dragonPauseStartTime = 0;
    bottomObstacles.clear();
    collectibles.clear();
    boosters.clear();
    topObstacles.clear();
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
}

void useWand(bool& wandPurchased, int& wandUsesRemaining, bool& wandOffered, bool& waitingForWand,
             bool& dragonStopped, bool& dragonMovingDiagonally, bool& wandEffectPlaying, int& wandEffectFrame,
             Uint32& wandEffectStartTime, bool& hidePlayer, int& dragonRoarChannel,
             std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles) {
    if (wandPurchased && wandUsesRemaining > 0 && !wandOffered) {
        SDL_Rect* closestObs = nullptr;
        int minDist = std::numeric_limits<int>::max();
        for (auto& obs : bottomObstacles) {
            if (obs.x > player.x && obs.x - player.x < minDist) {
                minDist = obs.x - player.x;
                closestObs = &obs;
            }
        }
        for (auto& obs : topObstacles) {
            if (obs.x > player.x && obs.x - player.x < minDist) {
                minDist = obs.x - player.x;
                closestObs = &obs;
            }
        }
        if (closestObs) {
            closestObs->x = -200;
            wandUsesRemaining--;
        }
        if (waitingForWand && dragonStopped) {
            waitingForWand = false;
            dragonStopped = false;
            dragonMovingDiagonally = false;
            wandEffectPlaying = true;
            wandEffectFrame = 0;
            wandEffectStartTime = SDL_GetTicks();
            wandUsesRemaining--;
            hidePlayer = true;
            if (dragonRoarChannel != -1) {
                Mix_HaltChannel(dragonRoarChannel);
                dragonRoarChannel = -1;
            }
        }
    }
}
*/
/*
#include "game_objects1.h"
#include "game_config1.h"
#include "utils1.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <limits>
#include <cmath>

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
               std::vector<SDL_Rect>& boosters, std::vector<SDL_Rect>& topObstacles) {
    player.x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    player.y = GROUND_Y - PLAYER_HEIGHT;
    player.w = PLAYER_WIDTH;
    player.h = PLAYER_HEIGHT;
    velocityY = 0;
    isJumping = false;
    onPlatform = false;
    score = 0;
    lives = (currentLevel == 1) ? 3 : 2;
    collisionCount = 0;
    cameraX = 0;
    wandOffered = false;
    wandPurchased = false;
    wandUsesRemaining = WAND_USES_LIMIT;
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();
    bottomObstaclesPassed = 0;
    obstaclesStopped = false;
    waitingForGround = false;
    dragonX = SCREEN_WIDTH;
    dragonY = SCREEN_HEIGHT / 2 - DRAGON_HEIGHT / 2;
    dragonFrame = 0;
    lastDragonFrameTime = SDL_GetTicks();
    dragonStopped = false;
    dragonMovingDiagonally = false;
    isDragonFalling = false;
    dragonDeadFrame = 0;
    collectibleFloatOffset = 0.0f;
    boosterFrame = 0;
    boosterAnimTime = SDL_GetTicks();
    isShaking = false;
    playerAnimationStopped = false;
    dragonRoarChannel = -1;
    dragonDeadChannel = -1;
    wandEffectPlaying = false;
    wandEffectFrame = 0;
    wandEffectStartTime = 0;
    hidePlayer = false;
    waitingForDragonPause = false;
    dragonPauseStartTime = 0;
    bottomObstacles.clear();
    collectibles.clear();
    boosters.clear();
    topObstacles.clear();
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
}

void useWand(bool& wandPurchased, int& wandUsesRemaining, bool& wandOffered, bool& waitingForWand,
             bool& dragonStopped, bool& dragonMovingDiagonally, bool& wandEffectPlaying, int& wandEffectFrame,
             Uint32& wandEffectStartTime, bool& hidePlayer, int& dragonRoarChannel, SDL_Rect& player,
             std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles) {
    if (wandPurchased && wandUsesRemaining > 0 && !wandOffered) {
        SDL_Rect* closestObs = nullptr;
        int minDist = std::numeric_limits<int>::max();
        for (auto& obs : bottomObstacles) {
            if (obs.x > player.x && obs.x - player.x < minDist) {
                minDist = obs.x - player.x;
                closestObs = &obs;
            }
        }
        for (auto& obs : topObstacles) {
            if (obs.x > player.x && obs.x - player.x < minDist) {
                minDist = obs.x - player.x;
                closestObs = &obs;
            }
        }
        if (closestObs) {
            closestObs->x = -200;
            wandUsesRemaining--;
        }
        if (waitingForWand && dragonStopped) {
            waitingForWand = false;
            dragonStopped = false;
            dragonMovingDiagonally = false;
            wandEffectPlaying = true;
            wandEffectFrame = 0;
            wandEffectStartTime = SDL_GetTicks();
            wandUsesRemaining--;
            hidePlayer = true;
            if (dragonRoarChannel != -1) {
                Mix_HaltChannel(dragonRoarChannel);
                dragonRoarChannel = -1;
            }
        }
    }
}
*/
/*
#include "game_objects1.h"
#include "game_config1.h"
#include <vector>
#include <limits>

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
) {
    player.x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    player.y = GROUND_Y - PLAYER_HEIGHT;
    player.w = PLAYER_WIDTH;
    player.h = PLAYER_HEIGHT;
    velocityY = 0; isJumping = false; onPlatform = false; score = 0; collisionCount = 0;
    lives = 3; cameraX = 0; wandOffered = false; wandPurchased = false;
    wandUsesRemaining = WAND_USES_LIMIT; currentFrame = 0; lastFrameTime = SDL_GetTicks();
    bottomObstaclesPassed = 0; obstaclesStopped = false; waitingForGround = false;
    dragonX = SCREEN_WIDTH; dragonY = SCREEN_HEIGHT / 2 - DRAGON_HEIGHT / 2;
    dragonFrame = 0; lastDragonFrameTime = SDL_GetTicks(); dragonStopped = false;
    dragonMovingDiagonally = false; isDragonFalling = false; dragonDeadFrame = 0;
    goldenEggVisible = false; shakeStartTime = 0; isShaking = false;
    collectibleFloatOffset = 0.0f; boosterFrame = 0; boosterAnimTime = SDL_GetTicks();
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
    goldenEggRect.x = 0; goldenEggRect.y = 0; goldenEggRect.w = GOLDEN_EGG_SIZE; goldenEggRect.h = GOLDEN_EGG_SIZE;
}

void useWand(
    SDL_Rect& player, std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles,
    int& wandUsesRemaining, bool& wandOffered, bool& waitingForWand, bool& dragonStopped,
    bool& showLightning, bool& lightningHitDragon, Uint32& lightningStartTime, SDL_Rect& lightningRect,
    int& dragonRoarChannel
) {
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
            lightningRect.x = playerCenterX - lightningRect.w / 2;
            lightningRect.y = playerCenterY - lightningRect.h / 2;
            lightningRect.w = 100;
            lightningRect.h = 100;
            if (dragonRoarChannel != -1) {
                Mix_HaltChannel(dragonRoarChannel);
                dragonRoarChannel = -1;
            }
        }
    }
}
*/
#include "game_objects1.h"
#include "game_config1.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

void useWand(SDL_Rect& player, std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles, int& wandUsesRemaining, bool& wandOffered, bool& waitingForWand, bool& waitingForGround, bool& isShaking, bool& wandEffectPlaying, Uint32& wandEffectStartTime, SDL_Rect& goldenEggRect, int& score) {
    if (wandPurchased && wandUsesRemaining > 0 && !wandOffered) {
        wandUsesRemaining--;
        waitingForWand = true;
        wandEffectPlaying = true;
        wandEffectStartTime = SDL_GetTicks();
        // Additional wand logic here
    }
}
