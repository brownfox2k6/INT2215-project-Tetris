#include <random.hpp>
#include <tetromino.hpp>
#include <utils.hpp>
#include <vector>

Tetromino::Tetromino() { erase(); }

Tetromino::Tetromino(TextureType _shape) {
  shape = _shape;
  initPos();
}

void Tetromino::erase() {
  shape = FREE;
  pos.clear();
}

void Tetromino::initPos() {
  if (shape == I) {
    pos = {{2, 3}, {2, 4}, {2, 5}, {2, 6}};
  } else if (shape == J) {
    pos = {{1, 3}, {2, 3}, {2, 4}, {2, 5}};
  } else if (shape == L) {
    pos = {{1, 5}, {2, 3}, {2, 4}, {2, 5}};
  } else if (shape == O) {
    pos = {{1, 4}, {1, 5}, {2, 4}, {2, 5}};
  } else if (shape == S) {
    pos = {{1, 4}, {1, 5}, {2, 3}, {2, 4}};
  } else if (shape == T) {
    pos = {{1, 4}, {2, 3}, {2, 4}, {2, 5}};
  } else if (shape == Z) {
    pos = {{1, 3}, {1, 4}, {2, 4}, {2, 5}};
  } else {
    assert(0);
  }
}

void Tetromino::initRandom() {
  std::vector<TextureType> shapes = {I, J, L, O, S, T, Z};
  shape = randElement(shapes);
  initPos();
}

bool Tetromino::isOccupying(int x, int y) const {
  for (const auto &[i, j] : pos) {
    if (x == i && y == j) {
      return true;
    }
  }
  return false;
}