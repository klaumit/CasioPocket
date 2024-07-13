@echo off
CALL ..\..\PATHSET.BAT
@echo\
@echo --- MAKE START ---
kmmake PMODEL=1
if exist make.i del make.i
@echo --- MAKE START (ForDebug)---
kmmake PMODEL=1 DEBUG=1 > err
if exist make.i del make.i
pause
