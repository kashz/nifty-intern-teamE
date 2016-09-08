#ifndef MODEDEVICE_H
#define MODEDEVICE_H

#include <WebSocketsClient.h>
#include <ESP8266WiFi.h>

#define USE_SERIAL Serial1

#define DEBUG

#ifdef DEBUG
#define debug_printf Serial.printf
#else
#define debug_printf 1 ? (void) 0 : Serial.printf
#endif

class DeviceHubClient {
public:
    DeviceHubClient();
    DeviceHubClient(uint16_t deviceId, String apiKey);

    void setModeInfo(uint16_t deviceId, String apiKey);

    void listenCommand();
    void triggerEvent(String eventType, String eventData);

    static bool sFlagListeningCommand;
private:
    const String mHost = "iot-device.jp-east-1.api.cloud.nifty.com";
    //const String mHost = "api.tinkermode.com";
    uint16_t mDeviceId;
    String mApiKey;

    WebSocketsClient mWebSocket;
};

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

#endif
