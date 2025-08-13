#ifndef _SRC_RUNNABLE_H_
#define _SRC_RUNNABLE_H_

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

class Runnable {
public:
  Runnable(void) {};
  virtual ~Runnable(void) {};
  virtual void InterruptRunnable(void) = 0;
  virtual int Run(void) = 0;
  std::string runner_debug;
protected:
  void Sleep(unsigned timeout) {
    std::unique_lock<std::mutex> lock(runnable_mutex);
    runnable_cv.wait_for(lock, std::chrono::milliseconds(timeout));
  }
  void CancelSleep(void) {
    std::unique_lock<std::mutex> lock(runnable_mutex);
    runnable_cv.notify_one();
  }
  std::mutex runnable_mutex;
  std::condition_variable runnable_cv;
};

#endif /* _SRC_RUNNABLE_H_ */
