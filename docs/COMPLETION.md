# My completed motor-positioning prototype

Completed: **November 10, 2026**. Status: **completed first learning prototype**.

I am closing this project at its current functional scope. I built serial-controlled stepper firmware, a working home-input prototype, and a Python STATUS/CSV client. Further mechanical refinement and precision measurements are optional future work. No additional purchases are required to complete this iteration.

## What I completed

| Area | What I demonstrated |
| --- | --- |
| Motor control | Scheduled stepping, forward/reverse movement, commanded targets from 0 to 1024 after homing, and software travel limits |
| Serial commands | Incremental reception, invalid-input/overflow/length checks, Busy handling, STOP, HOME, and STATUS |
| Manual home input | A debounced press during HOME stopped the motor, assigned zero, and left the system homed and idle |
| First mechanical iteration | I taped down the roller switch and reported that the motor arm pressed it and the motor stopped by itself with my hands off the switch; the following STATUS showed position=0 homed=1 mode=IDLE |
| Return movement | After manual HOME, I completed one move to 200 and back to 0 and visually observed a return to the starting angle |
| Python client | Five-sample STATUS capture, elapsed timestamps, CSV export, typed readback, validation, cleanup, and expected error handling |

The first Python client passed 58 offline/simulated checks and a real five-sample serial/CSV run on November 2. On November 10, the current Python source imported successfully and reloaded/validated that preserved five-row capture. The final firmware also compiled successfully with the recorded ESP32-S3 settings, using 305346 bytes of program storage and 22232 bytes of global-variable memory. This closeout check did not upload firmware, open a serial connection, or command movement.

## What the physical evidence establishes

I preserved complete manual-HOME output and my observation that the shaft stopped and stayed still through switch release. I also preserved the 200-to-0 return excerpt and my visual return observation.

For the taped-switch result, I confirmed hands-free stopping when the arm pressed the switch and supplied the following state:

```text
15:55:41.112 -> STATUS position=0 homed=1 mode=IDLE
```

The supplied excerpt for that taped-switch attempt does not include its HOME request or Homing Complete transition. I am presenting it as a reported physical result with a final STATUS snapshot. I have not completed a repeated-cycle or calibrated accuracy study.

## My first mechanical iteration

I mounted the motor and shaft arm on a predrilled plate with brass supports. The switch had no mounting holes, so I used tape to fasten its body. During alignment I encountered low roller height, upward movement of the silver arm, and movement of the switch body. Those were practical assembly problems I recorded while working toward the first functional contact.

I proposed added weight as a temporary way to hold the arm down, but I have not confirmed that a weight was installed or established it as a fix. My [mechanical iteration notes](MECHANICAL_ITERATIONS.md) distinguish actual construction from proposed changes.

## Limits of this completed scope

Position is an issued-step estimate, with no measured shaft-position feedback. I have not measured angular accuracy, missed steps, homing repeatability, load behavior, stopping latency, motor current, loaded supply voltage, or electrical coil release.

The taped switch is a first-iteration fixture. Long-term retention and resolution of the earlier arm lift/body movement have not been established. The firmware's ordinary switch stop acts on a newly accepted press; it is not a continuous hardware interlock.

I can later improve the switch support, fastening, arm retention, and roller approach and then measure repeatability. These are documented extension ideas, not requirements left open for this completed learning prototype.

## Evidence

- [Manual HOME and physical stopping](evidence/manual-home-and-direction-20261110.log).
- [One visual out-and-back return](evidence/manual-home-return-20261110.log).
- [Tape fastening, arm contact, and hands-free confirmation](evidence/taped-switch-contact-20261110.log).
- [Python final checks and real capture](evidence/python-client-final-checks-20261102.log).
- [Closeout Python/source check](evidence/closeout-offline-check-20261110.json).
- [Final firmware compilation](evidence/closeout-firmware-compile-20261110.log).

## Final wiring and photographs

My [wiring diagram and three final photographs](WIRING.md) document the saved GPIO assignments, switch contacts, separate motor power and common ground, and the taped first-iteration fixture.
