#include "synth.hpp"

Synth::Synth(SbMidi sbMidi)
    : m_sbMidi{sbMidi} {}

std::shared_ptr<Synth> Synth::buildSynth(const std::string& synthDef, const SbMidi& sbMidi) {
    std::shared_ptr<Synth> synth = std::make_shared<Synth>(sbMidi);
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    for (const auto& jPart : synthData["parts"]) {
        json messageformat = jPart["messageformat"];
std::cout << "messageFormat at constructor: " << messageformat.dump(2) << std::endl;
std::cout << "type: " << jPart["messageformat"].type_name() << std::endl;
        std::shared_ptr<Part> part = std::make_shared<Part>(jPart["name"],
                jPart["channel"].template get<uint8_t>(),
                messageformat);
        synth->m_parts.push_back(part);
        part->addObserver(synth);
        for (const auto& jSection : jPart["sections"]) {
            std::shared_ptr<Section> section = 
                std::make_shared<Section>(jSection["name"]);
            part->addSection(section);
            for (const auto& jParam : jSection["parameters"]) {
                std::shared_ptr<Parameter> param = synth->buildParameter(jParam);
                param->addObserver(part);
                section->addParameter(param);
                param->setValue(param->value());
            }
        }
    }
    return synth;
}


std::string Synth::getFullName(std::string synthDef) {
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    if (synthData.contains("manufacturer") && synthData.contains("model")) {
        return to_string(synthData["manufacturer"]) + " " + 
            to_string(synthData["model"]);
    } else {
        return "";
    }
}


std::string Synth::getShortName(std::string synthDef) {
    std::ifstream f(synthDef);
    json synthData = json::parse(f);
    if (synthData.contains("shortname")) {
        return synthData["shortname"];
    } else {
        return "";
    }
}


const std::vector<std::shared_ptr<Part>>& Synth::getParts() const {
    return this->m_parts;
}

std::shared_ptr<Parameter> Synth::buildParameter(json param) {
    std::shared_ptr<Parameter> paramPointer = nullptr;
    if (param["class"] == "range") {
        paramPointer = std::make_shared<RangeParameter>(param);
    } else if (param["class"] == "toggle") {
        paramPointer = std::make_shared<ToggleParameter>(param);
    } else if (param["class"] == "select") {
        paramPointer = std::make_shared<SelectParameter>(param);
    } else {
        std::cerr << "Error parameter class: " << param["class"] << std::endl;
    }
    std::cout << paramPointer->parameterNumber() << std::endl;
    return paramPointer;
}


void Synth::messageCreated(std::vector<char> message) {
    m_sbMidi.TransmitMessage(message);
}

Part::Part(std::string name, uint8_t channel, json messageFormat)
    : m_name{name}, m_channel{channel}, 
      m_messageFormat{messageFormat} {std::cout << "Message format: " << m_messageFormat.dump(2) << std::endl;}


void Part::notifyObservers(std::vector<char>  message) {
    for (auto observer : m_observers) {
        observer->messageCreated(message);
    }
}


void Part::addObserver(std::shared_ptr<PartObserver> observer) {
    m_observers.push_back(observer);
}


void Part::valueChanged(Parameter* parameter) {
    std::vector<char> message;
    for(const auto& messagePart : m_messageFormat[0]) {
        uint8_t byte;
        if (messagePart.type() == json::value_t::object) {
            if (!messagePart.contains("op") || !messagePart.contains("arg1") || !messagePart.contains("arg2")) {
                std::cerr << "Invalid messagePart object: missing 'op', 'arg1', or 'arg2'\n";
                std::cerr << messagePart.dump(2) << std::endl;
                continue;
            }
            std::string op = messagePart["op"];
            uint8_t arg1 = parseValue(parameter, messagePart["arg1"]);
            uint8_t arg2 = parseValue(parameter, messagePart["arg2"]);
            byte = resolveComplexByte(op, arg1, arg2);
        } else {
            byte = parseValue(parameter, messagePart);
        }
       message.push_back((char)byte);
    }
    notifyObservers(message);
}


uint8_t Part::parseValue(Parameter* parameter, const json& messagePart ) {
    if (messagePart.is_number_integer()) {
        return (uint8_t)messagePart.get<int>();
    } else if (messagePart.is_string()) {
        std::string key = messagePart.get<std::string>();
        if (key == "value") {
            return parameter->value();
        } else if (key == "parameter") {
            return parameter->parameterNumber();
        } else if (key == "channel") {
            return m_channel;
        } else {
            throw std::runtime_error("Error reading message format: " +  key) ;
        }
    } else {
        throw std::runtime_error("Invalid message format type " + 
                messagePart.dump(2) + ": " + messagePart.type_name());
    }
}


uint8_t Part::resolveComplexByte(std::string op, uint8_t arg1, uint8_t arg2) {
    if (op == "add") {
        return arg1 + arg2;
    } else {
        throw std::runtime_error("Unknown operation in message format: " + op);
    }
}


void Part::addSection(std::shared_ptr<Section> section) {
    m_sections.push_back(section);
}


std::string Part::getName() const {
    return m_name;
}


const std::vector<std::shared_ptr<Section>>& Part::getSections() const {
    return this->m_sections;
}


Section::Section(std::string name)
    : m_name{name} {}


void Section::addParameter(std::shared_ptr<Parameter> parameter) {
    m_parameters.push_back(parameter);
}


std::string Section::getName() const {
    return m_name;
}


const std::vector<std::shared_ptr<Parameter>>& Section::getParameters() const {
    return this->m_parameters;
}
