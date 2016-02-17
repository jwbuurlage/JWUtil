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

#include "color_output.hpp"

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
        switch (t_) {
        case LogType::info:
            std::cout << jw::colors::start["cyan"] << "INFO: ";
            break;

        case LogType::warning:
            std::cout << jw::colors::start["blue"] << "WARNING: ";
            break;

        case LogType::error:
            std::cout << jw::colors::start["red"] << "ERROR: ";
            break;

        case LogType::debug:
            std::cout << jw::colors::start["darkgray"] << "DEBUG: ";
            break;

        case LogType::benchmark:
            std::cout << jw::colors::start["purple"] << "BENCHMARK: ";
            break;

        case LogType::result:
            std::cout << jw::colors::start["yellow"] << "RESULT: ";
            break;

        default:
            break;
        }

        std::cout << jw::colors::end;
        std::cout << ss.str() << std::endl;

        finalized_ = true;
    }
};

} // namespace jw
