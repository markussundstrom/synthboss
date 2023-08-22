#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "parameter.hpp"

class SbGui;

class Synth {
    public:
        Synth(std::string);
        const std::vector<std::shared_ptr<Parameter>>& getParameters() const;

    private:
        std::vector<std::shared_ptr<Parameter>> m_parameters;
        std::shared_ptr<Parameter> buildParameter(json param);
};
