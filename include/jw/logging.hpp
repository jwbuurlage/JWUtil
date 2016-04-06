/*
File: include/jw/logging.hpp

Copyright (C) 2015 Jan-Willem Buurlage <janwillembuurlage@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.
*/

#pragma once

#include <sstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <set>
#include <map>

#define JWLogResult (jw::Logger() << jw::LogType::result)
#define JWLogBenchmark (jw::Logger() << jw::LogType::benchmark)
#define JWLogDebug (jw::Logger() << jw::LogType::debug)
#define JWLogError (jw::Logger() << jw::LogType::error)
#define JWLogInfo (jw::Logger() << jw::LogType::info)
#define JWLogWarning (jw::Logger() << jw::LogType::warning)

#define endLog jw::Logger::end()

#define JWLogVar(VAR)                                                          \
    (jw::Logger() << jw::LogType::debug << #VAR " = " << VAR << endLog)

#ifndef DEBUG
#define JWAssert(ASSERT)                                                       \
    if (!(ASSERT)) {                                                           \
        JWLogError << "assertion '" #ASSERT "' failed at " << __FILE__ << ":"  \
                   << __LINE__ << endLog;                                      \
        exit(-1);                                                              \
    }
#define JWAssertMsg(ASSERT, MESSAGE)                                           \
    if (!(ASSERT)) {                                                           \
        JWLogError << "assertion '" #ASSERT "' failed at " << __FILE__ << ":"  \
                   << __LINE__ << "\n"                                         \
                   << "\"" << MESSAGE << "\"" << endLog;                       \
        exit(-1);                                                              \
    }
#else
#define JWAssert(ASSERT)
#define JWAssertMsg(ASSERT, MESSAGE)
#endif

namespace jw {

enum LogType { info, warning, error, debug, benchmark, result };

class Logger {

  public:
    ~Logger() {
        if (!finalized_)
            finalize();
    }

    struct end {};

    Logger& operator<<(LogType t) {
        t_ = t;
        return *this;
    }

    template <typename T>
    Logger& operator<<(const T& rhs) {
        ss << rhs;
        return *this;
    }

    Logger& operator<<(const bool& rhs) {
        if (rhs)
            ss << "true";
        else
            ss << "false";
        return *this;
    }

    template <typename S>
    Logger& operator<<(const std::vector<S>& rhs) {
        auto sep = "";
        *this << "[";
        for (S value : rhs) {
            *this << sep << value;
            sep = ", ";
        }
        *this << "]";
        return *this;
    }

    template <typename S, size_t size>
    Logger& operator<<(const std::array<S, size>& rhs) {
        auto sep = "";
        *this << "[";
        for (S value : rhs) {
            *this << sep << value;
            sep = ", ";
        }
        *this << "]";
        return *this;
    }

    template <typename S>
    Logger& operator<<(const std::set<S>& rhs) {
        auto sep = "";
        *this << "[";
        for (S value : rhs) {
            *this << sep << value;
            sep = ", ";
        }
        *this << "]";
        return *this;
    }

    template <typename S, typename T>
    Logger& operator<<(const std::map<S, T>& rhs) {
        auto sep = "";
        *this << "{";
        for (auto& pair : rhs) {
            *this << sep << pair.first << " -> " << pair.second;
            sep = ", ";
        }
        *this << "}";
        return *this;
    }

    template <typename S, typename T>
    Logger& operator<<(std::pair<S, T> rhs) {
        ss << "[ " << rhs.first << ",\t" << rhs.second << " ]";
        return *this;
    }

    void operator<<(end) { finalize(); }

  private:
    std::stringstream ss;
    LogType t_ = LogType::info;
    bool finalized_ = false;

    void finalize() {
        // For reference, here is a table with terminal colors:
        // Black       0;30     Dark Gray     1;30
        // Blue        0;34     Light Blue    1;34
        // Green       0;32     Light Green   1;32
        // Cyan        0;36     Light Cyan    1;36
        // Red         0;31     Light Red     1;31
        // Purple      0;35     Light Purple  1;35
        // Brown       0;33     Yellow        1;33
        // Light Gray  0;37     White         1;37

        static std::map<std::string, std::string> colors_start = {
            {"darkgray", "\033[1;30m"}, {"blue", "\033[1;34m"},
            {"green", "\033[1;32m"},    {"cyan", "\033[1;36m"},
            {"red", "\033[1;31m"},      {"purple", "\033[1;35m"},
            {"yellow", "\033[1;33m"},   {"white", "\033[1;37m"},
        };

        static std::string colors_end = "\033[0m";

        switch (t_) {
        case LogType::info:
            std::cout << colors_start["cyan"] << "INFO: ";
            break;

        case LogType::warning:
            std::cout << colors_start["blue"] << "WARNING: ";
            break;

        case LogType::error:
            std::cout << colors_start["red"] << "ERROR: ";
            break;

        case LogType::debug:
            std::cout << colors_start["darkgray"] << "DEBUG: ";
            break;

        case LogType::benchmark:
            std::cout << colors_start["purple"] << "BENCHMARK: ";
            break;

        case LogType::result:
            std::cout << colors_start["yellow"] << "RESULT: ";
            break;

        default:
            break;
        }

        std::cout << colors_end;
        std::cout << ss.str() << std::endl;

        finalized_ = true;
    }
};

} // namespace jw
