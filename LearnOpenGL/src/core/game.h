#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H 

#include "../entities/game_level.h"
#include "../entities/characters.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

struct BackgroundLayer {
    Texture2D texture;
    glm::vec2 position;
    float velocity;
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState                       State;
    bool                            Keys[1024];
    bool                            MouseInput[3];
    unsigned int                    Width, Height;
    std::vector<GameLevel>          Levels;
    unsigned int                    Level;
    std::vector<BackgroundLayer>    BackgroundLayers;
    std::vector<GameObject>         Clouds;
    float                           LastCloud;
    std::vector<GameObject>         Stars;
    std::vector<GameObject>         Apples;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init(Characters& textCharacters);
    // game loop
    void ProcessInput(float dt, float MouseX, float MouseY);
    void Update(float dt);
    void Render();
};

#endif