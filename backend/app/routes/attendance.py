from datetime import datetime, timezone

from fastapi import APIRouter, Depends

from app.auth import require_api_key
from app.models import AttendancePayload

router = APIRouter(tags=["attendance"])
attendance_events: list[dict[str, str | None]] = []


@router.post("/attendance", dependencies=[Depends(require_api_key)])
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


@router.get("/attendance", dependencies=[Depends(require_api_key)])
def list_attendance() -> list[dict[str, str | None]]:
    return attendance_events
