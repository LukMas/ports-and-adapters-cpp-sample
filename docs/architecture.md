# GrabStation Architecture

This document describes the architecture and runtime model of GrabStation:
a C++20 sample project that simulates a robotic kiosk device. The main goals are to demonstrate **Ports & Adapters (
Hexagonal Architecture)**, a **command-driven state machine**, **status eventing**, **monitoring/watchdogs**, and 
**asynchronous logging** — all in a structure easy to test.

---

## 1. Architectural Style: Ports & Adapters (Hexagonal)

### Core idea

- The **core** contains the domain/application logic.
- The core depends only on **ports** (abstract interfaces).
- **Adapters** implement ports to integrate with the outside world (console UI, simulation, logging, messaging,
  monitors).

### Dependency rule

Dependencies point inward:

- Core → Ports (interfaces)
- Adapters → Ports and Core (to connect the system)

This avoids leaking infrastructure concerns into the domain model and keeps the core stable as adapters evolve.

---

## 2. Core Runtime Model

GrabStation simulates a device that runs continuously using a **cyclic scan (heartbeat)**.

Per-cycle responsibilities typically include:

1. Update simulated “plant” / sensor inputs (e.g., arm position update).
2. Run monitors/watchdogs.
3. Process pending commands in the core state machine.

This structure is common in control-style systems because it is:

- predictable,
- testable,
- easy to reason about,
- straightforward to instrument.

---

## 3. Core Domain Components

### 3.1 Kiosk (application core)

Responsibilities:

- Pull commands from the command queue port.
- Delegate command handling to the current state.
- Perform state transitions.
- Publish user-facing messages via the view port.
- Broadcast state changes via status listeners.
- Emit logs via the logger port.

Non-responsibilities (intentionally outside the core):

- terminal I/O,
- file I/O,
- threading details,
- device simulation details.

### 3.2 State machine

The core behavior is modeled with explicit states:

- Each state handles only the commands that make sense for that state.
- Unsupported commands result in “no transition”.

Benefits:

- illegal operations are naturally prevented by design,
- transitions are explicit and unit-testable,
- extension is localized (add a state/command/transition without rewriting everything).

Some transitions may be treated as global (e.g., forcing IDLE) to avoid duplicating logic in every state.

---

## 4. Ports (Core Interfaces)

Ports are small contracts that define what the core needs from the outside world.

### 4.1 Arm port

Represents motion control at the level the kiosk needs:

- set destination,
- read current position,
- check whether the target is reached.

The same core can work with:

- a simulation adapter,
- real hardware adapter,
- mock adapter in tests.

### 4.2 View port

Represents user-facing output:

- status/info messages,
- error messages.

This enables different presentation layers (console, GUI, service) without changing the core.

### 4.3 Command queue port (messaging boundary)

Represents input as commands:

- producers push commands,
- core pops commands.

This creates clean decoupling between:

- input sources (console/controller, monitors, tests),
- the domain consumer (kiosk).

### 4.4 Logger port

Represents logging as an interface so the core never depends on:

- file APIs,
- formatting implementation,
- background threads,
- output destinations.

---

## 5. Adapters (Infrastructure)

Adapters implement ports to integrate the system.

### 5.1 Console adapter

Often combines:

- a controller: reads input and pushes commands,
- a view: renders status/errors and (optionally) other telemetry like arm position.

Terminal handling and rendering stay out of the core.

### 5.2 Messaging adapter: synchronized command queue

A thread-safe queue adapter enables:

- one or more producers on separate threads,
- one consumer (core loop).

Conceptually, this is an in-process “message bus” that can later be replaced with IPC/network transport while keeping
the core unchanged.

### 5.3 Simulation adapter: simulated arm

A simulated arm adapter implements the arm port and updates its internal position over time, enabling:

- interactive demo runs without hardware,
- deterministic behavior for certain tests (or replacement with mocks).

### 5.4 Monitoring adapter: watchdog

A watchdog/monitor is modeled as a subscriber to machine status changes:

- it receives state changes via a listener interface,
- it tracks time/conditions,
- it injects corrective actions by pushing commands to the queue.

This keeps policy enforcement separate from the core state machine logic.

### 5.5 Logging adapter: async logger

The async logger pattern:

- `log()` enqueues quickly (low latency on the calling thread),
- a background flush loop waits for messages and writes them to a file,
- shutdown is handled via cooperative cancellation.

This keeps core execution responsive and avoids file I/O in the main loop.

---

## 6. Status Listener (Observer/Eventing)

The status listener mechanism broadcasts machine status changes to multiple consumers:

- monitors/watchdogs,
- views,
- telemetry publishers,
- integration harnesses.

Why it’s separate from the view:

- view is for presentation,
- status events are for system-level integration and monitoring.

This enables adding new subscribers without modifying core logic.

---

## 7. Cyclic Scan Timing (Heartbeat)

A busy loop without waiting will waste CPU. A fixed-rate heartbeat is typically preferred.

Plausible scan periods for this type of simulation:

- **10 ms (100 Hz)**: responsive and “control-like” (*actual speed*)
- **20 ms (50 Hz)**: still responsive; lighter CPU
- **50 ms (20 Hz)**: acceptable for UI/monitoring; may feel slow for motion/control responsiveness

A common approach is to run control logic at 10–20 ms and throttle UI rendering separately (e.g., 100 ms).

---

## 8. Testing Strategy

This architecture supports fast unit tests because:

- ports can be mocked,
- state transitions can be validated deterministically,
- side effects are asserted at boundaries (view/log/status events).

The core can be validated without:

- console I/O,
- file I/O,
- threads,
- real/simulated arm movement.

---

## 9. Extension Points

Typical extensions that fit naturally:

- add real hardware adapter implementing the arm port,
- add a telemetry adapter subscribing to status changes,
- add structured logging (JSON, correlation IDs),
- add fault states and recovery commands,
- refine queue semantics (blocking pop, timeouts, backpressure).