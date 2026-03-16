#include "components/ble/ESPService.h"
#include "systemtask/SystemTask.h"

using namespace Pinetime::Controllers;

constexpr ble_uuid128_t ESPService::espServiceUuid;
constexpr ble_uuid128_t ESPService::espCharUuid;

int ESPServiceCallback(uint16_t /*conn_handle*/, uint16_t /*attr_handle*/, struct ble_gatt_access_ctxt *ctxt, void *arg) {
    return static_cast<ESPService*>(arg)->OnBLEUpdate(ctxt);
}

ESPService::ESPService(Pinetime::System::SystemTask& system)
    : espSystemTask {system} {
        charDef[0] = {.uuid = &espCharUuid.u,
                      .access_cb = ESPServiceCallback,
                      .arg = this,
                      //.flags = BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE_NO_RSP,
                      .flags = BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_READ_ENC | BLE_GATT_CHR_F_WRITE_ENC | BLE_GATT_CHR_F_WRITE_NO_RSP,
                      //.flags = BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_READ_ENC | BLE_GATT_CHR_F_WRITE_ENC | BLE_GATT_CHR_F_WRITE_NO_RSP | BLE_GATT_CHR_F_READ_AUTHEN | BLE_GATT_CHR_F_WRITE_AUTHEN,
                      .val_handle = &espCharHandle};
        charDef[1] = {0};

        svcDef[0] = {.type = BLE_GATT_SVC_TYPE_PRIMARY,
                     .uuid = &espServiceUuid.u,
                     .characteristics = charDef};
        svcDef[1] = {0};
}

void ESPService::Init() {
    int res = 0;
    res = ble_gatts_count_cfg(svcDef);
    ASSERT(res == 0);

    res = ble_gatts_add_svcs(svcDef);
    ASSERT(res == 0);
}

int ESPService::OnBLEUpdate(struct ble_gatt_access_ctxt *ctxt) {
    if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
        size_t notifSize = OS_MBUF_PKTLEN(ctxt->om);
        size_t bufferSize = notifSize;

        char tmp[bufferSize + 1];
        os_mbuf_copydata(ctxt->om, 0, bufferSize, tmp);
        for (int i = 0; i < 2; i++) {
            read_data[i] = tmp[i];
        }
    }
    return 0;
}

void ESPService::SendValue(char *data, int len) {
    auto *om = ble_hs_mbuf_from_flat(data, len);

    uint16_t connectionHandle = espSystemTask.nimble().connHandle();

    if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) {
        return ;
    }

    ble_gattc_notify_custom(connectionHandle, espCharHandle, om);
}

void ESPService::GetReadValue(uint8_t *buf, int len) {
    for (int i = 0; i < len; i++) {
        buf[i] = (uint8_t) read_data[i];
    }
}