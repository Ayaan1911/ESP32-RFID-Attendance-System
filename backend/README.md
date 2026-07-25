# Backend

Minimal FastAPI server for local attendance testing.

## Setup

Create a virtual environment and install dependencies:

```bash
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements.txt
```

## Run

Start the server with:

```bash
uvicorn app.main:app --host 0.0.0.0 --port 8000 --reload
```

`--host 0.0.0.0` is required so the ESP32 can reach the server over your local network. Binding only to `127.0.0.1` will make it unavailable to other devices.

## Find Your Local IP

On Windows, run:

```bash
ipconfig
```

Use the IPv4 address of the active network adapter when testing from the ESP32 or another device on the same network.

## Endpoints

- `GET /health`
- `POST /attendance`
- `GET /attendance`
