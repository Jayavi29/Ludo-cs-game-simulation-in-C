# LUDO-CS Game Simulation

A command-line implementation of the Ludo game developed in C, focusing on
game-state management, piece movement, player turns, capturing, and home-path
logic.

## Overview

LUDO-CS simulates a four-player Ludo game using structured data models and
rule-based game logic. Each player has four pieces, with the game maintaining
the state of pieces across the base, main board path, home straight, and home.

The project was developed as a programming and data-structure exercise,
with emphasis on modular game logic and efficient state management.

## Features

- Four-player Ludo game simulation
- Four pieces per player
- Random dice rolling
- Player turn management
- Cyclic player order
- Piece movement around a 52-position board
- Entering pieces onto the board when a six is rolled
- Piece capturing and returning captured pieces to the base
- Home-straight movement
- Tracking pieces in the base, on the board, and at home
- Automatic determination of the starting player based on dice rolls
- Command-line game output
- Structured representation of players and pieces

## Game Representation

The game uses two main structures:

### Piece

Each piece stores:

- `location` — represents the current state/location of the piece
- `index` — identifies the piece
- `captureCounter` — stores the number of captures made

Piece locations are represented using:

- `-2` — Base
- `-1` — Home
- `0–51` — Main board path
- `-10–-5` — Home straight

### Player

Each player stores:

- Player color
- Four pieces
- Number of pieces on the path
- Number of pieces in the base
- Number of pieces at home
- Starting position
- Home-straight entry position

This structure allows the complete state of each player to be managed
through a single data model.

## Main Functions

| Function | Purpose |
|----------|---------|
| `diceRoll()` | Generates a random dice value |
| `nextplayer()` | Determines the next player in cyclic order |
| `InitializePlayer()` | Initializes players and their pieces |
| `getPiecesInPath()` | Identifies pieces currently on the main path |
| `updatePosition()` | Updates piece counts for each player |
| `isCapture()` | Checks whether a move captures an opponent's piece |
| `reset()` | Returns a captured piece to its base |
| `movePiece()` | Handles piece movement and capturing logic |
| `playGame()` | Controls the main game flow |

## Data Structures and Concepts

The project demonstrates the use of:

- Structures in C
- Arrays of structures
- Pointers
- Modular functions
- Global game-state management
- Conditional game logic
- Random number generation
- Array traversal
- Basic algorithmic complexity analysis

## Technologies

- **Language:** C
- **Concepts:** Data Structures, Algorithms, Game-State Management
- **Compiler:** GCC
- **Interface:** Command Line

## Project Structure

```text
FInal Ludo/
│
├── logic.c
├── logic_experiment.c
├── types.h
└── Final Ludo report.latex