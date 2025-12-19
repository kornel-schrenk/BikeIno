#include "streamback/StreamBackClient.h"
#include <ArduinoJson.h>

StreamBackClient* StreamBackClient::_instance = nullptr;

StreamBackClient::StreamBackClient() : _mqttClient(_wifiClient) {
    _instance = this;
    _lastReconnectAttempt = 0;
    _ssid = nullptr;
    _password = nullptr;
    _broker = nullptr;
    _port = 1883;
    _topic = nullptr;
}

void StreamBackClient::begin(const char* ssid, const char* password,
                             const char* broker, int port, const char* topic) {
    _ssid = ssid;
    _password = password;
    _broker = broker;
    _port = port;
    _topic = topic;

    _mqttClient.setServer(broker, port);
    _mqttClient.setCallback(_mqttCallback);

    _connectWiFi();
}

void StreamBackClient::loop() {
    // Handle WiFi reconnection
    if (WiFi.status() != WL_CONNECTED) {
        unsigned long now = millis();
        if (now - _lastReconnectAttempt > RECONNECT_INTERVAL) {
            _lastReconnectAttempt = now;
            _connectWiFi();
        }
        return;
    }

    // Handle MQTT reconnection
    if (!_mqttClient.connected()) {
        unsigned long now = millis();
        if (now - _lastReconnectAttempt > RECONNECT_INTERVAL) {
            _lastReconnectAttempt = now;
            _connectMqtt();
        }
    } else {
        _mqttClient.loop();
    }

    // Check data timeout
    if (_data.isValid && (millis() - _data.lastReceived > DATA_TIMEOUT)) {
        _data.isValid = false;
        Serial.println(F("StreamBack: Data timeout"));
    }
}

StreamBackData StreamBackClient::getData() {
    return _data;
}

bool StreamBackClient::isDataValid() {
    return _data.isValid && (millis() - _data.lastReceived < DATA_TIMEOUT);
}

bool StreamBackClient::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool StreamBackClient::isMqttConnected() {
    return _mqttClient.connected();
}

int StreamBackClient::getConnectionState() {
    if (!isWiFiConnected()) return 0;
    if (!isMqttConnected()) return 1;
    if (!isDataValid()) return 2;
    return 3;
}

void StreamBackClient::_connectWiFi() {
    if (_ssid == nullptr) return;

    Serial.print(F("StreamBack: Connecting to WiFi: "));
    Serial.println(_ssid);

    WiFi.begin(_ssid, _password);

    // Non-blocking - check status in loop()
}

void StreamBackClient::_connectMqtt() {
    if (_broker == nullptr) return;

    Serial.print(F("StreamBack: Connecting to MQTT broker: "));
    Serial.println(_broker);

    if (_mqttClient.connect("BikeIno")) {
        Serial.println(F("StreamBack: MQTT connected"));
        _mqttClient.subscribe(_topic);
        Serial.print(F("StreamBack: Subscribed to: "));
        Serial.println(_topic);
    } else {
        Serial.print(F("StreamBack: MQTT connection failed, rc="));
        Serial.println(_mqttClient.state());
    }
}

void StreamBackClient::_mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (_instance != nullptr) {
        _instance->_parseMessage(payload, length);
    }
}

void StreamBackClient::_parseMessage(byte* payload, unsigned int length) {
    // Parse JSON: {"lat":47.123,"lng":8.456,"speed":25.5,"alt":450.0,"ts":1702569600000,"duration":3600000}
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {
        Serial.print(F("StreamBack: JSON parse error: "));
        Serial.println(error.c_str());
        return;
    }

    _data.latitude = doc["lat"] | 0.0;
    _data.longitude = doc["lng"] | 0.0;
    _data.speed = doc["speed"] | 0.0;
    _data.altitude = doc["alt"] | 0.0;
    _data.timestamp = doc["ts"] | 0UL;
    _data.duration = doc["duration"] | 0UL;
    _data.lastReceived = millis();
    _data.isValid = true;

    Serial.printf("StreamBack: speed=%.1f lat=%.6f lng=%.6f duration=%lu\n",
                  _data.speed, _data.latitude, _data.longitude, _data.duration);
}