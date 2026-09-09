#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "game.h"

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
int loadHighScore(const std::string& filename);
void saveHighScore(const std::string& filename, int score);
bool loadLevelCompletion(const std::string& filename);
void saveLevelCompletion(const std::string& filename, bool completed);
void updateGameState(GameData& data, GameResources& resources);

#endif
