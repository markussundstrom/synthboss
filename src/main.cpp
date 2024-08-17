#include "config.h"
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <QApplication>
#include "synth.hpp"
#include "sbgui.hpp"
#include "sbmidi.hpp"

int main(int argc, char **argv)
{
    std::cout << "start" << std::endl;
    QApplication app (argc, argv);
    SbMidi sbMidi;
    std::string synthConfig;
    std::cout << "before map" << std::endl;
    std::map<std::string, std::array<std::string, 2>> synths;
    SbGui sbGui;
    struct dirent* cfgFile;
    std::cout << PKG_DATA_DIR << std::endl;

    std::cout << "before dirent" << std::endl;
    std::string devicePath = std::string(PKG_DATA_DIR) + "/devices";
    DIR* dir = opendir(devicePath.c_str());
    if (dir != NULL) {
        while ((cfgFile = readdir(dir)) != NULL) {
            if (std::string(cfgFile->d_name) == "." || 
                    std::string(cfgFile->d_name) == "..") {
                continue;
            }
            std::string fileWithPath = devicePath + "/" + cfgFile->d_name;
            std::cout << fileWithPath << std::endl;
            std::string fullName = Synth::getFullName(fileWithPath);
            std::string shortName = Synth::getShortName(fileWithPath);
            if (!fullName.empty() && !shortName.empty()) {
                synths[shortName] = {fullName, fileWithPath};
            }
        }
    }
    if (argc > 1) {
        if (access(argv[1], F_OK) != -1) {
            synthConfig = argv[1];
        } else if (synths.count(argv[1])) {
            auto search = synths.find(argv[1]);
            synthConfig = search->second[1];
        }
    }
    while (synthConfig.empty()) {
        //FIXME Infinite if user fails to select. Handle in some way
        std::string key = SbGui::synthSelectionWidget(synths);
        std::cout << "key " << key << std::endl;
        if (synths.find(key) != synths.end()) {
            synthConfig = synths[key][1];
            std::cout << synthConfig << std::endl;
        }
    }


    //FIXME decide on configuration to load
    //std::string synthConfig = "mks7.json";
    //synthConfig = "mks7.json";
    Synth synth = Synth::buildSynth(synthConfig, sbMidi);
    sbGui.buildSynthGui(synth);
    sbGui.show();
    return app.exec();
}

