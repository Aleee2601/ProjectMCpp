#include "../include/ClientLogic.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using json = nlohmann::json;  // Ensure this alias is present


ClientLogic::ClientLogic()
    : m_isPairGame(false),
    m_networkManager("http://localhost:18080"), // Inițializare NetworkManager
    m_clientFunctions(m_networkManager),       // Pasarea referinței către ClientFunctions
    m_serverUrl("http://localhost:18080"),
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
        std::cerr << "[Client] Error: Font is not initialized. in drawText\n";
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

    #include <filesystem>
void ClientLogic::initFont() {
    m_font = TTF_OpenFont("PixelifySans.ttf", 24);
    if (!m_font) {
        std::cerr << "[Client] Error: Failed to load font: " << TTF_GetError() << "\n";
    }
    if (!std::filesystem::exists("PixelifySans.ttf")) {
        std::cerr << "Font file 'PixelifySans.ttf' not found in current working directory: "
            << std::filesystem::current_path() << std::endl;
    }

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
        std::cerr << "[Client] Error: Font is not initialized. in drawbutton\n";
        return;
    }

    // Render the text for the button
    SDL_Color textColor{ 0, 0, 0, 255 }; // Black text color
    SDL_Surface* surf = TTF_RenderText_Solid(m_font, text.c_str(), textColor);
    if (!surf) {
        std::cerr << "[Client] Error: TTF_RenderText_Solid: " << TTF_GetError() << "\n";
        return;
    }

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
    int tw = surf->w;
    int th = surf->h;
    SDL_FreeSurface(surf);

    // Center the text inside the button
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
    initFont();

    return true;
}

void ClientLogic::initializeTextures() {
    m_freeCellTexture = loadTexture("src/grass.jpg",m_renderer);
    m_breakableCellTexture = loadTexture("src/policeCar.png", m_renderer);
    m_unbreakableCellTexture = loadTexture("src/brick.jpg", m_renderer);
    m_playerTexture = loadTexture("src/catel2.png", m_renderer);

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
    try {
        m_clientFunctions.viewMapFunction(m_networkManager); // Pasarea NetworkManager
        std::cout << "Map initialized successfully.\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error initializing map: " << ex.what() << "\n";
    }
}

void ClientLogic::updateMapLogic(int x, int y, int newType) {
    try {
        m_clientFunctions.updateMapFunction(m_networkManager,x, y, newType); // Asigură-te că toate argumentele sunt furnizate
        std::cout << "Map updated successfully.\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error updating map: " << ex.what() << "\n";
    }
}





void ClientLogic::fetchGameState() {
    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/game_state" });
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
    static int frameCounter = 0;
    if (++frameCounter % 60 == 0) { // La fiecare 60 de cadre (~1 secundă)
        m_clientFunctions.updateMapFunction(m_networkManager, 5, 5, static_cast<int>(CellTypeC::DESTRUCTIBLE_WALL));
    }
}

void ClientLogic::renderGame() {
    int tileSize = 50;
    for (int i = 0; i < m_mapHeight; ++i) {
        for (int j = 0; j < m_mapWidth; ++j) {
            SDL_Rect destRect{ j * tileSize, i * tileSize, tileSize, tileSize };
            SDL_Texture* texture = nullptr;

            switch (m_map[i][j]) {
            case CellTypeC::EMPTY: texture = m_freeCellTexture; break;
            case CellTypeC::DESTRUCTIBLE_WALL: texture = m_breakableCellTexture; break;
            case CellTypeC::INDESTRUCTIBLE_WALL: texture = m_unbreakableCellTexture; break;
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
            case CellTypeC::EMPTY:
                texture = m_freeCellTexture;
                break;
            case CellTypeC::DESTRUCTIBLE_WALL:
                texture = m_breakableCellTexture;
                break;
            case CellTypeC::INDESTRUCTIBLE_WALL:
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


void ClientLogic::renderLobby() {
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer);

    // Logo Battle City
    drawText("Battle City", 300, 20, { 0, 0, 0, 255 });

    // Chenare jucători
    int xOffset = 50;
    int yOffset = 100;
    for (const auto& player : m_playersInLobby) {
        SDL_Rect playerRect = { xOffset, yOffset, 200, 100 };
        SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(m_renderer, &playerRect);
        drawText(player.name, xOffset + 10, yOffset + 10, { 0, 0, 0, 255 });

        // Imaginea jucătorului
        SDL_Texture* playerTexture = loadTexture(player.imagePath, m_renderer);
        SDL_Rect imgRect = { xOffset + 10, yOffset + 30, 50, 50 };
        SDL_RenderCopy(m_renderer, playerTexture, nullptr, &imgRect);
        SDL_DestroyTexture(playerTexture);

        yOffset += 120;
    }

    // Cronometru
    drawText("Time remaining: " + std::to_string(m_timeRemaining), 600, 50, { 255, 0, 0, 255 });

    SDL_RenderPresent(m_renderer);
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
    char option;
    while (running) {
        handleEvents(running);
        update(); 
        render();

        if (m_state == ClientState::LOBBY) {
            fetchLobbyDetails();
            renderLobby();
        }
        else {
            render();
        }

        SDL_Delay(16); // ~60 FPS

    }
    while (true) {
        std::cout << "1. Vezi lobby\n2. Start Game\nQ. Quit\n";
        std::cin >> option;
        if (option == '1') {
            m_clientFunctions.fetchLobby();
        }
        else if (option == '2') {
            m_clientFunctions.startGame();
        }
        else if (option == 'Q' || option == 'q') {
            break;
        }
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
        else if (isMouseInsideRect(mouseX, mouseY, centerX, topY + 5 * (buttonH + spacing), buttonW, buttonH)) {
            std::cout << "Entering lobby...\n";
            m_state = ClientState::LOBBY;
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
            m_map.resize(height, std::vector<CellTypeC>(width, CellTypeC::EMPTY));

            // Populăm harta cu datele primite
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    m_map[i][j] = static_cast<CellTypeC>(jsonResponse["map"][i][j].get<int>());
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


void ClientLogic::fetchLobbyDetails() {
    auto response = cpr::Get(cpr::Url{ m_serverUrl + "/lobby" });
    if (response.status_code == 200) {
        auto lobbyData = json::parse(response.text);
        m_playersInLobby.clear();
        for (const auto& player : lobbyData["players"]) {
            PlayerInfo info;
            info.id = player["id"].get<int>();
            info.name = player["name"].get<std::string>();
            info.imagePath = player["image"].get<std::string>();
            m_playersInLobby.push_back(info);
        }
        m_timeRemaining = lobbyData["timeRemaining"].get<int>();
    }
    else {
        std::cerr << "Failed to fetch lobby details: " << response.status_code << "\n";
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