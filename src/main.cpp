#include <QApplication>
#include <QPushButton>
#include <config.h>
#include <iostream>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);

    QPushButton button ("hello world");
    button.show();
    return app.exec();
//    std::cout << "Hello\n";
//    return 0;
}

