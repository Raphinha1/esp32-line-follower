# Hardware Notes

These details are inferred from the sketches, not confirmed against a physical robot. No board model, sensor model, multiplexer model, motor driver, wiring diagram, or power specification was included with the two source files.

## `Robot/Robot.ino`

| Function | GPIO | Evidence / note |
|---|---:|---|
| Multiplexer select S0 | 26 | Digital output |
| Multiplexer select S1 | 27 | Digital output |
| Multiplexer select S2 | 14 | Digital output |
| Multiplexer select S3 | 12 | Digital output |
| Analog sensor input | 13 | Reads 16 selected channels |
| Right motor PWM | 17 | `analogWrite`, constrained to 0–70 |
| Right motor direction | 5 | Set HIGH by the sketch |
| Left motor PWM | 16 | `analogWrite`, constrained to 0–70 |
| Left motor direction | 4 | Set LOW by the sketch |

## `robotNucleo/robotNucleo.ino`

| Function | GPIO | Evidence / note |
|---|---:|---|
| Multiplexer select S0 | 26 | Digital output |
| Multiplexer select S1 | 27 | Digital output |
| Multiplexer select S2 | 14 | Digital output |
| Multiplexer select S3 | 12 | Digital output |
| Analog sensor input | 13 | Reads 16 selected channels |
| Right motor PWM | 4 | `analogWrite`, constrained to 0–255 |
| Right motor direction | 17 | Derived from a constrained nonnegative speed; intended direction behavior needs confirmation |
| Left motor PWM | 16 | `analogWrite`, constrained to 0–255 |
| Left motor direction | 2 | Derived from a constrained nonnegative speed; intended direction behavior needs confirmation |

## Components suggested by the source

- ESP32-class target: GPIO selection and `xTaskCreatePinnedToCore` indicate ESP32-specific APIs, but the exact module is unknown.
- A 16-channel reflectance/line sensor arrangement and analog multiplexer are implied by the 16 readings and four select pins; models and channel order are unknown.
- Two motor outputs and direction controls imply a dual-channel motor driver and two drive motors; their models and wiring are unknown.

## Confirm before reproducing

Record the exact board/module and FQBN, Arduino-ESP32 core version, sensor and multiplexer models, channel order, motor driver, motor polarity, supply/battery details, and a wiring diagram. Confirm both variants' motor direction behavior and calibration procedure on the actual hardware. No encoders, closed-loop wheel-speed feedback, or other physical components are established by these sketches.
