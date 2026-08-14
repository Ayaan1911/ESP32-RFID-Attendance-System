#ifndef SYNC_QUEUE_H
#define SYNC_QUEUE_H

#include <Arduino.h>

void enqueueAttendanceEvent(const String &uid, const String &name);
void attemptQueueFlush();

#endif
