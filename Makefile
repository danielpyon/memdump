CXX = g++
CFLAGS = -g -Wall -std=c++17

BUILD = './build/'

all: memdump

memlib.o: memlib.cc memlib.h
	$(CXX) $(CPPFLAGS) -c $<

main.o: main.cc
	$(CXX) $(CPPFLAGS) -c $<

memdump: main.o memlib.o
	$(CXX) $(CFLAGS) -o $(BUILD)$@ $^

# memdump: $(OBJS)
#  	$(CXX) $(CFLAGS) -o $@ $^ `pkg-config --cflags --libs gtkmm-3.0`

# .cc.o: $(HEADERS)
# 	$(CXX) $(CFLAGS) -c $< `pkg-config --cflags --libs gtkmm-3.0`

clean: FORCE
	/bin/rm -f *~ *.o $(BUILD)memdump

FORCE:
