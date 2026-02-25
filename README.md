# Ports & Adapters C++ Sample – Robotic Kiosk

This project demonstrates a C++20 implementation of a robotic kiosk system using Hexagonal Architecture (Ports & Adapters). It showcases a hardware-independent, modular core system designed for testability, concurrency and maintainability.

## Architecture Overview

**Inside-Out Dependency Rule:**

- **Core (Domain)**: The "brain" of the kiosk. Manages states (Waiting, Moving, Picking) and translates inputs (like "A1") into grid coordinates.

- **Ports (Interfaces)**: Abstract contracts (IArmPort, IViewPort) defining communication between core logic and external systems.

- **Adapters (Infrastructure)**:
  - SimulatedArm: Multithreaded robotic arm simulation.
  - MockArm: Google Mock implementation for unit testing without real hardware.

*Diagram (ASCII):*

```
[Adapters] <---- [Ports] <---- [Core Domain Logic]
   SimArm       IArmPort       KioskStateMachine
   MockArm      IViewPort      Input Parsing / Decision Logic
```

## Concurrency & Thread Safety

- **std::jthread** for automatic thread joining (RAII)
- **std::stop_token** for clean stop signaling
- **std::condition_variable_any** for event-driven updates
- **std::mutex** + **std::lock_guard** for safe shared data access

Key idea: The core logic is thread-safe and can handle asynchronous events without blocking CPU cycles unnecessarily.

## Testing Strategy

- **Logic Tests**: Google Mock verifies correct coordinate calculation and arm triggering.
- **State Tests**: Ensures valid state transitions only occur on correct inputs.
- **Fast & Deterministic**: Tests use mocks instead of real-time simulation, running in milliseconds.

## Features Demonstrated

- Modern C++20 practices (jthread, stop_token, RAII, concurrency primitives)
- Dependency Injection for testable adapters
- Event-driven architecture in a simulated hardware environment
- Unit tests for both domain logic and integration with adapters

## Usage

Build:

```
mkdir build && cd build
cmake ..
make
```

Run Simulation:

```
./kiosk_simulator
```

Run Tests:

```
ctest --output-on-failure
```

## Why This Project Matters

This sample shows how to structure a maintainable C++ system where domain logic is isolated from hardware, concurrency is handled safely, and testing is straightforward. 
It’s directly relevant to control systems, robotics, and embedded-like environments — all key areas in automation industries.
