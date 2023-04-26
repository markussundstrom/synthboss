#pragma once
#include <string>
#include <cstdint>
#include <map>

class Parameter {
    private:
        std::string m_name;
        uint8_t m_value;

    public:
        Parameter(const std::string& name, uint8_t value);
        const std::string& name() const;
        uint8_t value() const;
};

class RangeParameter : public Parameter {
    private:
        int m_min;
        int m_max;

    public:
        RangeParameter(const std::string& name, uint8_t value, 
                int min, int max);
        int min() const;
        int max() const;
};

class ToggleParameter : public Parameter {
    public:
        ToggleParameter(const std::string& name, uint8_t value);
};

class SelectParameter : public Parameter {
    private:
        std::map<std::string, uint8_t> m_choices;

    public:
        SelectParameter(const std::string& name, uint8_t value, 
                std::map<std::string, uint8_t> choices);
};

