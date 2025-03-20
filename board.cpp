#include <SDL3_ttf/SDL_ttf.h>
#include <board.hpp>
#include <constants.h>
#include <random.hpp>
#include <tetromino.hpp>
#include <utils.hpp>

const SDL_Scancode KEYS[] = {SDL_SCANCODE_UP,    SDL_SCANCODE_RIGHT,
                             SDL_SCANCODE_DOWN,  SDL_SCANCODE_LEFT,
                             SDL_SCANCODE_SPACE, SDL_SCANCODE_C,
                             SDL_SCANCODE_X};

Board::Board() { reset(); }

void Board::reset() {
  for (int row = 0; row < ROWS; ++row) {
    clearRow(row);
  }
  hold = FREE;
  queue.clear();
  pushNewTetrominoSet();
  nextTetromino();
  timeCur = SDL_GetTicks();
  for (SDL_Scancode key : KEYS) {
    timePrevKey[key] = timeCur;
    prevKeyboardState[key] = false;
  }
  isPlaying = canHold = true;
  timeFirstTouch = linesCleared = 0;
}

void Board::nextState() {
  updatePos();
  timeCur = SDL_GetTicks();
  const bool *keyboardState = SDL_GetKeyboardState(NULL);
  if (keyboardState[SDL_SCANCODE_DOWN]) {
    if (timeCur - timePrevKey[SDL_SCANCODE_DOWN] > 50) {
      softDrop();
      timePrevKey[SDL_SCANCODE_DOWN] = timeCur;
    }
  }
  if (keyboardState[SDL_SCANCODE_LEFT]) {
    if (!prevKeyboardState[SDL_SCANCODE_LEFT]) {
      goHorizontal(-1);
      timeInterval = 150;
      timePrevKey[SDL_SCANCODE_LEFT] = timeCur;
    } else if (timeCur - timePrevKey[SDL_SCANCODE_LEFT] > timeInterval) {
      goHorizontal(-1);
      timeInterval = 50;
      timePrevKey[SDL_SCANCODE_LEFT] = timeCur;
    }
  }
  if (keyboardState[SDL_SCANCODE_RIGHT]) {
    if (!prevKeyboardState[SDL_SCANCODE_RIGHT]) {
      goHorizontal(1);
      timeInterval = 150;
      timePrevKey[SDL_SCANCODE_RIGHT] = timeCur;
    } else if (timeCur - timePrevKey[SDL_SCANCODE_RIGHT] > timeInterval) {
      goHorizontal(1);
      timeInterval = 50;
      timePrevKey[SDL_SCANCODE_RIGHT] = timeCur;
    }
  }
  if (keyboardState[SDL_SCANCODE_UP] && !prevKeyboardState[SDL_SCANCODE_UP]) {
    rotate(1);
  }
  if (keyboardState[SDL_SCANCODE_X] && !prevKeyboardState[SDL_SCANCODE_X]) {
    rotate(-1);
  }
  if (keyboardState[SDL_SCANCODE_SPACE] &&
      !prevKeyboardState[SDL_SCANCODE_SPACE]) {
    hardDrop();
  }
  if (keyboardState[SDL_SCANCODE_C] && !prevKeyboardState[SDL_SCANCODE_C]) {
    holdCurrent();
  }
  if (timeCur - timePrevKey[SDL_SCANCODE_DOWN] > 100) {
    softDrop();
  }
  if (ghost.pos == current.pos) {
    if (timeFirstTouch == 0) {
      timeFirstTouch = timeCur;
    } else if (timeCur - timeFirstTouch > 1000) {
      lockCurrent();
      nextTetromino();
    }
  } else {
    timeFirstTouch = 0;
  }
  clearFullRows();
  if (isGameOver()) {
    reset();
  }
  for (SDL_Scancode key : KEYS) {
    prevKeyboardState[key] = keyboardState[key];
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
  canHold = true;
  timeFirstTouch = 0;
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

void Board::holdCurrent() {
  if (canHold) {
    if (hold == FREE) {
      hold = current.shape;
      nextTetromino();
    } else {
      std::swap(hold, current.shape);
      current.getInitPos();
    }
    canHold = false;
  }
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
      shiftRowsDown(row);
      ++cleared;
    }
  }
  linesCleared += cleared;
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
      TextureType t;
      if (current.isOccupying(row, col)) {
        t = (timeFirstTouch ? DYING : NORMAL) | current.shape;
      } else if (ghost.isOccupying(row, col)) {
        t = GHOST | current.shape;
      } else {
        t = matrix[row][col];
      }
      attachTextureToRenderer(textures[t], renderer, x, y);
      x += CELL_POS_INCREMENT;
    }
    x = CELL_POS_X_INIT;
    y += CELL_POS_INCREMENT;
  }
  // Display the hold tetromino
  if (hold != FREE) {
    SDL_Texture *t = textures[WHOLE | hold];
    const int _x = HOLD_POS_X + (BLOCK_WIDTH - t->w) / 2;
    const int _y = HOLD_POS_Y + (BLOCK_HEIGHT - t->h) / 2;
    attachTextureToRenderer(t, renderer, _x, _y);
  }
  // Display the queue
  y = QUEUE_POS_Y_INIT;
  for (int i = 0; i < 3; ++i) {
    SDL_Texture *t = textures[WHOLE | queue[i]];
    const int _x = QUEUE_POS_X + (BLOCK_WIDTH - t->w) / 2;
    const int _y = y + (BLOCK_HEIGHT - t->h) / 2;
    attachTextureToRenderer(t, renderer, _x, _y);
    y += BLOCK_HEIGHT;
  }
}

bool Board::softDrop() {
  for (const auto &[row, col] : current.pos) {
    if (isOccupied(row + 1, col)) {
      return false;
    }
  }
  ++current.dx;
  timePrevKey[SDL_SCANCODE_DOWN] = timeCur;
  return true;
}

bool Board::goHorizontal(int dy) {
  assert(abs(dy) == 1);
  for (const auto &[row, col] : current.pos) {
    if (isOccupied(row, col + dy)) {
      return false;
    }
  }
  current.dy += dy;
  return true;
}

bool Board::rotate(int d) {
  assert(abs(d) == 1);
  int dir = (current.dir + d + 4) % 4;
  std::vector<std::pair<int, int>> tests;
  if (d == -1) {
    if (current.shape == O) {
      tests = {{0, 0}};
    } else if (current.shape == I) {
      if (dir == 0) {
        tests = {{0, 0}, {+2, 0}, {-1, 0}, {+2, +1}, {-1, -2}};
      } else if (dir == 1) {
        tests = {{0, 0}, {+1, 0}, {-2, 0}, {+1, -2}, {-2, +1}};
      } else if (dir == 2) {
        tests = {{0, 0}, {-2, 0}, {+1, 0}, {-2, -1}, {+1, +2}};
      } else { // dir == 3
        tests = {{0, 0}, {-1, 0}, {+2, 0}, {-1, +2}, {+2, -1}};
      }
    } else { // current.shape == J, L, S, T or Z
      if (dir == 0) {
        tests = {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}};
      } else if (dir == 1) {
        tests = {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}};
      } else if (dir == 2) {
        tests = {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}};
      } else { // dir == 3
        tests = {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}};
      }
    }
  } else { // d == 1
    if (current.shape == O) {
      tests = {{0, 0}};
    } else if (current.shape == I) {
      if (dir == 0) {
        tests = {{0, 0}, {+1, 0}, {-2, 0}, {+1, -2}, {-2, +1}};
      } else if (dir == 1) {
        tests = {{0, 0}, {-2, 0}, {+1, 0}, {-2, -1}, {+1, +2}};
      } else if (dir == 2) {
        tests = {{0, 0}, {-1, 0}, {+2, 0}, {-1, +2}, {+2, -1}};
      } else {
        tests = {{0, 0}, {+2, 0}, {-1, 0}, {+2, +1}, {-1, -2}};
      }
    } else { // current.shape == J, L, S, T or Z
      if (dir == 0) {
        tests = {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}};
      } else if (dir == 1) {
        tests = {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}};
      } else if (dir == 2) {
        tests = {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}};
      } else {
        tests = {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}};
      }
    }
  }
  for (const auto &[dx, dy] : tests) {
    bool ok = true;
    for (const auto &[row, col] : current.getPos(dir)) {
      if (isOccupied(row + dx, col + dy)) {
        ok = false;
        break;
      }
    }
    if (ok) {
      current.dir = dir;
      current.dx += dx;
      current.dy += dy;
      return true;
    }
  }
  return false;
}

void Board::hardDrop() {
  current.pos = ghost.pos;
  lockCurrent();
  nextTetromino();
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