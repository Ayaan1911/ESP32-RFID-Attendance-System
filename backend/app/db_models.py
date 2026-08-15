from datetime import datetime

from sqlalchemy import DateTime, Integer, String
from sqlalchemy.orm import Mapped, mapped_column

from app.database import Base, engine


class AttendanceEvent(Base):
    __tablename__ = "attendance_events"

    id: Mapped[int] = mapped_column(Integer, primary_key=True, index=True)
    uid: Mapped[str] = mapped_column(String(128), nullable=False, index=True)
    name: Mapped[str | None] = mapped_column(String(255), nullable=True)
    timestamp: Mapped[str | None] = mapped_column(String(255), nullable=True)
    received_at: Mapped[datetime] = mapped_column(DateTime(timezone=True), nullable=False, index=True)

    def to_dict(self) -> dict[str, str | int | None]:
        return {
            "id": self.id,
            "uid": self.uid,
            "name": self.name,
            "timestamp": self.timestamp,
            "received_at": self.received_at.isoformat(),
        }


def create_tables() -> None:
    Base.metadata.create_all(bind=engine)
