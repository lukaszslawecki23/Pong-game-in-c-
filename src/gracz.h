#pragma once
#include <SDL3/SDL.h>
#include <algorithm>
#include <cstdlib>
using std::clamp;

class Gracz
{
    private:
        SDL_FRect cialo;
        float predkoscGracza;
        bool wsad;
    
    public:
        Gracz(float start_x, float start_y, float szerokosc, float wysokosc, float predkosc, bool wsad);
        void aktualizuj();
        void rysuj(SDL_Renderer* renderer);
        const SDL_FRect& pobierzCialo() const { return cialo; }
};

class Kolo
{
    private:
        float x, y, promien, startX, startY;
        float predkoscX = 9.0f;
        float predkoscY = 5.0f;
        bool wRuchu = false;

    public:
        Kolo(float srodekX, float srodekY, float r);
        void rysuj(SDL_Renderer* renderer);
        void czyKoliduje(const SDL_FRect& prostokat);
        void aktualizuj();
        bool gol();
        void resetuj();
};