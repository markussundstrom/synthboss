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
        static std::string synthSelectionWidget(std::map<std::string, std::array<std::string, 2>>& choices);
        void buildSynthGui(Synth synth);

    private:
        void addParameterWidget(const std::shared_ptr<Parameter>& parameter,
                QFormLayout* layout);
        QWidget* createParameterWidget(const std::shared_ptr<Parameter>& parameter);
        //void rejectSynthSelection();
        //void acceptSynthSelection();
        QTabWidget* tabWidget;
        static int selectedIndex;

        //signals:
        //static void synthSelected(int index);
};
