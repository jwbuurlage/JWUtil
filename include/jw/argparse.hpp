/*
File: include/jw/logging.hpp

Copyright (C) 2015 Jan-Willem Buurlage <janwillembuurlage@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.
*/

#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "logging.hpp"

namespace jw {

class ArgParse {
  public:
    ArgParse() {}

    bool parse(int argc, char* argv[]) {
        programName_ = std::string(argv[0]);

        std::string currentFlag;
        for (int i = 1; i < argc; ++i) {
            if (argv[i][0] == '-') {
                if (!currentFlag.empty() && args_[currentFlag].empty())
                    args_[currentFlag] = defaults_[currentFlag];

                currentFlag = std::string(argv[i]);
                if (currentFlag != "-h" &&
                    defaults_.find(currentFlag) == defaults_.end()) {
                    listHelp();
                    JWLogError << "Unrecognized option: " << currentFlag
                               << endLog;
                    return false;
                }
            } else {
                if (currentFlag.empty()) {
                    JWLogError
                        << "ArgParse: argument given before initial flag."
                        << endLog;
                    return false;
                }

                auto val = std::string(argv[i]);

                if (args_[currentFlag].empty()) {
                    args_[currentFlag] = val;
                } else {
                    args_[currentFlag] += " " + val;
                }
            }
        }

        if (!currentFlag.empty() && args_[currentFlag].empty()) {
            if (defaults_.find(currentFlag) != defaults_.end()) {
                args_[currentFlag] = defaults_.at(currentFlag);
            } else {
                args_[currentFlag] = "";
            }
        }

        if (wasPassed("-h")) {
            listHelp();
            return false;
        }

        // set defaults for non-passed values
        for (auto& option : defaults_) {
            if (args_.find(option.first) == args_.end()) {
                if (required_[option.first]) {
                    listHelp();
                    JWLogError << "Missing required argument: " << option.first
                               << endLog;
                    return false;
                }

                if (!option.second.empty()) {
                    args_[option.first] = option.second;
                }
            }
        }

        return true;
    }

    void listHelp() const {
        std::cout << "USAGE: " << std::endl;
        std::cout << "\t" << programName_ << " ";
        for (auto& option : defaults_) {
            if (required_.at(option.first))
                std::cout << option.first << " ";
            else
                std::cout << "[" << option.first << "] ";
        }

        std::cout << std::endl << std::endl << "OPTIONS: " << std::endl;

        for (auto& option : defaults_) {
            std::cout << "\t" << option.first << ": "
                      << descriptions_.at(option.first);
            if (!option.second.empty())
                std::cout << ". (" << option.second << ")";
            std::cout << std::endl;
        }
    }

    void addOption(std::string option, std::string description,
                   bool optionRequired = false) {
        defaults_[option] = "";
        descriptions_[option] = description;
        required_[option] = optionRequired;
    }

    template <typename T>
    void addOptionWithDefault(std::string option, std::string description,
                              T defaultValue) {
        std::stringstream ss;
        ss << defaultValue;
        defaults_[option] = ss.str();
        descriptions_[option] = description;
        required_[option] = false;
    }

    bool wasPassed(std::string key) const {
        if (args_.find(key) != args_.end()) {
            return true;
        }
        return false;
    }

    std::vector<std::string> asList(std::string key) const {
        std::vector<std::string> result;
        if (!wasPassed(key))
            return result;

        std::stringstream ss(args_.at(key));
        while (ss.good()) {
            std::string element;
            ss >> element;
            result.push_back(element);
        }

        return result;
    }

    std::string asString(std::string key) const {
        if (!wasPassed(key))
            return "";

        return args_.at(key);
    }

    template <typename T>
    T as(std::string key) const {
        if (!wasPassed(key))
            return 0;

        T result = 0;
        std::stringstream ss(args_.at(key));
        while (ss.good()) {
            ss >> result;
        }

        return result;
    }

  private:
    std::string programName_;
    std::map<std::string, std::string> args_;
    std::map<std::string, std::string> defaults_;
    std::map<std::string, std::string> descriptions_;
    std::map<std::string, bool> required_;
};

} // namespace jw
