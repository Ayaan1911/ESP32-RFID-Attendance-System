# Security Considerations

The backend currently has no authentication and is intended for local-network testing only. Any device on the same network can read or write attendance data. This must be addressed before any internet exposure or the v2.0 hardened backend milestone.

The REST client added in v1.4 sends attendance events over plain, unauthenticated HTTP. That is acceptable for local-network testing only and must be hardened with HTTPS/TLS plus device authentication before any production or shared-network use. This is also the first milestone that transmits data off-device, making it the natural point to define device identity for future authentication design.
