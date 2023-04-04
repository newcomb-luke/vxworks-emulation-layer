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
- [x] msgQLib
    - [x] msgQCreate()
    - [x] msgQDelete()
    - [x] msgQSend()
    - [x] msgQReceive()
    - [x] msgQNumMsgs()
- [x] wdLib
    - [x] wdCreate()
    - [x] wdDelete()
    - [x] wdStart()
    - [x] wdCancel()
- [x] sigLib
    - [x] sigemptyset()
    - [x] sigfillset()
    - [x] sigaddset()
    - [x] sigdelset()
    - [x] sigismember()
    - [x] signal()
    - [x] sigaction()
    - [x] sigprocmask()
    - [x] sigpending()
    - [x] sigsuspend()
    - [x] pause()
    - [x] sigtimedwait()
    - [x] sigwaitinfo()
    - [x] sigsetmask()
    - [x] sigblock()
    - [x] raise()
    - [x] kill()
- [x] tickLib
    - [x] tickGet()
