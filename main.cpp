#define SDL_MAIN_HANDLED  // Или #undef main если conflict
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();  // TTF init
    
    SDL_Window* window = SDL_CreateWindow("Stage 2 - TTF", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Font* font = TTF_OpenFont("arial.ttf", 32);  // Шрифт в папке проекта!
    if (!font) {
        std::cerr << "Font error: " << TTF_GetError() << std::endl;
        return 1;
    }
    
    bool running = true;
    SDL_Event event;
    while (running) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) running = false;
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
        SDL_RenderClear(renderer);
        
        // Текст!
        std::string text = "Stage 2 SUCCESS! TrollEditor ready";
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surf = TTF_RenderUTF8_Solid(font, text.c_str(), white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        
        int w, h;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        SDL_Rect rect = {20, 20, w, h};
        SDL_RenderCopy(renderer, tex, NULL, &rect);
        SDL_DestroyTexture(tex);
        
        SDL_RenderPresent(renderer);
    }
    
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
