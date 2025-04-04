#include <random.hpp>
#include <tetromino.hpp>
#include <utils.hpp>
#include <vector>

Tetromino::Tetromino() { erase(); }

Tetromino::Tetromino(TextureType _shape) {
  shape = _shape;
  getInitPos();
}

void Tetromino::erase() {
  shape = FREE;
  pos.clear();
}

void Tetromino::getInitPos() {
  dir = dx = dy = 0;
  if (shape == I) {
    iPos = {{{1, 3}, {1, 4}, {1, 5}, {1, 6}},
            {{0, 5}, {1, 5}, {2, 5}, {3, 5}},
            {{2, 3}, {2, 4}, {2, 5}, {2, 6}},
            {{0, 4}, {1, 4}, {2, 4}, {3, 4}}};
  } else if (shape == J) {
    iPos = {{{0, 3}, {1, 3}, {1, 4}, {1, 5}},
            {{0, 4}, {0, 5}, {1, 4}, {2, 4}},
            {{2, 5}, {1, 3}, {1, 4}, {1, 5}},
            {{0, 4}, {1, 4}, {2, 4}, {2, 3}}};
  } else if (shape == L) {
    iPos = {{{0, 5}, {1, 3}, {1, 4}, {1, 5}},
            {{0, 4}, {2, 5}, {1, 4}, {2, 4}},
            {{2, 3}, {1, 3}, {1, 4}, {1, 5}},
            {{0, 4}, {1, 4}, {2, 4}, {0, 3}}};
  } else if (shape == O) {
    iPos = {{{0, 4}, {0, 5}, {1, 4}, {1, 5}},
            {{0, 4}, {0, 5}, {1, 4}, {1, 5}},
            {{0, 4}, {0, 5}, {1, 4}, {1, 5}},
            {{0, 4}, {0, 5}, {1, 4}, {1, 5}}};
  } else if (shape == S) {
    iPos = {{{0, 4}, {0, 5}, {1, 3}, {1, 4}},
            {{0, 4}, {1, 4}, {1, 5}, {2, 5}},
            {{1, 4}, {1, 5}, {2, 3}, {2, 4}},
            {{0, 3}, {1, 3}, {1, 4}, {2, 4}}};
  } else if (shape == T) {
    iPos = {{{0, 4}, {1, 3}, {1, 4}, {1, 5}},
            {{0, 4}, {2, 4}, {1, 4}, {1, 5}},
            {{2, 4}, {1, 3}, {1, 4}, {1, 5}},
            {{0, 4}, {2, 4}, {1, 4}, {1, 3}}};
  } else if (shape == Z) {
    iPos = {{{0, 3}, {0, 4}, {1, 4}, {1, 5}},
            {{0, 5}, {1, 5}, {1, 4}, {2, 4}},
            {{1, 3}, {1, 4}, {2, 4}, {2, 5}},
            {{0, 4}, {1, 4}, {1, 3}, {2, 3}}};
  } else {
    assert(0); // invalid shape
  }
}

bool Tetromino::isOccupying(int i, int j) const {
  for (const auto &[x, y] : pos) {
    if (x == i && y == j) {
      return true;
    }
  }
  return false;
}

void Tetromino::updatePos() {
  pos.clear();
  for (const auto &[x, y] : iPos[dir]) {
    pos.emplace_back(x + dx, y + dy);
  }
}

std::vector<std::pair<int, int>> Tetromino::getPos(int d) {
  std::vector<std::pair<int, int>> v;
  for (const auto &[x, y] : iPos[d]) {
    v.emplace_back(x + dx, y + dy);
  }
  return v;
}