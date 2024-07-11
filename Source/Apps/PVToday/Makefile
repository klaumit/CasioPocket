#Makefile for PVToday

include $(PVC)\COM_LNK\MakeSDK.1

### -------- Define Make Application -------- ###

#== TargetName ==
TARGET  = PVToday

#== Program Name ==
PROGNAME = "PVToday"

#== ProgramVersion(EX. 0100->Ver1.00) ==
VERSION = 0220

#== MenuIcon (Xsize=45dot,Ysize=28dot) ==
MICON = menuicon\icon.bmp

#== ListMenuIcon (Xsize=27dot,Ysize=20dot) ==
LICON = menuicon\Licon.bmp

#== CompileObjectFile ==
APLOBJS = $(ODIR)\PVToday.obj \
	$(ODIR)\PVToDGraData.obj \
	$(ODIR)\PVToDFlash.obj \
	$(ODIR)\shared.obj 

#== IncludeHeaderFile ==
HEADFILE = $(HDIR)\PVToday.h \
	$(HDIR)\PVToDGraData.h \
	$(HDIR)\PVToDFlash.h \
	$(HDIR)\shared.h

### ----------------------------------------- ###

include $(PVC)\COM_LNK\MakeSDK.2

