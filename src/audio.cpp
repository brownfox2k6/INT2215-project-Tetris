#include "SDL3/SDL_audio.h"
#include "constants.h"
#include <audio.hpp>

Audio::Audio(std::string filePath, const SDL_AudioDeviceID &audio_device)
    : audioDevice(audio_device) {
  SDL_AudioSpec spec;
  if (!SDL_LoadWAV((AUDIOS_PATH + filePath).c_str(), &spec, &wav_data,
                   &wav_data_len)) {
    SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
    return;
  }
  stream = SDL_CreateAudioStream(&spec, NULL);
  if (!stream) {
    SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
  } else if (!SDL_BindAudioStream(audioDevice, stream)) {
    SDL_Log("Failed to bind stream to device: %s", SDL_GetError());
  }
}

Audio::~Audio() {
  SDL_DestroyAudioStream(stream);
  SDL_free(wav_data);
}

void Audio::play() {
  if (!SDL_GetAudioStreamAvailable(stream)) {
    SDL_PutAudioStreamData(stream, wav_data, int(wav_data_len));
  }
}

void Audio::pause() { SDL_PauseAudioDevice(audioDevice); }

void Audio::resume() { SDL_ResumeAudioDevice(audioDevice); }

void Audio::clear() { SDL_ClearAudioStream(stream); }

void Audio::playOverwrite() {
  SDL_ClearAudioStream(stream);
  SDL_PutAudioStreamData(stream, wav_data, int(wav_data_len));
}