AM_CXXFLAGS = @QT_CXXFLAGS@
QT_LIBS = @QT_LIBS@
bin_PROGRAMS = synthboss
sbgui.moc.cpp: sbgui.hpp
	$(MOC) $< -o $@
synthboss_SOURCES = main.cpp parameter.cpp synth.cpp sbgui.cpp sbgui.moc.cpp
synthboss_LDADD  = $(QT_LIBS)
synthboss_CXXFLAGS = $(QT_CXXFLAGS)
AM_CPPFLAGS = $(JSON_CFLAGS)
LDADD = $(JSON_LIBS)
AM_CXXFLAGS += -std=c++11
