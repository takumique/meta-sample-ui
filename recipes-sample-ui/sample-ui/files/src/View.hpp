#ifndef _SRC_VIEW_H_
#define _SRC_VIEW_H_

#include <inttypes.h>

#include <lvgl.h>

class View {
public:
  View() {};
  virtual ~View() {};
  virtual void Initialize(uint32_t width, uint32_t height, uint32_t dpi) = 0;
  virtual void OnDraw() = 0;
};

#endif /* _SRC_VIEW_H_ */
