#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_timer.h"
#include <algorithm>
#include <board.hpp>
#include <cassert>
#include <constants.h>
#include <ctime>
#include <exception>
#include <iostream>
#include <random.hpp>
#include <random>
#include <tetromino.hpp>
#include <utility>
#include <utils.hpp>

Board::Board() { reset(); }

void Board::reset() {
  for (int row = 0; row < ROWS; ++row) {
    clearRow(row);
  }
  hold.erase();
  queue.clear();
  pushNewTetrominoSet();
  nextTetromino();
  timeCur = timePrevGoDown = timePrevKey = SDL_GetTicks();
  isPlaying = true;
}

void Board::nextState(SDL_Scancode &key, bool &isFirstPress) {
  timeCur = SDL_GetTicks();
  updatePos();
  if (key == SDL_SCANCODE_DOWN && timeCur - timePrevKey > 50) {
    goDown();
    timePrevKey = timeCur;
  } else if (key == SDL_SCANCODE_LEFT || key == SDL_SCANCODE_RIGHT) {
    if (isFirstPress) {
      goHorizontal(key);
      interval = 150;
      isFirstPress = false;
    } else if (timeCur - timePrevKey > interval) {
      goHorizontal(key);
      interval = 50;
    }
  } else if (key == SDL_SCANCODE_UP && isFirstPress) {
    rotate(1);
    isFirstPress = false;
    timePrevKey = timeCur;
  } else if (key == SDL_SCANCODE_SPACE && isFirstPress) {
    hardDrop();
    isFirstPress = false;
    timePrevKey = timeCur;
  }
  if (timeCur - timePrevGoDown > 200) {
    goDown();
    timePrevGoDown = timeCur;
  }
  if (ghost.pos == current.pos) {
    lockCurrent();
    nextTetromino();
  }
  clearFullRows();
  if (isGameOver()) {
    reset();
  }
}

void Board::nextTetromino() {
  current = Tetromino(getFromQueue());
  if (queue.size() < 4) {
    pushNewTetrominoSet();
  }
}

void Board::lockCurrent() {
  for (const auto &[x, y] : current.pos) {
    matrix[x][y] = LOCKED | current.shape;
  }
}

void Board::pushNewTetrominoSet() {
  std::vector<TextureType> tetrominoSet = {I, J, L, O, S, T, Z};
  randShuffle(tetrominoSet);
  for (const TextureType &t : tetrominoSet) {
    queue.emplace_back(t);
  }
}

TextureType Board::getFromQueue() {
  TextureType t = queue.front();
  queue.pop_front();
  if (queue.size() < 4) {
    pushNewTetrominoSet();
  }
  return t;
}

bool Board::isWithinBounds(int row, int col) const {
  return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

bool Board::isOccupied(int row, int col) const {
  return !isWithinBounds(row, col) || matrix[row][col] != FREE;
}

bool Board::isRowFull(int row) const {
  for (const TextureType &cell : matrix[row]) {
    if (cell == FREE) {
      return false;
    }
  }
  return true;
}

int Board::clearFullRows() {
  int cleared = 0;
  for (int row = ROWS - 1; row >= 0; --row) {
    if (isRowFull(row)) {
      clearRow(row);
      shiftRowsDown(row);
      ++cleared;
    }
  }
  return cleared;
}

void Board::clearRow(int row) {
  for (TextureType &cell : matrix[row]) {
    cell = FREE;
  }
}

void Board::shiftRowsDown(int start_row) {
  for (int row = start_row; row > 0; --row) {
    std::copy(matrix[row - 1], matrix[row - 1] + COLS, matrix[row]);
  }
}

bool Board::isGameOver() const {
  for (int row = 0; row < HIDDEN_ROWS; ++row) {
    for (const TextureType &cell : matrix[row]) {
      if (cell != FREE) {
        return true;
      }
    }
  }
  return false;
}

void Board::attachToRenderer(
    std::unordered_map<TextureType, SDL_Texture *> &textures,
    SDL_Renderer *renderer) {
  // Display the matrix
  int x = CELL_POS_X_INIT;
  int y = CELL_POS_Y_INIT;
  for (int row = HIDDEN_ROWS; row < ROWS; ++row) {
    for (int col = 0; col < COLS; ++col) {
      if (current.isOccupying(row, col)) {
        TextureType t = NORMAL | current.shape;
        attachTextureToRenderer(textures[t], renderer, x, y);
      } else if (ghost.isOccupying(row, col)) {
        TextureType t = GHOST | current.shape;
        attachTextureToRenderer(textures[t], renderer, x, y);
      } else {
        attachTextureToRenderer(textures[matrix[row][col]], renderer, x, y);
      }
      x += CELL_POS_INCREMENT;
    }
    x = CELL_POS_X_INIT;
    y += CELL_POS_INCREMENT;
  }
  // Display the hold tetromino

  // Display the queue
  x = 600;
  y = 100;
  for (int i = 0; i < 3; ++i) {
    SDL_Texture *t = textures[WHOLE | queue[i]];
    int xx = x + (115 - t->w) / 2;
    int yy = y + (68 - t->h) / 2;
    attachTextureToRenderer(t, renderer, xx, yy);
    y += 68;
  }
}

bool Board::goDown() {
  for (const auto &[row, col] : current.pos) {
    if (isOccupied(row + 1, col)) {
      return false;
    }
  }
  ++current.dx;
  timePrevGoDown = timeCur;
  return true;
}

bool Board::goHorizontal(const SDL_Scancode &key) {
  assert(key == SDL_SCANCODE_RIGHT || key == SDL_SCANCODE_LEFT);
  int dy = key == SDL_SCANCODE_RIGHT? 1 : -1;
  for (const auto &[row, col] : current.pos) {
    if (isOccupied(row, col + dy)) {
      return false;
    }
  }
  current.dy += dy;
  timePrevKey = timeCur;
  return true;
}

bool Board::rotate(int d) {
  assert(abs(d) == 1);
  int dir = (current.dir + d + 4) % 4;
  for (const auto &[row, col] : current.getPos(dir)) {
    if (isOccupied(row, col)) {
      return false;
    }
  }
  current.dir = dir;
  timePrevKey = timeCur;
  return true;
}

void Board::hardDrop() {
  current.pos = ghost.pos;
  lockCurrent();
  nextTetromino();
  timePrevKey = timeCur;
}

void Board::updatePos() {
  current.updatePos();
  int can = 0;
  bool ok = true;
  while (true) {
    for (const auto &[row, col] : current.pos) {
      if (isOccupied(row + can + 1, col)) {
        ok = false;
        break;
      }
    }
    if (!ok) {
      break;
    }
    ++can;
  }
  ghost.pos.clear();
  for (const auto &[row, col] : current.pos) {
    ghost.pos.emplace_back(row + can, col);
  }
}