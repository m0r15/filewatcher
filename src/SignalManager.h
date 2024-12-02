#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

namespace FileWatcher {

class SignalManager final
{
public:
    static SignalManager &instance();

    static void init();

    [[nodiscard]] bool keepRunning() const;

private:
    static void sigintHandler(int sig);
    SignalManager() = default;

private:
    bool m_keepRunning = true;
}; // end of SignalManager

}

#endif // SIGNALMANAGER_H