#include "manager.h"
#include "gracz.h"

Points::Points() : punktyGracz1(0), punktyGracz2(0) {}

bool Points::czyGol(Kolo pilka)
{
    float pilkaX = pilka.polozenieX();
    if (pilkaX >= 1920.0f)
    {
        punktyGracz1 += 1;
        return true;
    }
    if (pilkaX <= 0.0f)
    {
        punktyGracz2 += 1;
        return true;
    }

    return false;
}

void Points::rysuj(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetRenderScale(renderer, 4.0f, 4.0f);
    SDL_RenderDebugTextFormat(renderer, 221.0f, 10.0f, "%d : %d", punktyGracz1, punktyGracz2);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

bool Points::koniecGry()
{
    if (punktyGracz1 >= 11 && punktyGracz1 - punktyGracz2 >= 2) return true;
    if (punktyGracz2 >= 11 && punktyGracz2 - punktyGracz1 >= 2) return true;

    return false;
}