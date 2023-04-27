#include "synth.hpp"
#include "parameter.hpp"

Synth::Synth() {
//    this->parameters = (Parameter*)calloc(2, sizeof (Parameter*));
    
    parameters.push_back(RangeParameter("range", 0, 0, 127));
    parameters.push_back(ToggleParameter("toggle", 0));
}

std::string Synth::printSynth() {
    return parameters[0].name() + parameters[1].name();
}

