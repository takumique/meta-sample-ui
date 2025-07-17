#include <Runner.hpp>

//#define RUNNER_DEBUG
#ifdef RUNNER_DEBUG
#include <log.h>
#define RUNNER_DEBUG_LOG(...) LOG(__VA_ARGS__);
#else
#define RUNNER_DEBUG_LOG(...)
#endif


#include <csignal>
#include <stdexcept>
#include <typeinfo>
#include <cxxabi.h>

Runner::Runner() : state(State::STOP), thread_return_promise(nullptr), runnable(nullptr) {
}

Runner::~Runner() {
  Stop();
}

Runner::State Runner::GetState() {
  return state;
}


bool Runner::Start(Runnable *runnable) {
  std::unique_lock<std::mutex> lock(state_mutex);
  switch(state) {
  case State::STOP:
    break;
  default:
    return false;
  }
  if(runnable == nullptr) {
    return false;
  }
  if(this->thread_return_promise != nullptr) {
    return false;
  }
  this->thread_return_promise = new std::promise<int>();
  this->thread_return_future = thread_return_promise->get_future();
  this->runnable = runnable;
  state = State::START;
  thread = std::thread([this]{
    // mask all signals
    sigset_t sigmask_block;
    sigfillset(&sigmask_block);
    pthread_sigmask(SIG_BLOCK, &sigmask_block, nullptr);
    // run
    int return_value = this->runnable->Run();
    thread_return_promise->set_value(return_value);
  });
  thread.detach();
  RUNNER_DEBUG_LOG("%s: runner started\n", this->runnable->runner_debug.c_str());
  return true;
}

void Runner::Stop(int *return_value) {
  std::unique_lock<std::mutex> lock(state_mutex);
  switch(state) {
  case State::START:
    break;
  default:
    return;
  }
  RUNNER_DEBUG_LOG("%s: Runner::Stop\n", this->runnable->runner_debug.c_str());
  state = State::STOP_PENDING;

  this->runnable->InterruptRunnable();
  unsigned int count = thread_return_timeout / 100;
  for(auto status = thread_return_future.wait_for(std::chrono::milliseconds(100));
      status == std::future_status::timeout;
      status = thread_return_future.wait_for(std::chrono::milliseconds(100))) {
    if(--count == 0) {
      state = State::STOP;
      throw std::runtime_error("Thread exit timeout : " + GetRunnableClassName());
    }
  }
  if(return_value != nullptr) {
    *return_value = thread_return_future.get();
  }
  delete this->thread_return_promise;
  this->thread_return_promise = nullptr;
  state = State::STOP;
  RUNNER_DEBUG_LOG("%s: runner stopped\n", this->runnable->runner_debug.c_str());
}

std::string Runner::GetRunnableClassName() {
  int status;
  const char *classname = abi::__cxa_demangle(typeid(*runnable).name(), 0, 0, &status);
  return std::string(classname);
}
