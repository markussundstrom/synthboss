#pragma once
#include <QMainWindow>
#include <QWidget>
#include "synth.hpp"
#include "parameter.hpp"

class GuiParameter : public QWidget {
    public:
        GuiParameter(std::unique_ptr<Parameter>& p);
};

class SbGui : public QMainWindow {
    public:
        SbGui(Synth synth);

    private:
        QWidget* m_window;
        QVector<GuiParameter> m_guiparams;
};

