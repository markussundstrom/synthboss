#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include "sbgui.hpp"
#include "parameter.hpp"

SbGui::SbGui(Synth synth) : m_synth(synth) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    const std::vector<std::shared_ptr<Parameter>>& parameters = synth.getParameters();
    for (const auto& parameter : parameters) {
        QWidget* parameterWidget = createParameterWidget(parameter);
        layout->addWidget(parameterWidget);
    }
    setLayout(layout);
}

SbGui::~SbGui() {
}


QWidget* SbGui::createParameterWidget(const std::shared_ptr<Parameter>& parameter) {
    QHBoxLayout* layout = new QHBoxLayout;
    QLabel* label = new QLabel(QString::fromStdString(parameter->name()));
    QWidget* widget = nullptr;

    if (auto rangeP = std::dynamic_pointer_cast<RangeParameter>(parameter)) {
        QSlider* slider = new QSlider(Qt::Orientation::Horizontal);
        slider->setMinimum(rangeP->min());
        slider->setMaximum(rangeP->max());
        widget = slider;
    } else if (auto toggleP = std::dynamic_pointer_cast<ToggleParameter>(parameter)) {
        QCheckBox* checkbox = new QCheckBox("");
        widget = checkbox;
    } else if (auto selectP = std::dynamic_pointer_cast<SelectParameter>(parameter)) {
        QComboBox* combobox = new QComboBox();
        for (const auto& c : selectP->choices()) {
            const std::string& choiceName = c.first;
            uint8_t choiceValue = c.second;
            combobox->addItem(QString::fromStdString(choiceName), QVariant(choiceValue));
        }
        widget = combobox;
    } else {
        return nullptr;
    }
    layout->addWidget(label);
    layout->addWidget(widget);
    QWidget* parameterWidget = new QWidget();
    parameterWidget->setLayout(layout);
    return parameterWidget;
}

