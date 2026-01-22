#define SDL_MAIN_HANDLED  // Или #undef main если conflict
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <fstream> 

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { std::cerr << SDL_GetError(); return 1; }
    if (TTF_Init() < 0) { std::cerr << TTF_GetError(); return 1; }
    
    SDL_Window* window = SDL_CreateWindow("Stage 3 - Text Input", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 700, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Font* font = TTF_OpenFont("arial.ttf", 28);
    if (!font) { std::cerr << "Font: " << TTF_GetError(); return 1; }
    
    std::string textBuffer = "Начни печатать! Backspace работает";
    SDL_StartTextInput();  // Включаем SDL_TEXTINPUT
    
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            
            if (event.type == SDL_TEXTINPUT) {
                textBuffer += event.text.text;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_BACKSPACE:
                        if (!textBuffer.empty()) textBuffer.pop_back();
                        break;
                    case SDLK_RETURN:
                        std::cout << "Enter! Length: " << textBuffer.length() << std::endl;
                        break;
                    case SDLK_F2:
                        {
                            std::ofstream f("text.txt");
                            f << textBuffer;
                            std::cout << "Saved to text.txt" << std::endl;
                        }
                        break;
                    case SDLK_F3:
                        {
                            std::ifstream f("text.txt");
                            if (f) {
                                f.seekg(0, std::ios::end);
                                textBuffer.resize(f.tellg());
                                f.seekg(0);
                                f.read(&textBuffer[0], textBuffer.size());
                            }
                            std::cout << "Loaded!" << std::endl;
                        }
                        break;
                }
            }
        }
        
        // Рендер
        SDL_SetRenderDrawColor(renderer, 15, 15, 35, 255);
        SDL_RenderClear(renderer);
        
        // Основной текст
        SDL_Color white = {240, 240, 255, 255};
        SDL_Surface* surf = TTF_RenderUTF8_Blended_Wrapped(font, textBuffer.c_str(), white, 760);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        
        int w, h;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        SDL_Rect rect = {20, 50, w, h};
        SDL_RenderCopy(renderer, tex, NULL, &rect);
        SDL_DestroyTexture(tex);
        
        // Инструкции
        std::string instr = "F2=save F3=load | Backspace | Enter=trigger test | ESC=quit";
        SDL_Surface* instrSurf = TTF_RenderUTF8_Blended(font, instr.c_str(), white);
        SDL_Texture* instrTex = SDL_CreateTextureFromSurface(renderer, instrSurf);
        SDL_FreeSurface(instrSurf);
        SDL_QueryTexture(instrTex, NULL, NULL, &w, &h);
        SDL_Rect instrRect = {20, 550, w, h};
        SDL_RenderCopy(renderer, instrTex, NULL, &instrRect);
        SDL_DestroyTexture(instrTex);
        
        SDL_RenderPresent(renderer);
    }
    
    SDL_StopTextInput();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
