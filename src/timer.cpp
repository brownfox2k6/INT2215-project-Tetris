#include <timer.hpp>

Timer::Timer() { reset(); }

void Timer::start() {
  isStarted = true;
  isPaused = false;
  startTicks = SDL_GetTicks();
  pausedTicks = 0;
}

void Timer::reset() {
  isStarted = isPaused = false;
  startTicks = pausedTicks = 0;
}

void Timer::pause() {
  if (!isStarted || isPaused) {
    return;
  }
  isPaused = true;
  pausedTicks = SDL_GetTicks() - startTicks;
  startTicks = 0;
}

void Timer::resume() {
  if (!isStarted) {
    start();
  }
  if (!isPaused) {
    return;
  }
  isPaused = false;
  startTicks = SDL_GetTicks() - pausedTicks;
  pausedTicks = 0;
}

void Timer::lap() { lastLap = getTicks(); }

Uint64 Timer::distanceFromLap() { return getTicks() - lastLap; }

Uint64 Timer::getTicks() {
  if (!isStarted) {
    return 0;
  }
  if (!isPaused) {
    return SDL_GetTicks() - startTicks;
  }
  return pausedTicks;
}