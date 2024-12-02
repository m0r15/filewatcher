#include "FileSystemEvent.h"

namespace FileWatcher {
FileSystemEvent::FileSystemEvent(int wd, long mask, const std::filesystem::path& path)
    : wd(wd)
    , mask(mask)
    , path(path)
{}
}