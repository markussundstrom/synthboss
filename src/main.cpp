#include <QApplication>
#include <QPushButton>
#include <config.h>
#include <iostream>
#include "synth.hpp"
#include "sbgui.hpp"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    Synth synth;
    SbGui sbGui(synth);
    sbGui.show();
    return app.exec();
}

