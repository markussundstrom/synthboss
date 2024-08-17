#include "synth.hpp"

Synth::Synth(SbMidi sbMidi)
    : m_sbMidi{sbMidi} {}

Synth Synth::buildSynth(std::string synthDef, SbMidi sbMidi) {
    Synth synth(sbMidi);
    std::shared_ptr<Synth> synthPtr = std::make_shared<Synth>(synth);
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    for (const auto& jPart : synthData["parts"]) {
        std::shared_ptr<Part> part = std::make_shared<Part>(jPart["name"],
                jPart["channel"].template get<uint8_t>(),
                jPart["channeloffset"], 
                jPart["messageformat"]);
        synth.m_parts.push_back(part);
        part->addObserver(synthPtr);
        for (const auto& jSection : jPart["sections"]) {
            std::shared_ptr<Section> section = 
                std::make_shared<Section>(jSection["name"]);
            part->addSection(section);
            for (const auto& jParam : jSection["parameters"]) {
                std::shared_ptr<Parameter> param = synth.buildParameter(jParam);
                param->addObserver(part);
                section->addParameter(param);
            }
        }
    }
    return synth;
}


std::string Synth::getFullName(std::string synthDef) {
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    if (synthData.contains("manufacturer") && synthData.contains("model")) {
        return to_string(synthData["manufacturer"]) + " " + 
            to_string(synthData["model"]);
    } else {
        return "";
    }
}


std::string Synth::getShortName(std::string synthDef) {
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    if (synthData.contains("shortname")) {
        return synthData["shortname"];
    } else {
        return "";
    }
}


const std::vector<std::shared_ptr<Part>>& Synth::getParts() const {
    return this->m_parts;
}

std::shared_ptr<Parameter> Synth::buildParameter(json param) {
    std::shared_ptr<Parameter> paramPointer = nullptr;
    if (param["class"] == "range") {
        paramPointer = std::make_shared<RangeParameter>(param);
    } else if (param["class"] == "toggle") {
        paramPointer = std::make_shared<ToggleParameter>(param);
    } else if (param["class"] == "select") {
        paramPointer = std::make_shared<SelectParameter>(param);
    } else {
        std::cerr << "Error parameter class: " << param["class"] << std::endl;
    }
    return paramPointer;
}


void Synth::messageCreated(std::vector<char> message) {
    m_sbMidi.TransmitMessage(message);
}

Part::Part(std::string name, uint8_t channel, int channelOffset, 
        std::string messageFormat)
    : m_name{name}, m_channel{channel}, m_channelOffset{channelOffset}, 
      m_messageFormat{messageFormat} {}


void Part::notifyObservers(std::vector<char>  message) {
    for (auto observer : m_observers) {
        observer->messageCreated(message);
    }
}


void Part::addObserver(std::shared_ptr<PartObserver> observer) {
    m_observers.push_back(observer);
}


void Part::valueChanged(Parameter* parameter) {
    std::vector<char> message;
    int start, end = -1;
    do {
        start = end + 1;
        end = m_messageFormat.find(" ", start);
        std::string current = m_messageFormat.substr(start, end - start);
        unsigned char byte;
        if (sscanf(current.c_str(), "%hhx", &byte)) {
            ;
        } else if (current == "{channel}") {
            byte = m_channel + m_channelOffset;
        } else if (current == "{parameter}") {
            byte = parameter->parameterNumber();
        } else if (current == "{value}") {
            if (parameter->coherence()) {
                byte = 0;
                for (const auto& s : m_sections) {
                    for (const auto& p : s->getParameters()) {
                        if (parameter->coherence() == p->coherence()) {
                            byte += p->value();
                        }
                    }
                }
            } else {
                byte = parameter->value();
            }
        } else {
            std::cerr << "Error when constructing MIDI message" << std::endl;
            continue;
        }
        message.push_back(byte);
    } while (end != -1);
    notifyObservers(message);
}


void Part::addSection(std::shared_ptr<Section> section) {
    m_sections.push_back(section);
}


std::string Part::getName() const {
    return m_name;
}


const std::vector<std::shared_ptr<Section>>& Part::getSections() const {
    return this->m_sections;
}


Section::Section(std::string name)
    : m_name{name} {}


void Section::addParameter(std::shared_ptr<Parameter> parameter) {
    m_parameters.push_back(parameter);
}


std::string Section::getName() const {
    return m_name;
}


const std::vector<std::shared_ptr<Parameter>>& Section::getParameters() const {
    return this->m_parameters;
}
