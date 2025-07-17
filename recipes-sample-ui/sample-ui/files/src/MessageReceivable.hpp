#ifndef _SRC_MESSAGERECEIVABLE_H_
#define _SRC_MESSAGERECEIVABLE_H_

#include <string>

class MessageReceivable {
public:
  virtual void OnReceive(std::string &message) = 0;
};

#endif /* _SRC_MESSAGERECEIVABLE_H_ */
