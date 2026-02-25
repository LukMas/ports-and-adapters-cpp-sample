# GrabStation (C++20) — Ports & Adapters Robotic Kiosk Sample

GrabStation is a C++20 sample project that simulates a small robotic kiosk device using **Ports & Adapters (Hexagonal Architecture)**. The core runs a **command-driven state machine** and is intentionally isolated from infrastructure concerns such as UI, messaging, logging, and hardware/simulation.

The application is executed as a **cyclic scan (heartbeat)**: at each cycle it updates the simulated plant (arm), runs monitors/watchdogs, and processes queued commands through the core.

## Highlights
- **Ports & Adapters** boundaries (core depends on interfaces only)
- **State machine** with explicit transitions and constraints
- **Command queue** as a messaging boundary between producers and the core loop
- **Status listener (observer)** for broadcasting machine state changes
- **Watchdog/monitor** that can inject corrective commands
- **Async logging** via a logger port + background flush adapter

## Project Layout
- `src/main/core/` — domain logic (kiosk + states) and ports (interfaces)
- `src/main/adapters/` — console UI, messaging queue, simulation, monitors, logging
- `src/test/` — unit tests (mocking ports)

## Documentation
- Architecture overview: `docs/architecture.md`