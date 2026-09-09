/*#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

// Rendering functions for each game state
void renderIntro(GameState state, int introFrameIndex, const std::vector<SDL_Texture*>& introFrames);
void renderStory(GameState state, int storyIndex, const std::vector<std::string>& storySlides,
                 const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font,
                 const std::vector<SDL_Texture*>& textures);
void renderMenu(GameState state, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer,
                const std::vector<SDL_Texture*>& textures);
void renderLevelSelect(GameState state, bool level1Completed, const std::vector<SDL_Rect>& candles,
                      SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures);
void renderScoreboard(GameState state, int highScore, const std::vector<SDL_Rect>& candles,
                     SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures);
void renderCountdown(GameState state, int countDownValue, int currentLevel, float cameraX,
                     const SDL_Rect& player, int currentFrame, int boosterFrame, float collectibleFloatOffset,
                     const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles,
                     const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles,
                     SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures,
                     SDL_Texture* spriteSheet, int frameWidth, int frameHeight);
void renderPaused(GameState state, SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures);
void renderPlaying(GameState state, int currentLevel, float cameraX, const SDL_Rect& player, float velocityY,
                   bool isJumping, bool onPlatform, bool obstaclesStopped, bool dragonStopped,
                   bool dragonMovingDiagonally, bool isDragonFalling, bool goldenEggVisible, bool wandOffered,
                   bool wandPurchased, bool waitingForWand, bool waitingForGround, bool isShaking,
                   bool playerAnimationStopped, bool hidePlayer, bool wandEffectPlaying, bool waitingForDragonPause,
                   int score, int lives, int collisionCount, float collectibleFloatOffset, int currentFrame,
                   int dragonFrame, int dragonDeadFrame, int boosterFrame, int dragonX, int dragonY,
                   int bottomObstaclesPassed, int wandUsesRemaining, Uint32 wandEffectStartTime, int wandEffectFrame,
                   const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles,
                   const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles,
                   const SDL_Rect& goldenEggRect, SDL_Renderer* renderer, TTF_Font* font,
                   const std::vector<SDL_Texture*>& textures, SDL_Texture* spriteSheet, SDL_Texture* wandEffectSheet,
                   const std::vector<SDL_Texture*>& dragonFrames, SDL_Texture* dragonDead[3],
                   int frameWidth, int frameHeight, int wandEffectFrameWidth, int wandEffectFrameHeight);
void renderGameOver(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame,
                    int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer,
                    const std::vector<SDL_Texture*>& textures, SDL_Texture* spriteSheet,
                    SDL_Texture* dragonDead[3], int frameWidth, int frameHeight);
void renderCongrats(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame,
                    int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer,
                    TTF_Font* font, const std::vector<SDL_Texture*>& textures, SDL_Texture* spriteSheet,
                    SDL_Texture* dragonDead[3], int frameWidth, int frameHeight);

#endif
*/
/*
#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

void renderIntro(GameState state, int introFrameIndex, const std::vector<SDL_Texture*>& introFrames);
void renderStory(GameState state, int storyIndex, const std::vector<std::string>& storySlides,
                 const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font,
                 const std::vector<SDL_Texture*>& textures);
void renderMenu(GameState state, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer,
                const std::vector<SDL_Texture*>& textures);
void renderLevelSelect(GameState state, bool level1Completed, const std::vector<SDL_Rect>& candles,
                      SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures);
void renderScoreboard(GameState state, int highScore, const std::vector<SDL_Rect>& candles,
                     SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures);
void renderCountdown(GameState state, int countDownValue, int currentLevel, float cameraX,
                     const SDL_Rect& player, int currentFrame, int boosterFrame, float collectibleFloatOffset,
                     const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles,
                     const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles,
                     SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures,
                     SDL_Texture* spriteSheet, int frameWidth, int frameHeight);
void renderPaused(GameState state, SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures);
void renderPlaying(GameState state, int currentLevel, float cameraX, const SDL_Rect& player, float velocityY,
                   bool isJumping, bool onPlatform, bool obstaclesStopped, bool dragonStopped,
                   bool dragonMovingDiagonally, bool isDragonFalling, bool goldenEggVisible, bool wandOffered,
                   bool wandPurchased, bool waitingForWand, bool waitingForGround, bool isShaking,
                   bool playerAnimationStopped, bool hidePlayer, bool wandEffectPlaying, bool waitingForDragonPause,
                   int score, int lives, int collisionCount, float collectibleFloatOffset, int currentFrame,
                   int dragonFrame, int dragonDeadFrame, int boosterFrame, int dragonX, int dragonY,
                   int bottomObstaclesPassed, int wandUsesRemaining, Uint32 wandEffectStartTime, int wandEffectFrame,
                   const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles,
                   const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles,
                   const SDL_Rect& goldenEggRect, SDL_Renderer* renderer, TTF_Font* font,
                   const std::vector<SDL_Texture*>& textures, SDL_Texture* spriteSheet, SDL_Texture* wandEffectSheet,
                   const std::vector<SDL_Texture*>& dragonFrames, SDL_Texture* dragonDead[3],
                   int frameWidth, int frameHeight, int wandEffectFrameWidth, int wandEffectFrameHeight);
void renderGameOver(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame,
                    int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer,
                    const std::vector<SDL_Texture*>& textures, SDL_Texture* spriteSheet,
                    SDL_Texture* dragonDead[3], int frameWidth, int frameHeight);
void renderCongrats(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame,
                    int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer,
                    TTF_Font* font, const std::vector<SDL_Texture*>& textures, SDL_Texture* spriteSheet,
                    SDL_Texture* dragonDead[3], int frameWidth, int frameHeight);

#endif
*/
/*
#ifndef RENDERING1_H
#define RENDERING1_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "game_state.h"

void renderIntro(GameState state, int introFrameIndex, SDL_Renderer* renderer, SDL_Texture* textures[], const std::vector<SDL_Texture*>& introFrames);
void renderStory(GameState state, int storyIndex, const std::vector<std::string>& storySlides, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]);
void renderMenu(GameState state, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, SDL_Texture* textures[]);
void renderLevelSelect(GameState state, bool level1Completed, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]);
void renderScoreboard(GameState state, int highScore, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]);
void renderCountdown(GameState state, int countDownValue, int currentLevel, float cameraX, const SDL_Rect& player, int currentFrame, int boosterFrame, float collectibleFloatOffset, const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles, const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, int frameWidth, int frameHeight);
void renderPaused(GameState state, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]);
void renderPlaying(GameState state, int currentLevel, float cameraX, const SDL_Rect& player, float velocityY, bool isJumping, bool onPlatform, bool obstaclesStopped, bool dragonStopped, bool dragonMovingDiagonally, bool isDragonFalling, bool goldenEggVisible, bool wandOffered, bool wandPurchased, bool waitingForWand, bool waitingForGround, bool isShaking, bool playerAnimationStopped, bool hidePlayer, bool wandEffectPlaying, bool waitingForDragonPause, int score, int lives, int collisionCount, float collectibleFloatOffset, int currentFrame, int dragonFrame, int dragonDeadFrame, int boosterFrame, int dragonX, int dragonY, int bottomObstaclesPassed, int wandUsesRemaining, Uint32 wandEffectStartTime, int wandEffectFrame, const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles, const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles, const SDL_Rect& goldenEggRect, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* wandEffectSheet, const std::vector<SDL_Texture*>& dragonFrames, SDL_Texture* dragonDead[], int frameWidth, int frameHeight, int wandEffectFrameWidth, int wandEffectFrameHeight);
void renderGameOver(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame, int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* dragonDead[], int frameWidth, int frameHeight);
void renderCongrats(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame, int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* dragonDead[], int frameWidth, int frameHeight);

#endif // RENDERING1_H
*/

#ifndef RENDERING1_H
#define RENDERING1_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "game_state.h"

void renderIntro(GameState state, int introFrameIndex, SDL_Renderer* renderer, SDL_Texture* textures[], const std::vector<SDL_Texture*>& introFrames);
void renderStory(GameState state, int storyIndex, const std::vector<std::string>& storySlides, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]);
void renderMenu(GameState state, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, SDL_Texture* textures[]);
void renderLevelSelect(GameState state, bool level1Completed, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]);
void renderScoreboard(GameState state, int highScore, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]);
void renderCountdown(GameState state, int countDownValue, int currentLevel, float cameraX, const SDL_Rect& player, int currentFrame, int boosterFrame, float collectibleFloatOffset, const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles, const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, int frameWidth, int frameHeight);
void renderPaused(GameState state, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]);
void renderPlaying(GameState state, int currentLevel, float cameraX, const SDL_Rect& player, float velocityY, bool isJumping, bool onPlatform, bool obstaclesStopped, bool dragonStopped, bool dragonMovingDiagonally, bool isDragonFalling, bool goldenEggVisible, bool wandOffered, bool wandPurchased, bool waitingForWand, bool waitingForGround, bool isShaking, bool playerAnimationStopped, bool hidePlayer, bool wandEffectPlaying, bool waitingForDragonPause, int score, int lives, int collisionCount, float collectibleFloatOffset, int currentFrame, int dragonFrame, int dragonDeadFrame, int boosterFrame, int dragonX, int dragonY, int bottomObstaclesPassed, int wandUsesRemaining, Uint32 wandEffectStartTime, int wandEffectFrame, const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles, const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles, const SDL_Rect& goldenEggRect, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* wandEffectSheet, const std::vector<SDL_Texture*>& dragonFrames, SDL_Texture* dragonDead[], int frameWidth, int frameHeight, int wandEffectFrameWidth, int wandEffectFrameHeight, int shakeOffsetX, int shakeOffsetY);
void renderGameOver(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame, int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* dragonDead[], int frameWidth, int frameHeight);
void renderCongrats(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame, int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* dragonDead[], int frameWidth, int frameHeight);

#endif // RENDERING1_H
