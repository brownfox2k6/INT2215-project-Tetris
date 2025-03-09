#ifndef utils_hpp_
#define utils_hpp_

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
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
  FREE = 1 << 13,
};


// to combine flags using bitwise OR
TextureType operator|(TextureType a, TextureType b);

// attach texture to renderer's (x, y) position
void attachTextureToRenderer(SDL_Texture *texture, SDL_Renderer *renderer,
                             int x, int y);

// load all textures to renderer
void loadTexturesToRenderer(
    std::unordered_map<TextureType, SDL_Texture *> &textures,
    SDL_Renderer *renderer);

// load program logo
void loadLogo(SDL_Window *window);

#endif // utils_hpp_