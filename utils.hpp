#ifndef utils_hpp_
#define utils_hpp_

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include <string>

void attachTextureToRenderer(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);
void attachTextureToRendererFilled(SDL_Texture *texture, SDL_Renderer *renderer);
void attachTextureToRendererCentered(SDL_Texture *texture, SDL_Renderer *renderer);
void loadTexturesToRenderer(std::unordered_map<std::string, SDL_Texture*> &textures, SDL_Renderer *renderer);

#endif  // utils_hpp_