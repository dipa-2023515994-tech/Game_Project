#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "game_state.h"
#include "resource_manager.h"
#include <SDL2/SDL.h>

void handleInput(GameState& state, bool& running, GameResources& resources, SDL_Renderer* renderer);

#endif
