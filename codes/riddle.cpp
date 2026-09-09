/*#include "riddle.h"
#include "game_config.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include <cmath>

struct Button {
    SDL_Rect rect;
    std::string text;
    bool isCorrect;
};

bool runRiddleGame() {
    bool quit = false;
    bool intro = true;
    bool answered = false;
    bool eggClicked = false;
    bool eggStable = false;
    bool showOpenEgg = false;
    SDL_Event e;
    int eggX = (SCREEN_WIDTH - 200) / 2;
    float eggY = (SCREEN_HEIGHT - 200) / 2;
    float openEggY = SCREEN_HEIGHT - 300;
    Uint32 lastTime = SDL_GetTicks();
    const float AMPLITUDE = 100.0f;
    const float OPEN_EGG_AMPLITUDE = 20.0f;
    const float FREQUENCY = 1.0f;
    float time = 0.0f;
    Uint32 openEggStartTime = 0;
    const Uint32 OPEN_EGG_DURATION = 1000;

    SDL_Color white = {255, 255, 255, 255};
    std::string introText = " You've flown past flame and dodged the beast, The egg you hold a prize at least. But one mistake could now mislead.";
    SDL_Texture* introTexture = renderText(introText, white, 800);
    int itw, ith;
    SDL_QueryTexture(introTexture, NULL, NULL, &itw, &ith);
    SDL_Rect introRect = {(SCREEN_WIDTH - itw) / 2, (SCREEN_HEIGHT - ith) / 2 - 200, itw, ith};

    std::string riddleText =
        "In shadows deep, one creature flies,\n"
        "It drops you aid from cloudy skies.\n"
        "Another moans through haunted air,\n"
        "And pulls your health with icy stare.\n"
        "One blocks your path with breath incendiary,\n"
        "Another falls when you leap airy.\n\n"
        "To reach the Cup, remember true—\n"
        "Which one helps when trouble’s due?";

    SDL_Texture* riddleTexture = renderText(riddleText, white, 900);
    int rtw, rth;
    SDL_QueryTexture(riddleTexture, NULL, NULL, &rtw, &rth);
    SDL_Rect riddleRect = {(SCREEN_WIDTH - rtw) / 2, 100, rtw, rth};

    std::vector<Button> buttons = {
        {{(SCREEN_WIDTH / 2) - 350, (SCREEN_HEIGHT / 2) + 50, 300, 50}, "The Ghost", false},
        {{(SCREEN_WIDTH / 2) - 350, (SCREEN_HEIGHT / 2) + 110, 300, 50}, "The Owl", true},
        {{(SCREEN_WIDTH / 2) + 50, (SCREEN_HEIGHT / 2) + 50, 300, 50}, "The Obstacle", false},
        {{(SCREEN_WIDTH / 2) + 50, (SCREEN_HEIGHT / 2) + 110, 300, 50}, "The Dragon", false}
    };

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = current Reserves0:0:0currentTime;

        if (intro) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, blurredBgTexture, NULL, NULL);

            if (!eggClicked) {
                time += deltaTime;
                eggY = (SCREEN_HEIGHT - 200) / 2 + AMPLITUDE * std::sin(FREQUENCY * time);
            } else if (!eggStable) {
                if (eggY < SCREEN_HEIGHT - 300) {
                    eggY += 2.0f * deltaTime * 60.0f;
                    if (eggY > SCREEN_HEIGHT - 300) {
                        eggY = SCREEN_HEIGHT - 300;
                        eggStable = true;
                        showOpenEgg = true;
                        openEggStartTime = currentTime;
                    }
                }
            } else if (showOpenEgg) {
                time += deltaTime;
                openEggY = (SCREEN_HEIGHT - 300) + OPEN_EGG_AMPLITUDE * std::sin(FREQUENCY * time);
                if (currentTime - openEggStartTime >= OPEN_EGG_DURATION) {
                    showOpenEgg = false;
                    intro = false;
                }
            }

            SDL_Rect eggRect = {static_cast<int>(eggX), static_cast<int>(showOpenEgg ? openEggY : eggY), 200, 200};
            SDL_RenderCopy(renderer, showOpenEgg ? openEggTexture : eggTexture, NULL, &eggRect);

            if (!eggClicked || !eggStable) {
                SDL_RenderCopy(renderer, introTexture, NULL, &introRect);
            }

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return false;
                else if (e.type == SDL_MOUSEBUTTONDOWN && !eggClicked) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    if (isMouseInside(eggRect, mx, my)) {
                        eggClicked = true;
                    }
                }
            }
            SDL_RenderPresent(renderer);
        } else {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

            time += deltaTime;
            openEggY = (SCREEN_HEIGHT - 300) + OPEN_EGG_AMPLITUDE * std::sin(FREQUENCY * time);
            SDL_Rect eggRect = {static_cast<int>(eggX), static_cast<int>(openEggY), 200, 200};
            SDL_RenderCopy(renderer, openEggTexture, NULL, &eggRect);

            SDL_RenderCopy(renderer, riddleTexture, NULL, &riddleRect);

            for (auto& btn : buttons) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                SDL_RenderFillRect(renderer, &btn.rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &btn.rect);

                SDL_Texture* txt = renderText(btn.text, white, 260);
                int tx, ty;
                SDL_QueryTexture(txt, NULL, NULL, &tx, &ty);
                SDL_Rect trect = {btn.rect.x + 20, btn.rect.y + 10, tx, ty};
                SDL_RenderCopy(renderer, txt, NULL, &trect);
                SDL_DestroyTexture(txt);
            }

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return false;
                else if (e.type == SDL_MOUSEBUTTONDOWN && !answered) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    for (auto& btn : buttons) {
                        if (isMouseInside(btn.rect, mx, my)) {
                            answered = true;
                            if (btn.isCorrect) {
                                Mix_PlayChannel(-1, correctSound, 0);
                                SDL_Delay(1200);
                                return true;
                            } else {
                                Mix_PlayChannel(-1, wrongSound, 0);
                                SDL_Delay(1200);
                                return false;
                            }
                        }
                    }
                }
            }
            SDL_RenderPresent(renderer);
        }
    }
    SDL_DestroyTexture(introTexture);
    SDL_DestroyTexture(riddleTexture);
    return false;
}

*/
/*

#include "riddle.h"
#include "game_config.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include <cmath>

struct Button {
    SDL_Rect rect;
    std::string text;
    bool isCorrect;
};

bool runRiddleGame() {
    bool quit = false;
    bool intro = true;
    bool answered = false;
    bool eggClicked = false;
    bool eggStable = false;
    bool showOpenEgg = false;
    SDL_Event e;
    int eggX = (SCREEN_WIDTH - 200) / 2;
    float eggY = (SCREEN_HEIGHT - 200) / 2;
    float openEggY = SCREEN_HEIGHT - 300;
    Uint32 lastTime = SDL_GetTicks();
    const float AMPLITUDE = 100.0f;
    const float OPEN_EGG_AMPLITUDE = 20.0f;
    const float FREQUENCY = 1.0f;
    float time = 0.0f;
    Uint32 openEggStartTime = 0;
    const Uint32 OPEN_EGG_DURATION = 1000;

    SDL_Color white = {255, 255, 255, 255};
    std::string introText = " You've flown past flame and dodged the beast, The egg you hold a prize at least. But one mistake could now mislead.";
    SDL_Texture* introTexture = renderText(introText, white, 800);
    int itw, ith;
    SDL_QueryTexture(introTexture, NULL, NULL, &itw, &ith);
    SDL_Rect introRect = {(SCREEN_WIDTH - itw) / 2, (SCREEN_HEIGHT - ith) / 2 - 200, itw, ith};

    std::string riddleText =
        "In shadows deep, one creature flies,\n"
        "It drops you aid from cloudy skies.\n"
        "Another moans through haunted air,\n"
        "And pulls your health with icy stare.\n"
        "One blocks your path with breath incendiary,\n"
        "Another falls when you leap airy.\n\n"
        "To reach the Cup, remember true—\n"
        "Which one helps when trouble’s due?";

    SDL_Texture* riddleTexture = renderText(riddleText, white, 900);
    int rtw, rth;
    SDL_QueryTexture(riddleTexture, NULL, NULL, &rtw, &rth);
    SDL_Rect riddleRect = {(SCREEN_WIDTH - rtw) / 2, 100, rtw, rth};

    std::vector<Button> buttons = {
        {{(SCREEN_WIDTH / 2) - 350, (SCREEN_HEIGHT / 2) + 50, 300, 50}, "The Ghost", false},
        {{(SCREEN_WIDTH / 2) - 350, (SCREEN_HEIGHT / 2) + 110, 300, 50}, "The Owl", true},
        {{(SCREEN_WIDTH / 2) + 50, (SCREEN_HEIGHT / 2) + 50, 300, 50}, "The Obstacle", false},
        {{(SCREEN_WIDTH / 2) + 50, (SCREEN_HEIGHT / 2) + 110, 300, 50}, "The Dragon", false}
    };

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        if (intro) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, blurredBgTexture, NULL, NULL);

            if (!eggClicked) {
                time += deltaTime;
                eggY = (SCREEN_HEIGHT - 200) / 2 + AMPLITUDE * std::sin(FREQUENCY * time);
            } else if (!eggStable) {
                if (eggY < SCREEN_HEIGHT - 300) {
                    eggY += 2.0f * deltaTime * 60.0f;
                    if (eggY > SCREEN_HEIGHT - 300) {
                        eggY = SCREEN_HEIGHT - 300;
                        eggStable = true;
                        showOpenEgg = true;
                        openEggStartTime = currentTime;
                    }
                }
            } else if (showOpenEgg) {
                time += deltaTime;
                openEggY = (SCREEN_HEIGHT - 300) + OPEN_EGG_AMPLITUDE * std::sin(FREQUENCY * time);
                if (currentTime - openEggStartTime >= OPEN_EGG_DURATION) {
                    showOpenEgg = false;
                    intro = false;
                }
            }

            SDL_Rect eggRect = {static_cast<int>(eggX), static_cast<int>(showOpenEgg ? openEggY : eggY), 200, 200};
            SDL_RenderCopy(renderer, showOpenEgg ? openEggTexture : eggTexture, NULL, &eggRect);

            if (!eggClicked || !eggStable) {
                SDL_RenderCopy(renderer, introTexture, NULL, &introRect);
            }

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return false;
                else if (e.type == SDL_MOUSEBUTTONDOWN && !eggClicked) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    if (isMouseInside(eggRect, mx, my)) {
                        eggClicked = true;
                    }
                }
            }
            SDL_RenderPresent(renderer);
        } else {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

            time += deltaTime;
            openEggY = (SCREEN_HEIGHT - 300) + OPEN_EGG_AMPLITUDE * std::sin(FREQUENCY * time);
            SDL_Rect eggRect = {static_cast<int>(eggX), static_cast<int>(openEggY), 200, 200};
            SDL_RenderCopy(renderer, openEggTexture, NULL, &eggRect);

            SDL_RenderCopy(renderer, riddleTexture, NULL, &riddleRect);

            for (auto& btn : buttons) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                SDL_RenderFillRect(renderer, &btn.rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &btn.rect);

                SDL_Texture* txt = renderText(btn.text, white, 260);
                int tx, ty;
                SDL_QueryTexture(txt, NULL, NULL, &tx, &ty);
                SDL_Rect trect = {btn.rect.x + 20, btn.rect.y + 10, tx, ty};
                SDL_RenderCopy(renderer, txt, NULL, &trect);
                SDL_DestroyTexture(txt);
            }

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return false;
                else if (e.type == SDL_MOUSEBUTTONDOWN && !answered) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    for (auto& btn : buttons) {
                        if (isMouseInside(btn.rect, mx, my)) {
                            answered = true;
                            if (btn.isCorrect) {
                                Mix_PlayChannel(-1, correctSound, 0);
                                SDL_Delay(1200);
                                return true;
                            } else {
                                Mix_PlayChannel(-1, wrongSound, 0);
                                SDL_Delay(1200);
                                return false;
                            }
                        }
                    }
                }
            }
            SDL_RenderPresent(renderer);
        }
    }
    SDL_DestroyTexture(introTexture);
    SDL_DestroyTexture(riddleTexture);
    return false;
}
*/

#include "riddle.h"
#include "game_config.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include <cmath>

struct Button {
    SDL_Rect rect;
    std::string text;
    bool isCorrect;
};

bool runRiddleGame() {
    bool quit = false;
    bool intro = true;
    bool answered = false;
    bool eggClicked = false;
    bool eggStable = false;
    bool showOpenEgg = false;
    SDL_Event e;
    int eggX = (SCREEN_WIDTH - 200) / 2;
    float eggY = (SCREEN_HEIGHT - 200) / 2;
    float openEggY = SCREEN_HEIGHT - 300;
    Uint32 lastTime = SDL_GetTicks();
    const float AMPLITUDE = 100.0f;
    const float OPEN_EGG_AMPLITUDE = 20.0f;
    const float FREQUENCY = 1.0f;
    float time = 0.0f;
    Uint32 openEggStartTime = 0;
    const Uint32 OPEN_EGG_DURATION = 1000;

    SDL_Color white = {255, 255, 255, 255};
    std::string introText = " You've flown past flame and dodged the beast, The egg you hold a prize at least. But one mistake could now mislead.";
    SDL_Texture* introTexture = renderText(introText, white, 800);
    int itw, ith;
    SDL_QueryTexture(introTexture, NULL, NULL, &itw, &ith);
    SDL_Rect introRect = {(SCREEN_WIDTH - itw) / 2, (SCREEN_HEIGHT - ith) / 2 - 200, itw, ith};

    std::string riddleText =
        "In shadows deep, one creature flies,\n"
        "It drops you aid from cloudy skies.\n"
        "Another moans through haunted air,\n"
        "And pulls your health with icy stare.\n"
        "One blocks your path with breath incendiary,\n"
        "Another falls when you leap airy.\n\n"
        "To reach the Cup, remember true—\n"
        "Which one helps when trouble’s due?";

    SDL_Texture* riddleTexture = renderText(riddleText, white, 900);
    int rtw, rth;
    SDL_QueryTexture(riddleTexture, NULL, NULL, &rtw, &rth);
    SDL_Rect riddleRect = {(SCREEN_WIDTH - rtw) / 2, 100, rtw, rth};

    std::vector<Button> buttons = {
        {{(SCREEN_WIDTH / 2) - 350, (SCREEN_HEIGHT / 2) + 50, 300, 50}, "The Ghost", false},
        {{(SCREEN_WIDTH / 2) - 350, (SCREEN_HEIGHT / 2) + 110, 300, 50}, "The Owl", true},
        {{(SCREEN_WIDTH / 2) + 50, (SCREEN_HEIGHT / 2) + 50, 300, 50}, "The Obstacle", false},
        {{(SCREEN_WIDTH / 2) + 50, (SCREEN_HEIGHT / 2) + 110, 300, 50}, "The Dragon", false}
    };

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        if (intro) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, blurredBgTexture, NULL, NULL);

            if (!eggClicked) {
                time += deltaTime;
                eggY = (SCREEN_HEIGHT - 200) / 2 + AMPLITUDE * std::sin(FREQUENCY * time);
            } else if (!eggStable) {
                if (eggY < SCREEN_HEIGHT - 300) {
                    eggY += 2.0f * deltaTime * 60.0f;
                    if (eggY > SCREEN_HEIGHT - 300) {
                        eggY = SCREEN_HEIGHT - 300;
                        eggStable = true;
                        showOpenEgg = true;
                        openEggStartTime = currentTime;
                    }
                }
            } else if (showOpenEgg) {
                time += deltaTime;
                openEggY = (SCREEN_HEIGHT - 300) + OPEN_EGG_AMPLITUDE * std::sin(FREQUENCY * time);
                if (currentTime - openEggStartTime >= OPEN_EGG_DURATION) {
                    showOpenEgg = false;
                    intro = false;
                }
            }

            SDL_Rect eggRect = {static_cast<int>(eggX), static_cast<int>(showOpenEgg ? openEggY : eggY), 200, 200};
            SDL_RenderCopy(renderer, showOpenEgg ? openEggTexture : eggTexture, NULL, &eggRect);

            if (!eggClicked || !eggStable) {
                SDL_RenderCopy(renderer, introTexture, NULL, &introRect);
            }

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return false;
                else if (e.type == SDL_MOUSEBUTTONDOWN && !eggClicked) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    if (isMouseInside(eggRect, mx, my)) {
                        eggClicked = true;
                    }
                }
            }
            SDL_RenderPresent(renderer);
        } else {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

            time += deltaTime;
            openEggY = (SCREEN_HEIGHT - 300) + OPEN_EGG_AMPLITUDE * std::sin(FREQUENCY * time);
            SDL_Rect eggRect = {static_cast<int>(eggX), static_cast<int>(openEggY), 200, 200};
            SDL_RenderCopy(renderer, openEggTexture, NULL, &eggRect);

            SDL_RenderCopy(renderer, riddleTexture, NULL, &riddleRect);

            for (auto& btn : buttons) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                SDL_RenderFillRect(renderer, &btn.rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &btn.rect);

                SDL_Texture* txt = renderText(btn.text, white, 260);
                int tx, ty;
                SDL_QueryTexture(txt, NULL, NULL, &tx, &ty);
                SDL_Rect trect = {btn.rect.x + 20, btn.rect.y + 10, tx, ty};
                SDL_RenderCopy(renderer, txt, NULL, &trect);
                SDL_DestroyTexture(txt);
            }

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return false;
                else if (e.type == SDL_MOUSEBUTTONDOWN && !answered) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    for (auto& btn : buttons) {
                        if (isMouseInside(btn.rect, mx, my)) {
                            answered = true;
                            if (btn.isCorrect) {
                                Mix_PlayChannel(-1, correctSound, 0);
                                SDL_Delay(1200);
                                return true;
                            } else {
                                Mix_PlayChannel(-1, wrongSound, 0);
                                SDL_Delay(1200);
                                return false;
                            }
                        }
                    }
                }
            }
            SDL_RenderPresent(renderer);
        }
    }
    SDL_DestroyTexture(introTexture);
    SDL_DestroyTexture(riddleTexture);
    return false;
}
