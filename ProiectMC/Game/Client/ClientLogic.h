#pragma once

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <vector>
//#include "../ServerDemo/Player.h"

enum class ClientState
{
    MENU,
    LOGIN,
    REGISTER,
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

    void renderMenu();
    void renderLogin();
    void renderRegister();
    void renderGame();

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

    // Poziția locală a jucătorului (după fetch de la server)
    enum class CellType { FREE, BREAKABLE, UNBREAKABLE };
    std::vector<std::vector<CellType>> m_map;
    int m_mapWidth;
    int m_mapHeight;

    // Poziția locală a jucătorului
    int m_playerX;
    int m_playerY;
};
