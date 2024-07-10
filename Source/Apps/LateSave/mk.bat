@echo off
CALL ..\..\PATHSET.BAT
@echo\
@echo --- MAKE START ---
kmmake -f MAKEFILE PMODEL=1 %1 %2 %3
if exist make.i del make.i
@echo --- MAKE START (ForDebug)---
kmmake -f MAKEFILE PMODEL=1 DEBUG=1  %1 %2 %3 > err
if exist make.i del make.i
