#include "synth.hpp"


Synth Synth::buildSynth(std::string synthDef, SbMidi sbMidi) {
    Synth synth(sbMidi);
    std::shared_ptr<Synth> synthPtr = std::make_shared<Synth>(synth);
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    for (const auto& jPart : synthData["parts"]) {
        std::shared_ptr<Part> part = std::make_shared<Part>(jPart["name"]);
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



    

Synth::Synth(SbMidi sbMidi) : m_sbMidi{sbMidi} { 
    /*std::cerr << "Start Synth ctor" << std::endl;
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    std::shared_ptr<PartObserver> observer = shared_from_this();
    std::cerr << "before loops" << std::endl;
    for (const auto jPart : synthData["parts"]) {
        std::shared_ptr<Part> part = std::make_shared<Part>(jPart["name"]);
        m_parts.push_back(part);
        //part->addObserver(observer);
        for (const auto jSection : jPart["sections"]) {
            std::shared_ptr<Section> section = 
                std::make_shared<Section>(jSection["name"]);
            part->addSection(section);
            for (const auto jParam : jSection["parameters"]) {
                std::shared_ptr<Parameter> param = buildParameter(jParam);
                param->addObserver(part);
                section->addParameter(param);
            }
        }
        
    }
          


    for (const auto section : synthData["parts"][0]["sections"]) {
        for (const auto param : section["parameters"]) {
            std::shared_ptr<Parameter> p = buildParameter(param);
            if (p != nullptr) {
                m_parameters.push_back(p);
            }
        }
    }
    */
    //std::cerr << "End Synth ctor" << std::endl;
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

void Synth::messageCreated(std::string message) {
    std::cout << message << std::endl;
}

Part::Part(std::string name)
    : m_name{name} {}

void Part::notifyObservers(std::string message) {
    for (auto observer : m_observers) {
        observer->messageCreated(message);
    }
}

void Part::addObserver(std::shared_ptr<PartObserver> observer) {
    m_observers.push_back(observer);
}

void Part::valueChanged(Parameter* parameter) {
    std::cout << m_name << " " << parameter->name() << " " << parameter->value() << std::endl;
    notifyObservers("message from part");
    //create midi message, notify synth
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
