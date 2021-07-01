/*
  ==============================================================================

    Logger.h
    Created: 30 Jun 2021 12:51:17am
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <fstream>
#include <iostream>
#include <string>

class Logger {
public:
    Logger();
    Logger(std::string log_filename);

    ~Logger();

    void Log(const std::string& log_msg);
private:
    std::string log_filename;
    std::ofstream output_file;
};