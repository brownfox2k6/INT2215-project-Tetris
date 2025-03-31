#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "audio.hpp"
// #include "sdl2_mixer/SDL_mixer.h"
#include <string>
#include <unordered_map>
#define SDL_MAIN_USE_CALLBACKS // use the callbacks instead of main()
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <board.hpp>
#include <constants.h>
#include <iostream>
#include <random.hpp>
#include <utils.hpp>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_AudioDeviceID audioDevice;
std::unordered_map<TextureType, SDL_Texture *> *textures;
std::unordered_map<std::string, TTF_Font *> *fonts;
std::unordered_map<std::string, Audio *> *audios;
Board *board;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  textures = new std::unordered_map<TextureType, SDL_Texture *>();
  fonts = new std::unordered_map<std::string, TTF_Font *>();
  audios = new std::unordered_map<std::string, Audio *>();
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH * DISPLAY_SCALE,
                                   WINDOW_HEIGHT * DISPLAY_SCALE, 0, &window,
                                   &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
  if (audioDevice == 0) {
    SDL_Log("Couldn't open audio device: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  loadTexturesToRenderer(textures, renderer);
  TTF_Init();
  loadFonts(fonts);
  loadLogo(window);
  loadAudios(audios, audioDevice);
  board = new Board(renderer, textures, fonts, audios);
  return SDL_APP_CONTINUE;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs.
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program.
SDL_AppResult SDL_AppIterate(void *appstate) {
  attachTextureToRenderer(textures->at(BACKGROUND), renderer, BACKGROUND_POS_X,
                          BACKGROUND_POS_Y);
  attachTextureToRenderer(textures->at(MATRIX), renderer, MATRIX_POS_X,
                          MATRIX_POS_Y);
  board->nextState();
  board->attachToRenderer();
  SDL_RenderPresent(renderer);
  audios->at(AUDIO_BGM_2)->play();
  return SDL_APP_CONTINUE;
}

// This function runs once at shutdown.
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  for (auto &[_, txt] : *textures) {
    SDL_DestroyTexture(txt);
  }
  SDL_CloseAudioDevice(audioDevice);
}