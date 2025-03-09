#ifndef random_hpp_
#define random_hpp_

#include <algorithm>
#include <chrono>
#include <random>

// rng = random number generator
static std::mt19937
    rng(std::chrono::steady_clock::now().time_since_epoch().count());

// gives a random integer in range [lo, hi]
int randInt(int lo, int hi);

// gives a random real number in range [lo, hi)
float randReal(float lo, float hi);

// randomly shuffle the vector
template <typename T> void randShuffle(std::vector<T> &v) {
  std::shuffle(begin(v), end(v), rng);
}

// returns a random element in the container
template <typename T> T randElement(std::vector<T> &v) {
  int n = v.size();
  return v[randInt(0, n - 1)];
}

#endif // random_hpp_