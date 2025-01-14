#pragma once
/* 
	images, text
*/

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

class SDLHelper {
public:
    // Constructor and destructor
    SDLHelper(SDL_Renderer* renderer);
    ~SDLHelper();

    // Load a texture from a file
    SDL_Texture* loadTexture(const std::string& filePath);

    // Draw text on the screen
    void drawText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* font);

private:
    SDL_Renderer* m_renderer;
};
