# The Dragon's Den - Text-Based Adventure Game

## Overview
**The Dragon's Den** is a text-based adventure game developed as part of the **Programming 2** course during my second semester. The game challenges players to explore a cave system filled with interconnected rooms, avoid hazards like mines and portals, and ultimately defeat the dragon using a limited number of arrows. The game features a menu-driven interface, random room generation, and the ability to save and load game progress.

## Features
- **Randomized Cave System**: The cave system is dynamically generated based on a difficulty level.
- **Player Interaction**: Move between rooms, shoot arrows, and search for additional arrows.
- **Hazards**: Avoid mines, navigate portals, and defeat the dragon.
- **Save and Load**: Save your progress to a file and load it later to continue playing.
- **Menu-Driven Interface**: Easy-to-use menus for starting, saving, loading, and quitting the game.
- **Graph-Based Structure**: The cave system is represented as a graph, allowing for dynamic room connections and traversal.

## How to Play
1. **Start the Game**:
   - Run the program using the following command:
     ```bash
     ./play [level_factor]
     ```
     - `level_factor` (optional): Determines the difficulty level of the game. If not provided, the default is 0.

2. **Main Menu**:
   - **New**: Start a new game.
   - **Load**: Load a previously saved game.
   - **Save**: Save the current game progress.
   - **Continue**: Continue playing the current game.
   - **Help**: View game instructions.
   - **Quit**: Exit the game.

3. **Gameplay**:
   - **Move**: Move to an adjacent room by specifying its ID.
   - **Shoot**: Shoot an arrow into an adjacent room to defeat the dragon.
   - **Exit**: Exit the caves and return to the main menu.

4. **Hazards**:
   - **Mines**: Triggering a mine destroys the room and teleports you to a random room.
   - **Portals**: Entering a portal room teleports you to a new, unexplored room.
   - **Dragon**: Defeat the dragon by shooting an arrow into its room. Entering the dragon's room results in a loss.

5. **Saving and Loading**:
   - Save your progress to a file and load it later to continue playing. The save file is named after the player and has a `.sav` extension.

## Files
- **`hw3.c`**: The main program that handles the game loop, menus, and player interaction.
- **`game.c`**: Contains the game logic, including room traversal, hazards, and player actions.
- **`graph.c`**: Implements the graph-based structure of the cave system, including room connections and traversal.
- **`fileio.c`**: Handles saving and loading game progress to/from files.
- **`random.c`**: Implements a custom random number generator for room generation and events.
- **`msg.h`**: Contains all the text messages displayed in the game.
- **`libhw3.h`**: Header file for graph-related functions.
- **`graph.h`**: Defines the graph and vertex structures used in the game.
- **`game.h`**: Defines game-related structures and functions.
- **`random.h`**: Header file for the random number generator.

## Dependencies
- **Standard C Libraries**: The program relies on standard C libraries such as `stdio.h`, `stdlib.h`, `string.h`, and `unistd.h`.
- **Custom Headers**: The program uses custom headers (`msg.h`, `libhw3.h`, `graph.h`, `game.h`, `random.h`) for game logic and data structures.

## Compilation
The project includes a `Makefile` to simplify the build process. The `Makefile` contains the following rules:

- **`play`**: Compiles the game executable.
- **`clean`**: Removes all object files and the executable.
- **`test1` and `test2`**: Run automated tests with input files `in.1` and `in.2`, and compare the output with `out.1` and `out.2`.
- **`tests`**: Runs both `test1` and `test2`.

To compile the program, use the following command:
```bash
make