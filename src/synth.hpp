#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "parameter.hpp"
#include "sbmidi.hpp"

using json = nlohmann::json;

class Synth {
    public:
        Synth(std::string synthDef, SbMidi sbMidi);
        const std::vector<std::shared_ptr<Parameter>>& getParameters() const;

    private:
        SbMidi m_sbMidi;
        std::vector<std::shared_ptr<Parameter>> m_parameters;
        std::shared_ptr<Parameter> buildParameter(json param);
};
