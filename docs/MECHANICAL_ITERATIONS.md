# My mechanical build iterations

## Iteration 1 - Getting the physical home trigger working

Updated November 10, 2026. Status: **completed first prototype; one hands-free arm-triggered stop was reported with zero/homed/idle STATUS. Further mounting refinement and repeatability measurements are optional extensions.**

For my first mechanical iteration, I focused on getting the motor arm to operate the home switch without me holding it in position. I have now reported the first hands-free stop using tape fastening, followed by zero/homed/idle STATUS. I can use this functional first result as a baseline for improving the mounting and checking repeated contact.

I have already mounted the motor and shaft arm on a predrilled plate using brass supports. I tested HOME by pressing the switch myself, observed the motor stop and stay still, and completed one move to 200 and back to zero with a visual return to the starting angle. Those results give me a working software and manual-input baseline for the mechanical build.

### Closeout

I closed the first prototype on November 10, 2026, using the demonstrated functional results and existing parts. I am keeping the mounting problems and proposed improvements in this record as future development ideas. The [completion record](COMPLETION.md) defines the finished scope and the limits of its evidence.

### Problems I ran into

The switch needs to sit higher so the roller lines up with the moving arm. When the arm makes contact, the silver arm lifts and the switch body moves a little as the arm continues its approach. I proposed added weight to keep the arm down while I work toward the first functional setup. I still need to identify whether the arm rises at the hub connection, flexes, or is pushed upward by the contact geometry.

The switch case has no mounting holes, so my first contact arrangement was held by hand. I have now used tape to fasten down its body and reported that the arm pressed the switch. The tape type and attachment surface still need to be recorded. I need to check whether the taped arrangement stays in place while leaving the roller and lever free to move.

### My adjustments and DIY work

| Problem or constraint | What I built or proposed | Current result |
| --- | --- | --- |
| The kit motor started with a bare shaft | I installed a shaft arm and mounted the motor on a predrilled plate with brass supports | Shown in my assembly photos; manual HOME and one visual out-and-back cycle passed |
| The switch has no mounting holes | I used tape to fasten down the switch body | Tape fastening reported; tape type, attachment surface, and support details still need to be recorded |
| The roller sits too low | I need to raise the switch and line up the roller with the arm edge | Adjustment pending |
| The silver arm lifts and the switch body shifts at contact | I proposed weight to restrain the arm; I also need to check its shaft attachment and secure the switch support | Reported mechanical problems; the weight remains a proposed experiment |
| I was holding the switch during the contact setup | I taped down the switch and let the arm press it | I confirmed that the motor stopped by itself with my hands off the switch; subsequent STATUS showed commanded 0, homed=1, and IDLE |

I will check the switch height, tape fastening, and arm-to-shaft attachment before deciding whether added weight is needed. I want the arm to meet the roller at the intended height while the switch stays in place. I have observed the arm lifting and the switch body shifting, but I have not yet established the source of the arm lift. Added weight remains a proposed temporary experiment.

### First taped-switch contact

I used tape to fasten down the switch and let the motor arm press it. I confirmed that the motor stopped by itself with my hands off the switch. My following STATUS output showed position=0 homed=1 mode=IDLE at 15:55:41.112. I preserved [the report and follow-up](evidence/taped-switch-contact-20261110.log). The excerpt does not include the HOME request or Homing Complete message. I have not yet recorded whether contact still lifted the arm or shifted the switch body.

### First working result and next checks

My physical report confirms the first hands-free arm-triggered stop in this taped arrangement, and the supplied STATUS confirms the final zero/homed/idle state. I am recording that as a functional first-iteration result. The full HOME admission/completion transcript was not captured in the supplied excerpt.

Next I will preserve a complete move-away and HOME sequence, observe repeated stopping, and record any arm lift, body movement, or shift in the stopping angle. Positioning accuracy and measured repeatability remain to establish.

### What I will improve afterward

With the first reported hands-free stop working, I can improve the support height, body fastening, arm retention, and roller approach. I will choose the changes from what I observe during testing, then repeat the checks. Later I can measure return-angle variation and homing repeatability with a fixed assembly.

### How I will document each change

For each adjustment, I will keep a photo, the materials and fastening method I actually used, the reason I tried it, the test output, and what I observed physically. I will record what helped, what still moved or caught, and what I changed next. This will show the practical problems and DIY work that got the build to a functional state without presenting proposed changes as completed work.

## Related records

- [Mechanical parts and assembly plan](../README.md).
- [Progress log](PROGRESS.md) and [test log](TESTING.md).
- [Manual HOME and physical-stop evidence](evidence/manual-home-and-direction-20261110.log).
- [One post-HOME visual return](evidence/manual-home-return-20261110.log).
- [Height and contact-lift report](evidence/mechanical-iteration-1-lift-20261110.log).
