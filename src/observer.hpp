#pragma once
 
class Parameter;

class ParameterObserver {
    public:
        virtual void valueChanged(Parameter* parameter) = 0;
};

class PartObserver {
    public:
        virtual void messageCreated(std::vector<char> message) = 0;
};
