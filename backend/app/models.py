from pydantic import BaseModel


class AttendancePayload(BaseModel):
    uid: str
    name: str | None = None
    timestamp: str | None = None
