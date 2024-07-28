@echo off
CALL ..\..\..\Vendor\PATHSET.BAT
@echo\

@echo --- MAKE MAIN ---
copy /y v_main\makefile
@echo --- MAKE START ---
kmmake PMODEL=1
if exist make.i del make.i
@echo --- MAKE START (ForDebug)---
kmmake PMODEL=1 DEBUG=1 > err
if exist make.i del make.i

@echo --- MAKE FULL ---
copy /y v_full\makefile
@echo --- MAKE START ---
kmmake PMODEL=1
if exist make.i del make.i
@echo --- MAKE START (ForDebug)---
kmmake PMODEL=1 DEBUG=1 > err
if exist make.i del make.i
