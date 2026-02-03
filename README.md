# Robotic Kiosk: Ports & Adapters Architecture (C++20)

This project is a high-fidelity C++ implementation of a robotic kiosk system using **Hexagonal Architecture** (Ports 
and Adapters). It demonstrates how to build a hardware-independent core system that is highly testable and thread-safe.

## 🏗 Architecture

The system is designed with a strict "Inside-Out" dependency rule:

- **Core (Domain):** The "Brain" of the kiosk. It manages state transitions (Waiting, Moving, Picking) and 
  translates string inputs (like "A1") into grid coordinates.
- **Ports (Interfaces):** Abstract contracts (`IArmPort`, `IViewPort`) that define how the Core communicates with 
  the outside world.
- **Adapters (Infrastructure):** - **SimulatedArm:** A multithreaded simulation of a robotic arm.
    - **MockArm:** A GoogleMock-based implementation for unit testing logic without hardware.



## 🧵 Threading & Concurrency

The project utilizes modern **C++20** features to handle asynchronous hardware simulation:

- **`std::jthread`:** Ensures the simulation thread is automatically joined on destruction (RAII).
- **`std::stop_token`:** Provides a clean way to signal the arm to stop immediately during application shutdown.
- **`std::condition_variable_any`:** Implements an event-driven model. The arm thread sleeps and consumes **0% CPU** 
  until a new destination is provided, at which point it wakes up instantly.
- **Thread Safety:** All shared data between the Kiosk thread and the Arm thread is synchronized using `std::mutex` 
  and `std::lock_guard`.



## 🧪 Testing Strategy

By using the Ports and Adapters pattern, the system can be tested in isolation:

- **Logic Testing:** We use **Google Mock** to verify that the Kiosk logic correctly calculates coordinates and 
  triggers the arm.
- **State Testing:** We verify that the Kiosk moves from `WaitingState` to `MovingState` only when valid input is 
  received.
- **Deterministic Results:** Tests run in milliseconds because they use "hollow" mocks rather than waiting for the 
  real-time simulation.



## 🤖 AI-Augmented Development

This project was developed using an **AI-Collaborative workflow**, where AI (specifically Google Gemini) served as a 
"digital pair-programmer." A primary goal of this approach was to **refresh and modernize C++ expertise** by 
applying C++20 standards to a real-world architectural challenge.

The AI was utilized for:

- **Concurrency Design:** Applied modern C++20 threading patterns, specifically the integration of `std::jthread` 
  with `std::condition_variable_any` for efficient, event-driven simulation.
- **Unit Test Generation:** Designed and implemented Google Mock objects and test fixtures to ensure coverage of the 
  logic sections.
- **Documentation:** Authored technical guides and this README to communicate the system's design clearly.

This collaboration allowed for rapid prototyping and the immediate application of modern C++ best practices, 
ensuring the code is not only functional but also aligned with contemporary industry standards.

## 📂 Project Structure

```text
.
├── src/
│   ├── core/           # Kiosk.cpp, State Machine logic
│   ├── ports/          # IArmPort.h, IViewPort.h (Interfaces)
│   ├── adapters/       # SimulatedArm.cpp (C++20 Threaded Sim)
│   └── util/           # Coordinate.h, CommandQueue.h
├── tests/              # GTest suites (test_Kiosk.cpp)
└── main.cpp            # Composition root (wiring it all together)

