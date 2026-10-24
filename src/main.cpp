#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <algorithm>
#include "gracz.h"
#include "manager.h"
#include "sound.h"
using std::endl;
using std::cerr;

struct Button
{
    SDL_FRect rect;
    SDL_Texture *texture;
    SDL_Texture *textureHover;
    bool isHoverd = false;

    void aktualizuj(const SDL_FPoint& mysz)
    {
        isHoverd = SDL_PointInRectFloat(&mysz, &rect);
    }

    bool czyKlikniety(const SDL_FPoint& click)
    {
        return SDL_PointInRectFloat(&click, &rect);
    }

    void rysuj(SDL_Renderer* renderer) const
    {
        SDL_Texture *aktywna = (isHoverd && textureHover) ? textureHover : texture;
        if (aktywna)
        {
            SDL_RenderTexture(renderer, aktywna, NULL, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, isHoverd ? 200 : 255, 200, 200, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    void zniszcz()
    {
        if (texture) SDL_DestroyTexture(texture);
        if (textureHover) SDL_DestroyTexture(textureHover);
    }
};

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) 
    {
        cerr << "Blad SDL_Init: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Pong",
        1920, 1080,
        0
    );

    if (!window) 
    {
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
    SDL_Event event;

    SDL_Color tloGry = { 30, 120, 90, 255 };

    Gracz gracz1(80.0f, 465.0f, 40.0f, 150.0f, 10.5f, true);
    Gracz gracz2(1800.0f, 465.0f, 40.0f, 150.0f, 10.5f, false);
    Kolo pilka(950.0f, 540.0f, 15.0f);
    Manager manager;

    Button przyciskGraj 
    { 
        { (1920.0f - 300.0f) / 2.0f, (1080.0f - 80.0f) / 3.0f, 300.0f, 80.0f },
        IMG_LoadTexture(renderer, "texture/playButton.png"),
        IMG_LoadTexture(renderer, "texture/playButtonHover.png")
    };

    Button przyciskWyjdz
    {
        { (1920.0f - 300.0f) / 2.0f, (1080.0f - 80.0f) / 2.0f, 300.0f, 80.0f },
        IMG_LoadTexture(renderer, "texture/exitButton.png"),
        IMG_LoadTexture(renderer, "texture/exitButtonHover.png")
    };

    Sound dzwiek;
    dzwiek.wczytaj("texture/hitHurt.wav");
    
    while(state != GameState::Exit)
    {
        while(SDL_PollEvent(&event))
        {
            SDL_ConvertEventToRenderCoordinates(renderer, &event);

            if (event.type == SDL_EVENT_QUIT)
            {
                state = GameState::Exit;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
            {
                state = GameState::Menu;
            }

            if (state == GameState::Menu)
            {
                if (event.type == SDL_EVENT_MOUSE_MOTION)
                {
                    SDL_FPoint pozycjaMyszy = { event.motion.x, event.motion.y };
                    przyciskGraj.aktualizuj(pozycjaMyszy);
                    przyciskWyjdz.aktualizuj(pozycjaMyszy);
                }

                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        SDL_FPoint click = {event.button.x, event.button.y};

                        if (przyciskGraj.czyKlikniety(click))
                        {
                            state = GameState::Playing;
                        }

                        if (przyciskWyjdz.czyKlikniety(click))
                        {
                            state = GameState::Exit;
                        }
                    }
                }
            }
        }

        if (state == GameState::Menu)
        {
            SDL_SetRenderDrawColor(renderer, 30, 120, 90, 255);
            SDL_RenderClear(renderer);

            przyciskGraj.rysuj(renderer);
            przyciskWyjdz.rysuj(renderer);

            SDL_RenderPresent(renderer);
        }

        else if (state == GameState::Playing)
        {
            if (manager.czyGol(pilka))
            {
                if (manager.koniecGry()) 
                {
                    tloGry = { 30, 120, 90, 255 };

                    state = GameState::Menu;
                }
            
                if (manager.punktyGracz1 > manager.punktyGracz2)
                {
                    tloGry = { 30, 120, 90, 255 };
                }
                if (manager.punktyGracz1 < manager.punktyGracz2)
                {
                    tloGry = { 90, 30, 30, 255 };
                }
                pilka.resetuj();
            }

            gracz1.aktualizuj();
            gracz2.aktualizuj();
            pilka.aktualizuj(dzwiek);

            pilka.czyKoliduje(gracz1.pobierzCialo(), dzwiek);
            pilka.czyKoliduje(gracz2.pobierzCialo(), dzwiek);

            SDL_SetRenderDrawColor(renderer, tloGry.r, tloGry.g, tloGry.b, 255);
            SDL_RenderClear(renderer);

            gracz1.rysuj(renderer);
            gracz2.rysuj(renderer);
            pilka.rysuj(renderer);
            manager.rysuj(renderer);

            SDL_RenderPresent(renderer);
        }
        SDL_Delay(16);

    }

    przyciskGraj.zniszcz();
    przyciskWyjdz.zniszcz();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}