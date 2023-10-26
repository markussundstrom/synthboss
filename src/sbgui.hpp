#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include "synth.hpp"
#include "parameter.hpp"


class SbGui : public QWidget {
    Q_OBJECT

    public:
        SbGui();
        virtual ~SbGui() override;
        void buildSynthGui(Synth synth);

    private:
        QWidget* createParameterWidget(const std::shared_ptr<Parameter>& parameter);
        QTabWidget* tabWidget;
};
