#include <Arduino.h>
#if defined ESP32
#include <WiFi.h>
#include <esp_wifi.h>
#elif defined ESP32C3
#include <WiFi.h>
#include <esp_wifi.h>
#elif defined ESP8266
#include <ESP8266WiFi.h>
#define WIFI_MODE_STA WIFI_STA 
#else
#error "Unsupported platform"
#endif //ESP32
#include <QuickEspNow.h>

static const String msg = "";

static uint8_t receiver[] = { 0x12, 0x34, 0x56, 0x78, 0x90, 0x12 };

//#define DEST_ADDR receiver
#define DEST_ADDR ESPNOW_BROADCAST_ADDRESS 

bool sent = true;

const unsigned int SEND_MSG_MSEC = 1;

void dataSent (uint8_t* address, uint8_t status) {
    sent = true;
    Serial.printf ("Message sent to " MACSTR ", status: %d\n", MAC2STR (address), status);
}

void dataReceived (uint8_t* address, uint8_t* data, uint8_t len, signed int rssi, bool broadcast) {
    Serial.print ("\033[1;96mReceived: \033[0m");
    Serial.printf ("%.*s\t%d\t", len, data, len);
    Serial.printf ("RSSI: %d dBm\t", rssi);
    Serial.printf ("From: " MACSTR "\t", MAC2STR (address));
    Serial.printf ("%s\n", broadcast ? "Broadcast" : "Unicast");
}

void setup () {
    Serial.begin (115200);
    WiFi.mode (WIFI_MODE_STA);
#if defined ESP32
    WiFi.disconnect (false, true);
#elif defined ESP8266
    WiFi.disconnect (false);
#endif //ESP32

    Serial.printf ("Connected to %s in channel %d\n", WiFi.SSID ().c_str (), WiFi.channel ());
    Serial.printf ("IP address: %s\n", WiFi.localIP ().toString ().c_str ());
    Serial.printf ("MAC address: %s\n", WiFi.macAddress ().c_str ());
    quickEspNow.begin (1);
    quickEspNow.onDataSent (dataSent);
    quickEspNow.onDataRcvd (dataReceived);
}

void loop () {
    static time_t lastSend = 0;
    static unsigned int counter = 0;

  /*    // Sent flag is needed to wait for the message to be actually sent. Avoids messages dropping, maximizing throughput.
    if (sent && ((millis () - lastSend) > SEND_MSG_MSEC)) {
        lastSend = millis ();
        String message = String (msg) + " " + String (counter++);
        sent = false;
        if (!quickEspNow.send (DEST_ADDR, (uint8_t*)message.c_str (), message.length ())) {
            Serial.printf ("\033[1;44mSENT: %s \t\033[0m", message);
        } else {
            Serial.printf ("\033[1;41mNOT SENT\033[0mT ");
            sent = true;
        }

    } */

}
