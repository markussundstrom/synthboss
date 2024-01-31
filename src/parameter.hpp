#pragma once
#include <iostream>
#include <string>
#include <map>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <map>
#include "observer.hpp"

using json = nlohmann::json;

class Parameter {
    private:
        std::string m_name;
        uint8_t m_pNumber;
        std::vector<std::shared_ptr<ParameterObserver>> m_observers;

    protected:
        void notifyObservers();
        uint8_t m_value;
        int m_coherence;

    public:
        Parameter();
        Parameter(const std::string& name, uint8_t pNumber, uint8_t value, 
                int coherence);
        virtual ~Parameter() = default;
        void addObserver(std::shared_ptr<ParameterObserver> observer);
        virtual void setValue(int value);
        const std::string& name() const;
        uint8_t parameterNumber() const;
        uint8_t value() const;
        int coherence() const;
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
    private:
        int m_on;
        int m_off;

    public:
        ToggleParameter(const json param);
        void setValue(int state) override;
};

class SelectParameter : public Parameter {
    private:
        std::map<std::string, uint8_t> m_choices;

    public:
        SelectParameter(const json param);
        std::map<std::string, uint8_t> choices() const;
};

