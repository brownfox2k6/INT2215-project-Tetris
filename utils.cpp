#include <constants.h>
#include <utils.hpp>

void attachTextureToRenderer(SDL_Texture *texture, SDL_Renderer *renderer,
                             int x, int y) {
  SDL_FRect rect;
  rect.x = x * RATIO;
  rect.y = y * RATIO;
  rect.w = texture->w * RATIO;
  rect.h = texture->h * RATIO;
  SDL_RenderTexture(renderer, texture, NULL, &rect);
}

void attachTextureToRendererFilled(SDL_Texture *texture,
                                   SDL_Renderer *renderer) {
  attachTextureToRenderer(texture, renderer, 0, 0);
}

void attachTextureToRendererCentered(SDL_Texture *texture,
                                     SDL_Renderer *renderer) {
  float x = (WINDOW_WIDTH - texture->w) / 2.0;
  float y = (WINDOW_HEIGHT - texture->h) / 2.0;
  attachTextureToRenderer(texture, renderer, x, y);
}

void loadTexturesToRenderer(
    std::unordered_map<std::string, SDL_Texture *> &texture,
    SDL_Renderer *renderer) {
  texture["background"] = IMG_LoadTexture(renderer, BACKGROUND_PATH);
  texture["matrix"] = IMG_LoadTexture(renderer, MATRIX_PATH);
  texture["dying I"] = IMG_LoadTexture(renderer, DYING_I_PATH);
  texture["dying J"] = IMG_LoadTexture(renderer, DYING_J_PATH);
  texture["dying L"] = IMG_LoadTexture(renderer, DYING_L_PATH);
  texture["dying O"] = IMG_LoadTexture(renderer, DYING_O_PATH);
  texture["dying S"] = IMG_LoadTexture(renderer, DYING_S_PATH);
  texture["dying T"] = IMG_LoadTexture(renderer, DYING_T_PATH);
  texture["dying Z"] = IMG_LoadTexture(renderer, DYING_Z_PATH);
  texture["ghost I"] = IMG_LoadTexture(renderer, GHOST_I_PATH);
  texture["ghost J"] = IMG_LoadTexture(renderer, GHOST_J_PATH);
  texture["ghost L"] = IMG_LoadTexture(renderer, GHOST_L_PATH);
  texture["ghost O"] = IMG_LoadTexture(renderer, GHOST_O_PATH);
  texture["ghost S"] = IMG_LoadTexture(renderer, GHOST_S_PATH);
  texture["ghost T"] = IMG_LoadTexture(renderer, GHOST_T_PATH);
  texture["ghost Z"] = IMG_LoadTexture(renderer, GHOST_Z_PATH);
  texture["locked I"] = IMG_LoadTexture(renderer, LOCKED_I_PATH);
  texture["locked J"] = IMG_LoadTexture(renderer, LOCKED_J_PATH);
  texture["locked L"] = IMG_LoadTexture(renderer, LOCKED_L_PATH);
  texture["locked O"] = IMG_LoadTexture(renderer, LOCKED_O_PATH);
  texture["locked S"] = IMG_LoadTexture(renderer, LOCKED_S_PATH);
  texture["locked T"] = IMG_LoadTexture(renderer, LOCKED_T_PATH);
  texture["locked Z"] = IMG_LoadTexture(renderer, LOCKED_Z_PATH);
  texture["normal I"] = IMG_LoadTexture(renderer, NORMAL_I_PATH);
  texture["normal J"] = IMG_LoadTexture(renderer, NORMAL_J_PATH);
  texture["normal L"] = IMG_LoadTexture(renderer, NORMAL_L_PATH);
  texture["normal O"] = IMG_LoadTexture(renderer, NORMAL_O_PATH);
  texture["normal S"] = IMG_LoadTexture(renderer, NORMAL_S_PATH);
  texture["normal T"] = IMG_LoadTexture(renderer, NORMAL_T_PATH);
  texture["normal Z"] = IMG_LoadTexture(renderer, NORMAL_Z_PATH);
}