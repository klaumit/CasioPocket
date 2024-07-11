#Makefile for PocketViewer2 Sample Program

include $(PVC)\COM_LNK\MakeSDK.1

### -------- Define Make Application -------- ###

#== TargetName ==
TARGET  = iscratch

#== Program Name ==
PROGNAME = "IScratch"

#== ProgramVersion(EX. 0100->Ver1.00) ==
VERSION = 0001

#== MenuIcon (Xsize=45dot,Ysize=28dot) ==
MICON = menuicon\icon.bmp

#== ListMenuIcon (Xsize=27dot,Ysize=20dot) ==
LICON = menuicon\Licon.bmp

#== CompileObjectFile ==
APLOBJS =	$(ODIR)\iscratch.obj \
		$(ODIR)\lever.obj \
		$(ODIR)\modehook.obj
#== IncludeHeaderFile ==

HEADFILE = 	$(HDIR)\modehook.h

### ----------------------------------------- ###

include $(PVC)\COM_LNK\MakeSDK.2
