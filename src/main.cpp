#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <algorithm>
#include "gracz.h"
#include "manager.h"
using std::endl;
using std::cerr;

struct Button
{
    SDL_FRect rect;
    SDL_Color baseColor;
    SDL_Color hoverColor;
    bool isHoverd = false;
};

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
    
    if (!renderer)
    {
        cerr << "Blad przy tworzeniu obiektu" << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    GameState state = GameState::Menu;
    bool graDziala = true;
    SDL_Event event;

    int r = 30, g = 120, b = 90;

    Gracz gracz1(80.0f, 465.0f, 40.0f, 150.0f, 10.5f, true);
    Gracz gracz2(1800.0f, 465.0f, 40.0f, 150.0f, 10.5f, false);
    Kolo pilka(950.0f, 540.0f, 15.0f);
    Manager manager;

    Button przyciskGraj 
    { 
        { (1920.0f - 300.0f) / 2.0f, (1080.0f - 80.0f) / 4.0f, 300.0f, 80.0f },
        { 255, 255, 255, 255 },
        { 200, 200, 200, 255 }
    };
    
    while(graDziala && state != GameState::Exit)
    {
        while(SDL_PollEvent(&event))
        {
            SDL_ConvertEventToRenderCoordinates(renderer, &event);

            if (event.type == SDL_EVENT_QUIT)
            {
                graDziala = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
            {
                graDziala = false;
            }

            if (state == GameState::Menu)
            {
                if (event.type == SDL_EVENT_MOUSE_MOTION)
                {
                    SDL_FPoint pozycjaMyszy = { event.motion.x, event.motion.y };
                    przyciskGraj.isHoverd = SDL_PointInRectFloat(&pozycjaMyszy, &przyciskGraj.rect);
                }

                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        SDL_FPoint click = {event.button.x, event.button.y};

                        if (SDL_PointInRectFloat(&click, &przyciskGraj.rect))
                        {
                            state = GameState::Playing;
                        }
                    }
                }
            }
        }

        if (state == GameState::Menu)
        {
            SDL_SetRenderDrawColor(renderer, 30, 120, 90, 255);
            SDL_RenderClear(renderer);

            SDL_Color kolor = przyciskGraj.isHoverd ? przyciskGraj.hoverColor : przyciskGraj.baseColor;
            SDL_SetRenderDrawColor(renderer, kolor.r, kolor.g, kolor.b, kolor.a);
            SDL_RenderFillRect(renderer, &przyciskGraj.rect);

            SDL_RenderPresent(renderer);
        }

        else if (state == GameState::Playing)
        {
            if (manager.czyGol(pilka))
            {
                if (manager.koniecGry()) state = GameState::Menu;
            
                if (manager.punktyGracz1 > manager.punktyGracz2)
                {
                    r = 30;
                    g = 120;
                    b = 90;
                }
                if (manager.punktyGracz1 < manager.punktyGracz2)
                {
                    r = 90;
                    g = 30;
                    b = 30;
                }
                pilka.resetuj();
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
            manager.rysuj(renderer);

            SDL_RenderPresent(renderer);
        }
        SDL_Delay(16);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}