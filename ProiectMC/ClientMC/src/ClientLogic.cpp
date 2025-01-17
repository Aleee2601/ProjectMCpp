#include "../include/ClientLogic.h"
#include <iostream>
#include <stdexcept>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "../include/ClientFunctions.h"
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../ServerMC/include/Map.h"


using json = nlohmann::json;  // Ensure this alias is present

ClientLogic::ClientLogic()
    :m_isPairGame(false), 
    m_clientFunctions("http://localhost:8080"),
    m_serverUrl("http://localhost:8080"), /* other initializations */
    m_window(nullptr), m_renderer(nullptr),
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

bool ClientLogic::isMouseInsideRect(int mouseX, int mouseY, int x, int y, int w, int h)
{
    return !(mouseX < x || mouseX > x + w || mouseY < y || mouseY > y + h);
}

void ClientLogic::drawText(const std::string& text, int x, int y, SDL_Color color) {
    if (!m_font) {
        std::cerr << "[Client] Error: Font is not initialized.\n";
        return;
    }

    SDL_Surface* surf = TTF_RenderText_Solid(m_font, text.empty() ? " " : text.c_str(), color);
    if (!surf) {
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
    // Draw the button background
    SDL_Rect rect{ x, y, w, h };
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &rect);

    // Draw the button border
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(m_renderer, &rect);

    // Ensure the font is initialized
    if (!m_font) {
        std::cerr << "[Client] Error: Font is not initialized.\n";
        return;
    }

    // Ensure the button text is not empty
    if (text.empty()) {
        std::cerr << "[Client] Error: Button text is empty.\n";
        return;
    }

    // Render the text for the button
    SDL_Color textColor{ 0, 0, 0, 255 }; // Black text color
    SDL_Surface* surf = TTF_RenderText_Solid(m_font, text.c_str(), textColor);
    if (!surf) {
        std::cerr << "[Client] Eroare TTF_RenderText_Solid: " << TTF_GetError() << "\n";
        return;
    }

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
    int tw = surf->w;
    int th = surf->h;
    SDL_FreeSurface(surf); // Free the surface

    // Center the text inside the button
    if (texture) {
        SDL_Rect dst{
            x + (w - tw) / 2,
            y + (h - th) / 2,
            tw,
            th
        };
        SDL_RenderCopy(m_renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture); // Destroy the texture
    }
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

    m_font = TTF_OpenFont("E:/Facultate/Anul_2/Sem_1/MC/ProjectMCpp/ProiectMC/ClientMC/include/PixelifySans.ttf", 24); // Adjust font size as needed

    if (!m_font) {
        std::cerr << "Error loading font: " << TTF_GetError() << "\n";
    }

    return true;
}

void ClientLogic::initializeTextures() {
    m_freeCellTexture = loadTexture("E:/Facultate/Anul_2/Sem_1/MC/ProjectMCpp/ProiectMC/ClientMC/images/grass.jpg",m_renderer);
    m_breakableCellTexture = loadTexture("E:/Facultate/Anul_2/Sem_1/MC/ProjectMCpp/ProiectMC/ClientMC/images/policeCar.png", m_renderer);
    m_unbreakableCellTexture = loadTexture("E:/Facultate/Anul_2/Sem_1/MC/ProjectMCpp/ProiectMC/ClientMC/images/brick.jpg", m_renderer);
    m_playerTexture = loadTexture("E:/Facultate/Anul_2/Sem_1/MC/ProjectMCpp/ProiectMC/ClientMC/images/catel2.png", m_renderer);

    if (!m_freeCellTexture || !m_breakableCellTexture || !m_unbreakableCellTexture || !m_playerTexture) {
        std::cerr << "Eroare încărcare texturi.\n";
    }
}

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
   /* while (SDL_PollEvent(&e)) {
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
    }*/

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

void ClientLogic::renderGameMap() {
    if (m_map.empty()) {
        std::cerr << "Map data is empty. Cannot render the map.\n";
        return;
    }
    
    int tileSize = 32; // Dimensiunea fiecărei celule
    for (int i = 0; i < m_map.size(); ++i) {
        for (int j = 0; j < m_map[i].size(); ++j) {
            SDL_Rect destRect = { j * tileSize, i * tileSize, tileSize, tileSize };
            SDL_Texture* texture = nullptr;

            switch (m_map[i][j]) {
            case CellType::FREE:
                texture = m_freeCellTexture;
                break;
            case CellType::BREAKABLE:
                texture = m_breakableCellTexture;
                break;
            case CellType::UNBREAKABLE:
                texture = m_unbreakableCellTexture;
                break;
            default:
                continue;
            }

            if (texture) {
                SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);
            }
        }
    }
}

void ClientLogic::render() {
    SDL_SetRenderDrawColor(m_renderer, 255, 192, 203, 255);
    SDL_RenderClear(m_renderer);

    switch (m_state) {
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
    case ClientState::PAIR_GAME:
        renderPairGame(); 
        break;
    }

    SDL_RenderPresent(m_renderer);
}

void ClientLogic::run() {
    if (!initSDL()) return;
    initializeTextures();
    initializeMap();
    fetchInitialMap();
    bool running = true;
    while (running) {
        handleEvents(running);
        update();
        render();
        SDL_Delay(16);
    }
}

void ClientLogic::renderMenu() {
    int buttonW = 200;
    int buttonH = 60;
    int spacing = 20;

    int centerX = (m_windowWidth - buttonW) / 2;
    int totalHeight = 3 * buttonH + 2 * spacing;
    int topY = (m_windowHeight - totalHeight) / 2;

    drawButton(centerX, topY, buttonW, buttonH, "Login", { 200, 200, 200, 255 });
    drawButton(centerX, topY + buttonH + spacing, buttonW, buttonH, "Register", { 200, 200, 200, 255 });
    drawButton(centerX, topY + 2 * (buttonH + spacing), buttonW, buttonH, "Start Game", { 200, 200, 200, 255 });
    drawButton(centerX, topY + 3 * (buttonH + spacing), buttonW, buttonH, "Single Player", { 200, 200, 200, 255 });
    drawButton(centerX, topY + 4 * (buttonH + spacing), buttonW, buttonH, "Pair Game", { 200, 200, 200, 255 });
    drawButton(centerX, topY + 5 * (buttonH + spacing), buttonW, buttonH, "Show Map", { 200, 200, 200, 255 }); 

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

    // Check if usernameInput is not empty before rendering
    if (!usernameInput.empty())
    {
        drawText(usernameInput, 110, 160, { 0,0,0,255 });
    }

    // Mask the password with '*' and render only if passwordInput is not empty
    if (!passwordInput.empty())
    {
        std::string mask(passwordInput.size(), '*');
        drawText(mask, 110, 260, { 0,0,0,255 });
    }

    drawButton(100, 350, 150, 50, "Login", { 200,200,200,255 });
}


void ClientLogic::renderRegister()
{
    // Render Title
    drawText("Register Screen", 100, 50, { 255, 255, 255, 255 });

    // Render Labels
    drawText("Username:", 100, 120, { 255, 255, 255, 255 });
    drawText("Password:", 100, 220, { 255, 255, 255, 255 });

    // Input Boxes
    SDL_Rect userRect{ 100, 150, 200, 50 };
    SDL_Rect passRect{ 100, 250, 200, 50 };

    // Background for input boxes
    SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(m_renderer, &userRect);
    SDL_RenderFillRect(m_renderer, &passRect);

    // Border for input boxes
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(m_renderer, &userRect);
    SDL_RenderDrawRect(m_renderer, &passRect);

    // Render username if not empty
    if (!usernameInput.empty())
    {
        drawText(usernameInput, 110, 160, { 0, 0, 0, 255 });
    }

    // Render masked password if not empty
    if (!passwordInput.empty())
    {
        std::string mask(passwordInput.size(), '*');
        drawText(mask, 110, 260, { 0, 0, 0, 255 });
    }

    // Render Register Button
    drawButton(100, 350, 150, 50, "Register", { 200, 200, 200, 255 });
}

void ClientLogic::renderPairGame() {
    drawText("Pair Game Mode", 100, 50, { 255, 255, 255, 255 });
    drawText("Waiting for second player...", 100, 150, { 255, 255, 255, 255 });
}



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
        else if (isMouseInsideRect(mouseX, mouseY, centerX, topY, buttonW, buttonH)) {
            std::cout << "Single Player selected!\n";
            m_isPairGame = false; // Setam Single Player
            m_state = ClientState::GAME; // Trecem in starea de joc
        }
        // Buton Pair Game
        else if (isMouseInsideRect(mouseX, mouseY, centerX, topY + buttonH + spacing, buttonW, buttonH)) {
            std::cout << "Pair Game selected!\n";
            m_isPairGame = true; // Setam Pair Game
            m_state = ClientState::PAIR_GAME; // Trecem in starea Pair Game
        }
        else if (isMouseInsideRect(mouseX, mouseY, centerX, topY + 4 * (buttonH + spacing), buttonW, buttonH)) {
            std::cout << "Show Map selected!\n";
            m_state = ClientState::GAME; // Trecem la afișarea hărții
        }
       
        //// Buton 3: StartGame
        //else if (isMouseInsideRect(mouseX, mouseY, centerX, topY + 2 * (buttonH + spacing), buttonW, buttonH))
        //{
        //    //// Facem request la /startGame
        //    //if (doStartGameRequest())
        //    //{
        //    //    m_state = ClientState::GAME;
        //    //}
        //    //else
        //    //{
        //    //    std::cout << "[Client] StartGame fail.\n";
        //    //}
        //}
    }
}
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
            bool ok = m_clientFunctions.doLoginRequest(usernameInput, passwordInput);
            if (ok)
            {
                //std::cout << "Login ok!\n";
                m_state = ClientState::LOGIN; // după register, revii la meniu
                //m_state = ClientState::GAME;
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
            bool ok = m_clientFunctions.doLoginRequest(usernameInput, passwordInput);
            if (ok) m_state = ClientState::GAME;
            else    std::cout << "[Client] Login fail.\n";
        }
    }
}

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
            bool ok = m_clientFunctions.doRegisterRequest(usernameInput, passwordInput);
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
            bool ok = m_clientFunctions.doRegisterRequest(usernameInput, passwordInput);
            if (ok) m_state = ClientState::MENU;
            else    std::cout << "[Client] Register fail.\n";
        }
    }
}

void ClientLogic::fetchInitialMap() {
    auto response = cpr::Get(cpr::Url{ m_serverUrl + "/currentMap" });
    if (response.status_code == 200) {
        try {
            auto jsonResponse = json::parse(response.text);
            int height = jsonResponse["height"];
            int width = jsonResponse["width"];

            // Redimensionăm harta locală
            m_map.resize(height, std::vector<CellType>(width, CellType::FREE));

            // Populăm harta cu datele primite
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    m_map[i][j] = static_cast<CellType>(jsonResponse["map"][i][j].get<int>());
                }
            }

            std::cout << "Map fetched successfully from server.\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "Error parsing map data: " << ex.what() << "\n";
        }
    }
    else {
        std::cerr << "Failed to fetch map from server. Status code: " << response.status_code << "\n";
    }
}

void ClientLogic::sendMapUpdate(int x, int y) {
    try {
        // Creăm payload-ul JSON pentru cererea POST
        json data;
        data["x"] = x;
        data["y"] = y;

        // Trimitem cererea POST către server
        auto response = cpr::Post(
            cpr::Url{ m_serverUrl + "/updateMap" },
            cpr::Body{ data.dump() },
            cpr::Header{ { "Content-Type", "application/json" } }
        );

        // Verificăm răspunsul serverului
        if (response.status_code == 200) {
            std::cout << "Map update sent successfully.\n";
        }
        else {
            std::cerr << "Failed to update map on server. Status: " << response.status_code
                << ", Response: " << response.text << "\n";
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error while sending map update: " << ex.what() << "\n";
    }
}