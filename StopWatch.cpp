#include "StopWatch.h"

StopWatch::StopWatch() { }
uint32_t StopWatch::elapsed() { return stopWatchValue(); }

void StopWatch::stopWatchStart() {
  if (stopWatchState == RESET || stopWatchState == STOPPED) {
     stopWatchState = RUNNING;
     uint32_t t = millis();
     swStartTime += t - swStopTime;
     swStopTime = t;
  }
}

void StopWatch::stopWatchReset() {
  stopWatchState = RESET;
  swStartTime = swStopTime = 0;
}

uint32_t StopWatch::stopWatchValue() {
  if (stopWatchState == RUNNING) swStopTime = millis();
  return swStopTime - swStartTime;
}

void StopWatch::stopWatchStop() {
  if (stopWatchState == RUNNING) {
     stopWatchState = STOPPED;
     swStopTime = millis();
  }
}
