#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

struct GameResources {
    SDL_Texture* spriteSheet;
    std::vector<SDL_Texture*> dragonFrames;
    SDL_Texture* dragonDead[3];
    SDL_Texture* textures[40];
    std::vector<SDL_Texture*> introFrames;
    Mix_Chunk* collectibleSound;
    Mix_Chunk* boosterSound;
    Mix_Chunk* buttonClickSound;
    Mix_Chunk* dragonRoarSound;
    Mix_Chunk* dragonDeadSound;
    Mix_Music* introMusic;
    Mix_Music* storyMusic;
    Mix_Music* gameMusic;
    TTF_Font* font;
    int frameWidth, frameHeight;
};

bool loadResources(SDL_Renderer* renderer, GameResources& resources);
void cleanupResources(GameResources& resources);

#endif
