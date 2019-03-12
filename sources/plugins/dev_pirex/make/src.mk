# dev_pirex plug-in source files

# search path for source files
VPATH = ../../

# source files
SRC = dev_pirex.cpp \
	PiRexBotPlugin.cpp PiRexBotPluginDescriptor.cpp

# additional include folders
INCLUDES = -I../../../../../../CVSandbox/sources/afx/afx_types \
	-I../../../../../../CVSandbox/sources/afx/afx_types+ \
	-I../../../../../../CVSandbox/sources/afx/afx_video+ \
	-I../../../../../../CVSandbox/sources/afx/afx_platform+ \
	-I../../../../../../CVSandbox/sources/afx/video/afx_video_mjpeg+ \
	-I../../../../../../CVSandbox/sources/core/iplugin

# libraries to use
LIBS =  -liplugin -lafx_video_mjpeg+ \
	-lafx_platform+ -lafx_types+ -lafx_types \
	-lcurldll -lws2_32
