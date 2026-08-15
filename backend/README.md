# Backend

Minimal FastAPI server for local attendance testing.

## Setup

Create a virtual environment and install dependencies:

```bash
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements.txt
```

Create your local environment file from the example and set a shared API key:

```bash
copy .env.example .env
```

Edit `.env` and replace `API_KEY=your_shared_secret_key_here` with a real secret.

## Run

Start the server with:

```bash
uvicorn app.main:app --host 0.0.0.0 --port 8000 --reload
```

`--host 0.0.0.0` is required so the ESP32 can reach the server over your local network. Binding only to `127.0.0.1` will make it unavailable to other devices.

`/attendance` now requires the `X-API-Key` header. `/health` remains unauthenticated for simple liveness checks.

## Find Your Local IP

On Windows, run:

```bash
ipconfig
```

Use the IPv4 address of the active network adapter when testing from the ESP32 or another device on the same network.

## Endpoints

- `GET /health`
- `POST /attendance` with `X-API-Key`
- `GET /attendance` with `X-API-Key`
