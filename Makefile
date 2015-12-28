FLTK_CONFIG = fltk-config
#FLTK_CONFIG = /home/vuolasah/ele/koulu/dippa/pcsofta/fltkinst/bin/fltk-config

CXX      = $(shell $(FLTK_CONFIG) --cxx)
DEBUG    = -g
CXXFLAGS = $(shell $(FLTK_CONFIG) --cxxflags ) -I. -Wall -Wextra
LDFLAGS  = $(shell $(FLTK_CONFIG) --ldflags --use-images) -lusb-1.0
#LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags )
LINK     = $(CXX)
TARGET = urburbtool
OBJS = urburbtool.o main.o logic.o hexbrowser.o images.o
SRCS = urburbtool.cxx main.cxx logic.cxx hexbrowser.cxx
.SUFFIXES: .o .cxx
%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $<
all: $(TARGET)


$(TARGET): $(OBJS)
	$(LINK) -o $(TARGET) $(OBJS) $(LDFLAGS)
urburbtool%cxx urburtool%h: userinterface.fl
	fluid -c -o urburbtool.cxx -h urburbtool.h userinterface.fl
images.cxx: sent.png received.png error.png timeout.png
	xxd -i sent.png >images.cxx
	xxd -i received.png >>images.cxx
	xxd -i error.png >>images.cxx
	xxd -i timeout.png >>images.cxx
images.o:images.cxx
urburbtool.o: urburbtool.cxx urburbtool.h
logic.o: logic.cxx logic.h
main.o:main.cxx
hexbrowser.o: hexbrowser.cxx
clean: $(TARGET) $(OBJS)
	rm -f *.o 2> /dev/null
	rm -f $(TARGET) 2> /dev/null
	rm -f urburbtool.cxx urburbtool.h 2> /dev/null
	rm -f *~  2> /dev/null
