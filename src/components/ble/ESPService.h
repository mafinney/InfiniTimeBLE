#pragma once
#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min

#define KEY_REQ "KEY_REQ"

namespace Pinetime::System {
    class SystemTask;
}

namespace Pinetime::Controllers {
    class ESPService {
        public:
            ESPService(Pinetime::System::SystemTask& system);
            void Init();
            int OnBLEUpdate(ble_gatt_access_ctxt *context);
            void SendValue(uint8_t *data, int len); // Call this when a new value is ready to send
            void GetReadValue(uint8_t *data, int len); // If len is greater than BUFSIZ, this will fail
            
        private:
            Pinetime::System::SystemTask& espSystemTask;
            
            //  03B80E5A-EDE8-4B33-A751-6CE34EC4C700
            static constexpr ble_uuid128_t espServiceUuid {
                .u = {.type = BLE_UUID_TYPE_128},
                .value = {0x00, 0xc7, 0xc4, 0x4e, 0xe3, 0x6c, 0x51, 0xa7, 0x33, 0x4b, 0xe8, 0xed, 0x5a, 0x0e, 0xb8, 0x03}};
            
            // 7772E5DB-3868-4112-A1A9-F2669D106BF3
            static constexpr ble_uuid128_t espCharUuid {
                .u = {.type = BLE_UUID_TYPE_128},
                .value = {0xf3, 0x6b, 0x10, 0x9d, 0x66, 0xf2, 0xa9, 0xa1, 0x12, 0x41, 0x68, 0x38, 0xdb, 0xe5, 0x72, 0x77}};
            
            struct ble_gatt_chr_def charDef[2];
            struct ble_gatt_svc_def svcDef[2];
            uint16_t espCharHandle;

            uint8_t buf[BUFSIZ];
    };
}