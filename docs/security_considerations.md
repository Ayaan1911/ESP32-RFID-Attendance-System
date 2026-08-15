# Security Considerations

The `/attendance` endpoints are now protected by a static shared API key sent in the `X-API-Key` header by the firmware and validated by the backend. That is acceptable for local-network testing, but it does not provide rotation, revocation, or per-device identity. A production or multi-device deployment would need real device credentials and stronger transport security.

The REST client still sends attendance events over plain HTTP. API-key authentication reduces casual unauthorized access, but it does not encrypt traffic or stop replay/sniffing on an untrusted network. HTTPS/TLS is still required before any internet exposure or shared-network deployment.

The v1.5 offline sync queue keeps pending attendance events in plaintext RAM only. It is not persisted across reboot, so an outage followed by a reboot will lose any queued events. That limitation is documented here and is not fixed in this milestone.
