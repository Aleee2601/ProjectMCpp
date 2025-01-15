#pragma once
#define NOMINMAX
#include <windows.h>

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <vector>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <filesystem>

using json = nlohmann::json;


enum class ClientState
{
    MENU,
    LOGIN,
    REGISTER,
    GAME_SELECTION, // Noua stare
    GAME
};

class ClientLogic
{
public:
    ClientLogic(const std::string& serverUrl);
    ~ClientLogic();

    void run();

private:
    // Inițializări
    bool initSDL();
    void initializeTextures();

    // Bucla principală
    void handleEvents(bool& running);
    void update();
    void render();

    // Meniuri
    void handleEventsMenu(const SDL_Event& e);
    void handleEventsLogin(const SDL_Event& e);
    void handleEventsRegister(const SDL_Event& e);
    void handleEventsGame(const SDL_Event& e);
    void handleEventsGameSelection(const SDL_Event& e);

    void renderMenu();
    void renderLogin();
    void renderRegister();
    void renderGame();
    void renderGameSelection();

    // Funcții "buton"
    void drawText(const std::string& text, int x, int y, SDL_Color color); 
    void drawButton(int x, int y, int w, int h, const std::string& text, SDL_Color color);
    bool isMouseInsideRect(int mouseX, int mouseY, int x, int y, int w, int h);

    // Funcții request la server
    bool doRegisterRequest(const std::string& user, const std::string& pass);
    bool doLoginRequest(const std::string& user, const std::string& pass);
    bool doStartGameRequest();
    bool sendMoveRequest(int dx, int dy);
    bool sendShootRequest();
    bool fetchGameState();
    bool getGameInfo();
    bool fetchLobbyPlayers();
    bool fetchActiveGames();

    // Helper texturi
    SDL_Texture* loadTexture(const std::string& filePath, SDL_Renderer* renderer);

private:
    // Server
    std::string m_serverUrl;

    // SDL
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;

    int m_windowWidth;
    int m_windowHeight;

    // Stare
    ClientState m_state;

    // Input text
    std::string usernameInput;
    std::string passwordInput;
    bool enteringUsername;

    // Texturi
    SDL_Texture* m_freeCellTexture;
    SDL_Texture* m_breakableCellTexture;
    SDL_Texture* m_unbreakableCellTexture;
    SDL_Texture* m_playerTexture;

    // Harta locală, ca să afișăm
    enum class CellType { FREE, BREAKABLE, UNBREAKABLE };
    std::vector<std::vector<CellType>> m_map;
    int m_mapWidth;
    int m_mapHeight;

    // Poziția locală a jucătorului (după fetch de la server)
    int m_playerX;
    int m_playerY;

    int currentGameId; // ID-ul jocului curent alocat clientului
    int lobbyTimer; // Timp rămas pentru pornirea jocului
    std::vector<std::pair<int, std::vector<std::string>>> activeGames;

};