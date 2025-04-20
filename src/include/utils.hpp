#ifndef utils_hpp_
#define utils_hpp_

#include "SDL3_ttf/SDL_ttf.h"
#include <SDL3/SDL.h>
#include <audio.hpp>
#include <string>
#include <unordered_map>

struct Coordinate {
  int x, y;
  Coordinate(int _x, int _y) {
    x = _x;
    y = _y;
  }
};

enum TextureType {
  BACKGROUND = 1 << 0,
  MATRIX = 1 << 1,
  I = 1 << 2,
  J = 1 << 3,
  L = 1 << 4,
  O = 1 << 5,
  S = 1 << 6,
  T = 1 << 7,
  Z = 1 << 8,
  DYING = 1 << 9,
  GHOST = 1 << 10,
  LOCKED = 1 << 11,
  NORMAL = 1 << 12,
  WHOLE = 1 << 13,
  FREE = 1 << 14,
};

// to combine flags using bitwise OR
TextureType operator|(TextureType a, TextureType b);

// attach texture to renderer's (x, y) position
void attachTextureToRenderer(SDL_Texture *texture, SDL_Renderer *renderer,
                             int x, int y);

// load all textures to renderer
void loadTexturesToRenderer(
    std::unordered_map<TextureType, SDL_Texture *> *textures,
    SDL_Renderer *renderer);

// load audios used in this program
void loadAudios(std::unordered_map<std::string, Audio *> *audios,
                const SDL_AudioDeviceID &audioDevice);

// load fonts used in this program
void loadFonts(std::unordered_map<std::string, TTF_Font *> *fonts);

// load program logo
void loadLogo(SDL_Window *window);

// write text on renderer (center aligned)
void writeText(SDL_Renderer *renderer, std::string message, TTF_Font *font,
               Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h);

// right align the string, using a specified character as the fill character.
std::string rjust(std::string s, int len, char pad);

// convert milisec to time string (format: <minutes>:<seconds>.<miliseconds>)
std::string convertMilisecToTimeString(Uint64 ms);

// change display scale to a specific value
void changeDisplayScale(float f);

#endif // utils_hpp_