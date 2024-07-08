#Makefile for PocketViewer Sample Program

include $(PVC)\COM_LNK\MakeSDK.1

### -------- Define Make Application -------- ###

#== TargetName ==
TARGET  = FileSave

#== Program Name ==
PROGNAME = "FileSave"

#== ProgramVersion(EX. 0100->Ver1.00) ==
VERSION = 0100

#== MenuIcon (Xsize=45dot,Ysize=28dot) ==
MICON = menuicon\icon.bmp

#== ListMenuIcon (Xsize=27dot,Ysize=20dot) ==
LICON = menuicon\Licon.bmp

#== CompileObjectFile ==
APLOBJS =	$(ODIR)\FileSave.obj \
		$(ODIR)\Serial.obj \
		$(ODIR)\Flash.obj \
		$(ODIR)\ListBox.obj \
		$(ODIR)\Trm.obj

#== IncludeHeaderFile ==
HEADFILE = 	$(HDIR)\FileSave.h \
		$(HDIR)\Serial.h \
		$(HDIR)\Flash.h \
		$(HDIR)\ListBox.h \
		$(HDIR)\Common.h \
		$(HDIR)\Trm.h 

### ----------------------------------------- ###

include $(PVC)\COM_LNK\MakeSDK.2

