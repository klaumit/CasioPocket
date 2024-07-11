#Makefile for PocketViewer2 Filemanager

include $(PVC)\COM_LNK\MakeSDK.1

### -------- Define Make Application -------- ###

#== TargetName ==
TARGET  = Filem

#== Program Name ==
PROGNAME = "Filemanager"

#== ProgramVersion(EX. 0100->Ver1.00) ==
VERSION = 0003

#== MenuIcon (Xsize=45dot,Ysize=28dot) ==
MICON = menuicon\icon.bmp

#== ListMenuIcon (Xsize=27dot,Ysize=20dot) ==
LICON = menuicon\Licon.bmp

#== CompileObjectFile ==
APLOBJS =       $(ODIR)\Filem.obj \
                $(ODIR)\fm.obj\
                $(ODIR)\mem.obj\
                $(ODIR)\gdefs.obj \
                $(ODIR)\stdprocs.obj \
                $(ODIR)\graphics.obj \
                $(ODIR)\debug.obj \
                $(ODIR)\xymodem.obj

#== IncludeHeaderFile ==
HEADFILE =      $(HDIR)\fm.h \
                $(HDIR)\mem.h \
                $(HDIR)\gdefs.h \
                $(HDIR)\stdprocs.h \
        	$(HDIR)\graphics.h \
        	$(HDIR)\debug.h \
                $(HDIR)\xymodem.h

### ----------------------------------------- ###

include $(PVC)\COM_LNK\MakeSDK.2

