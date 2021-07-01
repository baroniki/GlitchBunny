/*
  ==============================================================================

    Logger.cpp
    Created: 30 Jun 2021 12:51:17am
    Author:  Alex

  ==============================================================================
*/

#include "Logger.h"
Logger::Logger() : log_filename{ "log.txt" } {
    output_file.open(log_filename);
}

Logger::Logger(std::string log_filename) : log_filename{ log_filename } {
    output_file.open(log_filename);
}

Logger::~Logger() {
    output_file << "closing" << std::endl;
    output_file.close();
}

void Logger::Log(const std::string& log_msg) {
    output_file << log_msg << std::endl;
}