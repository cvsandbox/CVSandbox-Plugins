# dev_phidgets plug-in source files

# search path for source files
VPATH = ../../

# source files
SRC = dev_phidgets.cpp \
    PhidgetAdvancedServoPlugin.cpp PhidgetAdvancedServoPluginDescriptor.cpp \
    PhidgetInterfaceKitPlugin.cpp PhidgetInterfaceKitPluginDescriptor.cpp

# additional include folders
INCLUDES = -I../../../../../../CVSandbox/sources/afx/afx_types \
	-I../../../../../../CVSandbox/sources/core/iplugin

# libraries to use
LIBS = -liplugin -lafx_types
