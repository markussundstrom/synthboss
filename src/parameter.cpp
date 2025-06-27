#include "parameter.hpp"

Parameter::Parameter() 
    : m_name(""), m_value(0) {}

Parameter::Parameter(const std::string& name, uint8_t pNumber, uint8_t value,
        int coherence, nlohmann::json encoding)
    : m_name{name}, m_pNumber{pNumber}, m_value{value},
    m_coherence{coherence}, m_encoding{encoding} {}

void Parameter::notifyObservers() {
    for (auto observer : m_observers) {
        observer->valueChanged(this);
    }
}


void Parameter::addObserver(std::shared_ptr<ParameterObserver> observer) {
    m_observers.push_back(observer);
}


void Parameter::setValue(uint8_t value) {
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

uint8_t Parameter::encodedValue() const {
    if (!m_encoding.is_object()) {
        return m_value;
    }

    std::string type = m_encoding["type"];

    if (type == "signed") {
        int valueBits = m_encoding["valuebits"];
        int signBit = m_encoding["signBit"];
        int offset = m_encoding.contains("offset") ? m_encoding["offset"].get<int>() : 0;
        int value = m_value + offset;
        bool isNeg = value < 0;
        uint8_t result = std::abs(value) & ((1 << valueBits) - 1);
        if (isNeg) {
            result |= (1 << signBit);
        }
        return result;
    } else if (type == "unsigned") {
        int valueBits = m_encoding["valuebits"];
        int offset = m_encoding.contains("offset") ? m_encoding["offset"].get<int>() : 0;
        uint8_t value = m_value + offset;
        return (value & ((1 << valueBits) - 1));
    } else {
        std::cerr << "Invalid parameter value encoding, returning base value" << std::endl;
        return m_value;
    }
}


RangeParameter::RangeParameter(const json param) 
    : Parameter {
        param["name"], param["parameternumber"], param["value"], 
        param.contains("coherence") ?  param["coherence"].get<int>() : 0,
        param.contains("encoding") ? param["encoding"] : nullptr
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
        param.contains("coherence") ?  param["coherence"].get<int>() : 0,
        param.contains("encoding") ? param["encoding"] : nullptr
    },
    m_on{param["on"]}, m_off{param["off"]} {}

void ToggleParameter::setValue(uint8_t state) {
    m_value = (state) ? m_on : m_off;
    notifyObservers();
}


bool ToggleParameter::valueBool() {
    return m_value == m_on;
}


SelectParameter::SelectParameter(const json param)
    : Parameter {
        param["name"], param["parameternumber"], param["value"], 
        param.contains("coherence") ?  param["coherence"].get<int>() : 0,
        param.contains("encoding") ? param["encoding"] : nullptr
    },
    m_choices{} {
        for (const auto& choice : param["choices"]) {
            m_choices[choice["name"]] = static_cast<uint8_t>(choice["optionvalue"]);
        }
    }


std::map<std::string, uint8_t> SelectParameter::choices() const {
    return m_choices;
}

