# MinGW makefile

include ../src.mk
include ../../../../make/settings/mingw/compiler_cpp.mk

OUT = dev_pirex.dll
OUT_SUB_FOLDER = cvsplugins-extra\dev_pirex

LDFLAGS += -shared

include ../../../../make/settings/mingw/build_app.mk

post_build: $(OUT)
	xcopy /Y "..\..\*.txt" $(OUT_FOLDER)
