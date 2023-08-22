#pragma once
#include <iostream>
#include <string>
#include <map>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <map>

using json = nlohmann::json;

class Parameter {
    private:
        std::string m_name;
        uint8_t m_value;

    public:
        Parameter();
        Parameter(const std::string& name, uint8_t value);
        virtual ~Parameter() = default;
        const std::string& name() const;
        uint8_t value() const;
};

class RangeParameter : public Parameter {
    private:
        int m_min;
        int m_max;

    public:
        RangeParameter(const json param);
        int min() const;
        int max() const;
};

class ToggleParameter : public Parameter {
    public:
        ToggleParameter(const json param);
};

class SelectParameter : public Parameter {
    private:
        std::map<std::string, uint8_t> m_choices;

    public:
        SelectParameter(const json param);
        std::map<std::string, uint8_t> choices() const;
};

