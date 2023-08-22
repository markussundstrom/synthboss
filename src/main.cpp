#include <QApplication>
#include <config.h>
#include <iostream>
#include "synth.hpp"
#include "sbgui.hpp"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    //FIXME decide on configuration to load
    std::string synthConfig = "mks7.json";
    Synth synth(synthConfig);
    SbGui sbGui(synth);
    sbGui.show();
    return app.exec();
}

