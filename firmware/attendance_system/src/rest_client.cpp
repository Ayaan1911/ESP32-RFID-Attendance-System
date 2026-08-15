#include "rest_client.h"

#include <HTTPClient.h>

#include "backend_config.h"
#include "sync_queue.h"
#include "wifi_manager.h"

namespace
{
String escapeJson(const String &value)
{
    String escaped;
    escaped.reserve(value.length() + 8);

    for (size_t i = 0; i < value.length(); ++i)
    {
        const char c = value[i];

        switch (c)
        {
        case '\\':
            escaped += "\\\\";
            break;
        case '"':
            escaped += "\\\"";
            break;
        case '\b':
            escaped += "\\b";
            break;
        case '\f':
            escaped += "\\f";
            break;
        case '\n':
            escaped += "\\n";
            break;
        case '\r':
            escaped += "\\r";
            break;
        case '\t':
            escaped += "\\t";
            break;
        default:
            if (static_cast<unsigned char>(c) < 0x20)
            {
                escaped += ' ';
            }
            else
            {
                escaped += c;
            }
            break;
        }
    }

    return escaped;
}
} // namespace

bool postAttendanceEvent(const String &uid, const String &name)
{
    WiFiClient client;
    HTTPClient http;
    String url = "http://" + String(BACKEND_HOST) + ":" + String(BACKEND_PORT) + "/attendance";
    String payload = "{\"uid\":\"" + escapeJson(uid) + "\",\"name\":\"" + escapeJson(name) + "\"}";

    if (!http.begin(client, url))
    {
        Serial.println("[REST] Failed to initialize HTTP client.");
        return false;
    }

    http.setTimeout(4000);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-API-Key", BACKEND_API_KEY);

    int statusCode = http.POST(payload);
    bool success = (statusCode >= 200 && statusCode < 300);

    if (success)
    {
        Serial.print("[REST] Attendance sync response: ");
        Serial.println(statusCode);
    }
    else
    {
        Serial.print("[REST] Attendance sync failed: ");
        Serial.println(http.errorToString(statusCode));
    }

    http.end();

    return success;
}

void sendAttendanceEvent(const String &uid, const String &name)
{
    if (!isWiFiConnected())
    {
        Serial.println("[REST] WiFi offline; queueing attendance sync.");
        enqueueAttendanceEvent(uid, name);
        return;
    }

    if (postAttendanceEvent(uid, name))
    {
        return;
    }

    enqueueAttendanceEvent(uid, name);
}
