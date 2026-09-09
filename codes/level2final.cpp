#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <algorithm>
#include <ctime>

// Forward declaration
void showWinScreen();
void showGameOverScreen();

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 1000;
const int MAP_SIZE = 25;
const int MAZE_SIZE = 900; // 80% of 900 = 720 pixels
const int TILE_SIZE = MAZE_SIZE / MAP_SIZE; // 720 / 25 = 28 pixels
const int MAZE_OFFSET_X = (SCREEN_WIDTH - MAZE_SIZE) / 2; // (1300 - 720) / 2 = 290 pixels
const int MAZE_OFFSET_Y = (SCREEN_HEIGHT - MAZE_SIZE) / 2; // (900 - 720) / 2 = 90 pixels

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

//Riddle Game Textures
SDL_Texture* bgTexture = nullptr;          // Background image for the riddle screen
SDL_Texture* blurredBgTexture = nullptr;   // Blurred version of the background (e.g., for focus effects)
SDL_Texture* eggTexture = nullptr;         // Texture of the golden egg before solving
SDL_Texture* openEggTexture = nullptr;     // Texture of the egg after solving the riddle

Mix_Chunk* correctSound = nullptr;         // Sound to play when the riddle answer is correct
Mix_Chunk* wrongSound = nullptr;           // Sound to play when the answer is wrong
Mix_Chunk* eggClickSound = nullptr;        // Sound when the player clicks the egg
//Maze Game Textures
SDL_Texture* playerTex = nullptr;          // Texture for the player character in the maze
SDL_Texture* opponentTex = nullptr;        // Texture for the enemy character
SDL_Texture* backgroundTex = nullptr;      // Background image for the maze level
SDL_Texture* orbTex = nullptr;             // Collectible orb texture (booster)
SDL_Texture* wandTex = nullptr;            // Texture for the wand item
SDL_Texture* wallTex = nullptr;            // Texture for the maze walls
SDL_Texture* cupTex = nullptr;             // Triwizard Cup texture (end goal)
SDL_Texture* gameOverTex = nullptr;        // Image shown on game over screen

Mix_Chunk* opponentCloseSound = nullptr;   // Sound that plays when enemy is near

// Structure for riddle buttons
struct Button {
    SDL_Rect rect;
    std::string text;
    bool isCorrect;
};

// Structure for A* pathfinding nodes, this is a pathfinding algorithm used for finding the shortest path between two points
struct Node {
    int x, y;         //x=col.index, y=row.index
    int g, h, f;         //f=g+h, total estimated costs
    Node* parent;
    Node(int x, int y, int g, int h) : x(x), y(y), g(g), h(h), f(g + h), parent(nullptr) {}    //Constructor
    bool operator>(const Node& other) const { return f > other.f; } // This overload helps in priority queue(min-heap or max_heap) to always expand the code
};

// Manhattan distance heuristic
int manhattanDistance(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// A* pathfinding to find the next step towards the player
bool findNextStep(int gameMap[], int startX, int startY, int goalX, int goalY, int& nextX, int& nextY) {
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::vector<std::vector<bool>> closedList(MAP_SIZE, std::vector<bool>(MAP_SIZE, false));
    std::vector<std::vector<Node*>> nodes(MAP_SIZE, std::vector<Node*>(MAP_SIZE, nullptr));

    Node* startNode = new Node(startX, startY, 0, manhattanDistance(startX, startY, goalX, goalY));      //indicating initial and final position
    openList.push(*startNode);
    nodes[startX][startY] = startNode;

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, down, left, right- these are the directions where the player can move

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();
        int x = current.x;
        int y = current.y;

        if (x == goalX && y == goalY) {
            Node* node = nodes[x][y];
            while (node && node->parent && node->parent->parent) {
                node = node->parent;
            }
            if (node && node->parent) {
                nextX = node->x;
                nextY = node->y;
                for (int i = 0; i < MAP_SIZE; i++)
                    for (int j = 0; j < MAP_SIZE; j++)
                        delete nodes[i][j];
                return true;
            }
        }

        closedList[x][y] = true;    //the current position as explored

        for (auto& dir : directions) {
            int newX = x + dir[0];          //New position for each posible move
            int newY = y + dir[1];

            if (newX >= 0 && newX < MAP_SIZE && newY >= 0 && newY < MAP_SIZE &&
                !closedList[newX][newY] && gameMap[newX + newY * MAP_SIZE] != 1) {
                int newG = current.g + 1;
                int newH = manhattanDistance(newX, newY, goalX, goalY);
                Node* neighbor = new Node(newX, newY, newG, newH);
                neighbor->parent = nodes[x][y];

                if (!nodes[newX][newY] || newG < nodes[newX][newY]->g) {
                    nodes[newX][newY] = neighbor;
                    openList.push(*neighbor);
                } else {
                    delete neighbor;
                }
            }
        }
    }

    for (int i = 0; i < MAP_SIZE; i++)
        for (int j = 0; j < MAP_SIZE; j++)
            delete nodes[i][j];
    return false; // No path found
}

// Render text for all games
SDL_Texture* renderText(const std::string& message, SDL_Color color, int wrapWidth) {
    SDL_Surface* surf = TTF_RenderUTF8_Blended_Wrapped(font, message.c_str(), color, wrapWidth);
    if (!surf) {
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
    }
    return texture;
}

// Check if mouse is inside a rectangle
bool isMouseInside(SDL_Rect rect, int x, int y) {
    return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
}

// Initialize maze state
void initMaze(int gameMap[], SDL_Rect displayRects[], SDL_Point& player, SDL_Rect& playerRect,
              SDL_Point& opponent, SDL_Rect& opponentRect, SDL_Rect& cupRect, SDL_Rect boosters[], int& numBoosters) {
    int predefinedMap[MAP_SIZE][MAP_SIZE] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1},
        {1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
        {1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1},
        {1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
        {1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
        {1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1},
        {1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
        {1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1},
        {1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
        {1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1} // Cup at (23,24), outer walls with open space
    };

    for (int y = 0; y < MAP_SIZE; ++y) {
        for (int x = 0; x < MAP_SIZE; ++x) {
            gameMap[x + y * MAP_SIZE] = predefinedMap[y][x];
            displayRects[x + y * MAP_SIZE] = {MAZE_OFFSET_X + x * TILE_SIZE, MAZE_OFFSET_Y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        }
    }

    player = {1, 1};
    opponent = {23, 23};
    playerRect = {MAZE_OFFSET_X + player.x * TILE_SIZE, MAZE_OFFSET_Y + player.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    opponentRect = {MAZE_OFFSET_X + opponent.x * TILE_SIZE, MAZE_OFFSET_Y + opponent.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    cupRect = {MAZE_OFFSET_X + 23 * TILE_SIZE, MAZE_OFFSET_Y + 24 * TILE_SIZE, TILE_SIZE, TILE_SIZE};

    numBoosters = 0;
    std::srand(std::time(0));
    while (numBoosters < 5) {
        int x = std::rand() % MAP_SIZE;
        int y = std::rand() % MAP_SIZE;
        if (gameMap[x + y * MAP_SIZE] == 0 && (x != player.x || y != player.y) && (x != 23 || y != 24)) {
            gameMap[x + y * MAP_SIZE] = 2;
            boosters[numBoosters++] = {MAZE_OFFSET_X + x * TILE_SIZE, MAZE_OFFSET_Y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        }
    }
}


// Render static maze for congratulatory screen
void renderMazeStatic(int gameMap[], SDL_Rect displayRects[], SDL_Rect playerRect, SDL_Rect opponentRect, SDL_Rect cupRect, SDL_Rect boosters[], int numBoosters) {
    float mazeScale = 0.5f; // Initial scale for pop-up effect
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTex, nullptr, nullptr);
    SDL_RenderSetScale(renderer, mazeScale, mazeScale);
    for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
        if (gameMap[i] == 1) SDL_RenderCopy(renderer, wallTex, nullptr, &displayRects[i]);
    }
    for (int i = 0; i < numBoosters; i++) {
        SDL_RenderCopy(renderer, orbTex, nullptr, &boosters[i]);
    }
    SDL_Rect floatingCupRect = cupRect;
    floatingCupRect.y += static_cast<int>(sin(0 * 0.05) * 10); // Static at frame 0
    SDL_RenderCopy(renderer, cupTex, nullptr, &floatingCupRect);
    SDL_RenderCopy(renderer, playerTex, nullptr, &playerRect);
    SDL_RenderCopy(renderer, opponentTex, nullptr, &opponentRect);
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);


    // Render UI
    SDL_Rect wandOutline = {10, 10, 24, 24};
    SDL_Rect boosterOutline = {10, 30, 24, 24};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &wandOutline);
    SDL_RenderFillRect(renderer, &boosterOutline);
    SDL_Rect wandIcon = {12, 12, 20, 20};
    SDL_Rect boosterIcon = {12, 32, 20, 20};
    SDL_RenderCopy(renderer, wandTex, nullptr, &wandIcon);
    SDL_RenderCopy(renderer, orbTex, nullptr, &boosterIcon);

    SDL_Color textColor = {255, 255, 255, 255};
    std::string wandText = "Wands: 0";
    std::string boosterText = "Boosters: " + std::to_string(numBoosters);
    SDL_Surface* wandSurface = TTF_RenderText_Solid(font, wandText.c_str(), textColor);
    SDL_Surface* boosterSurface = TTF_RenderText_Solid(font, boosterText.c_str(), textColor);
    SDL_Texture* wandTextTex = SDL_CreateTextureFromSurface(renderer, wandSurface);
    SDL_Texture* boosterTextTex = SDL_CreateTextureFromSurface(renderer, boosterSurface);
    SDL_Rect wandTextRect = {40, 12, wandSurface->w, wandSurface->h};
    SDL_Rect boosterTextRect = {40, 32, boosterSurface->w, boosterSurface->h};
    SDL_RenderCopy(renderer, wandTextTex, nullptr, &wandTextRect);
    SDL_RenderCopy(renderer, boosterTextTex, nullptr, &boosterTextRect);
    SDL_FreeSurface(wandSurface);
    SDL_FreeSurface(boosterSurface);
    SDL_DestroyTexture(wandTextTex);
    SDL_DestroyTexture(boosterTextTex);
}

// Riddle game function
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

    SDL_Color white = {255, 255, 255, 255}; // Moved to top of function
    std::string introText = " You've flown past flame and dodged the beast, The egg you hold a prize at least. But one mistake could now mislead.";
    SDL_Texture* introTexture = renderText(introText, white, 800);
    if (!introTexture) {
        std::cerr << "Failed to render intro text" << std::endl;
        return false;
    }
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
    if (!riddleTexture) {
        std::cerr << "Failed to render riddle text" << std::endl;
        SDL_DestroyTexture(introTexture);
        return false;
    }
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
                    Mix_HaltChannel(-1); // Stop egg click sound when riddle appears
                }
            }

            SDL_Rect eggRect = {static_cast<int>(eggX), static_cast<int>(showOpenEgg ? openEggY : eggY), 200, 200};
            SDL_RenderCopy(renderer, showOpenEgg ? openEggTexture : eggTexture, NULL, &eggRect);

            if (!eggClicked || !eggStable) {
                SDL_RenderCopy(renderer, introTexture, NULL, &introRect);
            }

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    SDL_DestroyTexture(introTexture);
                    SDL_DestroyTexture(riddleTexture);
                    return false;
                } else if (e.type == SDL_MOUSEBUTTONDOWN && !eggClicked) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    if (isMouseInside(eggRect, mx, my)) {
                        eggClicked = true;
                        Mix_PlayChannel(-1, eggClickSound, 0); // Play egg click sound
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
                if (txt) {
                    int tx, ty;
                    SDL_QueryTexture(txt, NULL, NULL, &tx, &ty);
                    SDL_Rect trect = {btn.rect.x + 20, btn.rect.y + 10, tx, ty};
                    SDL_RenderCopy(renderer, txt, NULL, &trect);
                    SDL_DestroyTexture(txt);
                }
            }
           
       //Main game event
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    SDL_DestroyTexture(introTexture);
                    SDL_DestroyTexture(riddleTexture);
                    return false;
                } else if (e.type == SDL_MOUSEBUTTONDOWN && !answered) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    for (auto& btn : buttons) {
                        if (isMouseInside(btn.rect, mx, my)) {
                            answered = true;
                            if (btn.isCorrect) {
                                Mix_PlayChannel(-1, correctSound, 0);
                                SDL_Delay(1200);
                                SDL_DestroyTexture(introTexture);
                                SDL_DestroyTexture(riddleTexture);
                                return true; // Proceed to maze
                            } else {
                                Mix_PlayChannel(-1, wrongSound, 0);
                                SDL_Delay(1200);
                                SDL_DestroyTexture(introTexture);
                                SDL_DestroyTexture(riddleTexture);
                                return false; // Restart riddle
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

// Maze game function
bool runMazeGame() {
    int gameMap[MAP_SIZE * MAP_SIZE];
    SDL_Rect displayRects[MAP_SIZE * MAP_SIZE];
    SDL_Point player;
    SDL_Rect playerRect;
    SDL_Point opponent;
    SDL_Rect opponentRect;
    SDL_Rect cupRect;
    SDL_Rect boosters[5];
    int numBoosters;

    // Initialize maze
    initMaze(gameMap, displayRects, player, playerRect, opponent, opponentRect, cupRect, boosters, numBoosters);

    int lives = 3;
    int wandUses = 0;
    bool running = true;
    int frameCount = 0;
    float mazeScale = 0.5f;
    bool opponentClose = false;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return false;
            else if (e.type == SDL_KEYDOWN) {
                int newX = player.x, newY = player.y;
                switch (e.key.keysym.sym) {
                    case SDLK_UP: newY--; break;
                    case SDLK_DOWN: newY++; break;
                    case SDLK_LEFT: newX--; break;
                    case SDLK_RIGHT: newX++; break;
                    case SDLK_SPACE:
                        if (wandUses > 0 && player.x == opponent.x && player.y == opponent.y) {
                            wandUses--;
                            opponent.x = 23; opponent.y = 23;
                            opponentRect.x = MAZE_OFFSET_X + opponent.x * TILE_SIZE;
                            opponentRect.y = MAZE_OFFSET_Y + opponent.y * TILE_SIZE;
                            std::cout << "Opponent defeated with wand! Wand uses left: " << wandUses << std::endl;
                        }
                        break;
                }
                if (e.key.keysym.sym != SDLK_SPACE &&
                    newX >= 0 && newX < MAP_SIZE &&
                    newY >= 0 && newY < MAP_SIZE &&
                    gameMap[newX + newY * MAP_SIZE] != 1) {
                    player.x = newX;
                    player.y = newY;
                    playerRect.x = MAZE_OFFSET_X + player.x * TILE_SIZE;
                    playerRect.y = MAZE_OFFSET_Y + player.y * TILE_SIZE;
                }
            }
        }

        // Enemy movement every 20 frames
        if (frameCount % 20 == 0) {
            int nextX, nextY;
            if (findNextStep(gameMap, opponent.x, opponent.y, player.x, player.y, nextX, nextY)) {
                opponent.x = nextX;
                opponent.y = nextY;
                opponentRect.x = MAZE_OFFSET_X + opponent.x * TILE_SIZE;
                opponentRect.y = MAZE_OFFSET_Y + opponent.y * TILE_SIZE;
            }

            // Check if opponent is close (within 2 tiles Manhattan distance)
            int distance = manhattanDistance(player.x, player.y, opponent.x, opponent.y);
            if (distance <= 5 && !opponentClose) {
                Mix_PlayChannel(-1, opponentCloseSound, 0); // Play sound when opponent gets close
                opponentClose = true;
            } else if (distance > 7 && opponentClose) {
                Mix_HaltChannel(-1); // Stop sound when opponent moves away
                opponentClose = false;
            }
        }

        // Booster collection
        if (gameMap[player.x + player.y * MAP_SIZE] == 2) {
            gameMap[player.x + player.y * MAP_SIZE] = 0;
            for (int i = 0; i < numBoosters; i++) {
                if ((boosters[i].x - MAZE_OFFSET_X) / TILE_SIZE == player.x && (boosters[i].y - MAZE_OFFSET_Y) / TILE_SIZE == player.y) {
                    if (i != numBoosters - 1) boosters[i] = boosters[numBoosters - 1];
                    numBoosters--;
                    break;
                }
            }
            wandUses++;
            std::cout << "Booster collected! Wand uses: " << wandUses << std::endl;
        }

        // Win condition
        if (gameMap[player.x + player.y * MAP_SIZE] == 3) {
            showWinScreen();
            return true; // Exit to main loop for potential replay
        }

        // Collision with opponent
        if (player.x == opponent.x && player.y == opponent.y) {
            if (wandUses > 0) {
                wandUses--;
                opponent.x = 23; opponent.y = 23;
                opponentRect.x = MAZE_OFFSET_X + opponent.x * TILE_SIZE;
                opponentRect.y = MAZE_OFFSET_Y + opponent.y * TILE_SIZE;
                std::cout << "Opponent defeated with wand! Wand uses left: " << wandUses << std::endl;
                Mix_HaltChannel(-1); // Stop sound on wand use
                opponentClose = false;
            } else {
                lives--;
                std::cout << "Hit by enemy! Lives left: " << lives << std::endl;
                if (lives <= 0) {
                    showGameOverScreen();
                    // Reinitialize maze to restart
                    initMaze(gameMap, displayRects, player, playerRect, opponent, opponentRect, cupRect, boosters, numBoosters);
                    lives = 3;
                    wandUses = 0;
                    frameCount = 0;
                    mazeScale = 0.5f;
                    Mix_HaltChannel(-1); // Stop sound on game over
                    opponentClose = false;
                    continue; // Continue the game loop to restart
                }
            }
        }

        // Update maze scale for pop-up effect
        if (frameCount < 30) {
            mazeScale = 0.5f + (0.5f * frameCount / 30.0f);
        } else {
            mazeScale = 1.0f;
        }

        // Render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTex, nullptr, nullptr);
        SDL_RenderSetScale(renderer, mazeScale, mazeScale);
        for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
            if (gameMap[i] == 1) SDL_RenderCopy(renderer, wallTex, nullptr, &displayRects[i]);
        }
        for (int i = 0; i < numBoosters; i++) {
            SDL_RenderCopy(renderer, orbTex, nullptr, &boosters[i]);
        }
        SDL_Rect floatingCupRect = cupRect;
        floatingCupRect.y += static_cast<int>(sin(frameCount * 0.05) * 10);
        SDL_RenderCopy(renderer, cupTex, nullptr, &floatingCupRect);
        SDL_RenderCopy(renderer, playerTex, nullptr, &playerRect);
        SDL_RenderCopy(renderer, opponentTex, nullptr, &opponentRect);
        SDL_RenderSetScale(renderer, 1.0f, 1.0f);

        // Render UI
        SDL_Rect wandOutline = {10, 10, 24, 24};
        SDL_Rect boosterOutline = {10, 30, 24, 24};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &wandOutline);
        SDL_RenderFillRect(renderer, &boosterOutline);
        SDL_Rect wandIcon = {12, 12, 20, 20};
        SDL_Rect boosterIcon = {12, 32, 20, 20};
        SDL_RenderCopy(renderer, wandTex, nullptr, &wandIcon);
        SDL_RenderCopy(renderer, orbTex, nullptr, &boosterIcon);

        SDL_Color textColor = {255, 255, 255, 255};
        std::string wandText = "Wands: " + std::to_string(wandUses);
        std::string boosterText = "Boosters: " + std::to_string(numBoosters);
        SDL_Surface* wandSurface = TTF_RenderText_Solid(font, wandText.c_str(), textColor);
        SDL_Surface* boosterSurface = TTF_RenderText_Solid(font, boosterText.c_str(), textColor);
        SDL_Texture* wandTextTex = SDL_CreateTextureFromSurface(renderer, wandSurface);
        SDL_Texture* boosterTextTex = SDL_CreateTextureFromSurface(renderer, boosterSurface);
        SDL_Rect wandTextRect = {40, 12, wandSurface->w, wandSurface->h};
        SDL_Rect boosterTextRect = {40, 32, boosterSurface->w, boosterSurface->h};
        SDL_RenderCopy(renderer, wandTextTex, nullptr, &wandTextRect);
        SDL_RenderCopy(renderer, boosterTextTex, nullptr, &boosterTextRect);
        SDL_FreeSurface(wandSurface);
        SDL_FreeSurface(boosterSurface);
        SDL_DestroyTexture(wandTextTex);
        SDL_DestroyTexture(boosterTextTex);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
        frameCount++;
    }
    return false;
}

// Show congratulatory screen before maze
void showNextPage() {
    // Initialize maze state for background
    int gameMap[MAP_SIZE * MAP_SIZE];
    SDL_Rect displayRects[MAP_SIZE * MAP_SIZE];
    SDL_Point player;
    SDL_Rect playerRect;
    SDL_Point opponent;
    SDL_Rect opponentRect;
    SDL_Rect cupRect;
    SDL_Rect boosters[5];
    int numBoosters;
    initMaze(gameMap, displayRects, player, playerRect, opponent, opponentRect, cupRect, boosters, numBoosters);

    Uint32 startTime = SDL_GetTicks();
    const Uint32 DISPLAY_DURATION = 2000; // 2 seconds
    bool running = true;
    SDL_Event e;

    while (running && (SDL_GetTicks() - startTime < DISPLAY_DURATION)) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        // Render maze background
        renderMazeStatic(gameMap, displayRects, playerRect, opponentRect, cupRect, boosters, numBoosters);

        // Draw 50% transparent overlay
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlayRect);

        // Render congratulatory text
        SDL_Color white = {255, 255, 255, 255};
        std::string nextText = "Congratulations! You've solved the riddle and reached the next stage!";
        SDL_Texture* nextTexture = renderText(nextText, white, 1000);
        if (nextTexture) {
            int tw, th;
            SDL_QueryTexture(nextTexture, NULL, NULL, &tw, &th);
            SDL_Rect nextRect = {(SCREEN_WIDTH - tw) / 2, (SCREEN_HEIGHT - th) / 2, tw, th};
            SDL_RenderCopy(renderer, nextTexture, NULL, &nextRect);
            SDL_DestroyTexture(nextTexture);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }
}

// Show win screen after winning the cup
void showWinScreen() {
    Mix_HaltChannel(-1); // Stop opponent close sound when game is finished
    SDL_Event e;
    bool running = true;
    SDL_Color white = {255, 255, 255, 255};
    Uint32 startTime = SDL_GetTicks();
    float time = 0.0f;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                SDL_Rect playAgainRect = {(SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT + 350) / 2, 200, 50};
                if (isMouseInside(playAgainRect, mx, my)) {
                    running = false; // Restart the game
                }
            }
        }

        // Render background with overlay
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTex, nullptr, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlayRect);

        // Render "Winner!!" text
       
       
        std::string winnerText = "Winner!!";
        SDL_Texture* winnerTexture = renderText(winnerText, white, 0);
        if (winnerTexture) {
            int tw, th;
            SDL_QueryTexture(winnerTexture, NULL, NULL, &tw, &th);
            SDL_Rect winnerRect = {(SCREEN_WIDTH - tw) / 2, (SCREEN_HEIGHT - 500) / 2, tw, th};
            SDL_RenderCopy(renderer, winnerTexture, NULL, &winnerRect);
            SDL_DestroyTexture(winnerTexture);
        }

        // Render Triwizard cup image with small float movement
        time = (SDL_GetTicks() - startTime) / 1000.0f; // Convert to seconds
        int floatOffset = static_cast<int>(15.0f * sin(time * 0.5f)); // Small amplitude (15 pixels), slow frequency (0.5 Hz)
        SDL_Rect cupRect = {(SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 150) / 2 + floatOffset, 200, 200};
        SDL_RenderCopy(renderer, cupTex, nullptr, &cupRect);

        // Render "Play Again" button
         SDL_Rect playAgainRect = {(SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT + 350) / 2, 200, 50};
        SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // Green button
        SDL_RenderFillRect(renderer, &playAgainRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &playAgainRect);
        std::string playAgainText = "Play Again";
        SDL_Texture* playAgainTexture = renderText(playAgainText, white, 0);
        if (playAgainTexture) {
            int tw, th;
            SDL_QueryTexture(playAgainTexture, NULL, NULL, &tw, &th);
            SDL_Rect playAgainTextRect = {playAgainRect.x + (playAgainRect.w - tw) / 2, playAgainRect.y + (playAgainRect.h - th) / 2, tw, th};
            SDL_RenderCopy(renderer, playAgainTexture, NULL, &playAgainTextRect);
            SDL_DestroyTexture(playAgainTexture);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }
}

// Show game over screen
void showGameOverScreen() {
    Mix_HaltChannel(-1); // Stop opponent close sound when game is finished
    SDL_Event e;
    bool running = true;
    SDL_Color white = {255, 255, 255, 255};

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                SDL_Rect playAgainRect = {(SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT + 250) / 2, 200, 50};
                if (isMouseInside(playAgainRect, mx, my)) {
                    running = false; // Exit to restart maze
                }
            }
        }

        // Render background with overlay
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTex, nullptr, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlayRect);

        // Render game over image with increased size
        SDL_Rect gameOverRect = {(SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 400) / 2, 400, 400};
        SDL_RenderCopy(renderer, gameOverTex, nullptr, &gameOverRect);

        // Render "Play Again" button
        SDL_Rect playAgainRect = {(SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT + 250) / 2, 200, 50};
        SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // Green button
        SDL_RenderFillRect(renderer, &playAgainRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &playAgainRect);
        std::string playAgainText = "Play Again";
        SDL_Texture* playAgainTexture = renderText(playAgainText, white, 0);
        if (playAgainTexture) {
            int tw, th;
            SDL_QueryTexture(playAgainTexture, NULL, NULL, &tw, &th);
            SDL_Rect playAgainTextRect = {playAgainRect.x + (playAgainRect.w - tw) / 2, playAgainRect.y + (playAgainRect.h - th) / 2, tw, th};
            SDL_RenderCopy(renderer, playAgainTexture, NULL, &playAgainTextRect);
            SDL_DestroyTexture(playAgainTexture);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer initialization failed: " << Mix_GetError() << std::endl;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("Triwizard Adventure", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    font = TTF_OpenFont("assets/font.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font 'assets/font.ttf': " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load riddle assets
    bgTexture = IMG_LoadTexture(renderer, "assets/riddle_bg1.png");
    if (!bgTexture) {
        std::cerr << "Failed to load texture 'assets/riddle_bg1.png': " << IMG_GetError() << std::endl;
    }

    blurredBgTexture = IMG_LoadTexture(renderer, "assets/riddle_bg1.png");
    if (!blurredBgTexture) {
        std::cerr << "Failed to load texture 'assets/riddle_bg1.png' (blurred): " << IMG_GetError() << std::endl;
    } else {
        SDL_SetTextureBlendMode(blurredBgTexture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(blurredBgTexture, 128);
    }

    eggTexture = IMG_LoadTexture(renderer, "assets/golden_egg.png");
    if (!eggTexture) {
        std::cerr << "Failed to load texture 'assets/golden_egg.png': " << IMG_GetError() << std::endl;
    }

    openEggTexture = IMG_LoadTexture(renderer, "assets/open_golden_egg1.png");
    if (!openEggTexture) {
        std::cerr << "Failed to load texture 'assets/open_golden_egg1.png': " << IMG_GetError() << std::endl;
    }
     //Load sound effects for riddle logic
    correctSound = Mix_LoadWAV("assets/correct.wav");
    if (!correctSound) {
        std::cerr << "Failed to load sound 'assets/correct.wav': " << Mix_GetError() << std::endl;
    }

    wrongSound = Mix_LoadWAV("assets/wrong.wav");
    if (!wrongSound) {
        std::cerr << "Failed to load sound 'assets/wrong.wav': " << Mix_GetError() << std::endl;
    }

    eggClickSound = Mix_LoadWAV("assets/egg_click.wav");
    if (!eggClickSound) {
        std::cerr << "Failed to load sound 'assets/egg_click.wav': " << Mix_GetError() << std::endl;
    }

    // Load maze assets
    backgroundTex = IMG_LoadTexture(renderer, "assets/bg.png");
    if (backgroundTex) {
        SDL_SetTextureBlendMode(backgroundTex, SDL_BLENDMODE_NONE); // Independent rendering
    } else {
        std::cerr << "Failed to load texture 'assets/bg.png': " << IMG_GetError() << std::endl;
    }
  //Load wall texture used in maze layout
    wallTex = IMG_LoadTexture(renderer, "assets/wall4.png");
    if (wallTex) {
        int w, h;
        SDL_QueryTexture(wallTex, NULL, NULL, &w, &h);
        std::cout << "wall4.png loaded successfully, dimensions: " << w << "x" << h << std::endl;
        SDL_SetTextureBlendMode(wallTex, SDL_BLENDMODE_NONE); // Preserve original image colors
    } else {
        std::cerr << "Failed to load texture 'assets/wall4.png': " << IMG_GetError() << std::endl;
        return 1; // Exit if wall texture fails to load
    }
   //Load player sprite for maze
    playerTex = IMG_LoadTexture(renderer, "assets/sprite3.png");
    if (!playerTex) {
        std::cerr << "Failed to load texture 'assets/sprite3.png': " << IMG_GetError() << std::endl;
    }
    //Load transparent maze
    opponentTex = IMG_LoadTexture(renderer, "assets/trap1.png");
    if (!opponentTex) {
        std::cerr << "Failed to load texture 'assets/trap1.png': " << IMG_GetError() << std::endl;
    }
   //Load booster orb 
    orbTex = IMG_LoadTexture(renderer, "assets/booster1.png");
    if (!orbTex) {
        std::cerr << "Failed to load texture 'assets/booster1.png': " << IMG_GetError() << std::endl;
    }

    wandTex = IMG_LoadTexture(renderer, "assets/wand.png");
    if (!wandTex) {
        std::cerr << "Failed to load texture 'assets/wand.png': " << IMG_GetError() << std::endl;
    }

    cupTex = IMG_LoadTexture(renderer, "assets/cup.png");
    if (!cupTex) {
        std::cerr << "Failed to load texture 'assets/cup.png': " << IMG_GetError() << std::endl;
    }

    gameOverTex = IMG_LoadTexture(renderer, "assets/game_over_window.png");
    if (!gameOverTex) {
        std::cerr << "Failed to load texture 'assets/game_over.png': " << IMG_GetError() << std::endl;
    }

    opponentCloseSound = Mix_LoadWAV("assets/opponent_close.wav");
    if (!opponentCloseSound) {
        std::cerr << "Failed to load sound 'assets/opponent_close.wav': " << Mix_GetError() << std::endl;
    }

    // Check if any critical assets failed to load
    if (!bgTexture || !blurredBgTexture || !eggTexture || !openEggTexture || !correctSound || !wrongSound || !eggClickSound ||
        !playerTex || !opponentTex || !backgroundTex || !orbTex || !wandTex || !cupTex || !gameOverTex || !opponentCloseSound) {
        std::cerr << "One or more assets failed to load. Exiting.\n";
        // Cleanup loaded assets before exiting
        if (bgTexture) SDL_DestroyTexture(bgTexture);
        if (blurredBgTexture) SDL_DestroyTexture(blurredBgTexture);
        if (eggTexture) SDL_DestroyTexture(eggTexture);
        if (openEggTexture) SDL_DestroyTexture(openEggTexture);
        if (correctSound) Mix_FreeChunk(correctSound);
        if (wrongSound) Mix_FreeChunk(wrongSound);
        if (eggClickSound) Mix_FreeChunk(eggClickSound);
        if (opponentCloseSound) Mix_FreeChunk(opponentCloseSound);
        if (playerTex) SDL_DestroyTexture(playerTex);
        if (opponentTex) SDL_DestroyTexture(opponentTex);
        if (backgroundTex) SDL_DestroyTexture(backgroundTex);
        if (orbTex) SDL_DestroyTexture(orbTex);
        if (wandTex) SDL_DestroyTexture(wandTex);
        if (wallTex) SDL_DestroyTexture(wallTex);
        if (cupTex) SDL_DestroyTexture(cupTex);
        if (gameOverTex) SDL_DestroyTexture(gameOverTex);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    while (!quit) {
        bool solvedRiddle = runRiddleGame();
        if (!solvedRiddle) {
            continue; // Restart riddle on incorrect answer or window close
        }
        showNextPage();
        bool wonMaze = runMazeGame();
        if (wonMaze) {
            // Allow replay by looping back to maze if "Play Again" is clicked
        } else {
            quit = true; // Exit on game over unless "Play Again" is clicked in game over screen
        }
    }

    // Cleanup
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(blurredBgTexture);
    SDL_DestroyTexture(eggTexture);
    SDL_DestroyTexture(openEggTexture);
    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(opponentTex);
    SDL_DestroyTexture(backgroundTex);
    SDL_DestroyTexture(orbTex);
    SDL_DestroyTexture(wandTex);
    SDL_DestroyTexture(wallTex);
    SDL_DestroyTexture(cupTex);
    SDL_DestroyTexture(gameOverTex);
    Mix_FreeChunk(correctSound);
    Mix_FreeChunk(wrongSound);
    Mix_FreeChunk(eggClickSound);
    Mix_FreeChunk(opponentCloseSound);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
