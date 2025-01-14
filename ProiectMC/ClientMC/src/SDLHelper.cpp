#include "../include/SDLHelper.h"
#include <iostream>

SDLHelper::SDLHelper(SDL_Renderer* renderer)
    : m_renderer(renderer) {
}

SDLHelper::~SDLHelper() {
    // Destructor logic if needed
}

SDL_Texture* SDLHelper::loadTexture(const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << filePath << " - " << IMG_GetError() << "\n";
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << "\n";
    }

    return texture;
}

void SDLHelper::drawText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* font) {
    if (text.empty() || !font) {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << "\n";
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create texture from text surface: " << SDL_GetError() << "\n";
        return;
    }

    int textW = surface->w;
    int textH = surface->h;
    SDL_Rect destRect = { x, y, textW, textH };

    SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);
    SDL_DestroyTexture(texture);
}
