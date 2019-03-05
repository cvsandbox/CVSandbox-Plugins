# Executable application build steps valid for MS Windows + MinGW environment

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifeq ($(OUT_SUB_FOLDER),)
    OUT_FOLDER = "$(SELF_DIR)..\..\..\..\..\CVSandbox\build\$(TARGET)\$(BUILD_TYPE)\bin\"
else
    OUT_FOLDER = "$(SELF_DIR)..\..\..\..\..\CVSandbox\build\$(TARGET)\$(BUILD_TYPE)\bin\$(OUT_SUB_FOLDER)\"
endif

LIBDIR += -L$(SELF_DIR)..\..\..\..\..\CVSandbox\build\$(TARGET)\$(BUILD_TYPE)\lib

CFLAGS += $(INCLUDES)
LDFLAGS += $(LIBDIR) $(LIBS)

all: build post_build

debug: build post_build
 
%.o: $(SRC_FILE_EXT)
	$(COMPILER) $(CFLAGS) -c $^ -o $@

$(OUT): $(OBJ)
	$(COMPILER) -o $@ $(OBJ) $(LDFLAGS)

build: $(OUT)
	xcopy /Y $(OUT) $(OUT_FOLDER)

post_build: $(OUT)

clean:
	del $(OBJ) $(OUT)
