#pragma once
#include <SDL3/SDL.h>
#include "gracz.h"

class Points
{
    public:
        int punktyGracz1;
        int punktyGracz2;

        Points();
        bool czyGol(Kolo pilka);
        void rysuj(SDL_Renderer* renderer);
        bool koniecGry();
};