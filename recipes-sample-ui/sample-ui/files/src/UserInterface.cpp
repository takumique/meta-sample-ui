#include "UserInterface.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <time.h>

#include <log.h>

UserInterface::UserInterface(void) {
  run = false;
  runner = new Runner();
  curr_view = nullptr;
  console_fd = -1;
  Initialize();
}

UserInterface::~UserInterface(void) {
  Stop();
  delete runner;
  Deinitialize();
}

int UserInterface::Start(void) {
  if(!run) {
    run = true;
    runner->Start(this);
  }
  return 0;
}

void UserInterface::Stop(void) {
  if(run) {
    runner->Stop();
  }
}

int UserInterface::Run(void) {
  while(run) {
    if(curr_view) {
      curr_view->OnDraw();
    }
    Draw();
    Sleep(lvgl_tick_period);
  }
  return 0;
}

void UserInterface::InterruptRunnable(void) {
  run = false;
  CancelSleep();
}

void UserInterface::Initialize(void) {
  LOG("UserInterface::Initialize\n");
  // console
  const std::string default_console_device = "/dev/tty1";
  int ret, fd;
  int32_t vt_num;
  struct vt_stat vts;
  fd = open(default_console_device.c_str(), O_WRONLY);
  if(fd < 0) {
    ERR("Failed to open console device %s\n", default_console_device.c_str());
    return;
  }
  ret = ioctl(fd, VT_OPENQRY, &vt_num);
  if(ret < 0) {
    ERR("VT_OPENQRY failed %d\n", ret);
    return;
  }
  close(fd);
  // open VT and activate
  std::string console_device = "/dev/tty" + std::to_string(vt_num);;
  LOG("console: %s\n", console_device.c_str());
  console_fd = open(console_device.c_str(), O_RDWR | O_NDELAY);
  if(console_fd < 0) {
    ERR("Failed to open console device %s\n", console_device.c_str());
    return;
  }
  ret = ioctl(console_fd, VT_GETSTATE, &vts);
  if(ret < 0) {
    ERR("VT_GETSTATE failed %d\n", ret);
    close(console_fd);
    console_fd = -1;
    return;
  }
  prev_active_vt = vts.v_active;
  ret = ioctl(console_fd, VT_ACTIVATE, vt_num);
  if(ret < 0) {
    ERR("VT_ACTIVATE failed %d\n", ret);
    close(console_fd);
    console_fd = -1;
    return;
  }
  ret = ioctl(console_fd, VT_WAITACTIVE, vt_num);
  if(ret < 0) {
    ERR("VT_WAITACTIVE failed %d\n", ret);
    close(console_fd);
    console_fd = -1;
    return;
  }
  // Set active VT to graphics mode
  ret = ioctl(console_fd, KDSETMODE, KD_GRAPHICS);
  if(ret < 0) {
    ERR("KDSETMODE failed %d\n", ret);
    ioctl(console_fd, VT_ACTIVATE, prev_active_vt);
    close(console_fd);
    console_fd = -1;
    return;
  }
  // Frame buffer
  lv_init();
  fbdev_init();
  fbdev_get_sizes(&disp_width, &disp_height, &disp_dpi);
  disp_buf = (lv_color_t *) malloc(sizeof(lv_color_t) * disp_width * disp_height);
  if(disp_buf == nullptr) {
    ERR("Failed to alocate display buffer\n");
  }
  lv_disp_draw_buf_init(&draw_buf, disp_buf, NULL, disp_width * disp_height);
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &draw_buf;
  disp_drv.flush_cb = fbdev_flush;
  disp_drv.hor_res = disp_width;
  disp_drv.ver_res = disp_height;
  disp_drv.dpi = disp_dpi;
  disp = lv_disp_drv_register(&disp_drv);
  // Input device
  evdev_init();
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = evdev_read;
  indev = lv_indev_drv_register(&indev_drv);
}

void UserInterface::Deinitialize(void) {
  LOG("UserInterface::Deinitialize\n");
  // Input device
  lv_indev_delete(indev);
  // Frame buffer
  lv_disp_remove(disp);
  fbdev_exit();
  lv_deinit();
  free(disp_buf);
  // Deactivate and close VT
  if(console_fd >= 0) {
    int ret;
    ret = ioctl(console_fd, KDSETMODE, KD_TEXT);
    if(ret < 0) {
      ERR("KDSETMODE failed %d\n", ret);
    }
    ret = ioctl(console_fd, VT_ACTIVATE, prev_active_vt);
    if(ret < 0) {
      ERR("KDSETMODE failed %d\n", ret);
    }
    close(console_fd);
  }
}

void UserInterface::Draw(void) {
  std::unique_lock<std::mutex> draw_lock(draw_mutex);
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  uint64_t curr = (uint64_t) ts.tv_sec * 1000000000 + (uint64_t) ts.tv_nsec;
  if(lvgl_tick_prev < 0) {
    lvgl_tick_prev = curr;
  } else {
    uint32_t actual_tick_period = (curr - lvgl_tick_prev) / 1000000;
    lv_tick_inc(actual_tick_period);
  }
  lv_task_handler();
}

void UserInterface::SetView(View *view) {
  curr_view = view;
  curr_view->Initialize(disp_width, disp_height, disp_dpi);
}

std::mutex &UserInterface::GetDrawMutex(void) {
  return draw_mutex;
}
