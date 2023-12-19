#include "sbgui.hpp"

//SbGui::SbGui(Synth synth) : m_synth(synth) {
//QVBoxLayout* layout = new QVBoxLayout(this);
//const std::vector<std::shared_ptr<Parameter>>& parameters = synth.getParameters();
//for (const auto& parameter : parameters) {
//QWidget* parameterWidget = createParameterWidget(parameter);
//layout->addWidget(parameterWidget);
//}
//setLayout(layout);
//
//}


SbGui::SbGui() {
    tabWidget = new QTabWidget;
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
    setLayout(layout);
}


SbGui::~SbGui() {
}


void SbGui::buildSynthGui(Synth synth) {
    const std::vector<std::shared_ptr<Part>>& parts = synth.getParts();
    for (const auto& part : parts) {
        QWidget* partPage = new QWidget;
        QVBoxLayout* partLayout = new QVBoxLayout;
        const std::vector<std::shared_ptr<Section>>& sections = 
            part->getSections();
        for (const auto& section : sections) {
            QGroupBox* sectionBox = new QGroupBox(QString::fromStdString(section->getName()));
            //QVBoxLayout* sectionLayout = new QVBoxLayout;
            QFormLayout* sectionLayout = new QFormLayout();
            const std::vector<std::shared_ptr<Parameter>>& parameters = 
                section->getParameters();
            for (const auto& parameter : parameters) {
                //QWidget* parameterWidget = createParameterWidget(parameter);
                //sectionLayout->addWidget(parameterWidget);
                addParameterWidget(parameter, sectionLayout);
            }
            sectionBox->setLayout(sectionLayout);
            partLayout->addWidget(sectionBox);
        }
        partPage->setLayout(partLayout);
        tabWidget->addTab(partPage, QString::fromStdString(part->getName()));
    }
} 

void SbGui::addParameterWidget(const std::shared_ptr<Parameter>& parameter,
        QFormLayout* layout) {
    QWidget* widget = nullptr;

    if (auto rangeP = std::dynamic_pointer_cast<RangeParameter>(parameter)) {
        QSlider* slider = new QSlider(Qt::Orientation::Horizontal);
        slider->setMinimum(rangeP->min());
        slider->setMaximum(rangeP->max());
        QObject::connect(slider, &QSlider::valueChanged, 
                [rangeP](int newValue) { rangeP->setValue(newValue); });
        widget = slider;
    } else if (auto toggleP = std::dynamic_pointer_cast<ToggleParameter>(parameter)) {
        QCheckBox* checkbox = new QCheckBox("");
        QObject::connect(checkbox, &QCheckBox::stateChanged,
                [toggleP](int newValue) { toggleP->setValue(newValue); });
        widget = checkbox;
    } else if (auto selectP = std::dynamic_pointer_cast<SelectParameter>(parameter)) {
        QComboBox* combobox = new QComboBox();
        for (const auto& c : selectP->choices()) {
            const std::string& choiceName = c.first;
            uint8_t choiceValue = c.second;
            combobox->addItem(QString::fromStdString(choiceName), QVariant(choiceValue));
        }
        QObject::connect(combobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [selectP](int newValue) { selectP->setValue(newValue); });
        widget = combobox;
    } else {
        std::cerr << "Unable to determine parameter class" << std::endl;
        return;
    }
    layout->addRow(QString::fromStdString(parameter->name()), widget);
    return;
}






QWidget* SbGui::createParameterWidget(const std::shared_ptr<Parameter>& parameter) {
    QHBoxLayout* layout = new QHBoxLayout;
    QLabel* label = new QLabel(QString::fromStdString(parameter->name()));
    QWidget* widget = nullptr;

    if (auto rangeP = std::dynamic_pointer_cast<RangeParameter>(parameter)) {
        QSlider* slider = new QSlider(Qt::Orientation::Horizontal);
        slider->setMinimum(rangeP->min());
        slider->setMaximum(rangeP->max());
        QObject::connect(slider, &QSlider::valueChanged, 
                [rangeP](int newValue) { rangeP->setValue(newValue); });
        widget = slider;
    } else if (auto toggleP = std::dynamic_pointer_cast<ToggleParameter>(parameter)) {
        QCheckBox* checkbox = new QCheckBox("");
        QObject::connect(checkbox, &QCheckBox::stateChanged,
                [toggleP](int newValue) { toggleP->setValue(newValue); });
        widget = checkbox;
    } else if (auto selectP = std::dynamic_pointer_cast<SelectParameter>(parameter)) {
        QComboBox* combobox = new QComboBox();
        for (const auto& c : selectP->choices()) {
            const std::string& choiceName = c.first;
            uint8_t choiceValue = c.second;
            combobox->addItem(QString::fromStdString(choiceName), QVariant(choiceValue));
        }
        QObject::connect(combobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [selectP](int newValue) { selectP->setValue(newValue); });
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

