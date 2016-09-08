#include "DeviceHubClient.h"

bool DeviceHubClient::sFlagListeningCommand = false;

DeviceHubClient::DeviceHubClient(){
    sFlagListeningCommand = false;
}

DeviceHubClient::DeviceHubClient(uint16_t deviceId, String apiKey){
    mDeviceId = deviceId;
    mApiKey = apiKey;
    sFlagListeningCommand = false;
}

void DeviceHubClient::setModeInfo(uint16_t deviceId, String apiKey){
    mDeviceId = deviceId;
    mApiKey = apiKey;
}

void DeviceHubClient::listenCommand() {
    String auth = "ModeCloud " + String(mApiKey);
    String path = "/devices/" + String(mDeviceId) + "/command";


    if (sFlagListeningCommand){
        mWebSocket.loop();
	    return;
    }

    mWebSocket.beginSSL(mHost, 443, path.c_str());
    mWebSocket.setAuthorization(auth.c_str());
    mWebSocket.onEvent(webSocketEvent);

    sFlagListeningCommand = true;
}

void DeviceHubClient::triggerEvent(String eventType, String eventData) {
    WiFiClient client;
    String url = "/devices/" + String(mDeviceId) + "/event";

    if (!client.connect(mHost.c_str(), 80)) {
        Serial.println("connection failed");
        return;
    } 

    String body = "{\"eventType\": \"" + eventType + "\"," +
                  "\"eventData\": " + eventData + "}";

    String payload = "PUT " + url + " HTTP/1.1\r\n" +
                     "Host: " + mHost + "\r\n" + 
                     "Content-Type: application/json\r\n" +
                     "Content-Length: " + body.length() + "\r\n" +
                     "Authorization: ModeCloud " + mApiKey + "\r\n\r\n" +
                     body;

    client.print(payload.c_str());
    client.stop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      debug_printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      debug_printf("[WSc] Connected to url: %s\n",  payload);
      break;
    case WStype_TEXT:
      debug_printf("[WSc] get text: %s\n", payload);
      break;
  }
}
