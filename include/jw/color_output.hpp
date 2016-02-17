/*
File: include/jw/color_output.hpp

Copyright (C) 2015 Jan-Willem Buurlage <janwillembuurlage@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.
*/

#pragma once

#include <string>
#include <map>

namespace jw {

namespace colors {

// For reference, here is a table with terminal colors:
// Black       0;30     Dark Gray     1;30
// Blue        0;34     Light Blue    1;34
// Green       0;32     Light Green   1;32
// Cyan        0;36     Light Cyan    1;36
// Red         0;31     Light Red     1;31
// Purple      0;35     Light Purple  1;35
// Brown       0;33     Yellow        1;33
// Light Gray  0;37     White         1;37

static std::map<std::string, std::string> start =
{
    {"darkgray", "\033[1;30m"},
    {"blue", "\033[1;34m"},
    {"green", "\033[1;32m"},
    {"cyan", "\033[1;36m"},
    {"red", "\033[1;31m"},
    {"purple", "\033[1;35m"},
    {"yellow", "\033[1;33m"},
    {"white", "\033[1;37m"},
};

static std::string end = "\033[0m";

} // namespace colors

} // namespace jw
