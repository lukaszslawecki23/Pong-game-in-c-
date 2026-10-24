#pragma once
#include <SDL3/SDL.h>
#include <iostream>

struct Sound
{
    Uint8* bufor = nullptr;
    Uint32 dlugosc = 0;
    SDL_AudioSpec spec;
    SDL_AudioStream* strumien = nullptr;

    bool wczytaj(const char* sciezka);
    void otworz();
    void zniszcz();
};