#pragma once
//#include "parameter.hpp"
 
class Parameter;

class ParameterObserver {
    public:
        virtual void valueChanged(Parameter* parameter) = 0;
};

class PartObserver {
    public:
        virtual void messageCreated(std::string message) = 0;
};
