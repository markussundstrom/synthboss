#include <QApplication>
#include <QPushButton>
#include <config.h>
#include <iostream>
#include "synth.hpp"

int main(int argc, char **argv)
{
    Synth* synth = new Synth();
    std::cout << synth->printSynth();
    QApplication app (argc, argv);

    QPushButton button ("hello world");
    button.show();
    return app.exec();
//    std::cout << "Hello\n";
//    return 0;
}

