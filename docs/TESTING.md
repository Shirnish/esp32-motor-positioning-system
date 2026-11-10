# My test notes

Updated November 2, 2026. These results belong to successive documented revisions. They are not one full acceptance run of the latest source.

| Check | Recorded result | Scope |
| --- | --- | --- |
| Incremental serial receiver | 12 automated serial assertions passed on the receiver revision | Earlier revision; full suite not repeated after the HOME guard |
| STOP and recovery | Forward stop at count 291; idle/repeated STOP; reverse interruption at 823 | Serial results; I also observed stopping and stillness in the forward test |
| Debounced input | 25 ms filter; clean press/release reporting; a held press did not repeat messages | Manual input tests |
| Switch-triggered stop | Move to 1024 interrupted at count 117 | I observed the shaft stop physically |
| HOME reference and target guard | A manual press assigned zero; startup/canceled-HOME targets were rejected | Software reference behavior, not repeatable mechanical homing |
| Homing search limit | Search stopped at 256 issued steps; recorded interval 2.576 s | Step budget; timeout elapsed time was not independently measured |
| STATUS | IDLE, HOMING, MOVING, and homed/unhomed states reported | Serial state reporting |
| New roller switch | One manual press/release cycle reported on GPIO14 | No arm-driven activation in that excerpt |
| Python client | 58 offline/simulated checks and a real five-sample COM3 capture; CSV readback and exit 0 | STATUS only, with the motor idle; no physical motion observation |
| Source comments | C++ tokens excluding comments and Python AST identical to earlier source | No new firmware upload or hardware commands |

## Evidence

- [STOP transcript](evidence/stop-command-20261027.log) and [edge cases](evidence/stop-edge-cases-20261027.log).
- [Debounced switch](evidence/home-button-debounced-20261027.log) and [switch-triggered stop](evidence/home-button-stop-20261027.log).
- [Home-reference assignment](evidence/homing-reference-runtime-20261029.log), [target guard](evidence/homed-target-guard-runtime-20261030.log), [search budget](evidence/homing-search-step-limit-20261029.log), and [STOP during search](evidence/homing-search-serial-stop-20261029.log).
- [STATUS during movement](evidence/status-moving-completion-20261030.log) and [roller-switch input](evidence/roller-switch-input-20261102.log).
- [Python final test record](evidence/python-client-final-checks-20261102.log), [structured results](evidence/python-client-final-checks-20261102.json), and [sample CSV](evidence/status-samples-20261102.csv).
- [Comment-only source comparison and hashes](evidence/source-comments-20261102.json).

I replaced local machine paths with placeholders in the publication copies of logs. Numeric results, timestamps, and source hashes are retained. In the target-guard transcript, the first Received: HOME and Homing requested lines came from separate attempts; their gap is not a measured start delay.

The 58 Python checks cover protocol parsing, CSV loading, batch validation, and integrated normal/error paths using simulated serial/time and temporary files. Five initial cases hit a recorder issue involving Python 3.14 FrameLocalsProxy; after correcting only the recorder I reran those five, yielding 58 distinct passes. The real capture separately verified serial I/O, five saved samples, typed readback, closure, and exit 0.

## November 10 completion checkpoint

I completed a manual HOME test with physical stopping and stillness through release, then one 200-to-0 move with a visual return to the starting angle. I taped down the switch and confirmed that the arm pressed it and the motor stopped by itself with my hands off it. Following STATUS showed position=0 homed=1 mode=IDLE. The full taped-switch HOME request/completion transcript was not supplied.

The final sketch compiled with exit 0, using 305346 bytes of program storage and 22232 bytes of global-variable memory. The current Python source imported and reloaded/validated the preserved five-row hardware CSV. These closeout checks did not open a port or run new hardware tests.

- [Manual HOME and physical stop](evidence/manual-home-and-direction-20261110.log).
- [One visual return](evidence/manual-home-return-20261110.log).
- [Taped-switch contact and hands-free confirmation](evidence/taped-switch-contact-20261110.log).
- [Final compilation](evidence/closeout-firmware-compile-20261110.log) and [offline CSV validation](evidence/closeout-offline-check-20261110.json).
- [Completed scope and limitations](COMPLETION.md).

## Optional future measurements

Rigid mounting, arm lift/body movement, full transition capture, repeated-cycle behavior, angular accuracy, missed steps, load behavior, and electrical/timing measurements are optional extensions. The completed scope is a learning prototype with commanded-step accounting and a first functional taped fixture.
