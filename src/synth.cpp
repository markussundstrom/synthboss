#include "parameter.hpp"

class Synth {
    private Parameter* parameters;

    public Synth() {
        this->parameters = calloc(2, size_of(Parameter*));
        parameters[0] = new RangeParameter();
        parameters[1] = new ToggleParameter();
    }
}

