# Broken Stance

A 2D fighting game built in C++ using SFML (Simple and Fast Multimedia Library)

## Features
- **Local PvP**: Two-player combat on the same machine
- **Smooth Animations**: Character animations for idle, running, jumping, attacking, and hurt states
- **Health System**: Real-time health bars and damage tracking
- **Collision Detection**: Player-to-player collision and attack hitboxes
- **Physics**: Gravity and jump mechanics
- **Menu System**: Main menu with instructions screen

## Controls

### Player 1
- **A/D**: Move Left/Right
- **W**: Jump
- **Q**: Attack

### Player 2
- **Arrow Keys**: Move Left/Right
- **Up Arrow**: Jump
- **K**: Attack

### General
- **ESC**: Return to Menu (or Exit from Menu)
- **R**: Restart Match (Game Over screen)

## Requirements
- C++ Compiler (g++ recommended)
- SFML 2.5+ (Graphics, Window, System modules)
- Windows/Linux/MacOS

## Building & Running

### Windows (MinGW)
```bash
g++ game.cpp -o game.exe -IC:/SFML/include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

**Note:** Adjust the SFML include and lib paths according to your SFML installation location.

### Linux
```bash
g++ game.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
```

## Project Structure
```
broken-stance/
├── game.cpp           # Main game source code
├── README.md          # Project documentation
└── Assets/
    ├── Fonts/         # Game fonts
    └── Sprites/       # Character sprites and backgrounds
        ├── player1/   # Player 1 animations
        └── player2/   # Player 2 animations
```

## Future Plans
- Online multiplayer
- AI opponents
- More characters
- Special moves and combos
- Sound effects and music

## Technologies Used
- **C++**: Core game logic
- **SFML**: Graphics, window management, and input handling

---

Made with ❤️ for learning game development
