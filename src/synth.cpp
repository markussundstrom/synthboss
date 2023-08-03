#include "synth.hpp"
#include "parameter.hpp"
#include "sbgui.hpp"

Synth::Synth() {
    m_parameters.push_back(std::make_shared<RangeParameter>("range", 0, 0, 127));
    m_parameters.push_back(std::make_shared<ToggleParameter>("toggle", 0));
}

const std::vector<std::shared_ptr<Parameter>>& Synth::getParameters() const {
    return this->m_parameters;
}
