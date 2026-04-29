#pragma once

#include "systemtask/SystemTask.h"
#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/Controllers.h"
#include "Symbols.h"
// #include "components/ble/NimbleController.h"

namespace Pinetime::Controllers {
    class ESPService;
}

namespace Pinetime::Applications::Screens {
    class CarControl : public Screen {
        public:
            CarControl(Pinetime::Controllers::ESPService& espService, Pinetime::System::SystemTask& systemTask);
            ~CarControl() override;
            void OnButtonEvent(lv_obj_t *obj, lv_event_t event);
            void Refresh() override;

        private:
            static constexpr uint8_t MED_BUTTON_WIDTH = 115;
            static constexpr uint8_t MED_BUTTON_HEIGHT = 80;
            static constexpr uint8_t SMALL_BUTTON_WIDTH = 80;
            static constexpr uint8_t SMALL_BUTTON_HEIGHT = 50;

            Pinetime::Controllers::ESPService& espService;
            Pinetime::System::SystemTask& systemTask;
            lv_task_t* refresh_task;

            // Each button is made of two lv objects, a button and a label
            struct button {
                lv_obj_t *button;
                lv_obj_t *label;
            };

            lv_obj_t *car_screen;
            button incr, send;
            lv_obj_t *count_lbl, *read_lbl;
            int count;

            /*HELPER FUNCTIONS*/

            /**
             * CreateButton is a wrapper function for all the calls needed to create a button struct object
             * Takes a pointer to a button
             * A parent screen
             * A function callback
             * A size
             * An alignment
             * An initial label
             */
            void CreateButton(button *b, lv_obj_t *par, lv_event_cb_t event_cb, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, char *text);

            /**
             * CreateLabel is a wrapper function for all the calls needed to create a label
             * Takes a pointer to a label
             * A parent screen
             * A size
             * An alignment
             * An initial label
             */
            void CreateLabel(lv_obj_t **l, lv_obj_t *par, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, char *text);

            /**
             * CreateSwitch is a wrapper function for all the calls needed to create a switch
             * Takes a pointer to a switch
             * A parent screen
             * A function callback
             * A size
             * An alignment
             */
            void CreateSwitch(lv_obj_t *s, lv_obj_t *par, lv_event_cb_t event_cb, uint8_t w, uint8_t h, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
    };
}

namespace Pinetime::Applications {
    template <>
    struct AppTraits<Apps::CarControl> {
        static constexpr Apps app = Apps::CarControl;
        static constexpr const char *icon = "ES";

        static Screens::Screen *Create(AppControllers &controllers) {
            auto& esp = controllers.systemTask->nimble().esp();
            return new Screens::CarControl(esp, *controllers.systemTask);
        };

        static bool IsAvailable(Pinetime::Controllers::FS &) {
            return true;
        };
    };
}