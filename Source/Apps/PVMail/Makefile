#Makefile for PocketViewer Sample Program

include $(PVC)\COM_LNK\MakeSDK.1

### -------- Define Make Application -------- ###

#== TargetName ==
TARGET  = PVMail

#== Program Name ==
PROGNAME = "PVMail"

#== ProgramVersion(EX. 0100->Ver1.00) ==
VERSION = 0204

#== MenuIcon (Xsize=45dot,Ysize=28dot) ==
MICON = menuicon\icon.bmp

#== ListMenuIcon (Xsize=27dot,Ysize=20dot) ==
LICON = menuicon\Licon.bmp

#== CompileObjectFile ==
APLOBJS =       $(ODIR)\PVMail.obj     \
                $(ODIR)\Display.obj    \
                $(ODIR)\Edit.obj       \
                $(ODIR)\Flash.obj      \
                $(ODIR)\ListBox.obj    \
                $(ODIR)\Serial.obj     \
                $(ODIR)\Search.obj     
#                $(ODIR)\qsort.obj

#== IncludeHeaderFile ==
HEADFILE =      $(HDIR)\PVMail.h \
                $(HDIR)\Flash.h      \
                $(HDIR)\ListBox.h    \
                $(HDIR)\Common.h     \
                $(HDIR)\Serial.h     \
                $(HDIR)\Graphics.h   
 
### ----------------------------------------- ###

include $(PVC)\COM_LNK\MakeSDK.2

