#include "parameter.hpp"

Parameter::Parameter() 
    : m_name(""), m_value(0) {}

Parameter::Parameter(const std::string& name, uint8_t pNumber, uint8_t value,
        int coherence)
    : m_name{name}, m_pNumber{pNumber}, m_value{value},
    m_coherence {coherence} {}

void Parameter::notifyObservers() {
    for (auto observer : m_observers) {
        observer->valueChanged(this);
    }
}

void Parameter::addObserver(std::shared_ptr<ParameterObserver> observer) {
    m_observers.push_back(observer);
}

void Parameter::setValue(int value) {
    m_value = value;
    notifyObservers();
}

const std::string& Parameter::name() const {
    return m_name;
}

uint8_t Parameter::parameterNumber() const {
    return m_pNumber;
}

uint8_t Parameter::value() const {
    return m_value;
}

int Parameter::coherence() const {
    return m_coherence;
}

RangeParameter::RangeParameter(const json param) 
    : Parameter {
        param["name"], param["parameternumber"], param["value"], 
        param.contains("coherence") ?  param["coherence"].get<int>() : 0
    },
    m_min{param["min"]}, m_max{param["max"]} {}

int RangeParameter::min() const {
    return m_min;
}

int RangeParameter::max() const {
    return m_max;
}

ToggleParameter::ToggleParameter(const json param)
    : Parameter {
        param["name"], param["parameternumber"], param["value"],
        param.contains("coherence") ?  param["coherence"].get<int>() : 0
    },
    m_on{param["on"]}, m_off{param["off"]} {}

void ToggleParameter::setValue(int state) {
    m_value = (state) ? m_on : m_off;
    notifyObservers();
}

SelectParameter::SelectParameter(const json param)
    : Parameter {
        param["name"], param["parameternumber"], param["value"], 
        param.contains("coherence") ?  param["coherence"].get<int>() : 0
    },
    m_choices{} {
        for (const auto& choice : param["choices"]) {
            m_choices[choice["name"]] = choice["optionvalue"];
        }
    }

std::map<std::string, uint8_t> SelectParameter::choices() const {
    return m_choices;
}

