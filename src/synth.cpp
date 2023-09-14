#include "synth.hpp"

using json = nlohmann::json;

Synth::Synth(std::string synthDef, SbMidi sbMidi) : m_sbMidi(sbMidi) { 
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    for (const auto section : synthData["parts"][0]["sections"]) {
        for (const auto param : section["parameters"]) {
            std::shared_ptr<Parameter> p = buildParameter(param);
            if (p != nullptr) {
                m_parameters.push_back(p);
            }
        }
    }
}

const std::vector<std::shared_ptr<Parameter>>& Synth::getParameters() const {
    return this->m_parameters;
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


