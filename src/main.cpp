#include <iostream>
#include <fstream>
#include <string>

#include "SignalManager.h"
#include "Watcher.h"

int main(int argc, char** argv)
{
    // init listen C-c
    FileWatcher::SignalManager::init();

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <configname>" << std::endl;
        return 1;
    }

    FileWatcher::Watcher watcher;
    std::ifstream configFile(argv[1]);
    std::string line;
    while(FileWatcher::SignalManager::instance().keepRunning()
          && configFile)
    {
        std::getline(configFile, line);
        watcher.watch(line);
    }

    watcher.exec();

    return 0;
}