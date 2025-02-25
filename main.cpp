#define SDL_MAIN_USE_CALLBACKS  // use the callbacks instead of main()
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <constants.h>
#include <utils.hpp>

SDL_Window *window;
SDL_Renderer *renderer;
std::unordered_map<std::string, SDL_Texture*> texture;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH * RATIO,
                              WINDOW_HEIGHT * RATIO, 0, &window,
                              &renderer);
  loadTexturesToRenderer(texture, renderer);
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
  attachTextureToRendererFilled(texture["background"], renderer);
  attachTextureToRendererCentered(texture["matrix"], renderer);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

// This function runs once at shutdown.
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  for (auto &[_, txt] : texture) {
    SDL_DestroyTexture(txt);
  }
}