#include "gracz.h"
#include <algorithm>
#include <random>

Gracz::Gracz(float start_x, float start_y, float szerokosc, float wysokosc, bool wsad)
{
    cialo = { start_x, start_y, szerokosc, wysokosc };
    this->wsad = wsad;
}

void Gracz::aktualizuj()
{
    const bool* klawisze = SDL_GetKeyboardState(nullptr);
    float kierunekY = 0.0f;

    if (wsad)
    {
        if (klawisze[SDL_SCANCODE_W]) kierunekY -= 1.0f;
        if (klawisze[SDL_SCANCODE_S]) kierunekY += 1.0f;
    }
    else
    {
        if (klawisze[SDL_SCANCODE_UP]) kierunekY -= 1.0f;
        if (klawisze[SDL_SCANCODE_DOWN]) kierunekY += 1.0f;
    }

    if (kierunekY != 0.0f)
    {
        predkoscGracza += kierunekY * przyspieszenie;
        predkoscGracza = clamp(predkoscGracza, -maxPredkosc, maxPredkosc);
    }
    else
    {
        predkoscGracza *= tarcie;

        if (abs(predkoscGracza) < 0.05f)
        {
            predkoscGracza = 0.0f;
        }
    }

    cialo.y += predkoscGracza;

    if (cialo.y <= 1.0f)
    {
        cialo.y = 1.0f;
        predkoscGracza = 0.0f;
    }
    else if (cialo.y >= 929.0f)
    {
        cialo.y = 929.0f;
        predkoscGracza = 0.0f;
    }
    
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

void Kolo::czyKoliduje(const SDL_FRect& prostokat, Sound dzwiek)
{
    float najblizszeX = clamp(x, prostokat.x, prostokat.x + prostokat.w);
    float najblizszeY = clamp(y, prostokat.y, prostokat.y + prostokat.h);

    float dystansX = x - najblizszeX;
    float dystansY = y - najblizszeY;

    float dystans = dystansX * dystansX + dystansY * dystansY;

    if (dystans < (promien * promien))
    {
        dzwiek.otworz();
        predkoscX *= -1.0f;

        float srodek = prostokat.y + (prostokat.h / 2.0f);
        float offset = (y - srodek) / (prostokat.h / 2.0f);

        offset = clamp(offset, -1.0f, 1.0f);

        float maxSkos = 10.0f;
        predkoscY = offset * maxSkos;
        
        if (x < prostokat.x + prostokat.w / 2) x = prostokat.x - promien - 1.0f;
        else x = prostokat.x + prostokat.w + promien + 1.0f;
    }

}

void Kolo::aktualizuj(Sound dzwiek)
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
            dzwiek.otworz();
            y = promien;
            predkoscY *= -1.0f;
        }

        if (y + promien >= 1080.0f)
        {
            dzwiek.otworz();
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