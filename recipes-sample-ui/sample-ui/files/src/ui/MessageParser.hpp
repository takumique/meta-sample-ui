#ifndef _SRC_UI_MESSAGEPARSER_H_
#define _SRC_UI_MESSAGEPARSER_H_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace msg {

  typedef struct {
    std::string wlan_state;
    std::string api_state;
    std::string core_state;
    std::string msg;
  } msg_t;

  class MessageParser {
    public:
    MessageParser(void);
    virtual ~MessageParser(void);
    bool Deserialize(std::string &str, msg_t &msg);
  };
}

#endif /* _SRC_UI_MESSAGEPARSER_H_ */
