#ifndef board_hpp_
#define board_hpp_

#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_stdinc.h"
#include <SDL3/SDL.h>
#include <deque>
#include <tetromino.hpp>
#include <unordered_map>
#include <utils.hpp>


class Board {
private:
public:
  // just last 20 rows are used to display, first 3 rows are hidden
  static const int VISIBLE_ROWS = 20;
  static const int HIDDEN_ROWS = 4;
  static const int ROWS = HIDDEN_ROWS + VISIBLE_ROWS;
  static const int COLS = 10;
  TextureType matrix[ROWS][COLS];
  std::deque<TextureType> queue;
  Tetromino current, ghost;
  TextureType hold;
  Uint64 timeCur, timePrevGoDown, timePrevKey;
  bool isPlaying, canHold;
  Uint64 timeInterval, timeFirstTouch;

  Board();

  // Resets the board to an empty state
  void reset();

  // go to next state of the game
  void nextState(SDL_Scancode &key, bool &isFirstPress);

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

  // Checks if the game is over
  // (if any hidden rows contains any block)
  bool isGameOver() const;

  // attach board to the Renderer
  void
  attachToRenderer(std::unordered_map<TextureType, SDL_Texture *> &textures,
                   SDL_Renderer *renderer);

  // make the current tetromino one step down
  // return true if it can go down, false otherwise
  bool goDown();

  // make the current tetromino to move
  //   one step to the right if key == SDL_SCANCODE_RIGHT
  //   one step to the left if key == SDL_SCANCODE_LEFT
  // return true if it can go, false otherwise
  bool goHorizontal(const SDL_Scancode &key);

  // make the current tetromino to rotate
  //   90 degrees to the right if d = 1
  //   90 degrees to the leftt if d = -1
  // return true if it can rotate, false otherwise
  bool rotate(int d);

  // hard drop current tetromino and go to next one
  void hardDrop();

  // update position of current tetromino and its ghost 
  void updatePos();
};

#endif // board_hpp_