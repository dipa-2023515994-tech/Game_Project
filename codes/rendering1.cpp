/*#include "rendering.h"
#include "game_config.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>

void renderIntro(GameState state, int introFrameIndex, const std::vector<SDL_Texture*>& introFrames) {
    if (state == INTRO && introFrameIndex < introFrames.size()) {
        if (introFrames[introFrameIndex]) {
            SDL_RenderCopy(renderer, introFrames[introFrameIndex], nullptr, nullptr);
        }
    }
}

void renderStory(GameState state, int storyIndex, const std::vector<std::string>& storySlides,
                 const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font,
                 const std::vector<SDL_Texture*>& textures) {
    if (state == STORY) {
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
}

void renderMenu(GameState state, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer,
                const std::vector<SDL_Texture*>& textures) {
    if (state == MENU) {
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
        SDL_RenderFillRect(renderer, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        for (auto& candle : candles) {
            candle.y -= 1;
            if (candle.y + candle.h < 0) candle.y = SCREEN_HEIGHT;
            SDL_RenderCopy(renderer, textures[21], nullptr, &candle);
        }
        SDL_Rect startBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.3), 300, 90};
        SDL_Rect scoreboardBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
        SDL_Rect instrBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5), 300, 90};
        SDL_Rect quitBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.6), 300, 90};
        SDL_RenderCopy(renderer, textures[13], nullptr, &startBtn);
        SDL_RenderCopy(renderer, textures[14], nullptr, &scoreboardBtn);
        SDL_RenderCopy(renderer, textures[15], nullptr, &instrBtn);
        SDL_RenderCopy(renderer, textures[16], nullptr, &quitBtn);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mouse = {mx, my};
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
    }
}

void renderLevelSelect(GameState state, bool level1Completed, const std::vector<SDL_Rect>& candles,
                      SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures) {
    if (state == LEVEL_SELECT) {
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, textures[8], nullptr, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
        SDL_RenderFillRect(renderer, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        for (auto& candle : candles) {
            candle.y -= 1;
            if (candle.y + candle.h < 0) candle.y = SCREEN_HEIGHT;
            SDL_RenderCopy(renderer, textures[21], nullptr, &candle);
        }
        SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
        SDL_Rect level2Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5 + 10), 300, 90};
        SDL_RenderCopy(renderer, textures[9], nullptr, &level1Btn);
        SDL_RenderCopy(renderer, level1Completed ? textures[10] : textures[11], nullptr, &level2Btn);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mouse = {mx, my};
        if (SDL_PointInRect(&mouse, &level1Btn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &level1Btn);
        if (SDL_PointInRect(&mouse, &level2Btn) && level1Completed) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &level2Btn);
        if (textures[20]) {
            SDL_Rect wandCursor = {(SDL_PointInRect(&mouse, &level1Btn) ? level1Btn.x : level2Btn.x) - 40,
                                  (SDL_PointInRect(&mouse, &level1Btn) ? level1Btn.y : level2Btn.y) + 10, 40, 80};
            SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
        }
        SDL_Texture* levelText = renderText(renderer, font, "Select Level");
        if (levelText) {
            int w, h;
            TTF_SizeText(font, "Select Level", &w, &h);
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
    }
}

void renderScoreboard(GameState state, int highScore, const std::vector<SDL_Rect>& candles,
                     SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures) {
    if (state == SCOREBOARD) {
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
        SDL_RenderFillRect(renderer, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        for (auto& candle : candles) {
            candle.y -= 1;
            if (candle.y + candle.h < 0) candle.y = SCREEN_HEIGHT;
            SDL_RenderCopy(renderer, textures[21], nullptr, &candle);
        }
        int scrollWidth = static_cast<int>(SCREEN_WIDTH * 0.4), scrollHeight = static_cast<int>(SCREEN_HEIGHT * 0.4);
        SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
        SDL_RenderCopy(renderer, textures[31], nullptr, &scrollRect);
        SDL_Texture* scoreText = renderText(renderer, font, "High Score: " + std::to_string(highScore));
        if (scoreText) {
            int w, h;
            TTF_SizeText(font, ("High Score: " + std::to_string(highScore)).c_str(), &w, &h);
            SDL_Rect textRect = {scrollRect.x + (scrollRect.w - w) / 2, scrollRect.y + scrollRect.h / 2 - h / 2, w, h};
            SDL_RenderCopy(renderer, scoreText, nullptr, &textRect);
            SDL_DestroyTexture(scoreText);
        }
        SDL_Rect backBtn = {scrollRect.x + scrollRect.w - 200, scrollRect.y + scrollRect.h - 100, 180, 80};
        SDL_RenderCopy(renderer, textures[17], nullptr, &backBtn);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mouse = {mx, my};
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

void renderCountdown(GameState state, int countDownValue, int currentLevel, float cameraX,
                     const SDL_Rect& player, int currentFrame, int boosterFrame, float collectibleFloatOffset,
                     const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles,
                     const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles,
                     SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures,
                     SDL_Texture* spriteSheet, int frameWidth, int frameHeight) {
    if (state == COUNTDOWN) {
        int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
        SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
        SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
        SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
        for (const auto& obs : bottomObstacles) SDL_RenderCopy(renderer, textures[2], nullptr, &obs);
        for (const auto& obs : topObstacles) SDL_RenderCopy(renderer, textures[30], nullptr, &obs);
        for (const auto& col : collectibles) {
            SDL_Rect floatRect = {col.x, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)), col.w, col.h};
            SDL_RenderCopy(renderer, textures[3], nullptr, &floatRect);
        }
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
            int w, h;
            TTF_SizeText(font, std::to_string(countDownValue).c_str(), &w, &h);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h};
            SDL_RenderCopy(renderer, countDownText, nullptr, &textRect);
            SDL_DestroyTexture(countDownText);
        }
    }
}

void renderPaused(GameState state, SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures) {
    if (state == PAUSED) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlayRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_Texture* pausedText = renderText(renderer, font, "Paused");
        if (pausedText) {
            int w, h;
            TTF_SizeText(font, "Paused", &w, &h);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h};
            SDL_RenderCopy(renderer, pausedText, nullptr, &textRect);
            SDL_DestroyTexture(pausedText);
        }
    }
}

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
                   int frameWidth, int frameHeight, int wandEffectFrameWidth, int wandEffectFrameHeight) {
    if (state == PLAYING) {
        int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
        SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
        SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
        SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
        for (const auto& obs : bottomObstacles) SDL_RenderCopy(renderer, textures[2], nullptr, &obs);
        for (const auto& obs : topObstacles) SDL_RenderCopy(renderer, textures[30], nullptr, &obs);
        for (const auto& col : collectibles) {
            SDL_Rect floatRect = {col.x, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)), col.w, col.h};
            SDL_RenderCopy(renderer, textures[3], nullptr, &floatRect);
        }
        for (const auto& boost : boosters) SDL_RenderCopy(renderer, textures[5 + boosterFrame], nullptr, &boost);
        if (!hidePlayer) {
            SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
            SDL_Rect drawPlayer = {player.x, player.y, player.h, player.h};
            SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
        }
        if (wandEffectPlaying) {
            SDL_Rect srcWandEffect = {wandEffectFrame * wandEffectFrameWidth, 0, wandEffectFrameWidth, wandEffectFrameHeight};
            SDL_Rect destWandEffect = {player.x + player.w / 2, player.y - WAND_EFFECT_HEIGHT / 2, WAND_EFFECT_WIDTH, WAND_EFFECT_HEIGHT};
            SDL_RenderCopy(renderer, wandEffectSheet, &srcWandEffect, &destWandEffect);
        }
        if (currentLevel == 2) {
            if (!dragonStopped && !isDragonFalling) {
                SDL_Rect dragonRect = {dragonX, dragonY, DRAGON_WIDTH, DRAGON_HEIGHT};
                SDL_RenderCopy(renderer, dragonFrames[dragonFrame], nullptr, &dragonRect);
            } else {
                SDL_Rect dragonRect = {dragonX, dragonY, DRAGON_WIDTH, DRAGON_HEIGHT};
                SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonRect);
            }
        }
        if (goldenEggVisible) {
            SDL_Rect drawEgg = {goldenEggRect.x, goldenEggRect.y, goldenEggRect.w, goldenEggRect.h};
            SDL_RenderCopy(renderer, textures[0], nullptr, &drawEgg);
        }
        SDL_Rect scoreIcon = {10, 10, 50, 50};
        SDL_RenderCopy(renderer, textures[26], nullptr, &scoreIcon);
        SDL_Texture* scoreText = renderText(renderer, font, std::to_string(score));
        if (scoreText) {
            int w, h;
            TTF_SizeText(font, std::to_string(score).c_str(), &w, &h);
            SDL_Rect textRect = {70, 10, w, h};
            SDL_RenderCopy(renderer, scoreText, nullptr, &textRect);
            SDL_DestroyTexture(scoreText);
        }
        SDL_Rect lifeIcon = {10, 70, 50, 50};
        SDL_RenderCopy(renderer, textures[27], nullptr, &lifeIcon);
        SDL_Texture* livesText = renderText(renderer, font, std::to_string(lives));
        if (livesText) {
            int w, h;
            TTF_SizeText(font, std::to_string(lives).c_str(), &w, &h);
            SDL_Rect textRect = {70, 70, w, h};
            SDL_RenderCopy(renderer, livesText, nullptr, &textRect);
            SDL_DestroyTexture(livesText);
        }
        if (wandPurchased) {
            SDL_Rect wandIcon = {SCREEN_WIDTH - 70, 120, 50, 50};
            SDL_RenderCopy(renderer, textures[29], nullptr, &wandIcon);
            SDL_Texture* wandText = renderText(renderer, font, std::to_string(wandUsesRemaining));
            if (wandText) {
                int w, h;
                TTF_SizeText(font, std::to_string(wandUsesRemaining).c_str(), &w, &h);
                SDL_Rect wandTextRect = {SCREEN_WIDTH - 70, 180, w, h};
                SDL_RenderCopy(renderer, wandText, nullptr, &wandTextRect);
                SDL_DestroyTexture(wandText);
            }
        }
        if (wandOffered) {
            int popupWidth = static_cast<int>(SCREEN_WIDTH * 0.45);
            int popupHeight = static_cast<int>(SCREEN_HEIGHT * 0.45);
            SDL_Rect popupRect = {SCREEN_WIDTH / 2 - popupWidth / 2, SCREEN_HEIGHT / 2 - popupHeight / 2, popupWidth, popupHeight};
            SDL_RenderCopy(renderer, textures[4], nullptr, &popupRect);
            SDL_Rect buyBtn = {popupRect.x + (popupRect.w - 360) / 2, popupRect.y + popupRect.h - 90, 180, 80};
            SDL_Rect laterBtn = {buyBtn.x + 180, buyBtn.y, 180, 80};
            SDL_RenderCopy(renderer, textures[24], nullptr, &buyBtn);
            SDL_RenderCopy(renderer, textures[25], nullptr, &laterBtn);
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            SDL_Point mouse = {mx, my};
            if (SDL_PointInRect(&mouse, &buyBtn)) {
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                SDL_RenderDrawRect(renderer, &buyBtn);
            }
            if (SDL_PointInRect(&mouse, &laterBtn)) {
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                SDL_RenderDrawRect(renderer, &laterBtn);
            }
            if (textures[20]) {
                SDL_Rect wandCursor = {(SDL_PointInRect(&mouse, &buyBtn) ? buyBtn.x : laterBtn.x) - 40,
                                      (SDL_PointInRect(&mouse, &buyBtn) ? buyBtn.y : laterBtn.y) + 10, 40, 80};
                SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
            }
        }
    }
}

void renderGameOver(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame,
                    int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer,
                    const std::vector<SDL_Texture*>& textures, SDL_Texture* spriteSheet,
                    SDL_Texture* dragonDead[3], int frameWidth, int frameHeight) {
    if (state == GAME_OVER) {
        int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
        SDL_Rect bgRect = {shakeOffsetX, shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, &bgRect);
        SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
        SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
        SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
        SDL_Rect drawPlayer = {player.x + shakeOffsetX, player.y + shakeOffsetY, player.h, player.h};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
        SDL_Rect dragonRect = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
        SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlayRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_Rect gameOverRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        SDL_RenderCopy(renderer, textures[37], nullptr, &gameOverRect);
        SDL_Rect playAgainBtn = {SCREEN_WIDTH / 4 + (SCREEN_WIDTH / 2 - 300) / 2, SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 2 - 90), 300, 90};
        SDL_RenderCopy(renderer, textures[38], nullptr, &playAgainBtn);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mouse = {mx, my};
        if (SDL_PointInRect(&mouse, &playAgainBtn)) {
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_RenderDrawRect(renderer, &playAgainBtn);
        }
        if (textures[20]) {
            SDL_Rect wandCursor = {playAgainBtn.x - 40, playAgainBtn.y + 10, 40, 80};
            SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
        }
    }
}

void renderCongrats(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame,
                    int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer,
                    TTF_Font* font, const std::vector<SDL_Texture*>& textures, SDL_Texture* spriteSheet,
                    SDL_Texture* dragonDead[3], int frameWidth, int frameHeight) {
    if (state == CONGRATS) {
        int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
        SDL_Rect bgRect = {shakeOffsetX, shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, &bgRect);
        SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
        SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
        SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
        SDL_Rect drawPlayer = {player.x + shakeOffsetX, player.y + shakeOffsetY, player.h, player.h};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
        SDL_Rect dragonRect = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
        SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlayRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_Texture* congratsText = renderText(renderer, font, "Congratulations! Level Completed!");
        if (congratsText) {
            int w, h;
            TTF_SizeText(font, "Congratulations! Level Completed!", &w, &h);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h};
            SDL_RenderCopy(renderer, congratsText, nullptr, &textRect);
            SDL_DestroyTexture(congratsText);
        }
    }
}
*/
/*
#include "rendering1.h"
#include "game_config1.h"
#include "utils1.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>

void renderIntro(GameState state, int introFrameIndex, const std::vector<SDL_Texture*>& introFrames) {
    if (state == INTRO && introFrameIndex < introFrames.size()) {
        if (introFrames[introFrameIndex]) {
            SDL_RenderCopy(renderer, introFrames[introFrameIndex], nullptr, nullptr);
        }
    }
}

void renderStory(GameState state, int storyIndex, const std::vector<std::string>& storySlides,
                 const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font,
                 const std::vector<SDL_Texture*>& textures) {
    if (state == STORY) {
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
}

void renderMenu(GameState state, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer,
                const std::vector<SDL_Texture*>& textures) {
    if (state == MENU) {
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
        SDL_RenderFillRect(renderer, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        for (auto& candle : candles) {
            candle.y -= 1;
            if (candle.y + candle.h < 0) candle.y = SCREEN_HEIGHT;
            SDL_RenderCopy(renderer, textures[21], nullptr, &candle);
        }
        SDL_Rect startBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.3), 300, 90};
        SDL_Rect scoreboardBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
        SDL_Rect instrBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5), 300, 90};
        SDL_Rect quitBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.6), 300, 90};
        SDL_RenderCopy(renderer, textures[13], nullptr, &startBtn);
        SDL_RenderCopy(renderer, textures[14], nullptr, &scoreboardBtn);
        SDL_RenderCopy(renderer, textures[15], nullptr, &instrBtn);
        SDL_RenderCopy(renderer, textures[16], nullptr, &quitBtn);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mouse = {mx, my};
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
    }
}

void renderLevelSelect(GameState state, bool level1Completed, const std::vector<SDL_Rect>& candles,
                      SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures) {
    if (state == LEVEL_SELECT) {
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, textures[8], nullptr, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
        SDL_RenderFillRect(renderer, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        for (auto& candle : candles) {
            candle.y -= 1;
            if (candle.y + candle.h < 0) candle.y = SCREEN_HEIGHT;
            SDL_RenderCopy(renderer, textures[21], nullptr, &candle);
        }
        SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
        SDL_Rect level2Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5 + 10), 300, 90};
        SDL_RenderCopy(renderer, textures[9], nullptr, &level1Btn);
        SDL_RenderCopy(renderer, level1Completed ? textures[10] : textures[11], nullptr, &level2Btn);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mouse = {mx, my};
        if (SDL_PointInRect(&mouse, &level1Btn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &level1Btn);
        if (SDL_PointInRect(&mouse, &level2Btn) && level1Completed) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &level2Btn);
        if (textures[20]) {
            SDL_Rect wandCursor = {(SDL_PointInRect(&mouse, &level1Btn) ? level1Btn.x : level2Btn.x) - 40,
                                  (SDL_PointInRect(&mouse, &level1Btn) ? level1Btn.y : level2Btn.y) + 10, 40, 80};
            SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
        }
        SDL_Texture* levelText = renderText(renderer, font, "Select Level");
        if (levelText) {
            int w, h;
            TTF_SizeText(font, "Select Level", &w, &h);
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
    }
}

void renderScoreboard(GameState state, int highScore, const std::vector<SDL_Rect>& candles,
                     SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures) {
    if (state == SCOREBOARD) {
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
        SDL_RenderFillRect(renderer, &bgRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        for (auto& candle : candles) {
            candle.y -= 1;
            if (candle.y + candle.h < 0) candle.y = SCREEN_HEIGHT;
            SDL_RenderCopy(renderer, textures[21], nullptr, &candle);
        }
        int scrollWidth = static_cast<int>(SCREEN_WIDTH * 0.4), scrollHeight = static_cast<int>(SCREEN_HEIGHT * 0.4);
        SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
        SDL_RenderCopy(renderer, textures[31], nullptr, &scrollRect);
        SDL_Texture* scoreText = renderText(renderer, font, "High Score: " + std::to_string(highScore));
        if (scoreText) {
            int w, h;
            TTF_SizeText(font, ("High Score: " + std::to_string(highScore)).c_str(), &w, &h);
            SDL_Rect textRect = {scrollRect.x + (scrollRect.w - w) / 2, scrollRect.y + scrollRect.h / 2 - h / 2, w, h};
            SDL_RenderCopy(renderer, scoreText, nullptr, &textRect);
            SDL_DestroyTexture(scoreText);
        }
        SDL_Rect backBtn = {scrollRect.x + scrollRect.w - 200, scrollRect.y + scrollRect.h - 100, 180, 80};
        SDL_RenderCopy(renderer, textures[17], nullptr, &backBtn);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mouse = {mx, my};
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

void renderCountdown(GameState state, int countDownValue, int currentLevel, float cameraX,
                     const SDL_Rect& player, int currentFrame, int boosterFrame, float collectibleFloatOffset,
                     const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles,
                     const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles,
                     SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures,
                     SDL_Texture* spriteSheet, int frameWidth, int frameHeight) {
    if (state == COUNTDOWN) {
        int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
        SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
        SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
        SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
        for (const auto& obs : bottomObstacles) SDL_RenderCopy(renderer, textures[2], nullptr, &obs);
        for (const auto& obs : topObstacles) SDL_RenderCopy(renderer, textures[30], nullptr, &obs);
        for (const auto& col : collectibles) {
            SDL_Rect floatRect = {col.x, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)), col.w, col.h};
            SDL_RenderCopy(renderer, textures[3], nullptr, &floatRect);
        }
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
            int w, h;
            TTF_SizeText(font, std::to_string(countDownValue).c_str(), &w, &h);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h};
            SDL_RenderCopy(renderer, countDownText, nullptr, &textRect);
            SDL_DestroyTexture(countDownText);
        }
    }
}

void renderPaused(GameState state, SDL_Renderer* renderer, TTF_Font* font, const std::vector<SDL_Texture*>& textures) {
    if (state == PAUSED) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlayRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_Texture* pausedText = renderText(renderer, font, "Paused");
        if (pausedText) {
            int w, h;
            TTF_SizeText(font, "Paused", &w, &h);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h};
            SDL_RenderCopy(renderer, pausedText, nullptr, &textRect);
            SDL_DestroyTexture(pausedText);
        }
    }
}

void renderPlaying(GameState state, int currentLevel, float cameraX, const SDL_Rect& player, float velocityY,
                   bool isJumping, bool onPlatform, bool obstaclesStopped, bool dragonStopped,
                   bool dragonMovingDiagonally, bool isDragonFalling, bool goldenEggVisible, bool wandOffered,
                   bool wandPurchased, bool waitingForWand, bool waitingForGround, bool isShaking,
                   bool playerAnimationStopped, bool hidePlayer, bool wandEffectPlaying, bool waitingForDragonPause,
                   int score, int lives, int collisionCount, float collectibleFloatOffset, int currentFrame,
                   int dragonFrame, int dragonDeadFrame, int boosterFrame, int dragonX, int dragonY,
                   int bottomObstaclesPassed, int wandUsesRemaining, Uint32 wandEffectStartTime, int wandEffect
                   */
                   
    /*
   #include "rendering1.h"
#include "utils1.h"
#include <cmath>

void renderIntro(SDL_Renderer* renderer, int introFrameIndex, Uint32 introStartTime) {
    if (introFrameIndex < introFrames.size() && introFrames[introFrameIndex]) {
        if (SDL_GetTicks() - introStartTime >= FRAME_DURATION) {
            // Note: State transition handled in main loop
        }
        SDL_RenderCopy(renderer, introFrames[introFrameIndex], nullptr, nullptr);
    }
}

void renderStory(SDL_Renderer* renderer, int storyIndex, int mx, int my) {
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

void renderMenu(SDL_Renderer* renderer, int mx, int my, std::vector<SDL_Rect>& candles) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
    SDL_RenderFillRect(renderer, &bgRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    for (auto& candle : candles) {
        candle.y -= 1; if (candle.y + candle.h < 0) candle.y = SCREEN_HEIGHT;
        SDL_RenderCopy(renderer, textures[21], nullptr, &candle);
    }
    SDL_Rect startBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.3), 300, 90};
    SDL_Rect scoreboardBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
    SDL_Rect instrBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5), 300, 90};
    SDL_Rect quitBtn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.6), 300, 90};
    SDL_RenderCopy(renderer, textures[13], nullptr, &startBtn);
    SDL_RenderCopy(renderer, textures[14], nullptr, &scoreboardBtn);
    SDL_RenderCopy(renderer, textures[15], nullptr, &instrBtn);
    SDL_RenderCopy(renderer, textures[16], nullptr, &quitBtn);
    SDL_Point mouse = {mx, my};
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
}

void renderLevelSelect(SDL_Renderer* renderer, int mx, int my) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[8], nullptr, &bgRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
    SDL_RenderFillRect(renderer, &bgRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_Rect level1Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.4), 300, 90};
    SDL_Rect level2Btn = {SCREEN_WIDTH / 2 - 150, static_cast<int>(SCREEN_HEIGHT * 0.5 + 10), 300, 90};
    SDL_RenderCopy(renderer, textures[9], nullptr, &level1Btn);
    SDL_RenderCopy(renderer, textures[11], nullptr, &level2Btn);
    SDL_Point mouse = {mx, my};
    if (SDL_PointInRect(&mouse, &level1Btn)) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255), SDL_RenderDrawRect(renderer, &level1Btn);
    if (textures[20]) {
        SDL_Rect wandCursor = {level1Btn.x - 40, level1Btn.y + 10, 40, 80};
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
}

void renderScoreboard(SDL_Renderer* renderer, int highScore, int mx, int my) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20);
    SDL_RenderFillRect(renderer, &bgRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
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
    SDL_Point mouse = {mx, my};
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

void renderCountdown(SDL_Renderer* renderer, int countDownValue, float cameraX, SDL_Rect& player,
                     int currentFrame, std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles,
                     std::vector<SDL_Rect>& collectibles, float collectibleFloatOffset,
                     std::vector<SDL_Rect>& boosters, int boosterFrame) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
    for (const auto& obs : bottomObstacles) SDL_RenderCopy(renderer, textures[2], nullptr, &obs);
    for (const auto& obs : topObstacles) SDL_RenderCopy(renderer, textures[30], nullptr, &obs);
    for (const auto& col : collectibles) {
        SDL_Rect floatRect = {col.x, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)), col.w, col.h};
        SDL_RenderCopy(renderer, textures[3], nullptr, &floatRect);
    }
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
}

void renderPaused(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, textures[32], nullptr, nullptr);
    SDL_Texture* pauseText = renderText(renderer, font, "Paused - Press P to Resume");
    if (pauseText) {
        SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 25, 500, 50};
        SDL_RenderCopy(renderer, pauseText, nullptr, &pauseRect);
        SDL_DestroyTexture(pauseText);
    }
}

void renderPlaying(SDL_Renderer* renderer, SDL_Rect& player, float cameraX, int currentFrame,
                   std::vector<SDL_Rect>& bottomObstacles, std::vector<SDL_Rect>& topObstacles,
                   std::vector<SDL_Rect>& collectibles, float collectibleFloatOffset,
                   std::vector<SDL_Rect>& boosters, int boosterFrame, bool obstaclesStopped,
                   int dragonX, int dragonY, int dragonFrame, bool isDragonFalling, int dragonDeadFrame,
                   bool goldenEggVisible, SDL_Rect& goldenEggRect, bool wandOffered, int score,
                   int lives, int wandUsesRemaining, bool wandPurchased, bool isShaking,
                   int shakeOffsetX, int shakeOffsetY, bool showLightning, SDL_Rect& lightningRect,
                   Uint32 lightningStartTime) {
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
    for (const auto& boost : boosters) SDL_RenderCopy(renderer, textures[5 + boosterFrame], nullptr, &boost);
    if (goldenEggVisible) {
        SDL_Rect floatEggRect = {goldenEggRect.x + shakeOffsetX, goldenEggRect.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)) + shakeOffsetY, goldenEggRect.w, goldenEggRect.h};
        SDL_RenderCopy(renderer, textures[0], nullptr, &floatEggRect);
    }
    SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_Rect drawPlayer = {player.x + shakeOffsetX, player.y + shakeOffsetY, player.h, player.h};
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    if (obstaclesStopped) {
        SDL_Rect dragonDst = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
        if (isDragonFalling && dragonDeadFrame >= 0) SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
        else if (!isDragonFalling) SDL_RenderCopy(renderer, dragonFrames[dragonFrame], nullptr, &dragonDst);
    }
    if (showLightning && SDL_GetTicks() - lightningStartTime < LIGHTNING_DURATION) {
        int dragonCenterX = dragonX + DRAGON_WIDTH / 2;
        int dragonCenterY = dragonY + DRAGON_HEIGHT / 2;
        int playerCenterX = player.x + PLAYER_WIDTH / 2;
        int playerCenterY = player.y + PLAYER_HEIGHT / 2;
        float angle = std::atan2(dragonCenterY - playerCenterY, dragonCenterX - playerCenterX) * 180 / M_PI;
        SDL_Rect lightningDrawRect = {lightningRect.x + shakeOffsetX, lightningRect.y + shakeOffsetY, lightningRect.w, lightningRect.h};
        SDL_RenderCopyEx(renderer, textures[39], nullptr, &lightningDrawRect, angle, nullptr, SDL_FLIP_NONE);
    }
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

void renderGameOver(SDL_Renderer* renderer, SDL_Rect& player, float cameraX, int currentFrame,
                    int dragonX, int dragonY, int dragonDeadFrame, int mx, int my) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[1], nullptr, &groundRect);
    SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_Rect drawPlayer = {player.x, player.y, player.w, player.h};
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    if (dragonDeadFrame >= 0) {
        SDL_Rect dragonDst = {dragonX, dragonY, DRAGON_WIDTH, DRAGON_HEIGHT};
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
    SDL_Point mouse = {mx, my};
    if (SDL_PointInRect(&mouse, &playAgainBtn)) {
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(renderer, &playAgainBtn);
    }
    if (textures[20]) {
        SDL_Rect wandCursor = {playAgainBtn.x - 40, playAgainBtn.y + 10, 40, 80};
        SDL_RenderCopy(renderer, textures[20], nullptr, &wandCursor);
    }
}

void renderCongrats(SDL_Renderer* renderer, SDL_Rect& player, float cameraX, int currentFrame,
                    int dragonX, int dragonY, int dragonDeadFrame, int shakeOffsetX, int shakeOffsetY) {
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
    SDL_Rect congratsRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_RenderFillRect(renderer, &congratsRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_Texture* congratsText = renderText(renderer, font, "Congratulations! Level Completed!");
    if (congratsText) {
        int w, h; TTF_SizeText(font, "Congratulations! Level Completed!", &w, &h);
        SDL_Rect textRect = {congratsRect.x + (congratsRect.w - w) / 2, congratsRect.y + (congratsRect.h - h) / 2, w, h};
        SDL_RenderCopy(renderer, congratsText, nullptr, &textRect);
        SDL_DestroyTexture(congratsText);
    }
} 
    */
    /*
   #include "rendering1.h"
#include "game_config1.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

void renderIntro(GameState state, int introFrameIndex, SDL_Renderer* renderer, SDL_Texture* textures[], const std::vector<SDL_Texture*>& introFrames) {
    if (introFrameIndex < introFrames.size() && introFrames[introFrameIndex]) {
        if (SDL_GetTicks() - introStartTime >= FRAME_DURATION) {
            // Assuming introStartTime is managed elsewhere
            SDL_RenderCopy(renderer, introFrames[introFrameIndex], nullptr, nullptr);
        }
    }
}

void renderStory(GameState state, int storyIndex, const std::vector<std::string>& storySlides, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[35], nullptr, &bgRect);
    
    SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &scrollRect);
    
    if (storyIndex < storySlides.size()) {
        SDL_Texture* slideText = renderText(renderer, font, storySlides[storyIndex], scrollRect.w - 120);
        SDL_Rect textRect = {scrollRect.x + 60, scrollRect.y + 60, 0, 0};
        SDL_QueryTexture(slideText, nullptr, nullptr, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, slideText, nullptr, &textRect);
        SDL_DestroyTexture(slideText);
    }
}

void renderMenu(GameState state, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, SDL_Texture* textures[]) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
    // Additional rendering logic for menu (buttons, etc.) assumed to be implemented
}

void renderLevelSelect(GameState state, bool level1Completed, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[8], nullptr, &bgRect);
    
    SDL_Rect levelRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &levelRect);
    
    SDL_Texture* levelText = renderText(renderer, font, "Select Level");
    SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25, 0, 0};
    SDL_QueryTexture(levelText, nullptr, nullptr, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, levelText, nullptr, &textRect);
    SDL_DestroyTexture(levelText);
}

void renderScoreboard(GameState state, int highScore, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
    
    SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &scrollRect);
    
    SDL_Texture* scoreText = renderText(renderer, font, "High Score: " + std::to_string(highScore));
    SDL_Rect textRect = {scrollRect.x + 60, scrollRect.y + 60, 0, 0};
    SDL_QueryTexture(scoreText, nullptr, nullptr, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, scoreText, nullptr, &textRect);
    SDL_DestroyTexture(scoreText);
}

void renderCountdown(GameState state, int countDownValue, int currentLevel, float cameraX, const SDL_Rect& player, int currentFrame, int boosterFrame, float collectibleFloatOffset, const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles, const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, int frameWidth, int frameHeight) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    
    SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[39], nullptr, &groundRect);
    
    for (const auto& col : collectibles) {
        SDL_Rect floatRect = {col.x, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)), col.w, col.h};
        SDL_RenderCopy(renderer, textures[0], nullptr, &floatRect);
    }
    
    SDL_Rect drawPlayer = player;
    SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    
    SDL_Texture* countDownText = renderText(renderer, font, std::to_string(countDownValue));
    SDL_Rect textRect = {SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 0, 0};
    SDL_QueryTexture(countDownText, nullptr, nullptr, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, countDownText, nullptr, &textRect);
    SDL_DestroyTexture(countDownText);
}

void renderPaused(GameState state, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]) {
    SDL_RenderCopy(renderer, textures[32], nullptr, nullptr);
    
    SDL_Texture* pauseText = renderText(renderer, font, "Paused - Press P to Resume");
    SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 25, 500, 50};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_RenderFillRect(renderer, &pauseRect);
    SDL_RenderCopy(renderer, pauseText, nullptr, &pauseRect);
    SDL_DestroyTexture(pauseText);
}

void renderPlaying(GameState state, int currentLevel, float cameraX, const SDL_Rect& player, float velocityY, bool isJumping, bool onPlatform, bool obstaclesStopped, bool dragonStopped, bool dragonMovingDiagonally, bool isDragonFalling, bool goldenEggVisible, bool wandOffered, bool wandPurchased, bool waitingForWand, bool waitingForGround, bool isShaking, bool playerAnimationStopped, bool hidePlayer, bool wandEffectPlaying, bool waitingForDragonPause, int score, int lives, int collisionCount, float collectibleFloatOffset, int currentFrame, int dragonFrame, int dragonDeadFrame, int boosterFrame, int dragonX, int dragonY, int bottomObstaclesPassed, int wandUsesRemaining, Uint32 wandEffectStartTime, int wandEffectFrame, const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles, const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles, const SDL_Rect& goldenEggRect, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* wandEffectSheet, const std::vector<SDL_Texture*>& dragonFrames, SDL_Texture* dragonDead[], int frameWidth, int frameHeight, int wandEffectFrameWidth, int wandEffectFrameHeight) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    
    SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[39], nullptr, &groundRect);
    
    for (const auto& col : collectibles) {
        SDL_Rect floatRect = {col.x + shakeOffsetX, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)) + shakeOffsetY, col.w, col.h};
        SDL_RenderCopy(renderer, textures[0], nullptr, &floatRect);
    }
    
    if (goldenEggVisible) {
        SDL_Rect eggRect = {goldenEggRect.x + shakeOffsetX, goldenEggRect.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)) + shakeOffsetY, goldenEggRect.w, goldenEggRect.h};
        SDL_RenderCopy(renderer, textures[0], nullptr, &eggRect);
    }
    
    if (!hidePlayer) {
        SDL_Rect drawPlayer = player;
        drawPlayer.x += shakeOffsetX;
        drawPlayer.y += shakeOffsetY;
        SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    }
    
    SDL_Rect dragonDst = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
    if (isDragonFalling && dragonDeadFrame >= 0) {
        SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
    } else if (!isDragonFalling) {
        SDL_RenderCopy(renderer, dragonFrames[dragonFrame], nullptr, &dragonDst);
    }
    
    if (wandOffered && !wandPurchased) {
        SDL_Rect popupRect = {SCREEN_WIDTH / 2 - popupWidth / 2, SCREEN_HEIGHT / 2 - popupHeight / 2, popupWidth, popupHeight};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
        SDL_RenderFillRect(renderer, &popupRect);
        SDL_Texture* wandText = renderText(renderer, font, "Purchase Wand for 10 points?");
        SDL_Rect textRect = {popupRect.x + 10, popupRect.y + 10, 0, 0};
        SDL_QueryTexture(wandText, nullptr, nullptr, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, wandText, nullptr, &textRect);
        SDL_DestroyTexture(wandText);
    }
    
    SDL_Texture* scoreText = renderText(renderer, font, std::to_string(score));
    SDL_Rect scoreRect = {10, 10, 0, 0};
    SDL_QueryTexture(scoreText, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
    SDL_RenderCopy(renderer, scoreText, nullptr, &scoreRect);
    SDL_DestroyTexture(scoreText);
}

void renderGameOver(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame, int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* dragonDead[], int frameWidth, int frameHeight) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    
    SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[39], nullptr, &groundRect);
    
    SDL_Rect drawPlayer = player;
    SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    
    SDL_Rect dragonDst = {dragonX, dragonY, DRAGON_WIDTH, DRAGON_HEIGHT};
    SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
}

void renderCongrats(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame, int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* dragonDead[], int frameWidth, int frameHeight) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    
    SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[39], nullptr, &groundRect);
    
    SDL_Rect drawPlayer = player;
    drawPlayer.x += shakeOffsetX;
    drawPlayer.y += shakeOffsetY;
    SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    
    SDL_Rect dragonDst = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
    SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
    
    SDL_Rect congratsRect = {SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 25, 500, 50};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_RenderFillRect(renderer, &congratsRect);
    
    SDL_Texture* congratsText = renderText(renderer, font, "Congratulations! Level Completed!");
    SDL_RenderCopy(renderer, congratsText, nullptr, &congratsRect);
    SDL_DestroyTexture(congratsText);
} 
*/

#include "rendering1.h"
#include "game_config1.h"
#include "utils1.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

void renderIntro(GameState state, int introFrameIndex, SDL_Renderer* renderer, SDL_Texture* textures[], const std::vector<SDL_Texture*>& introFrames) {
    if (introFrameIndex < introFrames.size() && introFrames[introFrameIndex]) {
        if (SDL_GetTicks() - introStartTime >= FRAME_DURATION) {
            SDL_RenderCopy(renderer, introFrames[introFrameIndex], nullptr, nullptr);
        }
    }
}

void renderStory(GameState state, int storyIndex, const std::vector<std::string>& storySlides, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[35], nullptr, &bgRect);
    
    SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &scrollRect);
    
    if (storyIndex < storySlides.size()) {
        SDL_Texture* slideText = renderText(renderer, font, storySlides[storyIndex], scrollRect.w - 120);
        SDL_Rect textRect = {scrollRect.x + 60, scrollRect.y + 60, 0, 0};
        SDL_QueryTexture(slideText, nullptr, nullptr, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, slideText, nullptr, &textRect);
        SDL_DestroyTexture(slideText);
    }
}

void renderMenu(GameState state, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, SDL_Texture* textures[]) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
}

void renderLevelSelect(GameState state, bool level1Completed, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[8], nullptr, &bgRect);
    
    SDL_Rect levelRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &levelRect);
    
    SDL_Texture* levelText = renderText(renderer, font, "Select Level");
    SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25, 0, 0};
    SDL_QueryTexture(levelText, nullptr, nullptr, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, levelText, nullptr, &textRect);
    SDL_DestroyTexture(levelText);
}

void renderScoreboard(GameState state, int highScore, const std::vector<SDL_Rect>& candles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]) {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, textures[12], nullptr, &bgRect);
    
    SDL_Rect scrollRect = {SCREEN_WIDTH / 2 - scrollWidth / 2, SCREEN_HEIGHT / 2 - scrollHeight / 2, scrollWidth, scrollHeight};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &scrollRect);
    
    SDL_Texture* scoreText = renderText(renderer, font, "High Score: " + std::to_string(highScore));
    SDL_Rect textRect = {scrollRect.x + 60, scrollRect.y + 60, 0, 0};
    SDL_QueryTexture(scoreText, nullptr, nullptr, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, scoreText, nullptr, &textRect);
    SDL_DestroyTexture(scoreText);
}

void renderCountdown(GameState state, int countDownValue, int currentLevel, float cameraX, const SDL_Rect& player, int currentFrame, int boosterFrame, float collectibleFloatOffset, const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles, const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, int frameWidth, int frameHeight) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    
    SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[39], nullptr, &groundRect);
    
    for (const auto& col : collectibles) {
        SDL_Rect floatRect = {col.x, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)), col.w, col.h};
        SDL_RenderCopy(renderer, textures[0], nullptr, &floatRect);
    }
    
    SDL_Rect drawPlayer = player;
    SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    
    SDL_Texture* countDownText = renderText(renderer, font, std::to_string(countDownValue));
    SDL_Rect textRect = {SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 0, 0};
    SDL_QueryTexture(countDownText, nullptr, nullptr, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, countDownText, nullptr, &textRect);
    SDL_DestroyTexture(countDownText);
}

void renderPaused(GameState state, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[]) {
    SDL_RenderCopy(renderer, textures[32], nullptr, nullptr);
    
    SDL_Texture* pauseText = renderText(renderer, font, "Paused - Press P to Resume");
    SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 25, 500, 50};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_RenderFillRect(renderer, &pauseRect);
    SDL_RenderCopy(renderer, pauseText, nullptr, &pauseRect);
    SDL_DestroyTexture(pauseText);
}

void renderPlaying(GameState state, int currentLevel, float cameraX, const SDL_Rect& player, float velocityY, bool isJumping, bool onPlatform, bool obstaclesStopped, bool dragonStopped, bool dragonMovingDiagonally, bool isDragonFalling, bool goldenEggVisible, bool wandOffered, bool wandPurchased, bool waitingForWand, bool waitingForGround, bool isShaking, bool playerAnimationStopped, bool hidePlayer, bool wandEffectPlaying, bool waitingForDragonPause, int score, int lives, int collisionCount, float collectibleFloatOffset, int currentFrame, int dragonFrame, int dragonDeadFrame, int boosterFrame, int dragonX, int dragonY, int bottomObstaclesPassed, int wandUsesRemaining, Uint32 wandEffectStartTime, int wandEffectFrame, const std::vector<SDL_Rect>& bottomObstacles, const std::vector<SDL_Rect>& collectibles, const std::vector<SDL_Rect>& boosters, const std::vector<SDL_Rect>& topObstacles, const SDL_Rect& goldenEggRect, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* wandEffectSheet, const std::vector<SDL_Texture*>& dragonFrames, SDL_Texture* dragonDead[], int frameWidth, int frameHeight, int wandEffectFrameWidth, int wandEffectFrameHeight, int shakeOffsetX, int shakeOffsetY) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    
    SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[39], nullptr, &groundRect);
    
    for (const auto& col : collectibles) {
        SDL_Rect floatRect = {col.x + shakeOffsetX, col.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)) + shakeOffsetY, col.w, col.h};
        SDL_RenderCopy(renderer, textures[0], nullptr, &floatRect);
    }
    
    if (goldenEggVisible) {
        SDL_Rect eggRect = {goldenEggRect.x + shakeOffsetX, goldenEggRect.y + static_cast<int>(FLOAT_AMPLITUDE * sin(collectibleFloatOffset)) + shakeOffsetY, goldenEggRect.w, goldenEggRect.h};
        SDL_RenderCopy(renderer, textures[0], nullptr, &eggRect);
    }
    
    if (!hidePlayer) {
        SDL_Rect drawPlayer = player;
        drawPlayer.x += shakeOffsetX;
        drawPlayer.y += shakeOffsetY;
        SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    }
    
    SDL_Rect dragonDst = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
    if (isDragonFalling && dragonDeadFrame >= 0) {
        SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
    } else if (!isDragonFalling) {
        SDL_RenderCopy(renderer, dragonFrames[dragonFrame], nullptr, &dragonDst);
    }
    
    if (wandOffered && !wandPurchased) {
        SDL_Rect popupRect = {SCREEN_WIDTH / 2 - popupWidth / 2, SCREEN_HEIGHT / 2 - popupHeight / 2, popupWidth, popupHeight};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
        SDL_RenderFillRect(renderer, &popupRect);
        SDL_Texture* wandText = renderText(renderer, font, "Purchase Wand for 10 points?");
        SDL_Rect textRect = {popupRect.x + 10, popupRect.y + 10, 0, 0};
        SDL_QueryTexture(wandText, nullptr, nullptr, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, wandText, nullptr, &textRect);
        SDL_DestroyTexture(wandText);
    }
    
    SDL_Texture* scoreText = renderText(renderer, font, std::to_string(score));
    SDL_Rect scoreRect = {10, 10, 0, 0};
    SDL_QueryTexture(scoreText, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
    SDL_RenderCopy(renderer, scoreText, nullptr, &scoreRect);
    SDL_DestroyTexture(scoreText);
}

void renderGameOver(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame, int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* dragonDead[], int frameWidth, int frameHeight) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    
    SDL_Rect groundRect = {0, GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[39], nullptr, &groundRect);
    
    SDL_Rect drawPlayer = player;
    SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    
    SDL_Rect dragonDst = {dragonX, dragonY, DRAGON_WIDTH, DRAGON_HEIGHT};
    SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
}

void renderCongrats(GameState state, float cameraX, const SDL_Rect& player, int currentFrame, int dragonDeadFrame, int dragonX, int dragonY, int shakeOffsetX, int shakeOffsetY, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* textures[], SDL_Texture* spriteSheet, SDL_Texture* dragonDead[], int frameWidth, int frameHeight) {
    int bgIndex = static_cast<int>(cameraX / SCREEN_WIDTH) % 3;
    SDL_RenderCopy(renderer, textures[32 + bgIndex], nullptr, nullptr);
    
    SDL_Rect groundRect = {shakeOffsetX, GROUND_Y + shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_Y};
    SDL_RenderCopy(renderer, textures[39], nullptr, &groundRect);
    
    SDL_Rect drawPlayer = player;
    drawPlayer.x += shakeOffsetX;
    drawPlayer.y += shakeOffsetY;
    SDL_Rect srcRect = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &drawPlayer);
    
    SDL_Rect dragonDst = {dragonX + shakeOffsetX, dragonY + shakeOffsetY, DRAGON_WIDTH, DRAGON_HEIGHT};
    SDL_RenderCopy(renderer, dragonDead[dragonDeadFrame], nullptr, &dragonDst);
    
    SDL_Rect congratsRect = {SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 25, 500, 50};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_RenderFillRect(renderer, &congratsRect);
    
    SDL_Texture* congratsText = renderText(renderer, font, "Congratulations! Level Completed!");
    SDL_RenderCopy(renderer, congratsText, nullptr, &congratsRect);
    SDL_DestroyTexture(congratsText);
}

