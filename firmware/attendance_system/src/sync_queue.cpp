#include "sync_queue.h"

#include "wifi_manager.h"

bool postAttendanceEvent(const String &uid, const String &name);

namespace
{
struct QueuedAttendanceEvent
{
    String uid;
    String name;
};

static const size_t QUEUE_CAPACITY = 24;
static QueuedAttendanceEvent queue[QUEUE_CAPACITY];
static size_t queueHead = 0;
static size_t queueCount = 0;

size_t queueIndex(size_t offset)
{
    return (queueHead + offset) % QUEUE_CAPACITY;
}
} // namespace

void enqueueAttendanceEvent(const String &uid, const String &name)
{
    if (queueCount == QUEUE_CAPACITY)
    {
        const QueuedAttendanceEvent &dropped = queue[queueHead];
        Serial.print("[QUEUE] Queue full. Dropping oldest event for ");
        Serial.print(dropped.name);
        Serial.print(" (");
        Serial.print(dropped.uid);
        Serial.println(").");

        queueHead = queueIndex(1);
        --queueCount;
    }

    const size_t insertIndex = queueIndex(queueCount);
    queue[insertIndex].uid = uid;
    queue[insertIndex].name = name;
    ++queueCount;

    Serial.print("[QUEUE] Enqueued attendance event for ");
    Serial.print(name);
    Serial.print(" (");
    Serial.print(uid);
    Serial.println(").");
}

void attemptQueueFlush()
{
    if (queueCount == 0)
    {
        return;
    }

    if (!isWiFiConnected())
    {
        Serial.println("[QUEUE] Flush skipped: WiFi offline.");
        return;
    }

    const QueuedAttendanceEvent &nextEvent = queue[queueHead];

    if (!postAttendanceEvent(nextEvent.uid, nextEvent.name))
    {
        Serial.print("[QUEUE] Flush failed for ");
        Serial.print(nextEvent.name);
        Serial.print(" (");
        Serial.print(nextEvent.uid);
        Serial.println("). Will retry later.");
        return;
    }

    queue[queueHead] = {};
    queueHead = queueIndex(1);
    --queueCount;

    Serial.print("[QUEUE] Flushed queued event for ");
    Serial.print(nextEvent.name);
    Serial.print(" (");
    Serial.print(nextEvent.uid);
    Serial.println(").");
}
