# ESP32 Line Follower — Historical Variants

This repository preserves two historical line-follower sketches from a university project archive. They are presented side by side for study; the available files do not establish which, if either, was used in a competition. The sketches are retained unchanged.

## Variants

- [`Robot.ino`](variants/Robot/Robot.ino) reads a 16-channel reflectance array through a multiplexed analog input, thresholds each channel, estimates line position, and adjusts two motor PWM outputs. Its active gains are `Kp=2.5`, `Ki=0`, and `Kd=5`, so the implemented controller is PD. Motor direction outputs are fixed in the sketch. A 200 ms delay in the sensor-reading routine limits its update rate.
- [`robotNucleo.ino`](variants/robotNucleo/robotNucleo.ino) calibrates per-channel minimum and maximum readings, normalizes sensor values, computes a weighted line position, and applies a time-based PID calculation. It starts a FreeRTOS task for sensor acquisition. The task and control/calibration code share the multiplexer select pins and sensor buffer without visible synchronization; concurrent behavior requires hardware validation.

Both variants use four digital select lines and one analog sensor input. Their motor pin assignments and control details differ; see [hardware notes](docs/hardware.md).

## Build and validation status

The archive contains no board definition, Arduino CLI or PlatformIO configuration, dependency lock, or verified toolchain version. The exact ESP32 board/module, Arduino-ESP32 core version, wiring, and motor driver are not identified. Therefore this repository does not claim a reproducible build. The sketches were inspected statically only; compilation, sensor calibration, motor behavior, and track performance remain unverified. Do not infer a competition result or final firmware from the variant names.

The code uses Arduino APIs; `robotNucleo.ino` explicitly includes `Arduino.h` and uses ESP32 FreeRTOS APIs (`xTaskCreatePinnedToCore`). No additional external library include was found in these two files.

## License

This snapshot is distributed under the MIT License; see [`LICENSE`](LICENSE). The Arduino/ESP32 core and FreeRTOS are external dependencies and remain subject to their own terms. No third-party library source or binary is bundled here.
