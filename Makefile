PF = -w


UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LIBS = -lGL -lglut -lz -lX11 -ldl -lXext
LIBDIR = -L/usr/local/lib -L/usr/lib -L/usr/X11/lib -L/p/lib -L/usr/lib/x86_64-linux-gnu
INCLUDE = -I/usr/local/include -I/usr/include -I/usr/X11/include -I/p/include -I/usr/include/X11
endif
ifeq ($(UNAME), Darwin)
LIBS = -framework OpenGL -framework GLUT
LIBDIR = -L/usr/lib -L/usr/X11/lib
INCLUDE = -I/usr/include
endif

CFLAGS = -g $(PF)

CC = g++

.cpp.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

PLATFORM.O = main.o ImageManager.o Bitmap.o GameEngine.o MainMenu.o LevelLoader.o Simulator.o
TEST.O = test.o
ALL.O = $(PLATFORM.O) $(TEST.O)

platform: $(PLATFORM.O)
	$(CC) $(CFLAGS) -o $@ $(PLATFORM.O) $(INCLUDE) $(LIBDIR) $(LIBS)

test: test.o
	$(CC) $(CFLAGS) -o $@ test.o $(INCLUDE) $(LIBDIR) $(LIBS)

clean:
	rm -f *.o platform test

