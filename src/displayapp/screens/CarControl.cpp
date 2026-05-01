#include "displayapp/screens/CarControl.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Symbols.h"
#include "components/ble/ESPService.h"

using namespace Pinetime::Applications::Screens;
using namespace Pinetime::Controllers;

// static void ButtonEvent(lv_obj_t *obj, lv_event_t event) {
//     auto *screen = static_cast<CarControl*>(obj->user_data);
//     screen->OnButtonEvent(obj, event);
// }

// void CarControl::OnButtonEvent(lv_obj_t *obj, lv_event_t event) {
//     if (event != LV_EVENT_CLICKED) {
//         if (obj == incr.button) {
//             lv_label_set_text_fmt(count_lbl, "%i", ++count);
//         } else if (obj == send.button) {
//             // uint8_t data[2];
//             // data[0] = count;
//             // data[1] = 40;
//             //espService.SendValue(data, 2);
//         }
//     }
// }

void CarControl::Refresh() {
    // uint8_t data[2];
    // data[0] = 0;
    // data[1] = 1;
    // // espService.GetReadValue(data, 2);
    // lv_label_set_text_fmt(read_lbl, "%i %i", data[0], data[1]);
}

CarControl::CarControl(Pinetime::Controllers::ESPService& espService, Pinetime::System::SystemTask& systemTask) : espService {espService}, systemTask {systemTask} {
    count = 0;
    // CreateButton(&incr, car_screen, ButtonEvent, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0, (char *) "Incr");
    CreateLabel(&count_lbl, car_screen, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT, LV_ALIGN_IN_TOP_LEFT, 0, 0, (char *) "lbl");
    // CreateButton(&send, car_screen, ButtonEvent, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0, (char *) "send");
    CreateLabel(&read_lbl, car_screen, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT, LV_ALIGN_IN_TOP_RIGHT, 0, 0, (char *) "read");
    lv_scr_load(car_screen);

    // refresh_task = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

CarControl::~CarControl() {
    // lv_task_del(refresh_task);
    lv_obj_clean(lv_scr_act());
}

/******HELPER FUNCTIONS******/

void CarControl::CreateButton(button *b, lv_obj_t *par, lv_event_cb_t event_cb, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, char *text) {
    b->button = lv_btn_create(par, nullptr);
    b->button->user_data = this;
    lv_obj_set_event_cb(b->button, event_cb);
    lv_obj_set_size(b->button, w, h);
    lv_obj_align(b->button, par, align, x_ofs, y_ofs);
    b->label = lv_label_create(b->button, nullptr);
    lv_label_set_text_static(b->label, text);
}

void CarControl::CreateLabel(lv_obj_t **l, lv_obj_t *par, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, char *text) {
    *l = lv_label_create(par, nullptr);
    lv_obj_set_size(*l, w, h);
    lv_obj_align(*l, par, align, x_ofs, y_ofs);
    lv_label_set_text_static(*l, text);
}

// void CarControl::CreateSwitch(lv_obj_t *s, lv_obj_t *par, lv_event_cb_t event_cb, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) {
//     s = lv_switch_create(par, nullptr);
//     s->user_data = this;
//     lv_obj_set_event_cb(s, event_cb);
//     lv_obj_set_size(s, w, h);
//     lv_obj_align(s, par, align, x_ofs, y_ofs);
//     lv_obj_set_style_local_bg_color(s, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, Colors::bgAlt);
// }