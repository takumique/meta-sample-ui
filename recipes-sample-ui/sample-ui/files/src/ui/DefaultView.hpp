#ifndef _SRC_UI_DEFAULTVIEW_H_
#define _SRC_UI_DEFAULTVIEW_H_

#include <string>
#include <mutex>

#include <View.hpp>

class DefaultView : public View {
public:
  DefaultView(std::mutex &draw_mutex);
  virtual ~DefaultView(void);
  void Initialize(uint32_t width, uint32_t height, uint32_t dpi);
  void OnDraw(void);
  void SetWlanState(std::string &wlan_state);
  void SetMsg(std::string &msg);
private:
  std::mutex &draw_mutex;
  lv_obj_t *label_clock;
  lv_obj_t *label_connectivity;
  lv_obj_t *label_msg;
  void UpdateClock(void);
};

#endif /* _SRC_UI_DEFAULTVIEW_H_ */
