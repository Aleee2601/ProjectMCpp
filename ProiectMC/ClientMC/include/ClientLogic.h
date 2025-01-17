﻿#pragma once

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ClientFunctions.h"



// Starea aplicației
enum class ClientState { MENU, GAME, LOGIN, REGISTER , PAIR_GAME};

// Tipuri de celule pentru hartă
enum CellTypeC {
    EMPTY,
    DESTRUCTIBLE_WALL,
    INDESTRUCTIBLE_WALL
};

// Clasa ClientLogic
class ClientLogic {
public:
    // Constructor și destructor
    ClientLogic();
    ~ClientLogic();

    // Funcția principală care pornește jocul
    //
    void run();

private:
    // Membri principali
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;

    SDL_Texture* m_freeCellTexture;
    SDL_Texture* m_breakableCellTexture;
    SDL_Texture* m_unbreakableCellTexture;
    SDL_Texture* m_playerTexture;

    int m_windowWidth;
    int m_windowHeight;

    ClientState m_state;

    int m_playerX, m_playerY;
    int m_mapWidth, m_mapHeight;
    std::vector<std::vector<CellTypeC>> m_map;

    // login
    std::string usernameInput;
    std::string passwordInput;
    bool enteringUsername;

    std::string m_serverUrl;

    ClientFunctions m_clientFunctions;


    bool m_isPairGame = false;
    
    void renderGameMap();

private:

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
    void drawButton(int x, int y, int w, int h, const std::string& text, SDL_Color color);
    bool isMouseInsideRect(int mouseX, int mouseY, int x, int y, int w, int h);


    // RENDER
    void renderMenu();
    void renderLogin();
    void renderRegister();
    void renderPairGame();
    // FETCH
    void fetchInitialMap();
    void sendMapUpdate(int x, int y);
    // HANDLE EVENTS
    void handleEventsMenu(const SDL_Event& e);
    void handleEventsLogin(const SDL_Event& e);
    void handleEventsRegister(const SDL_Event& e);


};
