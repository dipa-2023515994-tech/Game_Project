/*#include "resource_manager.h"
#include "utils.h"
#include <iostream>

bool loadResources(SDL_Renderer* renderer, GameResources& resources) {
    resources.font = TTF_OpenFont("assets/font.ttf", 36);
    if (!resources.font) {
        std::cerr << "Failed to load font" << std::endl;
        return false;
    }

    resources.collectibleSound = loadSound("assets/collectible.wav");
    resources.boosterSound = loadSound("assets/booster.wav");
    resources.buttonClickSound = loadSound("assets/button_click.wav");
    resources.dragonRoarSound = loadSound("assets/dragon_roar.wav");
    resources.dragonDeadSound = loadSound("assets/dragon_dead.wav");
    resources.introMusic = loadMusic("assets/intro_music.ogg");
    resources.storyMusic = loadMusic("assets/story_music.ogg");
    resources.gameMusic = loadMusic("assets/game_music.ogg");
    if (!resources.collectibleSound || !resources.boosterSound || !resources.buttonClickSound || 
        !resources.dragonRoarSound || !resources.dragonDeadSound || !resources.introMusic || 
        !resources.storyMusic || !resources.gameMusic) {
        std::cerr << "Failed to load sound effects or music" << std::endl;
        return false;
    }

    SDL_Surface* tempSurface = IMG_Load("assets/sprite.png");
    if (!tempSurface) {
        std::cerr << "Failed to load sprite sheet: " << IMG_GetError() << std::endl;
        return false;
    }
    resources.spriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    resources.frameWidth = tempSurface->w / FRAME_COUNT;
    resources.frameHeight = tempSurface->h;
    SDL_FreeSurface(tempSurface);

    resources.dragonFrames.resize(FRAME_COUNT);
    for (int i = 0; i < FRAME_COUNT; ++i) {
        resources.dragonFrames[i] = loadTexture(renderer, "assets/drag" + std::to_string(i + 1) + ".png");
        if (!resources.dragonFrames[i]) return false;
    }

    resources.dragonDead[0] = loadTexture(renderer, "assets/dragon_dead1.png");
    resources.dragonDead[1] = loadTexture(renderer, "assets/dragon_dead2.png");
    resources.dragonDead[2] = loadTexture(renderer, "assets/dragon_dead3.png");
    if (!resources.dragonDead[0] || !resources.dragonDead[1] || !resources.dragonDead[2]) return false;

    const char* texturePaths[] = {
        "assets/golden_egg.png", "assets/ground.png", "assets/obstacle_bottom.png", 
        "assets/collectible.png", "assets/wand_popup.png", "assets/booster1.png",
        "assets/booster2.png", "assets/booster3.png", "assets/level_bg.png",
        "assets/level1_button.png", "assets/level2_button.png", "assets/level2_locked.png",
        "assets/menu_bg.png", "assets/start_button.png", "assets/scoreboard_button.png",
        "assets/instructions_button.png", "assets/quit_button.png", "assets/back_button.png",
        "assets/next_button.png", "assets/prev_button.png", "assets/wand.png",
        "assets/candle.png", "assets/title_glow.png", "assets/dumbledore.png",
        "assets/buy_button.png", "assets/later_button.png", "assets/score_icon.png",
        "assets/life_icon.png", "assets/owl.png", "assets/wandIcon.png",
        "assets/platform.png", "assets/scroll_box.png", "assets/bg1.png",
        "assets/bg2.png", "assets/bg3.png", "assets/new_story_bg.png",
        "assets/exit_button.png", "assets/game_over_window.png", 
        "assets/play_again_button.png", "assets/lightning.png"
    };
    for (int i = 0; i < 40; ++i) {
        resources.textures[i] = loadTexture(renderer, texturePaths[i]);
        if (!resources.textures[i]) {
            std::cerr << "Failed to load texture: " << texturePaths[i] << std::endl;
            return false;
        }
    }

    resources.introFrames.resize(26);
    for (int i = 0; i < 26; ++i) {
        resources.introFrames[i] = loadTexture(renderer, "assets/intro" + std::to_string(i + 1) + ".png");
        if (!resources.introFrames[i]) return false;
    }

    return true;
}

void cleanupResources(GameResources& resources) {
    for (auto tex : resources.dragonFrames) SDL_DestroyTexture(tex);
    for (auto tex : resources.introFrames) SDL_DestroyTexture(tex);
    for (int i = 0; i < 40; ++i) SDL_DestroyTexture(resources.textures[i]);
    for (int i = 0; i < 3; ++i) SDL_DestroyTexture(resources.dragonDead[i]);
    SDL_DestroyTexture(resources.spriteSheet);
    Mix_FreeChunk(resources.collectibleSound);
    Mix_FreeChunk(resources.boosterSound);
    Mix_FreeChunk(resources.buttonClickSound);
    Mix_FreeChunk(resources.dragonRoarSound);
    Mix_FreeChunk(resources.dragonDeadSound);
    Mix_FreeMusic(resources.introMusic);
    Mix_FreeMusic(resources.storyMusic);
    Mix_FreeMusic(resources.gameMusic);
    TTF_CloseFont(resources.font);
}
*/

#include "resources.h"
#include <iostream>

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        std::cerr << "Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
    } else {
        std::cout << "Successfully loaded texture: " << path << std::endl;
    }
    return texture;
}

Mix_Chunk* loadSound(const std::string& path) {
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (!sound) {
        std::cerr << "Failed to load sound: " << path << " - " << Mix_GetError() << std::endl;
    } else {
        std::cout << "Successfully loaded sound: " << path << std::endl;
    }
    return sound;
}

Mix_Music* loadMusic(const std::string& path) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << path << " - " << Mix_GetError() << std::endl;
    } else {
        std::cout << "Successfully loaded music: " << path << std::endl;
    }
    return music;
}

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int wrapWidth) {
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

bool loadGameResources(SDL_Renderer* renderer, GameResources& resources) {
    resources.font = TTF_OpenFont("assets/font.ttf", 36);
    if (!resources.font) {
        std::cerr << "Failed to load font" << std::endl;
        return false;
    }

    resources.collectibleSound = loadSound("assets/collectible.wav");
    resources.boosterSound = loadSound("assets/booster.wav");
    resources.buttonClickSound = loadSound("assets/button_click.wav");
    resources.dragonRoarSound = loadSound("assets/dragon_roar.wav");
    resources.dragonDeadSound = loadSound("assets/dragon_dead.wav");
    resources.introMusic = loadMusic("assets/intro_music.ogg");
    resources.storyMusic = loadMusic("assets/story_music.ogg");
    resources.gameMusic = loadMusic("assets/game_music.ogg");
    if (!resources.collectibleSound || !resources.boosterSound || !resources.buttonClickSound ||
        !resources.dragonRoarSound || !resources.dragonDeadSound || !resources.introMusic ||
        !resources.storyMusic || !resources.gameMusic) {
        return false;
    }

    SDL_Surface* tempSurface = IMG_Load("assets/sprite.png");
    if (!tempSurface) {
        std::cerr << "Failed to load sprite sheet: " << IMG_GetError() << std::endl;
        return false;
    }
    resources.spriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    resources.frameWidth = tempSurface->w / FRAME_COUNT;
    resources.frameHeight = tempSurface->h;
    SDL_FreeSurface(tempSurface);
    if (!resources.spriteSheet) return false;

    resources.dragonFrames.resize(FRAME_COUNT);
    for (int i = 0; i < FRAME_COUNT; ++i) {
        resources.dragonFrames[i] = loadTexture(renderer, "assets/drag" + std::to_string(i + 1) + ".png");
        if (!resources.dragonFrames[i]) return false;
    }

    resources.dragonDead[0] = loadTexture(renderer, "assets/dragon_dead1.png");
    resources.dragonDead[1] = loadTexture(renderer, "assets/dragon_dead2.png");
    resources.dragonDead[2] = loadTexture(renderer, "assets/dragon_dead3.png");
    if (!resources.dragonDead[0] || !resources.dragonDead[1] || !resources.dragonDead[2]) return false;

    std::vector<std::string> texturePaths = {
        "assets/golden_egg.png", "assets/ground.png", "assets/obstacle_bottom.png", "assets/collectible.png",
        "assets/wand_popup.png", "assets/booster1.png", "assets/booster2.png", "assets/booster3.png",
        "assets/level_bg.png", "assets/level1_button.png", "assets/level2_button.png", "assets/level2_locked.png",
        "assets/menu_bg.png", "assets/start_button.png", "assets/scoreboard_button.png", "assets/instructions_button.png",
        "assets/quit_button.png", "assets/back_button.png", "assets/next_button.png", "assets/prev_button.png",
        "assets/wand.png", "assets/candle.png", "assets/title_glow.png", "assets/dumbledore.png",
        "assets/buy_button.png", "assets/later_button.png", "assets/score_icon.png", "assets/life_icon.png",
        "assets/owl.png", "assets/wandIcon.png", "assets/platform.png", "assets/scroll_box.png",
        "assets/bg1.png", "assets/bg2.png", "assets/bg3.png", "assets/new_story_bg.png",
        "assets/exit_button.png", "assets/game_over_window.png", "assets/play_again_button.png", "assets/lightning.png"
    };
    resources.textures.resize(40);
    for (int i = 0; i < 40; ++i) {
        resources.textures[i] = loadTexture(renderer, texturePaths[i]);
        if (!resources.textures[i]) return false;
    }

    resources.introFrames.resize(26);
    for (int i = 0; i < 26; ++i) {
        resources.introFrames[i] = loadTexture(renderer, "assets/intro" + std::to_string(i + 1) + ".png");
        if (!resources.introFrames[i]) return false;
    }

    return true;
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font,
             Mix_Chunk* collectibleSound, Mix_Chunk* boosterSound, Mix_Chunk* buttonClickSound,
             Mix_Chunk* dragonRoarSound, Mix_Chunk* dragonDeadSound, Mix_Music* introMusic,
             Mix_Music* storyMusic, Mix_Music* gameMusic, SDL_Texture* spriteSheet,
             std::vector<SDL_Texture*>& dragonFrames, SDL_Texture* dragonDead[],
             std::vector<SDL_Texture*>& textures, std::vector<SDL_Texture*>& introFrames) {
    for (auto tex : dragonFrames) if (tex) SDL_DestroyTexture(tex);
    for (auto tex : introFrames) if (tex) SDL_DestroyTexture(tex);
    for (auto tex : textures) if (tex) SDL_DestroyTexture(tex);
    for (int i = 0; i < 3; ++i) if (dragonDead && dragonDead[i]) SDL_DestroyTexture(dragonDead[i]);
    if (spriteSheet) SDL_DestroyTexture(spriteSheet);
    if (collectibleSound) Mix_FreeChunk(collectibleSound);
    if (boosterSound) Mix_FreeChunk(boosterSound);
    if (buttonClickSound) Mix_FreeChunk(buttonClickSound);
    if (dragonRoarSound) Mix_FreeChunk(dragonRoarSound);
    if (dragonDeadSound) Mix_FreeChunk(dragonDeadSound);
    if (introMusic) Mix_FreeMusic(introMusic);
    if (storyMusic) Mix_FreeMusic(storyMusic);
    if (gameMusic) Mix_FreeMusic(gameMusic);
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
