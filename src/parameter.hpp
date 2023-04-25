#pragma once

class Parameter {
    private:
        string name;
        uint8_t value;

    public:
        string getName();
        uint8_t getValue();
};

class RangeParameter : public Parameter {
    private:
        int min;
        int max;
};

class ToggleParameter : public Parameter {
};

class SelectParameter : public Parameter {
    private:
        std::map<string, uint8_t>;
};

