#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <vector>

enum LogType
{
    LOG_INFO,
    LOG_ERROR
};

struct LogEntry
{
    LogType type;
    std::string message;
};

class Logger{

    public: 
        static void Log(const std::string& message);
        static void Error(const std::string& message);
    
    private:
        static std::vector<LogEntry> messages;
};
#endif