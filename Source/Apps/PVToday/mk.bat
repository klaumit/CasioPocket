@echo off
CALL ..\..\..\Vendor\PATHSET.BAT
@echo\
@echo --- MAKE START ---
kmmake PMODEL=1 DNAME=LANG_ENG %KM_OPT1% %KM_OPT2%
if exist make.i del make.i
@echo --- MAKE START (ForDebug)---
kmmake PMODEL=1 DNAME=LANG_ENG DEBUG=1 > err
if exist make.i del make.i
