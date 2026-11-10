# My progress

## Basic motion and serial control — October 2026

I started with steady rotation, finite movement, and forward/reverse stepping. I added target validation, a commanded position counter, and software travel limits.

## Scheduled motion and stopping — October 2026

I separated one step from the movement loop and scheduled updates with millis(). I replaced the waiting serial reader with incremental reception. I tested Busy responses, partial and overlong input, STOP, and recovery. I observed the shaft stop and stay still during a recorded STOP test.

## Home input and bounded search — October 2026

I wired the home input with INPUT_PULLUP, added a 25 ms debounce filter, and used a new accepted press to stop motion. I added HOME with a step budget, timeout, held-switch guard, and manual zero assignment. Numeric targets now require a valid reference. STATUS reports the count, home flag, and operating mode.

## First Python client complete — November 2, 2026

I completed five-sample STATUS capture, elapsed timestamps, CSV export, typed readback, validation, and expected error handling. The final client passed 58 offline/simulated checks and a real serial capture with exit code 0.

## Mechanical assembly and documentation — November 2, 2026

I received the mechanical parts, mounted the motor and arm, checked my roller switch contacts, and reported one press/release input cycle. I added short code comments and checked that executable behavior was unchanged. Automatic arm-triggered homing and measured repeatability remain the next milestones.

## First learning prototype completed — November 10, 2026

I closed the current scope after manual HOME stopping, one visual out-and-back return, and a reported hands-free arm-triggered stop using a taped switch. Final compilation and offline validation of the saved Python capture passed. I documented the mounting difficulties, actual tape fastening, proposed weight, and unmeasured behavior in the [completion record](COMPLETION.md) and [mechanical iteration notes](MECHANICAL_ITERATIONS.md). Further mounting changes and repeatability measurements are optional future work.
