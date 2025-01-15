#include "ClientLogic.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#define NOMINMAX
#include <windows.h>

// cpr pentru request-uri HTTP
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::vector<std::pair<std::string, std::pair<int, int>>> allPlayers;

// ----------------------------------------------------------------------------
// Constructor / Destructor
// ----------------------------------------------------------------------------
ClientLogic::ClientLogic(const std::string& serverUrl)
    : m_serverUrl(serverUrl)
    , m_window(nullptr)
    , m_renderer(nullptr)
    , m_font(nullptr)
    , m_windowWidth(1280)
    , m_windowHeight(720)
    , m_state(ClientState::MENU)
    , enteringUsername(true)
    , m_freeCellTexture(nullptr)
    , m_breakableCellTexture(nullptr)
    , m_unbreakableCellTexture(nullptr)
    , m_playerTexture(nullptr)
    , m_mapWidth(0)
    , m_mapHeight(0)
    , m_playerX(-1)
    , m_playerY(-1)
    , currentGameId(-1)
    , lobbyTimer(30)
{
    // Inițializare generală
}

ClientLogic::~ClientLogic()
{
    // Eliberăm resursele SDL
    if (m_font)
        TTF_CloseFont(m_font);

    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);

    if (m_window)
        SDL_DestroyWindow(m_window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// ----------------------------------------------------------------------------
// Run
// ----------------------------------------------------------------------------
void ClientLogic::run()
{
    if (!initSDL())
    {
        std::cerr << "[Client] Eroare la initSDL!\n";
        return;
    }

    initializeTextures();

    bool running = true;
    while (running)
    {
        handleEvents(running);
        update();
        render();

        SDL_Delay(16); // 60 FPS
    }
}

// ----------------------------------------------------------------------------
// Inițializare SDL
// ----------------------------------------------------------------------------
    bool ClientLogic::initSDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "[Client] SDL nu s-a putut inițializa: " << SDL_GetError() << "\n";
            return false;
        }
        if (TTF_Init() == -1)
        {
            std::cerr << "[Client] TTF_Init Error: " << TTF_GetError() << "\n";
            return false;
        }
        if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
        {
            std::cerr << "[Client] SDL_Image init failed: " << IMG_GetError() << std::endl;
            return false;
        }

        m_window = SDL_CreateWindow(
            "Battle City Client",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_windowWidth,
            m_windowHeight,
            SDL_WINDOW_SHOWN
        );
        if (!m_window)
        {
            std::cerr << "[Client] Eroare creare fereastra: " << SDL_GetError() << "\n";
            return false;
        }

        m_renderer = SDL_CreateRenderer(
            m_window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
        if (!m_renderer)
        {
            std::cerr << "[Client] Eroare creare renderer: " << SDL_GetError() << "\n";
            return false;
        }

        m_font = TTF_OpenFont("PixelifySans.ttf", 24);
        if (!m_font)
        {
            std::cerr << "[Client] Eroare la încărcarea fontului: " << TTF_GetError() << "\n";
            return false;
        }

        return true;
    }


// ----------------------------------------------------------------------------
// Handle events
// ----------------------------------------------------------------------------
void ClientLogic::handleEvents(bool& running)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
            return;
        }

        switch (m_state)
        {
        case ClientState::MENU:
            handleEventsMenu(e);
            break;
        case ClientState::LOGIN:
            handleEventsLogin(e);
            break;
        /*case ClientState::GAME_SELECTION:
            handleEventsGameSelection(e);
            break;*/
        case ClientState::REGISTER:
            handleEventsRegister(e);
            break;
        case ClientState::GAME:
            handleEventsGame(e);
            break;
        }
    }
}

// ----------------------------------------------------------------------------
// Update 
// ----------------------------------------------------------------------------
void ClientLogic::update()
{
    if (m_state == ClientState::GAME_SELECTION)
    {
        static Uint32 lastFetch = SDL_GetTicks();
        Uint32 now = SDL_GetTicks();

        if (now - lastFetch >= 5000)
        {
            fetchLobbyPlayers();
            lastFetch = now;
        }
    }
}

// ----------------------------------------------------------------------------
// Render
// ----------------------------------------------------------------------------
void ClientLogic::render()
{
    // Curățăm ecranul
    SDL_SetRenderDrawColor(m_renderer, 255, 192, 203, 255);
    SDL_RenderClear(m_renderer);

    switch (m_state)
    {
    case ClientState::MENU:
        renderMenu();
        break;
    case ClientState::LOGIN:
        renderLogin();
        break;
    case ClientState::REGISTER:
        renderRegister();
        break;
   /* case ClientState::GAME_SELECTION:
        renderGameSelection();
        break;*/
    case ClientState::GAME:
        renderGame();
        break;
    }

    SDL_RenderPresent(m_renderer);
}

// ----------------------------------------------------------------------------
// Menu
// ----------------------------------------------------------------------------
void ClientLogic::handleEventsMenu(const SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        int buttonW = 200;
        int buttonH = 60;
        int spacing = 20;
        int centerX = (m_windowWidth - buttonW) / 2;
        int totalHeight = 3 * buttonH + 2 * spacing;
        int topY = (m_windowHeight - totalHeight) / 2;

        // Buton 1: Login
        if (isMouseInsideRect(mouseX, mouseY, centerX, topY, buttonW, buttonH))
        {
            m_state = ClientState::LOGIN;
        }
        // Buton 2: Register
        else if (isMouseInsideRect(mouseX, mouseY, centerX, topY + buttonH + spacing, buttonW, buttonH))
        {
            m_state = ClientState::REGISTER;
        }
        // Buton 3: StartGame
        else if (isMouseInsideRect(mouseX, mouseY, centerX, topY + 2 * (buttonH + spacing), buttonW, buttonH))
        {
            // Facem request la /startGame
            if (doStartGameRequest())
            {
                m_state = ClientState::GAME;
            }
            else
            {
                std::cout << "[Client] StartGame fail.\n";
            }
        }
    }
}

void ClientLogic::renderMenu()
{
    int buttonW = 200;
    int buttonH = 60;
    int spacing = 20;

    int centerX = (m_windowWidth - buttonW) / 2;
    int totalHeight = 3 * buttonH + 2 * spacing;
    int topY = (m_windowHeight - totalHeight) / 2;

    drawButton(centerX, topY, buttonW, buttonH, "Login", { 200,200,200,255 });
    drawButton(centerX, topY + buttonH + spacing, buttonW, buttonH, "Register", { 200,200,200,255 });
    drawButton(centerX, topY + 2 * (buttonH + spacing), buttonW, buttonH, "Start Game", { 200,200,200,255 });
}

// ----------------------------------------------------------------------------
// Login
// ----------------------------------------------------------------------------
void ClientLogic::handleEventsLogin(const SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // Caseta user
        if (isMouseInsideRect(mouseX, mouseY, 100, 150, 200, 50))
        {
            enteringUsername = true;
        }
        // Caseta pass
        else if (isMouseInsideRect(mouseX, mouseY, 100, 250, 200, 50))
        {
            enteringUsername = false;
        }
        // Buton Login
        if (isMouseInsideRect(mouseX, mouseY, 100, 350, 150, 50))
        {
            bool ok = doLoginRequest(usernameInput, passwordInput);
            if (ok)
            {
                m_state = ClientState::GAME; // Intră în joc
            }
            else
            {
                std::cout << "[Client] Login fail.\n";
            }
        }
    }
    else if (e.type == SDL_TEXTINPUT)
    {
        if (enteringUsername)
            usernameInput += e.text.text;
        else
            passwordInput += e.text.text;
    }
    else if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_BACKSPACE)
        {
            if (enteringUsername && !usernameInput.empty())
                usernameInput.pop_back();
            else if (!enteringUsername && !passwordInput.empty())
                passwordInput.pop_back();
        }
    }
}

void ClientLogic::renderLogin()
{
    drawText("Login Screen", 100, 50, { 255,255,255,255 });

    drawText("Username:", 100, 120, { 255,255,255,255 });
    drawText("Password:", 100, 220, { 255,255,255,255 });

    // Casete
    SDL_Rect userRect{ 100, 150, 200, 50 };
    SDL_Rect passRect{ 100, 250, 200, 50 };

    // Fundal
    SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(m_renderer, &userRect);
    SDL_RenderFillRect(m_renderer, &passRect);

    // Contur
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(m_renderer, &userRect);
    SDL_RenderDrawRect(m_renderer, &passRect);

    drawText(usernameInput, 110, 160, { 0,0,0,255 });
    std::string mask(passwordInput.size(), '*');
    drawText(mask, 110, 260, { 0,0,0,255 });

    drawButton(100, 350, 150, 50, "Login", { 200,200,200,255 });
}

// ----------------------------------------------------------------------------
// Register
// ----------------------------------------------------------------------------
void ClientLogic::handleEventsRegister(const SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // caseta user
        if (isMouseInsideRect(mouseX, mouseY, 100, 150, 200, 50))
        {
            enteringUsername = true;
        }
        // caseta pass
        else if (isMouseInsideRect(mouseX, mouseY, 100, 250, 200, 50))
        {
            enteringUsername = false;
        }
        // buton register
        if (isMouseInsideRect(mouseX, mouseY, 100, 350, 150, 50))
        {
            bool ok = doRegisterRequest(usernameInput, passwordInput);
            if (ok)
            {
                m_state = ClientState::MENU; // după register, revii la meniu
            }
            else
            {
                std::cout << "[Client] Register fail.\n";
            }
        }
    }
    else if (e.type == SDL_TEXTINPUT)
    {
        if (enteringUsername)
            usernameInput += e.text.text;
        else
            passwordInput += e.text.text;
    }
    else if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_BACKSPACE)
        {
            if (enteringUsername && !usernameInput.empty())
                usernameInput.pop_back();
            else if (!enteringUsername && !passwordInput.empty())
                passwordInput.pop_back();
        }
        else if (e.key.keysym.sym == SDLK_RETURN)
        {
            bool ok = doRegisterRequest(usernameInput, passwordInput);
            if (ok) m_state = ClientState::MENU;
            else    std::cout << "[Client] Register fail.\n";
        }
    }
}

void ClientLogic::renderRegister()
{
    drawText("Register Screen", 100, 50, { 255,255,255,255 });

    drawText("Username:", 100, 120, { 255,255,255,255 });
    drawText("Password:", 100, 220, { 255,255,255,255 });

    SDL_Rect userRect{ 100, 150, 200, 50 };
    SDL_Rect passRect{ 100, 250, 200, 50 };

    SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(m_renderer, &userRect);
    SDL_RenderFillRect(m_renderer, &passRect);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(m_renderer, &userRect);
    SDL_RenderDrawRect(m_renderer, &passRect);

    drawText(usernameInput, 110, 160, { 0,0,0,255 });
    std::string mask(passwordInput.size(), '*');
    drawText(mask, 110, 260, { 0,0,0,255 });

    drawButton(100, 350, 150, 50, "Register", { 200,200,200,255 });
}

// ----------------------------------------------------------------------------
// Game
// ----------------------------------------------------------------------------
void ClientLogic::handleEventsGame(const SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_w: sendMoveRequest(0, -1); break;
        case SDLK_s: sendMoveRequest(0, 1);  break;
        case SDLK_a: sendMoveRequest(-1, 0); break;
        case SDLK_d: sendMoveRequest(1, 0);  break;
        case SDLK_ESCAPE:
            m_state = ClientState::MENU;
            break;
        default:
            break;
        }
    }
}

void ClientLogic::renderGame()
{
    std::cout << "[Debug] Redesenăm harta. Dimensiuni: " << m_mapWidth << "x" << m_mapHeight << "\n";

    int tileSize = 40;
    for (int row = 0; row < m_mapHeight; row++)
    {
        for (int col = 0; col < m_mapWidth; col++)
        {
            SDL_Rect cellRect{ col * tileSize, row * tileSize, tileSize, tileSize };
            auto c = m_map[row][col];
            SDL_Texture* tex = nullptr;
            switch (c)
            {
            case CellType::FREE:
                tex = m_freeCellTexture;
                break;
            case CellType::BREAKABLE:
                tex = m_breakableCellTexture;
                break;
            case CellType::UNBREAKABLE:
                tex = m_unbreakableCellTexture;
                break;
            }
            if (tex)
            {
                SDL_RenderCopy(m_renderer, tex, nullptr, &cellRect);
            }
        }
    }

    // Debug pentru jucători
    std::cout << "[Debug] Lista jucătorilor:\n";
    for (const auto& player : allPlayers)
    {
        std::cout << " - " << player.first << " la (" << player.second.first << ", " << player.second.second << ")\n";
    }
}




void ClientLogic::renderGameSelection() {
    drawText("Active Games Lobby", 100, 50, { 255, 255, 255, 255 });

    int buttonW = 300;
    int buttonH = 60;
    int spacing = 20;
    int startY = 100;

    // Display each game's details
    for (size_t i = 0; i < activeGames.size(); ++i) {
        int x = (m_windowWidth - buttonW) / 2;
        int y = startY + i * (buttonH + spacing);

        std::string buttonText = "Game " + std::to_string(activeGames[i].first) +
            " (" + std::to_string(activeGames[i].second.size()) + " players)";
        drawButton(x, y, buttonW, buttonH, buttonText, { 200, 200, 200, 255 });
    }

    // Display the lobby for the first game
    if (!activeGames.empty() && !activeGames[0].second.empty()) {
        int lobbyX = 50;
        int lobbyY = startY + activeGames.size() * (buttonH + spacing) + 40;
        int lobbyWidth = 400;
        int lobbyHeight = 300;

        SDL_Rect lobbyRect{ lobbyX, lobbyY, lobbyWidth, lobbyHeight };
        SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(m_renderer, &lobbyRect);
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(m_renderer, &lobbyRect);

        // Show up to 4 players in the lobby
        size_t maxPlayersToShow = std::min(static_cast<size_t>(4), activeGames[0].second.size());
        int playerY = lobbyY + 20;

        for (size_t i = 0; i < maxPlayersToShow; ++i) {
            drawText(activeGames[0].second[i], lobbyX + 20, playerY, { 255, 255, 255, 255 });
            playerY += 40;
        }

        // Display the countdown timer
        drawText("Time to start: " + std::to_string(lobbyTimer) + "s", lobbyX + 20, playerY + 40, { 255, 255, 255, 255 });
    } else {
        // Show a message if no players are connected
        drawText("No players connected yet.", 100, startY + 200, { 255, 0, 0, 255 });
    }
}

// ----------------------------------------------------------------------------
// Funcții request la server (cpr)
// ----------------------------------------------------------------------------
bool ClientLogic::doRegisterRequest(const std::string& user, const std::string& pass)
{
    // POST /register
    json j;
    j["username"] = user;
    j["password"] = pass;

    auto r = cpr::Post(cpr::Url{ m_serverUrl + "/register" },
        cpr::Body{ j.dump() },
        cpr::Header{ {"Content-Type", "application/json"} });
    return (r.status_code == 200);
}

bool ClientLogic::doLoginRequest(const std::string& user, const std::string& pass)
{
    // POST /login
    json j;
    j["username"] = user;
    j["password"] = pass;

    auto r = cpr::Post(cpr::Url{ m_serverUrl + "/login" },
        cpr::Body{ j.dump() },
        cpr::Header{ {"Content-Type", "application/json"} });
    return (r.status_code == 200);
}

bool ClientLogic::doStartGameRequest()
{
    auto r = cpr::Post(cpr::Url{ m_serverUrl + "/startGame" });
    return (r.status_code == 200);
}
bool ClientLogic::sendMoveRequest(int dx, int dy)
{
    // POST /move
    json j;
    j["username"] = usernameInput;
    j["dx"] = dx;
    j["dy"] = dy;
    auto r = cpr::Post(cpr::Url{ m_serverUrl + "/move" },
        cpr::Body{ j.dump() },
        cpr::Header{ {"Content-Type", "application/json"} });
    return (r.status_code == 200);
}

bool ClientLogic::sendShootRequest()
{
    // POST /shoot
    json j;
    j["username"] = usernameInput;
    auto r = cpr::Post(cpr::Url{ m_serverUrl + "/shoot" },
        cpr::Body{ j.dump() },
        cpr::Header{ {"Content-Type", "application/json"} });
    return (r.status_code == 200);
}

bool ClientLogic::fetchGameState()
{
    auto r = cpr::Get(cpr::Url{ m_serverUrl + "/gameState" },
        cpr::Parameters{ {"gameId", std::to_string(currentGameId)} });

    if (r.status_code == 200)
    {
        auto j = json::parse(r.text);
        std::cout << "[Debug] Răspuns gameState: " << r.text << "\n";

        // Actualizăm dimensiunile hărții
        m_mapWidth = j["width"];
        m_mapHeight = j["height"];

        if (m_mapWidth == 0 || m_mapHeight == 0)
        {
            std::cerr << "[Error] Dimensiunile hărții sunt 0.\n";
            return false;
        }

        // Reconstruim harta
        m_map = std::vector<std::vector<CellType>>(m_mapHeight, std::vector<CellType>(m_mapWidth, CellType::FREE));
        auto cells = j["cells"];
        for (int row = 0; row < m_mapHeight; ++row)
        {
            for (int col = 0; col < m_mapWidth; ++col)
            {
                std::string cellType = cells[row][col];
                if (cellType == "FREE")
                    m_map[row][col] = CellType::FREE;
                else if (cellType == "BREAKABLE")
                    m_map[row][col] = CellType::BREAKABLE;
                else if (cellType == "UNBREAKABLE")
                    m_map[row][col] = CellType::UNBREAKABLE;
            }
        }

        return true;
    }

    std::cerr << "[Client] Eroare la obținerea gameState: " << r.status_code << "\n";
    return false;
}


bool ClientLogic::fetchActiveGames() {
    auto r = cpr::Get(cpr::Url{ m_serverUrl + "/activeGames" });

    if (r.status_code == 200) {
        auto j = json::parse(r.text);
        activeGames.clear();

        for (const auto& game : j["games"]) {
            int gameId = game["gameId"];
            std::vector<std::string> players;

            for (const auto& player : game["players"]) {
                players.push_back(player.get<std::string>());
            }

            activeGames.emplace_back(gameId, players);
        }

        if (!activeGames.empty()) {
            currentGameId = activeGames[0].first; // Selectăm primul joc
            std::cout << "[Client] Joc activ selectat: " << currentGameId << std::endl;
        }
        else {
            std::cerr << "[Client] Niciun joc activ găsit.\n";
        }

        return true;
    }

    std::cerr << "[Client] Eroare la obținerea jocurilor active.\n";
    return false;
}




// ----------------------------------------------------------------------------
// Funcții de UI
// ----------------------------------------------------------------------------
SDL_Texture* ClientLogic::loadTexture(const std::string& filePath, SDL_Renderer* renderer)
{
    if (!std::filesystem::exists(filePath))
    {
        std::cerr << "[Client] Fișierul imagine nu există: " << filePath << "\n";
        return nullptr;
    }
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface)
    {
        std::cerr << "[Client] Eroare la IMG_Load: " << IMG_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex)
    {
        std::cerr << "[Client] Eroare la SDL_CreateTextureFromSurface: " << SDL_GetError() << "\n";
    }
    return tex;
}

void ClientLogic::initializeTextures()
{
    m_freeCellTexture = loadTexture("../images/grass.jpg", m_renderer);
    m_breakableCellTexture = loadTexture("../images/policeCar.png", m_renderer);
    m_unbreakableCellTexture = loadTexture("../images/brick.jpg", m_renderer);
    m_playerTexture = loadTexture("../images/cat.png", m_renderer);
}

void ClientLogic::drawText(const std::string& text, int x, int y, SDL_Color color)
{
    if (text.empty()) return;
    SDL_Surface* surf = TTF_RenderText_Solid(m_font, text.c_str(), color);
    if (!surf)
    {
        std::cerr << "[Client] Eroare TTF_RenderText_Solid: " << TTF_GetError() << "\n";
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
    int w = surf->w;
    int h = surf->h;
    SDL_FreeSurface(surf);
    SDL_Rect dst{ x, y, w, h };
    SDL_RenderCopy(m_renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

void ClientLogic::drawButton(int x, int y, int w, int h, const std::string& text, SDL_Color color) {
    // Fundal
    SDL_Rect rect{ x, y, w, h };
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &rect);

    // Contur
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(m_renderer, &rect);

    // Text centrat
    SDL_Color textColor{ 0, 0, 0, 255 };
    SDL_Surface* surf = TTF_RenderText_Solid(m_font, text.c_str(), textColor);
    if (!surf) {
        std::cerr << "[Client] Eroare TTF_RenderText_Solid: " << TTF_GetError() << "\n";
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
    int tw = surf->w;
    int th = surf->h;
    SDL_FreeSurface(surf);

    if (texture) {
        SDL_Rect dst{
            x + (w - tw) / 2,
            y + (h - th) / 2,
            tw,
            th
        };
        SDL_RenderCopy(m_renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
}


bool ClientLogic::isMouseInsideRect(int mouseX, int mouseY, int x, int y, int w, int h)
{
    return !(mouseX < x || mouseX > x + w || mouseY < y || mouseY > y + h);
}


bool ClientLogic::getGameInfo() {
    auto r = cpr::Get(cpr::Url{ m_serverUrl + "/gameInfo" },
        cpr::Parameters{ {"username", usernameInput} });

    if (r.status_code == 200) {
        auto j = json::parse(r.text);
        if (j.contains("gameId")) {
            currentGameId = j["gameId"].get<int>();
            std::cout << "[Client] Alocat în jocul: " << currentGameId << std::endl;
            return true;
        }
    }

    std::cerr << "[Client] Eroare la obținerea informațiilor despre joc." << std::endl;
    return false;
}

bool ClientLogic::fetchLobbyPlayers() {
    if (currentGameId == -1) {
        std::cerr << "[Client] currentGameId este invalid (-1).\n";
        return false;
    }

    auto r = cpr::Get(cpr::Url{ m_serverUrl + "/lobbyPlayers" },
        cpr::Parameters{ {"gameId", std::to_string(currentGameId)} });

    if (r.status_code == 200) {
        auto j = json::parse(r.text);
        allPlayers.clear();

        for (const auto& player : j["players"].items()) {
            std::string username = player.key();
            int x = player.value()["x"];
            int y = player.value()["y"];
            allPlayers.emplace_back(username, std::make_pair(x, y));
        }

        return true;
    }

    std::cerr << "[Client] Eroare la obținerea jucătorilor din lobby.\n";
    return false;
}

void ClientLogic::handleEventsGameSelection(const SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        int buttonW = 300;
        int buttonH = 60;
        int spacing = 20;
        int startY = 100;

        for (size_t i = 0; i < activeGames.size(); ++i) {
            int x = (m_windowWidth - buttonW) / 2;
            int y = startY + i * (buttonH + spacing);

            if (isMouseInsideRect(mouseX, mouseY, x, y, buttonW, buttonH)) {
                currentGameId = activeGames[i].first;
                std::cout << "[Client] Joc selectat: " << currentGameId << std::endl;
                m_state = ClientState::GAME; // Intră în joc
                break;
            }
        }
    }
}
