#ifndef WATCHER_H
#define WATCHER_H

#include <string_view>
#include <vector>
#include <queue>
#include <map>
#include <filesystem>

namespace FileWatcher {

class FileSystemEvent;

class Watcher final
{
public:
    Watcher();
    ~Watcher();

    void watch(std::string_view path);
    void exec();

private:
    [[nodiscard]] bool eventCheck() const;
    [[nodiscard]] bool backup(std::string_view path);
    void addWatch(std::string_view path);
    [[nodiscard]] ssize_t readEventsIntoBuffer(std::vector<uint8_t> &buffer);
    void readEventsFromBuffer(uint8_t *buffer, ssize_t length, std::queue<FileSystemEvent> &events);
    void handleEvents();

private:
    int m_inotifyFd;
    std::queue<FileSystemEvent> m_events;
    std::vector<uint8_t> m_eventBuffer;
    std::map<int, std::filesystem::path> m_watchFiles;
}; // end of Watcher

}

#endif // WATCHER_H