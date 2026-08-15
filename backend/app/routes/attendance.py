from datetime import datetime, timezone

from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy.orm import Session

from app.auth import require_api_key
from app.database import get_db
from app.db_models import AttendanceEvent
from app.models import AttendancePayload

router = APIRouter(tags=["attendance"])


@router.post("/attendance", dependencies=[Depends(require_api_key)])
def receive_attendance(
    payload: AttendancePayload,
    db: Session = Depends(get_db),
) -> dict[str, str]:
    received_at = datetime.now(timezone.utc)
    event = AttendanceEvent(
        uid=payload.uid,
        name=payload.name,
        timestamp=payload.timestamp,
        received_at=received_at,
    )
    try:
        db.add(event)
        db.commit()
        db.refresh(event)
    except SQLAlchemyError as exc:
        db.rollback()
        raise HTTPException(
            status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
            detail="Failed to persist attendance event",
        ) from exc

    print(
        f"[ATTENDANCE] received uid={payload.uid} "
        f"name={payload.name or ''} timestamp={payload.timestamp or ''} "
        f"received_at={received_at}"
    )

    return {"status": "received"}


@router.get("/attendance", dependencies=[Depends(require_api_key)])
def list_attendance(db: Session = Depends(get_db)) -> list[dict[str, str | int | None]]:
    events = (
        db.query(AttendanceEvent)
        .order_by(AttendanceEvent.received_at.asc(), AttendanceEvent.id.asc())
        .all()
    )
    return [event.to_dict() for event in events]
