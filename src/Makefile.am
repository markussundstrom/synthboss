AM_CXXFLAGS = @QT_CXXFLAGS@
QT_LIBS = @QT_LIBS@
AM_CPPFLAGS = -DPKG_DATA_DIR='"$(pkgdatadir)"'
bin_PROGRAMS = synthboss
sbgui.moc.cpp: sbgui.hpp
	$(MOC) $< -o $@
synthboss_SOURCES = main.cpp sbmidi.cpp parameter.cpp synth.cpp\
					sbgui.cpp sbgui.moc.cpp
synthboss_LDADD  = $(QT_LIBS) $(JACK_LIBS) $(JSON_LIBS)
synthboss_CXXFLAGS = $(QT_CXXFLAGS) $(JACK_CFLAGS) $(JSON_CLAGS) -std=c++11
