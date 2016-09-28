CC= g++
CFLAGS= -std=c++11 

INCLUDES=  -I"extlib/SFML/include"
LIBCLUDES= -L"extlib/SFML/lib"
DEFINES= -DSFML_STATIC

LINKLIBS= -lGL -lsfml-graphics-s -lsfml-window-s -lsfml-system-s 

SRCFILES= src/main.o

APPNAME= 1337Jump

all: $(SRCFILES)
	$(CC) $(LIBCLUDES) -o $(APPNAME) $(SRCFILES) $(LINKLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $(DEFINES) $(INCLUDES) $< -o $@
	
clean:
	rm -f $(APPNAME) src/*.o