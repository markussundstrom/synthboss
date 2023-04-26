#include "parameter.hpp"

Parameter::Parameter(const std::string& name, uint8_t value)
    : m_name{name}, m_value{value} {}

const std::string& Parameter::name() const {
    return m_name;
}

uint8_t Parameter::value() const {
    return m_value;
}

RangeParameter::RangeParameter(const std::string& name, uint8_t value, 
        int min, int max)
    : Parameter{name, value}, m_min{min}, m_max{max} {}

int RangeParameter::min() const {
    return m_min;
}

int RangeParameter::max() const {
    return m_max;
}

ToggleParameter::ToggleParameter(const std::string& name, uint8_t value)
    : Parameter{name, value} {}

SelectParameter::SelectParameter(const std::string& name, uint8_t value, 
        std::map<std::string, uint8_t> choices)
    : Parameter{name, value}, m_choices{choices} {}

