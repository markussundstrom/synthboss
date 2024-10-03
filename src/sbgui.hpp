#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QTableWidget>
#include "synth.hpp"
#include "parameter.hpp"


class SbGui : public QWidget {
    Q_OBJECT

    public:
        SbGui();
        virtual ~SbGui() override;
        static std::string synthSelectionWidget(std::map<std::string, 
                std::array<std::string, 2>>& choices);
        void buildSynthGui(Synth synth);
        void syncFromBackend();

    private:
        void addParameterWidget(const std::shared_ptr<Parameter>& parameter,
                QFormLayout* layout);
        QTabWidget* tabWidget;
        std::map<std::shared_ptr<Parameter>, QWidget*> m_parameterMap;
        static int selectedIndex;
};
