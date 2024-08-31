#include "audio.h"

#include "context.h"
#include "types.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

struct Sound {
    Mix_Chunk *chunk;
};

struct Audio::Pimpl {
    Map<String, Sound> sounds;
};

Audio::Audio() : pimpl(new Audio::Pimpl()) {
}

Audio::~Audio() = default;

void Audio::init() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    Mix_AllocateChannels(16);

    for (int i = 0; i < 16; i++) {
        Mix_Volume(i, 80);
    }
}

void Audio::loadSound(const String &name) {
    std::string path;
    path = "res/" + name + ".wav";

    Sound sound;

    sound.chunk = Mix_LoadWAV(path.c_str());

    pimpl->sounds[name] = sound;

    std::cout << "Loaded sound '" << name << "'" << std::endl;
}

void Audio::playSound(const String &name) {
    auto &sound = pimpl->sounds[name];

    auto channel = Mix_PlayChannel(-1, sound.chunk, 0);

    if (channel == -1) {
        channel = rand() % 16;
        Mix_PlayChannel(channel, sound.chunk, 0);
    }

    Mix_SetPanning(channel, 255, 255);
}

void Audio::playSound(const String &name, const Vector2 &position) {
    auto &renderer = Context::get().renderer;
    auto &sound = pimpl->sounds[name];

    auto channel = Mix_PlayChannel(-1, sound.chunk, 0);

    if (channel == -1) {
        channel = rand() % 16;
        Mix_PlayChannel(channel, sound.chunk, 0);
    }

    const auto &cam_pos = Context::get().cameraEntity->position;
    auto delta = position - cam_pos;

    float s = 1000;
    float f = (delta.x + s) / (renderer.width + 2 * s);
    f = std::min(std::max(0.0f, f), 1.0f);

    Mix_SetPanning(channel, int(255 * (1.0f - f)), int(255 * f));
}
