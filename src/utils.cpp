#include <SDL3/SDL_audio.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <constants.h>
#include <audio.hpp>
#include <board.hpp>
#include <utils.hpp>
#include <cassert>
#include <cstdlib>
#include <string>
#include <unordered_map>

TextureType operator|(TextureType a, TextureType b) {
  return static_cast<TextureType>(static_cast<int>(a) | static_cast<int>(b));
}

void attachTextureToRenderer(SDL_Texture *texture, SDL_Renderer *renderer,
                             int x, int y) {
  if (!texture) {
    return;
  }
  SDL_FRect rect;
  rect.x = DISPLAY_SCALE * x;
  rect.y = DISPLAY_SCALE * y;
  rect.w = DISPLAY_SCALE * texture->w;
  rect.h = DISPLAY_SCALE * texture->h;
  SDL_RenderTexture(renderer, texture, NULL, &rect);
}

void loadTexturesToRenderer(
    std::unordered_map<TextureType, SDL_Texture *> *textures,
    SDL_Renderer *renderer) {
  (*textures)[BACKGROUND] = IMG_LoadTexture(renderer, BACKGROUND_PNG);
  (*textures)[MATRIX] = IMG_LoadTexture(renderer, MATRIX_PNG);
  (*textures)[DYING | I] = IMG_LoadTexture(renderer, DYING_PATH I_PNG);
  (*textures)[DYING | J] = IMG_LoadTexture(renderer, DYING_PATH J_PNG);
  (*textures)[DYING | L] = IMG_LoadTexture(renderer, DYING_PATH L_PNG);
  (*textures)[DYING | O] = IMG_LoadTexture(renderer, DYING_PATH O_PNG);
  (*textures)[DYING | S] = IMG_LoadTexture(renderer, DYING_PATH S_PNG);
  (*textures)[DYING | T] = IMG_LoadTexture(renderer, DYING_PATH T_PNG);
  (*textures)[DYING | Z] = IMG_LoadTexture(renderer, DYING_PATH Z_PNG);
  (*textures)[GHOST | I] = IMG_LoadTexture(renderer, GHOST_PATH I_PNG);
  (*textures)[GHOST | J] = IMG_LoadTexture(renderer, GHOST_PATH J_PNG);
  (*textures)[GHOST | L] = IMG_LoadTexture(renderer, GHOST_PATH L_PNG);
  (*textures)[GHOST | O] = IMG_LoadTexture(renderer, GHOST_PATH O_PNG);
  (*textures)[GHOST | S] = IMG_LoadTexture(renderer, GHOST_PATH S_PNG);
  (*textures)[GHOST | T] = IMG_LoadTexture(renderer, GHOST_PATH T_PNG);
  (*textures)[GHOST | Z] = IMG_LoadTexture(renderer, GHOST_PATH Z_PNG);
  (*textures)[LOCKED | I] = IMG_LoadTexture(renderer, LOCKED_PATH I_PNG);
  (*textures)[LOCKED | J] = IMG_LoadTexture(renderer, LOCKED_PATH J_PNG);
  (*textures)[LOCKED | L] = IMG_LoadTexture(renderer, LOCKED_PATH L_PNG);
  (*textures)[LOCKED | O] = IMG_LoadTexture(renderer, LOCKED_PATH O_PNG);
  (*textures)[LOCKED | S] = IMG_LoadTexture(renderer, LOCKED_PATH S_PNG);
  (*textures)[LOCKED | T] = IMG_LoadTexture(renderer, LOCKED_PATH T_PNG);
  (*textures)[LOCKED | Z] = IMG_LoadTexture(renderer, LOCKED_PATH Z_PNG);
  (*textures)[NORMAL | I] = IMG_LoadTexture(renderer, NORMAL_PATH I_PNG);
  (*textures)[NORMAL | J] = IMG_LoadTexture(renderer, NORMAL_PATH J_PNG);
  (*textures)[NORMAL | L] = IMG_LoadTexture(renderer, NORMAL_PATH L_PNG);
  (*textures)[NORMAL | O] = IMG_LoadTexture(renderer, NORMAL_PATH O_PNG);
  (*textures)[NORMAL | S] = IMG_LoadTexture(renderer, NORMAL_PATH S_PNG);
  (*textures)[NORMAL | T] = IMG_LoadTexture(renderer, NORMAL_PATH T_PNG);
  (*textures)[NORMAL | Z] = IMG_LoadTexture(renderer, NORMAL_PATH Z_PNG);
  (*textures)[WHOLE | I] = IMG_LoadTexture(renderer, WHOLE_PATH I_PNG);
  (*textures)[WHOLE | J] = IMG_LoadTexture(renderer, WHOLE_PATH J_PNG);
  (*textures)[WHOLE | L] = IMG_LoadTexture(renderer, WHOLE_PATH L_PNG);
  (*textures)[WHOLE | O] = IMG_LoadTexture(renderer, WHOLE_PATH O_PNG);
  (*textures)[WHOLE | S] = IMG_LoadTexture(renderer, WHOLE_PATH S_PNG);
  (*textures)[WHOLE | T] = IMG_LoadTexture(renderer, WHOLE_PATH T_PNG);
  (*textures)[WHOLE | Z] = IMG_LoadTexture(renderer, WHOLE_PATH Z_PNG);
  (*textures)[FREE] = NULL;
}

void loadAudios(std::unordered_map<std::string, Audio *> *audios,
                const SDL_AudioDeviceID &audioDevice) {
  std::string list[] = {
      AUDIO_BGM_1,        AUDIO_BGM_2,    AUDIO_BGM_3,      AUDIO_BACK_TO_BACK,
      AUDIO_BLOCKOUT,     AUDIO_COLLAPSE, AUDIO_HARD_DROP,  AUDIO_HOLD,
      AUDIO_INPUT_FAILED, AUDIO_LEVEL_UP, AUDIO_LINE_CLEAR, AUDIO_LOCK,
      AUDIO_MOVE,         AUDIO_ROTATE,   AUDIO_TETRIS,     AUDIO_WIN};
  for (std::string &file : list) {
    (*audios)[file] = new Audio(file, audioDevice);
  }
}

void loadFonts(std::unordered_map<std::string, TTF_Font *> *fonts) {
  (*fonts)["consolab 24"] = TTF_OpenFont(FONTS_PATH "consolab.ttf", 24);
  (*fonts)["consolab 20"] = TTF_OpenFont(FONTS_PATH "consolab.ttf", 20);
}

void loadLogo(SDL_Window *window) {
  SDL_Surface *logo = IMG_Load(LOGO_PNG);
  SDL_SetWindowIcon(window, logo);
  SDL_DestroySurface(logo);
}

void writeText(SDL_Renderer *renderer, std::string message, TTF_Font *font,
               Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h) {
  SDL_Color color = {r, g, b};
  SDL_Surface *surface = TTF_RenderText_Solid(font, message.c_str(), 0, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);
  int xx = x + (w - texture->w) / 2;
  int yy = y + (h - texture->h) / 2;
  attachTextureToRenderer(texture, renderer, xx, yy);
  SDL_DestroyTexture(texture);
}

std::string rjust(std::string s, int len, char pad) {
  const int d = len - s.length();
  assert(d >= 0);
  return std::string(d, pad) + s;
}

std::string convertMilisecToTimeString(Uint64 ms) {
  Uint64 m = ms / 60000;
  ms -= m * 60000;
  Uint64 s = ms / 1000;
  ms -= s * 1000;
  return rjust(std::to_string(m), 2, '0') + ':' +
         rjust(std::to_string(s), 2, '0') + '.' +
         rjust(std::to_string(ms), 3, '0');
}