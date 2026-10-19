#include <SDL3/SDL.h>
#include <iostream>
#include <algorithm>
#include "gracz.h"
using std::endl;
using std::cerr;



int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        cerr << "Blad SDL_Init: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Moja gra",
        1920, 1080,
        0
    );

    if (!window) {
        cerr << "Blad tworzenia okna: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    if (!renderer)
    {
        cerr << "Blad przy tworzeniu obiektu" << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool graDziala = true;
    SDL_Event event;

    int r = 30, g = 120, b = 90;
    int temp_r = 90, temp_g = 30, temp_b = 30;

    Gracz gracz1(80.0f, 465.0f, 40.0f, 150.0f, 10.5f, true);
    Gracz gracz2(1800.0f, 465.0f, 40.0f, 150.0f, 10.5f, false);
    Kolo pilka(950.0f, 540.0f, 15.0f);

    while(graDziala)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                graDziala = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
            {
                graDziala = false;
            }
        }

        if (pilka.gol())
        {
            pilka.resetuj();
            std::swap(r, temp_r);
            std::swap(g, temp_g);
            std::swap(b, temp_b);
        }

        gracz1.aktualizuj();
        gracz2.aktualizuj();
        pilka.aktualizuj();

        pilka.czyKoliduje(gracz1.pobierzCialo());
        pilka.czyKoliduje(gracz2.pobierzCialo());

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderClear(renderer);

        gracz1.rysuj(renderer);
        gracz2.rysuj(renderer);
        pilka.rysuj(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}