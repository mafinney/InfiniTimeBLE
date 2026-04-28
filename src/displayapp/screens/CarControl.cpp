#include "displayapp/screens/CarControl.h"
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
    auto *screen = static_cast<CarControl*>(obj->user_data);
    screen->OnButtonEvent(obj, event);
}

void CarControl::OnButtonEvent(lv_obj_t *obj, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        if (obj == send.button) {
            uint8_t data[2] = {1, 2};
            car->SendData(data, 2);
        }
    }
}

void CarControl::Refresh() {
    uint8_t data[2];
    car->ReadData(data, 2);
    lv_label_set_text_fmt(read, "%i %i", data[0], data[1]);
}

CarControl::CarControl(Pinetime::Controllers::ESPService& espService, Pinetime::System::SystemTask& systemTask) : espService {espService}, systemTask {systemTask} {
    car = new Car(espService);
    
    CreateButton(&send, car_screen, ButtonEvent, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0, (char *) "Send");
    CreateLabel(&read, car_screen, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT, LV_ALIGN_IN_TOP_LEFT, 0, 0, (char *) "//");

    refresh_task = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

CarControl::~CarControl() {
    lv_task_del(refresh_task);
    lv_obj_clean(lv_scr_act());
}

Car::Car(Pinetime::Controllers::ESPService& espService) : espService {espService} {
    ReadData(states, 2);
}

Car::~Car() {
    // Do nothing?
}

/**
 * Each action method has the following format
 * Send message byte array
 * Read status
 * Return the state of the action taken. (Ex, if a Door action is called, return the state of the physical doors)
 */

uint8_t Car::LockDoors() {
    // Send lock byte
    uint8_t data[2] = { LOCK, states[1] };
    SendData(data, 2);
    ReadData(states, 2);
    return states[0];
}

uint8_t Car::UnlockDoors() {
    // Send unlock byte
    uint8_t data[2] = { UNLOCK, states[1] };
    SendData(data, 2);
    ReadData(states, 2);
    return states[0];
}

uint8_t Car::RollDownWindows() {
    // Send down byte
    uint8_t data[2] = { states[0], DOWN };
    SendData(data, 2);
    ReadData(states, 2);
    return states[1];
}

uint8_t Car::RollUpWindows() {
    // Send up byte
    uint8_t data[2] = { states[0], UP };
    SendData(data, 2);
    ReadData(states, 2);
    return states[1];
}

uint8_t Car::GetDoorState() {
    ReadData(states, 2);
    return states[0];
}

uint8_t Car::GetWindowState() {
    ReadData(states, 2);
    return states[1];
}

void Car::SendData(uint8_t *data, int len) {
    espService.SendValue(data, len);
}

void Car::ReadData(uint8_t *data, int len) {
    if (len < BUFSIZ) {
        espService.GetReadValue(data, len);    
    }
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

void CarControl::CreateSwitch(lv_obj_t *s, lv_obj_t *par, lv_event_cb_t event_cb, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) {
    s = lv_switch_create(par, nullptr);
    s->user_data = this;
    lv_obj_set_event_cb(s, event_cb);
    lv_obj_set_size(s, w, h);
    lv_obj_align(s, par, align, x_ofs, y_ofs);
    lv_obj_set_style_local_bg_color(s, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, Colors::bgAlt);
}