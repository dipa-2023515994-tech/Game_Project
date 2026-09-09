#include "input_handler.h"
#include "game_logic.h"

void handleInput(GameState& state, bool& running, GameResources& resources, SDL_Renderer* renderer) {
    static bool keyPressed = false;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN && !keyPressed) {
            keyPressed = true;
            if (e.key.keysym.sym == SDLK_RETURN && (state.state == MENU || state.state == GAME_OVER || state.state == SCOREBOARD || state.state == CONGRATS)) {
                state.state = LEVEL_SELECT;
            } else if (e.key.keysym.sym == SDLK_p && state.state == PLAYING && !state.wandOffered) {
                state.state = PAUSED;
            } else if (e.key.keysym.sym == SDLK_p && state.state == PAUSED) {
                state.state = PLAYING;
                if (!state.obstaclesStopped) Mix_PlayMusic(resources.gameMusic, -1);
            } else if (e.key.keysym.sym == SDLK_w && state.state == PLAYING && state.wandPurchased && !state.wandOffered) {
                useWand(state, resources);
            } else if (e.key.keysym.sym == SDLK_SPACE && state.state == PLAYING && !state.wandOffered) {
                Uint32 currentTime = SDL_GetTicks();
                if (!state.isJumping || (state.spacePressCount == 1 && currentTime - state.lastSpacePressTime <= 300)) {
                    state.velocityY = (state.spacePressCount == 1) ? DOUBLE_JUMP_VELOCITY : JUMP_VELOCITY;
                    state.isJumping = true;
                    state.onPlatform = false;
                    state.spacePressCount = (state.spacePressCount == 1) ? 0 : 1;
                    state.lastSpacePressTime = currentTime;
                }
            } else if (state.state == STORY) {
                if (e.key.keysym.sym == SDLK_RIGHT) {
                    state.storyIndex = std::min(state.storyIndex + 1, (int)state.storySlides.size() - 1);
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    state.storyIndex = std::max(state.storyIndex - 1, 0);
                } else if (e.key.keysym.sym == SDLK_s) {
                    Mix_HaltMusic();
                    Mix_PlayMusic(resources.introMusic, -1);
                    state.state = MENU;
                    state.storyIndex = 0;
                }
            }
        } else if (e.type == SDL_KEYUP) {
            keyPressed = false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            SDL_Point mouse = {mx, my};
            handleMouseClick(state, mouse, resources, renderer);
        }
    }
}
