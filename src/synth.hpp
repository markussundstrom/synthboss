#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "parameter.hpp"
#include "sbmidi.hpp"
#include "observer.hpp"

using json = nlohmann::json;


class Section {
    public:
        Section(std::string name);
        void addParameter(std::shared_ptr<Parameter> parameter);
        std::string getName() const;
        const std::vector<std::shared_ptr<Parameter>>& getParameters() const;

    private:
        std::vector<std::shared_ptr<Parameter>> m_parameters;
        std::string m_name;
};


class Part : public ParameterObserver {
    public:
        Part(std::string name, uint8_t channel, json messageFormat);
        void addObserver(std::shared_ptr<PartObserver> observer);
        void valueChanged(Parameter* parameter) override;
        void addSection(std::shared_ptr<Section> section);
        std::string getName() const;
        const std::vector<std::shared_ptr<Section>>& getSections() const;

    private:
        void notifyObservers(std::vector<char> message);
        uint8_t parseValue(Parameter* parameter, const json& messagePart);
        uint8_t resolveComplexByte(std::string op, uint8_t arg1, uint8_t arg2);
        std::string m_name;
        uint8_t m_channel;
        nlohmann::json m_messageFormat;
        std::vector<std::shared_ptr<Section>> m_sections;
        std::vector<std::shared_ptr<PartObserver>> m_observers;
};


class Synth : public PartObserver, public std::enable_shared_from_this<Synth> {
    public:
        static std::shared_ptr<Synth> buildSynth(const std::string& syntDef, const SbMidi& sbMidi);
        static std::string getFullName(std::string synthDef);
        static std::string getShortName(std::string synthDef);
        Synth(SbMidi sbMidi);
        void messageCreated(std::vector<char> message) override;
        const std::vector<std::shared_ptr<Part>>& getParts() const;

    private:
        SbMidi m_sbMidi;
        std::vector<std::shared_ptr<Part>> m_parts;
        std::shared_ptr<Parameter> buildParameter(json param);
};
