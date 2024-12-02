#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <cstring>
#include <sys/inotify.h>

#include "SignalManager.h"
#include "FileSystemEvent.h"
#include "Watcher.h"

namespace fs = std::filesystem;

namespace {
constexpr auto EVENT_SIZE = sizeof (inotify_event);
constexpr long SELECT_TIMEOUT_USEC = 100;
constexpr long SELECT_TIMEOUT_SEC  = 0;
const fs::path BACKUP_DIR_PATH = fs::current_path() / ".backup";
}

namespace FileWatcher {
Watcher::Watcher()
    : m_inotifyFd(-1)
{
    m_inotifyFd = inotify_init();
    if(m_inotifyFd < 0) {
        std::cerr << "Watcher: inotify_init failed. Error: "
            << std::strerror(errno)
            << std::endl;
    }
}

Watcher::~Watcher()
{
    if(m_inotifyFd >= 0
       && close(m_inotifyFd) < 0) {
        std::cerr << "Watcher: close inotify fd failed. Error: "
            << std::strerror(errno)
            << std::endl;
       }
}

void Watcher::watch(std::string_view path)
{
    if (backup(path)) {
        addWatch(path);
    }
}

void Watcher::exec()
{
    while(SignalManager::instance().keepRunning()
        /* TODO: watch for watching queue */) {
        if(eventCheck()) {
            auto length = readEventsIntoBuffer(m_eventBuffer);
            readEventsFromBuffer(m_eventBuffer.data(), length, m_events);
            handleEvents();
        }
        // TODO: check queue
    }
}

bool Watcher::eventCheck() const
{
    timeval tv = {SELECT_TIMEOUT_SEC, SELECT_TIMEOUT_USEC};
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(m_inotifyFd, &rfds);
    return select(FD_SETSIZE
                  , &rfds
                  , nullptr
                  , nullptr
                  , &tv) >= 0;
}

bool Watcher::backup(std::string_view path)
{
    fs::path p(path);
    if(!fs::exists(p) || fs::is_directory(p)) {
        std::cout << path << " is not exist or is a directory." << std::endl;
        return false;
    }

    try {
        std::cout << p << " Backup ... ";
        fs::copy_file(p, BACKUP_DIR_PATH/p);
        std::cout << "OK" << std::endl;
    }
    catch (fs::filesystem_error& e) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Could not copy file: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void Watcher::addWatch(std::string_view path)
{
    constexpr auto eventMask = IN_MODIFY
                               | IN_CLOSE_WRITE
                               | IN_MOVED_FROM
                               | IN_MOVED_TO
                               | IN_DELETE
                               | IN_DELETE_SELF;
    fs::path p(path);
    const auto wd = inotify_add_watch(m_inotifyFd
                                      , p.c_str()
                                      , eventMask);
    if(wd < 0) {
        std::cerr << "Watcher: Cannot add watch for "
            << path
            << "with event mask "
            << std::hex << eventMask << std::dec
            << ". Error: " << std::strerror(errno);
        return;
    }

    m_watchFiles.insert({wd, fs::path(path)});

    std::cout << "Add file for watch: " << path << std::endl;
}

ssize_t Watcher::readEventsIntoBuffer(std::vector<uint8_t>& buffer)
{
    ssize_t length = 0;
    length = 0;

    length = read(m_inotifyFd, buffer.data(), buffer.size());
    if (length < 0) {
        std::cout << "read events error: " << std::strerror(errno) << std::endl;
        length = 0;
    }

    return length;
}

void Watcher::readEventsFromBuffer(uint8_t* buffer, ssize_t length, std::queue<FileSystemEvent>& events)
{
    int i = 0;
    while (i < length) {
        auto* event = ((struct inotify_event*)&buffer[i]);

        auto path = m_watchFiles[event->wd];

        FileSystemEvent fsEvent(event->wd, event->mask, path);

        if (!fsEvent.path.empty()) {
            events.push(fsEvent);
        } else {
            // Event is not complete --> ignore
        }

        i += EVENT_SIZE + event->len;
    }
}

void Watcher::handleEvents()
{
    while(!m_events.empty()) {
        auto event = m_events.front();
        m_events.pop();

    }
}
}