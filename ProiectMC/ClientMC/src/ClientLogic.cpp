#include "../include/ClientLogic.h"
#include <iostream>
#include <stdexcept>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ClientLogic::ClientLogic()
    : m_window(nullptr), m_renderer(nullptr),
    m_freeCellTexture(nullptr), m_breakableCellTexture(nullptr),
    m_unbreakableCellTexture(nullptr), m_playerTexture(nullptr),
    m_windowWidth(800), m_windowHeight(600), m_state(ClientState::MENU),
    m_playerX(0), m_playerY(0), m_mapWidth(10), m_mapHeight(10) {
}

ClientLogic::~ClientLogic() {
    // Curățare resurse SDL
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window) SDL_DestroyWindow(m_window);
    if (m_freeCellTexture) SDL_DestroyTexture(m_freeCellTexture);
    if (m_breakableCellTexture) SDL_DestroyTexture(m_breakableCellTexture);
    if (m_unbreakableCellTexture) SDL_DestroyTexture(m_unbreakableCellTexture);
    if (m_playerTexture) SDL_DestroyTexture(m_playerTexture);
    IMG_Quit();
    SDL_Quit();
}

bool ClientLogic::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL nu s-a putut inițializa: " << SDL_GetError() << "\n";
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init nu a reușit: " << IMG_GetError() << "\n";
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init nu a reușit: " << TTF_GetError() << "\n";
        return false;
    }

    m_window = SDL_CreateWindow("Client Game", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, m_windowWidth,
        m_windowHeight, SDL_WINDOW_SHOWN);
    if (!m_window) {
        std::cerr << "Eroare creare fereastră: " << SDL_GetError() << "\n";
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        std::cerr << "Eroare creare renderer: " << SDL_GetError() << "\n";
        return false;
    }

    return true;
}

void ClientLogic::initializeTextures() {
    m_freeCellTexture = loadTexture("../images/grass.jpg",m_renderer);
    m_breakableCellTexture = loadTexture("../images/policeCar.png", m_renderer);
    m_unbreakableCellTexture = loadTexture("../images/brick.jpg", m_renderer);
    m_playerTexture = loadTexture("../images/cat.png", m_renderer);

    if (!m_freeCellTexture || !m_breakableCellTexture || !m_unbreakableCellTexture || !m_playerTexture) {
        std::cerr << "Eroare încărcare texturi.\n";
    }
}

//SDL_Texture* ClientLogic::loadTexture(const std::string& filePath) {
//    SDL_Surface* surface = IMG_Load(filePath.c_str());
//    if (!surface) {
//        std::cerr << "IMG_Load error: " << IMG_GetError() << "\n";
//        return nullptr;
//    }
//    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
//    SDL_FreeSurface(surface);
//    return texture;
//}
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


void ClientLogic::initializeMap() {
    m_map = std::vector<std::vector<CellType>>(m_mapHeight, std::vector<CellType>(m_mapWidth, CellType::FREE));
    for (int i = 0; i < m_mapHeight; ++i) {
        for (int j = 0; j < m_mapWidth; ++j) {
            if (i == 0 || i == m_mapHeight - 1 || j == 0 || j == m_mapWidth - 1) {
                m_map[i][j] = CellType::UNBREAKABLE;
            }
        }
    }
}

void ClientLogic::fetchGameState() {
    auto response = cpr::Get(cpr::Url{ "http://localhost:8080/game_state" });
    if (response.status_code == 200) {
        json gameState = json::parse(response.text);
        for (const auto& row : gameState["map"]) {
            // Actualizare logică hartă
        }
        std::cout << "Harta a fost actualizată.\n";
    }
    else {
        std::cerr << "Eroare obținere stare joc: " << response.status_code << "\n";
    }
}

void ClientLogic::handleEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_w: m_playerY--; break;
            case SDLK_s: m_playerY++; break;
            case SDLK_a: m_playerX--; break;
            case SDLK_d: m_playerX++; break;
            }
        }
    }
}

void ClientLogic::update() {
    // Logica jocului, actualizare stare
}

void ClientLogic::renderGame() {
    int tileSize = 50;
    for (int i = 0; i < m_mapHeight; ++i) {
        for (int j = 0; j < m_mapWidth; ++j) {
            SDL_Rect destRect{ j * tileSize, i * tileSize, tileSize, tileSize };
            SDL_Texture* texture = nullptr;

            switch (m_map[i][j]) {
            case CellType::FREE: texture = m_freeCellTexture; break;
            case CellType::BREAKABLE: texture = m_breakableCellTexture; break;
            case CellType::UNBREAKABLE: texture = m_unbreakableCellTexture; break;
            }
            if (texture) SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);
        }
    }

    SDL_Rect playerRect{ m_playerX * tileSize, m_playerY * tileSize, tileSize, tileSize };
    SDL_RenderCopy(m_renderer, m_playerTexture, nullptr, &playerRect);
}

void ClientLogic::render() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    renderGame();
    SDL_RenderPresent(m_renderer);
}

void ClientLogic::run() {
    if (!initSDL()) return;
    initializeTextures();
    initializeMap();

    bool running = true;
    while (running) {
        handleEvents(running);
        update();
        render();
        SDL_Delay(16);
    }
}
