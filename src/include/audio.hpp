#ifndef audio_hpp_
#define audio_hpp_

#include "SDL3/SDL_audio.h"
#include <string>

class Audio {
private:
  Uint8 *wav_data;
  Uint32 wav_data_len;
  SDL_AudioStream *stream;

public:
  Audio(std::string filePath, const SDL_AudioDeviceID &audio_device);
  ~Audio();
  void play();
  void playOverwrite();
};

#endif // audio_hpp_