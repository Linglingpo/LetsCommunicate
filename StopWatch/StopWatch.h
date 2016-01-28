#ifndef STOPWATCH_H
#define STOPWATCH_H

/***********************************************************/
/* StopWatch Variables for Monitoring Time - DO NOT CHANGE */
/***********************************************************/
#define RESET   0
#define RUNNING 1
#define STOPPED 2

#define DEFAULTPERIOD 10 // 1000 = 1 Second

class StopWatch
{
  public:
    StopWatch() {  };
    unsigned long elapsed() { return stopWatchValue(); };

    void stopWatchStart() {
      if (stopWatchState == RESET || stopWatchState == STOPPED) {
         stopWatchState = RUNNING;
         unsigned long t = millis();
         swStartTime += t - swStopTime;
         swStopTime = t;
      }
    };

  private:
    byte stopWatchState;
    unsigned long swStartTime = 0UL;
    unsigned long swStopTime = 0UL;

    void stopWatchReset() {
      stopWatchState = RESET;
      swStartTime = swStopTime = 0;
    }

    unsigned long stopWatchValue() {
      if (stopWatchState == RUNNING) swStopTime = millis();
      return swStopTime - swStartTime;
    }

    void stopWatchStop() {
      if (stopWatchState == RUNNING) {
         stopWatchState = STOPPED;
         swStopTime = millis();
      }
    }
};
#endif // STOPWATCH_H
