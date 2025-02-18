#define SDL_MAIN_USE_CALLBACKS true  // use the callbacks instead of main()
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define SQUARE_BLOCK_SIZE_IN_PIXELS 40
#define SDL_WINDOW_WIDTH (SQUARE_BLOCK_SIZE_IN_PIXELS * SQUARE_GAME_WIDTH)
#define SDL_WINDOW_HEIGHT (SQUARE_BLOCK_SIZE_IN_PIXELS * SQUARE_GAME_HEIGHT)

#define SQUARE_GAME_WIDTH 10U
#define SQUARE_GAME_HEIGHT 20U
#define SQUARE_MATRIX_SIZE (SQUARE_GAME_WIDTH * SQUARE_GAME_HEIGHT)

struct AppState {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_APP_FAILURE;
  }
  AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
  if (!as) {
    return SDL_APP_FAILURE;
  }
  *appstate = as;
  if (!SDL_CreateWindowAndRenderer("Tetris game", SDL_WINDOW_WIDTH,
                                   SDL_WINDOW_HEIGHT, 0, &as->window,
                                   &as->renderer)) {
    return SDL_APP_FAILURE;
  }
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
  return SDL_APP_CONTINUE;
}

// This function runs once at shutdown.
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  // just free the memory, SDL will clean up the window/renderer for us.
  SDL_free(appstate);
}