#pragma once
/*
	Run, handle events, update, render, fetch game state
*/
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Tipuri de celule pentru hartă
enum class CellType { FREE, BREAKABLE, UNBREAKABLE };

// Starea aplicației
enum class ClientState { MENU, GAME, LOGIN };

// Clasa ClientLogic
class ClientLogic {
public:
    // Constructor și destructor
    ClientLogic();
    ~ClientLogic();

    // Funcția principală care pornește jocul
    void run();

private:
    // Membri principali
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    SDL_Texture* m_freeCellTexture;
    SDL_Texture* m_breakableCellTexture;
    SDL_Texture* m_unbreakableCellTexture;
    SDL_Texture* m_playerTexture;

    int m_windowWidth;
    int m_windowHeight;

    ClientState m_state;

    int m_playerX, m_playerY;
    int m_mapWidth, m_mapHeight;
    std::vector<std::vector<CellType>> m_map;

    // Funcții de gestionare
    bool initSDL();
    void handleEvents(bool& running);
    void update();
    void render();
    void fetchGameState();

    // Funcții utilitare
    void initializeTextures();
    void initializeMap();
    SDL_Texture* loadTexture(const std::string& filePath, SDL_Renderer* renderer);
    void renderGame();
    void drawText(const std::string& text, int x, int y, SDL_Color color);
};
