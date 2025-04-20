#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <board.hpp>
#include <constants.h>
#include <random.hpp>
#include <string>
#include <tetromino.hpp>
#include <unordered_map>
#include <utils.hpp>

const SDL_Scancode KEYS[] = {SDL_SCANCODE_UP,    SDL_SCANCODE_RIGHT,
                             SDL_SCANCODE_DOWN,  SDL_SCANCODE_LEFT,
                             SDL_SCANCODE_SPACE, SDL_SCANCODE_C,
                             SDL_SCANCODE_X};

Board::Board(SDL_Renderer *_renderer,
             std::unordered_map<TextureType, SDL_Texture *> *_textures,
             std::unordered_map<std::string, TTF_Font *> *_fonts,
             std::unordered_map<std::string, Audio *> *_audios) {
  renderer = _renderer;
  textures = _textures;
  fonts = _fonts;
  audios = _audios;
  reset();
}

void Board::reset() {
  for (int row = 0; row < ROWS; ++row) {
    clearRow(row);
  }
  hold = FREE;
  queue.clear();
  pushNewTetrominoSet();
  nextTetromino();
  timer.reset();
  for (SDL_Scancode key : KEYS) {
    timePrevKey[key] = timer.getTicks();
    prevKeyboardState[key] = false;
  }
  isPlaying = canHold = true;
  timeFirstTouch = linesCleared = linesCount = score = 0;
  level = 1;
}

void Board::nextState() {
  const bool *keyboardState = SDL_GetKeyboardState(NULL);
  if (keyboardState[SDL_SCANCODE_DOWN]) {
    if (timer.getTicks() - timePrevKey[SDL_SCANCODE_DOWN] > 50) {
      softDrop();
      audios->at(AUDIO_MOVE)->playOverwrite();
      timePrevKey[SDL_SCANCODE_DOWN] = timer.getTicks();
    }
  }
  if (keyboardState[SDL_SCANCODE_LEFT]) {
    if (!prevKeyboardState[SDL_SCANCODE_LEFT]) {
      goHorizontal(-1);
      timeInterval = 150;
      timePrevKey[SDL_SCANCODE_LEFT] = timer.getTicks();
    } else if (timer.getTicks() - timePrevKey[SDL_SCANCODE_LEFT] >
               timeInterval) {
      goHorizontal(-1);
      timeInterval = 50;
      timePrevKey[SDL_SCANCODE_LEFT] = timer.getTicks();
    }
  }
  if (keyboardState[SDL_SCANCODE_RIGHT]) {
    if (!prevKeyboardState[SDL_SCANCODE_RIGHT]) {
      goHorizontal(1);
      timeInterval = 150;
      timePrevKey[SDL_SCANCODE_RIGHT] = timer.getTicks();
    } else if (timer.getTicks() - timePrevKey[SDL_SCANCODE_RIGHT] >
               timeInterval) {
      goHorizontal(1);
      timeInterval = 50;
      timePrevKey[SDL_SCANCODE_RIGHT] = timer.getTicks();
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
  if (timer.getTicks() - timePrevKey[SDL_SCANCODE_DOWN] >
      autoDropInterval[level]) {
    softDrop();
  }
  if (ghost.pos == current.pos) {
    if (timeFirstTouch == 0) {
      timeFirstTouch = timer.getTicks();
    } else if (timer.getTicks() - timeFirstTouch > 2000) {
      lockCurrent();
      nextTetromino();
    }
  } else {
    timeFirstTouch = 0;
  }
  int cleared = clearFullRows();
  if (cleared == 1) {
    score += 100 * level;
    audios->at(AUDIO_COLLAPSE)->playOverwrite();
  } else if (cleared == 2) {
    score += 300 * level;
    audios->at(AUDIO_COLLAPSE)->playOverwrite();
  } else if (cleared == 3) {
    score += 500 * level;
    audios->at(AUDIO_COLLAPSE)->playOverwrite();
  } else if (cleared == 4) {
    score += 800 * level;
    audios->at(AUDIO_TETRIS)->playOverwrite();
  }
  linesCleared += cleared;
  linesCount += cleared;
  if (linesCount >= 10) {
    level = std::min(15, level + 1);
    linesCount = 0;
    audios->at(AUDIO_LEVEL_UP)->playOverwrite();
  }
  for (SDL_Scancode key : KEYS) {
    prevKeyboardState[key] = keyboardState[key];
  }
  updatePos();
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
  audios->at(AUDIO_LOCK)->playOverwrite();
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
  if (!canHold) {
    audios->at(AUDIO_INPUT_FAILED)->playOverwrite();
    return;
  }
  if (hold == FREE) {
    hold = current.shape;
    nextTetromino();
  } else {
    std::swap(hold, current.shape);
    current.getInitPos();
  }
  canHold = false;
  audios->at(AUDIO_HOLD)->playOverwrite();
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
  int next_free_row = ROWS - 1;
  int cleared = 0;
  for (int row = ROWS - 1; row >= 0; --row) {
    if (!isRowFull(row)) {
      if (row != next_free_row) {
        std::copy(matrix[row], matrix[row] + COLS, matrix[next_free_row]);
      }
      --next_free_row;
    } else {
      ++cleared;
    }
  }
  for (int row = next_free_row; row >= 0; --row) {
    clearRow(row);
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

bool Board::isGameOver() {
  for (int row = 0; row < HIDDEN_ROWS; ++row) {
    for (const TextureType &cell : matrix[row]) {
      if (cell != FREE) {
        audios->at(AUDIO_BLOCKOUT)->play();
        timer.pause();
        return true;
      }
    }
  }
  return false;
}

void Board::attachToRenderer() {
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
      attachTextureToRenderer(textures->at(t), renderer, x, y);
      x += CELL_POS_INCREMENT;
    }
    x = CELL_POS_X_INIT;
    y += CELL_POS_INCREMENT;
  }
  // Display the hold tetromino
  if (hold != FREE) {
    SDL_Texture *t = textures->at(WHOLE | hold);
    const int _x = HOLD_POS_X + (BLOCK_WIDTH - t->w) / 2;
    const int _y = HOLD_POS_Y + (BLOCK_HEIGHT - t->h) / 2;
    attachTextureToRenderer(t, renderer, _x, _y);
  }
  // Display the queue
  y = QUEUE_POS_Y_INIT;
  for (int i = 0; i < 3; ++i) {
    SDL_Texture *t = textures->at(WHOLE | queue[i]);
    const int _x = QUEUE_POS_X + (BLOCK_WIDTH - t->w) / 2;
    const int _y = y + (BLOCK_HEIGHT - t->h) / 2;
    attachTextureToRenderer(t, renderer, _x, _y);
    y += BLOCK_HEIGHT;
  }
  // Pause instruction
  writeText(renderer, "[P] = pause", fonts->at("consolab 24"), COLOR_RED, 570,
            550, 170, 28);
}

bool Board::softDrop() {
  for (const auto &[row, col] : current.pos) {
    if (isOccupied(row + 1, col)) {
      return false;
    }
  }
  ++current.dx;
  timePrevKey[SDL_SCANCODE_DOWN] = timer.getTicks();
  return true;
}

bool Board::goHorizontal(int dy) {
  assert(abs(dy) == 1);
  for (const auto &[row, col] : current.pos) {
    if (isOccupied(row, col + dy)) {
      audios->at(AUDIO_INPUT_FAILED)->playOverwrite();
      return false;
    }
  }
  current.dy += dy;
  audios->at(AUDIO_MOVE)->playOverwrite();
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
      audios->at(AUDIO_ROTATE)->playOverwrite();
      return true;
    }
  }
  audios->at(AUDIO_INPUT_FAILED)->playOverwrite();
  return false;
}

void Board::hardDrop() {
  current.pos = ghost.pos;
  lockCurrent();
  nextTetromino();
  audios->at(AUDIO_HARD_DROP)->playOverwrite();
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

void Board::writeStats() {
  // Display score
  writeText(renderer, std::to_string(score), fonts->at("consolab 24"),
            COLOR_WHITE, 83, 378, 120, 28);
  // Display level
  writeText(renderer, std::to_string(level), fonts->at("consolab 24"),
            COLOR_WHITE, 83, 438, 120, 28);
  // Display lines cleared
  writeText(renderer, std::to_string(linesCleared), fonts->at("consolab 24"),
            COLOR_WHITE, 83, 498, 120, 28);
  // Display elapsed time
  writeText(renderer, "ELAPSED TIME", fonts->at("consolab 24"), COLOR_BLACK,
            570, 397, 170, 28);
  writeText(renderer, convertMilisecToTimeString(timer.getTicks()),
            fonts->at("consolab 32"), COLOR_BLUE, 570, 432, 170, 32);
}