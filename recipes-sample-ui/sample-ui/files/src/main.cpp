#include <csignal>

#include <DefaultUserInterface.hpp>

#include <Messaging.hpp>

namespace {
  bool interrupted = false;
  bool shutdown = false;
  DefaultUserInterface *ui;
  Messaging *msg;
} /* namespace anonymous */

static void BlockSignal(void) {
  sigset_t sigmask;
  sigfillset(&sigmask);
  pthread_sigmask(SIG_BLOCK, &sigmask, nullptr);
}

static void WaitSignal() {
  sigset_t sigmask;
  siginfo_t siginfo;
  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGINT);
  sigaddset(&sigmask, SIGTERM);
  int sig = sigwaitinfo(&sigmask, &siginfo);
  switch(sig) {
  case SIGINT:
    shutdown = true;
    // fall down
  case SIGTERM:
    interrupted = true;
    break;
  }
}

extern "C" int main(int argc, char** argv) {
  BlockSignal();

  ui = new DefaultUserInterface();
  ui->Start();

  msg = new Messaging();
  msg->RegisterReceiver(ui);
  msg->Start();

  while(!interrupted) {
    WaitSignal();
  }

  msg->Stop();
  delete msg;

  ui->Stop();
  delete ui;

  if(shutdown) {
    exit(130);
  }
  exit(EXIT_SUCCESS);
}
