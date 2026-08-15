from pathlib import Path
import os

from dotenv import load_dotenv

BASE_DIR = Path(__file__).resolve().parent.parent
load_dotenv(BASE_DIR / ".env", override=True)

API_KEY = os.getenv("API_KEY", "").strip()
DATABASE_URL = os.getenv("DATABASE_URL", "").strip()
