📁 README.txt — Project Guide and Assignment Overview

🎮 Project Title: Hogwarts: The Triwizard Trials

📜 Assignment Overview:
This assignment is a 2D side-scrolling + maze puzzle game inspired by Harry Potter and the Goblet of Fire. The player participates in magical trials, solving riddles, collecting magical items, escaping trap rooms, and facing mythical creatures such as dragons.

The project is divided into two main levels:
1. Level 1 (Runner + Dragon Encounter)
   - A Temple-Run-style horizontal runner game.
   - Player dodges obstacles, collects golden eggs, and evades a dragon.
   - The game ends when the player collects a golden egg or collides fatally.

2. Level 2 (Riddle + Maze Challenge)
   - Starts with a scroll-based riddle interface.
   - If the riddle is answered correctly, the player enters a complex maze.
   - The maze includes boosters, wand logic, enemies, and a final exit condition.

🛠 Requirements:
- Operating System: Ubuntu Linux (preferred)
- Compiler: g++
- Libraries:
  - SDL2
  - SDL2_image
  - SDL2_ttf
  - SDL2_mixer
  - SDL2_gfx

Install necessary libraries using:
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx-dev

🚀 How to Compile and Run:

🔧 Step 1:
g++ level1final.cpp -o level1 -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

🔧 Step 2: 
g++ level2final.cpp -o level2 -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

🔧 Step 3:
chmod +x level1final level2final

🔧 Step 4:
./level1final

📁 Project File Structure:
/Project
├── assets/                   → Images, fonts, music
│   ├── background/
│   ├── player/
│   ├── dragon/
│   ├── riddle/
│   ├── maze/
│   └── fonts/
├── level1final.cpp           → Level 1: Runner + Dragon Game
├── level2final.cpp           → Level 2: Riddle + Maze
├── Makefile                  → (Optional) Build automation
├── readme.txt                → Project description (this file)

🎯 Objectives Achieved:
- Implemented smooth player animation and collision in SDL2.
- Scroll-based riddle system with interactive options.
- Maze with enemy AI, boosters, wand mechanics, and trap logic.
- Sound and image integration.
- Level transition from Level 1 → Level 2.


Submitted by:

1. Adeba Jahan 
2. Shuchita Islam Shuvra
3. Samarina Sarwar
4. Dipa Biswas

Submission Date: 12/07/2025
Batch:30
Department of Computer Science and Engineering, University Of Dhaka.
