#include "parameter.hpp"

Parameter::Parameter() 
    : m_name(""), m_value(0) {}

Parameter::Parameter(const std::string& name, uint8_t value)
    : m_name{name}, m_value{value} {}

const std::string& Parameter::name() const {
    return m_name;
}

uint8_t Parameter::value() const {
    return m_value;
}

RangeParameter::RangeParameter(const json param) 
    : Parameter{param["name"], param["value"]}, 
      m_min{param["min"]}, m_max{param["max"]} {}

int RangeParameter::min() const {
    return m_min;
}

int RangeParameter::max() const {
    return m_max;
}

ToggleParameter::ToggleParameter(const json param)
    : Parameter{param["name"], param["value"]} {}

SelectParameter::SelectParameter(const json param)
    : Parameter{param["name"], param["value"]}, m_choices{} {
        for (const auto& choice : param["choices"]) {
            m_choices[choice["name"]] = choice["value"];
        }
}

std::map<std::string, uint8_t> SelectParameter::choices() const {
    return m_choices;
}

