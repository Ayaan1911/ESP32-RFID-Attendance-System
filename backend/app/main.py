from datetime import datetime, timezone

from fastapi import FastAPI

from app.models import AttendancePayload

app = FastAPI(title="ESP32 RFID Attendance Backend")

attendance_events: list[dict[str, str | None]] = []


@app.get("/health")
def health() -> dict[str, str]:
    return {"status": "ok"}


@app.post("/attendance")
def receive_attendance(payload: AttendancePayload) -> dict[str, str]:
    received_at = datetime.now(timezone.utc).isoformat()
    event = {
        "uid": payload.uid,
        "name": payload.name,
        "timestamp": payload.timestamp,
        "received_at": received_at,
    }
    attendance_events.append(event)

    print(
        f"[ATTENDANCE] received uid={payload.uid} "
        f"name={payload.name or ''} timestamp={payload.timestamp or ''} "
        f"received_at={received_at}"
    )

    return {"status": "received"}


@app.get("/attendance")
def list_attendance() -> list[dict[str, str | None]]:
    return attendance_events
