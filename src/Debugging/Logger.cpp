#include "Logger.h"
#include <iostream>

std::vector<LogEntry> Logger::messages;

void Logger::Log(const std::string &message)
{
    LogEntry newLog;

    newLog.type = LOG_INFO;
    newLog.message = message;

    std::cout << "LOG: "<< message << std::endl;

    messages.push_back(newLog);
}

void Logger::Error(const std::string &message)
{
    LogEntry newLog;

    newLog.type = LOG_ERROR;
    newLog.message = message;

    std::cerr << "ERROR: " << message << std::endl;
    messages.push_back(newLog);
}