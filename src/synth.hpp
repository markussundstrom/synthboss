#pragma once
#include <string>
#include <vector>
#include "parameter.hpp"

class Synth {
    private:
        std::vector<Parameter> parameters;

    public:
        Synth();
        std::string printSynth(); 
};
