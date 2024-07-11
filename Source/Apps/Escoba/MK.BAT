@echo off
SET filename=Compilando Escoba ...
echo %filename%
CALL ..\..\PATHSET.BAT
PATH=%PATH%;%Windir%\command

echo --- FASE DE COMPILACION ---
kmmake PMODEL=1
if exist make.i del make.i

echo\
echo --- REFERENCIAS DEBUGGER ---
kmmake PMODEL=1 DEBUG=1 > err
if exist make.i del make.i

Pause


