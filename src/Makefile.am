SUBDIRS = helpers windowimpl

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -I$(SFML_INCDIR)
AM_LDFLAGS = -L$(SFML_LIBDIR) 


# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
lib_LIBRARIES = libGrylloEngine.a

libGrylloEngine_a_LDADD = helpers/libhelpers.a windowimpl/libwindowimpl.a $(OTHER_LIBS) $(SFML_LIBS)

libGrylloEngine_a_SOURCES = event.cpp \
                            eventhandling.cpp \
                            physics.cpp \
                            threadstate.cpp \
                            widgets.cpp \
                            windowrunner.cpp \
                            loader.cpp

include_HEADERS = windowrunner.h \
                  widgets.h \
                  threadstate.h \
                  physics.h \
                  loader.h \
                  event.h \
                  eventhandling.h \
                  contexts.h 
