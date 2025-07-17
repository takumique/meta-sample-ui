#ifndef _SRC_USERINTERFACE_H_
#define _SRC_USERINTERFACE_H_

#include <string>
#include <mutex>

#include <Runner.hpp>
#include <Runnable.hpp>

#include <lvgl.h>
#include <display/fbdev.h>
#include <indev/evdev.h>

#include <View.hpp>

class UserInterface : public Runnable {
public:
  UserInterface(void);
  virtual ~UserInterface(void);
  int Start(void);
  void Stop(void);
  void InterruptRunnable(void);
  int Run(void);
protected:
  void SetView(View *view);
  std::mutex &GetDrawMutex(void);
private:
  Runner *runner;
  bool run;
  std::mutex draw_mutex;
  View *curr_view;
  // VT stuff
  int console_fd;
  unsigned short prev_active_vt;
  // LVGL stuff
  uint32_t disp_width;
  uint32_t disp_height;
  uint32_t disp_dpi;
  lv_color_t *disp_buf;
  lv_disp_draw_buf_t draw_buf;
  lv_disp_drv_t disp_drv;
  lv_disp_t *disp;
  const int lvgl_tick_period = 30;
  uint64_t lvgl_tick_prev;
  lv_indev_drv_t indev_drv;
  lv_indev_t *indev;
  // View
  void Initialize(void);
  void Deinitialize(void);
  void Draw(void);
};

#endif /* _SRC_USERINTERFACE_H_ */
