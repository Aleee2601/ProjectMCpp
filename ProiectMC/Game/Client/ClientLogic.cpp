#include "ClientLogic.h"
#include <iostream>
#include <filesystem>

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
    , m_windowWidth(1120)
    , m_windowHeight(520)
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
{
    // Nimic special aici
}

ClientLogic::~ClientLogic()
{
    if (m_font)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
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

        SDL_Delay(16);
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
    // În starea de joc, cerem periodic starea jocului de la server
    if (m_state == ClientState::GAME)
    {
        fetchGameState();
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
        // buton Login
        if (isMouseInsideRect(mouseX, mouseY, 100, 350, 150, 50))
        {
            bool ok = doLoginRequest(usernameInput, passwordInput);
            if (ok)
            {
                m_state = ClientState::GAME;
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
        else if (e.key.keysym.sym == SDLK_RETURN)
        {
            bool ok = doLoginRequest(usernameInput, passwordInput);
            if (ok) m_state = ClientState::GAME;
            else    std::cout << "[Client] Login fail.\n";
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
        case SDLK_SPACE:
            sendShootRequest();
            break;
        case SDLK_ESCAPE:
            m_state = ClientState::MENU;
            break;
        default:
            break;
        }
    }
}

void ClientLogic::renderGame() {
    int tileSize = 40;

    // Desenează harta
    for (int row = 0; row < m_mapHeight; ++row) {
        for (int col = 0; col < m_mapWidth; ++col) {
            SDL_Rect cellRect{ col * tileSize, row * tileSize, tileSize, tileSize };
            auto c = m_map[row][col];
            SDL_Texture* tex = nullptr;
            switch (c) {
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
            if (tex) {
                SDL_RenderCopy(m_renderer, tex, nullptr, &cellRect);
            }
        }
    }

    // Solicită jucătorii de la server
    auto r = cpr::Get(cpr::Url{ m_serverUrl + "/gameState" });
    if (r.status_code == 200) {
        auto j = json::parse(r.text);
        auto players = j["players"];

        // Desenează jucătorii
        for (const auto& p : players) {
            int x = p["x"];
            int y = p["y"];
            SDL_Rect playerRect{ x * tileSize, y * tileSize, tileSize, tileSize };
            SDL_RenderCopy(m_renderer, m_playerTexture, nullptr, &playerRect);
        }
    }
    else {
        std::cerr << "[Client] Eroare la obținerea jucătorilor de la server.\n";
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

bool ClientLogic::fetchGameState() {
    auto r = cpr::Get(cpr::Url{ m_serverUrl + "/gameState" });
    if (r.status_code == 200) {
        auto j = json::parse(r.text);

        // Actualizează dimensiunile hărții
        m_mapWidth = j["width"];
        m_mapHeight = j["height"];
        m_map = std::vector<std::vector<CellType>>(m_mapHeight,
            std::vector<CellType>(m_mapWidth, CellType::FREE));

        auto cells = j["cells"];
        for (int row = 0; row < m_mapHeight; ++row) {
            for (int col = 0; col < m_mapWidth; ++col) {
                std::string cellType = cells[row][col];
                if (cellType == "FREE")
                    m_map[row][col] = CellType::FREE;
                else if (cellType == "BREAKABLE")
                    m_map[row][col] = CellType::BREAKABLE;
                else if (cellType == "UNBREAKABLE")
                    m_map[row][col] = CellType::UNBREAKABLE;
            }
        }

        std::cout << "Map size: " << m_mapWidth << "x" << m_mapHeight << std::endl;
        return true;
    }
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

void ClientLogic::drawButton(int x, int y, int w, int h, const std::string& text, SDL_Color color)
{
    // Fundal
    SDL_Rect rect{ x, y, w, h };
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &rect);

    // Contur
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(m_renderer, &rect);

    // Text centrat
    SDL_Color textColor{ 0,0,0,255 };
    SDL_Surface* surf = TTF_RenderText_Solid(m_font, text.c_str(), textColor);
    if (!surf)
    {
        std::cerr << "[Client] Eroare TTF_RenderText_Solid: " << TTF_GetError() << "\n";
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
    int tw = surf->w;
    int th = surf->h;
    SDL_FreeSurface(surf);

    if (texture)
    {
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
