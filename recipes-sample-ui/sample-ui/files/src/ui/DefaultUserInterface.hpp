#ifndef _SRC_UI_DEFAULTUSERINTERFACE_H_
#define _SRC_UI_DEFAULTUSERINTERFACE_H_

#include <UserInterface.hpp>
#include "DefaultView.hpp"

#include <MessageReceivable.hpp>

class DefaultUserInterface : public UserInterface, public MessageReceivable {
public:
  DefaultUserInterface(void);
  virtual ~DefaultUserInterface(void);
  void OnReceive(std::string &message);
private:
  DefaultView *default_view;
};

#endif /* _SRC_UI_DEFAULTUSERINTERFACE_H_ */
