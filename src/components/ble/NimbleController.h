#pragma once

#include <cstdint>

#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min
#include "components/ble/AlertNotificationClient.h"
#include "components/ble/AlertNotificationService.h"
#include "components/ble/BatteryInformationService.h"
#include "components/ble/CurrentTimeClient.h"
#include "components/ble/CurrentTimeService.h"
#include "components/ble/DeviceInformationService.h"
#include "components/ble/DfuService.h"
#include "components/ble/FSService.h"
#include "components/ble/HeartRateService.h"
#include "components/ble/ImmediateAlertService.h"
#include "components/ble/MusicService.h"
#include "components/ble/NavigationService.h"
#include "components/ble/ServiceDiscovery.h"
#include "components/ble/MotionService.h"
#include "components/ble/SimpleWeatherService.h"
#include "components/fs/FS.h"
#include "components/ble/ESPService.h"

namespace Pinetime {
  namespace Drivers {
    class SpiNorFlash;
  }

  namespace System {
    class SystemTask;
  }

  namespace Controllers {
    class Ble;
    class DateTime;
    class NotificationManager;

    class NimbleController {

    public:
      NimbleController(Pinetime::System::SystemTask& systemTask,
                       Ble& bleController,
                       DateTime& dateTimeController,
                       NotificationManager& notificationManager,
                       Battery& batteryController,
                       Pinetime::Drivers::SpiNorFlash& spiNorFlash,
                       HeartRateController& heartRateController,
                       MotionController& motionController,
                       FS& fs);
      void Init();
      void StartAdvertising();
      int OnGAPEvent(ble_gap_event* event);
      void StartDiscovery();

      Pinetime::Controllers::MusicService& music() {
        return musicService;
      };

      Pinetime::Controllers::NavigationService& navigation() {
        return navService;
      };

      Pinetime::Controllers::AlertNotificationService& alertService() {
        return anService;
      };

      Pinetime::Controllers::SimpleWeatherService& weather() {
        return weatherService;
      };

      Pinetime::Controllers::ESPService& esp() {
        return espService;
      };

      uint16_t connHandle();
      void NotifyBatteryLevel(uint8_t level);

      void RestartFastAdv() {
        fastAdvCount = 0;
      };

      void EnableRadio();
      void DisableRadio();

    private:
      void PersistBond(struct ble_gap_conn_desc& desc);
      void RestoreBond();

      static constexpr const char* deviceName = "InfiniTime";
      Pinetime::System::SystemTask& systemTask;
      Ble& bleController;
      DateTime& dateTimeController;
      Pinetime::Drivers::SpiNorFlash& spiNorFlash;
      FS& fs;
      DfuService dfuService;

      DeviceInformationService deviceInformationService;
      CurrentTimeClient currentTimeClient;
      AlertNotificationService anService;
      AlertNotificationClient alertNotificationClient;
      CurrentTimeService currentTimeService;
      MusicService musicService;
      SimpleWeatherService weatherService;
      NavigationService navService;
      BatteryInformationService batteryInformationService;
      ImmediateAlertService immediateAlertService;
      HeartRateService heartRateService;
      MotionService motionService;
      FSService fsService;
      ServiceDiscovery serviceDiscovery;
      ESPService espService;

      uint8_t addrType;
      uint16_t connectionHandle = BLE_HS_CONN_HANDLE_NONE;
      uint8_t fastAdvCount = 0;
      uint8_t bondId[16] = {0};

      ble_uuid128_t espServiceUuid {
        .u = {.type = BLE_UUID_TYPE_128},
        .value = {0x00, 0xc7, 0xc4, 0x4e, 0xe3, 0x6c, 0x51, 0xa7, 0x33, 0x4b, 0xe8, 0xed, 0x5a, 0x0e, 0xb8, 0x03}};
    };

    static NimbleController* nptr;
  }
}
