#include "SDL3_ttf/SDL_ttf.h"
#include <string>
#include <unordered_map>
#define SDL_MAIN_USE_CALLBACKS // use the callbacks instead of main()
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <board.hpp>
#include <constants.h>
#include <random.hpp>
#include <utils.hpp>

SDL_Window *window;
SDL_Renderer *renderer;
std::unordered_map<TextureType, SDL_Texture *> textures;
std::unordered_map<std::string, TTF_Font *> fonts;
Board board;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH * DISPLAY_SCALE,
                              WINDOW_HEIGHT * DISPLAY_SCALE, 0, &window,
                              &renderer);
  loadTexturesToRenderer(textures, renderer);
  TTF_Init();
  loadFonts(fonts);
  loadLogo(window);
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
  attachTextureToRenderer(textures[BACKGROUND], renderer, BACKGROUND_POS_X,
                          BACKGROUND_POS_Y);
  attachTextureToRenderer(textures[MATRIX], renderer, MATRIX_POS_X,
                          MATRIX_POS_Y);
  board.nextState();
  board.attachToRenderer(textures, fonts, renderer);
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