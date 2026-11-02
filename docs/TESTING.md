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

## Remaining physical tests

I still need to verify automatic arm-triggered homing with a secured switch, repeatability, missed steps, shaft-angle accuracy, load behavior, and electrical/timing measurements. A manual switch press establishes a temporary software reference only. In a later bench attempt, I found the external motor supply unplugged from the wall; powered recovery and a fresh home test have not yet been reported.
