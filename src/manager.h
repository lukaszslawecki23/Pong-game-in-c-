#pragma once
#include <SDL3/SDL.h>
#include "gracz.h"

class Manager
{
    public:
        int punktyGracz1;
        int punktyGracz2;

        Manager();
        bool czyGol(Kolo pilka);
        void rysuj(SDL_Renderer* renderer);
        bool koniecGry();
};

enum class GameState
{
    Menu,
    Playing,
    Exit
};