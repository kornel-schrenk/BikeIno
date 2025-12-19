#ifndef StreamBackClient_h
#define StreamBackClient_h

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "BikeInoBase.h"

class StreamBackClient {
public:
    StreamBackClient();

    // Initialize WiFi and MQTT
    void begin(const char* ssid, const char* password,
               const char* broker, int port, const char* topic);

    // Call in loop() to maintain connection and process messages
    void loop();

    // Get current StreamBack data
    StreamBackData getData();

    // Check if data is valid (received within timeout)
    bool isDataValid();

    // Check if connected to WiFi
    bool isWiFiConnected();

    // Check if connected to MQTT broker
    bool isMqttConnected();

    // Get connection state for UI indicator
    // 0 = Disabled/Disconnected, 1 = WiFi only, 2 = MQTT connected, 3 = Receiving data
    int getConnectionState();

private:
    WiFiClient _wifiClient;
    PubSubClient _mqttClient;

    const char* _ssid;
    const char* _password;
    const char* _broker;
    int _port;
    const char* _topic;

    StreamBackData _data;
    unsigned long _lastReconnectAttempt;

    static const unsigned long RECONNECT_INTERVAL = 5000;  // 5 seconds
    static const unsigned long DATA_TIMEOUT = 5000;        // 5 seconds

    void _connectWiFi();
    void _connectMqtt();
    void _parseMessage(byte* payload, unsigned int length);

    // Static callback wrapper for PubSubClient
    static void _mqttCallback(char* topic, byte* payload, unsigned int length);
    static StreamBackClient* _instance;  // For callback routing
};

#endif