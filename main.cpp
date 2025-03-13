#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"
#include <cstdint>
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
std::unordered_map<TextureType, SDL_Texture *> textures;
Board board;
SDL_Scancode holdingKey;
Uint64 timePrev;
bool isFirstPress, isReleased;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH * DISPLAY_SCALE,
                              WINDOW_HEIGHT * DISPLAY_SCALE, 0, &window,
                              &renderer);
  loadTexturesToRenderer(textures, renderer);
  loadLogo(window);
  isFirstPress = false;
  isReleased = true;
  timePrev = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs.
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN:
    holdingKey = event->key.scancode;
    break;
  case SDL_EVENT_KEY_UP:
    holdingKey = SDL_SCANCODE_UNKNOWN;
    isFirstPress = true;
    break;
  }
  return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program.
SDL_AppResult SDL_AppIterate(void *appstate) {
  attachTextureToRenderer(textures[BACKGROUND], renderer, BACKGROUND_POS_X,
                          BACKGROUND_POS_Y);
  attachTextureToRenderer(textures[MATRIX], renderer, MATRIX_POS_X,
                          MATRIX_POS_Y);
  board.nextState(holdingKey, isFirstPress);
  board.attachToRenderer(textures, renderer);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

// This function runs once at shutdown.
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  for (auto &[_, txt] : textures) {
    SDL_DestroyTexture(txt);
  }
}