from pydantic import BaseModel, field_validator


class AttendancePayload(BaseModel):
    uid: str
    name: str | None = None
    timestamp: str | None = None

    @field_validator("uid")
    @classmethod
    def validate_uid(cls, value: str) -> str:
        normalized = value.strip()
        if not normalized:
            raise ValueError("uid must not be empty")
        return normalized
