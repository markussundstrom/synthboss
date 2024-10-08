#include "sbgui.hpp"

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
            QFormLayout* sectionLayout = new QFormLayout();
            //sectionLayout->setSizeConstraint(QLayout::SetFixedSize);
            const std::vector<std::shared_ptr<Parameter>>& parameters = 
                section->getParameters();
            for (const auto& parameter : parameters) {
                addParameterWidget(parameter, sectionLayout);
            }
            sectionBox->setLayout(sectionLayout);
            partLayout->addWidget(sectionBox);
        }
        partLayout->addStretch();
        partPage->setLayout(partLayout);
        tabWidget->addTab(partPage, QString::fromStdString(part->getName()));
    }
} 


void SbGui::syncFromBackend() {
    for (auto const& p  : this->m_parameterMap) {
        if (auto rangeP = std::dynamic_pointer_cast<RangeParameter>(p.first)) {
            static_cast<QSlider*>(p.second)->setValue(rangeP->value());
        } else if (auto toggleP = std::dynamic_pointer_cast<ToggleParameter>(p.first)) {
            static_cast<QCheckBox*>(p.second)->setChecked(toggleP->valueBool());
        } else if (auto selectP = std::dynamic_pointer_cast<SelectParameter>(p.first)) {
            int index  = static_cast<QComboBox*>(p.second)->findData(p.first->value());
            static_cast<QComboBox*>(p.second)->setCurrentIndex(index);
        } else {
            std::cerr << "Unable to determine parameter class" << std::endl;
        }
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
            int choiceValue = c.second;
            combobox->addItem(QString::fromStdString(choiceName), QVariant(choiceValue));
        }
        QObject::connect(combobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [selectP, combobox]() { 
                    QVariant varValue = combobox->currentData();
                    if (varValue.isValid()) {
                        uint8_t actualValue = varValue.toInt();
                        selectP->setValue(actualValue);
                    }
        });                
        widget = combobox;
    } else {
        std::cerr << "Unable to determine parameter class" << std::endl;
        return;
    }
    m_parameterMap[parameter] = widget;
    layout->addRow(QString::fromStdString(parameter->name()), widget);
    return;
}


std::string SbGui::synthSelectionWidget(std::map<std::string, std::array<std::string, 2>>& choices) {
    std::string selected = "";
    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QTableWidget* tableWidget = new QTableWidget(choices.size(), 3, dialog);
    layout->addWidget(tableWidget);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
            QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, [=, &selected]() mutable {
            if (tableWidget->selectedItems().isEmpty()) {
                selected= "";
            } else {
                selected = tableWidget->item(tableWidget->currentRow(), 0)->text().toStdString();
            }
            dialog->accept();
    });
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, [=, &selected]() mutable {
            selected = "";
            dialog->reject();
    });

    int index = 0;
    for (auto const& c : choices) {
        tableWidget->setItem(index, 0, new QTableWidgetItem(QString::fromStdString(c.first)));
        tableWidget->setItem(index, 1, new QTableWidgetItem(QString::fromStdString(c.second[0])));
        tableWidget->setItem(index, 2, new QTableWidgetItem(QString::fromStdString(c.second[1])));
        index++;
    }
    dialog->exec();
    delete dialog;
    return selected;
}
