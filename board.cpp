#include "SDL3/SDL_timer.h"
#include <algorithm>
#include <board.hpp>
#include <constants.h>
#include <ctime>
#include <iostream>
#include <tetromino.hpp>
#include <utils.hpp>

Board::Board() { reset(); }

void Board::reset() {
  for (int row = 0; row < ROWS; ++row) {
    clearRow(row);
  }
  current.initRandom();
  ghost = current;
  getGhost();
  hold.erase();
  next.clear();
  for (int i = HOLDS; i--;) {
    pushNewTetromino();
  }
  timeCur = timePrev = SDL_GetTicks();
  isPlaying = true;
}

void Board::nextState() {
  if (timeCur - timePrev > 200) {
    goDown();
    timePrev = timeCur;
  }
}

void Board::nextTetromino() {
  current = ghost = next.front();
  next.pop_front();
  pushNewTetromino();
}

void Board::pushNewTetromino() {
  Tetromino t;
  t.initRandom();
  next.emplace_back(t);
}

bool Board::isWithinBounds(int row, int col) const {
  return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

bool Board::isOccupied(int row, int col) const {
  return isWithinBounds(row, col) && matrix[row][col] == FREE;
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
  int x = CELL_POS_X_INIT;
  int y = CELL_POS_Y_INIT;
  for (int row = HIDDEN_ROWS; row < ROWS; ++row) {
    for (int col = 0; col < COLS; ++col) {
      if (current.isOccupying(row, col)) {
        TextureType t = NORMAL | current.shape;
        attachTextureToRenderer(textures[t], renderer, x, y);
      } else if (ghost.isOccupying(row, col)) {
        TextureType t = GHOST | ghost.shape;
        attachTextureToRenderer(textures[t], renderer, x, y);
      } else {
        attachTextureToRenderer(textures[matrix[row][col]], renderer, x, y);
      }
      x += CELL_POS_INCREMENT;
    }
    x = CELL_POS_X_INIT;
    y += CELL_POS_INCREMENT;
  }
}

bool Board::goDown() {
  std::vector<std::pair<int, int>> newPos;
  for (const auto &[x, y] : current.pos) {
    if (x + 1 == ROWS || matrix[x + 1][y] != FREE) {
      return false;
    }
    newPos.emplace_back(x + 1, y);
  }
  current.pos = newPos;
  timePrev = timeCur;
  return true;
}

bool Board::goLeft() {
  std::vector<std::pair<int, int>> newPos;
  for (const auto &[x, y] : current.pos) {
    if (y == 0 || matrix[x][y - 1] != FREE) {
      return false;
    }
    newPos.emplace_back(x, y - 1);
  }
  current.pos = newPos;
  timePrev = timeCur;
  getGhost();
  return true;
}

bool Board::goRight() {
  std::vector<std::pair<int, int>> newPos;
  for (const auto &[x, y] : current.pos) {
    if (y + 1 == COLS || matrix[x][y + 1] != FREE) {
      return false;
    }
    newPos.emplace_back(x, y + 1);
  }
  current.pos = newPos;
  timePrev = timeCur;
  getGhost();
  return true;
}

bool Board::rotateLeft() { return false; }

bool Board::rotateRight() { return false; }

void Board::getGhost() {
  // binary search on how many rows it can go down
  int lo = 0;
  int hi = VISIBLE_ROWS;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    bool ok = true;
    for (const auto &[x, y] : current.pos) {
      if (x + mid >= ROWS || matrix[x + mid][y] != FREE) {
        ok = false;
        break;
      }
    }
    if (ok) {
      lo = mid + 1;
    } else {
      hi = mid - 1;
    }
  }
  ghost.pos.clear();
  for (const auto &[x, y] : current.pos) {
    ghost.pos.emplace_back(x + hi, y);
  }
}