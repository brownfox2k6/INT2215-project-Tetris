#define SDL_MAIN_USE_CALLBACKS // use the callbacks instead of main()
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <audio.hpp>
#include <board.hpp>
#include <constants.h>
#include <random.hpp>
#include <string>
#include <unordered_map>
#include <utils.hpp>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_AudioDeviceID audioDevice;
std::unordered_map<TextureType, SDL_Texture *> *textures;
std::unordered_map<std::string, TTF_Font *> *fonts;
std::unordered_map<std::string, Audio *> *audios;
Audio *bgm;
Board *board;
enum { INTRO, PLAYING, GAMEOVER, PAUSED, COUNTDOWN } gameState, lastGameState;
Timer countdownTimer;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  textures = new std::unordered_map<TextureType, SDL_Texture *>();
  fonts = new std::unordered_map<std::string, TTF_Font *>();
  audios = new std::unordered_map<std::string, Audio *>();
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                                   &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetWindowResizable(window, true);
  SDL_SetWindowAspectRatio(window, WINDOW_ASPECT_RATIO, WINDOW_ASPECT_RATIO);
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
  std::vector<std::string> listBgm = {AUDIO_BGM_1, AUDIO_BGM_2, AUDIO_BGM_3};
  bgm = audios->at(randElement(listBgm));
  board = new Board(renderer, textures, fonts, audios);
  return SDL_APP_CONTINUE;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs.
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  if (event->type == SDL_EVENT_WINDOW_RESIZED) {
    changeDisplayScale(1.0 * event->window.data1 / WINDOW_WIDTH);
    return SDL_APP_CONTINUE;
  }
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.scancode == SDL_SCANCODE_P) {
      if (gameState == INTRO || gameState == PAUSED || gameState == GAMEOVER) {
        if (gameState == GAMEOVER) {
          board->reset();
        }
        if (gameState != PAUSED) {
          lastGameState = gameState;
        }
        gameState = COUNTDOWN;
        countdownTimer.reset();
        countdownTimer.start();
      } else if (gameState == PLAYING) {
        lastGameState = PLAYING;
        gameState = PAUSED;
        board->timer.pause();
        bgm->pause();
      }
    }
  }
  return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program.
SDL_AppResult SDL_AppIterate(void *appstate) {
  attachTextureToRenderer(textures->at(BACKGROUND), renderer, BACKGROUND_POS_X,
                          BACKGROUND_POS_Y);
  if (gameState == INTRO) {
    writeText(renderer, "TETRIS GAME", fonts->at("consolab 40"), COLOR_YELLOW,
              0, 0, 800, 200);
    writeText(renderer, "Press 'P' to play", fonts->at("consolab 24"),
              COLOR_GREEN, 0, 200, 800, 100);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
  } else if (gameState == PLAYING) {
    attachTextureToRenderer(textures->at(MATRIX), renderer, MATRIX_POS_X,
                            MATRIX_POS_Y);
    board->nextState();
    board->attachToRenderer();
    bgm->play();
    if (board->isGameOver()) {
      gameState = GAMEOVER;
      bgm->clear();
      std::vector<std::string> listBgm = {AUDIO_BGM_1, AUDIO_BGM_2,
                                          AUDIO_BGM_3};
      bgm = audios->at(randElement(listBgm));
    }
  } else if (gameState == PAUSED) {
    writeText(renderer, "GAME PAUSED", fonts->at("consolab 40"), COLOR_YELLOW,
              0, 0, 800, 200);
    writeText(renderer, "Press 'P' to resume", fonts->at("consolab 24"),
              COLOR_GREEN, 0, 200, 800, 100);
  } else if (gameState == COUNTDOWN) {
    int countdownSeconds = 3 - countdownTimer.getTicks() / 1000;
    if (countdownSeconds < 0) {
      gameState = PLAYING;
      board->timer.resume();
      bgm->resume();
    } else {
      writeText(renderer, "The game is", fonts->at("consolab 32"), COLOR_YELLOW,
                0, 0, 800, 300);
      std::string t = (lastGameState == PLAYING) ? "resume" : "start";
      writeText(renderer, "about to " + t, fonts->at("consolab 32"),
                COLOR_YELLOW, 0, 40, 800, 300);
      writeText(renderer, std::to_string(countdownSeconds),
                fonts->at("consolab 100"), COLOR_WHITE, 0, 0, 800, 600);
    }
  } else if (gameState == GAMEOVER) {
    writeText(renderer, "GAME OVER", fonts->at("consolab 40"), COLOR_YELLOW, 0,
              0, 800, 200);
    writeText(renderer, "Press 'P' to restart", fonts->at("consolab 24"),
              COLOR_GREEN, 0, 200, 800, 100);
  }
  board->writeStats();
  SDL_RenderPresent(renderer);
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
