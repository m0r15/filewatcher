#ifndef FILESYSTEMEVENT_H
#define FILESYSTEMEVENT_H

#include <filesystem>

namespace FileWatcher {

class FileSystemEvent {
public:
    FileSystemEvent(int wd, long mask, const std::filesystem::path& path);

public:
    int wd;
    long mask;
    std::filesystem::path path;

}; // end of FileSystemEvent

}

#endif // FILESYSTEMEVENT_H