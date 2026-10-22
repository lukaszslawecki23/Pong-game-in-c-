#include "gracz.h"
#include <algorithm>
#include <random>

Gracz::Gracz(float start_x, float start_y, float szerokosc, float wysokosc, float predkosc, bool wsad)
{
    cialo = { start_x, start_y, szerokosc, wysokosc };
    predkoscGracza = predkosc;
    this->wsad = wsad;
}

void Gracz::aktualizuj()
{
    const bool* klawisze = SDL_GetKeyboardState(nullptr);

    if (wsad)
    {
        if (klawisze[SDL_SCANCODE_W]) cialo.y -= predkoscGracza;
        if (klawisze[SDL_SCANCODE_S]) cialo.y += predkoscGracza;
    }
    else
    {
        if (klawisze[SDL_SCANCODE_UP]) cialo.y -= predkoscGracza;
        if (klawisze[SDL_SCANCODE_DOWN]) cialo.y += predkoscGracza;
    }

    if (cialo.y <= 1.0f) cialo.y = 1.0f;
    if (cialo.y >= 929.0f) cialo.y = 929.0f;
    
}

void Gracz::rysuj(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &cialo);
}

Kolo::Kolo(float srodekX, float srodekY, float r)
{
    x = srodekX;
    y = srodekY;
    promien = r;
    startX = srodekX;
    startY = srodekY;
}

void Kolo::rysuj(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (float dy = -promien; dy <= promien; dy += 1.0f) 
    {
        for (float dx = -promien; dx <= promien; dx += 1.0f) 
        {
            if (dx * dx + dy * dy <= promien * promien) 
            {
                SDL_RenderPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void Kolo::czyKoliduje(const SDL_FRect& prostokat)
{
    float najblizszeX = clamp(x, prostokat.x, prostokat.x + prostokat.w);
    float najblizszeY = clamp(y, prostokat.y, prostokat.y + prostokat.h);

    float dystansX = x - najblizszeX;
    float dystansY = y - najblizszeY;

    float dystans = dystansX * dystansX + dystansY * dystansY;

    if (dystans < (promien * promien))
    {
        predkoscX *= -1.0f;
        
        if (x < prostokat.x + prostokat.w / 2) x = prostokat.x - promien - 1.0f;
        else x = prostokat.x + prostokat.w + promien + 1.0f;
    }
}

void Kolo::aktualizuj()
{
    const bool* klawisze = SDL_GetKeyboardState(nullptr);
    if (klawisze[SDL_SCANCODE_SPACE] && !wRuchu)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_real_distribution<float> dist(-2.0f, 2.0f);
        predkoscY = dist(gen);
        wRuchu = true;
    }

    if (wRuchu)
    {
        x += predkoscX;
        y += predkoscY;
        predkoscX *= 1.0003f;

        if (y - promien <= 0.0f)
        {
            y = promien;
            predkoscY *= -1.0f;
        }

        if (y + promien >= 1080.0f)
        {
            y = 1080.0f - promien;
            predkoscY *= -1.0f;
        }
    } 
}

void Kolo::resetuj() 
{
    x = startX;
    y = startY;
    wRuchu = false;
    
    if (predkoscX < 0.0f)
    {
        predkoscX = 9.0f;
    }
    else
    {
        predkoscX = -9.0f;
    }
}

float Kolo::polozenieX() 
{
    return x;
}