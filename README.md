# Testing Repository for our Real-Time Embedded Systems Final Project

This repository contains not only platform-agnostic C code that is being tested
for our project, but also sports a shiny VxWorks emulation layer.

## Final Project Roadmap

- [ ] Simulator
    - [ ] Calculations
    - [ ] Sensor Writing
        - [ ] Level Sensor Writing
        - [ ] Temperature Sensor Writing
        - [ ] Pressure Sensor Writing
- [ ] Controller
    - [ ] Sensor Reading
        - [ ] Level Sensor Reading
        - [ ] Temperature Sensor Reading
        - [ ] Pressure Sensor Reading

## VxWorks Emulation Layer Roadmap

- [x] sysLib
    - [x] sysClkRateGet()
    - [x] sysClkRateSet()
- [x] taskLib
    - [x] taskSpawn()
    - [x] taskDelete()
    - [x] taskDelay()
- [x] semLib
    - [x] semBCreate()
    - [x] semCCreate()
    - [x] semMCreate()
    - [x] semGive()
    - [x] semTake()
    - [x] semDelete()
- [ ] msgQLib
    - [ ] msgQCreate()
    - [ ] msgQDelete()
    - [ ] msgQSend()
    - [ ] msgQReceive()
    - [ ] msgQNumMsgs()
- [ ] wdLib
    - [ ] wdCreate()
    - [ ] wdDelete()
    - [ ] wdStart()
    - [ ] wdCancel()
- [ ] sigLib
    - [ ] #include <signal.h>
- [ ] tickLib
    - [ ] tickGet()
