
# Snake Game with Data Structures

A modern implementation of the classic Snake game in C++ using SFML, featuring advanced data structures and algorithms for educational purposes.

##  Features

### Core Gameplay
- **Classic Snake Mechanics**: Move the snake to collect food and grow longer
- **Progressive Difficulty**: Increasing speed and complexity with each level
- **Multiple Food Items**: Up to 3 food items spawned simultaneously
- **Dynamic Wall Generation**: Procedurally generated walls based on current level
- **Collision Detection**: Advanced collision system using graph-based validation

### Data Structures Implementation
- **Linked List**: Snake body representation for efficient insertion/deletion operations
- **Stack**: Score history management for undo functionality
- **Queue**: Recent scores tracking with FIFO behavior
- **Graph**: Wall layout and movement validation using adjacency lists
- **Vector**: Food positions and high scores storage

### Audio & Visual Features
- **Background Music**: Looping background soundtrack
- **Sound Effects**: Eat, collision, and level-up audio feedback
- **Textured Graphics**: Custom apple texture for food items
- **Modern UI**: Clean interface with real-time game statistics
- **Grid-based Rendering**: Smooth grid system with visual feedback

### Game States
- **Main Menu**: Start game, view high scores, or exit
- **Playing**: Active gameplay with full controls
- **Paused**: Pause/resume functionality
- **Game Over**: Final score display and restart options
- **High Scores**: Persistent leaderboard system

##  Technical Architecture

### Class Structure

```
Game (Main Controller)
├── Snake (Linked List Implementation)
├── Food (Vector-based Management)
├── ScoreManager (Stack & Queue Implementation)
└── Graph (Adjacency List for Walls)
```

### Key Components

#### Snake Class
- Uses `std::list<SnakeSegment>` for body representation
- Efficient head insertion and tail removal
- Direction validation to prevent immediate reversal
- Self-collision detection algorithm

#### Food Class
- Manages multiple food items using `std::vector`
- Random spawn algorithm with collision avoidance
- Texture loading and rendering system
- Dynamic food count maintenance

#### ScoreManager Class
- **Stack**: Implements undo functionality for score history
- **Queue**: Tracks recent score additions (FIFO)
- **Vector**: Maintains sorted high scores leaderboard
- Timestamp tracking for score entries

#### Graph Class
- Adjacency list representation for grid connectivity
- Wall placement and removal algorithms
- Movement validation system
- Level-based procedural wall generation

##  Controls

| Key | Action |
|-----|--------|
| `W` | Move Up |
| `A` | Move Left |
| `S` | Move Down |
| `D` | Move Right |
| `P` | Pause/Resume |
| `U` | Undo Last Score |
| `H` | View High Scores (Menu) |
| `Space` | Start Game / Return to Menu |
| `R` | Restart (Game Over) |
| `ESC` | Exit Game |

##  Prerequisites

### System Requirements
- **Operating System**: Windows 10/11, Linux (Ubuntu 18.04+), macOS 10.14+
- **Compiler**: C++17 compatible compiler
  - Windows: Visual Studio 2019+ or MinGW-w64
  - Linux: GCC 7+ or Clang 6+
  - macOS: Xcode 10+ or Clang 6+
- **Memory**: 512 MB RAM minimum
- **Storage**: 100 MB available space

### Dependencies
- **SFML 2.5+**: Simple and Fast Multimedia Library
- **CMake 3.16+**: Build system generator

##  Installation

### Method 1: Download Pre-built Release (Recommended)
1. Visit the [Releases Page](https://github.com/Vansh12970/Snake_Game/releases)
2. Download the latest release for your platform
3. Extract the archive
4. Run the executable directly

### Method 2: Build from Source

#### Windows (Visual Studio)
```bash
# Install vcpkg (if not already installed)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install SFML
.\vcpkg install sfml:x64-windows

# Clone and build the project
git clone https://github.com/Vansh12970/Snake_Game.git
cd Snake_Game
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

#### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install libsfml-dev cmake build-essential git

# Clone and build
git clone https://github.com/Vansh12970/Snake_Game.git
cd Snake_Game
mkdir build && cd build
cmake ..
make -j$(nproc)
```

#### macOS
```bash
# Install dependencies using Homebrew
brew install sfml cmake

# Clone and build
git clone https://github.com/Vansh12970/Snake_Game.git
cd Snake_Game
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

##  Project Structure

```
Snake_Game/
├── .vscode/                    # VS Code configuration
│   ├── c_cpp_properties.json  # IntelliSense settings
│   └── settings.json          # Editor settings
├── assets/                     # Game assets (not in repo)
│   ├── audio/                 # Sound effects and music
│   │   ├── background.ogg     # Background music
│   │   ├── eat.mp3           # Food consumption sound
│   │   ├── hit.wav           # Collision sound
│   │   └── levelup.wav       # Level progression sound
│   └── images/               # Textures and sprites
│       └── apple.png         # Food texture
├── src/                      # Source code
│   ├── Food.cpp/h           # Food management system
│   ├── Game.cpp/h           # Main game controller
│   ├── Graph.cpp/h          # Wall and movement validation
│   ├── ScoreManager.cpp/h   # Scoring and statistics
│   ├── Snake.cpp/h          # Snake entity and logic
│   └── main.cpp             # Application entry point
├── CMakeLists.txt           # Build configuration
├── README.md               # This documentation
└── .gitignore             # Git ignore rules
```

##  Educational Value

### Data Structures Demonstrated

1. **Linked List (Snake Body)**
   - Dynamic size management
   - Efficient insertion/deletion at ends
   - Memory-efficient representation

2. **Stack (Score History)**
   - LIFO (Last In, First Out) operations
   - Undo functionality implementation
   - Memory management for historical data

3. **Queue (Recent Scores)**
   - FIFO (First In, First Out) operations
   - Circular buffer behavior
   - Real-time data tracking

4. **Graph (Game Board)**
   - Adjacency list representation
   - Path validation algorithms
   - Dynamic graph modification

5. **Vector (High Scores & Food)**
   - Dynamic array implementation
   - Sorting algorithms integration
   - Random access capabilities

### Algorithms Implemented

- **Collision Detection**: Efficient O(n) snake self-collision
- **Pathfinding Validation**: Graph-based movement validation
- **Random Generation**: Pseudorandom food placement with conflict resolution
- **Sorting**: High score leaderboard maintenance
- **Memory Management**: Efficient resource allocation and deallocation

##  Configuration

### Game Settings
The game can be configured by modifying constants in `Game.h`:

```cpp
static const int GRID_WIDTH = 30;    // Game board width
static const int GRID_HEIGHT = 20;   // Game board height
static const int CELL_SIZE = 25;     // Pixel size per grid cell
```

### Audio Settings
- Background music volume: 10% (adjustable in `Game.cpp`)
- Sound effects volume: 100% (adjustable in `Game.cpp`)

### Performance Settings
- Frame rate limit: 60 FPS
- Initial game speed: 0.15 seconds per move
- Speed increase per level: 0.01 seconds reduction

##  Troubleshooting

### Common Issues

**Issue**: Game crashes on startup
- **Solution**: Ensure SFML libraries are properly installed and accessible
- **Check**: Verify asset files are in the correct directory structure

**Issue**: No sound/music
- **Solution**: Check audio file paths and formats
- **Note**: Ensure system audio is not muted

**Issue**: Build fails with CMake
- **Solution**: Verify CMake version (3.16+) and SFML installation
- **Windows**: Ensure proper vcpkg integration

**Issue**: Font not loading
- **Solution**: The game tries multiple font paths; system fonts should work automatically
- **Fallback**: Default SFML font will be used if system fonts fail

### Debug Information
Enable console output by running the executable from command line to see detailed debug information including:
- Asset loading status
- Snake movement coordinates
- Wall collision detection
- Audio system initialization

##  Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Code Style Guidelines
- Use modern C++17 features
- Follow RAII principles
- Maintain const-correctness
- Document complex algorithms
- Keep functions focused and small


##  Acknowledgments

- **SFML Team**: For the excellent multimedia library
- **Data Structures Community**: For educational inspiration
- **Game Development Community**: For best practices and optimization techniques


---

*Built with ❤️ for educational purposes and gaming enjoyment*
