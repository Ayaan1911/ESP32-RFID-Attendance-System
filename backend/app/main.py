from fastapi import FastAPI

from app.db_models import create_tables
from app.routes.attendance import router as attendance_router
from app.routes.health import router as health_router

app = FastAPI(title="ESP32 RFID Attendance Backend")

app.include_router(health_router)
app.include_router(attendance_router)


@app.on_event("startup")
def startup_create_tables() -> None:
    create_tables()
