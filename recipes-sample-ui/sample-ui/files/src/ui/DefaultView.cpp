#include "DefaultView.hpp"

#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include <log.h>

namespace
{
  void event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    // if(code < 18) {
    //     LOG("event_handler: %02x\n", code);
    // }
    if(code == LV_EVENT_CLICKED) {
        kill(getpid(), SIGINT);
    }
  }
}

DefaultView::DefaultView(std::mutex &draw_mutex) : draw_mutex(draw_mutex) {
}

DefaultView::~DefaultView(void) {
}

void DefaultView::Initialize(uint32_t width, uint32_t height, uint32_t dpi) {
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), LV_PART_MAIN);

  // header

  // header container
  lv_obj_t * container_header = lv_obj_create(lv_scr_act());
  lv_obj_set_size(container_header, width, LV_SIZE_CONTENT);
  lv_obj_align(container_header, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_flex_flow(container_header, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(container_header, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_border_color(container_header, lv_color_white(), LV_PART_MAIN);


  label_clock = lv_label_create(container_header);
  // lv_obj_set_style_text_font(label_clock, &lv_font_montserrat_24, 0);
  lv_label_set_text(label_clock, "");

  label_connectivity = lv_label_create(container_header);
  lv_label_set_text(label_connectivity, "");

  lv_obj_t *shutdown_btn = lv_btn_create(container_header);
  lv_obj_add_event_cb(shutdown_btn, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_t *shutdown_label = lv_label_create(shutdown_btn);
  // lv_obj_set_style_text_font(shutdown_label, &lv_font_montserrat_24, 0);
  lv_label_set_text(shutdown_label, "Shutdown");
  lv_obj_center(shutdown_label);

  // content

  // content container
  lv_obj_t * container_content = lv_obj_create(lv_scr_act());
  lv_obj_set_size(container_content, width, LV_SIZE_CONTENT);
  lv_obj_align_to(container_content, container_header, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_obj_set_flex_flow(container_content, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(container_content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_border_width(container_content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(container_content, lv_color_white(), LV_PART_MAIN);

  // raw message
  label_msg = lv_label_create(container_content);
  lv_label_set_text(label_msg, "");
}

void DefaultView::OnDraw(void) {
  UpdateClock();
}

void DefaultView::UpdateClock(void) {
  std::unique_lock<std::mutex> draw_lock(draw_mutex);
  struct timespec ts;
  if(clock_gettime(CLOCK_REALTIME, &ts) < 0) {
    return;
  }
  struct tm gt;
  if(gmtime_r(&ts.tv_sec, &gt) == nullptr) {
    return;
  }
  char clock[64];
  snprintf(clock, 64, "%02u-%02u-%04u %02u:%02u:%02u (UTC)",
    gt.tm_mon + 1,
    gt.tm_mday,
    gt.tm_year + 1900,
    gt.tm_hour,
    gt.tm_min,
    gt.tm_sec);
  lv_label_set_text(label_clock, clock);
}

void DefaultView::SetWlanState(std::string &wlan_state) {
  lv_label_set_text(label_connectivity, wlan_state.c_str());
}

void DefaultView::SetMsg(std::string &msg) {
  lv_label_set_text(label_msg, msg.c_str());
}
