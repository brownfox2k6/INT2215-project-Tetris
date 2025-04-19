#ifndef board_hpp_
#define board_hpp_

#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_stdinc.h"
#include "timer.hpp"
#include <SDL3/SDL.h>
#include <deque>
#include <string>
#include <tetromino.hpp>
#include <unordered_map>
#include <utils.hpp>

class Board {
private:
public:
  // drop rate (ms. per row) by level (1 to 15)
  const Uint64 autoDropInterval[16] = {0,   800, 717, 633, 550, 467, 383, 300,
                                       217, 133, 100, 83,  67,  50,  33,  17};
  // just last 20 rows are used to display, first 3 rows are hidden
  static const int VISIBLE_ROWS = 20;
  static const int HIDDEN_ROWS = 3;
  static const int ROWS = HIDDEN_ROWS + VISIBLE_ROWS;
  static const int COLS = 10;
  TextureType matrix[ROWS][COLS];
  std::deque<TextureType> queue;
  Tetromino current, ghost;
  TextureType hold;
  Uint64 timeInterval, timeFirstTouch;
  bool isPlaying, canHold;
  Timer timer;
  std::unordered_map<SDL_Scancode, Uint64> timePrevKey;
  std::unordered_map<SDL_Scancode, bool> prevKeyboardState;
  int level, linesCleared, linesCount, score;
  SDL_Renderer *renderer;
  std::unordered_map<TextureType, SDL_Texture *> *textures;
  std::unordered_map<std::string, TTF_Font *> *fonts;
  std::unordered_map<std::string, Audio *> *audios;

  Board(SDL_Renderer *renderer,
        std::unordered_map<TextureType, SDL_Texture *> *textures,
        std::unordered_map<std::string, TTF_Font *> *fonts,
        std::unordered_map<std::string, Audio *> *audios);

  // Resets the board to an empty state
  void reset();

  // go to next state of the game
  void nextState();

  // get a new tetromino to `current` from `queue`
  void nextTetromino();

  // lock current tetromino (to move on the next one)
  void lockCurrent();

  // create a new tetromino set and push to queue
  void pushNewTetrominoSet();

  // get next tetromino from queue and erase it
  // push a new tetromino set if needed
  TextureType getFromQueue();

  // hold current tetromino
  void holdCurrent();

  // Checks if a position is within bounds
  bool isWithinBounds(int row, int col) const;

  // Checks if a position is occupied
  bool isOccupied(int row, int col) const;

  // Checks if a row is full
  bool isRowFull(int row) const;

  // Clear full rows and returns the number of cleared rows
  int clearFullRows();

  // Clears a specific row
  void clearRow(int row);

  // Shift all rows from specific start_row down by one row
  void shiftRowsDown(int start_row);

  // Checks if the game is over (if any hidden rows contains any block)
  // if it's over, plays gameover sound and pause the timer
  bool isGameOver();

  // attach board to the Renderer
  void attachToRenderer();

  // make the current tetromino one step down
  // return true if it can go down, false otherwise
  bool softDrop();

  // make the current tetromino to move
  //   one step to the right if key == SDL_SCANCODE_RIGHT
  //   one step to the left if key == SDL_SCANCODE_LEFT
  // return true if it can go, false otherwise
  bool goHorizontal(int dy);

  // make the current tetromino to rotate
  //   to the left if d == -1
  //   to the right if d == 1
  // returns false if it can rotate, false otherwise.
  // implementation based on
  // https://tetris.wiki/Super_Rotation_System#Wall_Kicks
  bool rotate(int d);

  // hard drop current tetromino and go to next one
  void hardDrop();

  // update position of current tetromino and its ghost
  void updatePos();

  // write stats to the renderer while in wait screen (paused, game over)
  void writeStats();
};

#endif // board_hpp_