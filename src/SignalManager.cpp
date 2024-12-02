#include <csignal>

#include "SignalManager.h"

namespace FileWatcher {

SignalManager& SignalManager::instance()
{
    static SignalManager instance_;
    return instance_;
}

void SignalManager::init()
{
    if(std::signal(SIGINT, sigintHandler) == SIG_IGN) {
        std::signal(SIGINT, SIG_IGN);
    }
}

bool SignalManager::keepRunning() const
{
    return m_keepRunning;
}

void SignalManager::sigintHandler(int sig)
{
    instance().m_keepRunning = false;
}

}