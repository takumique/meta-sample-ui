#include "DefaultUserInterface.hpp"

#include <log.h>
#include "MessageParser.hpp"
using namespace msg;

DefaultUserInterface::DefaultUserInterface(void) : UserInterface() {
  default_view = new DefaultView(GetDrawMutex());
  SetView(default_view);
}

DefaultUserInterface::~DefaultUserInterface(void) {
  if(default_view) {
    delete default_view;
  }
}

void DefaultUserInterface::OnReceive(std::string &message_str) {
  LOG("DefaultUserInterface::OnReceive: %s\n", message_str.c_str());
  msg_t message;
  bool ret = MessageParser().Deserialize(message_str, message);
  if(message.wlan_state != "__na__") {
    default_view->SetWlanState(message.wlan_state);
  }
  if(message.api_state != "__na__") {
    default_view->SetApiState(message.api_state);
  }
  if(message.core_state != "__na__") {
    default_view->SetCoreState(message.core_state);
  }
  if(message.msg != "__na__") {
    default_view->SetMsg(message.msg);
  }
}
