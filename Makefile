CC= g++
RCC= windres

CFLAGS= -Wall -g -std=c++11 
LINKFLAGS= -mwindows -mconsole
RCCFLAGS= -O coff

INCLUDES=  -I"extlib/SFML/include"
LIBCLUDES= -L"extlib/SFML/lib" -L"extlib/SFML/extlibs/libs-mingw/x86"
DEFINES= -DSFML_STATIC

LINKLIBS= -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -ljpeg -lopengl32 -lglu32 -lwinmm 

SRCFILES= $(wildcard src/*.cpp src/*/*.cpp) #our sources and resources
RESFILES= $(wildcard res/*.rc)

OBJFILES= $(addprefix obj/,$(notdir $(SRCFILES:.cpp=.o) $(RESFILES:.rc=.res)))

APPNAME= 1337Jump

SETUP_SH:= $(shell mkdir -p obj) #create obj/ directory

#=============================================#

$(APPNAME): $(OBJFILES)
	$(CC) $(LINKFLAGS) $(LIBCLUDES) -o $@ $^ $(LINKLIBS)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $(DEFINES) $(INCLUDES) $< -o $@

obj/%.o: src/*/%.cpp
	$(CC) $(CFLAGS) -c $(DEFINES) $(INCLUDES) $< -o $@

obj/%.res: res/%.rc
	$(RCC) $< $(RCCFLAGS) -o $@
	
#=============================================#
.PHONY: clean_all clean

clean_all: clean
	rm -f $(APPNAME) *.exe 

clean:
	rm -f */*.o */*.res */*/*.o