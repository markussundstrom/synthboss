#include <QApplication>
#include <config.h>
#include <iostream>
#include "synth.hpp"
#include "sbgui.hpp"
#include "sbmidi.hpp"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    SbMidi sbMidi;
    //FIXME decide on configuration to load
    std::string synthConfig = "mks7.json";
    Synth synth(synthConfig, sbMidi);
    SbGui sbGui(synth);
    sbGui.show();
    return app.exec();
}

