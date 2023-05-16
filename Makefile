CXX = g++
CFLAGS = -g -Wall -std=c++17 `pkg-config --cflags --libs gtkmm-3.0`
BUILD = './build/'

all: memdump

memlib.o: memlib.cc memlib.h
	$(CXX) -g -Wall -std=c++17 -c $<

main.o: main.cc
	$(CXX) $(CFLAGS) -c $<

menu.o: menu.cc menu.h
	$(CXX) $(CFLAGS) -c $<

dump.o: dump.cc dump.h
	$(CXX) $(CFLAGS) -c $<

memdump: main.o memlib.o menu.o dump.o
	$(CXX) $(CFLAGS) -o $(BUILD)$@ $^

clean: FORCE
	/bin/rm -f *~ *.o $(BUILD)memdump

FORCE:
