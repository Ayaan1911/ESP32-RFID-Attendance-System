#include "wifi_manager.h"

#include <WiFi.h>

#include "display_manager.h"

#if __has_include("wifi_credentials.h")
#include "wifi_credentials.h"
#elif __has_include("wifi_credentials.h.example")
#include "wifi_credentials.h.example"
#else
#error "Missing wifi_credentials.h or wifi_credentials.h.example"
#endif

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    updateDisplay("Connecting WiFi...", "", "");
    Serial.println("[WIFI] Connecting...");

    const unsigned long timeoutMs = 12000;
    const unsigned long startMs = millis();

    while (WiFi.status() != WL_CONNECTED && (millis() - startMs) < timeoutMs)
    {
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        String ipAddress = WiFi.localIP().toString();
        updateDisplay("WiFi Connected", ipAddress, "");
        Serial.print("[WIFI] Connected. IP: ");
        Serial.println(ipAddress);
        delay(1500);
        return;
    }

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    updateDisplay("WiFi Failed -", "Offline Mode", "");
    Serial.println("[WIFI] Connection failed. Offline mode enabled.");
    delay(1500);
}

bool isWiFiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}
