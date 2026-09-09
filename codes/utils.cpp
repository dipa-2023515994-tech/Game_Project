#include "utils.h"
#include <fstream>
#include <algorithm>
#include <limits>
#include <cmath>

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    SDL_Rect intersection;
    if (!SDL_IntersectRect(&a, &b, &intersection)) return false;
    int areaA = a.w * a.h, areaB = b.w * b.h;
    int intersectionArea = intersection.w * intersection.h;
    return (intersectionArea / static_cast<float>(std::min(areaA, areaB))) * 100.0f >= 50.0f;
}

int loadHighScore(const std::string& filename) {
    std::ifstream file(filename);
    int hs = 0;
    if (file >> hs) return hs;
    return 0;
}

void saveHighScore(const std::string& filename, int score) {
    std::ofstream file(filename);
    if (file) file << score;
    else std::cerr << "Failed to save high score to " << filename << std::endl;
}

bool loadLevelCompletion(const std::string& filename) {
    std::ifstream file(filename);
    bool completed = false;
    if (file >> completed) return completed;
    return false;
}

void saveLevelCompletion(const std::string& filename, bool completed) {
    std::ofstream file(filename);
    if (file) file << completed;
    else std::cerr << "Failed to save level completion to " << filename << std::endl;
}

void updateGameState(GameData& data, GameResources& resources) {
    if (!data.obstaclesStopped && data.bottomObstacles.empty() && !data.waitingForGround) {
        if (!data.isJumping && !data.onPlatform && data.player.y == GROUND_Y - PLAYER_HEIGHT) {
            data.obstaclesStopped = true;
            Mix_HaltMusic();
            if (data.dragonRoarChannel == -1) {
                data.dragonRoarChannel = Mix_PlayChannel(-1, resources.dragonRoarSound, -1);
            }
        } else data.waitingForGround = true;
    }
    data.velocityY += 0.4f; data.player.y += static_cast<int>(data.velocityY);
    if (data.player.y > GROUND_Y - PLAYER_HEIGHT) {
        data.player.y = GROUND_Y - PLAYER_HEIGHT; data.velocityY = 0; data.isJumping = false; data.onPlatform = false; data.spacePressCount = 0;
        if (data.waitingForGround && data.bottomObstacles.empty()) {
            data.obstaclesStopped = true;
            Mix_HaltMusic();
            if (data.dragonRoarChannel == -1) {
                data.dragonRoarChannel = Mix_PlayChannel(-1, resources.dragonRoarSound, -1);
            }
            data.waitingForGround = false;
        }
    }
    bool landedOnPlatform = false;
    for (const auto& obs : data.topObstacles) {
        if (data.velocityY > 0 && data.player.y + data.player.h >= obs.y && data.player.y + data.player.h <= obs.y + obs.h &&
            data.player.x + data.player.w > obs.x && data.player.x < obs.x + obs.w) {
            data.player.y = obs.y - data.player.h; data.velocityY = 0; data.isJumping = false; data.onPlatform = true; data.spacePressCount = 0; landedOnPlatform = true;
        }
    }
    if (!landedOnPlatform && data.onPlatform && data.player.y + data.player.h < TOP_OBSTACLE_Y) data.onPlatform = false;
    if (!data.obstaclesStopped) {
        data.cameraX += PLAYER_AUTO_SPEED;
        data.player.x = static_cast<int>(SCREEN_WIDTH / 2 - data.cameraX);
        for (auto& obs : data.bottomObstacles) obs.x -= PLAYER_AUTO_SPEED;
        for (auto& col : data.collectibles) col.x -= PLAYER_AUTO_SPEED;
        for (auto& boost : data.boosters) boost.x -= PLAYER_AUTO_SPEED;
        for (auto& obs : data.topObstacles) obs.x -= TOP_OBSTACLE_SPEED;
        if (data.goldenEggVisible) data.goldenEggRect.x -= PLAYER_AUTO_SPEED;
        data.collectibleFloatOffset += FLOAT_SPEED;
        for (auto it = data.bottomObstacles.begin(); it != data.bottomObstacles.end();) {
            if (it->x + it->w < 0) it = data.bottomObstacles.erase(it); else ++it;
        }
        for (auto it = data.collectibles.begin(); it != data.collectibles.end();) {
            if (it->x + it->w < 0) it = data.collectibles.erase(it); else ++it;
        }
        for (auto it = data.boosters.begin(); it != data.boosters.end();) {
            if (it->x + it->w < 0) it = data.boosters.erase(it); else ++it;
        }
        for (auto it = data.topObstacles.begin(); it != data.topObstacles.end();) {
            if (it->x + it->w < 0) it = data.topObstacles.erase(it); else ++it;
        }
        if (!data.topObstacles.empty() && data.topObstacles.back().x + data.topObstacles.back().w < SCREEN_WIDTH - OBSTACLE_SPACING) {
            int newX = data.topObstacles.back().x + OBSTACLE_SPACING;
            data.topObstacles.push_back({newX, TOP_OBSTACLE_Y, TOP_OBSTACLE_WIDTH, TOP_OBSTACLE_HEIGHT});
            data.boosters.push_back({newX + (TOP_OBSTACLE_WIDTH - BOOSTER_SIZE) / 2, TOP_OBSTACLE_Y - BOOSTER_SIZE, BOOSTER_SIZE, BOOSTER_SIZE});
        }
        if (!data.bottomObstacles.empty() && data.bottomObstacles.back().x + data.bottomObstacles.back().w < SCREEN_WIDTH - OBSTACLE_SPACING) {
            int newX = data.bottomObstacles.back().x + OBSTACLE_SPACING;
            data.bottomObstacles.push_back({newX, GROUND_Y - 70, 70, 70});
            data.bottomObstacles.push_back({newX + 450, GROUND_Y - 70, 70, 70});
            data.collectibles.push_back({newX + 2 * 450 + 150, GROUND_Y - COLLECTIBLE_SIZE, COLLECTIBLE_SIZE, COLLECTIBLE_SIZE});
        }
        for (const auto& obs : data.bottomObstacles) {
            if (checkCollision(data.player, obs)) {
                data.lives--; data.isShaking = true; data.shakeStartTime = SDL_GetTicks();
                if (data.lives <= 0) {
                    if (data.score > data.highScore) {
                        data.highScore = data.score;
                        saveHighScore("assets/highscore.txt", data.highScore);
                    }
                    if (data.dragonRoarChannel != -1) {
                        Mix_HaltChannel(data.dragonRoarChannel);
                        data.dragonRoarChannel = -1;
                    }
                }
            }
        }
        for (const auto& obs : data.topObstacles) {
            if (checkCollision(data.player, obs) && !data.onPlatform) {
                data.lives--; data.isShaking = true; data.shakeStartTime = SDL_GetTicks();
                if (data.lives <= 0) {
                    if (data.score > data.highScore) {
                        data.highScore = data.score;
                        saveHighScore("assets/highscore.txt", data.highScore);
                    }
                    if (data.dragonRoarChannel != -1) {
                        Mix_HaltChannel(data.dragonRoarChannel);
                        data.dragonRoarChannel = -1;
                    }
                }
            }
        }
        for (auto it = data.collectibles.begin(); it != data.collectibles.end();) {
            if (checkCollision(data.player, *it)) {
                Mix_PlayChannel(-1, resources.collectibleSound, 0);
                data.score++; it = data.collectibles.erase(it);
                if (data.score == 10 && !data.wandPurchased) data.wandOffered = true;
            } else ++it;
        }
        for (auto it = data.boosters.begin(); it != data.boosters.end();) {
            if (checkCollision(data.player, *it)) {
                Mix_PlayChannel(-1, resources.boosterSound, 0);
                data.score += 3; it = data.boosters.erase(it);
                if (data.score == 10 && !data.wandPurchased) data.wandOffered = true;
            } else ++it;
        }
        if (data.bottomObstacles.size() <= MAX_OBSTACLES / 2) data.bottomObstaclesPassed++;
        if (data.bottomObstaclesPassed >= BOTTOM_OBSTACLES_BEFORE_TOP && !data.goldenEggVisible) {
            data.goldenEggRect.x = data.player.x + SCREEN_WIDTH + GOLDEN_EGG_DISTANCE;
            data.goldenEggRect.y = GROUND_Y - GOLDEN_EGG_SIZE;
            data.goldenEggVisible = true;
        }
        if (data.goldenEggVisible && checkCollision(data.player, data.goldenEggRect)) {
            data.score += 10; data.goldenEggVisible = false;
            if (!data.wandPurchased) data.wandOffered = true;
            if (data.currentLevel == 1) {
                data.level1Completed = true;
                saveLevelCompletion("assets/level1_completion.txt", true);
            }
        }
    }
    if (data.lives <= 0) {
        data.playerAnimationStopped = true;
        data.isShaking = false;
    }
    if (SDL_GetTicks() - data.lastFrameTime >= FRAME_DURATION && !data.playerAnimationStopped) {
        data.currentFrame = (data.currentFrame + 1) % FRAME_COUNT;
        data.lastFrameTime = SDL_GetTicks();
    }
    if (data.obstaclesStopped) {
        int playerCenterX = data.player.x + PLAYER_WIDTH / 2;
        int dragonCenterX = data.dragonX + DRAGON_WIDTH / 2;
        int distance = std::abs(dragonCenterX - playerCenterX);
        if (distance > DRAGON_STOP_DISTANCE && !data.isDragonFalling) {
            if (!data.dragonMovingDiagonally && distance < 2 * DRAGON_STOP_DISTANCE) {
                data.dragonMovingDiagonally = true; data.dragonStopTime = SDL_GetTicks();
            }
            if (data.dragonMovingDiagonally) {
                data.dragonX -= DRAGON_SPEED; data.dragonY -= DIAGONAL_SPEED;
                if (data.dragonY <= 0) data.dragonY = 0, data.dragonMovingDiagonally = false;
            } else data.dragonX -= DRAGON_SPEED;
        } else if (!data.isDragonFalling) {
            data.dragonStopped = true; data.waitingForWand = true;
        }
        if (SDL_GetTicks() - data.lastDragonFrameTime >= FRAME_DURATION && !data.isDragonFalling) {
            data.dragonFrame = (data.dragonFrame + 1) % FRAME_COUNT;
            data.lastDragonFrameTime = SDL_GetTicks();
        }
        if (data.showLightning && SDL_GetTicks() - data.lightningStartTime < LIGHTNING_DURATION) {
            int dragonCenterX = data.dragonX + DRAGON_WIDTH / 2;
            int dragonCenterY = data.dragonY + DRAGON_HEIGHT / 2;
            int lightningCenterX = data.lightningRect.x + data.lightningRect.w / 2;
            int lightningCenterY = data.lightningRect.y + data.lightningRect.h / 2;
            data.lightningRect.x += static_cast<int>(LIGHTNING_SPEED * (dragonCenterX - lightningCenterX) / std::sqrt(std::pow(dragonCenterX - lightningCenterX, 2) + std::pow(dragonCenterY - lightningCenterY, 2)));
            data.lightningRect.y += static_cast<int>(LIGHTNING_SPEED * (dragonCenterY - lightningCenterY) / std::sqrt(std::pow(dragonCenterX - lightningCenterX, 2) + std::pow(dragonCenterY - lightningCenterY, 2)));
            if (!data.lightningHitDragon) {
                SDL_Rect dragonRect = {data.dragonX, data.dragonY, DRAGON_WIDTH, DRAGON_HEIGHT};
                if (checkCollision(data.lightningRect, dragonRect)) {
                    data.lightningHitDragon = true; data.isDragonFalling = true;
                    data.dragonDeadFrame = 0;
                    if (data.dragonRoarChannel != -1) {
                        Mix_HaltChannel(data.dragonRoarChannel);
                        data.dragonRoarChannel = -1;
                    }
                    data.dragonDeadChannel = Mix_PlayChannel(-1, resources.dragonDeadSound, 0);
                }
            }
        } else if (data.showLightning && SDL_GetTicks() - data.lightningStartTime >= LIGHTNING_DURATION) {
            data.showLightning = false;
        }
        if (data.isDragonFalling) {
            data.dragonY += FALL_SPEED;
            if (SDL_GetTicks() - data.lastDragonFrameTime >= FRAME_DURATION) {
                data.dragonDeadFrame = (data.dragonDeadFrame + 1) % 3;
                data.lastDragonFrameTime = SDL_GetTicks();
            }
            if (data.dragonY >= GROUND_Y - DRAGON_HEIGHT) {
                data.dragonY = GROUND_Y - DRAGON_HEIGHT; data.isDragonFalling = false;
                if (data.dragonDeadChannel != -1) {
                    Mix_HaltChannel(data.dragonDeadChannel);
                    data.dragonDeadChannel = -1;
                }
                data.dragonDeadFrame = -1;
            }
        }
        if (data.dragonStopped && !data.isDragonFalling && data.dragonDeadFrame == -1 && !data.showLightning) {
            data.isShaking = true; data.shakeStartTime = SDL_GetTicks();
        }
    }
}
