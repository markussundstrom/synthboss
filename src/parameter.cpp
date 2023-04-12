class Parameter {
    string name;
    uint8_t value;
}

class Range : public Parameter {
    int min;
    int max;
}

class Toggle : public Parameter {
}

class Select : public Parameter {
    std::map<string, uint8_t>;
}

