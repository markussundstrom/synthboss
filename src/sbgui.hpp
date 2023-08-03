#pragma once
#include <QMainWindow>
#include <QWidget>
#include "synth.hpp"
#include "parameter.hpp"

class Parameter;

class SbGui : public QWidget {
    Q_OBJECT

    public:
        SbGui(Synth synth);
        virtual ~SbGui() override;

    private:
        QWidget* createParameterWidget(const std::shared_ptr<Parameter>& parameter);
        Synth m_synth;
};

