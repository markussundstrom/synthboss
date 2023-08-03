#pragma once
#include <string>
#include <vector>
#include <memory>
#include "parameter.hpp"

//class SbGui;

class Synth {
    public:
        Synth();
        const std::vector<std::shared_ptr<Parameter>>& getParameters() const;

    private:
        std::vector<std::shared_ptr<Parameter>> m_parameters;
};
