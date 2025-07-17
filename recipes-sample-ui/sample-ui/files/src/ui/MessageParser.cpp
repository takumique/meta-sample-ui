#include "MessageParser.hpp"

#include <log.h>

namespace msg {

  void to_json(json &j, const msg_t &m) {
    // TODO
  }

  void from_json(const json &j, msg_t &m) {
    try {
      j.at("wlan").get_to(m.wlan_state);
    } catch(std::exception &e) {
      m.wlan_state = "__na__";
    }
    try {
      j.at("msg").get_to(m.msg);
    } catch(std::exception &e) {
      m.msg = "__na__";
    }
  }

  MessageParser::MessageParser(void) {
  }

  MessageParser::~MessageParser(void) {
  }

  bool MessageParser::Deserialize(std::string &str, msg_t &msg) {
    try {
      json j = json::parse(str);
      msg = j.get<msg_t>();
    } catch(std::exception &e) {
      return false;
    }
    return true;
  }
}
