# VxWorks Emulation Repository for our Real-Time Embedded Systems Final Project

This repository contains a shiny new VxWorks emulation layer for testing
code on Linux.

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
