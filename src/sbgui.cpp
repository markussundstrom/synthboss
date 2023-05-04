#include <QGridLayout>
#include "sbgui.hpp"
#include "parameter.hpp"

SbGui::SbGui() {
    QGridLayout* layout = new QGridLayout;
    QWidget* m_window = new QWidget();
    setCentralWidget(m_window);
    centralWidget()->setLayout(layout);
}

SbGui::addGuiParameter(std::unique_ptr<Parameter>& p) {
    m_guiParameters.push_back(new GuiParameter(p));
    m_window.layout().addWidget(m_guiParameters[i]);

GuiParameter::Guiparameter(std::unique_ptr<Parameter>& p) {
    QLabel* label = new QLabel;
    label.setText(p.name());

    if (auto rangeP = dynamic_cast<RangeParameter*>(p.get())) {
        QSlider* control = new QSlider();
        control.setMinimum(rangeP->min());
        control.setMaximum(rangeP->max());
    } else if (auto toggleP = dynamic_cast<ToggleParameter*>(p.get())) {
        QCheckbox* control = new QCheckbox("");
    } else if (auto selectP = dynamic_cast<SelectParameter*>(p.get())) {
        //
    } else {
        //
    }

    QHboxLayout* layout = new QHBoxLayout;
    this->setLayout(layout);
    layout.addWidget(label);
    layout.addWidget(control);
}


