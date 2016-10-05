CC= g++
CFLAGS= -Wall -g -std=c++11 
LINKFLAGS= -mwindows

INCLUDES=  -I"extlib/SFML/include"
LIBCLUDES= -L"extlib/SFML/lib" -L"extlib/SFML/extlibs/libs-mingw/x86"
DEFINES= -DSFML_STATIC

LINKLIBS= -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -ljpeg -lopengl32 -lglu32 -lwinmm 

SRCFILES= src/main.o src/world.o

APPNAME= 1337Jump

all: $(SRCFILES)
	$(CC) $(LINKFLAGS) $(LIBCLUDES) -o $(APPNAME) $(SRCFILES) $(LINKLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $(DEFINES) $(INCLUDES) $< -o $@
	
clean:
	rm -f $(APPNAME) src/*.o