#include "displayapp/screens/ESP.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Symbols.h"
#include "components/ble/ESPService.h"

using namespace Pinetime::Applications::Screens;
using namespace Pinetime::Controllers;

uint8_t DOOR = 2;
uint8_t WINDOW = 3;
uint8_t UNLOCK = 4;
uint8_t LOCK = 5;
uint8_t DOWN = 6;
uint8_t UP = 7;

static void ButtonEvent(lv_obj_t *obj, lv_event_t event) {
    auto *screen = static_cast<ESP*>(obj->user_data);
    screen->OnButtonEvent(obj, event);
}

void ESP::OnButtonEvent(lv_obj_t *obj, lv_event_t event) {
    if (event != LV_EVENT_CLICKED) {
        return ;
    }
    espService.GetReadValue(read_data, 2);
    char data[2];

    if (obj == wrx.door_b.button) {
        data[0] = DOOR;
        if (read_data[0] == UNLOCK) {
            data[1] = LOCK;
        } else if (read_data[0] == LOCK) {
            data[1] = UNLOCK;
        }
    } else if (obj == wrx.window_b.button) {
        data[0] = WINDOW;
        if (read_data[1] == DOWN) {
            data[1] = UP;
        } else if (read_data[1] == UP) {
            data[1] = DOWN;
        }
    } else if (obj == wrx.auto_t) {
        // nothing yet
    }
    espService.SendValue(data, 2);
}

void ESP::Refresh() {
    espService.GetReadValue(read_data, 2);
    lv_label_set_text_fmt(wrx.door_l, "[0]: %i", read_data[0]);
    lv_label_set_text_fmt(wrx.window_l, "[1]:%i", read_data[1]);
}

ESP::ESP(Pinetime::Controllers::ESPService& espService, Pinetime::System::SystemTask& systemTask) : espService {espService}, systemTask {systemTask} {
    refresh_task = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
    init_car(&wrx);
    lv_scr_load(wrx.main_scr);
}

ESP::~ESP() {
    lv_task_del(refresh_task);
    lv_obj_clean(lv_scr_act());
}

/*-------------CAR SETUP-------------*/

void ESP::init_car(car *c) {
    c->main_scr = lv_obj_create(NULL, NULL);
    c->debug_scr = lv_obj_create(NULL, NULL);
    CreateButton(&(c->door_b), c->main_scr, ButtonEvent, MED_BUTTON_WIDTH, MED_BUTTON_HEIGHT, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0, (char *) "Doors");
    CreateButton(&(c->window_b), c->main_scr, ButtonEvent, MED_BUTTON_WIDTH, MED_BUTTON_HEIGHT, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0, (char *) "Windows");
    CreateLabel(&(c->door_l), c->main_scr, MED_BUTTON_WIDTH, MED_BUTTON_HEIGHT, LV_ALIGN_IN_LEFT_MID, 0, 0, (char *) "?");
    CreateLabel(&(c->window_l), c->main_scr, MED_BUTTON_WIDTH, MED_BUTTON_HEIGHT, LV_ALIGN_IN_RIGHT_MID, 0, 0, (char *) "?");
    CreateSwitch(c->auto_t, c->main_scr, ButtonEvent, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    CreateLabel(&(c->connected_t), c->main_scr, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT, LV_ALIGN_IN_TOP_RIGHT, 0, 0, (char *) Symbols::moon);
}

/******HELPER FUNCTIONS******/

void ESP::CreateButton(button *b, lv_obj_t *par, lv_event_cb_t event_cb, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, char *text) {
    b->button = lv_btn_create(par, nullptr);
    b->button->user_data = this;
    lv_obj_set_event_cb(b->button, event_cb);
    lv_obj_set_size(b->button, w, h);
    lv_obj_align(b->button, par, align, x_ofs, y_ofs);
    b->label = lv_label_create(b->button, nullptr);
    lv_label_set_text_static(b->label, text);
}

void ESP::CreateLabel(lv_obj_t **l, lv_obj_t *par, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, char *text) {
    *l = lv_label_create(par, nullptr);
    lv_obj_set_size(*l, w, h);
    lv_obj_align(*l, par, align, x_ofs, y_ofs);
    lv_label_set_text_static(*l, text);
}

void ESP::CreateSwitch(lv_obj_t *s, lv_obj_t *par, lv_event_cb_t event_cb, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) {
    s = lv_label_create(par, nullptr);
    s->user_data = this;
    lv_obj_set_event_cb(s, event_cb);
    lv_obj_set_size(s, w, h);
    lv_obj_align(s, par, align, x_ofs, y_ofs);
}