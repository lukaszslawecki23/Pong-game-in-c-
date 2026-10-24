#include "sound.h"
#include <SDL3/SDL.h>
#include <iostream>
using std::cerr;
using std::endl;

bool Sound::wczytaj(const char* sciezka)
{
    {
        if (!SDL_LoadWAV(sciezka, &spec, &bufor, &dlugosc))
        {
            cerr << "Blad wczytania WAV: " << SDL_GetError() << endl;
            return false;
        }

        strumien = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
        if (!strumien)
        {
            cerr << "Blad tworzenia strumienia: " << SDL_GetError() << endl;
            SDL_free(bufor);
            bufor = nullptr;
            return false;
        }

        SDL_ResumeAudioStreamDevice(strumien);
        return true;
    }
}

void Sound::otworz()
{
    if (strumien && bufor)
    {
        SDL_PutAudioStreamData(strumien, bufor, dlugosc);
    }
}

void Sound::zniszcz()
{
    if (strumien)
    {
        SDL_DestroyAudioStream(strumien);
        strumien = nullptr;
    }

    if (bufor)
    {
        SDL_free(bufor);
        bufor = nullptr;
    }
}