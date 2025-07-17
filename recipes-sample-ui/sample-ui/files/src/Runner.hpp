#ifndef _SRC_RUNNER_H_
#define _SRC_RUNNER_H_

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include <Runnable.hpp>

class Runner {
public:
  Runner();
  virtual ~Runner();
  enum class State : int {
    START,
    STOP_PENDING,
    STOP,
  };
  State GetState();
  bool Start(Runnable *runnable);
  void Stop(int *return_value = nullptr);
private:
  State state;
  std::mutex state_mutex;
  std::thread thread;
  std::promise<int> *thread_return_promise;
  std::future<int> thread_return_future;
  const unsigned thread_return_timeout = 10000;
  Runnable *runnable;
  std::string GetRunnableClassName();
};

#endif /* _SRC_RUNNER_H_ */
