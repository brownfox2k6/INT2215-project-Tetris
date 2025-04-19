#ifndef audio_hpp_
#define audio_hpp_

#include <SDL3/SDL.h>
#include <string>

class Audio {
private:
  Uint8 *wav_data;
  Uint32 wav_data_len;
  SDL_AudioStream *stream;
  const SDL_AudioDeviceID audioDevice;
public:
  Audio(std::string filePath, const SDL_AudioDeviceID &audio_device);
  ~Audio();
  void play();
  void pause();
  void resume();
  void clear();
  void playOverwrite();
};

#endif // audio_hpp_